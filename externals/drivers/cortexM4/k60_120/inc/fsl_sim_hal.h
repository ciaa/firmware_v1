/*
* Copyright (c) 2013 - 2014, Freescale Semiconductor, Inc.
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without modification,
* are permitted provided that the following conditions are met:
*
* o Redistributions of source code must retain the above copyright notice, this list
*   of conditions and the following disclaimer.
*
* o Redistributions in binary form must reproduce the above copyright notice, this
*   list of conditions and the following disclaimer in the documentation and/or
*   other materials provided with the distribution.
*
* o Neither the name of Freescale Semiconductor, Inc. nor the names of its
*   contributors may be used to endorse or promote products derived from this
*   software without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
* ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
* DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
* ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
* LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
* ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
* (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
* SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#if !defined(__FSL_SIM_HAL_H__)
#define __FSL_SIM_HAL_H__

#include <stdint.h>
#include <stdbool.h>
#include <assert.h>
#include "fsl_device_registers.h"

/*! @addtogroup sim_hal*/
/*! @{*/

/*! @file*/

/*******************************************************************************
* Definitions
******************************************************************************/

/*! @brief SIM HAL API return status*/
typedef enum _sim_hal_status {
    kSimHalSuccess,  /*!< Success.      */
    kSimHalFail,     /*!< Error occurs. */
} sim_hal_status_t;

/*******************************************************************************
* API
******************************************************************************/

/*
* Include the CPU-specific clock API header files.
*/
#if (defined(K02F12810_SERIES))
    /* Clock System Level API header file */
    #include "fsl_sim_hal_MK02F12810.h"

#elif (defined(K20D5_SERIES))

#elif (defined(K22F12810_SERIES))

/* Clock System Level API header file */
#include "fsl_sim_hal_MK22F12810.h"


#elif (defined(K22F25612_SERIES))

/* Clock System Level API header file */
#include "fsl_sim_hal_MK22F25612.h"


#elif (defined(K22F51212_SERIES))

/* Clock System Level API header file */
#include "fsl_sim_hal_MK22F51212.h"


#elif (defined(K24F12_SERIES))

/* Clock System Level API header file */
#include "fsl_sim_hal_MK24F12.h"

#elif (defined(K24F25612_SERIES))

#include "fsl_sim_hal_MK24F25612.h"

#elif (defined(K26F18_SERIES))
/* Clock System Level API header file */
#include "fsl_sim_hal_MK26F18.h"

#elif (defined(K10D10_SERIES))
#include "fsl_sim_hal_MK10D10.h"

#elif (defined(K20D10_SERIES))
#include "fsl_sim_hal_MK20D10.h"

#elif (defined(K30D10_SERIES))
#include "fsl_sim_hal_MK30D10.h"

#elif (defined(K40D10_SERIES))
#include "fsl_sim_hal_MK40D10.h"

#elif (defined(K50D10_SERIES))
#include "fsl_sim_hal_MK50D10.h"

#elif (defined(K51D10_SERIES))
#include "fsl_sim_hal_MK51D10.h"

#elif (defined(K52D10_SERIES))
#include "fsl_sim_hal_MK52D10.h"

#elif (defined(K53D10_SERIES))
#include "fsl_sim_hal_MK53D10.h"

#elif (defined(K60D10_SERIES))

/* Clock System Level API header file */
#include "fsl_sim_hal_MK60D10.h"

#elif (defined(K60F12_SERIES))

/* Clock System Level API header file */
#include "fsl_sim_hal_MK60F12.h"

#elif (defined(K63F12_SERIES))

/* Clock System Level API header file */
#include "fsl_sim_hal_MK63F12.h"

#elif (defined(K64F12_SERIES))

/* Clock System Level API header file */
#include "fsl_sim_hal_MK64F12.h"

#elif (defined(K65F18_SERIES))

/* Clock System Level API header file */
#include "fsl_sim_hal_MK65F18.h"

#elif (defined(K66F18_SERIES))

/* Clock System Level API header file */
#include "fsl_sim_hal_MK66F18.h"

#elif (defined(K70F12_SERIES))


#elif (defined(K70F15_SERIES))


#elif (defined(KL02Z4_SERIES))
#include "fsl_sim_hal_MKL02Z4.h"

#elif (defined(KL03Z4_SERIES))
/* Clock System Level API header file */
#include "fsl_sim_hal_MKL03Z4.h"

#elif (defined(KL28T7_SERIES))
#include "fsl_sim_hal_MKL28T7.h"

#elif (defined(KL05Z4_SERIES))


#elif (defined(KL13Z4_SERIES))

#elif (defined(KL14Z4_SERIES))
#include "fsl_sim_hal_MKL14Z4.h"

#elif (defined(KL15Z4_SERIES))
#include "fsl_sim_hal_MKL15Z4.h"

#elif (defined(KL23Z4_SERIES))

#elif (defined(KL24Z4_SERIES))
#include "fsl_sim_hal_MKL24Z4.h"

#elif (defined(KL25Z4_SERIES))
/* Clock System Level API header file */
#include "fsl_sim_hal_MKL25Z4.h"

#elif (defined(KL17Z4_SERIES))
#include "fsl_sim_hal_MKL17Z4.h"

#elif (defined(KL27Z4_SERIES))
#include "fsl_sim_hal_MKL27Z4.h"

#elif (defined(KL33Z4_SERIES))
#include "fsl_sim_hal_MKL33Z4.h"

#elif (defined(KL34Z4_SERIES))
#include "fsl_sim_hal_MKL34Z4.h"

#elif (defined(KL43Z4_SERIES))
#include "fsl_sim_hal_MKL43Z4.h"

#elif (defined (KL17Z644_SERIES))
#include "fsl_sim_hal_MKL17Z644.h"

#elif (defined (KL27Z644_SERIES))
#include "fsl_sim_hal_MKL27Z644.h"

#elif (defined(KL16Z4_SERIES))
#include "fsl_sim_hal_MKL16Z4.h"

#elif (defined(KL26Z4_SERIES))
#include "fsl_sim_hal_MKL26Z4.h"

#elif (defined(KL36Z4_SERIES))
#include "fsl_sim_hal_MKL36Z4.h"

#elif (defined(KL46Z4_SERIES))

/* Clock System Level API header file */
#include "fsl_sim_hal_MKL46Z4.h"

#elif (defined(KV30F12810_SERIES))
/* Clock System Level API header file */
#include "fsl_sim_hal_MKV30F12810.h"

#elif (defined(KV31F12810_SERIES))

/* Clock System Level API header file */
#include "fsl_sim_hal_MKV31F12810.h"

#elif (defined(KV31F25612_SERIES))

/* Clock System Level API header file */
#include "fsl_sim_hal_MKV31F25612.h"


#elif (defined(KV31F51212_SERIES))

/* Clock System Level API header file */
#include "fsl_sim_hal_MKV31F51212.h"

#elif (defined(KV40F15_SERIES))

#include "fsl_sim_hal_MKV40F15.h"

#elif (defined(KV43F15_SERIES))

#include "fsl_sim_hal_MKV43F15.h"

#elif (defined(KV44F15_SERIES))

#include "fsl_sim_hal_MKV44F15.h"

#elif (defined(KV45F15_SERIES))

#include "fsl_sim_hal_MKV45F15.h"

#elif (defined(KV46F15_SERIES))

#include "fsl_sim_hal_MKV46F15.h"

#elif (defined(KV10Z7_SERIES))

#include "fsl_sim_hal_MKV10Z7.h"

#elif (defined(KW01Z4_SERIES))
/* Clock System Level API header file */
#include "fsl_sim_hal_MKW01Z4.h"

#elif (defined(K11DA5_SERIES))

#include "fsl_sim_hal_MK11DA5.h"

#elif (defined(K21DA5_SERIES))

#include "fsl_sim_hal_MK21DA5.h"



#elif (defined(KW21D5_SERIES))


#include "fsl_sim_hal_MKW21D5.h"


#elif (defined(KW22D5_SERIES))

#include "fsl_sim_hal_MKW22D5.h"

#elif (defined(KW24D5_SERIES))

#include "fsl_sim_hal_MKW24D5.h"

#elif (defined(K21FA12_SERIES))
#include "fsl_sim_hal_MK21FA12.h"
#if FSL_FEATURE_SOC_SIM_COUNT
#else
#error "No valid CPU defined!"
#endif

/*! @}*/

#endif
#endif /* __FSL_SIM_HAL_H__*/
/*******************************************************************************
* EOF
******************************************************************************/

