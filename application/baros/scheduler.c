/*
 * baros - tiny RTOS
 * Designed and written by Tr6r
 */

#include "baros.h"
#include "scb.h"

static void ready_list_init(ready_list_t *self);
static ready_node_t *get_next_node(scheduler_t *self);
static void scheduler_remove_ready_task(scheduler_t *self, tcb_t *t);
static void scheduler_wake_expired(scheduler_t *self);

__attribute__((naked)) void PendSV_Handler(void) {
	__asm volatile("mrs   r0, psp          \n"
				   "stmdb r0!, {r4-r11}    \n"
				   "push  {lr}             \n"
				   "bl    scheduler_next_task  \n"
				   "pop   {lr}             \n"
				   "ldmia r0!, {r4-r11}    \n"
				   "msr   psp, r0          \n"
				   "bx    lr               \n");
}

void scheduler_init(scheduler_t *self) {
	self->tick	  = 0;
	self->started = 0;
	self->current = NULL;
	ready_list_init(&self->ready_list);

	// behaviors
	self->get_next_node = get_next_node;
}

static void ready_list_init(ready_list_t *self) {
	self->ready_mask = 0;
	for (uint32_t p = 0; p < MAX_PRIO; p++) {
		self->cursor[p] = NULL;
	}
	for (uint32_t i = 0; i < MAX_TASKS; i++) {
		self->nodes[i].task = NULL;
		self->nodes[i].next = NULL;
		self->nodes[i].prev = NULL;
	}
}

static ready_node_t *get_next_node(scheduler_t *self) {
	ready_list_t *rl = &self->ready_list;
	if (rl->ready_mask == 0u) {
		return self->current;
	}
	uint32_t p		= 31u - (uint32_t)__builtin_clz(rl->ready_mask);
	ready_node_t *n = rl->cursor[p];
	rl->cursor[p]	= n->next;
	return n;
}

void scheduler_add_ready_task(scheduler_t *self, tcb_t *t) {
	ready_list_t *rl = &self->ready_list;
	uint32_t i		 = (uint32_t)(t - g_baros.pool.tasks);
	ready_node_t *n	 = &rl->nodes[i];
	uint32_t p		 = (uint32_t)t->priority;

	n->task = t;

	if (rl->cursor[p] == NULL) {
		n->next = n->prev = n;
		rl->cursor[p]	  = n;
		rl->ready_mask |= (1u << p);
	}
	else {	  // chèn cuối vòng (trước cursor)
		ready_node_t *tail	= rl->cursor[p]->prev;
		n->prev				= tail;
		n->next				= rl->cursor[p];
		tail->next			= n;
		rl->cursor[p]->prev = n;
	}

	t->state = TASK_READY;
}

static void scheduler_remove_ready_task(scheduler_t *self, tcb_t *t) {
	ready_list_t *rl = &self->ready_list;
	uint32_t i		 = (uint32_t)(t - g_baros.pool.tasks);
	ready_node_t *n	 = &rl->nodes[i];
	uint32_t p		 = (uint32_t)t->priority;

	if (n->next == n) {
		rl->cursor[p] = NULL;
		rl->ready_mask &= ~(1u << p);
	}
	else {
		n->prev->next = n->next;
		n->next->prev = n->prev;
		if (rl->cursor[p] == n) {
			rl->cursor[p] = n->next;
		}
	}
	n->next = n->prev = NULL;
}

void scheduler_pick_first(scheduler_t *self) {
	self->current = self->get_next_node(self);
}

uint32_t *scheduler_next_task(uint32_t *old_sp) {
	scheduler_t *s		 = &g_baros.scheduler;
	s->current->task->sp = old_sp;

	// Remove from readly list if done
	if (s->current->task->state == TASK_BLOCKED) {
		scheduler_remove_ready_task(s, s->current->task);
	}

	s->current = s->get_next_node(s);
	return s->current->task->sp;
}

static void scheduler_wake_expired(scheduler_t *self) {
	task_pool_t *pool = &g_baros.pool;
	for (uint32_t i = 0; i < pool->task_count; i++) {
		tcb_t *t = &pool->tasks[i];
		if (t->state == TASK_BLOCKED && (int32_t)(self->tick - t->wake_tick) >= 0) {
			scheduler_add_ready_task(self, t);	  // BLOCKED -> READY
		}
	}
}

void SysTick_Handler(void) {
	scheduler_t *s = &g_baros.scheduler;
	s->tick++;

	if (!s->started) {
		return;
	}

	scheduler_wake_expired(s);
	SCB_PendSV_Set();
}
