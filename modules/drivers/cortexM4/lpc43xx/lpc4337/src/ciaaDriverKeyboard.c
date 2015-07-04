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
 *JJR			Joaquin Rodriguez
 *
 */


/*
 * modification history (new versions first)
 * -----------------------------------------------------------
 * 20150612 v0.0.1 initials initial version
 */

/*==================[inclusions]=============================================*/
#include "ciaaDriverKeyboard.h"
#include "ciaaDriverKeyboard_Internal.h"
#include "ciaaPOSIX_stdlib.h"
#include "ciaaPOSIX_string.h"
#include "chip.h"

/*==================[macros and definitions]=================================*/
#define KEYB_LINE_CTRL   11

#define SELECTED_LINE_0    0
#define SELECTED_LINE_1    1
#define SELECTED_LINE_2    2
#define SELECTED_LINE_3    3
#define TEC_FIL0           0
#define TEC_FIL1           1
#define TEC_FIL2           2
#define TEC_FIL3           3

#define ACTIVATE_LINE      0
#define DESACTIVATE_LINE   1

#define TEC_COL0_MASK      0x01
#define TEC_COL1_MASK      0x02
#define TEC_COL2_MASK      0x04
#define TEC_COL3_MASK      0x08
/** \brief Pointer to Devices */
typedef struct  {
   ciaaDevices_deviceType * const * const devices;
   uint8_t countOfDevices;
} ciaaDriverConstType;

/*==================[internal data declaration]==============================*/

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/
/** \brief Device for Keyboard 0 */
static ciaaDevices_deviceType ciaaDriverKeyboard_keyb0 = {
   "keyb0",                             /** <= driver name */
   ciaaDriverKeyboard_open,              /** <= open function */
   ciaaDriverKeyboard_close,             /** <= close function */
   ciaaDriverKeyboard_read,              /** <= read function */
   ciaaDriverKeyboard_write,             /** <= write function */
   ciaaDriverKeyboard_ioctl,             /** <= ioctl function */
   NULL,                                 /** <= seek function is not provided */
   NULL,                                 /** <= upper layer */
   (void*)&ciaaDriverKeyboard_keyb,     /** <= layer */
   NULL                                  /** <= NULL no lower layer */
};

static ciaaDevices_deviceType * const ciaaKeyboardDevices[] = {
   &ciaaDriverKeyboard_keyb0,
};

static ciaaDriverConstType const ciaaDriverKeyboardConst = {
   ciaaKeyboardDevices,
   1
};

/*==================[external data definition]===============================*/
/** \brief Keyboard 0 */
ciaaDriverKeyboard_keybType ciaaDriverKeyboard_keyb;


/*==================[internal functions definition]==========================*/

void ciaa_lpc4337_Keyboard_init(void)
{
   Chip_GPIO_Init(LPC_GPIO_PORT);

#if (BOARD == ciaa_nxp)
   /* GPIO */

#elif (BOARD == edu_ciaa_nxp)
   /* GPIO */

#if (CIAA_PNCH_USER_INTERFACE == CIAA_ENABLE)
   /* Keyboard Initialization */
     Chip_SCU_PinMux(1,5,MD_PUP|MD_EZI|MD_ZI,FUNC0);   /* GPIO1[8],  T_COL0 */
     Chip_SCU_PinMux(7,4,MD_PUP|MD_EZI|MD_ZI,FUNC0);   /* GPIO3[12], T_COL1 */
     Chip_SCU_PinMux(7,5,MD_PUP|MD_EZI|MD_ZI,FUNC0);   /* GPIO3[13], T_COL2 */
     Chip_SCU_PinMux(6,12,MD_PUP|MD_EZI|MD_ZI,FUNC0);  /* GPIO2[8],  T_COL3 */

     Chip_SCU_PinMux(4,0,MD_PUP,FUNC0);   /* GPIO2[0],  T_FIL0 */
     Chip_SCU_PinMux(4,1,MD_PUP,FUNC0);   /* GPIO2[1],  T_FIL1 */
     Chip_SCU_PinMux(4,2,MD_PUP,FUNC0);   /* GPIO2[2],  T_FIL2 */
     Chip_SCU_PinMux(4,3,MD_PUP,FUNC0);   /* GPIO2[3],  T_FIL3 */

     Chip_GPIO_SetDir(LPC_GPIO_PORT, 2,(1<<0)|(1<<1)|(1<<2)|(1<<3),1);

     Chip_GPIO_SetDir(LPC_GPIO_PORT, 1,(1<<8),0);
     Chip_GPIO_SetDir(LPC_GPIO_PORT, 3,(1<<12)|(1<<13),0);
     Chip_GPIO_SetDir(LPC_GPIO_PORT, 2,(1<<8),0);

     Chip_GPIO_ClearValue(LPC_GPIO_PORT, 2,(1<<0)|(1<<1)|(1<<2)|(1<<3));
#endif

#else
   #error please define BOARD variable!
#endif
}

void ciaa_lpc4337_writeKeyboard(uint32_t outputNumber, uint32_t value)
{
#if (BOARD == edu_ciaa_nxp)
#if (CIAA_PNCH_USER_INTERFACE == CIAA_ENABLE)
	switch(outputNumber)
	   {
	      case TEC_FIL0: /* FILA 0 */
	         if(value)
	         {
	            Chip_GPIO_SetValue(LPC_GPIO_PORT, 2, 1<<0);
	         }
	         else
	         {
	            Chip_GPIO_ClearValue(LPC_GPIO_PORT,2, 1<<0);
	         }
	         break;
	      case TEC_FIL1: /* FILA 1 */
	         if(value)
	         {
	            Chip_GPIO_SetValue(LPC_GPIO_PORT, 2, 1<<1);
	         }
	         else
	         {
	            Chip_GPIO_ClearValue(LPC_GPIO_PORT, 2, 1<<1);
	         }
	         break;
	      case TEC_FIL2: /* FILA 2 */
	         if(value)
	         {
	            Chip_GPIO_SetValue(LPC_GPIO_PORT, 2, 1<<2);
	         }
	         else
	         {
	            Chip_GPIO_ClearValue(LPC_GPIO_PORT, 2, 1<<2);
	         }
	         break;
	      case TEC_FIL3: /* FILA 3 */
	         if(value)
	         {
	            Chip_GPIO_SetValue(LPC_GPIO_PORT, 2, 1<<3);
	         }
	         else
	         {
	            Chip_GPIO_ClearValue(LPC_GPIO_PORT, 2, 1<<3);
	         }
	         break;
	     }
#endif
#endif
}

/*==================[external functions definition]==========================*/
extern ciaaDevices_deviceType * ciaaDriverKeyboard_open(char const * path,
      ciaaDevices_deviceType * device, uint8_t const oflag)
{
   return device;
}

extern int32_t ciaaDriverKeyboard_close(ciaaDevices_deviceType const * const device)
{
   return 0;
}

/** \brief keyboard ioctl function
 *
 * This function changes the active output of the keyboard
 * The keyboard works with negative logic, so a low level output
 * is taken as active.
 ** \param[in]  device   pointer to the device to be controlled
 ** \param[in]  request  action to be taken
 ** \param[in]  param    line to be activated
 */


extern int32_t ciaaDriverKeyboard_ioctl(ciaaDevices_deviceType const * const device, int32_t const request, void * param)
{
   uint8_t ret = -1;
   if(device == ciaaKeyboardDevices[0])
   {
	   switch(request){
	   /* CASE KEYBOARD LINE CONTROL
	    * This request changes the active output of the keyboard
	    * The keyboard works with negative logic, so a low level output
	    * is taken as active. It returns the active line:
	    * sLine == SELECTED_LINE_0  --> T_FIL0 = 0, T_FIL1 = 1, T_FIL2 = 1, T_FIL3 = 1
	    * sLine == SELECTED_LINE_1  --> T_FIL0 = 1, T_FIL1 = 0, T_FIL2 = 1, T_FIL3 = 1
	    * sLine == SELECTED_LINE_2  --> T_FIL0 = 1, T_FIL1 = 1, T_FIL2 = 0, T_FIL3 = 1
	    * sLine == SELECTED_LINE_3  --> T_FIL0 = 1, T_FIL1 = 1, T_FIL2 = 1, T_FIL3 = 0
	    */

	         case KEYB_LINE_CTRL:
	   	     switch(((int)param)){
	   	     case SELECTED_LINE_0:
	   	    	ciaa_lpc4337_writeKeyboard(TEC_FIL0,ACTIVATE_LINE);
	   	    	ciaa_lpc4337_writeKeyboard(TEC_FIL1,DESACTIVATE_LINE);
	   	    	ciaa_lpc4337_writeKeyboard(TEC_FIL2,DESACTIVATE_LINE);
	   	    	ciaa_lpc4337_writeKeyboard(TEC_FIL3,DESACTIVATE_LINE);
	   	    	break;
	   	     case SELECTED_LINE_1:
		   	    ciaa_lpc4337_writeKeyboard(TEC_FIL0,DESACTIVATE_LINE);
		   	    ciaa_lpc4337_writeKeyboard(TEC_FIL1,ACTIVATE_LINE);
		   	    ciaa_lpc4337_writeKeyboard(TEC_FIL2,DESACTIVATE_LINE);
		   	    ciaa_lpc4337_writeKeyboard(TEC_FIL3,DESACTIVATE_LINE);
		   	    break;
	   	     case SELECTED_LINE_2:
		   	   	ciaa_lpc4337_writeKeyboard(TEC_FIL0,DESACTIVATE_LINE);
		   	   	ciaa_lpc4337_writeKeyboard(TEC_FIL1,DESACTIVATE_LINE);
		   	   	ciaa_lpc4337_writeKeyboard(TEC_FIL2,ACTIVATE_LINE);
		   	   	ciaa_lpc4337_writeKeyboard(TEC_FIL3,DESACTIVATE_LINE);
		   	   	break;
	   	     case SELECTED_LINE_3:
		   	    ciaa_lpc4337_writeKeyboard(TEC_FIL0,DESACTIVATE_LINE);
		   	    ciaa_lpc4337_writeKeyboard(TEC_FIL1,DESACTIVATE_LINE);
		   	    ciaa_lpc4337_writeKeyboard(TEC_FIL2,DESACTIVATE_LINE);
		   	    ciaa_lpc4337_writeKeyboard(TEC_FIL3,ACTIVATE_LINE);
		   	    break;
	   	     }
	   	     break;

	   	  default:
	   		  break;
	      }
      ret = 1;
   }
   return ret;
}

/** \brief keyboard read function
 *
 * This function read the columns of the keyboard
 * The keyboard works with negative logic, so a low level output
 * is taken as active.
 ** \param[in]  device   pointer to the device to be controlled
 ** \param[in]  buffer   pointer to variable where the read information will be saved
 ** \param[in]  size     size of the buffer
 ** At the end, buffer saves the column number which has been detected as pressed. If
 ** no switch has been pressed, buffer has 0xFF
 */

extern uint16_t ciaaDriverKeyboard_read(ciaaDevices_deviceType const * const device, uint16_t * buffer, size_t size)
{
   ssize_t ret = -1;

   /* Can't store read result in buffer. At least 1 byte required. */
   if(size != 0)
   {
      if(device == ciaaKeyboardDevices[0])
      {
#if(BOARD == edu_ciaa_nxp)
#if (CIAA_PNCH_USER_INTERFACE == CIAA_ENABLE)
         buffer[0]  = Chip_GPIO_GetPinState(LPC_GPIO_PORT, 1, 8)  ? 1 : 0;
         buffer[0] |= Chip_GPIO_GetPinState(LPC_GPIO_PORT, 3, 12) ? 2 : 0;
         buffer[0] |= Chip_GPIO_GetPinState(LPC_GPIO_PORT, 3, 13) ? 4 : 0;
         buffer[0] |= Chip_GPIO_GetPinState(LPC_GPIO_PORT, 2, 8)  ? 8 : 0;
#endif
#endif
      if(!(TEC_COL0_MASK & buffer[0]))
      {
         buffer[0] = 0;
         return 1;
      }
      else
      {
      if(!(TEC_COL1_MASK & buffer[0]))
      {
              buffer[0] = 1;
              return 1;
      }
      else
      {
      if(!(TEC_COL2_MASK & buffer[0]))
      {
         buffer[0] = 2;
         return 1;
      }
      else
      {
      if(!(TEC_COL3_MASK & buffer[0]))
      {
         buffer[0] = 3;
         return 1;
      }
      }
      }
      }
         /* 1 byte read */
         ret = 1;
      }

      else {
    	  /* Put your code if you have another device */
      }
   }
   return ret;
}

extern uint16_t ciaaDriverKeyboard_write(ciaaDevices_deviceType const * const device, uint16_t const * const buffer, size_t const size)
{
	/* keyboard cannot be written */
   ssize_t ret = -1;

  return ret;
}

void ciaaDriverKeyboard_init(void)
{
   uint8_t loopi;

   ciaa_lpc4337_Keyboard_init();

   /* add dio driver to the list of devices */
   for(loopi = 0; loopi < ciaaDriverKeyboardConst.countOfDevices; loopi++) {
      /* add each device */
      ciaaKeyboardDevices_addDriver(ciaaDriverKeyboardConst.devices[loopi]);
      /* init layer data for each device */
      *((ciaaDriverKeyboard_keybType *)ciaaDriverKeyboardConst.devices[loopi]->layer) = 0;
   }
}


/*==================[interrupt hanlders]=====================================*/

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/

