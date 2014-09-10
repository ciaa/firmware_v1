LPTMR_DRIVER_DIR := $(SDK_ROOT)/platform/drivers/lptmr
SOURCES += $(LPTMR_DRIVER_DIR)/src/fsl_lptmr_driver.c \
           $(LPTMR_DRIVER_DIR)/common/fsl_lptmr_common.c
INCLUDES += $(LPTMR_DRIVER_DIR) \
            $(LPTMR_DRIVER_DIR)/common
