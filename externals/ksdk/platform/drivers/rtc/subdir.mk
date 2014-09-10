RTC_DRIVER_DIR := $(SDK_ROOT)/platform/drivers/rtc
SOURCES += $(wildcard $(RTC_DRIVER_DIR)/src/*.c) \
           $(RTC_DRIVER_DIR)/common/fsl_rtc_common.c
INCLUDES += $(RTC_DRIVER_DIR) \
            $(RTC_DRIVER_DIR)/common
