ADC_HAL_DIR := $(SDK_ROOT)/platform/hal/adc
SOURCES += $(wildcard $(ADC_HAL_DIR)/*.c)
INCLUDES += $(ADC_HAL_DIR)
