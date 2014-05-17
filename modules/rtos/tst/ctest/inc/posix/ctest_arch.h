

#ifndef _CTEST_ARCH_H_
#define _CTEST_ARCH_H_
/** \brief FreeOSEK Os Conformance Test
 **
 ** \file FreeOSEK/Os/tst/ctest/inc/posix/ctest_arch.h
 **/

/** \addtogroup FreeOSEK
 ** @{ */
/** \addtogroup FreeOSEK_Os
 ** @{ */
/** \addtogroup FreeOSEK_Os_CT Conformance Test
 ** @{ */

/*
 * Initials     Name
 * ---------------------------
 * MaCe			 Mariano Cerdeiro
 */

/*
 * modification history (new versions first)
 * -----------------------------------------------------------
 * 20140503 v0.1.0 MaCe	initial version
 */

/*==================[inclusions]=============================================*/

/*==================[macros]=================================================*/
#define TriggerISR2_Arch()                                                 \
      do {                                                                 \
        	int mq_ret;                                                       \
         char msg = 8;                                                     \
         mq_ret = mq_send(MessageQueue, &msg, sizeof(msg), 0);             \
         if (mq_ret < 0) {                                                 \
            printf("Trigger ISR2 Failed: %d %s\n", errno, strerror(errno));\
            sleep(3);                                                      \
         }                                                                 \
      } while(0)

/*==================[typedef]================================================*/

/*==================[external data declaration]==============================*/

/*==================[external functions declaration]=========================*/

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/
#endif /* #ifndef _CTEST_ARCH_H_ */

