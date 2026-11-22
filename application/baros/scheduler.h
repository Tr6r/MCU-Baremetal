/*
 * baros - tiny RTOS
 * Designed and written by Tr6r
 */

#ifndef __SCHEDULER__
#define __SCHEDULER__

#include "thread.h"

#define MAX_PRIO 8

typedef struct scheduler_t scheduler_t;
typedef struct ready_node_t ready_node_t;

struct ready_node_t {
	tcb_t *thread;
	ready_node_t *next;
	ready_node_t *prev;
};

typedef struct {
	ready_node_t nodes[MAX_THREADS];
	ready_node_t *cursor[MAX_PRIO];	   // cursor round-robin
	uint32_t ready_mask;
} ready_list_t;

struct scheduler_t {
	ready_list_t ready_list;
	uint32_t tick;
	ready_node_t *current;
	uint8_t started;

	ready_node_t *(*get_next_node)(scheduler_t *self);
};

// scheduler accessors
static inline tcb_t *scheduler_current(scheduler_t *self) {
	return self->current->thread;
}

static inline uint32_t scheduler_tick(const scheduler_t *self) {
	return self->tick;
}

void scheduler_init(scheduler_t *self);
void scheduler_add_ready_thread(scheduler_t *self, tcb_t *t);
void scheduler_pick_first(scheduler_t *self);

#endif	  //__SCHEDULER__
