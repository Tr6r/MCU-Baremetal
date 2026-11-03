#include "systick.h"

int SYSTICK_Init(RCC_ClockLevel_t clockLevel) {
	uint32_t reload;

	switch (clockLevel) {
	case RCC_CLOCK_LOW: {
		reload = 8000U - 1U;
		break;
	}

	case RCC_CLOCK_MEDIUM: {
		reload = 32000U - 1U;
		break;
	}

	case RCC_CLOCK_HIGH: {
		reload = 72000U - 1U;
		break;
	}

	default: {
		return HAL_INVALID;
	}
	}

	SYSTICK->CTRL = 0U;		   // Reset CTRL register
	SYSTICK->LOAD = reload;	   // Set 1 ms tick period
	SYSTICK->VAL  = 0U;		   // Reset Counter

	SYSTICK->CTRL = SYSTICK_CTRL_CLKSOURCE | SYSTICK_CTRL_TICKINT;

	return HAL_OK;
}

void SYSTICK_Start(void) {
	SYSTICK->CTRL |= SYSTICK_CTRL_ENABLE;
}

void SYSTICK_Stop(void) {
	SYSTICK->CTRL &= ~SYSTICK_CTRL_ENABLE;
}

void SYSTICK_Reset(void) {
	SYSTICK->VAL = 0U;
}

void SYSTICK_DeInit(void) {
	SYSTICK_Stop();
	SYSTICK_Reset();
	SYSTICK->CTRL = 0U;	   // Reset CTRL register
	SYSTICK->LOAD = 0U;	   // Reset LOAD register
}