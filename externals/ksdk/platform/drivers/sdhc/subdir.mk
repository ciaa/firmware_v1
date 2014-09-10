SDHC_DRIVER_DIR := $(SDK_ROOT)/platform/drivers/sdhc
SOURCES += $(wildcard $(SDHC_DRIVER_DIR)/src/*.c) \
           $(wildcard $(SDHC_DRIVER_DIR)/common/*.c)
INCLUDES += $(SDHC_DRIVER_DIR) \
            $(SDHC_DRIVER_DIR)/common
