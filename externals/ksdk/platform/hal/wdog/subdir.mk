WDOG_HAL_DIR := $(SDK_ROOT)/platform/hal/wdog
SOURCES += $(wildcard $(WDOG_HAL_DIR)/*.c)
INCLUDES += $(WDOG_HAL_DIR)
