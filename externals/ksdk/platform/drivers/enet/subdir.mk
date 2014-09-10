ENET_DRIVER_DIR := $(SDK_ROOT)/platform/drivers/enet
SOURCES += $(ENET_DRIVER_DIR)/src/fsl_enet_driver.c \
           $(ENET_DRIVER_DIR)/common/fsl_enet_common.c
INCLUDES += $(ENET_DRIVER_DIR) \
            $(ENET_DRIVER_DIR)/common

