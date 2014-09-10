SDCARD_DRIVER_DIR := $(SDK_ROOT)/platform/drivers/sdcard
SOURCES += $(wildcard $(SDCARD_DRIVER_DIR)/src/*.c)
INCLUDES += $(SDCARD_DRIVER_DIR)
