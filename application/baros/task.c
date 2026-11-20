/*
 * baros - tiny RTOS
 * Designed and written by Tr6r
 */

#include "baros.h"

static void task_exit(void) {
	while (1) {}
}

void tcb_init(tcb_t *self, const task_cfg_t *cfg, uint32_t *stack_top) {
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

static void baros_init(baros_t *self, const task_cfg_t *cfg, uint32_t n) {
	if (n > MAX_TASKS) {
		n = MAX_TASKS;
	}

	for (uint32_t i = 0; i < n; i++) {
		tcb_init(&self->tasks[i], &cfg[i], &self->stacks[i][STACK_WORDS]);
	}

	self->task_count   = n;
	self->current_task = &self->tasks[0];
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

static void baros_start(baros_t *self) {
	baros_start_first(self->current_task->sp);
}

baros_t g_baros = {
	.init  = baros_init,
	.start = baros_start,
};
