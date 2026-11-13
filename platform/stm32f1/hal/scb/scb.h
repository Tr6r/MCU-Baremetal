#ifndef __SCB__
#define __SCB__

#include "stm32f1.h"
#define SCB_BASE 0xE000ED00

typedef struct {
	volatile uint32_t CPUID;	// CPU ID: read core type and revision
	volatile uint32_t ICSR;		// Interrupt control/state: pend-clear PendSV, SysTick, NMI; active/pending vector
	volatile uint32_t VTOR;		// Vector table offset: relocate the vector table (bootloader -> app)
	volatile uint32_t AIRCR;	// App interrupt/reset control: software reset, priority grouping, endianness
	volatile uint32_t SCR;		// System control: sleep modes (sleep, deep-sleep, sleep-on-exit)
	volatile uint32_t CCR;		// Configuration/control: fault traps (div-by-zero, unaligned), stack alignment
	volatile uint32_t SHPR1;	// System handler priority: MemManage, BusFault, UsageFault
	volatile uint32_t SHPR2;	// System handler priority: SVCall
	volatile uint32_t SHPR3;	// System handler priority: PendSV, SysTick (set PendSV lowest here)
	volatile uint32_t SHCSR;	// System handler control/state: enable fault handlers, read fault state
	volatile uint32_t CFSR;		// Configurable fault status: MemManage / BusFault / UsageFault details
	volatile uint32_t HFSR;		// HardFault status
	volatile uint32_t DFSR;		// Debug fault status
	volatile uint32_t MMFAR;	// MemManage fault address
	volatile uint32_t BFAR;		// BusFault address
	volatile uint32_t AFSR;		// Auxiliary fault status (vendor-specific)
} SCB_TypeDef;

#define SCB ((SCB_TypeDef *)SCB_BASE)

void SCB_PendSV_Set(void);
void SCB_SetPendSV_Priority_Lowest(void);

#endif	  //__SCB__