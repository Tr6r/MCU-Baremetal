#ifndef __COMMON__
#define __COMMON__

#include "stm32f1.h"

bool HAL_WaitBits(volatile uint32_t *reg, uint32_t mask, uint32_t expected, uint32_t timeout);

#endif	  //__COMMON__
