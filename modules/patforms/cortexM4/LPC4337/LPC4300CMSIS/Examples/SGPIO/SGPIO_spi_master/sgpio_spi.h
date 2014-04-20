#ifndef __SGPIO_SPI_H__
#define __SGPIO_SPI_H__

#include <stdint.h>
#include <stdbool.h>


#include "sgpio.h"

/* clock polarity can be either high or low when idle */
typedef enum clockPolarity_t { IDLE_LOW = 0, IDLE_HIGH = 1 } clockPolarity_t;

/* clock phase */
/* CPHA = 0 means the data is sampled on the first edge, shifted on the second edge */
/* CPHA = 1 means the data is shifted on the first edge, sampled on the second edge */
typedef enum clockPhase_t { CPHA_0 = 0, CPHA_1 = 1 } clockPhase_t;

/* supported transmission sizes for the data */
typedef enum wordSize_t { 	
								DBIT_4 = 4, 
								DBIT_8 = 8,
								DBIT_10 = 10,
								DBIT_12 = 12,
								DBIT_14 = 14,
								DBIT_16 = 16,
								DBIT_18 = 18,
								DBIT_20 = 20,
								DBIT_22 = 22,
								DBIT_24 = 24,
								DBIT_26 = 26,
								DBIT_28 = 28,
								DBIT_30 = 30,
								DBIT_32 = 32,
} wordSize_t;

/* peripheral mode */
typedef enum mode_t { MASTER, SLAVE } mode_t;

/* transmission bit rate in Hz (SPI clock speed) */
typedef uint32_t bitRate_t;


typedef enum CsPatternIdx { 	
	CS_DBIT_4 = 0, 
	CS_DBIT_8,
	CS_DBIT_10,
	CS_DBIT_12,
	CS_DBIT_14,
	CS_DBIT_16,
	CS_DBIT_18,
	CS_DBIT_20,
	CS_DBIT_22,
	CS_DBIT_24,
	CS_DBIT_26,
	CS_DBIT_28,
	CS_DBIT_30,
	CS_DBIT_32,
	NUM_CS_IDX,

} CsPatternEntry;

#define CHIP_SELECT_PATTERN(n) CS_ ## n

typedef uint32_t CsPattern;


typedef struct SpiParam {

	mode_t			spiMode;			// select if master or slave
	wordSize_t		wordLenght;			// select the number of bits transmitted per word
	clockPhase_t 	clockPhase;			// SPI phase 0 or 1 transfer format
	clockPolarity_t	clockPolarity;		// clock idle high or low
	bitRate_t		bitRateHz;			// speed of the spi interface
	CsPattern		chipSelect;

} SpiParam;



/* number of supported interfaces */
typedef enum { SPI_MASTER_0, SPI_MASTER_1 } spiNum_t;
	

/* spi emulation API calls */

/* initialize the peripheral */
void SGPIO_spiInit(void);

/* open a simulated SPI interface */
void SGPIO_spiOpen(spiNum_t spiId, SpiParam const *config);

/* close a simulated SPI interface */
void SGPIO_spiClose(spiNum_t spiId);

/* write one data item to the spi interface */
void SGPIO_spiWrite(spiNum_t spiId, uint32_t data, uint32_t* dataRead);

/* check if a new data item can be written */
bool isSpiReady(spiNum_t spiId);

/* read from the spi interface */
uint32_t SGPIO_spiRead(spiNum_t spiId);



/* used internally to check configuration errors within the code */
spiEmuStatus SGPIO_spiEmuCheckErrors (void) __attribute__((always_inline));

/* modify sgpio register contents outside the static configuration */
/* this shall be replaced by internal functions for all modules */
spiEmuStatus SGPIO_spiIoctl(const SGPIO_Slice sliceId, const sliceIoctl cmd, const void* param) __attribute__((always_inline));



#endif

