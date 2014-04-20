

#include "lpc43xx_cgu.h"
#include "lpc43xx_scu.h"
#include "lpc43xx_gpio.h"
#include "lpc43xx_libcfg.h"

#include "mcb4300_lpc4350.h"


void setupMCB4350(void) {

	SystemInit();
	CGU_Init();
	
	scu_pinmux (0x9, 0, MD_PUP, FUNC0);   
	LPC_GPIO_PORT->DIR[4] |= 1 << 12;
	clearLed();

	/* Pin configuration for SCT */
	/* configuration for Keil MCB4350 board flashless */
	/* note: pins use EMC interface, so run only from internal ram to test */
	/* for other configurations, choose different ports */
 	
	/* outputs */
	scu_pinmux (0x2, 8, MD_PUP, FUNC1);   /* P2_8:	SCTOUT_0 */
 	scu_pinmux (0x2, 7, MD_PUP, FUNC1);   /* P2_7:  SCTOUT_1 */
 	scu_pinmux (0x2,10, MD_PUP, FUNC1);   /* P2_10: SCTOUT_2 */
 	scu_pinmux (0x2, 9, MD_PUP, FUNC1);   /* P2_9:  SCTOUT_3 */
 	scu_pinmux (0x2,12, MD_PUP, FUNC1);   /* P2_12: SCTOUT_4 */
 	scu_pinmux (0x2,11, MD_PUP, FUNC1);   /* P2_11: SCTOUT_5 */
	scu_pinmux (0x1, 2, MD_PUP, FUNC1);   /* P1_2:  SCTOUT_6 */
	scu_pinmux (0x1, 1, MD_PUP, FUNC1);   /* P1_1:  SCTOUT_7 */
	scu_pinmux (0x1, 3, MD_PUP, FUNC1);   /* P1_3:  SCTOUT_8 */
	scu_pinmux (0x1, 4, MD_PUP, FUNC1);   /* P1_4:  SCTOUT_9 */
	scu_pinmux (0x1, 5, MD_PUP, FUNC1);   /* P1_5:  SCTOUT_10 */	
 	scu_pinmux (0x1, 9, MD_PUP, FUNC2);   /* P1_8:  SCTOUT_11 */
	scu_pinmux (0x1, 8, MD_PUP, FUNC2);   /* P1_8:  SCTOUT_12 */	
	scu_pinmux (0x1, 7, MD_PUP, FUNC2);   /* P1_7:  SCTOUT_13 */
 	scu_pinmux (0x1,10, MD_PLN, FUNC2);   /* P1_10: SCTOUT_14 */
 	scu_pinmux (0x1,11, MD_PLN, FUNC2);   /* P1_11: SCTOUT_15 */

	/* inputs */
 	scu_pinmux (0x2, 4, MD_PUP | INBUF_ENABLE, FUNC3);   /* P2_4:  SCTIN_0 */
 	scu_pinmux (0x2, 3, MD_PUP | INBUF_ENABLE, FUNC3);   /* P2_3:  SCTIN_1 */
 	scu_pinmux (0x2, 5, MD_PUP | INBUF_ENABLE, FUNC1);   /* P2_5:  SCTIN_2 */
	scu_pinmux (0x1, 0, MD_PUP | INBUF_ENABLE, FUNC1);   /* P4_3:  SCTIN_3 */
 	scu_pinmux (0x2,13, MD_PUP | INBUF_ENABLE, FUNC1);   /* P2_13: SCTIN_4 */
	scu_pinmux (0x1, 6, MD_PUP | INBUF_ENABLE, FUNC1);   /* P1_6:  SCTIN_5 */
 	scu_pinmux (0x2, 2, MD_PUP | INBUF_ENABLE, FUNC5);   /* P2_2:  SCTIN_6 */
 	scu_pinmux (0x2, 6, MD_PUP | INBUF_ENABLE, FUNC5);   /* P2_6:  SCTIN_7 */


}



