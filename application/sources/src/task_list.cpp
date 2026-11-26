#include "task_list.h"
#include "task_led.h"

const thread_cfg_t g_task_list[] = {
	{TASK_ID_LED ,task_led_handler, nullptr, THREAD_PRIO_4, },
};

const uint32_t g_task_count = sizeof(g_task_list) / sizeof(g_task_list[0]);
