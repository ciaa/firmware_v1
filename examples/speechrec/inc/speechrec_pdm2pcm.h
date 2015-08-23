/* Copyright 2014, ACSE & CADIEEL
 *    ACSE   : http://www.sase.com.ar/asociacion-civil-sistemas-embebidos/ciaa/
 *    CADIEEL: http://www.cadieel.org.ar
 * All rights reserved.
 *
 *    or
 *
 * Copyright 2014, Your Name <youremail@domain.com>
 * All rights reserved.
 *
 *    or
 *
 * Copyright 2014, ACSE & CADIEEL & Your Name <youremail@domain.com
 *    ACSE   : http://www.sase.com.ar/asociacion-civil-sistemas-embebidos/ciaa/
 *    CADIEEL: http://www.cadieel.org.ar
 * All rights reserved.
 *
 * This file is part of CIAA Firmware.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 */

#ifndef SPEECHREC_PDM2PCM_H
#define SPEECHREC_PDM2PCM_H
/** \brief Short description of this file
 **
 ** Long description of this file
 **
 **/

/** \addtogroup CIAA_Firmware CIAA Firmware
 ** @{ */
/** \addtogroup Template Template to start a new module
 ** @{ */

/*
 * Initials     Name
 * ---------------------------
 *
 */

/*
 * modification history (new versions first)
 * -----------------------------------------------------------
 * yyyymmdd v0.0.1 initials initial version
 */

/*==================[inclusions]=============================================*/
#include "speechrec_audioDefinitions.h"	/* <= Audio processing definitions header */

/*==================[cplusplus]==============================================*/
#ifdef __cplusplus
extern "C" {
#endif

/*==================[macros]=================================================*/

/** \brief CIC filter decimation factor */
#define speechrecDECIM_FACT_CIC 16

/** \brief FIR filter 1 decimation factor */
#define speechrecDECIM_FACT_FIR1 2

/** \brief FIR filter 2 decimation factor */
#define speechrecDECIM_FACT_FIR2 2

/** \brief Size of memory buffer for CIC filter output */
#define speechrecMEMCICSIZE  (speechrecMEMDMASIZE/(speechrecDECIM_FACT_CIC/8))

/** \brief Size of memory buffer for FIR filter with decimation output */
#define speechrecMEMFIR1SIZE (speechrecMEMCICSIZE/speechrecDECIM_FACT_FIR1)

/** \brief Size of memory buffer for FIR filter with decimation output */
#define speechrecMEMFIR2SIZE (speechrecMEMFIR1SIZE/speechrecDECIM_FACT_FIR2)

/** \brief Size of memory buffer for FIR filter with decimation output */
#define speechrecMEMPCMSIZE (speechrecMEMFIR2SIZE*2)


/*==================[typedef]================================================*/

/*==================[external data declaration]==============================*/

/*==================[external functions declaration]=========================*/
/** \brief Initialization function
 **
 **/
extern void speechrec_pdm2pcm_init(void);

/** \brief Perform a PDM to PCM conversion
 ** Cuando llamo a esta funcion tengo que asegurarme de pasar el inicio o la mitad del buffer PCM que corresponda (memPCM1out o memPCM2out)
 **/
extern void speechrec_pdm2pcm(uint8_t *, int16_t *);

/*==================[cplusplus]==============================================*/
#ifdef __cplusplus
}
#endif
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/
#endif /* #ifndef SPEECHREC_PDM2PCM_H */

