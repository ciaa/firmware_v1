ENET_HAL_DIR := $(SDK_ROOT)/platform/hal/enet
SOURCES += $(wildcard $(ENET_HAL_DIR)/*.c)
INCLUDES += $(ENET_HAL_DIR)
