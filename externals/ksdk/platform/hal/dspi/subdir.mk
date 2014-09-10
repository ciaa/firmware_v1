DSPI_HAL_DIR := $(SDK_ROOT)/platform/hal/dspi
SOURCES += $(wildcard $(DSPI_HAL_DIR)/*.c)
INCLUDES += $(DSPI_HAL_DIR)
