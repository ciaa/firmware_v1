/* Copyright 2015, Juan Pablo Moreno (UNCa)
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

/** \brief CIAA PWM Driver for LPC4337
 **
 ** Implements the PWM (PWM) Driver for LPC4337
 **
 **/

/** \addtogroup CIAA_Firmware CIAA Firmware
 ** @{ */
/** \addtogroup Drivers CIAA Drivers
 ** @{ */
/** \addtogroup PWM PWM Drivers
 ** @{ */

/*
 * Initials     Name
 * ---------------------------
 * JPM       Juan Pablo Moreno
 */

/*
 * modification history (new versions first)
 * -----------------------------------------------------------
 * 20150923 v0.0.1   JPM first functional version
 */

/*==================[inclusions]=============================================*/
#include "ciaaDriverPwm.h"
#include "ciaaDriverPwm_Internal.h"
#include "chip.h"

/*==================[macros and definitions]=================================*/
/** \brief Pointer to Devices */
typedef struct  {
   ciaaDevices_deviceType * const * const devices;
   uint8_t countOfDevices;
} ciaaDriverConstType;

/*==================[internal data declaration]==============================*/
/** \brief Port resources definitions for PWM 0 */
static ciaaDriverPwm_portType const ciaaDriverPwm_port0;

/** \brief Port resources definitions for PWM 1 */
static ciaaDriverPwm_portType const ciaaDriverPwm_port1;

/** \brief Port resources definitions for PWM 2 */
static ciaaDriverPwm_portType const ciaaDriverPwm_port2;

/** \brief Port settings for PWM 0 */
ciaaDriverPwm_pwmType ciaaDriverPwm_pwm0;

/** \brief Port settings for PWM 1 */
ciaaDriverPwm_pwmType ciaaDriverPwm_pwm1;

/** \brief Port settings for PWM 2 */
ciaaDriverPwm_pwmType ciaaDriverPwm_pwm2;

/** \brief Device for PWM 0 */
static ciaaDevices_deviceType ciaaDriverPwm_device0;

/** \brief Device for PWM 1 */
static ciaaDevices_deviceType ciaaDriverPwm_device1;

/** \brief Device for PWM 2 */
static ciaaDevices_deviceType ciaaDriverPwm_device2;
/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/
#if (BOARD == ciaa_nxp)
   #define CIAA_DRIVER_PWM_DEVICE_COUNTER	1
   /** \brief Port resources definitions for PWM 0 */
   static ciaaDriverPwm_portType const ciaaDriverPwm_port0 = { 1, 5, FUNC1, 10 };
#elif (BOARD == edu_ciaa_nxp)
   #define CIAA_DRIVER_PWM_DEVICE_COUNTER	3
   /** \brief Port resources definitions for PWM 0 */
   static ciaaDriverPwm_portType const ciaaDriverPwm_port0 = { 2, 10, FUNC1, 2 };
   /** \brief Port resources definitions for PWM 1 */
   static ciaaDriverPwm_portType const ciaaDriverPwm_port1 = { 2, 11, FUNC1, 5 };
   /** \brief Port resources definitions for PWM 2 */
   static ciaaDriverPwm_portType const ciaaDriverPwm_port2 = { 2, 12, FUNC1, 4 };
#endif

/** \brief Device for PWM 0 */
static ciaaDevices_deviceType ciaaDriverPwm_device0 = {
   "pwm/0",                        /** <= driver name */
   ciaaDriverPwm_open,             /** <= open function */
   ciaaDriverPwm_close,            /** <= close function */
   ciaaDriverPwm_read,             /** <= read function */
   ciaaDriverPwm_write,            /** <= write function */
   ciaaDriverPwm_ioctl,            /** <= ioctl function */
   NULL,                           /** <= seek function is not provided */
   NULL,                           /** <= upper layer */
   (void *)&ciaaDriverPwm_pwm0,	   /** <= layer */
   (void *)&ciaaDriverPwm_port0    /** <= NULL no lower layer */
};
#if (CIAA_DRIVER_PWM_DEVICE_COUNTER >= 2)
/** \brief Device for PWM 1 */
static ciaaDevices_deviceType ciaaDriverPwm_device1 = {
   "pwm/1",                        /** <= driver name */
   ciaaDriverPwm_open,             /** <= open function */
   ciaaDriverPwm_close,            /** <= close function */
   ciaaDriverPwm_read,             /** <= read function */
   ciaaDriverPwm_write,            /** <= write function */
   ciaaDriverPwm_ioctl,            /** <= ioctl function */
   NULL,                           /** <= seek function is not provided */
   NULL,                           /** <= upper layer */
   (void *)&ciaaDriverPwm_pwm1,	   /** <= layer */
   (void *)&ciaaDriverPwm_port1    /** <= NULL no lower layer */
};
#endif
#if (CIAA_DRIVER_PWM_DEVICE_COUNTER >= 3)
/** \brief Device for PWM 2 */
static ciaaDevices_deviceType ciaaDriverPwm_device2 = {
   "pwm/2",                         /** <= driver name */
   ciaaDriverPwm_open,             /** <= open function */
   ciaaDriverPwm_close,            /** <= close function */
   ciaaDriverPwm_read,             /** <= read function */
   ciaaDriverPwm_write,            /** <= write function */
   ciaaDriverPwm_ioctl,            /** <= ioctl function */
   NULL,                           /** <= seek function is not provided */
   NULL,                           /** <= upper layer */
   (void *)&ciaaDriverPwm_pwm2,	   /** <= layer */
   (void *)&ciaaDriverPwm_port2    /** <= NULL no lower layer */
};
#endif


#if (CIAA_DRIVER_PWM_DEVICE_COUNTER == 3)
static ciaaDevices_deviceType * const ciaaPwmDevices[] = {
   &ciaaDriverPwm_device0,
   &ciaaDriverPwm_device1,
   &ciaaDriverPwm_device2
};
#elif (CIAA_DRIVER_PWM_DEVICE_COUNTER == 2)
static ciaaDevices_deviceType * const ciaaPwmDevices[] = {
   &ciaaDriverPwm_device0,
   &ciaaDriverPwm_device1
};
#else
static ciaaDevices_deviceType * const ciaaPwmDevices[] = {
   &ciaaDriverPwm_device0
};
#endif

static ciaaDriverConstType const ciaaDriverPwmConst = {
  ciaaPwmDevices,
  CIAA_DRIVER_PWM_DEVICE_COUNTER
};

/*==================[external data definition]===============================*/
void ciaaDriverPwm_configInit(ciaaDevices_deviceType * device, uint8_t index_counter)
{
   ciaaDriverPwm_pwmType * pwm = device->layer;

   pwm->config.frequence = 10000;
   pwm->config.index_counter = index_counter;
}

/*==================[internal functions definition]==========================*/

/*==================[external functions definition]==========================*/
extern ciaaDevices_deviceType * ciaaDriverPwm_open(char const * path,
      ciaaDevices_deviceType * device, uint8_t const oflag)
{
   ciaaDriverPwm_pwmType * pwm = device->layer;
   ciaaDriverPwm_portType * port = device->loLayer;

   if((device != &ciaaDriverPwm_device0) && (device != &ciaaDriverPwm_device1) &&
	  (device != &ciaaDriverPwm_device2))
   {
	  device = NULL;
   }
   else
   {
	   Chip_SCU_PinMux(port->port, port->pin, MD_PUP|MD_EZI, port->function);
	   Chip_SCTPWM_Init(LPC_SCT);
	   Chip_SCTPWM_SetRate(LPC_SCT, pwm->config.frequence);
	   Chip_SCTPWM_SetOutPin(LPC_SCT, pwm->config.index_counter, port->ctout);
	   Chip_SCTPWM_Start(LPC_SCT);
   }

   return device;
}

extern int32_t ciaaDriverPwm_close(ciaaDevices_deviceType const * const device)
{
   Chip_SCTPWM_Stop(LPC_SCT);

   return 0;
}

extern int32_t ciaaDriverPwm_ioctl(ciaaDevices_deviceType const * const device, int32_t const request, void * param)
{
   return -1;
}

extern ssize_t ciaaDriverPwm_read(ciaaDevices_deviceType const * const device, uint8_t * buffer, size_t size)
{
   ciaaDriverPwm_pwmType * pwm = device->layer;
   uint32_t ticks, ticks_per_cycle;
   ssize_t ret = -1;

   if((device != &ciaaDriverPwm_device0) && (device != &ciaaDriverPwm_device1) &&
	  (device != &ciaaDriverPwm_device2))
   {
	   ret = -1;
   }
   else
   {
	   ticks = Chip_SCTPWM_GetDutyCycle(LPC_SCT, pwm->config.index_counter);
	   ticks_per_cycle = Chip_SCTPWM_GetTicksPerCycle(LPC_SCT);
	   buffer[0] = (uint8_t)((ticks * 100) / ticks_per_cycle);
	   ret = 0;
   }
   return ret;
}

extern ssize_t ciaaDriverPwm_write(ciaaDevices_deviceType const * const device, uint8_t const * const buffer, size_t const size)
{
   ciaaDriverPwm_pwmType * pwm = device->layer;
   ssize_t ret = -1;

   if((device != &ciaaDriverPwm_device0) && (device != &ciaaDriverPwm_device1) &&
	  (device != &ciaaDriverPwm_device2))
   {
	   ret = -1;
   }
   else if((buffer[0] >= 0) && (buffer[0] <= 100))
   {
	   Chip_SCTPWM_SetDutyCycle(LPC_SCT, pwm->config.index_counter,
			   Chip_SCTPWM_PercentageToTicks(LPC_SCT, buffer[0]));
	   ret = 0;
   }
   return ret;
}

void ciaaDriverPwm_init(void)
{
   uint8_t loopi;

   /* add pwm driver to the list of devices */
   for(loopi = 0; loopi < ciaaDriverPwmConst.countOfDevices; loopi++) {
      /* add each device */
	  ciaaDriverPwm_configInit(ciaaDriverPwmConst.devices[loopi], (loopi + 1));
      ciaaDioDevices_addDriver(ciaaDriverPwmConst.devices[loopi]);
   }
}

/*==================[interrupt handlers]=====================================*/

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/
