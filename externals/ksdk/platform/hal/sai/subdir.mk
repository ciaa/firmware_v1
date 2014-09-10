SAI_HAL_DIR := $(SDK_ROOT)/platform/hal/sai
SOURCES += $(wildcard $(SAI_HAL_DIR)/*.c)
INCLUDES += $(SAI_HAL_DIR)
