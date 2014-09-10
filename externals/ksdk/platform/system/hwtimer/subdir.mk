HWTIMER_DRIVER_DIR := $(SDK_ROOT)/platform/system/hwtimer
SOURCES += $(wildcard $(HWTIMER_DRIVER_DIR)/src/*.c)
INCLUDES += $(HWTIMER_DRIVER_DIR)
