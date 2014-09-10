MPU_DRIVER_DIR := $(SDK_ROOT)/platform/drivers/mpu
SOURCES += $(MPU_DRIVER_DIR)/src/fsl_mpu_driver.c \
           $(MPU_DRIVER_DIR)/common/fsl_mpu_common.c
INCLUDES += $(MPU_DRIVER_DIR) \
            $(MPU_DRIVER_DIR)/common
