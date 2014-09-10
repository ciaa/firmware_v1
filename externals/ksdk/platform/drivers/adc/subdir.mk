ADC_DRIVER_DIR := $(SDK_ROOT)/platform/drivers/adc
SOURCES += $(ADC_DRIVER_DIR)/src/fsl_adc_driver.c \
           $(ADC_DRIVER_DIR)/common/fsl_adc_common.c
INCLUDES += $(ADC_DRIVER_DIR) \
            $(ADC_DRIVER_DIR)/common
