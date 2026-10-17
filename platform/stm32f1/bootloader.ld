/* STM32F103C8T6 bootloader: 16 KiB/ 64 KiB Flash, 20 KiB SRAM */

ENTRY(Reset_Handler)

MEMORY
{
    FLASH (rx)  : ORIGIN = 0x08000000, LENGTH = 16K /* 16 × 1024 bytes. */ 
    RAM   (rwx) : ORIGIN = 0x20000000, LENGTH = 20K /* 20 × 1024 bytes. */
}

_estack = ORIGIN(RAM) + LENGTH(RAM);

/* Define the sections in the final image and where they go. */
SECTIONS
{
    .isr_vector ORIGIN(FLASH) :
    {
        *(.isr_vector)
    } > FLASH

    .text :
    {
        . = ALIGN(4);
        *(.text*) /* main code, variables */
        *(.rodata*) /* constant variables */
        . = ALIGN(4);
    } > FLASH

    .data :
    {
        . = ALIGN(4);
        _sdata = .;    /* Start address of .data in RAM. */
        *(.data*)      /* Collect initialized-data input sections. */
        . = ALIGN(4);
        _edata = .;    /* Address just after .data in RAM. */
    } > RAM AT > FLASH

    
    _sidata = LOADADDR(.data);

    .bss (NOLOAD) :
    {
        . = ALIGN(4);
        _sbss = .;     /* Start of the RAM range to clear. */
        *(.bss*)       /* Collect .bss input sections. */
        *(COMMON)      /* Collect older-style common globals, if any. */
        . = ALIGN(4);
        _ebss = .;     /* Address just after the range to clear. */
    } > RAM
}