RTC_HAL_DIR := $(SDK_ROOT)/platform/hal/rtc
SOURCES += $(wildcard $(RTC_HAL_DIR)/*.c)
INCLUDES += $(RTC_HAL_DIR)
