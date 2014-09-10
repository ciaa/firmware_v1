LPUART_DRIVER_DIR := $(SDK_ROOT)/platform/drivers/lpuart
SOURCES += $(LPUART_DRIVER_DIR)/src/fsl_lpuart_driver.c \
           $(LPUART_DRIVER_DIR)/common/fsl_lpuart_common.c
INCLUDES += $(LPUART_DRIVER_DIR) \
            $(LPUART_DRIVER_DIR)/common
