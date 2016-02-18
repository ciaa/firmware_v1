/* --COPYRIGHT--,BSD
 * Copyright (c) 2014, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * --/COPYRIGHT--*/
//*****************************************************************************
//
// mpy.c - Driver for the mpy Module.
//
//*****************************************************************************

//*****************************************************************************
//
//! \addtogroup mpy_api mpy
//! @{
//
//*****************************************************************************

#include "inc/hw_memmap.h"

#ifdef __MSP430_HAS_MPY__
#include "mpy.h"

#include <assert.h>

void MPY_setOperandOne8Bit(uint8_t multiplicationType,
                           uint8_t operand)
{
    HWREG8(MPY_BASE + OFS_MPY + multiplicationType) = operand;
}

void MPY_setOperandOne16Bit(uint8_t multiplicationType,
                            uint16_t operand)
{
    HWREG16(MPY_BASE + OFS_MPY + multiplicationType) = operand;
}

void MPY_setOperandTwo8Bit(uint8_t operand)
{
    HWREG8(MPY_BASE + OFS_OP2) = operand;
}

void MPY_setOperandTwo16Bit(uint16_t operand)
{
    HWREG16(MPY_BASE + OFS_OP2) = operand;
}

uint32_t MPY_getResult(void)
{
    uint32_t result;
    result = HWREG16(MPY_BASE + OFS_RESLO);
    result += ((uint32_t) HWREG16(MPY_BASE + OFS_RESHI) << 16);
    return(result);
}

uint16_t MPY_getSumExtension(void)
{
    return (HWREG16(MPY_BASE + OFS_SUMEXT));
}

#endif
//*****************************************************************************
//
//! Close the doxygen group for mpy_api
//! @}
//
//*****************************************************************************
