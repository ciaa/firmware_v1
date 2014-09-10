CLOCK_DRIVER_DIR := $(SDK_ROOT)/platform/system/clock
SOURCES += $(CLOCK_DRIVER_DIR)/fsl_clock_manager.c \
           $(CLOCK_DRIVER_DIR)/M$(CHIP)/fsl_clock_$(CHIP).c
INCLUDES += $(CLOCK_DRIVER_DIR) \
            $(CLOCK_DRIVER_DIR)/M$(CHIP)

