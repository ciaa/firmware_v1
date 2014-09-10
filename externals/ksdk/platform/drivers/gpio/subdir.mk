GPIO_DRIVER_DIR := $(SDK_ROOT)/platform/drivers/gpio
SOURCES += $(wildcard $(GPIO_DRIVER_DIR)/src/*.c) \
		   $(GPIO_DRIVER_DIR)/common/fsl_gpio_common.c
INCLUDES += $(GPIO_DRIVER_DIR) \
			$(GPIO_DRIVER_DIR)/common

