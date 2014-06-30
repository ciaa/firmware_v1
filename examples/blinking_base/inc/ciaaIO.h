/*
 * ciaaIO.h
 *
 *  Created on: Jun 12, 2014
 *      Author: pablo
 */

#ifndef CIAAIO_H_
#define CIAAIO_H_

#include "LPC43xx.h"
#include "lpc43xx_scu.h"
#include "lpc43xx_gpio.h"

#define ciaaDigitalInputs() ((uint8_t)((GPIO_ReadValue(3) & (0x0F<<11))>>7)|(GPIO_ReadValue(2) & 0x0F))

void ciaaIOInit(void);
uint32_t ciaaWriteOutput(uint32_t outputNumber, uint32_t value);

#endif /* CIAAIO_H_ */
