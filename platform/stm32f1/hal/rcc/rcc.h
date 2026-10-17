#ifndef __RCC__
#define __RCC__

#include <stm32f1.h>

// RCC_ADDRESS
#define RCC_BASE_ADDRESS (PERIPHERAL_ADDRESS + 0x00021000U)

#define CR_HSION  (1U << 0)
#define CR_HSIRDY (1U << 1)
#define CR_HSEON  (1U << 16)
#define CR_HSERDY (1U << 17)
#define CR_HSEBYP (1U << 18)
#define CR_CSSON  (1U << 19)
#define CR_PLLON  (1U << 24)
#define CR_PLLRDY (1U << 25)

#define CFGR_SW_MASK	 (3U << 0)
#define CFGR_SW_HSE		 (1U << 0)
#define CFGR_SW_PLL		 (2U << 0)
#define CFGR_SWS_MASK	 (3U << 2)
#define CFGR_SWS_HSE	 (1U << 2)
#define CFGR_SWS_PLL	 (2U << 2)
#define CFGR_HPRE_MASK	 (15U << 4)
#define CFGR_PPRE1_MASK	 (7U << 8)
#define CFGR_PPRE1_DIV2	 (4U << 8)
#define CFGR_PPRE2_MASK	 (7U << 11)
#define CFGR_ADCPRE_MASK (3U << 14)
#define CFGR_ADCPRE_DIV4 (1U << 14)
#define CFGR_ADCPRE_DIV6 (2U << 14)
#define CFGR_PLLSRC_HSE	 (1U << 16)
#define CFGR_PLLXTPRE	 (1U << 17)
#define CFGR_PLLMUL_MASK (15U << 18)
#define CFGR_USBPRE		 (1U << 22)

#define FLASH_LATENCY_MASK 7U
#define FLASH_PREFETCH	   (1U << 4)
#define FLASH_ACR (*(volatile uint32_t *)0x40022000U)

#define RCC_TIMEOUT 1000000U

typedef struct {
	volatile uint32_t CR;		   // Clock control register
	volatile uint32_t CFGR;		   // Clock configuration register
	volatile uint32_t CIR;		   // Clock interrupt register
	volatile uint32_t APB2RSTR;	   // APB2 peripheral reset register
	volatile uint32_t APB1RSTR;	   // APB1 peripheral reset register
	volatile uint32_t AHBENR;	   // AHB peripheral clock enable register
	volatile uint32_t APB2ENR;	   // APB2 peripheral clock enable register
	volatile uint32_t APB1ENR;	   // APB1 peripheral clock enable register
	volatile uint32_t BDCR;		   // Backup domain control register
	volatile uint32_t CSR;		   // Control/status register
} RCC_TypeDef_t;

#define RCC ((RCC_TypeDef_t *)RCC_BASE_ADDRESS)

typedef enum {
	RCC_CLOCK_LOW,		 // HSE: 8 MHz
	RCC_CLOCK_MEDIUM,	 // HSE 8 MHz x 4: 32 MHz
	RCC_CLOCK_HIGH		 // HSE 8 MHz x 9: 72 MHz
} RCC_ClockLevel_t;

bool RCC_Init(RCC_ClockLevel_t level);

#endif	  //__RCC__
