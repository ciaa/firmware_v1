I2C_DRIVER_DIR := $(SDK_ROOT)/platform/drivers/i2c
SOURCES += $(wildcard $(I2C_DRIVER_DIR)/common/*.c) \
           $(wildcard $(I2C_DRIVER_DIR)/i2c_master/src/*.c) \
           $(wildcard $(I2C_DRIVER_DIR)/i2c_slave/src/*.c)
INCLUDES += $(I2C_DRIVER_DIR)/common \
            $(I2C_DRIVER_DIR)/i2c_master \
            $(I2C_DRIVER_DIR)/i2c_slave
