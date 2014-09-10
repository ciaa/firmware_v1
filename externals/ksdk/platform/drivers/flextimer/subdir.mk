FTM_DRIVER_DIR := $(SDK_ROOT)/platform/drivers/flextimer
SOURCES += $(FTM_DRIVER_DIR)/src/fsl_ftm_driver.c \
           $(FTM_DRIVER_DIR)/common/fsl_ftm_common.c
INCLUDES += $(FTM_DRIVER_DIR) \
            $(FTM_DRIVER_DIR)/common
