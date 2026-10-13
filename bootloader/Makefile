TARGET := bootloader
BUILD_DIR := build

-include config.mk
OUTPUT_FORMAT ?= elf

VALID_FORMATS := elf bin
ifeq ($(filter $(OUTPUT_FORMAT),$(VALID_FORMATS)),)
$(error OUTPUT_FORMAT must be elf or bin)
endif

TOOLCHAIN_BIN ?= $(HOME)/Documents/workspace/tools/gcc-arm-none-eabi-10.3-2021.10/bin
CC := $(TOOLCHAIN_BIN)/arm-none-eabi-gcc
OBJCOPY := $(TOOLCHAIN_BIN)/arm-none-eabi-objcopy
OPENOCD ?= openocd
OPENOCD_INTERFACE ?= interface/stlink.cfg
OPENOCD_TARGET ?= target/stm32f1x.cfg
FLASH_ORIGIN ?= 0x08000000

STARTUP := platform/stm32f1/startup.S
MAIN := sources/src/main.c
LINKER_SCRIPT := platform/stm32f1/bootloader.ld

ELF := $(BUILD_DIR)/$(TARGET).elf
BIN := $(BUILD_DIR)/$(TARGET).bin
OUTPUT := $(BUILD_DIR)/$(TARGET).$(OUTPUT_FORMAT)

CPU_FLAGS := -mcpu=cortex-m3 -mthumb
CFLAGS := $(CPU_FLAGS) -std=c11 -O0 -g3 -ffreestanding -Wall -Wextra
ASFLAGS := $(CPU_FLAGS) -g3
LDFLAGS := $(CPU_FLAGS) -nostdlib -T $(LINKER_SCRIPT) -Wl,--build-id=none

.PHONY: all clean flash
all: $(OUTPUT)

$(OUTPUT): $(STARTUP) $(MAIN) $(LINKER_SCRIPT) Makefile config.mk
	@mkdir -p $(BUILD_DIR)
	@set -eu; \
	 cleanup() { \
	   result=$$?; \
	   rm -f "$(BUILD_DIR)/startup.o" "$(BUILD_DIR)/main.o"; \
	   if [ "$$result" -ne 0 ]; then rm -f "$(ELF)" "$(BIN)"; fi; \
	 }; \
	 trap cleanup EXIT; \
	 rm -f "$(ELF)" "$(BIN)"; \
	 $(CC) $(ASFLAGS) -c $(STARTUP) -o "$(BUILD_DIR)/startup.o"; \
	 $(CC) $(CFLAGS) -c $(MAIN) -o "$(BUILD_DIR)/main.o"; \
	 $(CC) $(LDFLAGS) "$(BUILD_DIR)/startup.o" "$(BUILD_DIR)/main.o" -o "$(ELF)"; \
	 if [ "$(OUTPUT_FORMAT)" = bin ]; then \
	   $(OBJCOPY) -O binary "$(ELF)" "$(BIN)"; \
	   rm -f "$(ELF)"; \
	 fi; \
	 bytes=$$(wc -c < "$(OUTPUT)"); \
	 printf "Output: %s (%s bytes)\n" "$(OUTPUT)" "$$bytes"

clean:
	rm -rf $(BUILD_DIR)

flash: $(OUTPUT)
ifeq ($(OUTPUT_FORMAT),bin)
	$(OPENOCD) -f $(OPENOCD_INTERFACE) -f $(OPENOCD_TARGET) -c "program $(OUTPUT) $(FLASH_ORIGIN) verify reset exit"
else
	$(OPENOCD) -f $(OPENOCD_INTERFACE) -f $(OPENOCD_TARGET) -c "program $(OUTPUT) verify reset exit"
endif
