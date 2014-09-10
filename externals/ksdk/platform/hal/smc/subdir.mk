SMC_HAL_DIR := $(SDK_ROOT)/platform/hal/smc
SOURCES += $(wildcard $(SMC_HAL_DIR)/*.c)
INCLUDES += $(SMC_HAL_DIR)
