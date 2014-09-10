PIT_DRIVER_DIR := $(SDK_ROOT)/platform/drivers/pit
SOURCES += $(PIT_DRIVER_DIR)/src/fsl_pit_driver.c \
		   $(PIT_DRIVER_DIR)/src/fsl_pit_irq.c \
           $(PIT_DRIVER_DIR)/common/fsl_pit_common.c
INCLUDES += $(PIT_DRIVER_DIR) \
            $(PIT_DRIVER_DIR)/common
