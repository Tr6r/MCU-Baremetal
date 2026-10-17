TARGET := application
BUILD_DIR := ../build/$(TARGET)
include ../config.mk

ELF := $(BUILD_DIR)/$(TARGET).elf
BIN := $(BUILD_DIR)/$(TARGET).bin

.PHONY: all flash debug_flash clean
all: firmware
firmware: $(ELF) $(BIN)

$(ELF): $(STARTUP) $(MAIN) $(LINKER_SCRIPT) Makefile ../config.mk
	@mkdir -p $(BUILD_DIR)
	@set -eu; \
	 cleanup() { \
	   result=$$?; \
	   rm -f "$(BUILD_DIR)/startup.o" "$(BUILD_DIR)/main.o"; \
	   if [ "$$result" -ne 0 ]; then rm -f "$(ELF)" "$(BIN)"; fi; \
	 }; \
	 trap cleanup EXIT; \
	 rm -f "$(ELF)"; \
	 $(CC) $(ASFLAGS) -c $(STARTUP) -o "$(BUILD_DIR)/startup.o"; \
	 $(CC) $(CFLAGS) -c $(MAIN) -o "$(BUILD_DIR)/main.o"; \
	 $(CC) $(LDFLAGS) "$(BUILD_DIR)/startup.o" "$(BUILD_DIR)/main.o" -o "$(ELF)"; \
	 bytes=$$(wc -c < "$(ELF)"); \
	 printf "Output: %s (%s bytes)\n" "$(ELF)" "$$bytes"

$(BIN): $(ELF)
	$(OBJCOPY) -O binary "$(ELF)" "$(BIN)"
	@bytes=$$(wc -c < "$(BIN)"); printf "Output: %s (%s bytes)\n" "$(BIN)" "$$bytes"

clean:
	rm -rf ../build/$(TARGET)

flash: $(BIN)
	$(OPENOCD) -f $(OPENOCD_INTERFACE) -f $(OPENOCD_TARGET) -c "program $(BIN) $(FLASH_ORIGIN) verify reset exit"

debug_flash: $(ELF)
	$(OPENOCD) -f $(OPENOCD_INTERFACE) -f $(OPENOCD_TARGET) -c "program $(ELF) verify reset exit"
