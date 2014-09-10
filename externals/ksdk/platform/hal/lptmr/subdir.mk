LPTMR_HAL_DIR := $(SDK_ROOT)/platform/hal/lptmr
SOURCES += $(wildcard $(LPTMR_HAL_DIR)/*.c)
INCLUDES += $(LPTMR_HAL_DIR)
