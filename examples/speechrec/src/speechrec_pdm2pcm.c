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
#include "speechrec_pdm2pcm_internal.h"	/* <= PDM to PCM interface internal header */

/*==================[macros and definitions]=================================*/

/*==================[internal data declaration]==============================*/

/** \brief Sigma variables declaration for the CIC filter
 *
 */
static int32_t Sigma1, Sigma2, Sigma3, Sigma4, Sigma5;

/** \brief Delta variables declaration for the CIC filter
 *
 */
static int32_t Delta1, Delta2, Delta3, Delta4;

/** \brief Result variable for the CIC filter
 *
 */
static int32_t Result;

/** \brief Old variables declaration for the CIC filter
 *
 */
static int32_t OldDelta1, OldDelta2, OldDelta3, OldDelta4, OldSigma5;

/** \brief Output memory buffer of the CIC filter
 *
 */
static int16_t memCICout[SPEECHREC_MEMCICSIZE];

/** \brief Q15 FIR decimator 1 structure
 *
 */
static arm_fir_decimate_instance_q15 S1;

/** \brief Q15 FIR decimator 2 structure
 *
 */
static arm_fir_decimate_instance_q15 S2;

/** \brief FIR decimator 1 state buffer
 *
 */
static int16_t StateBuff1[SPEECHREC_NCOEFFS1 + SPEECHREC_MEMCICSIZE - 1];

/** \brief FIR decimator 2 state buffer
 *
 */
static int16_t StateBuff2[SPEECHREC_NCOEFFS2 + SPEECHREC_MEMFIR1SIZE - 1];

/** \brief Memory buffer for FIR decimator 1
 */
static int16_t memFIR1out[SPEECHREC_MEMFIR1SIZE];

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/

/** \brief Coefficients for FIR filter decimator 1
 * Low-pass FIR equiripple
 * Order: 31
 * fs: 64 KHz
 * fpass: 12 KHz
 * fstop: 16 KHz
 * pass band: 0 dB
 * stop band: -38 dB
 */
static const int16_t FIR1coeffs[SPEECHREC_NCOEFFS1] = {
       96,    309,   -120,   -357,    -46,    548,    328,   -668,   -817,
      621,   1571,   -210,  -2801,  -1173,   6070,  13215,  13215,   6070,
    -1173,  -2801,   -210,   1571,    621,   -817,   -668,    328,    548,
      -46,   -357,   -120,    309,     96
};

/** \brief Coeficients for FIR filter decimator 2
 * Low-pass FIR equiripple
 * Order: 31
 * fs: 32 KHz
 * fpass: 7 KHz
 * fstop: 8 KHz
 * pass band: +/-1 dB
 * stop band: -24 dB
 */
static const int16_t FIR2coeffs[SPEECHREC_NCOEFFS2] = {
     -628,    996,    820,   -117,   -612,    235,    952,    -44,  -1263,
     -219,   1751,    756,  -2640,  -2124,   5563,  14005,  14005,   5563,
    -2124,  -2640,    756,   1751,   -219,  -1263,    -44,    952,    235,
     -612,   -117,    820,    996,   -628
};

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/

/* Extraction of each bit from the received PDM data in the buffer */
static int8_t GetPDMbit(uint8_t *PDMbuff,uint8_t word, uint8_t bit)
{
   uint16_t current_word;
   int8_t current_bit;

   current_word = PDMbuff[word*(SPEECHREC_DECIM_FACT_CIC/8)];

   if (SPEECHREC_DECIM_FACT_CIC == 16){
      current_word |= PDMbuff[word*(SPEECHREC_DECIM_FACT_CIC/8) + 1] << 8;
   }

   current_word >>= bit;
   current_bit = current_word & 0x1;
   current_bit = current_bit * 2;
   current_bit = current_bit - 1;

   return current_bit;

}

/* 5th order CIC filter */
static void CIC_filter(uint8_t *PDMbuff)
{
   uint16_t i,j;

   for(j=0;j<SPEECHREC_MEMCICSIZE;j++)
   {
      OldSigma5 = Sigma5;
      OldDelta1 = Delta1;
      OldDelta2 = Delta2;
      OldDelta3 = Delta3;
      OldDelta4 = Delta4;

      /* Sigma operations for the PDM data from Microphone */
      for(i=0; i<SPEECHREC_DECIM_FACT_CIC; i++)
      {
         Sigma1 += GetPDMbit(PDMbuff,j,i);
         Sigma1 += 1;
         Sigma2 += Sigma1;
         Sigma3 += Sigma2;
         Sigma4 += Sigma3;
         Sigma5 += Sigma4;
      }

      /* Delta operations for the PDM data from Microphone */
      Delta1 = Sigma5 - OldSigma5;
      Delta2 = Delta1 - OldDelta1;
      Delta3 = Delta2 - OldDelta2;
      Delta4 = Delta3 - OldDelta3;
      Result = Delta4 - OldDelta4;

      /* Framed data from microphone to be sent for FIR decimation is stored in Result.
      since the operation is 5 stage divided by 16 decimation this field can grow up to
      21 bits. */
      memCICout[j] = (int16_t) (Result>>SPEECHREC_SHIFT_RES);

      }

}

/* FIR filter with decimator*/
static void FIR_decimator(int16_t *PCMbuff)
{
   arm_status ret;

   ret = arm_fir_decimate_init_q15(&S1,
								SPEECHREC_NCOEFFS1,
								(uint8_t) SPEECHREC_DECIM_FACT_FIR1,
								(q15_t *) FIR1coeffs,
								StateBuff1,
								(uint32_t) SPEECHREC_MEMCICSIZE);

   if (ret == ARM_MATH_LENGTH_ERROR)
      while(1){}

   ret = arm_fir_decimate_init_q15(&S2,
								SPEECHREC_NCOEFFS2,
								(uint8_t) SPEECHREC_DECIM_FACT_FIR2,
								(q15_t *) FIR2coeffs,
								StateBuff2,
								(uint32_t) SPEECHREC_MEMFIR1SIZE);

   if (ret == ARM_MATH_LENGTH_ERROR)
      while(1){}

   arm_fir_decimate_q15(&S1,
						memCICout,
						memFIR1out,
						(uint32_t) SPEECHREC_MEMCICSIZE);

   arm_fir_decimate_q15(&S2,
						memFIR1out,
						PCMbuff,
						(uint32_t) SPEECHREC_MEMFIR1SIZE);
}


/*==================[external functions definition]==========================*/

/* \brief Initialization function
 *
 */
extern void speechrec_pdm2pcm_init(void)
{

}

/* \brief Perform a PDM to PCM conversion
 * Cuando llamo a esta funcion tengo que asegurarme de pasar el inicio o la mitad del buffer PCM que corresponda (memPCM1out o memPCM2out)
 */
extern void speechrec_pdm2pcm(uint8_t *PDMbuff, int16_t *PCMbuff) // cuando llamo a esta funcion tengo que asegurarme de pasar el inicio o la mitad de memPCM1out o memPCM2out
{

   CIC_filter(PDMbuff);

   FIR_decimator(PCMbuff);

}



/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/
