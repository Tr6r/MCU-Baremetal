#include <stdint.h>

#include "rcc.h"
#include "gpio.h"

int main_app(void) {
	RCC_Init(RCC_CLOCK_LOW);
	GPIO_Init(GPIOC, GPIO_PIN_13, GPIO_MODE_OUTPUT_2MHz, GPIO_CNF_OUTPUT_OD);
	for (;;) {
		GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_LOW);
		for (volatile uint32_t i = 0; i < 1000000U; ++i) {}

		GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_HIGH);
		for (volatile uint32_t i = 0; i < 1000000U; ++i) {}
	}
}