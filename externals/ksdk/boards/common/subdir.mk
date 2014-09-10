#----------------------------------------------------------
# Include the drivers according to the chip.
#----------------------------------------------------------
ifeq "$(CHIP)" "K22F51212"
include $(SDK_ROOT)/boards/common/mma8451/subdir.mk
endif

ifeq "$(CHIP)" "K64F12"
include $(SDK_ROOT)/boards/common/mma8451/subdir.mk
endif

ifeq "$(CHIP)" "K70F12"
include $(SDK_ROOT)/boards/common/mma8451/subdir.mk
endif

ifeq "$(CHIP)" "KL25Z4"
include $(SDK_ROOT)/boards/common/mma8451/subdir.mk
endif

