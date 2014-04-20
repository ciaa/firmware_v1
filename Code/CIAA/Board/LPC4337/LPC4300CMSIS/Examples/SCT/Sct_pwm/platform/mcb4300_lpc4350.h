#ifndef __MCB4350_H__
#define __MCB4350_H__


void setupMCB4350(void);

#define clearLed() 	LPC_GPIO_PORT->CLR[4] = 1 << 12;
#define setLed() 	LPC_GPIO_PORT->SET[4] = 1 << 12;
#define toggleLed() LPC_GPIO_PORT->NOT[4] = 1 << 12;

#endif
