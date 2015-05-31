/* Copyright 2012-2015, Eric Nicolás Pernia
 * All rights reserved.
 *
 * This file is part of IDE4PLC and CIAA Firmware.
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

#ifndef PLC_ELEMENTARY_DATA_TYPES_H_
#define PLC_ELEMENTARY_DATA_TYPES_H_
/** \brief PLC Elementary data types
 **
 ** PLC Elementary data types
 **
 **/

/** \addtogroup CIAA_Firmware CIAA Firmware
 ** @{ */
/** \addtogroup PLC PLC Module
 ** @{ */

/*
 * Initials     Name
 * ---------------------------
 * ErPe         Eric Pernia
 */

/*
 * modification history (new versions first)
 * -----------------------------------------------------------
 * 20120204 v0.0.1 ErPe initial version
 */

/*==================[inclusions]=============================================*/
#include "PLC_Services.h"

/*==================[cplusplus]==============================================*/
#ifdef __cplusplus
extern "C" {
#endif

/*==================[macros]=================================================*/

/*==================[typedef]================================================*/

                                                 /*+------+--------------------------+----------------------------------------------------+*/
                                                 /*| Bits |         Data type        |                        Range                       |*/
                                                 /*|      |                          |                                                    |*/
/** \brief PLC BOOL Type */                      /*+------+--------------------------+----------------------------------------------------+*/
typedef uint8_t  PLC_BOOL;                       /*| 8    | Boolean                  | 0 = false, !0 = true                               |*/
                                                 /*|      |                          |                                                    |*/
/** \brief PLC BYTE Type */                      /*+------+--------------------------+----------------------------------------------------+*/
typedef uint8_t  PLC_BYTE;                       /*| 8    | Bit string of lenght 8   | 0x00..0xFF                                         |*/
/** \brief PLC WORD Type */                      /*+------+--------------------------+----------------------------------------------------+*/
typedef uint16_t PLC_WORD;                       /*| 16   | Bit string of lenght 16  | 0x0000..0xFFFF                                     |*/
/** \brief PLC DOUBLE WORD Type */               /*+------+--------------------------+----------------------------------------------------+*/
typedef uint32_t PLC_DWORD;                      /*| 32   | Bit string of lenght 32  | 0x00000000..0xFFFFFFFF                             |*/
/** \brief PLC LONG WORD Type */                 /*+------+--------------------------+----------------------------------------------------+*/
typedef uint64_t PLC_LWORD;                      /*| 64   | Bit string of lenght 64  | 0x0000000000000000..0xFFFFFFFFFFFFFFFF             |*/
                                                 /*|      |                          |                                                    |*/
/** \brief PLC SHORT INTEGER Type */             /*+------+--------------------------+----------------------------------------------------+*/
typedef int8_t   PLC_SINT;                       /*| 8    | Short integer            | -128..127                                          |*/
/** \brief PLC INTEGER Type */                   /*+------+--------------------------+----------------------------------------------------+*/
typedef int16_t  PLC_INT;                        /*| 16   | Integer                  | -32768..32767                                      |*/
/** \brief PLC DOUBLE INTEGER Type */            /*+------+--------------------------+----------------------------------------------------+*/
typedef int32_t  PLC_DINT;                       /*| 32   | Double integer           | -2147483648..2147483647                            |*/
/** \brief PLC LONG INTEGER Type */              /*+------+--------------------------+----------------------------------------------------+*/
typedef int64_t  PLC_LINT;                       /*| 64   | Long integer             | -9223372036854775808..9223372036854775807          |*/
                                                 /*|      |                          |                                                    |*/
/** \brief PLC UNSIGNED SHORT INTEGER Type */    /*+------+--------------------------+----------------------------------------------------+*/
typedef uint8_t  PLC_USINT;                      /*| 8    | Unsigned short integer   | 0..255                                             |*/
/** \brief PLC UNSIGNED INTEGER Type */          /*+------+--------------------------+----------------------------------------------------+*/
typedef uint16_t PLC_UINT;                       /*| 16   | Unsigned integer         | 0..65535                                           |*/
/** \brief PLC UNSIGNED DOUBLE INTEGER Type */   /*+------+--------------------------+----------------------------------------------------+*/
typedef uint32_t PLC_UDINT;                      /*| 32   | Unsigned double integer  | 0..4294967295                                      |*/
/** \brief PLC UNSIGNED LONG INTEGER Type */     /*+------+--------------------------+----------------------------------------------------+*/
typedef uint64_t PLC_ULINT;                      /*| 64   | Unsigned long integer    | 0..18446744073709551615                            |*/
                                                 /*|      |                          |                                                    |*/
/** \brief PLC REAL Type */                      /*+------+--------------------------+----------------------------------------------------+*/
typedef float    PLC_REAL;                       /*| 32   | Real numbers             | +/-1.18x10e-38..+/-3.40x10e38                      |*/
/** \brief PLC LONG REAL Type */                 /*+------+--------------------------+----------------------------------------------------+*/
typedef double   PLC_LREAL;                      /*| 64   | Long reals               | +/-2.23x10e-308..+/-1.79x10e308                    |*/
                                                 /*|      |                          |                                                    |*/
/** \brief PLC TIME Type */                      /*+------+--------------------------+----------------------------------------------------+*/
typedef PLC_DINT PLC_TIME;                       /*| 32   | Duration                 | T#-24d_20h_31m_23s_648ms..T#24d_20h_31m_23s_647ms  |*/
                                                 /*|      |                          | (stored as DInt -2147483648ms..2147483647ms)       |*/
/** \brief PLC DATE Type */                      /*+------+--------------------------+----------------------------------------------------+*/
typedef struct                                   /*| 40   | Date (only)              |  D#1970-01-01..D#2554-12-31                        |*/
{                                                /*|      |                          |                                                    |*/
   PLC_UINT      YEAR;                           /*| 16   |                          | 1970..2554 (1970 is the default)                   |*/
   PLC_USINT     MONTH;                          /*| 8    |                          | 1..12 (1 is the default)                           |*/
   PLC_USINT     DAY;                            /*| 8    |                          | 1..31 (1 is the default)                           |*/
   PLC_USINT     WEEKDAY;                        /*| 8    |                          | 1 (sunday)..7 (saturday). Day of the week, default |*/
                                                 /*|      |                          | is 5 (tuesday)                                     |*/
} PLC_DATE;                                      /*|      |                          |                                                    |*/
/** \brief PLC TIME_OF_DAY Type */               /*+------+--------------------------+----------------------------------------------------+*/
typedef struct                                   /*| 40   | Time of Day (only)       | TOD#00:00:00.0..23:59:59.999999999                 |*/
{                                                /*|      |                          |                                                    |*/
   PLC_USINT     HOUR;                           /*| 8    |                          | 0 a 23                                             |*/
   PLC_USINT     MINUTE;                         /*| 8    |                          | 0 a 59                                             |*/
   PLC_USINT     SECOND;                         /*| 8    |                          | 0 a 59                                             |*/
   PLC_UDINT     NANOSECOND;                     /*| 32   |                          | 0 a 999999999 (It depends of RTC resolution,       |*/
                                                 /*|      |                          |                Siemens DTL type use nanoseconds)   |*/
}PLC_TIME_OF_DAY;                                /*|      |                          |                                                    |*/
/** \brief PLC DATE_AND_TIME Type */             /*+------+--------------------------+----------------------------------------------------+*/
typedef struct                                   /*| 96   | Date and time of Day     | DT#1970-01-01-00:00:00.0 ..                        |*/
{                                                /*|      |                          | DT#2554-12-31-23:59:59.999999999                   |*/
   PLC_UINT      YEAR;                           /*| 16   |                          | 1970 a 2554                                        |*/
   PLC_USINT     MONTH;                          /*| 8    |                          | 1 a 12                                             |*/
   PLC_USINT     DAY;                            /*| 8    |                          | 1 a 31                                             |*/
   PLC_USINT     WEEKDAY;                        /*| 8    |                          | 1 (sunday) a 7 (saturday)                          |*/
   PLC_USINT     HOUR;                           /*| 8    |                          | 0 a 23                                             |*/
   PLC_USINT     MINUTE;                         /*| 8    |                          | 0 a 59                                             |*/
   PLC_USINT     SECOND;                         /*| 8    |                          | 0 a 59                                             |*/
   PLC_UDINT     NANOSECOND;                     /*| 32   |                          | 0 a 999999999 (It depends of RTC resolution,       |*/
                                                 /*|      |                          |                Siemens DTL type use nanoseconds)   |*/
}PLC_DATE_AND_TIME;                              /*|      |                          |                                                    |*/
                                                 /*|      |                          |                                                    |*/
/** \brief PLC STRING Type */                    /*+------+--------------------------+----------------------------------------------------+*/
typedef struct                                   /*|      | Variable-length single-  | 0 to 254 chars, each char of 1 byte size           |*/
{                                                /*|      | byte character string    |                                                    |*/
   PLC_BYTE      TotalCharNumber;                /*|      |                          |                                                    |*/
   PLC_BYTE      CurrentCharNumber;              /*|      |                          |                                                    |*/
   PLC_BYTE      Char[254];                      /*|      |                          |                                                    |*/
                                                 /*|      |                          |                                                    |*/
}PLC_STRING;                                     /*|      |                          |                                                    |*/
/** \brief PLC WSTRING Type */                   /*+------+--------------------------+----------------------------------------------------+*/
typedef struct                                   /*|      | Variable-length double-  | 0 to 254 chars, each char of 2 byte size           |*/
{                                                /*|      | byte character string    |                                                    |*/
   PLC_BYTE      TotalCharNumber;                /*|      |                          |                                                    |*/
   PLC_BYTE      CurrentCharNumber;              /*|      |                          |                                                    |*/
   PLC_WORD      WChar[254];                     /*|      |                          |                                                    |*/
                                                 /*|      |                          |                                                    |*/
}PLC_WSTRING;                                    /*|      |                          |                                                    |*/
                                                 /*|      |                          |                                                    |*/
/** \brief PLC BCD16 (NO STANDARD) Type */       /*+------+--------------------------+----------------------------------------------------+*/
typedef int16_t  PLC_BCD16;                      /*| 16   | 16 bit BCD               | -999 a 999                                         |*/
/** \brief PLC BCD32 (NO STANDARD) Type */       /*+------+--------------------------+----------------------------------------------------+*/
typedef int32_t  PLC_BCD32;                      /*| 32   | 32 bit BCD               | -9999999 a 9999999                                 |*/
                                                 /*+------+--------------------------+----------------------------------------------------+*/

/*==================[external data declaration]==============================*/

/*==================[external functions declaration]=========================*/

/*==================[cplusplus]==============================================*/
#ifdef __cplusplus
}
#endif
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/
#endif /* #ifndef PLC_ELEMENTARY_DATA_TYPES_H_ */

