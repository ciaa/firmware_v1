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

/** \brief CIAA Dio Driver for msp430g2231
 **
 ** Implements the Digital Input/Output (Dio) Driver for msp430g2231
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
#include "hw_memmap.h"

/*==================[macros and definitions]=================================*/

#define GPIO_PORT_TO_BASE	 ((t430IOstruct*)P1IN_)

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
 * This microcontroller hasn't any IO library provided by TI so the
 * imlementation is done directly using registers.
 */
#if (msp_ext430g2 == BOARD)
const ciaaDriverDio_dioType ciaaDriverDio_Inputs[] = { {1.3} };
const ciaaDriverDio_dioType ciaaDriverDio_Outputs[] =  { {1.0}, {1.6} };
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

static ciaaDevices_deviceType * const ciaaDioDevices[] = 
{
   &ciaaDriverDio_in0,
   &ciaaDriverDio_out0
};

static ciaaDriverConstType const ciaaDriverDioConst = 
{
   ciaaDioDevices,
   2
};

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/

/** \brief perform low level gpio initialization for msp430g2231
 */
static void ciaa_msp430g2231_gpio_init(void)
{
	t430IOstruct * baseAddress  ;
	unsigned char  flag;

#if (msp_ext430g2 == BOARD)
	//const ciaaDriverDio_dioType ciaaDriverDio_Inputs[] = { {1.3} };
	//const ciaaDriverDio_dioType ciaaDriverDio_Outputs[] =  { {1.0}, {1.6} };
	for(unsigned short i;i<ciaaDriverDio_InputCount;i++)
	{
		baseAddress = GPIO_PORT_TO_BASE[ciaaDriverDio_Inputs[i].port];
		flag = (1<<ciaaDriverDio_Inputs[i].pin);

		baseAddress->pDIR &= ~flag; //como entrada
	}

   	/* LEDs */
	for(unsigned short i;i<ciaaDriverDio_OutputCount;i++)
	{
		baseAddress = GPIO_PORT_TO_BASE[ciaaDriverDio_Outputs[i].port];
		flag = (1<<ciaaDriverDio_Outputs[i].pin);

		baseAddress->pOUT &= ~flag; //lo apago
		baseAddress->pDIR |= flag; //como salida
	}

/*
typedef struct
{
	volatile uint8_t	pIN;
	volatile uint8_t	pOUT;
	volatile uint8_t	pDIR;
	volatile uint8_t	pIFG;
	volatile uint8_t	pIES;
	volatile uint8_t	pIE;
	volatile uint8_t	pSEL;
	volatile uint8_t	pREN;
} t430IOstruct;
*/
#else
   #error please define BOARD variable!
#endif
}

/** \brief write managed output
 *  \param[in] outputNumber number of output to set (0 to ciaaDriverDio_OutputCount)
 *  \param[in] value new state for output (true or false)
 */
static void ciaa_msp430g2231_writeOutput(uint8_t outputNumber, uint8_t value)
{
	if (outputNumber < ciaaDriverDio_OutputCount)
	{ 
		t430IOstruct * baseAddress  ;
		unsigned char  flag;

		baseAddress = GPIO_PORT_TO_BASE[ciaaDriverDio_Outputs[outputNumber].port];
		flag = (1<<ciaaDriverDio_Outputs[outputNumber].pin);

		if( value!=0)
		{
			baseAddress->pOUT &= ~flag; 
		}
		else
		{
			baseAddress->pOUT |= flag; 
		}     
	}
}

/** \brief read managed input
 *  \param[in] inputNumber number of output to read (0 to ciaaDriverDio_InputCount)
 *  \return 1 if gpio is high, 0 if it's low, -1 if incorrect pin number
 */
static int8_t ciaa_msp430g2231_readInput(uint8_t inputNumber)
{
   int8_t rv = -1;

   if (inputNumber < ciaaDriverDio_InputCount)
   {
		t430IOstruct * baseAddress  ;
		unsigned char  flag;

		baseAddress = GPIO_PORT_TO_BASE[ciaaDriverDio_Inputs[inputNumber].port];
		flag = (1<<ciaaDriverDio_Inputs[inputNumber].pin);

      	rv = (baseAddress->pIN&flag) ? 1 : 0;
   }

   return rv;
}

/** \brief read managed output
 *  \param[in] outputNumber number of output to read (0 to ciaaDriverDio_OutputCount)
 *  \return 1 if gpio is high, 0 if it's low, -1 if incorrect pin number
 */
static int8_t ciaa_msp430g2231_readOutput(uint8_t outputNumber)
{
   int8_t rv = -1;

   if (outputNumber < ciaaDriverDio_OutputCount)
   {
		t430IOstruct * baseAddress  ;
		unsigned char  flag;

		baseAddress = GPIO_PORT_TO_BASE[ciaaDriverDio_Inputs[outputNumber].port];
		flag = (1<<ciaaDriverDio_Inputs[outputNumber].pin);

      	rv = (baseAddress->pOUT&flag) ? 1 : 0;     
   }

   return rv;
}

/** \brief pack bit states in byte buffer
 *  \param[in] number of pins to read (normally ciaaDriverDio_OutputCount or ciaaDriverDio_InputCount)
 *  \param[out] buffer user buffer
 *  \param[in] size user buffer size
 *  \param[in] readFunction function used to read pins (normally ciaa_msp430g2231_readOutput or ciaa_msp430g2231_readInput)
 *  \return number bytes required in buffer to store bits
 */
static int32_t ciaa_msp430g2231_readPins(int32_t pinCount, uint8_t * buffer, size_t size, int32_t (*readFunction)(uint32_t))
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
      buffer[j] |= readFunction(i) << (i - 8 * j);
      if((i > 0) && ((i & 0x07)==0))
      {
         j++;
      }
   }

   return count;
}

/*==================[external functions definition]==========================*/
extern ciaaDevices_deviceType * ciaaDriverDio_open(char const * path, ciaaDevices_deviceType * device, uint8_t const oflag)
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
      ret = ciaa_msp430g2231_readPins(ciaaDriverDio_InputCount, buffer, size, ciaa_msp430g2231_readInput);
   }
   else if(device == ciaaDioDevices[1])
   {
      /* accessing to outputs */
      ret = ciaa_msp430g2231_readPins(ciaaDriverDio_OutputCount, buffer, size, ciaa_msp430g2231_readOutput);
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
            ciaa_msp430g2231_writeOutput(i, buffer[j] & (1 << (i - 8 * j)));

            if( (i > 0) && ((i & 0x07) == 0) )
            {
               j++;
            }
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
   ciaa_msp430g2231_gpio_init();

   /* add dio driver to the list of devices */
   for(loopi = 0; loopi < ciaaDriverDioConst.countOfDevices; loopi++) 
{
      /* add each device */
      ciaaDioDevices_addDriver(ciaaDriverDioConst.devices[loopi]);
   }
}

/*==================[interrupt handlers]=====================================*/

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/
