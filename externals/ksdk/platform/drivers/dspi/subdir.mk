DSPI_DRIVER_DIR := $(SDK_ROOT)/platform/drivers/dspi
SOURCES += $(wildcard $(DSPI_DRIVER_DIR)/common/*.c) \
           $(wildcard $(DSPI_DRIVER_DIR)/dspi_master/src/*.c) \
           $(wildcard $(DSPI_DRIVER_DIR)/dspi_slave/src/*.c)
INCLUDES += $(DSPI_DRIVER_DIR)/common \
            $(DSPI_DRIVER_DIR)/dspi_master \
            $(DSPI_DRIVER_DIR)/dspi_slave
