#ifndef __HITEX_BOARD_INIT_H
#define __HITEX_BOARD_INIT_H

/* these functions need to be defined for the board specific module */
/* are placed in a special section called INIT_CODE to allow executing */
/* the functions from RAM by using scatter loading */
void hitex_externalMemoryInit(void) __attribute__ ((section("INIT_CODE")));
void hitex_externalSpifiInit(void) __attribute__ ((section("INIT_CODE")));
void hitex_clockInit(void) __attribute__ ((section("INIT_CODE")));

void hitex_ioInit(void);

#define externalMemoryInit()  	hitex_externalMemoryInit()
#define externalSpifiInit()  	hitex_externalSpifiInit()
#define clockInit()				hitex_clockInit()
#define ioInit()				hitex_ioInit()


#endif

