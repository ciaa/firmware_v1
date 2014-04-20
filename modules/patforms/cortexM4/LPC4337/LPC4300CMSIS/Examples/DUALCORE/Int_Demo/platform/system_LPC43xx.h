
#ifndef __SYSTEM_LPC43xx_H
#define __SYSTEM_LPC43xx_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

extern uint32_t SystemCoreClock;     /*!< System Clock Frequency (Core Clock)  */


/** Initialize the system.
 *
 *  This function is defined by the CMSIS standard.
 *  It will be called before main(), and before scatter loading and RAM initialization
 *  has taken place.
 *  Global variables have not been initialized yet!
 */
void SystemInit (void)
#if defined (__GNUC__)
__attribute__((weak))
#endif
    ;


/** Make sure the global variable \ref SystemCoreClock is up to date.
 */    
void SystemCoreClockUpdate (void);


#ifdef __cplusplus
}
#endif

#endif /* __SYSTEM_LPC43xx_H */

