#include <stdint.h>

#define APP_START 0x08004000u
#define VTOR	  (*(volatile uint32_t *)0xE000ED08u)

void jump_to_application(void);

int main(void) {
	jump_to_application();
	return 0;
}

void jump_to_application(void) {
	const uint32_t *vector = (const uint32_t *)APP_START;
	uint32_t app_stack	   = vector[0];	   // at 0x08004000u
	uint32_t app_reset	   = vector[1];	   // application reset_handler function
	VTOR				   = APP_START;	   // define application vector at vtor address

	__asm volatile("dsb\n isb" ::: "memory");

	__asm volatile("msr msp, %0\n"
				   "bx %1"
				   :
				   : "r"(app_stack), "r"(app_reset)
				   : "memory");
	__builtin_unreachable();
}