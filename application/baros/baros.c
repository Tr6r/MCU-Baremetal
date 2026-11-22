/*
 * baros - tiny RTOS
 * Designed and written by Tr6r
 */

#include <stddef.h>

#include "baros.h"
#include "scb.h"

static void baros_load_thread(baros_t *self, const thread_cfg_t *cfg, uint32_t n);
static void baros_load_idle_thread(baros_t *self);

static void baros_load_idle_thread(baros_t *self) {
	thread_cfg_t idle = {.func = thread_idle_handler, .argument = NULL, .priority = THREAD_PRIO_0};
	baros_load_thread(self, &idle, 1);
}

static void baros_init(baros_t *self) {
	thread_pool_init(&self->pool);
	scheduler_init(&self->scheduler);

	baros_load_idle_thread(self);
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

static void baros_load_thread(baros_t *self, const thread_cfg_t *cfg, uint32_t n) {
	thread_pool_t *pool = &self->pool;
	scheduler_t *s	  = &self->scheduler;

	for (uint32_t i = 0; i < n; i++) {
		if (pool->thread_count >= MAX_THREADS) {
			break;
		}

		uint32_t idx = pool->thread_count;
		tcb_t *t	 = &pool->threads[idx];

		pool->add_thread(t, &cfg[i], &pool->stacks[idx][STACK_WORDS]);
		scheduler_add_ready_thread(s, t);

		pool->thread_count++;
	}
}

static void baros_start(baros_t *self) {
	__asm volatile("cpsid i" ::: "memory");
	scheduler_pick_first(&self->scheduler);
	self->scheduler.started = 1;
	baros_start_first(self->scheduler.current->thread->sp);
}

void baros_delay(uint32_t ms) {
	scheduler_t *s = &g_baros.scheduler;
	tcb_t *t	   = scheduler_current(s);

	baros_enter_critical();
	tcb_set_wake(t, scheduler_tick(s) + ms);
	tcb_set_state(t, THREAD_BLOCKED);
	SCB_PendSV_Set();
	baros_exit_critical();
}

void baros_delay_until(uint32_t target) {
	tcb_t *t = scheduler_current(&g_baros.scheduler);

	baros_enter_critical();
	tcb_set_wake(t, target);
	tcb_set_state(t, THREAD_BLOCKED);
	SCB_PendSV_Set();
	baros_exit_critical();
}

baros_t g_baros = {
	.init	   = baros_init,
	.load_thread = baros_load_thread,
	.start	   = baros_start,
};
