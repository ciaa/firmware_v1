/* Copyright 2014, 2015 Pablo Ridolfi (UTN-FRBA)
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

/*==================[inclusions]=============================================*/
#include "ciaaDriverDio.h"
#include "ciaaDriverDio_Internal.h"
#include "ciaaPOSIX_stdlib.h"
#include "ciaaPOSIX_string.h"
#include "chip.h"

/*==================[macros and definitions]=================================*/

/** \brief Managed input count */
#define ciaaDriverDio_InputCount (sizeof(ciaaDriverDio_Inputs) / sizeof(ciaaDriverDio_dioType))

/** \brief Managed output count */
#define ciaaDriverDio_OutputCount (sizeof(ciaaDriverDio_Outputs) / sizeof(ciaaDriverDio_dioType))

/** \brief Pointer to Devices */
typedef struct  {
   ciaaDevices_deviceType * const * const devices;
   uint8_t countOfDevices;
} ciaaDriverConstType;

/*==================[internal data declaration]==============================*/

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/

/** \brief PORT/PIN map for managed GPIOs
 *
 * Define port/pin pairs for each GPIO managed by this driver
 */
#if (ciaa_nxp == BOARD)
const ciaaDriverDio_dioType ciaaDriverDio_Inputs[] = { {2,0},{2,1},{2,2},{2,3},{3,11},{3,12},{3,13},{3,14} };
const ciaaDriverDio_dioType ciaaDriverDio_Outputs[] =  { {5,1},{2,6},{2,5},{2,4},{5,12},{5,13},{5,14},{1,8} };
#elif (edu_ciaa_nxp == BOARD)
const ciaaDriverDio_dioType ciaaDriverDio_Inputs[] = { {0,4},{0,8},{0,9},{1,9} };
const ciaaDriverDio_dioType ciaaDriverDio_Outputs[] =  { {5,0},{5,1},{5,2},{0,14},{1,11},{1,12}, {3,0},{3,3},{3,4} };
#endif

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
   NULL,                           /** <= layer */
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
   NULL,                           /** <= layer */
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

/*==================[internal functions definition]==========================*/

/** \brief perform low level gpio initialization for LPC4337
 */
static void ciaa_lpc4337_gpio_init(void)
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
   Chip_SCU_PinMux(4,8,MD_PUP|MD_EZI,FUNC4);  /* GPIO5[12] */
   Chip_SCU_PinMux(4,9,MD_PUP|MD_EZI,FUNC4);  /* GPIO5[13] */
   Chip_SCU_PinMux(4,10,MD_PUP|MD_EZI,FUNC4); /* GPIO5[14] */
   Chip_SCU_PinMux(1,5,MD_PUP|MD_EZI,FUNC0);  /* GPIO1[8]  */
   Chip_GPIO_SetDir(LPC_GPIO_PORT, 5,(1<<12)|(1<<13)|(1<<14),1);
   Chip_GPIO_SetDir(LPC_GPIO_PORT, 1,(1<<8),1);
   Chip_GPIO_ClearValue(LPC_GPIO_PORT, 5,(1<<12)|(1<<13)|(1<<14));
   Chip_GPIO_ClearValue(LPC_GPIO_PORT, 1,(1<<8));

   /* Relays */
   Chip_SCU_PinMux(4,4,MD_PUP|MD_EZI,FUNC0); /* GPIO2[4] */
   Chip_SCU_PinMux(4,5,MD_PUP|MD_EZI,FUNC0); /* GPIO2[5] */
   Chip_SCU_PinMux(4,6,MD_PUP|MD_EZI,FUNC0); /* GPIO2[6] */
   Chip_SCU_PinMux(2,1,MD_PUP|MD_EZI,FUNC4); /* GPIO5[1] */
   Chip_GPIO_SetDir(LPC_GPIO_PORT, 2,(1<<4)|(1<<5)|(1<<6),1);
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
   Chip_SCU_PinMux(2,0,MD_PUP|MD_EZI,FUNC4);  /* GPIO5[0], LED0R */
   Chip_SCU_PinMux(2,1,MD_PUP|MD_EZI,FUNC4);  /* GPIO5[1], LED0G */
   Chip_SCU_PinMux(2,2,MD_PUP|MD_EZI,FUNC4);  /* GPIO5[2], LED0B */
   Chip_SCU_PinMux(2,10,MD_PUP|MD_EZI,FUNC0); /* GPIO0[14], LED1 */
   Chip_SCU_PinMux(2,11,MD_PUP|MD_EZI,FUNC0); /* GPIO1[11], LED2 */
   Chip_SCU_PinMux(2,12,MD_PUP|MD_EZI,FUNC0); /* GPIO1[12], LED3 */

   Chip_GPIO_SetDir(LPC_GPIO_PORT, 5,(1<<0)|(1<<1)|(1<<2),1);
   Chip_GPIO_SetDir(LPC_GPIO_PORT, 0,(1<<14),1);
   Chip_GPIO_SetDir(LPC_GPIO_PORT, 1,(1<<11)|(1<<12),1);

   Chip_GPIO_ClearValue(LPC_GPIO_PORT, 5,(1<<0)|(1<<1)|(1<<2));
   Chip_GPIO_ClearValue(LPC_GPIO_PORT, 0,(1<<14));
   Chip_GPIO_ClearValue(LPC_GPIO_PORT, 1,(1<<11)|(1<<12));

   /* EDU-CIAA-NXP GPIOs as outputs */
   Chip_SCU_PinMux(6,1,MD_PUP|MD_EZI,FUNC0);  /* GPIO3[0], GPIO0 */
   Chip_SCU_PinMux(6,4,MD_PUP|MD_EZI,FUNC0);  /* GPIO3[3], GPIO1 */
   Chip_SCU_PinMux(6,5,MD_PUP|MD_EZI,FUNC0);  /* GPIO3[4], GPIO2 */

   Chip_GPIO_SetDir(LPC_GPIO_PORT, 3,(1<<0)|(1<<3)|(1<<4),1);
   Chip_GPIO_ClearValue(LPC_GPIO_PORT, 3,(1<<0)|(1<<3)|(1<<4));
#else
   #error please define BOARD variable!
#endif
}

/** \brief write managed output
 *  \param[in] outputNumber number of output to set (0 to ciaaDriverDio_OutputCount)
 *  \param[in] value new state for output (true or false)
 */
static void ciaa_lpc4337_writeOutput(uint32_t outputNumber, uint32_t value)
{
   if (outputNumber < ciaaDriverDio_OutputCount)
   {
      Chip_GPIO_SetPinState(LPC_GPIO_PORT,
         ciaaDriverDio_Outputs[outputNumber].port,
         ciaaDriverDio_Outputs[outputNumber].pin,
         value != 0 ? 1 : 0);
   }
}

/** \brief read managed input
 *  \param[in] inputNumber number of output to read (0 to ciaaDriverDio_InputCount)
 *  \return 1 if gpio is high, 0 if it's low, -1 if incorrect pin number
 */
static int32_t ciaa_lpc4337_readInput(uint32_t inputNumber)
{
   int32_t rv = -1;

   if (inputNumber < ciaaDriverDio_InputCount)
   {
      rv = Chip_GPIO_GetPinState(LPC_GPIO_PORT,
         ciaaDriverDio_Inputs[inputNumber].port,
         ciaaDriverDio_Inputs[inputNumber].pin) ? 1 : 0;
   }

   return rv;
}

/** \brief read managed output
 *  \param[in] outputNumber number of output to read (0 to ciaaDriverDio_OutputCount)
 *  \return 1 if gpio is high, 0 if it's low, -1 if incorrect pin number
 */
static int32_t ciaa_lpc4337_readOutput(uint32_t outputNumber)
{
   int32_t rv = -1;

   if (outputNumber < ciaaDriverDio_OutputCount)
   {
      rv = Chip_GPIO_GetPinState(LPC_GPIO_PORT,
         ciaaDriverDio_Outputs[outputNumber].port,
         ciaaDriverDio_Outputs[outputNumber].pin) ? 1 : 0;
   }

   return rv;
}

/** \brief pack bit states in byte buffer
 *  \param[in] number of pins to read (normally ciaaDriverDio_OutputCount or ciaaDriverDio_InputCount)
 *  \param[out] buffer user buffer
 *  \param[in] size user buffer size
 *  \param[in] readFunction function used to read pins (normally ciaa_lpc4337_readOutput or ciaa_lpc4337_readInput)
 *  \return number bytes required in buffer to store bits
 */
static int32_t ciaa_lpc4337_readPins(int32_t pinCount, uint8_t * buffer, size_t size, int32_t (*readFunction)(uint32_t))
{
   int32_t count, i, j;
   /* amount of bytes necessary to store all input states */
   count = pinCount >> 3; /* ciaaDriverDio_InputCount / 8 */
   if( (pinCount & 0x07) != 0) /* (ciaaDriverDio_InputCount % 8) != 0 */
   {
      count += 1;
   }
   /* adjust gpios to read according to provided buffer length */
   if(count > size)
   {
      count = size;
   }
   /* read and store all inputs in user buffer */
   ciaaPOSIX_memset(buffer, 0, count);
   for(i = 0, j = 0; (i < pinCount) && (j < count); i++)
   {
      if((i > 0) && ((i & 0x07)==0))
      {
         j++;
      }
      buffer[j] |= readFunction(i) << (i - 8 * j);
   }
   return count;
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

   if(device == ciaaDioDevices[0])
   {
      /* accessing to inputs */
      ret = ciaa_lpc4337_readPins(ciaaDriverDio_InputCount, buffer, size, ciaa_lpc4337_readInput);
   }
   else if(device == ciaaDioDevices[1])
   {
      /* accessing to outputs */
      ret = ciaa_lpc4337_readPins(ciaaDriverDio_OutputCount, buffer, size, ciaa_lpc4337_readOutput);
   }
   else
   {
      /* Invalid device */
      ret = -1;
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
         /* Write outputs */
         int32_t i, j;

         /* set outputs according to bits defined in user buffer */
         for(i = 0, j = 0; (i < ciaaDriverDio_OutputCount) && (j < size); i++)
         {
            if( (i > 0) && ((i & 0x07) == 0) )
            {
               j++;
            }
            ciaa_lpc4337_writeOutput(i, buffer[j] & (1 << (i - 8 * j)));
         }
         if((ciaaDriverDio_OutputCount & 0x07) != 0)
         {
            j++;
         }
         ret = j;
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

   /* low level GPIO peripheral initialization */
   ciaa_lpc4337_gpio_init();

   /* add dio driver to the list of devices */
   for(loopi = 0; loopi < ciaaDriverDioConst.countOfDevices; loopi++) {
      /* add each device */
      ciaaDioDevices_addDriver(ciaaDriverDioConst.devices[loopi]);
   }
}

/*==================[interrupt handlers]=====================================*/

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/
