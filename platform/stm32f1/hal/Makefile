include ../../../config.mk

HAL_SOURCES :=
HAL_PUBLIC_HEADERS :=
HAL_CPPFLAGS := -I.
include $(wildcard */Makefile)

BUILD_DIR := ../../../build/hal
LIB := $(BUILD_DIR)/libstm32f1_hal.a
HAL_OBJECTS := $(patsubst %.c,$(BUILD_DIR)/obj/%.o,$(HAL_SOURCES))
HAL_MAKEFILES := $(MAKEFILE_LIST)

PUBLIC_HEADERS := $(addprefix $(BUILD_DIR)/include/,$(notdir $(HAL_PUBLIC_HEADERS)))
vpath %.h $(sort $(dir $(HAL_PUBLIC_HEADERS)))

.PHONY: all clean
all: $(LIB) $(PUBLIC_HEADERS)

$(BUILD_DIR)/obj/%.o: %.c $(HAL_MAKEFILES)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(HAL_CPPFLAGS) -MMD -MP -c $< -o $@

-include $(HAL_OBJECTS:.o=.d)

$(LIB): $(HAL_OBJECTS) $(HAL_MAKEFILES)
	@mkdir -p $(dir $@)
	rm -f $@
	$(AR) rcs $@ $(HAL_OBJECTS)

$(BUILD_DIR)/include/%.h: %.h
	@mkdir -p $(dir $@)
	cp $< $@

clean:
	rm -rf $(BUILD_DIR)
