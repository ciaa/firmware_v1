CAN_DRIVER_DIR := $(SDK_ROOT)/platform/drivers/can
SOURCES += $(wildcard $(CAN_DRIVER_DIR)/src/*.c) \
           $(CAN_DRIVER_DIR)/common/fsl_flexcan_common.c
INCLUDES += $(CAN_DRIVER_DIR) \
            $(CAN_DRIVER_DIR)/common
