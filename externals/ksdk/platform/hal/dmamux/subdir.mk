DMAMUX_HAL_DIR := $(SDK_ROOT)/platform/hal/dmamux
SOURCES += $(wildcard $(DMAMUX_HAL_DIR)/*.c)
INCLUDES += $(DMAMUX_HAL_DIR)
