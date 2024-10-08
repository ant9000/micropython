# Include the core environment definitions; this will set $(TOP).
include ../../py/mkenv.mk

# qstr definitions (must come before including py.mk)
QSTR_DEFS = qstrdefsport.h

# MicroPython feature configurations
MICROPY_ROM_TEXT_COMPRESSION ?= 1

# Include py core make definitions.
include $(TOP)/py/py.mk
include $(TOP)/extmod/extmod.mk

JLINK ?= JLinkExe
# Define toolchain and other tools.
CROSS_COMPILE ?= arm-none-eabi-

DEF  = -DAM_PART_APOLLO3
DEF += -DAM_PACKAGE_BGA
DEF += -Dgcc

INC  = -I.
INC += -I$(TOP)
INC += -I$(BUILD)
INC += -I$(TOP)/lib/cmsis/inc
INC += -Iambiq/mcu/apollo3
INC += -Iambiq/devices
INC += -Iambiq/utils
INC += -Iambiq/boards/apollo3_evb/bsp
INC += -Iambiq/CMSIS/AmbiqMicro/Include

# Set CFLAGS.
CFLAGS += $(DEF) $(INC) -Wall -std=c99 -nostdlib
CFLAGS += -mthumb -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard
CFLAGS += -ffunction-sections -fdata-sections -fomit-frame-pointer
CSUPEROPT = -Os # save some code space for performance-critical code

# Select debugging or optimisation build.
ifeq ($(DEBUG), 1)
CFLAGS += -g
else
CFLAGS += -Os -DNDEBUG
endif

# Set linker flags.
LDFLAGS += -nostdlib -T apollo3.ld -Map=$@.map --cref --gc-sections --entry=Reset_Handler

# Define the required source files.
SRC_C += \
    main.c \
    system.c \
    cpu.c \
    gccollect.c \
    mphalport.c \
    help.c \
    modules/machine/led.c \
    modules/machine/switch.c \
    shared/libc/printf.c \
    shared/readline/readline.c \
    shared/runtime/pyexec.c \
    shared/runtime/stdout_helpers.c \
    shared/libc/string0.c \
    shared/timeutils/timeutils.c \
    shared/runtime/sys_stdio_mphal.c \
    $(wildcard ambiq/mcu/apollo3/hal/*.c) \
    $(wildcard ambiq/boards/apollo3_evb/bsp/*.c) \
    ambiq/utils/am_util_stdio.c \
    ambiq/utils/am_util_delay.c \
    ambiq/devices/am_devices_led.c \
    ambiq/devices/am_devices_button.c

#   ambiq/CMSIS/AmbiqMicro/Source/
#   ambiq/utils/
#   ambiq/devices/

# Disable warnings for Ambiq SDK files: am_hal_ble
$(BUILD)/ambiq/mcu/apollo3/hal/am_hal_ble.o: CFLAGS += -Wno-maybe-uninitialized

# Math library:
SRC_C += $(SRC_LIB_LIBM_C)
SRC_C += $(SRC_LIB_LIBM_SQRT_HW_C)

# List of sources for qstr extraction
SRC_QSTR += $(SRC_C)

LIBS = $(shell $(CC) $(CFLAGS) -print-libgcc-file-name)

# Define the required object files.
OBJ += $(PY_O)
OBJ += $(addprefix $(BUILD)/, $(SRC_C:.c=.o))

define RUN_JLINK
    $(ECHO) "Writing $(1) to the board via JLink"
    $(Q)$(JLINK) -if swd -device AMA3B1KK-KBR -speed 4000 -CommandFile apollo3.jlink
endef

.PHONY: deploy

# Define the top-level target, the main firmware.
all: $(BUILD)/firmware.bin

testsrc:
	echo "$(SRC_C)"

$(BUILD)/firmware.elf: $(OBJ)
	$(ECHO) "LINK $@"
	$(Q)$(LD) $(LDFLAGS) -o $@ $^ $(LIBS)
	$(Q)$(SIZE) $@

$(BUILD)/firmware.bin: $(BUILD)/firmware.elf
	$(ECHO) "Create $@"
	$(Q)$(OBJCOPY) -O binary -j .isr_vector -j .text -j .data $^ $@

deploy: $(BUILD)/firmware.bin
	$(call RUN_JLINK,$(word 1,$^))

# Include remaining core make rules.
include $(TOP)/py/mkrules.mk
