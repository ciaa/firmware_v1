ksdk='/Volumes/C/Freescale/KSDK_1.2.0'
ciaa='/Volumes/C/externals/k60_120'
processor='MK63F12'

###############################################################################
# Preparation                                                                 #
###############################################################################
rm -r -f $ciaa/inc
#rm -r -f $ciaa/inc/devices/$processor
rm -r -f $ciaa/src
#rm -r -f $ciaa/src/devices/$processor

mkdir -p $ciaa/inc
#mkdir -p $ciaa/inc/devices/$processor
mkdir -p $ciaa/src
#mkdir -p $ciaa/src/devices/$processor

#cp $ksdk/platform/devices/$processor/startup/*.h $ciaa/inc/
#cp $ksdk/platform/devices/$processor/startup/*.c $ciaa/src/
#sed -i '' -- 's/device\///g' $ciaa/src/system_$processor.c
#cp $ksdk/platform/devices/$processor/startup/gcc/*.S $ciaa/src/

#cp $ksdk/platform/hal/src/sim/$processor/*.h $ciaa/inc/
#cp $ksdk/platform/hal/src/sim/$processor/*.c $ciaa/src/

###############################################################################
# System core                                                                 #
###############################################################################
# Common files for system core
cp $ksdk/platform/CMSIS/Include/*.h $ciaa/inc/

# Processor dependent files for system core
cp $ksdk/platform/devices/$processor/include/*.h $ciaa/inc/

###############################################################################
# System startup                                                               #
###############################################################################
# Common files for system startup
cp $ksdk/platform/devices/*.h $ciaa/inc/
cp $ksdk/platform/devices/*.c $ciaa/src/
# Clear file path from include headers
sed -i '' -- 's/[A-Z,0-9]*\/include\///g' $ciaa/inc/fsl_device_registers.h


# Processor dependent files for system startup
#cp $ksdk/platform/devices/$processor/startup/*.h $ciaa/inc/devices/$processor
cp $ksdk/platform/devices/$processor/startup/*.h $ciaa/inc/
#cp $ksdk/platform/devices/$processor/startup/*.c $ciaa/src/devices/$processor
cp $ksdk/platform/devices/$processor/startup/*.c $ciaa/src/

# Compiler dependent files for system startup
#cp $ksdk/platform/devices/$processor/startup/gcc/*.S $ciaa/src
cp $ksdk/platform/devices/$processor/startup/gcc/*.S $ciaa/src/

###############################################################################
# System clock                                                                #
###############################################################################
# Common files for system clock
cp $ksdk/platform/hal/inc/fsl_osc_hal.h $ciaa/inc/
cp $ksdk/platform/hal/src/osc/*.c $ciaa/src/

cp $ksdk/platform/hal/inc/fsl_mcg_hal.h $ciaa/inc/
cp $ksdk/platform/hal/inc/fsl_mcg_hal_modes.h $ciaa/inc/
cp $ksdk/platform/hal/src/mcg/*.c $ciaa/src/

cp $ksdk/platform/hal/inc/fsl_sim_hal.h $ciaa/inc/
# Clear file path from include headers
sed -i '' -- 's/..\/src\/sim\/MK.*\///g' $ciaa/inc/fsl_sim_hal.h

cp $ksdk/platform/system/inc/fsl_clock_manager.h $ciaa/inc/
cp $ksdk/platform/system/src/clock/*.c $ciaa/src/
# Clear file path from include headers
#sed -i '' -- 's/\/clock\//\/devices\//g' $ciaa/inc/fsl_clock_manager.h
sed -i '' -- 's/..\/src\/clock\/[A-Z,0-9]*\///g' $ciaa/inc/fsl_clock_manager.h
#Clear file from operating system dependances
sed -i '' -- 's/^.*include.*fsl_os_abstraction.*$//g' $ciaa/src/fsl_clock_manager.c
sed -i '' -- 's/^.*OSA_.*Critical(.*);.*$//g' $ciaa/src/fsl_clock_manager.c

# Processor dependent for system clock
#cp $ksdk/platform/hal/src/sim/$processor/*.h $ciaa/inc/devices/$processor
cp $ksdk/platform/hal/src/sim/$processor/*.h $ciaa/inc/
#cp $ksdk/platform/hal/src/sim/$processor/*.c $ciaa/src/devices/$processor
cp $ksdk/platform/hal/src/sim/$processor/*.c $ciaa/src/

#cp $ksdk/platform/system/src/clock/$processor/*.h $ciaa/inc/devices/$processor
cp $ksdk/platform/system/src/clock/$processor/*.h $ciaa/inc/
#cp $ksdk/platform/system/src/clock/$processor/*.c $ciaa/src/devices/$processor
cp $ksdk/platform/system/src/clock/$processor/*.c $ciaa/src/

###############################################################################
# Periferical Modules                                                         #
###############################################################################
cp $ksdk/platform/hal/inc/fsl_adc16_hal.h $ciaa/inc/
cp $ksdk/platform/hal/src/adc16/*.c $ciaa/src/

cp $ksdk/platform/hal/inc/fsl_gpio_hal.h $ciaa/inc/
cp $ksdk/platform/hal/src/gpio/*.c $ciaa/src/

#cp $ksdk/platform/hal/inc/fsl_lptmr_hal.h $ciaa/inc/
#cp $ksdk/platform/hal/src/lptmr/*.c $ciaa/src/

cp $ksdk/platform/hal/inc/fsl_port_hal.h $ciaa/inc/
cp $ksdk/platform/hal/src/port/*.c $ciaa/src/

cp $ksdk/platform/hal/inc/fsl_rtc_hal.h $ciaa/inc/
cp $ksdk/platform/hal/src/rtc/*.c $ciaa/src/

cp $ksdk/platform/hal/inc/fsl_uart_hal.h $ciaa/inc/
cp $ksdk/platform/hal/src/uart/*.c $ciaa/src/

#Archivos Editados
# fsl_device_registers.h
# fsl_sim_hal.h
# fsl_clock_manager.h

#Diferencias
#K60_F12 
#   Puede correr a 150 MHz
#   Tiene 16 KByte de Cache
#   Tiene 16 KBytes de EEPROM
#   Tiene controlador para NANDFlash
#   Tiene 2 i2s
#   USB UTG FS y FS/HS
#   No Tiene USB device chage detect cristall less
#   Tiene 2 devices CAN
#   Tiene 4 ADC de 16 bits en lugar de 2