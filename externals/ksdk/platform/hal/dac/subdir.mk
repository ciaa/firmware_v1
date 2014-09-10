DAC_HAL_DIR := $(SDK_ROOT)/platform/hal/dac
SOURCES += $(wildcard $(DAC_HAL_DIR)/*.c)
INCLUDES += $(DAC_HAL_DIR)
