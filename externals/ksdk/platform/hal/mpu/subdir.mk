MPU_HAL_DIR := $(SDK_ROOT)/platform/hal/mpu
SOURCES += $(wildcard $(MPU_HAL_DIR)/*.c)
INCLUDES += $(MPU_HAL_DIR)
