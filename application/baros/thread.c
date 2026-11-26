/*
 * baros - tiny RTOS
 * Designed and written by Tr6r
 */

#include "thread.h"

static void tcb_init(tcb_t *self);
static void tcb_add_thread(tcb_t *self, const thread_cfg_t *cfg, uint32_t *stack_top);
static void thread_exit(void);

void thread_pool_init(thread_pool_t *self) {
	self->thread_count = 0;
	tcb_init(self->threads);

	// behaviors
	self->add_thread = tcb_add_thread;
}

static void tcb_init(tcb_t *self) {
	for (uint32_t i = 0; i < MAX_THREADS; i++) {
		self[i].sp		  = NULL;
		self[i].state	  = THREAD_BLOCKED;
		self[i].priority  = THREAD_PRIO_0;
		self[i].wake_tick = 0;
		self[i].owner	  = NULL;
	}
}

static void thread_exit(void) {
	while (1) {}
}

static void tcb_add_thread(tcb_t *self, const thread_cfg_t *cfg, uint32_t *stack_top) {
	uint32_t *sp = (uint32_t *)((uint32_t)stack_top & ~0x7u);

	*(--sp) = 0x01000000u;				  // xPSR
	*(--sp) = (uint32_t)cfg->func;		  // PC
	*(--sp) = (uint32_t)thread_exit;	  // LR
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

	self->thread_id		= cfg->thread_id;
	self->sp			= sp;
	self->state			= THREAD_READY;
	self->priority		= cfg->priority;
	self->owner			= NULL;
}

void thread_idle_handler(void *argument) {
	(void)argument;
	for (;;) {
		__asm volatile("wfi");
	}
}
