TARGET := application
BUILD_DIR := ../build/$(TARGET)
include ../config.mk

HAL_ROOT := ../platform/stm32f1/hal
HAL_LIB := hal/libstm32f1_hal.a
HAL_INPUTS := $(wildcard $(HAL_ROOT)/Makefile $(HAL_ROOT)/*.h $(HAL_ROOT)/*/Makefile $(HAL_ROOT)/*/*.c $(HAL_ROOT)/*/*.h) ../config.mk
CFLAGS += -Ihal/include

# baros kernel (layer 2) — nguon liet ke trong baros/Makefile
BAROS_SOURCES :=
BAROS_CPPFLAGS :=
include baros/Makefile
CFLAGS += $(BAROS_CPPFLAGS)

# app layer (layer 3) — component C++, nguon liet ke trong sources/Makefile
APP_SOURCES :=
APP_CPPFLAGS :=
include sources/Makefile
CFLAGS += $(APP_CPPFLAGS)
CXXFLAGS += -Ihal/include $(BAROS_CPPFLAGS) $(APP_CPPFLAGS)

ELF := $(BUILD_DIR)/$(TARGET).elf
BIN := $(BUILD_DIR)/$(TARGET).bin

.PHONY: all flash debug_flash clean
all: firmware
firmware: $(ELF) $(BIN)

$(HAL_LIB): $(HAL_INPUTS)
	$(MAKE) -C $(HAL_ROOT) all

$(ELF): $(STARTUP) $(BAROS_SOURCES) $(APP_SOURCES) $(LINKER_SCRIPT) $(HAL_LIB) Makefile baros/Makefile sources/Makefile ../config.mk
	@mkdir -p $(BUILD_DIR)
	@set -eu; \
	 objs="$(BUILD_DIR)/startup.o"; \
	 cleanup() { \
	   result=$$?; \
	   for o in $$objs; do rm -f "$$o"; done; \
	   if [ "$$result" -ne 0 ]; then rm -f "$(ELF)" "$(BIN)"; fi; \
	 }; \
	 trap cleanup EXIT; \
	 rm -f "$(ELF)"; \
	 $(CC) $(ASFLAGS) -c $(STARTUP) -o "$(BUILD_DIR)/startup.o"; \
	 for src in $(BAROS_SOURCES); do \
	   obj="$(BUILD_DIR)/$$(basename "$$src" .c).o"; \
	   $(CC) $(CFLAGS) -c "$$src" -o "$$obj"; \
	   objs="$$objs $$obj"; \
	 done; \
	 for src in $(APP_SOURCES); do \
	   obj="$(BUILD_DIR)/$$(basename "$$src" .cpp).o"; \
	   $(CXX) $(CXXFLAGS) -c "$$src" -o "$$obj"; \
	   objs="$$objs $$obj"; \
	 done; \
	 $(CXX) $(LDFLAGS) $$objs "$(HAL_LIB)" -o "$(ELF)"; \
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
