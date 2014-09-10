EDMA_DRIVER_DIR := $(SDK_ROOT)/platform/drivers/edma
SOURCES += $(EDMA_DRIVER_DIR)/src/fsl_edma_driver.c \
           $(EDMA_DRIVER_DIR)/common/fsl_edma_common.c
INCLUDES += $(EDMA_DRIVER_DIR) \
            $(EDMA_DRIVER_DIR)/common
