ksdk='/Volumes/C/Freescale/KSDK_1.2.0'
ciaa='/Volumes/C/externals/k60_120'
processor='MK64F12'

rm -r -f $ciaa/inc
rm -r -f $ciaa/src

mkdir -p $ciaa/inc
mkdir -p $ciaa/src

include=$'#define '$processor'_SERIES
#define CPU_MK64FN1M0VDC12
/* CMSIS-style register definitions */
#include "'$processor'.h"
/* Extension register definitions */
#include "'$processor'_registers.h"
/* CPU specific feature definitions */
#include "'$processor'_features.h"'

#echo "$include" > $ciaa/inc/fsl_device_registers.h

cp $ksdk/platform/CMSIS/Include/*.h $ciaa/inc/
#cp $ksdk/platform/CMSIS/Include/device/*.h $ciaa/inc/
cp $ksdk/platform/devices/*.h $ciaa/inc/
cp $ksdk/platform/devices/*.c $ciaa/src/

# Replace device/MK64F12/MK64F12.h by MK64F12.h
#sed -i '' -- 's/device\/[A-Z,0-9]*\///g' $ciaa/inc/fsl_device_registers.h
#cp $ksdk/platform/CMSIS/Include/device/$processor/*.h $ciaa/inc/
sed -i '' -- 's/[A-Z,0-9]*\/include\///g' $ciaa/inc/fsl_device_registers.h
cp $ksdk/platform/devices/$processor/include/*.h $ciaa/inc/

#cp $ksdk/platform/startup/$processor/*.h $ciaa/inc/
#cp $ksdk/platform/startup/$processor/*.c $ciaa/src/
#sed -i '' -- 's/device\///g' $ciaa/src/system_$processor.c
#cp $ksdk/platform/startup/$processor/gcc/*.S $ciaa/src/

cp $ksdk/platform/devices/$processor/startup/*.h $ciaa/inc/
cp $ksdk/platform/devices/$processor/startup/*.c $ciaa/src/
sed -i '' -- 's/device\///g' $ciaa/src/system_$processor.c
cp $ksdk/platform/devices/$processor/startup/gcc/*.S $ciaa/src/

cp $ksdk/platform/hal/inc/fsl_sim_hal.h $ciaa/inc/
sed -i '' -- 's/..\/src\/sim\/MK.*\///g' $ciaa/inc/fsl_sim_hal.h

cp $ksdk/platform/hal/src/sim/$processor/*.h $ciaa/inc/
cp $ksdk/platform/hal/src/sim/$processor/*.c $ciaa/src/

cp $ksdk/platform/hal/inc/fsl_mcg_hal.h $ciaa/inc/
cp $ksdk/platform/hal/inc/fsl_mcg_hal_modes.h $ciaa/inc/
#cp $ksdk/platform/hal/src/mcg/*.h $ciaa/inc/
cp $ksdk/platform/hal/src/mcg/*.c $ciaa/src/

cp $ksdk/platform/hal/inc/fsl_osc_hal.h $ciaa/inc/
#cp $ksdk/platform/hal/src/osc/*.h $ciaa/inc/
cp $ksdk/platform/hal/src/osc/*.c $ciaa/src/

cp $ksdk/platform/hal/inc/fsl_lptmr_hal.h $ciaa/inc/
#cp $ksdk/platform/hal/src/lptmr/*.h $ciaa/inc/
cp $ksdk/platform/hal/src/lptmr/*.c $ciaa/src/

#### Nuevo en version 1.2.0
cp $ksdk/platform/hal/inc/fsl_rtc_hal.h $ciaa/inc/
#cp $ksdk/platform/hal/src/osc/*.h $ciaa/inc/
cp $ksdk/platform/hal/src/rtc/*.c $ciaa/src/

cp $ksdk/platform/hal/inc/fsl_port_hal.h $ciaa/inc/
cp $ksdk/platform/hal/src/port/*.c $ciaa/src/

cp $ksdk/platform/hal/inc/fsl_gpio_hal.h $ciaa/inc/
cp $ksdk/platform/hal/src/gpio/*.c $ciaa/src/

cp $ksdk/platform/hal/inc/fsl_uart_hal.h $ciaa/inc/
cp $ksdk/platform/hal/src/uart/*.c $ciaa/src/

cp $ksdk/platform/system/inc/fsl_clock_manager.h $ciaa/inc/
cp $ksdk/platform/system/src/clock/*.c $ciaa/src/
cp $ksdk/platform/system/src/clock/$processor/*.h $ciaa/inc/
cp $ksdk/platform/system/src/clock/$processor/*.c $ciaa/src/
sed -i '' -- 's/..\/src\/clock\/[A-Z,0-9]*\///g' $ciaa/inc/fsl_clock_manager.h
sed -i '' -- 's/^.*include.*fsl_os_abstraction.*$//g' $ciaa/src/fsl_clock_manager.c
sed -i '' -- 's/^.*OSA_.*Critical(.*);.*$//g' $ciaa/src/fsl_clock_manager.c