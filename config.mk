TOOLCHAIN_BIN ?= $(HOME)/Documents/workspace/tools/gcc-arm-none-eabi-10.3-2021.10/bin
CC := $(TOOLCHAIN_BIN)/arm-none-eabi-gcc
OBJCOPY := $(TOOLCHAIN_BIN)/arm-none-eabi-objcopy
AR := $(TOOLCHAIN_BIN)/arm-none-eabi-ar
OPENOCD ?= openocd
OPENOCD_INTERFACE ?= interface/stlink.cfg
OPENOCD_TARGET ?= target/stm32f1x.cfg

MAIN := sources/src/main.c

ifeq ($(TARGET),bootloader)
FLASH_ORIGIN ?= 0x08000000
STARTUP := ../platform/stm32f1/bootloader_startup.S
LINKER_SCRIPT := ../platform/stm32f1/bootloader.ld
else ifeq ($(TARGET),application)
FLASH_ORIGIN ?= 0x08004000
STARTUP := ../platform/stm32f1/application_startup.S
LINKER_SCRIPT := ../platform/stm32f1/application.ld
endif

MODE_FLAGS := -Os -g3
CPU_FLAGS := -mcpu=cortex-m3 -mthumb
CFLAGS := $(CPU_FLAGS) -std=c11 $(MODE_FLAGS) -ffreestanding -Wall -Wextra
ASFLAGS := $(CPU_FLAGS) $(MODE_FLAGS)
LDFLAGS := $(CPU_FLAGS) -nostdlib -T $(LINKER_SCRIPT) -Wl,--build-id=none -Wl,-n
