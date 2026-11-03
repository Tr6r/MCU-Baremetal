#include <stdint.h>

#include "rcc.h"
#include "gpio.h"
#include "systick.h"

volatile uint32_t sys_tick = 0;

int main_app(void) {
	if (RCC_Init(RCC_CLOCK_LOW)) {
		return -1;
	}
	if (SYSTICK_Init(RCC_CLOCK_LOW)) {
		return -1;
	}
	if (GPIO_Init(GPIOC, GPIO_PIN_13, GPIO_MODE_OUTPUT_10MHz, GPIO_CNF_OUTPUT_PP)) {
		return -1;
	}
	SYSTICK_Start();
	uint32_t last_event_ms = 0;

	for (;;) {
		if (sys_tick - last_event_ms >= 2000)	 // 2s
		{
			GPIO_Write_Toggle(GPIOC, GPIO_PIN_13);
			last_event_ms = sys_tick;
		}
	}
}

void SysTick_Handler(void) {
	sys_tick++;
}