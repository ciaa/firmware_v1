#----------------------------------------------------------
# Include the drivers according to the chip.
#----------------------------------------------------------

SUB_DIR := clock hwtimer interrupt power

subdir = $(foreach dir, $(SUB_DIR), $(SDK_ROOT)/platform/system/$(dir)/subdir.mk)

include $(subdir)
