#ifndef __KEIL_BOARD_INIT_H
#define __KEIL_BOARD_INIT_H

/* these functions need to be defined for the board specific module */
/* are placed in a special section called INIT_CODE to allow executing */
/* the functions from RAM by using scatter loading */
void keil_externalMemoryInit(void) __attribute__ ((section("INIT_CODE")));
void keil_externalSpifiInit(void) __attribute__ ((section("INIT_CODE")));
void keil_clockInit(void) __attribute__ ((section("INIT_CODE")));

void keil_ioInit(void);

#define externalMemoryInit()  	keil_externalMemoryInit()
#define externalSpifiInit()  	keil_externalSpifiInit()
#define clockInit()				keil_clockInit()
#define ioInit()				keil_ioInit()


#endif

