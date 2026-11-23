/*
 * baros - tiny RTOS
 * Designed and written by Tr6r
 */

#ifndef __THREAD__
#define __THREAD__

#include <stddef.h>
#include <stdint.h>

#define MAX_THREADS	4
#define STACK_WORDS 256

typedef void (*thread_func_t)(void *argument);

typedef enum {
	THREAD_READY = 0,
	THREAD_BLOCKED,
} thread_state_e;

typedef enum {
	THREAD_PRIO_0 = 0,
	THREAD_PRIO_1,
	THREAD_PRIO_2,
	THREAD_PRIO_3,
	THREAD_PRIO_4,
	THREAD_PRIO_5,
	THREAD_PRIO_6,
	THREAD_PRIO_7,
} thread_prio_e;

typedef struct {
	thread_func_t func;
	void *argument;
	thread_prio_e priority;
} thread_cfg_t;

// Task control block
typedef struct tcb_t tcb_t;

struct tcb_t {
	uint32_t *sp;
	uint32_t wake_tick;
	thread_state_e state;
	thread_prio_e priority;
};

// tcb accessors
static inline void tcb_set_wake(tcb_t *self, uint32_t tick) {
	self->wake_tick = tick;
}

static inline void tcb_set_state(tcb_t *self, thread_state_e state) {
	self->state = state;
}

static inline uint32_t tcb_wake(const tcb_t *self) {
	return self->wake_tick;
}

static inline thread_state_e tcb_state(const tcb_t *self) {
	return self->state;
}

static inline thread_prio_e tcb_priority(const tcb_t *self) {
	return self->priority;
}

typedef struct {
	tcb_t threads[MAX_THREADS];
	uint32_t stacks[MAX_THREADS][STACK_WORDS];	// 4 KB
	uint32_t thread_count;

	void (*add_thread)(tcb_t *self, const thread_cfg_t *cfg, uint32_t *stack_top);

} thread_pool_t;

void thread_pool_init(thread_pool_t *self);
void thread_idle_handler(void *argument);

#endif	  //__THREAD__
