#ifndef __SYSTICK__
#define __SYSTICK__

#include "stm32f1.h"
#include "rcc.h"

#define SYSTICK_BASE 0xE000E010U

typedef struct {
	volatile uint32_t CTRL;
	volatile uint32_t LOAD;
	volatile uint32_t VAL;
	volatile uint32_t CALIB;
} SysTick_TypeDef;

#define SYSTICK ((SysTick_TypeDef *)SYSTICK_BASE)

#define SYSTICK_CTRL_ENABLE	   (1U << 0)
#define SYSTICK_CTRL_TICKINT   (1U << 1)
#define SYSTICK_CTRL_CLKSOURCE (1U << 2)

int SYSTICK_Init(RCC_ClockLevel_t clockLevel);
void SYSTICK_Start(void);
void SYSTICK_Stop(void);
void SYSTICK_Reset(void);
void SYSTICK_DeInit(void);

#endif	  //__SYSTICK__