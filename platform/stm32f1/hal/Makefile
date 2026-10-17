include ../../../config.mk

HAL_SOURCES :=
HAL_HEADERS := stm32f1.h
HAL_CPPFLAGS := -I.
include common/Makefile
include rcc/Makefile

BUILD_DIR := ../../../build/hal
LIB := $(BUILD_DIR)/libstm32f1_hal.a
HAL_OBJECTS := $(patsubst %.c,$(BUILD_DIR)/obj/%.o,$(HAL_SOURCES))

PUBLIC_HEADERS := $(BUILD_DIR)/include/rcc.h

.PHONY: all clean
all: $(LIB) $(PUBLIC_HEADERS)

$(BUILD_DIR)/obj/%.o: %.c $(HAL_HEADERS) Makefile common/Makefile rcc/Makefile ../../../config.mk
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(HAL_CPPFLAGS) -c $< -o $@

$(LIB): $(HAL_OBJECTS) Makefile common/Makefile rcc/Makefile ../../../config.mk
	@mkdir -p $(dir $@)
	$(AR) rcs $@ $(HAL_OBJECTS)

$(BUILD_DIR)/include/rcc.h: rcc/rcc.h
	@mkdir -p $(dir $@)
	cp $< $@

clean:
	rm -rf $(BUILD_DIR)
