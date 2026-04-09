# ------------------------------------------------------------
# Project configuration
# ------------------------------------------------------------

TARGET = interrupt_gpio_driver

# Target CPU: STM32F446RE uses a Cortex-M4 core
MCU    = cortex-m4

# Hardware floating-point unit present on STM32F446RE
FPU    = fpv4-sp-d16
FLOAT_ABI = hard

# ------------------------------------------------------------
# Toolchain
# ------------------------------------------------------------

CC      = arm-none-eabi-gcc
OBJCOPY = arm-none-eabi-objcopy
SIZE    = arm-none-eabi-size

# ------------------------------------------------------------
# Build output directory
# ------------------------------------------------------------

OUT_DIR = out

# ------------------------------------------------------------
# Compiler flags
# ------------------------------------------------------------

CFLAGS  = -mcpu=$(MCU) -mthumb -mfpu=$(FPU) -mfloat-abi=$(FLOAT_ABI) \
          -O2 -ffreestanding -fno-builtin -Wall -Wextra

CFLAGS += -g3 -Og

# ------------------------------------------------------------
# Linker flags
# ------------------------------------------------------------

LDFLAGS = -T linker.ld -nostdlib -Wl,--gc-sections

# ------------------------------------------------------------
# Source files
# ------------------------------------------------------------

SRCS = \
src/main.c \
src/driver.c \
startup.s

# Convert source files to object files in OUT_DIR
OBJS = $(SRCS:%.c=$(OUT_DIR)/%.o)
OBJS := $(OBJS:%.s=$(OUT_DIR)/%.o)

# ------------------------------------------------------------
# Default build target
# ------------------------------------------------------------

all: $(OUT_DIR) $(OUT_DIR)/$(TARGET).elf $(OUT_DIR)/$(TARGET).bin size

# ------------------------------------------------------------
# Compile rules
# ------------------------------------------------------------

# Compile C source files
$(OUT_DIR)/%.o: %.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# Assemble startup code
$(OUT_DIR)/%.o: %.s
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# ------------------------------------------------------------
# Link ELF
# ------------------------------------------------------------

$(OUT_DIR)/$(TARGET).elf: $(OBJS) linker.ld
	$(CC) $(CFLAGS) $(OBJS) $(LDFLAGS) -o $@

# ------------------------------------------------------------
# Convert ELF → raw binary
# ------------------------------------------------------------

$(OUT_DIR)/$(TARGET).bin: $(OUT_DIR)/$(TARGET).elf
	$(OBJCOPY) -O binary $< $@

# ------------------------------------------------------------
# Size information
# ------------------------------------------------------------

# Prints flash/RAM usage
size: $(OUT_DIR)/$(TARGET).elf
	$(SIZE) $<

# ------------------------------------------------------------
# Flash to target
# ------------------------------------------------------------

flash: $(OUT_DIR)/$(TARGET).elf
	openocd -f interface/stlink.cfg -f target/stm32f4x.cfg \
		-c "program $< verify reset exit"

# ------------------------------------------------------------
# Utility targets
# ------------------------------------------------------------

# Create output directory if it doesn't exist
$(OUT_DIR):
	mkdir -p $(OUT_DIR)

# Remove all generated files
clean:
	rm -rf $(OUT_DIR)
