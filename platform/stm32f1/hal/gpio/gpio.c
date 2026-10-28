#include "gpio.h"
#include "rcc.h"

static bool GPIO_EnableClock(GPIO_TypeDef *Instance) {
	uintptr_t address = (uintptr_t)Instance;
	uintptr_t offset;

	if (address < GPIOA_ADDRESS || address > GPIOG_ADDRESS)
		return false;

	offset = address - GPIOA_ADDRESS;
	if (offset % (GPIOB_ADDRESS - GPIOA_ADDRESS) != 0U)
		return false;

	RCC->APB2ENR |= 1U << (2U + offset / (GPIOB_ADDRESS - GPIOA_ADDRESS));
	return true;
}

bool GPIO_Init(GPIO_TypeDef *Instance, GPIO_Pin_t Pin, GPIO_Mode_t Mode, GPIO_CNF_t CNF) {
	if (!GPIO_EnableClock(Instance))
		return false;

	if (Mode == GPIO_MODE_INPUT && CNF == GPIO_CNF_INPUT_PU_PD) {
		Instance->ODR |= 1U << Pin;	   // Pull-up by default.
	}
	else if (Mode != GPIO_MODE_INPUT && CNF >= GPIO_CNF_AF_PP) {
		AFIO_EN_CLOCK();
	}

	volatile uint32_t *config_reg;
	uint32_t shift;
	uint32_t config;

	Instance->CRL |= (Mode << (Pin * 4));	 // Set mode (2 bit)
	config_reg	= Pin < GPIO_PIN_8 ? &Instance->CRL : &Instance->CRH;
	shift		= ((uint32_t)Pin & 7U) * 4U;
	config		= ((uint32_t)CNF << 2) | (uint32_t)Mode;
	*config_reg = (*config_reg & ~(0xFU << shift)) | (config << shift);
}

void GPIO_WritePin(GPIO_Handle_t *xGPIO, GPIO_State_t State) {
	if (State == GPIO_LOW) {
		xGPIO->Instance->ODR &= ~(1 << xGPIO->GPIO_Config.Pin);
	}
	else {
		xGPIO->Instance->ODR |= (1 << xGPIO->GPIO_Config.Pin);
	}
}