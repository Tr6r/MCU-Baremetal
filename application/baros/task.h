/*
 * baros - tiny RTOS
 * Designed and written by Tr6r
 */

#ifndef __TASK__
#define __TASK__

#include <stdint.h>

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
typedef struct {
	uint32_t *sp;
	task_state_e state;
	task_prio_e priority;
} tcb_t;

void tcb_init(tcb_t *self, const task_cfg_t *cfg, uint32_t *stack_top);

#endif	  //__TASK__
