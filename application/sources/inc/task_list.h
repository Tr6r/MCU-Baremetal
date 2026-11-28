#ifndef __TASK_LIST__
#define __TASK_LIST__

extern "C" {
#include "baros.h"
}

typedef enum {
	TASK_ID_LED = 1,
	TASK_ID_SYS = 2
} task_id_e;

extern const thread_cfg_t g_task_list[];
extern const uint32_t g_task_count;

#endif
