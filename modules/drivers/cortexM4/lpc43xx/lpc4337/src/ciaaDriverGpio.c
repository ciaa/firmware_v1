/* Copyright 2015, Juan Pablo Vecchio (UNR-FCEIA)
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

/** \brief CIAA GPIO Driver for LPC4337
 **
 ** Implements the General Purpose Input/Output (GPIO) Driver for LPC4337
 **
 **/

/** \addtogroup CIAA_Firmware CIAA Firmware
 ** @{ */
/** \addtogroup Drivers CIAA Drivers
 ** @{ */
/** \addtogroup GPIO GPIO Drivers
 ** @{ */

/*
 * Initials     Name
 * ---------------------------
 * JPV           Juan Pablo Vecchio
 */

/*
 * modification history (new versions first)
 * -----------------------------------------------------------
 * 20150522 v0.0.1 initials initial version
 */

/*==================[inclusions]=============================================*/
#include "ciaaDriverGpio.h"
#include "ciaaDriverGpio_Internal.h"
#include "ciaaPOSIX_stdlib.h"
#include "ciaaPOSIX_string.h"
#include "chip.h"

/*==================[macros and definitions]=================================*/
#define GPIO_IN						(0x1)		/** Enable input buffer */

/** \brief Pointer to Devices */
typedef struct  {
   ciaaDevices_deviceType * const * const devices;
   uint8_t countOfDevices;
} ciaaDriverConstType;

/*==================[internal data declaration]==============================*/

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/
/** \brief Device for GPIO 0 */
static ciaaDevices_deviceType ciaaDriverGpio_in0 = {
   "in/0",                          /** <= driver name */
   ciaaDriverGpio_open,             /** <= open function */
   ciaaDriverGpio_close,            /** <= close function */
   ciaaDriverGpio_read,             /** <= read function */
   ciaaDriverGpio_write,            /** <= write function */
   ciaaDriverGpio_ioctl,            /** <= ioctl function */
   NULL,                           /** <= seek function is not provided */
   NULL,                           /** <= upper layer */
   (void*)&ciaaDriverGpio_gpio0,     /** <= layer */
   NULL                            /** <= NULL no lower layer */
};

/** \brief Device for GPIO 1 */
static ciaaDevices_deviceType ciaaDriverGpio_out0 = {
   "out/0",                          /** <= driver name */
   ciaaDriverGpio_open,             /** <= open function */
   ciaaDriverGpio_close,            /** <= close function */
   ciaaDriverGpio_read,             /** <= read function */
   ciaaDriverGpio_write,            /** <= write function */
   ciaaDriverGpio_ioctl,            /** <= ioctl function */
   NULL,                           /** <= seek function is not provided */
   NULL,                           /** <= upper layer */
   (void*)&ciaaDriverGpio_gpio1,     /** <= layer */
   NULL                            /** <= NULL no lower layer */
};

static ciaaDevices_deviceType * const ciaaGpioDevices[] = {
   &ciaaDriverGpio_in0,
   &ciaaDriverGpio_out0,
};

static ciaaDriverConstType const ciaaDriverGpioConst = {
   ciaaGpioDevices,
   2
};

/*==================[external data definition]===============================*/
/** \brief Gpio 0 */
ciaaDriverGpio_gpioType ciaaDriverGpio_gpio0;

/** \brief Gpio 1 */
ciaaDriverGpio_gpioType ciaaDriverGpio_gpio1;

/*==================[internal functions definition]==========================*/

void ciaa_lpc4337_gpio_init2(void)
{
   Chip_GPIO_Init(LPC_GPIO_PORT);

#if (BOARD == ciaa_nxp)
   /* GPIO */

#elif (BOARD == edu_ciaa_nxp)
   /* GPIO  Inicializo como salida */

#if (CIAA_PNCH_USER_INTERFACE == none)

   Chip_SCU_PinMux(6,1,MD_PUP,FUNC0);   /* GPIO3[0],  GPIO0 */
   Chip_SCU_PinMux(6,4,MD_PUP,FUNC0);   /* GPIO3[3],  GPIO1 */
   Chip_SCU_PinMux(6,5,MD_PUP,FUNC0);   /* GPIO3[4],  GPIO2 */
   Chip_SCU_PinMux(6,7,MD_PUP,FUNC4);   /* GPIO5[15], GPIO3 */
   Chip_SCU_PinMux(6,8,MD_PUP,FUNC4);   /* GPIO5[16], GPIO4 */
   Chip_SCU_PinMux(6,9,MD_PUP,FUNC0);   /* GPIO3[5],  GPIO5 */
   Chip_SCU_PinMux(6,10,MD_PUP,FUNC0);  /* GPIO3[6],  GPIO6 */
   Chip_SCU_PinMux(6,11,MD_PUP,FUNC0);  /* GPIO3[7],  GPIO7 */
   Chip_SCU_PinMux(6,12,MD_PUP,FUNC0);  /* GPIO2[8],  GPIO8 */

   Chip_GPIO_SetDir(LPC_GPIO_PORT, 3,(1<<0)|(1<<3)|(1<<4)|(1<<5)|(1<<6)|(1<<7),1);
   Chip_GPIO_SetDir(LPC_GPIO_PORT, 5,(1<<15)|(1<<16),1);
   Chip_GPIO_SetDir(LPC_GPIO_PORT, 2,(1<<8),1);

   Chip_GPIO_ClearValue(LPC_GPIO_PORT, 3,(1<<0)|(1<<3)|(1<<4)|(1<<5)|(1<<6)|(1<<7));
   Chip_GPIO_ClearValue(LPC_GPIO_PORT, 5,(1<<15)|(1<<16));
   Chip_GPIO_ClearValue(LPC_GPIO_PORT, 2,(1<<8));
#endif
#else
   #error please define BOARD variable!
#endif
}

/* Escritura Puertos salida */
void ciaa_lpc4337_writeGPIO(uint32_t outputNumber, uint32_t value)
{
#if (BOARD == edu_ciaa_nxp)
   switch(outputNumber)
   {
      case 0: /* GPIO0 */
         if(value)
         {
            Chip_GPIO_SetValue(LPC_GPIO_PORT, 3, 1<<0);
         }
         else
         {
            Chip_GPIO_ClearValue(LPC_GPIO_PORT, 3, 1<<0);
         }
         break;
      case 1: /* GPIO1 */
         if(value)
         {
            Chip_GPIO_SetValue(LPC_GPIO_PORT, 3, 1<<3);
         }
         else
         {
            Chip_GPIO_ClearValue(LPC_GPIO_PORT, 3, 1<<3);
         }
         break;
      case 2: /* GPIO2 */
         if(value)
         {
            Chip_GPIO_SetValue(LPC_GPIO_PORT, 3, 1<<4);
         }
         else
         {
            Chip_GPIO_ClearValue(LPC_GPIO_PORT, 3, 1<<4);
         }
         break;
      case 3: /* GPIO3 */
         if(value)
         {
            Chip_GPIO_SetValue(LPC_GPIO_PORT, 5, 1<<15);
         }
         else
         {
            Chip_GPIO_ClearValue(LPC_GPIO_PORT, 5, 1<<15);
         }
         break;
      case 4: /* GPIO4 */
         if(value)
         {
            Chip_GPIO_SetValue(LPC_GPIO_PORT, 5, 1<<16);
         }
         else
         {
            Chip_GPIO_ClearValue(LPC_GPIO_PORT, 5, 1<<16);
         }
         break;
      case 5: /* GPIO5 */
         if(value)
         {
            Chip_GPIO_SetValue(LPC_GPIO_PORT, 3, 1<<5);
         }
         else
         {
            Chip_GPIO_ClearValue(LPC_GPIO_PORT, 3, 1<<5);
         }
         break;
      case 6: /* GPIO6 */
         if(value)
         {
            Chip_GPIO_SetValue(LPC_GPIO_PORT, 3, 1<<6);
         }
         else
         {
            Chip_GPIO_ClearValue(LPC_GPIO_PORT, 3, 1<<6);
         }
         break;
      case 7: /* GPIO7 */
         if(value)
         {
            Chip_GPIO_SetValue(LPC_GPIO_PORT, 3, 1<<7);
         }
         else
         {
            Chip_GPIO_ClearValue(LPC_GPIO_PORT, 3, 1<<7);
         }
         break;
      case 8: /* GPIO8 */
         if(value)
         {
            Chip_GPIO_SetValue(LPC_GPIO_PORT, 2, 1<<8);
         }
         else
         {
            Chip_GPIO_ClearValue(LPC_GPIO_PORT, 2, 1<<8);
         }
         break;
   }
#endif
}

/*==================[external functions definition]==========================*/
extern ciaaDevices_deviceType * ciaaDriverGpio_open(char const * path,
      ciaaDevices_deviceType * device, uint8_t const oflag)
{
   return device;
}

extern int32_t ciaaDriverGpio_close(ciaaDevices_deviceType const * const device)
{
   return 0;
}

extern int32_t ciaaDriverGpio_ioctl(ciaaDevices_deviceType const * const device, int32_t const request, void * param)
{
   ssize_t ret = -1;

   if ((device == ciaaDriverGpioConst.devices[0]) ||
       (device == ciaaDriverGpioConst.devices[1]))
   {

   switch(request)
   {
      case ciaaPOSIX_IOCTL_GPIO_IN:

#if (BOARD == edu_ciaa_nxp)
         if(((int32_t)param)&ciaaGPIO_0)
         {
        	 Chip_SCU_PinMux(6,1,MD_PUP|MD_EZI|MD_ZI,FUNC0);   /* GPIO3[0],  GPIO0 */
        	 Chip_GPIO_SetDir(LPC_GPIO_PORT, 3,(1<<0),0);
        	 ret=1;
         }
         if(((int32_t)param)&ciaaGPIO_1)
         {
        	 Chip_SCU_PinMux(6,4,MD_PUP|MD_EZI|MD_ZI,FUNC0);   /* GPIO3[3],  GPIO1 */
        	 Chip_GPIO_SetDir(LPC_GPIO_PORT, 3,(1<<3),0);
        	 ret=1;
         }
         if(((int32_t)param)&ciaaGPIO_2)
         {
        	 Chip_SCU_PinMux(6,5,MD_PUP|MD_EZI|MD_ZI,FUNC0);   /* GPIO3[4],  GPIO2 */
        	 Chip_GPIO_SetDir(LPC_GPIO_PORT, 3,(1<<4),0);
        	 ret=1;
         }
    	 if(((int32_t)param)&ciaaGPIO_3)
    	 {
    		 Chip_SCU_PinMux(6,7,MD_PUP|MD_EZI|MD_ZI,FUNC4);   /* GPIO5[15], GPIO3 */
    		 Chip_GPIO_SetDir(LPC_GPIO_PORT, 5,(1<<15),0);
    		 ret=1;
    	 }
    	 if(((int32_t)param)&ciaaGPIO_4)
    	 {
    		 Chip_SCU_PinMux(6,8,MD_PUP|MD_EZI|MD_ZI,FUNC4);   /* GPIO5[16], GPIO4 */
    		 Chip_GPIO_SetDir(LPC_GPIO_PORT, 5,(1<<16),0);
    		 ret=1;
    	 }
    	 if(((int32_t)param)&ciaaGPIO_5)
    	 {
    		 Chip_SCU_PinMux(6,9,MD_PUP|MD_EZI|MD_ZI,FUNC0);   /* GPIO3[5],  GPIO5 */
    		 Chip_GPIO_SetDir(LPC_GPIO_PORT, 3,(1<<5),0);
    		 ret=1;
    	 }
    	 if(((int32_t)param)&ciaaGPIO_6)
    	 {
    		 Chip_SCU_PinMux(6,10,MD_PUP|MD_EZI|MD_ZI,FUNC0);  /* GPIO3[6],  GPIO6 */
    		 Chip_GPIO_SetDir(LPC_GPIO_PORT, 3,(1<<6),0);
    		 ret=1;
    	 }
    	 if(((int32_t)param)&ciaaGPIO_7)
    	 {
    		 Chip_SCU_PinMux(6,11,MD_PUP|MD_EZI|MD_ZI,FUNC0);  /* GPIO3[7],  GPIO7 */
    		 Chip_GPIO_SetDir(LPC_GPIO_PORT, 3,(1<<7),0);
    		 ret=1;
    	 }
    	 if(((int32_t)param)&ciaaGPIO_8)
    	 {
    		 Chip_SCU_PinMux(6,12,MD_PUP|MD_EZI|MD_ZI,FUNC0);  /* GPIO2[8],  GPIO8 */
    		 Chip_GPIO_SetDir(LPC_GPIO_PORT, 2,(1<<8),0);
    		 ret=1;
    	 }
    	 ret=1;
#else
      #error please define BOARD variable!
#endif

    	 break;
      case ciaaPOSIX_IOCTL_GPIO_OUT:

#if (BOARD == edu_ciaa_nxp)
         if(((int32_t)param)&ciaaGPIO_0)
         {
        	 Chip_SCU_PinMux(6,1,MD_PUP,FUNC0);   /* GPIO3[0],  GPIO0 */
        	 Chip_GPIO_SetDir(LPC_GPIO_PORT, 3,(1<<0),1);
        	 Chip_GPIO_ClearValue(LPC_GPIO_PORT, 3,(1<<0));
        	 ret=1;
         }
         if(((int32_t)param)&ciaaGPIO_1)
         {
        	 Chip_SCU_PinMux(6,4,MD_PUP,FUNC0);   /* GPIO3[3],  GPIO1 */
        	 Chip_GPIO_SetDir(LPC_GPIO_PORT, 3,(1<<3),1);
        	 Chip_GPIO_ClearValue(LPC_GPIO_PORT, 3,(1<<3));
        	 ret=1;
         }
         if(((int32_t)param)&ciaaGPIO_2)
         {
        	 Chip_SCU_PinMux(6,5,MD_PUP,FUNC0);   /* GPIO3[4],  GPIO2 */
        	 Chip_GPIO_SetDir(LPC_GPIO_PORT, 3,(1<<4),1);
        	 Chip_GPIO_ClearValue(LPC_GPIO_PORT, 3,(1<<4));
        	 ret=1;
         }
    	 if(((int32_t)param)&ciaaGPIO_3)
    	 {
    		 Chip_SCU_PinMux(6,7,MD_PUP,FUNC4);   /* GPIO5[15], GPIO3 */
    		 Chip_GPIO_SetDir(LPC_GPIO_PORT, 5,(1<<15),1);
    		 Chip_GPIO_ClearValue(LPC_GPIO_PORT, 5,(1<<15));
    		 ret=1;
    	 }
    	 if(((int32_t)param)&ciaaGPIO_4)
    	 {
    		 Chip_SCU_PinMux(6,8,MD_PUP,FUNC4);   /* GPIO5[16], GPIO4 */
    		 Chip_GPIO_SetDir(LPC_GPIO_PORT, 5,(1<<16),1);
    		 Chip_GPIO_ClearValue(LPC_GPIO_PORT, 5,(1<<16));
    		 ret=1;
    	 }
    	 if(((int32_t)param)&ciaaGPIO_5)
    	 {
    		 Chip_SCU_PinMux(6,9,MD_PUP,FUNC0);   /* GPIO3[5],  GPIO5 */
    		 Chip_GPIO_SetDir(LPC_GPIO_PORT, 3,(1<<5),1);
    		 Chip_GPIO_ClearValue(LPC_GPIO_PORT, 3,(1<<5));
    		 ret=1;
    	 }
    	 if(((int32_t)param)&ciaaGPIO_6)
    	 {
    		 Chip_SCU_PinMux(6,10,MD_PUP,FUNC0);  /* GPIO3[6],  GPIO6 */
    		 Chip_GPIO_SetDir(LPC_GPIO_PORT, 3,(1<<6),1);
    		 Chip_GPIO_ClearValue(LPC_GPIO_PORT, 3,(1<<6));
    		 ret=1;
    	 }
    	 if(((int32_t)param)&ciaaGPIO_7)
    	 {
    		 Chip_SCU_PinMux(6,11,MD_PUP,FUNC0);  /* GPIO3[7],  GPIO7 */
    		 Chip_GPIO_SetDir(LPC_GPIO_PORT, 3,(1<<7),1);
    		 Chip_GPIO_ClearValue(LPC_GPIO_PORT, 3,(1<<7));
    		 ret=1;
    	 }
    	 if(((int32_t)param)&ciaaGPIO_8)
    	 {
    		 Chip_SCU_PinMux(6,12,MD_PUP,FUNC0);  /* GPIO2[8],  GPIO8 */
    		 Chip_GPIO_SetDir(LPC_GPIO_PORT, 2,(1<<8),1);
    		 Chip_GPIO_ClearValue(LPC_GPIO_PORT, 2,(1<<8));
    		 ret=1;
    	 }
#else
      #error please define BOARD variable!
#endif
   }
   }
   	return ret;
}

extern uint16_t ciaaDriverGpio_read(ciaaDevices_deviceType const * const device, uint16_t * buffer, size_t size)
{
   ssize_t ret = -1;

   /* Can't store read result in buffer. At least 1 byte required. */
   if(size != 0)
   {
      if(device == ciaaGpioDevices[0])
      {
#if(BOARD == edu_ciaa_nxp)
    	 buffer[0] = 0;
         buffer[0] |= Chip_GPIO_GetPinState(LPC_GPIO_PORT, 3, 0)  ? 1 : 0;
         buffer[0] |= Chip_GPIO_GetPinState(LPC_GPIO_PORT, 3, 3)  ? 2 : 0;
         buffer[0] |= Chip_GPIO_GetPinState(LPC_GPIO_PORT, 3, 4)  ? 4 : 0;
         buffer[0] |= Chip_GPIO_GetPinState(LPC_GPIO_PORT, 5, 15) ? 8 : 0;
         buffer[0] |= Chip_GPIO_GetPinState(LPC_GPIO_PORT, 5, 16) ? 16 : 0;
         buffer[0] |= Chip_GPIO_GetPinState(LPC_GPIO_PORT, 3, 5)  ? 32 : 0;
         buffer[0] |= Chip_GPIO_GetPinState(LPC_GPIO_PORT, 3, 6)  ? 64 : 0;
         buffer[0] |= Chip_GPIO_GetPinState(LPC_GPIO_PORT, 3, 7)  ? 128 : 0;
         buffer[0] |= Chip_GPIO_GetPinState(LPC_GPIO_PORT, 2, 8)  ? 256 : 0;
#endif

         /* 1 byte read */
         ret = 1;
      }
      else if(device == ciaaGpioDevices[1])
      {
         /* read actual output state from layer data */
         buffer[0] = (uint16_t)*((ciaaDriverGpio_gpioType *)device->layer);

         ret = 1;
      }
      else
      {
         /* Invalid device */
         ret = -1;
      }
   }
   return ret;
}

extern uint16_t ciaaDriverGpio_write(ciaaDevices_deviceType const * const device, uint16_t const * const buffer, size_t const size)
{
   ssize_t ret = -1;

   if(size != 0)
   {
      if(device == ciaaGpioDevices[0])
      {
         /* Inputs can't be written. */
         ret = -1;
      }
      else if(device == ciaaGpioDevices[1])
      {
         int32_t i;

         for(i = 0; i < 9; i++)
         {
            ciaa_lpc4337_writeGPIO(i, buffer[0] & (1 << i));
         }

         /* save actual output state in layer data */
         *((ciaaDriverGpio_gpioType *)device->layer) = buffer[0];

         /* 1 byte written */
         ret = 1;
      }
      else
      {
         ret = -1;
      }
   }
   return ret;
}

void ciaaDriverGpio_init(void)
{
   uint8_t loopi;

   ciaa_lpc4337_gpio_init2();

   /* add dio driver to the list of devices */
   for(loopi = 0; loopi < ciaaDriverGpioConst.countOfDevices; loopi++) {
      /* add each device */
      ciaaGpioDevices_addDriver(ciaaDriverGpioConst.devices[loopi]);
      /* init layer data for each device */
      *((ciaaDriverGpio_gpioType *)ciaaDriverGpioConst.devices[loopi]->layer) = 0;
   }
}


/*==================[interrupt hanlders]=====================================*/

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/

