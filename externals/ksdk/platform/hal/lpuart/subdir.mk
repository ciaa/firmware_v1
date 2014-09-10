LPUART_HAL_DIR := $(SDK_ROOT)/platform/hal/lpuart
SOURCES += $(wildcard $(LPUART_HAL_DIR)/*.c)
INCLUDES += $(LPUART_HAL_DIR)
