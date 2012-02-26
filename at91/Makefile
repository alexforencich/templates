# ----------------------------------------------------------------------------
#         ATMEL Microcontroller Software Support  -  SDC  -
# ----------------------------------------------------------------------------
# Copyright (c) 2006, Atmel Corporation
#
# All rights reserved.
# 
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
# 
# - Redistributions of source code must retain the above copyright notice,
# this list of conditions and the disclaimer below.
# 
# - Redistributions in binary form must reproduce the above copyright notice,
# this list of conditions and the disclaimer below in the documentation and/or
# other materials provided with the distribution. 
# 
# Atmel's name may not be used to endorse or promote products derived from
# this software without specific prior written permission. 
# 
# DISCLAIMER: THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR
# IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
# MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
# DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR ANY DIRECT, INDIRECT,
# INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
# LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
# OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
# LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
# NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
# EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
# ---------------------------------------------------------------------------- 

# 	Makefile for compiling the Getting Started project

#-------------------------------------------------------------------------------
#		User-modifiable options
#-------------------------------------------------------------------------------

# Chip & board used for compilation
# (can be overriden by adding CHIP=chip and BOARD=board to the command-line)
CHIP = at91sam7s256
BOARD = Template

# Optimization level, put in comment for debugging
#OPTIMIZATION = -Os

# AT91 library directory
AT91LIB = at91lib

# Board directory
#BOARDDIR = $(AT91LIB)/boards/$(BOARD)
BOARDDIR = board

# Output file basename
OUTPUT = main-$(CHIP)

# Output directories
BIN = bin
OBJ = obj
DEP = dep

# Libraries
LIBS = -nostartfiles -lm -lc -lgcc

# Objects built from C source files
SRC = main.c
SRC += div0.c
#src += led.c
SRC += dbgu.c pio.c aic.c pio_it.c pit.c
SRC += board_memories.c board_lowlevel.c
SRC += timer.c hook.c usart.c
#SRC += ...

# Objects built from Assembly source files
ASRC = board_cstartup.S _udivsi3.S
#ASRC += ...

#-------------------------------------------------------------------------------
#		Tools
#-------------------------------------------------------------------------------

# Tool suffix when cross-compiling
CROSS = arm-elf-

# Compilation tools
CC = $(CROSS)gcc
AR = $(CROSS)ar
AS = $(CROSS)as
LD = $(CROSS)ld
SIZE = $(CROSS)size
STRIP = $(CROSS)strip
OBJCOPY = $(CROSS)objcopy
OBJDUMP = $(CROSS)objdump

# Flags
INCLUDES = -I$(BOARDDIR)
INCLUDES += -I$(AT91LIB)/chips
INCLUDES += -I$(AT91LIB)/peripherals
INCLUDES += -I$(AT91LIB)

# Defines
DEFS =

# Compiler flags to generate dependency files.
GENDEPFLAGS = -MD -MP -MF $(DEP)/$(@F).d

# Common Flags
COMMON_FLAGS = -mlong-calls -ffunction-sections
COMMON_FLAGS += -g $(OPTIMIZATION) $(INCLUDES) -D$(CHIP) $(DEFS)
COMMON_FLAGS += -Wall
COMMON_FLAGS += -Wa,-adhlns=$(basename $@).lst
COMMON_FLAGS += $(GENDEPFLAGS)

# C Specific flags
CFLAGS = $(COMMON_FLAGS)

# Assember flags
ASFLAGS = -g $(OPTIMIZATION) $(INCLUDES) -D$(CHIP) -D__ASSEMBLY__ $(DEFS)
ASFLAGS += -Wa,-adhlns=$(basename $@).lst

# Linker flags
LDFLAGS = -g $(OPTIMIZATION) -Wl,--gc-sections $(LIBS)

#-------------------------------------------------------------------------------
#		Files
#-------------------------------------------------------------------------------

# Directories where source files can be found
UTILITY = $(AT91LIB)/utility
PERIPH = $(AT91LIB)/peripherals
#BOARDS = $(AT91LIB)/boards
CHIPS = $(AT91LIB)/chips

VPATH += $(UTILITY)
VPATH += $(PERIPH)/dbgu $(PERIPH)/aic $(PERIPH)/pio $(PERIPH)/pit
VPATH += $(BOARDDIR)
VPATH += $(CHIPS)/$(CHIP)

# Append output directories to files
CSRC := $(filter %.c, $(SRC))
C_OBJECTS := $(addprefix $(OBJ)/, $(addsuffix .o,$(basename $(CSRC))))
ASM_OBJECTS := $(addprefix $(OBJ)/, $(addsuffix .o,$(basename $(ASRC))))
OUTPUT := $(addprefix $(BIN)/, $(OUTPUT))

#-------------------------------------------------------------------------------
#		JTAG
#-------------------------------------------------------------------------------

JTAG_INTERFACE = arm-usb-ocd
#JTAG_INTERFACE = usb-jtag-a1

JTAG_TARGET = flash
#JTAG_TARGET = debug

OPENOCD = openocd

JTAG_CFG = openocd-$(JTAG_TARGET).cfg

JTAG_FILE = $(OUTPUT)-$(JTAG_TARGET).bin

#-------------------------------------------------------------------------------
#		Rules
#-------------------------------------------------------------------------------

# Get the list of available targets from the board.mak file
include $(BOARDDIR)/board.mak

all: $(MEMORIES) size

program: all
	echo set FILENAME ../$(JTAG_FILE) > openocd/filename.cfg
	cd openocd; $(OPENOCD) -f interface/$(JTAG_INTERFACE).cfg -f $(JTAG_CFG)

$(BIN) $(OBJ):
	mkdir $@

$(MEMORIES): %: $(OUTPUT)-%.elf $(OUTPUT)-%.bin $(OUTPUT)-%.lss

.SECONDARY : %.elf
.PRECIOUS : $(ASM_OBJECTS) $(C_OBJECTS)
%.elf: $(ASM_OBJECTS) $(C_OBJECTS) | $(BIN)
	$(CC) $(LDFLAGS) -T"$(CHIPS)/$(CHIP)/$(patsubst $(OUTPUT)-%.elf,%,$@).lds" -Wl,-Map=$(basename $@).map,--cref -o $@ $^

%.bin: %.elf
	$(OBJCOPY) -O binary $< $@

%.lss: %.elf
	$(OBJDUMP) -S $< > $@

size: $(MEMORIES)
	$(SIZE) $(ASM_OBJECTS) $(C_OBJECTS) $(OUTPUT)-*.elf

$(OBJ)/%.o: %.c Makefile | $(OBJ)
	$(CC) $(CFLAGS) -c -o $@ $<

$(OBJ)/%.o: %.S Makefile | $(OBJ)
	$(CC) $(ASFLAGS) -c -o $@ $<

clean:
	rm -f $(OBJ)/*.o
	rm -f $(BIN)/*.bin
	rm -f $(BIN)/*.elf
	rm -f $(BIN)/*.lst
	rm -f $(DEP)/*

# Include the dependency files.
-include $(shell mkdir $(DEP) 2>/dev/null) $(wildcard $(DEP)/*)
