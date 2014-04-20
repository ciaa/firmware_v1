/*
 * ciaaMemory.h
 *
 *  Created on: 19/04/2014
 *      Author: J.Ezequiel
 */

#ifndef CIAAMEMORY_H_
#define CIAAMEMORY_H_

	#include "stdint.h"

	extern void* (*ciaaMemory_pfMalloc) (uint32_t size);
	extern void (*ciaaMemory_pfFree) (void* pointer);

	void ciaaMemory_SetMalloc (void* (*pf) (uint32_t));
	void ciaaMemory_SetFree (void (*pf) (void*));

#endif /* CIAAMEMORY_H_ */
