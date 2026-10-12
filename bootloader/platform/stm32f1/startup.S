.syntax unified
.cpu cortex-m3
.thumb

.section .isr_vector, "a", %progbits
.balign 4
.global g_pfnVectors
g_pfnVectors:
    .word _estack
    .word Reset_Handler

    .rept 5                /* NMI, HardFault, MemManage, BusFault, UsageFault */
    .word Default_Handler
    .endr

    .rept 4
        .word 0
    .endr

    .word Default_Handler  /* SVCall */
    .word Default_Handler  /* DebugMon */
    .word 0
    .word Default_Handler  /* PendSV */
    .word Default_Handler  /* SysTick */


.section .text.Reset_Handler, "ax", %progbits
.balign 4
.global Reset_Handler
.type Reset_Handler, %function
.thumb_func
Reset_Handler:
    ldr r0, =_sidata
    ldr r1, =_sdata
    ldr r2, =_edata
1:
    cmp r1, r2
    bcs 2f
    ldrb r3, [r0]
    strb r3, [r1]
    adds r0, r0, #1
    adds r1, r1, #1
    b 1b

2:
    ldr r1, =_sbss
    ldr r2, =_ebss
    movs r3, #0
3:
    cmp r1, r2
    bcs 4f
    strb r3, [r1]
    adds r1, r1, #1
    b 3b
4:
    bl main
5:
    b 5b
.size Reset_Handler, .-Reset_Handler

.section .text.Default_Handler, "ax", %progbits
.balign 4
.global Default_Handler
.type Default_Handler, %function
.thumb_func
Default_Handler:
    b .
.size Default_Handler, .-Default_Handler