CAN_HAL_DIR := $(SDK_ROOT)/platform/hal/can
SOURCES += $(wildcard $(CAN_HAL_DIR)/*.c)
INCLUDES += $(CAN_HAL_DIR)
