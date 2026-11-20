/*
 * baros - tiny RTOS
 * Designed and written by Tr6r
 */

#ifndef __BAROS__
#define __BAROS__

#include "task.h"

#define MAX_TASKS	8
#define STACK_WORDS 128

typedef struct baros_t baros_t;

struct baros_t {
	tcb_t tasks[MAX_TASKS];
	uint32_t stacks[MAX_TASKS][STACK_WORDS];	// 4 KB
	tcb_t *current_task;
	uint32_t task_count;

	// Constructor
	void (*init)(baros_t *self, const task_cfg_t *cfg, uint32_t n);

	void (*start)(baros_t *self);
};

extern baros_t g_baros;

#endif	  //__BAROS__
