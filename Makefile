# Assignment 1 -- Bare Metal Raspberry Pi
#
#   make power_blink   cross-compile power_blink.c -> kernel7l.img   (Task 3)
#   make led_blink     cross-compile led_blink.c   -> kernel7l.img   (Task 4)
#   make led_fade      cross-compile led_fade.c    -> kernel7l.img   (Bonus)
#   make all           build all three into build/ without touching kernel7l.img
#   make clean         remove build artifacts
#
# kernel7l.img is the file you copy onto the SD card. Only one program can be on the
# card at a time, so each target overwrites kernel7l.img -- that is intentional.
#
# On Windows the binaries are named e.g. arm-none-eabi-gcc-13.3.1.exe. Override the
# prefix/suffix if `make` cannot find them:
#   make led_blink CROSS=arm-none-eabi- SUFFIX=-13.3.1.exe

CROSS   ?= arm-none-eabi-
SUFFIX  ?=

CC      := $(CROSS)gcc$(SUFFIX)
OBJCOPY := $(CROSS)objcopy$(SUFFIX)

# These flags target the Raspberry Pi 400's BCM2711 (Cortex-A72) in 32-bit mode,
# with no C runtime startup files and no optimisation.
CFLAGS  := -g -nostartfiles -mfloat-abi=hard -O0 -DRPI4 \
           -mfpu=crypto-neon-fp-armv8 -march=armv8-a+crc -mcpu=cortex-a72

KERNEL  := kernel7l.img
BUILD   := build
PROGRAMS := power_blink led_blink led_fade

.PHONY: all clean toolchain $(PROGRAMS)

# Build each program into build/ -- used by CI, leaves kernel7l.img alone.
all: $(addprefix $(BUILD)/,$(addsuffix .img,$(PROGRAMS)))

$(BUILD):
	@mkdir -p $(BUILD)

$(BUILD)/%.elf: %.c | $(BUILD)
	$(CC) $(CFLAGS) $< -o $@

$(BUILD)/%.img: $(BUILD)/%.elf
	$(OBJCOPY) $< -O binary $@

# `make led_blink` etc.: build it, then put it where the SD card expects it.
$(PROGRAMS): %: $(BUILD)/%.img
	@cp $(BUILD)/$@.img $(KERNEL)
	@echo
	@echo "  Built $(KERNEL) from $@.c"
	@echo "  Copy it to the root of your FAT32 SD card, eject, and boot the Pi."
	@echo

toolchain:
	@$(CC) --version || (echo "arm-none-eabi-gcc not found -- see README.md"; exit 1)
	@$(OBJCOPY) --version

clean:
	rm -rf $(BUILD) $(KERNEL) *.elf

.PRECIOUS: $(BUILD)/%.elf
