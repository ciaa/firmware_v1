WDOG_DRIVER_DIR := $(SDK_ROOT)/platform/drivers/wdog
SOURCES += $(wildcard $(WDOG_DRIVER_DIR)/src/*.c) \
           $(wildcard $(WDOG_DRIVER_DIR)/common/*.c)
INCLUDES += $(WDOG_DRIVER_DIR) \
            $(WDOG_DRIVER_DIR)/common
