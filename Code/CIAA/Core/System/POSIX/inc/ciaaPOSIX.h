/*
 * ciaaPOSIX.h
 *
 *  Created on: 19/04/2014
 *      Author: J.Ezequiel
 */

#ifndef CIAAPOSIX_H_
#define CIAAPOSIX_H_

	#include "ciaaDevices.h"
	#include "ciaaMemory.h"

	struct ciaaPOSIX_Type_Base
	{
		int32_t fd;
		ciaaDevices_EStatus status;
		int32_t (*pOpen) (const char* pathName, int32_t flags);
		int32_t (*pClose) (int32_t fd);
		int32_t (*pIoctl) (int32_t fd, int32_t arg, uint32_t size);
		int32_t (*pRead) (int32_t fd, uint8_t* buffer, uint32_t size);
		int32_t (*pWrite) (int32_t fd, uint8_t* buffer, uint32_t size);
		void* data;
	};
	typedef struct ciaaPOSIX_Type_Base ciaaPOSIX_TBase;

	// TODO implement a generic list container
	// TODO move list definition and methods to System/Resources/ciaaGenericList.h
	struct ciaaPOSIX_Type_DeviceNode
	{
		struct ciaaPOSIX_Type_DeviceNode* pNext;
		ciaaPOSIX_TBase* pData;
	};
	typedef struct ciaaPOSIX_TDeviceNode;

	struct ciaaPOSIX_Type_DeviceList
	{
		ciaaPOSIX_Type_DeviceNode* pFirs;
		uint32_t size;
	};
	typedef struct ciaaPOSIX_Type_DeviceList ciaaPOSIX_TDeviceList;

	extern ciaaPOSIX_TDeviceList ciaaPOSIX_devices;

	int32_t ciaaPOSIX_open (const char* pathName, int32_t flags);
	int32_t ciaaPOSIX_close (int32_t fd);
	int32_t ciaaPOSIX_ioctl (int32_t fd, int32_t arg, void* param);
	int32_t ciaaPOSIX_read (int32_t fd, uint8_t* buffer, uint32_t size);
	int32_t ciaaPOSIX_write (int32_t fd, uint8_t* buffer, uint32_t size);

#endif /* CIAAPOSIX_H_ */
