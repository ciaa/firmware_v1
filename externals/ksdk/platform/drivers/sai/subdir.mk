SAI_DRIVER_DIR := $(SDK_ROOT)/platform/drivers/sai
SOURCES += $(wildcard $(SAI_DRIVER_DIR)/src/*.c) \
           $(wildcard $(SAI_DRIVER_DIR)/common/*.c)
INCLUDES += $(SAI_DRIVER_DIR) \
            $(SAI_DRIVER_DIR)/common \
            $(SDK_ROOT)/platform/drivers/soundcard
