# Include the core environment definitions; this will set $(TOP).
include ../../py/mkenv.mk

# qstr definitions (must come before including py.mk)
QSTR_DEFS = qstrdefsport.h

# MicroPython feature configurations
MICROPY_ROM_TEXT_COMPRESSION ?= 1

# Include py core make definitions.
include $(TOP)/py/py.mk

# Define toolchain and other tools.
CROSS_COMPILE ?= arm-none-eabi-

# Set CFLAGS.
CFLAGS += -I. -I$(TOP) -I$(BUILD)
CFLAGS += -Wall -Werror -std=c99 -nostdlib -Dgcc
CSUPEROPT = -Os # save some code space for performance-critical code
CFLAGS += -mthumb -mtune=cortex-m4 -mcpu=cortex-m4 -msoft-float
CSUPEROPT = -Os # save some code space for performance-critical code

# Select debugging or optimisation build.
ifeq ($(DEBUG), 1)
CFLAGS += -Og
else
CFLAGS += -Os -DNDEBUG
CFLAGS += -ffunction-sections -fdata-sections -fomit-frame-pointer
endif

# Set linker flags.
LDFLAGS += -nostdlib -T apollo3.ld -Map=$@.map --cref --gc-sections --entry=Reset_Handler

# Define the required source files.
SRC_C = \
	main.c \
	system.c \
    cpu.c \
	uart_core.c \
	shared/libc/printf.c \
	shared/readline/readline.c \
	shared/runtime/pyexec.c \
	shared/runtime/stdout_helpers.c \
    shared/libc/string0.c

SRC_QSTR += shared/readline/readline.c shared/runtime/pyexec.c

# Define the required object files.
OBJ += $(PY_CORE_O)
OBJ += $(addprefix $(BUILD)/, $(SRC_C:.c=.o))

# Define the top-level target, the main firmware.
all: $(BUILD)/firmware.bin

$(BUILD)/firmware.elf: $(OBJ)
	$(ECHO) "LINK $@"
	$(Q)$(LD) $(LDFLAGS) -o $@ $^
	$(Q)$(SIZE) $@

$(BUILD)/firmware.bin: $(BUILD)/firmware.elf
	$(ECHO) "Create $@"
	$(Q)$(OBJCOPY) -O binary -j .isr_vector -j .text -j .data $^ $@

# Include remaining core make rules.
include $(TOP)/py/mkrules.mk
