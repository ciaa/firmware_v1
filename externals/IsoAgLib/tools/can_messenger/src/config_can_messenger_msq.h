// File: config_can_messenger_msq.h
// IMPORTANT: Never change this header manually!!!
//            All manual changes are overwritten by the next call of "conf2build.sh conf_can_messenger_msq_x86linux" 
//            Perform changes direct in the feature and project setup file conf_can_messenger_msq_x86linux


// These defines are needed for inclusion of the correct HAL modules:
#define HAL_PATH_ISOAGLIB IsoAgLib/hal/pc
#define HAL_PATH_ISOAGLIB_SYSTEM IsoAgLib/hal/pc/system
#define HAL_PATH_ISOAGLIB_CAN IsoAgLib/hal/pc/can
#define HAL_PATH_SUPPLEMENTARY_EEPROM supplementary_driver/hal/pc/eeprom
#define HAL_PATH_SUPPLEMENTARY_RS232 supplementary_driver/hal/pc/rs232
#define HAL_PATH_SUPPLEMENTARY_OUTPUTS supplementary_driver/hal/pc/outputs
#define HAL_PATH_SUPPLEMENTARY_INPUTS supplementary_driver/hal/pc/inputs
#define HAL_PATH_SUPPLEMENTARY_DATASTREAMS supplementary_driver/hal/pc/datastreams


// These defines are set in the conf_can_messenger_msq_x86linux:
#define CAN_INSTANCE_CNT 4
#define PRT_INSTANCE_CNT 1
#define RS232_CHANNEL_CNT 1
#define RS232_INSTANCE_CNT 1


// Decide if the CPU stores number variables in BIG or LITTLE endian byte order in memory.
// Most CPU will use LITTLE ENDIAN. Only some types of ARM, mostly 68k and PowerPC CPU types will use big endian.
// Please check the manual of your targret cpu. This setting is used to activate some quick number conversion algorithms,
// which provide quick conversion from number variable to CAN strings ( which are always little endian in ISO ) - and other way.
#define OPTIMIZE_NUMBER_CONVERSIONS_FOR_LITTLE_ENDIAN

// #define OPTIMIZE_NUMBER_CONVERSIONS_FOR_BIG_ENDIAN

// Decide if HEAP allocation strategy shall reduce size about 5K to 10K in favour of speed
// Strong Advice: Don't activate this, as long your target has not too tight memory restrictions
// Initialization of CAN filters and of local process data might get too slow under worst case conditions
// #define OPTIMIZE_HEAPSIZE_IN_FAVOR_OF_SPEED

#ifndef USE_ISO_11783 
	#define USE_ISO_11783 
#endif
#ifndef DEF_Stream_IMPL   
	#define DEF_Stream_IMPL   StreamChunk   
#endif
#ifndef DEF_Stream_c_IMPL 
	#define DEF_Stream_c_IMPL StreamChunk_c 
#endif
#ifndef USE_RS232
	#define USE_RS232 1
#endif

// DONT REMOVE THIS AND THE FOLLOWING LINE AS THEY ARE NEEDED TO DETECT YOUR PERSONAL PROJECT ADAPTATIONS!!!
// START_INDIVIDUAL_PROJECT_CONFIG
