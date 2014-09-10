EDMA_HAL_DIR := $(SDK_ROOT)/platform/hal/edma
SOURCES += $(wildcard $(EDMA_HAL_DIR)/*.c)
INCLUDES += $(EDMA_HAL_DIR)
