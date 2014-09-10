#----------------------------------------------------------
# Include the drivers according to the chip.
# If a driver will be build in to lib, add the
# folder's name to SUB_DIR, then the file "subdir.mk" in
# the folder will be included.
# e.g, if adc driver will be added to the K70F12's lib,
# just add "adc" to K70F12's SUB_DIR. Here, "adc" is the
# foler's name where adc source code is.
#----------------------------------------------------------

ifeq "$(CHIP)" "K22F51212"
SUB_DIR := adc dac dmamux dspi edma flextimer gpio i2c llwu lptmr lpuart mcg osc pdb pit pmc port rtc \
           rcm sai sim smc uart wdog
endif

ifeq "$(CHIP)" "K22F25612"
SUB_DIR := adc dac dmamux dspi edma flextimer gpio i2c llwu lptmr lpuart mcg osc pdb pit pmc port rtc \
           rcm sai sim smc uart wdog
endif

ifeq "$(CHIP)" "K22F12810"
SUB_DIR := adc dac dmamux dspi edma flextimer gpio i2c llwu lptmr lpuart mcg osc pdb pit pmc port rtc \
           rcm sai sim smc uart wdog
endif

ifeq "$(CHIP)" "K64F12"
SUB_DIR := adc can dac dmamux dspi edma enet flextimer gpio i2c llwu lptmr mcg osc pdb \
           pit pmc port rcm rtc sai sdhc sim smc uart wdog mpu
endif

ifeq "$(CHIP)" "KV31F12810"
SUB_DIR := adc dac dmamux dspi edma flextimer gpio i2c llwu lptmr mcg osc pdb \
           pit pmc port rcm sim smc uart wdog
endif

ifeq "$(CHIP)" "KV31F51212"
SUB_DIR := adc dac dmamux dspi edma flextimer gpio i2c llwu lptmr mcg osc pdb \
           pit pmc port rcm sim smc uart wdog
endif

ifeq "$(CHIP)" "KV31F25612"
SUB_DIR := adc dac dmamux dspi edma flextimer gpio i2c llwu lptmr mcg osc pdb \
           pit pmc port rcm sim smc uart wdog
endif

ifeq "$(CHIP)" "K70F12"
SUB_DIR := adc can dmamux dspi edma enet flextimer gpio i2c mcg osc pit port rtc \
           sai sdhc sim smc uart wdog
endif

ifeq "$(CHIP)" "KL25Z4"
SUB_DIR := adc dac dma dmamux gpio i2c llwu lptmr mcg osc pit pmc port rcm rtc \
           sim smc spi uart
endif

subdir = $(foreach dir, $(SUB_DIR), $(SDK_ROOT)/platform/hal/$(dir)/subdir.mk)

include $(subdir)

