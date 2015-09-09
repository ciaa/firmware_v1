/* Copyright 2014, Pablo Ridolfi (UTN-FRBA)
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

/** \brief CIAA Dio Driver for LPC4337
 **
 ** Implements the Digital Input/Output (Dio) Driver for LPC4337
 **
 **/

/** \addtogroup CIAA_Firmware CIAA Firmware
 ** @{ */
/** \addtogroup Drivers CIAA Drivers
 ** @{ */
/** \addtogroup DIO DIO Drivers
 ** @{ */

/*
 * Initials     Name
 * ---------------------------
 * PR           Pablo Ridolfi
 */

/*
 * modification history (new versions first)
 * -----------------------------------------------------------
 * 20140731 v0.0.1   PR first functional version
 */

/*==================[inclusions]=============================================*/
#include "ciaaDriverDio.h"
#include "ciaaDriverDio_Internal.h"
#include "ciaaPOSIX_stdlib.h"
#include "ciaaPOSIX_string.h"
#include "chip.h"

/*==================[macros and definitions]=================================*/
/** \brief Pointer to Devices */
typedef struct  {
   ciaaDevices_deviceType * const * const devices;
   uint8_t countOfDevices;
} ciaaDriverConstType;

/*==================[internal data declaration]==============================*/

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/
/** \brief Device for DIO 0 */
static ciaaDevices_deviceType ciaaDriverDio_in0 = {
   "in/0",                          /** <= driver name */
   ciaaDriverDio_open,             /** <= open function */
   ciaaDriverDio_close,            /** <= close function */
   ciaaDriverDio_read,             /** <= read function */
   ciaaDriverDio_write,            /** <= write function */
   ciaaDriverDio_ioctl,            /** <= ioctl function */
   NULL,                           /** <= seek function is not provided */
   NULL,                           /** <= upper layer */
   (void*)&ciaaDriverDio_dio0,     /** <= layer */
   NULL                            /** <= NULL no lower layer */
};

/** \brief Device for DIO 1 */
static ciaaDevices_deviceType ciaaDriverDio_out0 = {
   "out/0",                          /** <= driver name */
   ciaaDriverDio_open,             /** <= open function */
   ciaaDriverDio_close,            /** <= close function */
   ciaaDriverDio_read,             /** <= read function */
   ciaaDriverDio_write,            /** <= write function */
   ciaaDriverDio_ioctl,            /** <= ioctl function */
   NULL,                           /** <= seek function is not provided */
   NULL,                           /** <= upper layer */
   (void*)&ciaaDriverDio_dio1,     /** <= layer */
   NULL                            /** <= NULL no lower layer */
};

static ciaaDevices_deviceType * const ciaaDioDevices[] = {
   &ciaaDriverDio_in0,
   &ciaaDriverDio_out0
};

static ciaaDriverConstType const ciaaDriverDioConst = {
   ciaaDioDevices,
   2
};

/*==================[external data definition]===============================*/
/** \brief Dio 0 */
ciaaDriverDio_dioType ciaaDriverDio_dio0;

/** \brief Dio 1 */
ciaaDriverDio_dioType ciaaDriverDio_dio1;

/*==================[internal functions definition]==========================*/

void ciaa_lpc4337_gpio_init(void)
{
   Chip_GPIO_Init(LPC_GPIO_PORT);

#if (BOARD == ciaa_nxp)
   /* Inputs */
   Chip_SCU_PinMux(4,0,MD_PUP|MD_EZI|MD_ZI,FUNC0); /* GPIO2[0]  */
   Chip_SCU_PinMux(4,1,MD_PUP|MD_EZI|MD_ZI,FUNC0); /* GPIO2[1]  */
   Chip_SCU_PinMux(4,2,MD_PUP|MD_EZI|MD_ZI,FUNC0); /* GPIO2[2]  */
   Chip_SCU_PinMux(4,3,MD_PUP|MD_EZI|MD_ZI,FUNC0); /* GPIO2[3]  */
   Chip_SCU_PinMux(7,3,MD_PUP|MD_EZI|MD_ZI,FUNC0); /* GPIO3[11] */
   Chip_SCU_PinMux(7,4,MD_PUP|MD_EZI|MD_ZI,FUNC0); /* GPIO3[12] */
   Chip_SCU_PinMux(7,5,MD_PUP|MD_EZI|MD_ZI,FUNC0); /* GPIO3[13] */
   Chip_SCU_PinMux(7,6,MD_PUP|MD_EZI|MD_ZI,FUNC0); /* GPIO3[14] */

   Chip_GPIO_SetDir(LPC_GPIO_PORT, 2,0xF, 0);
   Chip_GPIO_SetDir(LPC_GPIO_PORT, 3, 0xF<<11, 0);

   /* MOSFETs */
   Chip_SCU_PinMux(4,8,MD_PUP,FUNC4);  /* GPIO5[12] */
   Chip_SCU_PinMux(4,9,MD_PUP,FUNC4);  /* GPIO5[13] */
   Chip_SCU_PinMux(4,10,MD_PUP,FUNC4); /* GPIO5[14] */
   Chip_SCU_PinMux(1,5,MD_PUP,FUNC0);  /* GPIO1[8]  */

   Chip_GPIO_SetDir(LPC_GPIO_PORT, 5,(1<<12)|(1<<13)|(1<<14),1);
   Chip_GPIO_SetDir(LPC_GPIO_PORT, 1,(1<<8),1);


   Chip_GPIO_SetValue(LPC_GPIO_PORT, 5,(1<<12)|(1<<13)|(1<<14));
   Chip_GPIO_SetValue(LPC_GPIO_PORT, 1,(1<<8));

   /* Relays */
   Chip_GPIO_SetDir(LPC_GPIO_PORT, 2,(1<<4)|(1<<5)|(1<<6),1);
   Chip_SCU_PinMux(2,1,MD_PUP,FUNC4);
   Chip_GPIO_SetDir(LPC_GPIO_PORT, 5,(1<<1),1);

   Chip_GPIO_ClearValue(LPC_GPIO_PORT, 2,(1<<4)|(1<<5)|(1<<6));
   Chip_GPIO_ClearValue(LPC_GPIO_PORT, 5,(1<<1));

   /* LV-TTL GPIOs (not used yet) */
   Chip_SCU_PinMux(6,1,MD_PUP|MD_EZI|MD_ZI,FUNC0); /* GPIO0/P6_1/GPIO3[0] */
   Chip_SCU_PinMux(2,5,MD_PUP|MD_EZI|MD_ZI,FUNC4); /* GPIO1/P2_5/GPIO5[5] */
#elif (BOARD == edu_ciaa_nxp)
   /* Switches */
   Chip_SCU_PinMux(1,0,MD_PUP|MD_EZI|MD_ZI,FUNC0); /* GPIO0[4], SW1 */
   Chip_SCU_PinMux(1,1,MD_PUP|MD_EZI|MD_ZI,FUNC0); /* GPIO0[8], SW2 */
   Chip_SCU_PinMux(1,2,MD_PUP|MD_EZI|MD_ZI,FUNC0); /* GPIO0[9], SW3 */
   Chip_SCU_PinMux(1,6,MD_PUP|MD_EZI|MD_ZI,FUNC0); /* GPIO1[9], SW4 */

   Chip_GPIO_SetDir(LPC_GPIO_PORT, 0,(1<<4)|(1<<8)|(1<<9),0);
   Chip_GPIO_SetDir(LPC_GPIO_PORT, 1,(1<<9),0);

   /* LEDs */
   Chip_SCU_PinMux(2,0,MD_PUP,FUNC4);  /* GPIO5[0], LED0R */
   Chip_SCU_PinMux(2,1,MD_PUP,FUNC4);  /* GPIO5[1], LED0G */
   Chip_SCU_PinMux(2,2,MD_PUP,FUNC4);  /* GPIO5[2], LED0B */
   Chip_SCU_PinMux(2,10,MD_PUP,FUNC0); /* GPIO0[14], LED1 */
   Chip_SCU_PinMux(2,11,MD_PUP,FUNC0); /* GPIO1[11], LED2 */
   Chip_SCU_PinMux(2,12,MD_PUP,FUNC0); /* GPIO1[12], LED3 */

   Chip_GPIO_SetDir(LPC_GPIO_PORT, 5,(1<<0)|(1<<1)|(1<<2),1);
   Chip_GPIO_SetDir(LPC_GPIO_PORT, 0,(1<<14),1);
   Chip_GPIO_SetDir(LPC_GPIO_PORT, 1,(1<<11)|(1<<12),1);

   Chip_GPIO_ClearValue(LPC_GPIO_PORT, 5,(1<<0)|(1<<1)|(1<<2));
   Chip_GPIO_ClearValue(LPC_GPIO_PORT, 0,(1<<14));
   Chip_GPIO_ClearValue(LPC_GPIO_PORT, 1,(1<<11)|(1<<12));

#else
   #error please define BOARD variable!
#endif
}

void ciaa_lpc4337_writeOutput(uint32_t outputNumber, uint32_t value)
{
#if (BOARD == ciaa_nxp)
   switch(outputNumber)
   {
      case 0:
         if(value)
         {
            Chip_GPIO_SetValue(LPC_GPIO_PORT, 2, 1<<4);
         }
         else
         {
            Chip_GPIO_ClearValue(LPC_GPIO_PORT, 2, 1<<4);
         }
         break;
      case 1:
         if(value)
         {
            Chip_GPIO_SetValue(LPC_GPIO_PORT, 2, 1<<5);
         }
         else
         {
            Chip_GPIO_ClearValue(LPC_GPIO_PORT, 2, 1<<5);
         }
         break;
      case 2:
         if(value)
         {
            Chip_GPIO_SetValue(LPC_GPIO_PORT, 2, 1<<6);
         }
         else
         {
            Chip_GPIO_ClearValue(LPC_GPIO_PORT, 2, 1<<6);
         }
         break;
      case 3:
         if(value)
         {
            Chip_GPIO_SetValue(LPC_GPIO_PORT, 5, 1<<1);
         }
         else
         {
            Chip_GPIO_ClearValue(LPC_GPIO_PORT, 5, 1<<1);
         }
         break;
      case 4:
         if(value)
         {
            Chip_GPIO_ClearValue(LPC_GPIO_PORT, 5, 1<<12);
         }
         else
         {
            Chip_GPIO_SetValue(LPC_GPIO_PORT, 5, 1<<12);
         }
         break;
      case 5:
         if(value)
         {
            Chip_GPIO_ClearValue(LPC_GPIO_PORT, 5, 1<<13);
         }
         else
         {
            Chip_GPIO_SetValue(LPC_GPIO_PORT, 5, 1<<13);
         }
         break;
      case 6:
         if(value)
         {
            Chip_GPIO_ClearValue(LPC_GPIO_PORT, 5, 1<<14);
         }
         else
         {
            Chip_GPIO_SetValue(LPC_GPIO_PORT, 5, 1<<14);
         }
         break;
      case 7:
         if(value)
         {
            Chip_GPIO_ClearValue(LPC_GPIO_PORT, 1, 1<<8);
         }
         else
         {
            Chip_GPIO_SetValue(LPC_GPIO_PORT, 1, 1<<8);
         }
         break;
      default:
         break;
   }
#elif(BOARD == edu_ciaa_nxp)
   switch(outputNumber)
   {
      case 0:
         if(value)
         {
            Chip_GPIO_SetValue(LPC_GPIO_PORT, 5, 1<<0);
         }
         else
         {
            Chip_GPIO_ClearValue(LPC_GPIO_PORT, 5, 1<<0);
         }
         break;
      case 1:
         if(value)
         {
            Chip_GPIO_SetValue(LPC_GPIO_PORT, 5, 1<<1);
         }
         else
         {
            Chip_GPIO_ClearValue(LPC_GPIO_PORT, 5, 1<<1);
         }
         break;
      case 2:
         if(value)
         {
            Chip_GPIO_SetValue(LPC_GPIO_PORT, 5, 1<<2);
         }
         else
         {
            Chip_GPIO_ClearValue(LPC_GPIO_PORT, 5, 1<<2);
         }
         break;
      case 3:
         if(value)
         {
            Chip_GPIO_SetValue(LPC_GPIO_PORT, 0, 1<<14);
         }
         else
         {
            Chip_GPIO_ClearValue(LPC_GPIO_PORT, 0, 1<<14);
         }
         break;
      case 4:
         if(value)
         {
            Chip_GPIO_SetValue(LPC_GPIO_PORT, 1, 1<<11);
         }
         else
         {
            Chip_GPIO_ClearValue(LPC_GPIO_PORT, 1, 1<<11);
         }
         break;
      case 5:
         if(value)
         {
            Chip_GPIO_SetValue(LPC_GPIO_PORT, 1, 1<<12);
         }
         else
         {
            Chip_GPIO_ClearValue(LPC_GPIO_PORT, 1, 1<<12);
         }
         break;
   }
#endif
}

/*==================[external functions definition]==========================*/
extern ciaaDevices_deviceType * ciaaDriverDio_open(char const * path,
      ciaaDevices_deviceType * device, uint8_t const oflag)
{
   return device;
}

extern int32_t ciaaDriverDio_close(ciaaDevices_deviceType const * const device)
{
   return 0;
}

extern int32_t ciaaDriverDio_ioctl(ciaaDevices_deviceType const * const device, int32_t const request, void * param)
{
   return -1;
}

extern ssize_t ciaaDriverDio_read(ciaaDevices_deviceType const * const device, uint8_t * buffer, size_t size)
{
   ssize_t ret = -1;

   /* Can't store read result in buffer. At least 1 byte required. */
   if(size != 0)
   {
      if(device == ciaaDioDevices[0])
      {
#if(BOARD == ciaa_nxp)
         buffer[0] = ~((uint8_t) ((Chip_GPIO_ReadValue(LPC_GPIO_PORT,3) & (0x0F<<11))>>7)
                              | (Chip_GPIO_ReadValue(LPC_GPIO_PORT,2) & 0x0F));
#elif(BOARD == edu_ciaa_nxp)
         buffer[0]  = Chip_GPIO_GetPinState(LPC_GPIO_PORT, 0, 4) ? 0 : 1;
         buffer[0] |= Chip_GPIO_GetPinState(LPC_GPIO_PORT, 0, 8) ? 0 : 2;
         buffer[0] |= Chip_GPIO_GetPinState(LPC_GPIO_PORT, 0, 9) ? 0 : 4;
         buffer[0] |= Chip_GPIO_GetPinState(LPC_GPIO_PORT, 1, 9) ? 0 : 8;
#endif

         /* 1 byte read */
         ret = 1;
      }
      else if(device == ciaaDioDevices[1])
      {
         /* read actual output state from layer data */
         buffer[0] = (uint8_t)*((ciaaDriverDio_dioType *)device->layer);

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

extern ssize_t ciaaDriverDio_write(ciaaDevices_deviceType const * const device, uint8_t const * const buffer, size_t const size)
{
   ssize_t ret = -1;

   if(size != 0)
   {
      if(device == ciaaDioDevices[0])
      {
         /* Inputs can't be written. */
         ret = -1;
      }
      else if(device == ciaaDioDevices[1])
      {
         int32_t i;

         for(i = 0; i < 8; i++)
         {
            ciaa_lpc4337_writeOutput(i, buffer[0] & (1 << i));
         }

         /* save actual output state in layer data */
         *((ciaaDriverDio_dioType *)device->layer) = buffer[0];

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

void ciaaDriverDio_init(void)
{
   uint8_t loopi;

   ciaa_lpc4337_gpio_init();

   /* add dio driver to the list of devices */
   for(loopi = 0; loopi < ciaaDriverDioConst.countOfDevices; loopi++) {
      /* add each device */
      ciaaDioDevices_addDriver(ciaaDriverDioConst.devices[loopi]);
      /* init layer data for each device */
      *((ciaaDriverDio_dioType *)ciaaDriverDioConst.devices[loopi]->layer) = 0;
   }
}


/*==================[interrupt hanlders]=====================================*/

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/

