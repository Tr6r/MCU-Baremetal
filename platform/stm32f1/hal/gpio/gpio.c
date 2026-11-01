#include "gpio.h"
#include "rcc.h"

static int PR_GPIO_EnableClock(GPIO_TypeDef *Instance) {
	uintptr_t address = (uintptr_t)Instance;
	uintptr_t offset;

	if (address < GPIOA_ADDRESS || address > GPIOG_ADDRESS)
		return HAL_NG;

	offset = address - GPIOA_ADDRESS;
	if (offset % (GPIOB_ADDRESS - GPIOA_ADDRESS) != 0U)
		return HAL_NG;

	RCC->APB2ENR |= 1U << (2U + offset / (GPIOB_ADDRESS - GPIOA_ADDRESS));
	return HAL_OK;
}

int GPIO_Init(GPIO_TypeDef *instance, GPIO_Pin_t pin, GPIO_Mode_t mode, GPIO_CNF_t CNF) {
	if (PR_GPIO_EnableClock(instance)) {
		return HAL_NG;
	}

	uint8_t pinPos				 = (pin % 8) * 4;
	volatile uint32_t *configReg = (pin < GPIO_PIN_8) ? &instance->CRL : &instance->CRH;

	switch (mode) {
	case GPIO_MODE_INPUT: {
		*configReg &= ~(0xF << pinPos);
		*configReg |= (mode << pinPos);
		*configReg |= (CNF << (pinPos + 2));
		if (CNF == GPIO_CNF_INPUT_PU_PD) {
			instance->ODR |= (1 << pin);
		}
		break;
	}

	case GPIO_MODE_OUTPUT_10MHz:
	case GPIO_MODE_OUTPUT_2MHz:
	case GPIO_MODE_OUTPUT_50MHz: {
		*configReg &= ~(0xF << pinPos);
		*configReg |= (mode << pinPos);
		*configReg |= (CNF << (pinPos + 2));
		break;
	}

	default:
		return HAL_NG;
	}

	return HAL_OK;
}

void GPIO_WritePin(GPIO_TypeDef *Instance, GPIO_Pin_t Pin, GPIO_State_t State) {
	if (State == GPIO_LOW) {
		Instance->ODR |= (1 << Pin);
	}
	else {
		Instance->ODR &= ~(1 << Pin);
	}
}