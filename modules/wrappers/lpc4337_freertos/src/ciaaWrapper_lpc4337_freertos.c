#include "ciaaWrapper.h"
#include "ciaaWrapper_private.h"
#include "kciaa.h"

void ciaaWrapper_Config ()
{
   ciaaMemory_SetMalloc (pvPortMalloc);
   ciaaMemory_SetFree (vPortFree);
}

void ciaaWrapper_Init ()
{
   kciaa_init();
}
