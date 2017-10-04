#######################################
# Toolchain
#######################################

BINPATH = ~/opt/gcc-arm-none-eabi-6-2017-q2-update/bin/
PREFIX = arm-none-eabi-
CC = $(BINPATH)/$(PREFIX)gcc
AS = $(BINPATH)/$(PREFIX)gcc -x assembler-with-cpp
CP = $(BINPATH)/$(PREFIX)objcopy
AR = $(BINPATH)/$(PREFIX)ar
SZ = $(BINPATH)/$(PREFIX)size
HEX = $(CP) -O ihex
BIN = $(CP) -O binary -S

######################################
# Configuration
######################################

# Target name
TARGET = myProgram
# Build path
BUILD_DIR = bin

# CPU flags
CPU = -mcpu=cortex-m0
FPU = 
FLOAT-ABI =
MCU = $(CPU) -mthumb $(FPU) $(FLOAT-ABI)

# Libraries
LIBS = -lc -lm -lnosys

# Debug build?
DEBUG = 1
# Optimization
OPT = -Og

######################################
# Definitions
######################################

# AS defines
AS_DEFS = 

# C defines
C_DEFS =	-DUSE_HAL_DRIVER \
			-DSTM32F072xB

######################################
# Includes
######################################

# AS includes
AS_INCLUDES = 

# C includes
C_INCLUDES =	-Iinc\
				-Ithirdparty/CMSIS/Device/ST/STM32F0xx/Include \
				-Ithirdparty/CMSIS/Include\
				-Ithirdparty/STM32F0xx_HAL_Driver/Inc \
				-Ithirdparty/STM32F0xx_HAL_Driver/Inc/Legacy

######################################
# Source
######################################

# ASM sources
ASM_SOURCES =	src/startup_stm32f072xb.s

# C sources
C_SOURCES +=	src/main.c\
				src/system_stm32f0xx.c\
				src/console/console.c\
				src/led/led.c

# Peripheral library
C_SOURCES +=	thirdparty/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_uart_ex.c \
				thirdparty/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_rcc_ex.c \
				thirdparty/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_pwr.c \
				thirdparty/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_flash.c \
				thirdparty/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal.c \
				thirdparty/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_i2c.c \
				thirdparty/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_rcc.c \
				thirdparty/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_i2c_ex.c \
				thirdparty/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_uart.c \
				thirdparty/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_flash_ex.c \
				thirdparty/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_tim.c \
				thirdparty/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_tim_ex.c \
				thirdparty/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_pwr_ex.c \
				thirdparty/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_gpio.c \
				thirdparty/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_dma.c \
				thirdparty/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_cortex.c

# Link script
LDSCRIPT =		ld/STM32F072RBTx_FLASH.ld
 
#######################################
# FLAGS
#######################################

# GCC AS flags
ASFLAGS = $(MCU) $(AS_DEFS) $(AS_INCLUDES) $(OPT) -Wall -fdata-sections -ffunction-sections

# GCC C flags
CFLAGS = $(MCU) $(C_DEFS) $(C_INCLUDES) $(OPT) -Wall -fdata-sections -ffunction-sections
# Debug flags
ifeq ($(DEBUG), 1)
CFLAGS += -g -gdwarf-2
endif
# Generate dependency information
CFLAGS += -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)"

# Linker flags
LDFLAGS = $(MCU) -specs=nano.specs -T$(LDSCRIPT) $(LIBDIR) $(LIBS) -Wl,-Map=$(BUILD_DIR)/$(TARGET).map,--cref -Wl,--gc-sections

#######################################
# Rules and dependencies
#######################################

# Default action: build all
all: $(BUILD_DIR)/$(TARGET).elf $(BUILD_DIR)/$(TARGET).hex $(BUILD_DIR)/$(TARGET).bin doc

doc: Makefile | $(BUILD_DIR)
	mkdir -p doc
	(cd conf/doxygen/ && doxygen)

flash: Makefile | all
	JLinkExe -commanderscript conf/jlink/download.jlink

erase:
	JLinkExe -commanderscript conf/jlink/erase.jlink


#######################################
# Build the application
#######################################

# list of objects
OBJECTS = $(addprefix $(BUILD_DIR)/,$(notdir $(C_SOURCES:.c=.o)))
vpath %.c $(sort $(dir $(C_SOURCES)))
# list of ASM program objects
OBJECTS += $(addprefix $(BUILD_DIR)/,$(notdir $(ASM_SOURCES:.s=.o)))
vpath %.s $(sort $(dir $(ASM_SOURCES)))

$(BUILD_DIR)/%.o: %.c Makefile | $(BUILD_DIR)
	$(CC) -c $(CFLAGS) -Wa,-a,-ad,-alms=$(BUILD_DIR)/$(notdir $(<:.c=.lst)) $< -o $@

$(BUILD_DIR)/%.o: %.s Makefile | $(BUILD_DIR)
	$(AS) -c $(CFLAGS) $< -o $@

$(BUILD_DIR)/$(TARGET).elf: $(OBJECTS) Makefile
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@
	$(SZ) $@

$(BUILD_DIR)/%.hex: $(BUILD_DIR)/%.elf | $(BUILD_DIR)
	$(HEX) $< $@
	
$(BUILD_DIR)/%.bin: $(BUILD_DIR)/%.elf | $(BUILD_DIR)
	$(BIN) $< $@	
	
$(BUILD_DIR):
	mkdir $@

#######################################
# Clean up
#######################################
clean:
	-rm -fR .dep $(BUILD_DIR) doc
  
#######################################
# Dependencies
#######################################
-include $(shell mkdir .dep 2>/dev/null) $(wildcard .dep/*)

# *** EOF ***