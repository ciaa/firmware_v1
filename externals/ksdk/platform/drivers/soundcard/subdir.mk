SOUNDCARD_DRIVER_DIR := $(SDK_ROOT)/platform/drivers/soundcard
SOURCES += $(wildcard $(SAI_DRIVER_DIR)/src/*.c)
INCLUDES += $(SOUNDCARD_DRIVER_DIR)
