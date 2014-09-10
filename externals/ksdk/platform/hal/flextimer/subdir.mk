FTM_HAL_DIR := $(SDK_ROOT)/platform/hal/flextimer
SOURCES += $(wildcard $(FTM_HAL_DIR)/*.c)
INCLUDES += $(FTM_HAL_DIR)
