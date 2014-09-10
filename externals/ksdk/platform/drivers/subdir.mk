#----------------------------------------------------------
# Include the drivers according to the chip.
# If a driver will be build in to platform_lib, add the
# folder's name to SUB_DIR, then the file "subdir.mk" in
# the folder will be included.
# e.g, if adc driver will be added to the K70F12's platform_lib,
# just add "adc" to K70F12's SUB_DIR. Here, "adc" is the
# foler's name where adc source code is.
#----------------------------------------------------------

ifeq "$(CHIP)" "K22F51212"
SUB_DIR := adc dac edma dspi flextimer gpio i2c lptmr lpuart pdb \
           pit rtc sai uart wdog
endif

ifeq "$(CHIP)" "K22F25612"
SUB_DIR := adc dac dspi edma flextimer gpio i2c lptmr pdb \
           pit rtc sai uart wdog
endif

ifeq "$(CHIP)" "K22F12810"
SUB_DIR := adc dac edma dspi flextimer gpio i2c lptmr lpuart pdb \
           pit rtc sai uart wdog
endif

ifeq "$(CHIP)" "K64F12"
SUB_DIR := adc can dac dspi edma enet flextimer gpio i2c lptmr pdb \
           pit rtc sai sdcard sdhc uart wdog mpu
endif

ifeq "$(CHIP)" "KV31F12810"
SUB_DIR := adc dac dspi edma flextimer gpio i2c lptmr pdb \
           pit uart wdog
endif

ifeq "$(CHIP)" "KV31F25612"
SUB_DIR := adc dac dspi edma flextimer gpio i2c lptmr pdb \
           pit uart wdog
endif

ifeq "$(CHIP)" "KV31F51212"
SUB_DIR := adc dac dspi edma flextimer gpio i2c lptmr pdb \
           pit uart wdog
endif

ifeq "$(CHIP)" "K70F12"
SUB_DIR := adc can clock edma dspi enet flextimer gpio i2c interrupt \
           pit rtc sai sdhc smc uart wdog
endif

ifeq "$(CHIP)" "KL25Z4"
SUB_DIR := adc dac dma gpio i2c lptmr \
           pit rtc spi uart
endif

subdir = $(foreach dir, $(SUB_DIR), $(SDK_ROOT)/platform/drivers/$(dir)/subdir.mk)

include $(subdir)

