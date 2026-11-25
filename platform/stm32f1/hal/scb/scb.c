#include "stm32f1.h"
#include "scb.h"

static void SCB_SetPendSV_Priority_Lowest(void);
static void SCB_SetSysTick_Priority_Lowest(void);

void SCB_Init(void) {
	SCB_SetPendSV_Priority_Lowest();
	SCB_SetSysTick_Priority_Lowest();
}

void SCB_PendSV_Set(void) {
	// Request a context switch: mark PendSV pending
	SCB->ICSR = SCB_ICSR_PENDSVSET;
	__asm volatile("dsb" ::: "memory");
	__asm volatile("isb" ::: "memory");
}

static void SCB_SetPendSV_Priority_Lowest(void) {
	// Clear the PendSV priority byte, then set it to the lowest level so the
	SCB->SHPR3 = (SCB->SHPR3 & ~SCB_SHPR3_PENDSV_Msk) | (SCB_PRIORITY_LOWEST << SCB_SHPR3_PENDSV_Pos);
}

static void SCB_SetSysTick_Priority_Lowest(void) {
	SCB->SHPR3 = (SCB->SHPR3 & ~SCB_SHPR3_SYSTICK_Msk) | (SCB_PRIORITY_LOWEST << SCB_SHPR3_SYSTICK_Pos);
}

void SCB_SystemReset(void) {
	__asm volatile("dsb" ::: "memory");
	SCB->AIRCR = SCB_AIRCR_VECTKEY | (SCB->AIRCR & SCB_AIRCR_PRIGROUP_Msk) | SCB_AIRCR_SYSRESETREQ;
	__asm volatile("dsb" ::: "memory");
	for (;;) {}
}

