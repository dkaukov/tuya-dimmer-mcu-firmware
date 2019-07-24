#######
# makefile for STM8*_StdPeriph_Lib and SDCC compiler
#
# note: paths in this Makefile assume unmodified SPL folder structure
#
# usage:
#   1. if SDCC not in PATH set path -> CC_ROOT
#   2. set correct STM8 device -> DEVICE
#   3. set project paths -> PRJ_ROOT, PRJ_SRC_DIR, PRJ_INC_DIR
#   4. set SPL paths -> SPL_ROOT
#   5. add required SPL modules -> SPL_SOURCE
#   6. add required STM8S_EVAL modules -> EVAL_SOURCE, EVAL_128K_SOURCE, EVAL_COMM_SOURCE
#
#######

# STM8 device (for supported devices see stm8s.h)
DEVICE=STM8S103
DEVICE_FLASH=stm8s103f3

# trap handling requires SDCC >=v3.4.3
SKIP_TRAPS = 0

# set compiler path & parameters 
CC_ROOT =
CC      = sdcc
OBJCOPY = sdobjcopy
CFLAGS  = -mstm8 -lstm8 --opt-code-size 

## Extra optimization rules - use with care
CFLAGS  += --peep-file lib/extra.def
CFLAGS  += --stack-auto --noinduction --use-non-free

# set output folder and target name
OUTPUT_DIR = ./target
TARGET     = $(OUTPUT_DIR)/firmware-$(DEVICE).hex

# set project folder and files (all *.c)
PRJ_ROOT    = .
PRJ_SRC_DIR = $(PRJ_ROOT)/src
PRJ_INC_DIR = $(PRJ_ROOT)/src
PRJ_SOURCE  = $(notdir $(wildcard $(PRJ_SRC_DIR)/*.c))
PRJ_OBJECTS := $(addprefix $(OUTPUT_DIR)/, $(PRJ_SOURCE:.c=.rel))

# set SPL paths
SPL_ROOT = lib/STM8-SPL-SDCC
SPL_MAKE_DIR = $(SPL_ROOT)/Libraries/STM8S_StdPeriph_Driver
SPL_INC_DIR = $(SPL_ROOT)/Libraries/STM8S_StdPeriph_Driver/inc
SPL_LIB_DIR = $(SPL_ROOT)/Libraries/STM8S_StdPeriph_Driver/$(DEVICE)
SPL_LIB = spl.lib

# set PT paths
PT_ROOT = lib/pt
PT_INC_DIR = $(PT_ROOT)

# set path to STM8S_EVAL board routines
EVAL_DIR     = $(SPL_ROOT)/Utilities/STM8S_EVAL
EVAL_SOURCE  = 
EVAL_OBJECTS := $(addprefix $(OUTPUT_DIR)/, $(EVAL_SOURCE:.c=.rel))

# set path to STM8S_EVAL common routines
EVAL_COMM_DIR    = $(EVAL_DIR)/Common
EVAL_COMM_SOURCE  = 
EVAL_COMM_OBJECTS := $(addprefix $(OUTPUT_DIR)/, $(EVAL_COMM_SOURCE:.c=.rel))

# set path to STM8-128_EVAL board routines
EVAL_STM8S_128K_DIR     = $(EVAL_DIR)/STM8-128_EVAL
EVAL_STM8S_128K_SOURCE  = 
EVAL_STM8S_128K_OBJECTS := $(addprefix $(OUTPUT_DIR)/, $(EVAL_STM8S_128K_SOURCE:.c=.rel))

# set MCU_SDK paths
MCU_SDK_DIR = $(PRJ_ROOT)/sdk
MCU_SOURCE  = mcu_api.c protocol.c system.c
MCU_OBJECTS := $(addprefix $(OUTPUT_DIR)/, $(MCU_SOURCE:.c=.rel))

# collect all include folders
INCLUDE = -I$(PRJ_SRC_DIR) -I$(SPL_INC_DIR) -I$(EVAL_DIR) -I$(EVAL_COMM_DIR) -I$(EVAL_STM8S_128K_DIR) -I$(MCU_SDK_DIR) -I$(PT_INC_DIR)

# collect all source directories
VPATH=$(PRJ_SRC_DIR):$(SPL_SRC_DIR):$(EVAL_DIR):$(EVAL_COMM_DIR):$(EVAL_STM8S_128K_DIR):$(MCU_SDK_DIR)

.PHONY: clean

all: $(OUTPUT_DIR) $(TARGET) $(SPL_LIB)

$(SPL_LIB):
	$(MAKE) -C $(SPL_MAKE_DIR) DEVICE=$(DEVICE)

$(OUTPUT_DIR):
	mkdir -p $(OUTPUT_DIR)

$(OUTPUT_DIR)/%.rel: %.c
	$(CC) $(CFLAGS) -D$(DEVICE) $(INCLUDE) -DSKIP_TRAPS=$(SKIP_TRAPS) -c $?

$(OUTPUT_DIR)/%.rel: %.c
	$(CC) $(CFLAGS) -D$(DEVICE) $(INCLUDE) -DSKIP_TRAPS=$(SKIP_TRAPS) -c $? -o $@

$(TARGET): $(PRJ_OBJECTS) $(SPL_LIB) $(EVAL_OBJECTS) $(EVAL_COMM_OBJECTS) $(EVAL_STM8S_128K_OBJECTS) $(MCU_OBJECTS)
	$(CC) $(CFLAGS) -o $(TARGET) -L$(SPL_LIB_DIR) -l$(SPL_LIB) $^

flash: $(TARGET)
	sudo stm8flash -c stlinkv2 -p $(DEVICE_FLASH) -s flash -w $(TARGET)

clean: 
	rm -fr $(OUTPUT_DIR)
	$(MAKE) -C $(SPL_MAKE_DIR) clean DEVICE=$(DEVICE)

size:
	@$(OBJCOPY) -I ihex --output-target=binary $(TARGET) $(TARGET).bin
	@echo "----------"
	@echo "Image size:"
	@stat -L -c %s $(TARGET).bin