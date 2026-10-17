#include "rcc.h"
#include "common.h"

bool RCC_Init(RCC_ClockLevel_t level) {
	uint32_t latency;
	uint32_t pll_mul = 0;
	uint32_t apb1	 = 0;
	uint32_t adc	 = 0;

	switch (level) {
	case RCC_CLOCK_LOW:	   // HSE 8 MHz
		latency = 0;
		break;
	case RCC_CLOCK_MEDIUM:	  // HSE × 4 = 32 MHz
		latency = 1;
		pll_mul = 4;
		adc		= CFGR_ADCPRE_DIV4;
		break;
	case RCC_CLOCK_HIGH:	// HSE × 9 = 72 MHz
		latency = 2;
		pll_mul = 9;
		apb1	= CFGR_PPRE1_DIV2;
		adc		= CFGR_ADCPRE_DIV6;
		break;
	default:
		return false;
	}

	FLASH_ACR = (FLASH_ACR & ~FLASH_LATENCY_MASK) | latency | FLASH_PREFETCH;

	RCC->CR |= CR_HSEON;
	if (!HAL_WaitBits(&RCC->CR, CR_HSERDY, CR_HSERDY, RCC_TIMEOUT))
		return false;

	uint32_t cfgr = RCC->CFGR;
	cfgr &= ~(CFGR_HPRE_MASK | CFGR_PPRE1_MASK | CFGR_PPRE2_MASK | CFGR_ADCPRE_MASK | CFGR_PLLSRC_HSE | CFGR_PLLXTPRE | CFGR_PLLMUL_MASK | CFGR_USBPRE);
	cfgr |= apb1 | adc;

	if (level == RCC_CLOCK_LOW) {
		RCC->CFGR = cfgr | CFGR_SW_HSE;
		return HAL_WaitBits(&RCC->CFGR, CFGR_SWS_MASK, CFGR_SWS_HSE, RCC_TIMEOUT);
	}

	cfgr |= CFGR_PLLSRC_HSE | ((pll_mul - 2U) << 18);
	RCC->CFGR = cfgr;

	RCC->CR |= CR_PLLON;
	if (!HAL_WaitBits(&RCC->CR, CR_PLLRDY, CR_PLLRDY, RCC_TIMEOUT))
		return false;

	RCC->CFGR = (RCC->CFGR & ~CFGR_SW_MASK) | CFGR_SW_PLL;
	return HAL_WaitBits(&RCC->CFGR, CFGR_SWS_MASK, CFGR_SWS_PLL, RCC_TIMEOUT);
}

bool RCC_DeInit(void) {
	RCC->CR |= CR_HSION;
	if (!HAL_WaitBits(&RCC->CR, CR_HSIRDY, CR_HSIRDY, RCC_TIMEOUT))
		return false;

	RCC->CFGR &= ~CFGR_SW_MASK;
	if (!HAL_WaitBits(&RCC->CFGR, CFGR_SWS_MASK, 0, RCC_TIMEOUT))
		return false;

	RCC->CR &= ~(CR_CSSON | CR_PLLON);
	if (!HAL_WaitBits(&RCC->CR, CR_PLLRDY, 0, RCC_TIMEOUT))
		return false;

	RCC->CR &= ~CR_HSEON;
	if (!HAL_WaitBits(&RCC->CR, CR_HSERDY, 0, RCC_TIMEOUT))
		return false;
	RCC->CR &= ~CR_HSEBYP;

	RCC->CFGR &= ~(CFGR_HPRE_MASK | CFGR_PPRE1_MASK | CFGR_PPRE2_MASK | CFGR_ADCPRE_MASK | CFGR_PLLSRC_HSE | CFGR_PLLXTPRE | CFGR_PLLMUL_MASK | CFGR_USBPRE);

	FLASH_ACR &= ~FLASH_LATENCY_MASK;
	return true;
}