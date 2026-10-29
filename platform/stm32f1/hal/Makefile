include ../../../config.mk

HAL_SOURCES :=
HAL_PUBLIC_HEADERS := stm32f1.h
HAL_CPPFLAGS := -I.
include $(wildcard */Makefile)

BUILD_DIR := ../../../build/hal
LIB := $(BUILD_DIR)/libstm32f1_hal.a
HAL_OBJECTS := $(patsubst %.c,$(BUILD_DIR)/obj/%.o,$(HAL_SOURCES))
HAL_MAKEFILES := $(MAKEFILE_LIST)

PUBLIC_HEADERS := $(addprefix $(BUILD_DIR)/include/,$(notdir $(HAL_PUBLIC_HEADERS)))
HAL_EXPORT_DIRS := ../../../application/hal ../../../bootloader/hal
vpath %.h $(sort $(dir $(HAL_PUBLIC_HEADERS)))

.PHONY: all export clean
all: export

export: $(LIB) $(PUBLIC_HEADERS)
	@set -eu; for dir in $(HAL_EXPORT_DIRS); do \
		rm -rf "$$dir"; \
		mkdir -p "$$dir/include"; \
		cp "$(LIB)" "$$dir/"; \
		cp $(PUBLIC_HEADERS) "$$dir/include/"; \
	done

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
	rm -rf $(BUILD_DIR) $(HAL_EXPORT_DIRS)
