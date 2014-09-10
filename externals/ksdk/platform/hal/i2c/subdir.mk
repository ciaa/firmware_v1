I2C_HAL_DIR := $(SDK_ROOT)/platform/hal/i2c
SOURCES += $(wildcard $(I2C_HAL_DIR)/*.c)
INCLUDES += $(I2C_HAL_DIR) \
            $(SDK_ROOT)/platform/system/clock
