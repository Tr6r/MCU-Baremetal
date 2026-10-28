#ifndef __GPIO__
#define __GPIO__

#include <stm32f1.h>

#define GPIOA_ADDRESS (PERIPHERAL_ADDRESS + 0x00010800U)
#define GPIOB_ADDRESS (PERIPHERAL_ADDRESS + 0x00010C00U)
#define GPIOC_ADDRESS (PERIPHERAL_ADDRESS + 0x00011000U)
#define GPIOD_ADDRESS (PERIPHERAL_ADDRESS + 0x00011400U)
#define GPIOE_ADDRESS (PERIPHERAL_ADDRESS + 0x00011800U)
#define GPIOF_ADDRESS (PERIPHERAL_ADDRESS + 0x00011C00U)
#define GPIOG_ADDRESS (PERIPHERAL_ADDRESS + 0x00012000U)

typedef struct {
	volatile uint32_t CRL;	   // Configuration register low
	volatile uint32_t CRH;	   // Configuration register high
	volatile uint32_t IDR;	   // Input data register
	volatile uint32_t ODR;	   // Output data register
	volatile uint32_t BSRR;	   // Bit set/reset register
	volatile uint32_t BRR;	   // Bit reset register
	volatile uint32_t LCKR;	   // Configuration lock register
} GPIO_TypeDef;

#define GPIOA ((GPIO_TypeDef *)GPIOA_ADDRESS)
#define GPIOB ((GPIO_TypeDef *)GPIOB_ADDRESS)
#define GPIOC ((GPIO_TypeDef *)GPIOC_ADDRESS)
#define GPIOD ((GPIO_TypeDef *)GPIOD_ADDRESS)
#define GPIOE ((GPIO_TypeDef *)GPIOE_ADDRESS)
#define GPIOF ((GPIO_TypeDef *)GPIOF_ADDRESS)
#define GPIOG ((GPIO_TypeDef *)GPIOG_ADDRESS)

typedef enum {
	GPIO_MODE_INPUT		   = 0x00,
	GPIO_MODE_OUTPUT_10MHz = 0x1,
	GPIO_MODE_OUTPUT_2MHz  = 0x2,
	GPIO_MODE_OUTPUT_50MHz = 0x3,
} GPIO_Mode_t;

typedef enum {
	GPIO_LOW,
	GPIO_HIGH,
} GPIO_State_t;

typedef enum {
	GPIO_PIN_0	= 0,
	GPIO_PIN_1	= 1,
	GPIO_PIN_2	= 2,
	GPIO_PIN_3	= 3,
	GPIO_PIN_4	= 4,
	GPIO_PIN_5	= 5,
	GPIO_PIN_6	= 6,
	GPIO_PIN_7	= 7,
	GPIO_PIN_8	= 8,
	GPIO_PIN_9	= 9,
	GPIO_PIN_10 = 10,
	GPIO_PIN_11 = 11,
	GPIO_PIN_12 = 12,
	GPIO_PIN_13 = 13,
	GPIO_PIN_14 = 14,
	GPIO_PIN_15 = 15
} GPIO_Pin_t;

typedef enum {
	GPIO_CNF_INPUT_ANALOG	= 0x00,	   // Analog input
	GPIO_CNF_INPUT_FLOATING = 0x01,	   // Floating input
	GPIO_CNF_INPUT_PU_PD	= 0x2,
	GPIO_CNF_OUTPUT_PP		= 0x00,	   // Push-Pull output
	GPIO_CNF_OUTPUT_OD		= 0x01,	   // Open-Drain output
	GPIO_CNF_AF_PP			= 0x2,	   // Alternate function Push-Pull output
	GPIO_CNF_AF_OD			= 0x3,
} GPIO_CNF_t;

typedef enum {
	GPIO_PUSH_PULL = 0,
	GPIO_OPENDRAIN,
} GPIO_Pull_t;

typedef struct {
	GPIO_Mode_t Mode;
	GPIO_CNF_t CNF;
	GPIO_Pin_t Pin;

} GPIO_Config_t;

typedef struct {
	GPIO_Config_t GPIO_Config;
	GPIO_TypeDef *Instance;
} GPIO_Handle_t;

bool GPIO_Init(GPIO_TypeDef *Instance, GPIO_Pin_t Pin, GPIO_Mode_t Mode, GPIO_CNF_t CNF);
void GPIO_WritePin(GPIO_Handle_t *xGPIO, GPIO_State_t State);

#endif	  //__GPIO__
