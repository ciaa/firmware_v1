UART_DRIVER_DIR := $(SDK_ROOT)/platform/drivers/uart
SOURCES += $(wildcard $(UART_DRIVER_DIR)/src/*.c) \
           $(wildcard $(UART_DRIVER_DIR)/common/*.c)
INCLUDES += $(UART_DRIVER_DIR) \
            $(UART_DRIVER_DIR)/common
