/* Copyright 2008, 2009, 2014 Mariano Cerdeiro
 * Copyright 2014, Juan Cecconi
 * Copyright 2014, ACSE & CADIEEL
 *      ACSE: http://www.sase.com.ar/asociacion-civil-sistemas-embebidos/ciaa/
 *      CADIEEL: http://www.cadieel.org.ar
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

/** \brief FreeOSEK Os Internal Arch Implementation File
 **
 ** \file x86/Os_Internal_Arch.c
 ** \arch x86
 **/

/** \addtogroup FreeOSEK
 ** @{ */
/** \addtogroup FreeOSEK_Os
 ** @{ */
/** \addtogroup FreeOSEK_Os_Internal
 ** @{ */


/*
 * Initials     Name
 * ---------------------------
 * MaCe         Mariano Cerdeiro
 * JuCe         Juan Cecconi 
 */

/*
 * modification history (new versions first)
 * -----------------------------------------------------------
 * 20090719 v0.1.3 MaCe rename file to Os_
 * 20090424 v0.1.2 MaCe use the right counter macros
 * 20090130 v0.1.1 MaCe change type uint8_least to uint8f
 * 20080713 v0.1.0 MaCe initial version
 */

/*==================[inclusions]=============================================*/
#include "Os_Internal.h"
#include "ciaaLibs_CircBuf.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>

/*==================[macros and definitions]=================================*/

/*==================[internal data declaration]==============================*/

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/

/*==================[external data definition]===============================*/
uint8	InterruptState;

uint32 OsekHWTimer0;

InterruptFlagsType InterruptFlag;

#ifdef CPUTYPE
#if ( CPUTYPE == ia64 )
uint64 OsStack;

uint64 OsekStack;
#elif ( CPUTYPE == ia32 )
uint32 OsStack;

uint32 OsekStack;
#else /* #if ( CPUTYPE == ia64 ) */
#error Unknown CPUTYPE for ARCH x86
#endif /* #if ( CPUTYPE == ia64 ) */
#else /* #ifdef CPUTYPE */
#error CPUTPYE is not defined
#endif /* #idef CPUTYPE */

/*==================[internal functions definition]==========================*/

/*==================[external functions definition]==========================*/
void OSEK_ISR_HWTimer0(void)
{
#if (ALARMS_COUNT != 0)
   IncrementCounter(HardwareCounter, 1);
#endif /* #if (ALARMS_COUNT != 0) */
}

void OSEK_ISR_HWTimer1(void)
{
#if (defined HWCOUNTER1)
#if (ALARMS_COUNT != 0)
   IncrementCounter(HWCOUNTER1, 1);
#endif /* #if (ALARMS_COUNT != 0) */
#endif /* #if (defined HWCOUNTER1) */
}

void OsInterruptHandler(int signal)
{
   uint8 interrupt;

   if (SIGCHLD == signal)
   {
#if 0
      printf("Signal:%d (%s), pid:%d\n",signal,strsignal(signal), getpid());
#endif
      /* wait for an ending child */
      wait(NULL);
      /* kill Main process */
	  OsekKillSigHandler(0);
   }
   if (SIGTERM == signal)
   {
      /* Terminate Child process */
      *Os_Terminate_Flag = true;
   }

   /* repeat until the buffer is empty */
   while(!ciaaLibs_circBufEmpty(OSEK_IntCircBuf))
   {
      /* read one interrupt */
      ciaaLibs_circBufGet(OSEK_IntCircBuf, &interrupt, 1);

      /* only 0 .. 31 interrupts are allowed */
      if (32 > interrupt)
      {
#if 0
         printf("Interrupt: %d\n",interrupt);
#endif
         if ( (InterruptState) &&
               ( (InterruptMask & (1 << interrupt ) )  == 0 ) )
         {
            InterruptTable[interrupt]();
         }
         else
         {
            InterruptFlag |= 1 << interrupt;
         }
      }
   }

}

void HWTimerFork(uint8 timer)
{
   struct timespec rqtp;
   uint8 interrupt;

   if (timer <= 2)
   {
      /* intererupt every
       * 0 seconds and
       * 10 ms */
      rqtp.tv_sec=0;
      rqtp.tv_nsec=1000000;

      while(*Os_Terminate_Flag == false)
      {
         /* sleep */
         nanosleep(&rqtp,NULL);

         /* the timer interrupt is the interrupt 4 */
         interrupt = 4;

         /* add simulated interrupt to the interrupt queue */
         ciaaLibs_circBufPut(OSEK_IntCircBuf, &interrupt, 1);

         /* indicate interrupt using a signal */
         kill(getppid(), SIGALRM);
      }
   }
   exit(0);
}

void OsekKillSigHandler(int status)
{
   PreCallService();
   exit(0);
   PostCallService();
}

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/
