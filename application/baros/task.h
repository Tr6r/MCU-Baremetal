/*
 * baros - tiny RTOS
 * Designed and written by Tr6r
 */

#ifndef __TASK__
#define __TASK__

#include <stddef.h>
#include <stdint.h>

#define MAX_TASKS	8
#define STACK_WORDS 128

typedef void (*task_func_t)(void *argument);

typedef enum {
	TASK_READY = 0,
	TASK_BLOCKED,
} task_state_e;

typedef enum {
	TASK_PRIO_0 = 0,
	TASK_PRIO_1,
	TASK_PRIO_2,
	TASK_PRIO_3,
	TASK_PRIO_4,
	TASK_PRIO_5,
	TASK_PRIO_6,
	TASK_PRIO_7,
} task_prio_e;

typedef struct {
	task_func_t func;
	void *argument;
	task_prio_e priority;
} task_cfg_t;

// Task control block
typedef struct tcb_t tcb_t;

struct tcb_t {
	uint32_t *sp;
	uint32_t wake_tick;
	task_state_e state;
	task_prio_e priority;
};

// tcb accessors
static inline void tcb_set_wake(tcb_t *self, uint32_t tick) {
	self->wake_tick = tick;
}

static inline void tcb_set_state(tcb_t *self, task_state_e state) {
	self->state = state;
}

static inline uint32_t tcb_wake(const tcb_t *self) {
	return self->wake_tick;
}

static inline task_state_e tcb_state(const tcb_t *self) {
	return self->state;
}

static inline task_prio_e tcb_priority(const tcb_t *self) {
	return self->priority;
}

typedef struct {
	tcb_t tasks[MAX_TASKS];
	uint32_t stacks[MAX_TASKS][STACK_WORDS];	// 4 KB
	uint32_t task_count;

	void (*add_task)(tcb_t *self, const task_cfg_t *cfg, uint32_t *stack_top);

} task_pool_t;

void task_pool_init(task_pool_t *self);
void task_idle_handler(void *argument);

#endif	  //__TASK__
