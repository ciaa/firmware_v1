UART_HAL_DIR := $(SDK_ROOT)/platform/hal/uart
SOURCES += $(wildcard $(UART_HAL_DIR)/*.c)
INCLUDES += $(UART_HAL_DIR)
