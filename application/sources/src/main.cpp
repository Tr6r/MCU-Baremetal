#include <stdint.h>

extern "C" {
#include "rcc.h"
#include "systick.h"
#include "scb.h"
}

#include "task_list.h"

extern "C" int main_app(void) {
	if (RCC_Init(RCC_CLOCK_LOW)) {
		return -1;
	}
	if (SYSTICK_Init(RCC_CLOCK_LOW)) {
		return -1;
	}

	SCB_Init();

	g_baros.init(&g_baros);
	g_baros.load_thread(&g_baros, g_task_list, g_task_count);

	SYSTICK_Start();
	g_baros.start(&g_baros);

	for (;;) {}
}
