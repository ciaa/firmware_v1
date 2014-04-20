/*
 * ciaaPOSIX.c
 *
 *  Created on: 19/04/2014
 *      Author: J.Ezequiel
 */

#include "ciaaPOSIX.h"

ciaaPOSIX_TDeviceList ciaaPOSIX_devices;

int32_t ciaaPOSIX_open (const char* pathName, int32_t flags)
{
	// TODO Search the device's status flag in ciaaPOSIX_devices list

	// TODO Use GenericList to create new node and get the pointer

	if (pathName == "/dev/UART/UART1")
	{
		// TODO Add data to new node
		// ciaaPOSIX_devices[ciaaPOSIX_requestedDevices].fd = ciaaPOSIX_requestedDevices;
		// ciaaPOSIX_devices[ciaaPOSIX_requestedDevices].status = ciaaPOSIX_EStatus_Open;
		// ciaaPOSIX_devices[ciaaPOSIX_requestedDevices].pOpen = ciaaPOSIX_open;
		// ciaaPOSIX_devices[ciaaPOSIX_requestedDevices].pClose = ciaaUART_close;
		// ciaaPOSIX_devices[ciaaPOSIX_requestedDevices].pIoctl = ciaaUART_ioctl;
		// ciaaPOSIX_devices[ciaaPOSIX_requestedDevices].pRead = ciaaUART_read;
		// ciaaPOSIX_devices[ciaaPOSIX_requestedDevices].pWrite = ciaaUART_write;

		// ciaaPOSIX_devices[ciaaPOSIX_requestedDevices].pOpen (pathName, flags);
	}

	if (pathName == "/dev/UART/UART2")
	{

	}

	if (pathName == "/dev/I2C/I2C1")
	{

	}

	// TODO return ciaaPOSIX_devices size (file descriptor)
}

int32_t ciaaPOSIX_close (int32_t fd)
{
	// TODO search in ciaaPOSIX_devices by fd
	// TODO close the device
}

int32_t ciaaPOSIX_ioctl (int32_t fd, int32_t arg, void* param)
{

}

int32_t ciaaPOSIX_read (int32_t fd, uint8_t* buffer, uint32_t size)
{

}

int32_t ciaaPOSIX_write (int32_t fd, uint8_t* buffer, uint32_t size)
{

}

