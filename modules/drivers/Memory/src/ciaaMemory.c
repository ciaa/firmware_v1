/*
 * ciaaMemory.c
 *
 *  Created on: 19/04/2014
 *      Author: J.Ezequiel
 */

#include "ciaaMemory.h"

	void* (*ciaaMemory_pfMalloc) (uint32_t size);
	void (*ciaaMemory_pfFree) (void* pointer);

	void ciaaMemory_SetMalloc (void* (*pf) (uint32_t))
	{
		ciaaMemory_pfMalloc = pf;
	}

	void ciaaMemory_SetFree (void (*pf) (void*))
	{
		ciaaMemory_pfFree = pf;
	}
