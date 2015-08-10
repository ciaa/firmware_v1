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

/** \brief Speech Recognition Tesis main source file
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
 * MaMar        Mariano Marufo
 */

/*
 * modification history (new versions first)
 * -----------------------------------------------------------
 * 20150803 v0.0.1   MaMar   initials initial version
 */

/*==================[inclusions]=============================================*/
#include "os.h"               /* <= operating system header */
#include "ciaaPOSIX_stdio.h"  /* <= device handler header */
#include "ciaaPOSIX_string.h" /* <= string header */
#include "ciaak.h"            /* <= ciaa kernel header */
#include "tesis.h"         	  /* <= own header */


/*==================[macros and definitions]=================================*/
//#define SAMPLE_RATE 16000   /* Hz */
//#define DATA_SIZE   16      /* bits */
#define LPC_SSP         LPC_SSP1
#define SSP_DATA_BITS   (SSP_BITS_8)
#define BITRATE 2400 // in kbps
#define WINDOWSIZE 25 // in miliseconds
#define MEMDMASIZE  BITRATE*WINDOWSIZE/8/2  // in bytes - MAX: 4096
#define LPC_GPDMA_SSP_TX  GPDMA_CONN_SSP1_Tx
#define LPC_GPDMA_SSP_RX  GPDMA_CONN_SSP1_Rx

/*==================[internal data declaration]==============================*/

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/
/** \brief File descriptor for digital input ports
 *
 * Device path /dev/dio/in/0
 */
static int32_t fd_in;

/** \brief File descriptor for digital output ports
 *
 * Device path /dev/dio/out/0
 */
static int32_t fd_out;

/** \brief File descriptor of the USB uart
 *
 * Device path /dev/serial/uart/1
 */
static int32_t fd_uart1;

/** \brief File descriptor of the RS232 uart
 *
 * Device path /dev/serial/uart/2
 */
static int32_t fd_uart2;

/** \brief Periodic Task Counter
 *
 */
static uint32_t Periodic_Task_Counter;

/** \brief
 *
 */
static SSP_ConfigFormat ssp_format;

/** \brief
 *
 */
static uint8_t dmaChSSPTx, dmaChSSPRx;

/** \brief First memory buffer for window 1
 * Maximum DMA transference size: 4096 =>
 * => Two memory buffers used for each window
 */
static uint8_t memDest1ADMA[MEMDMASIZE];

/** \brief Second memory buffer for window 1
 * Maximum DMA transference size: 4096 =>
 * => Two memory buffers used for each window
 */
static uint8_t memDest1BDMA[MEMDMASIZE];

/** \brief First memory buffer for window 2
 * Maximum DMA transference size: 4096 =>
 * => Two memory buffers used for each window
 */
static uint8_t memDest2ADMA[MEMDMASIZE];

/** \brief Second memory buffer for window 2
 * Maximum DMA transference size: 4096 =>
 * => Two memory buffers used for each window
 */
static uint8_t memDest2BDMA[MEMDMASIZE];

/** \brief
 *
 */
static uint8_t currentDMA;

/** \brief Dummy transmission data used to generate the SSP transmission
 *
 */
static uint8_t memDMATx;

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/

/*==================[external functions definition]==========================*/
/** \brief Main function
 *
 * This is the main entry point of the software.
 *
 * \returns 0
 *
 * \remarks This function never returns. Return value is only to avoid compiler
 *          warnings or errors.
 */
int main(void)
{
   /* Starts the operating system in the Application Mode 1 */
   /* This example has only one Application Mode */
   StartOS(AppMode1);

   /* StartOs shall never returns, but to avoid compiler warnings or errors
    * 0 is returned */
   return 0;
}

/** \brief Error Hook function
 *
 * This fucntion is called from the os if an os interface (API) returns an
 * error. Is for debugging proposes. If called this function triggers a
 * ShutdownOs which ends in a while(1).
 *
 * The values:
 *    OSErrorGetServiceId
 *    OSErrorGetParam1
 *    OSErrorGetParam2
 *    OSErrorGetParam3
 *    OSErrorGetRet
 *
 * will provide you the interface, the input parameters and the returned value.
 * For more details see the OSEK specification:
 * http://portal.osek-vdx.org/files/pdf/specs/os223.pdf
 *
 */
void ErrorHook(void)
{
   ciaaPOSIX_printf("ErrorHook was called\n");
   ciaaPOSIX_printf("Service: %d, P1: %d, P2: %d, P3: %d, RET: %d\n", OSErrorGetServiceId(), OSErrorGetParam1(), OSErrorGetParam2(), OSErrorGetParam3(), OSErrorGetRet());
   ShutdownOS(0);
}

/** \brief Initial task
 *
 * This task is started automatically in the application mode 1.
 */
TASK(InitTask)
{
   /* init CIAA kernel and devices */
   ciaak_start();

   /* open CIAA digital inputs */
   fd_in = ciaaPOSIX_open("/dev/dio/in/0", O_RDONLY);

   /* open CIAA digital outputs */
   fd_out = ciaaPOSIX_open("/dev/dio/out/0", O_RDWR);

   /* open UART connected to USB bridge (FT2232) */
   fd_uart1 = ciaaPOSIX_open("/dev/serial/uart/1", O_RDWR);

   /* open UART connected to RS232 connector */
   fd_uart2 = ciaaPOSIX_open("/dev/serial/uart/2", O_RDWR);

   /* change baud rate for uart usb */
   ciaaPOSIX_ioctl(fd_uart1, ciaaPOSIX_IOCTL_SET_BAUDRATE, (void *)ciaaBAUDRATE_115200);

   /* change FIFO TRIGGER LEVEL for uart usb */
   ciaaPOSIX_ioctl(fd_uart1, ciaaPOSIX_IOCTL_SET_FIFO_TRIGGER_LEVEL, (void *)ciaaFIFO_TRIGGER_LEVEL3);

   /* activate example tasks */
   Periodic_Task_Counter = 0;
   SetRelAlarm(ActivatePeriodicTask, 200, 5);

   /* Activates the SerialEchoTask task */
   ActivateTask(SerialEchoTask);


   /* SSP DMA Read and Write: fixed on 8bits */

   /* Initialize CIAA Pins for the SSP interface */

   if (LPC_SSP == LPC_SSP1) {
      Chip_SCU_PinMuxSet(0x1, 5, (SCU_PINIO_FAST | SCU_MODE_FUNC5));  /* P1.5 => SSEL1 (not connected to microphone) */
      Chip_SCU_PinMuxSet(0xF, 4, (SCU_PINIO_FAST | SCU_MODE_FUNC0));  /* PF.4 => SCK1 */

   	  Chip_SCU_PinMuxSet(0x1, 4, (SCU_MODE_INACT | SCU_MODE_INBUFF_EN | SCU_MODE_ZIF_DIS | SCU_MODE_FUNC5)); /* P1.4 => MOSI1 (not connected to microphone) */
   	  Chip_SCU_PinMuxSet(0x1, 3, (SCU_MODE_INACT | SCU_MODE_INBUFF_EN | SCU_MODE_ZIF_DIS | SCU_MODE_FUNC5)); /* P1.3 => MISO1 */
   }
   else {
      while(1){}
   }


   /* Initialize the SSP interface */

   Chip_SSP_Init(LPC_SSP);
   Chip_SSP_SetBitRate(LPC_SSP, BITRATE*1000);

   /* Configure SSP Format */
   ssp_format.frameFormat = SSP_FRAMEFORMAT_SPI;
   ssp_format.bits = SSP_DATA_BITS;
   ssp_format.clockMode = SSP_CLOCK_MODE3;
   Chip_SSP_SetFormat(LPC_SSP, ssp_format.bits, ssp_format.frameFormat, ssp_format.clockMode);

   Chip_SSP_Enable(LPC_SSP);

   /* Initialize GPDMA controller */
   Chip_GPDMA_Init(LPC_GPDMA);

   /* Setting GPDMA interrupt */
   NVIC_DisableIRQ(DMA_IRQn);
   NVIC_SetPriority(DMA_IRQn, ((0x01 << 3) | 0x01));
   NVIC_EnableIRQ(DMA_IRQn);

   /* Set the SSP in master mode */
   Chip_SSP_SetMaster(LPC_SSP, 1);

   /* Get a free GPDMA channel for one DMA connection for transmission */
   dmaChSSPTx = Chip_GPDMA_GetFreeChannel(LPC_GPDMA, LPC_GPDMA_SSP_TX);

   /* Get a free GPDMA channel for one DMA connection for reception */
   dmaChSSPRx = Chip_GPDMA_GetFreeChannel(LPC_GPDMA, LPC_GPDMA_SSP_RX);

   Chip_SSP_DMA_Enable(LPC_SSP);

   /* Do a DMA transfer P2M: data SSP --> memDest2BDMA */
   Chip_GPDMA_Transfer(LPC_GPDMA, dmaChSSPRx,
   				  LPC_GPDMA_SSP_RX, // source
   				  (uint32_t) &memDest2BDMA[0], // destination
   				  GPDMA_TRANSFERTYPE_P2M_CONTROLLER_DMA,
   				  MEMDMASIZE);

   /* Do a DMA transfer P2M: memDMATx --> SSP */
   Chip_GPDMA_Transfer(LPC_GPDMA, dmaChSSPTx,
   				  (uint32_t) &memDMATx, // source
   				  LPC_GPDMA_SSP_TX, // destination
   				  GPDMA_TRANSFERTYPE_M2P_CONTROLLER_DMA,
   				  MEMDMASIZE);

   currentDMA = 1;

   /* end InitTask */
   TerminateTask();
}

/** \brief DMA interrupt handler sub-routine
 *
 */
ISR(DMA_IRQHandler)
{

   if(Chip_GPDMA_Interrupt(LPC_GPDMA, dmaChSSPRx) == SUCCESS){

	   if (currentDMA == 1){

		   /* Do a DMA transfer P2M: data SSP --> memDest1ADMA */
		   Chip_GPDMA_Transfer(LPC_GPDMA, dmaChSSPRx,
		   				  LPC_GPDMA_SSP_RX, // source
		   				  (uint32_t) &memDest1ADMA[0], // destination
		   				  GPDMA_TRANSFERTYPE_P2M_CONTROLLER_DMA,
		   				  MEMDMASIZE);

		   currentDMA = 2;
	   }
	   else if (currentDMA == 2){

		   /* Do a DMA transfer P2M: data SSP --> memDest1BDMA */
		   Chip_GPDMA_Transfer(LPC_GPDMA, dmaChSSPRx,
		   				  LPC_GPDMA_SSP_RX, // source
		   				  (uint32_t) &memDest1BDMA[0], // destination
		   				  GPDMA_TRANSFERTYPE_P2M_CONTROLLER_DMA,
		   				  MEMDMASIZE);


		   currentDMA = 3;
	   }
	   else if (currentDMA == 3){

		   /* Do a DMA transfer P2M: data SSP --> memDest2ADMA */
		   Chip_GPDMA_Transfer(LPC_GPDMA, dmaChSSPRx,
		   				  LPC_GPDMA_SSP_RX, // source
		   				  (uint32_t) &memDest2ADMA[0], // destination
		   				  GPDMA_TRANSFERTYPE_P2M_CONTROLLER_DMA,
		   				  MEMDMASIZE);


		   currentDMA = 4;
	   }
	   else{ //currentDMA = 4

		   /* Do a DMA transfer P2M: data SSP --> memDest2BDMA */
		   Chip_GPDMA_Transfer(LPC_GPDMA, dmaChSSPRx,
		   				  LPC_GPDMA_SSP_RX, // source
		   				  (uint32_t) &memDest2BDMA[0], // destination
		   				  GPDMA_TRANSFERTYPE_P2M_CONTROLLER_DMA,
		   				  MEMDMASIZE);


		   currentDMA = 1;
	   }

   }
   else if (Chip_GPDMA_Interrupt(LPC_GPDMA, dmaChSSPTx) == SUCCESS){

	   /* Do a DMA transfer P2M: memDMATx --> SSP */
	   Chip_GPDMA_Transfer(LPC_GPDMA, dmaChSSPTx,
	   				  (uint32_t) &memDMATx, // source
	   				  LPC_GPDMA_SSP_TX, // destination
	   				  GPDMA_TRANSFERTYPE_M2P_CONTROLLER_DMA,
	   				  MEMDMASIZE);

   }

}

/** \brief Serial Echo Task
 *
 * This tasks waits for input data from fd_uart1 and writes the received data
 * to fd_uart1 and fd_uart2. This taks alos blinkgs the output 5.
 *
 */
TASK(SerialEchoTask)
{
   int8_t buf[20];   /* buffer for uart operation              */
   uint8_t outputs;  /* to store outputs status                */
   int32_t ret;      /* return value variable for posix calls  */

   /* send a message to the world :) */
//   char message[] = "Hi! :)\nSerialEchoTask: Waiting for characters...\n";
//   ciaaPOSIX_write(fd_uart1, message, ciaaPOSIX_strlen(message));

   while(1)
   {
      /* wait for any character ... */
      ret = ciaaPOSIX_read(fd_uart1, buf, 20);

      if(ret > 0)
      {
         /* ... and write them to the same device */
         ciaaPOSIX_write(fd_uart1, buf, ret);

         /* also write them to the other device */
         ciaaPOSIX_write(fd_uart2, buf, ret);
      }

      /* blink output 5 with each loop */
      ciaaPOSIX_read(fd_out, &outputs, 1);
      outputs ^= 0x20;
      ciaaPOSIX_write(fd_out, &outputs, 1);
   }
}

/** \brief Periodic Task
 *
 * This task is activated by the Alarm ActivatePeriodicTask.
 * This task copies the status of the inputs bits 0..3 to the output bits 0..3.
 * This task also blinks the output 4
 */
TASK(PeriodicTask)
{
   /*
    * Example:
    *    Read inputs 0..3, update outputs 0..3.
    *    Blink output 4
    */

   /* variables to store input/output status */
   uint8_t inputs = 0, outputs = 0;

   /* read inputs */
   ciaaPOSIX_read(fd_in, &inputs, 1);

   /* read outputs */
   ciaaPOSIX_read(fd_out, &outputs, 1);

   /* update outputs with inputs */
   outputs &= 0xF0;
   outputs |= inputs & 0x0F;

   /* blink */
   outputs ^= 0x10;

   /* write */
   ciaaPOSIX_write(fd_out, &outputs, 1);

   /* Print Task info */
   Periodic_Task_Counter++;

   /* end PeriodicTask */
   TerminateTask();
}

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/

