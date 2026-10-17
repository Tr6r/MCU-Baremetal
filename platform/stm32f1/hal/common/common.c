#include "common.h"

bool HAL_WaitBits(volatile uint32_t *reg, uint32_t mask, uint32_t expected, uint32_t timeout) {
	for (uint32_t i = 0; i < timeout; ++i) {
		if ((*reg & mask) == expected) {
			return true;
		}
	}
	return false;
}
