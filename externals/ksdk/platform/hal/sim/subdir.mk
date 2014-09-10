SIM_HAL_DIR := $(SDK_ROOT)/platform/hal/sim
SOURCES += $(SIM_HAL_DIR)/M$(CHIP)/fsl_sim_hal_$(CHIP).c \
           $(SIM_HAL_DIR)/fsl_sim_hal.c
INCLUDES += $(SIM_HAL_DIR) \
            $(SIM_HAL_DIR)/M$(CHIP)
