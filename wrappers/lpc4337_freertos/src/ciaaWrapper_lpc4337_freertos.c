#include "ciaaWrapper.h"
#include "ciaaWrapper_private.h"

void ciaaWrapper_Config ()
{
	ciaaMemory_SetMalloc (pvPortMalloc);
	ciaaMemory_SetFree (vPortFree);
}

void ciaaWrapper_Init ()
{
	ciaaPOSIX_init ();
}
