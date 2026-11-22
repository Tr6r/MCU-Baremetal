/*
 * baros - tiny RTOS
 * Designed and written by Tr6r
 */

#include "task.h"

static void tcb_init(tcb_t *self);
static void tcb_add_task(tcb_t *self, const task_cfg_t *cfg, uint32_t *stack_top);
static void task_exit(void);

void task_pool_init(task_pool_t *self) {
	self->task_count = 0;
	tcb_init(self->tasks);

	// behaviors
	self->add_task = tcb_add_task;
}

static void tcb_init(tcb_t *self) {
	for (uint32_t i = 0; i < MAX_TASKS; i++) {
		self[i].sp		  = NULL;
		self[i].state	  = TASK_BLOCKED;
		self[i].priority  = TASK_PRIO_0;
		self[i].wake_tick = 0;
	}
}

static void task_exit(void) {
	while (1) {}
}

static void tcb_add_task(tcb_t *self, const task_cfg_t *cfg, uint32_t *stack_top) {
	uint32_t *sp = (uint32_t *)((uint32_t)stack_top & ~0x7u);

	*(--sp) = 0x01000000u;				  // xPSR
	*(--sp) = (uint32_t)cfg->func;		  // PC
	*(--sp) = (uint32_t)task_exit;		  // LR
	*(--sp) = 0u;						  // R12
	*(--sp) = 0u;						  // R3
	*(--sp) = 0u;						  // R2
	*(--sp) = 0u;						  // R1
	*(--sp) = (uint32_t)cfg->argument;	  // R0

	*(--sp) = 0u;	 // R11
	*(--sp) = 0u;	 // R10
	*(--sp) = 0u;	 // R9
	*(--sp) = 0u;	 // R8
	*(--sp) = 0u;	 // R7
	*(--sp) = 0u;	 // R6
	*(--sp) = 0u;	 // R5
	*(--sp) = 0u;	 // R4

	self->sp	   = sp;
	self->state	   = TASK_READY;
	self->priority = cfg->priority;
}

void task_idle_handler(void *argument) {
	(void)argument;
	for (;;) {
		__asm volatile("wfi");
	}
}
