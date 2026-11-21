/*
 * baros - tiny RTOS
 * Designed and written by Tr6r
 */

#include <stddef.h>

#include "baros.h"

static void baros_load_task(baros_t *self, const task_cfg_t *cfg, uint32_t n);

static void baros_idle_task(void *argument) {
	(void)argument;
	for (;;) {
		__asm volatile("wfi");	  // CPU ngủ tới ngắt kế -> tiết kiệm điện
	}
}

static void baros_init(baros_t *self) {
	task_pool_init(&self->pool);
	scheduler_init(&self->scheduler);

	// idle task: priority thấp nhất, luôn READY (CPU luôn có chỗ chạy)
	task_cfg_t idle = {.func = baros_idle_task, .argument = NULL, .priority = TASK_PRIO_0};
	baros_load_task(self, &idle, 1);
}

__attribute__((naked)) static void baros_start_first(uint32_t *sp __attribute__((unused))) {
	__asm volatile("ldmia r0!, {r4-r11}  \n"
				   "msr   psp, r0        \n"
				   "movs  r0, #2         \n"
				   "msr   control, r0    \n"
				   "isb                  \n"
				   "pop   {r0-r3, r12}   \n"
				   "pop   {lr}           \n"
				   "pop   {r1}           \n"
				   "pop   {r2}           \n"
				   "cpsie i              \n"
				   "bx    r1             \n");
}

static void baros_load_task(baros_t *self, const task_cfg_t *cfg, uint32_t n) {
	task_pool_t *pool = &self->pool;
	scheduler_t *s	  = &self->scheduler;

	for (uint32_t i = 0; i < n; i++) {
		if (pool->task_count >= MAX_TASKS) {
			break;
		}

		uint32_t idx = pool->task_count;
		tcb_t *t	 = &pool->tasks[idx];

		pool->add_task(t, &cfg[i], &pool->stacks[idx][STACK_WORDS]);
		scheduler_add_ready_task(s, t);

		pool->task_count++;
	}
}

static void baros_start(baros_t *self) {
	__asm volatile("cpsid i" ::: "memory");
	scheduler_pick_first(&self->scheduler);
	self->scheduler.started = 1;
	baros_start_first(self->scheduler.current->task->sp);
}

baros_t g_baros = {
	.init	   = baros_init,
	.load_task = baros_load_task,
	.start	   = baros_start,
};
