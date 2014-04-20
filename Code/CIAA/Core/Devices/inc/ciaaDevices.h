/*
 * ciaaDevices.h
 *
 *  Created on: 20/04/2014
 *      Author: J.Ezequiel
 */

#ifndef CIAADEVICES_H_
#define CIAADEVICES_H_

	enum ciaaDevices_Enum_Status
	{
		ciaaDevices_EStatus_Open = 0,
		ciaaDevices_EStatus_Close = 1
	};
	enum ciaaDevices_Enum_Status ciaaDevices_EStatus;

	extern const char* ciaaDevices_UART1 = "/dev/UART/UART1";
	extern const char* ciaaDevices_UART2 = "/dev/UART/UART2";
	extern const char* ciaaDevices_I2C1 = "/dev/I2C/I2C1";

#endif /* CIAADEVICES_H_ */
