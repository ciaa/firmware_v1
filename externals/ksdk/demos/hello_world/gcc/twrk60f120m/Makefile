#-----------------------------------------------
# Make command:
# mingw32-make build=<build> target=<target> stack=0x1000 heap=0x1000
# <build>: debug or release, release by default.
# <target>: sram or flash, flash by default.
#-----------------------------------------------

#-----------------------------------------------
# setup variables
# ----------------------------------------------

SDK_ROOT := $(abspath ../../../..)

#-----------------------------------------------
# set board infomation
# The CHIP must be set correctly, otherwise the
# platform_lib cannot be linked.
# CPU type: cortex-m0, cortex-m0plus,
# cortex-m1, cortex-m3, cortex-m4, 
#-----------------------------------------------
BOARD := twrk60f120m
CHIP := K60F12
CPU := cortex-m4

#-----------------------------------------------
# Choose the float point method.
# cortex-m4 can use: NO_FP, SOFT_FP and HARD_FP.
# Other cortex-m series should only use NO_FP.
# Application's float point configuration must
# be the same with the libraries it linked.
#-----------------------------------------------
CHOOSE_FLOAT = HARD_FP

APP_NAME = hello_world
#-----------------------------------------------
# Debug or Release
# Release by default
#-----------------------------------------------
build ?= release

#-----------------------------------------------
# Sram target or flash target
# flash target by defult.
#-----------------------------------------------
target ?= flash

include $(SDK_ROOT)/mk/common.mk

#-----------------------------------------------
# Link libraries
#-----------------------------------------------
ifeq "$(build)" "debug"
LINK_LIB_NAME := $(SDK_ROOT)/lib/ksdk_platform_lib/gcc/$(CHIP)/Debug/ksdk_platform_lib.a
else
LINK_LIB_NAME := $(SDK_ROOT)/lib/ksdk_platform_lib/gcc/$(CHIP)/Release/ksdk_platform_lib.a
endif

#-----------------------------------------------
# Include path. Add the include paths like this:
# INCLUDES += ./include/
#-----------------------------------------------
INCLUDES += $(SDK_ROOT)/platform/startup/M$(CHIP)     \
            $(SDK_ROOT)/boards/$(BOARD)               \
			../../src

include $(SDK_ROOT)/demos/common_include.mk

#-----------------------------------------------
# Startup files
#-----------------------------------------------
STARTUP_ROOT := $(SDK_ROOT)/platform/startup
STARTUP_SOURCES := $(STARTUP_ROOT)/startup.c \
                   $(STARTUP_ROOT)/M$(CHIP)/system_M$(CHIP).c \
                   $(STARTUP_ROOT)/M$(CHIP)/gcc/startup_M$(CHIP).S

#-----------------------------------------------
# Application files
#-----------------------------------------------
APP_SOURCES  := $(wildcard $(APPS_ROOT)/$(APP_NAME)/src/*.c)

#-----------------------------------------------
# Board related files, e.g. platform_pins.c
#-----------------------------------------------
BOARD_SOURCES := $(wildcard $(BOARD_ROOT)/*.c)

#-----------------------------------------------
# Add all sources needed here
#-----------------------------------------------
SOURCES = $(STARTUP_SOURCES) $(APP_SOURCES) $(BOARD_SOURCES) \
		  $(SDK_ROOT)/platform/utilities/src/fsl_debug_console.c \
		  $(SDK_ROOT)/platform/utilities/src/fsl_misc_utilities.c

#-----------------------------------------------
# Add all pre-defined symbols here
#-----------------------------------------------
DEFINES += -DCPU_MK60FN1M0VLQ12 \
		   -DTWR_K60F120M \
           -DTOWER

#-----------------------------------------------
# Link file
#-----------------------------------------------
ifeq "$(target)" "sram"
LD_FILE_NAME := $(LD_FILE_ROOT)/$(CHIP)/K60FN1Mxxx12_ram.ld
else
LD_FILE_NAME := $(LD_FILE_ROOT)/$(CHIP)/K60FN1Mxxx12_flash.ld
endif

#-----------------------------------------------
# Define the stack size and heap size.
#-----------------------------------------------
stack ?= 0x1000
heap  ?= 0x1000
DEFINES += -D__STACK_SIZE=$(stack) \
           -D__HEAP_SIZE=$(heap)

include $(SDK_ROOT)/mk/targets.mk 
