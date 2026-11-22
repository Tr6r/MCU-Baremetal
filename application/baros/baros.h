/*
 * baros - tiny RTOS
 * Designed and written by Tr6r
 */

#ifndef __BAROS__
#define __BAROS__

#include "scheduler.h"

typedef struct baros_t baros_t;

struct baros_t {
	thread_pool_t pool;
	scheduler_t scheduler;

	// Constructor
	void (*init)(baros_t *self);

	void (*load_thread)(baros_t *self, const thread_cfg_t *cfg, uint32_t n);
	void (*start)(baros_t *self);
};

extern baros_t g_baros;

// critical section
static inline void baros_enter_critical(void) {
	__asm volatile("cpsid i" ::: "memory");
}

static inline void baros_exit_critical(void) {
	__asm volatile("cpsie i" ::: "memory");
}

void baros_delay(uint32_t ms);
void baros_delay_until(uint32_t target);

#endif	  //__BAROS__
