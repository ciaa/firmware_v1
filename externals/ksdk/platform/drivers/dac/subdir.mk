DAC_DRIVER_DIR := $(SDK_ROOT)/platform/drivers/dac
SOURCES += $(DAC_DRIVER_DIR)/src/fsl_dac_driver.c \
           $(DAC_DRIVER_DIR)/common/fsl_dac_common.c
INCLUDES += $(DAC_DRIVER_DIR) \
            $(DAC_DRIVER_DIR)/common
