/**********************************************************************
* $Id$		Lcd_Demo.c				2011-06-02
*//**
* @file		Lcd_Demo.c
* @brief	This example describes how to use LCD on Hitex board
* 			and LogicPD 4.3
* @version	1.0
* @date		02. June. 2011
* @author	NXP MCU SW Application Team
*
* Copyright(C) 2011, NXP Semiconductor
* All rights reserved.
*
***********************************************************************
* Software that is described herein is for illustrative purposes only
* which provides customers with programming information regarding the
* products. This software is supplied "AS IS" without any warranties.
* NXP Semiconductors assumes no responsibility or liability for the
* use of the software, conveys no license or title under any patent,
* copyright, or mask work right to the product. NXP Semiconductors
* reserves the right to make changes in the software without
* notification. NXP Semiconductors also make no representation or
* warranty that such application will be suitable for the specified
* use without further testing or modification.
* Permission to use, copy, modify, and distribute this software and its
* documentation is hereby granted, under NXP Semiconductors'
* relevant copyright in the software, without fee, provided that it
* is used in conjunction with NXP Semiconductors microcontrollers.  This
* copyright, permission, and disclaimer notice must appear in all copies of
* this code.
**********************************************************************/
#include "lpc43xx_lcd.h"
#include "lpc43xx_gpio.h"
#include "lpc43xx_scu.h"
#include "lpc43xx_libcfg.h"
#include "lpc43xx_cgu.h"
#include "lpc43xx_adc.h"

/* Example group ----------------------------------------------------------- */
/** @defgroup Lcd_Demo	Lcd_Demo
 * @ingroup LCD_Examples
 * @{
 */

/************************** PRIVATE DEFINITIONS *************************/
#define LCD_WIDTH		800
#define LCD_HEIGHT		480

#define FRAMEBUFFER_ADDR        0x80000000

/************************** PRIVATE VARIABLES *************************/
uint32_t msec;

extern void SDRAM_Init (void);
extern const unsigned char image[];

/* LCD configured struct */
const LCD_CFG_Type ILux = {
	LCD_WIDTH,
	LCD_HEIGHT,
	{17,45},
	{22,22},
	2,
	2,
	1,
	LCD_SIGNAL_ACTIVE_HIGH,
	LCD_SIGNAL_ACTIVE_HIGH,
	LCD_CLK_RISING,
	LCD_SIGNAL_ACTIVE_HIGH,
	0,
	LCD_BPP8,
	LCD_TFT,
	LCD_COLOR_FORMAT_RGB,
	FALSE
};

/* pointer to frame buffer */
uint32_t *framebuffer = (uint32_t *)FRAMEBUFFER_ADDR;
/************************** PRIVATE FUNCTIONS *************************/

/*----------------- INTERRUPT SERVICE ROUTINES --------------------------*/
/*********************************************************************//**
 * @brief		SysTick handler sub-routine (1ms)
 * @param[in]	None
 * @return 		None
 **********************************************************************/
void SysTick_Handler (void) 					// SysTick Interrupt Handler @ 1000Hz
{
	int32_t SumADCPotValue;
	int32_t ADCVals[50], i;
	const int32_t ADCVals_Count = sizeof(ADCVals)/sizeof(ADCVals[0]);

	if(msec)msec--;

	// Read ADC and move display framebuffer pointer.
	// Implement filter (average of last 50 samples) to reduce display jitter.
	// This interrupt is running at a 1 khz rate.
	ADC_StartCmd(LPC_ADC0, ADC_START_NOW);
	while (!(ADC_ChannelGetStatus(LPC_ADC0,2,ADC_DATA_DONE)));

	// Make space in buffer for a new ADC sample
	for(i=0;i<(ADCVals_Count-1);i++)
		ADCVals[i] = ADCVals[i+1];
	// Add new ADC sample
    ADCVals[ADCVals_Count-1] = ADC_ChannelGetData(LPC_ADC0,2);
	// Sum all the samples in the ADC buffer
	SumADCPotValue = 0;
	for(i=0;i<ADCVals_Count;i++)
		SumADCPotValue += ADCVals[i];
	
	// Calculate and set framebuffer position
	LCD_SetFrameBuffer(LPC_LCD, (void*)(framebuffer + (((SumADCPotValue * (1310 - LCD_HEIGHT)) / (0x3FF*ADCVals_Count))*LCD_WIDTH/4)));
}
/*-------------------------PRIVATE FUNCTIONS------------------------------*/
/*********************************************************************//**
 * @brief		put a pixel
 * @param[in]	x,y postion of pixel
 * @param[in]	val color value of pixel in 32bit
 * @return 		None
 **********************************************************************/

void putpixel(uint32_t x, uint32_t y, uint32_t val){
	framebuffer[x+y*480] = val;
}

void Hitex_CGU_Init(void);

/*-------------------------MAIN FUNCTION------------------------------*/
/*********************************************************************//**
 * @brief		Main program body
 * @param[in]	None
 * @return 		int
 **********************************************************************/
uint32_t tempx, tempy;

#pragma anon_unions
typedef enum {
	false, true } bool;

typedef enum LPCLIB_Result {
    LPCLIB_PENDING = 1,                     /**< Operation pending (async mode) */
    LPCLIB_SUCCESS = 0,                     /**< Operation completed successfully */
    LPCLIB_BUSY = -1,                       /**< Resource busy */
    LPCLIB_ERROR = -2,                      /**< (unspecified) serious error */
    LPCLIB_ILLEGAL_PARAMETER = -3,          /**< Illegal or out-of-range parameter */
    LPCLIB_NOT_IMPLEMENTED = -4,            /**< Requested feature not implemented */
    LPCLIB_OUT_OF_MEMORY = -5,              /**< Memory exhausted */
    LPCLIB_NO_RESPONSE = -6,                /**< No response from slave (I2C bus) */
    LPCLIB_UNDEFINED = -7,                  /**< unspecified */
} LPCLIB_Result;

typedef enum I2C_JobOption {
    I2C_PHASE_SEND,
    I2C_PHASE_RECEIVE,
} I2C_JobOption;

typedef struct I2C_JobPhase {
    struct I2C_JobPhase *next;              /**< Pointer to next transaction phase (or NULL) */
    I2C_JobOption option;                   /**< ... */
    union {
        uint8_t *rxstart;                   /**< Start of RX buffer */
        const uint8_t *txstart;             /**< Start of TX buffer */
    };
    uint16_t length;                        /**< Transfer length (in bytes) */
} I2C_JobPhase;


/** Descriptor for an I2C transaction. */
typedef struct I2C_Job {
    const I2C_JobPhase *firstPhase;         /**< Points to the first in a list of
                                             *   phase descriptors.
                                             */
    uint8_t slaveAddress;                   /**< Slave address */
} I2C_Job;

struct I2C_Context {
    uint32_t rate;
    volatile I2C_Job *job;
    volatile const I2C_JobPhase *phase;
} i2cemu[1];
/** Handle for an I2C block */
typedef struct I2C_Context *I2C_Handle;

LPCLIB_Result I2CEMU_open (I2C_Handle *pHandle);

/** PCA9532 Register Names. */
enum PCA9532_RegisterNames {
    PCA9532_REG_INPUT0 = 0,
    PCA9532_REG_INPUT1 = 1,
    PCA9532_REG_PSC0 = 2,
    PCA9532_REG_PWM0 = 3,
    PCA9532_REG_PSC1 = 4,
    PCA9532_REG_PWM1 = 5,
    PCA9532_REG_LS0 = 6,
    PCA9532_REG_LS1 = 7,
    PCA9532_REG_LS2 = 8,
    PCA9532_REG_LS3 = 9,

    __PCA9532_REG_LAST__ = PCA9532_REG_LS3,
};

/** Pin names of port expander. */
typedef enum PCA9532_Pin {
    PCA9532_PIN0 = 0,
    PCA9532_PIN1 = 1,
    PCA9532_PIN2 = 2,
    PCA9532_PIN3 = 3,
    PCA9532_PIN4 = 4,
    PCA9532_PIN5 = 5,
    PCA9532_PIN6 = 6,
    PCA9532_PIN7 = 7,
    PCA9532_PIN8 = 8,
    PCA9532_PIN9 = 9,
    PCA9532_PIN10 = 10,
    PCA9532_PIN11 = 11,
    PCA9532_PIN12 = 12,
    PCA9532_PIN13 = 13,
    PCA9532_PIN14 = 14,
    PCA9532_PIN15 = 15,
} PCA9532_Pin;


/** Opcodes to specify the configuration command in a call to \ref PCA9532_ioctl. */
typedef enum PCA9532_Opcode {
    PCA9532_OPCODE_SET_PWM0 = PCA9532_REG_PSC0, /**< Config action: PWM0 setting */
    PCA9532_OPCODE_SET_PWM1 = PCA9532_REG_PSC1, /**< Config action: PWM1 setting */
    PCA9532_OPCODE_SET_PIN_FUNCTION,        /**< Config action: Select input/output/PWM0/PWM1 */
} PCA9532_Opcode;


/** Config option: PWM */
typedef enum PCA9532_PwmChannel {
    PCA9532_PWM0,                           /**< PWM channel 0 */
    PCA9532_PWM1,                           /**< PWM channel 1 */
} PCA9532_PwmChannel;
struct PCA9532_ConfigPwm {
    uint8_t period;                         /**< Period */
    uint8_t dutyCycle;                      /**< Duty cycle (128 = 50%) */
};

/** Config option: Pin function */
typedef enum PCA9532_PinFunction {
    PCA9532_PINFUNC_OFF,                    /**< OFF (high impedance) */
    PCA9532_PINFUNC_ON,                     /**< ON (driving low) */
    PCA9532_PINFUNC_PWM0,                   /**< Pin driven by PWM0 */
    PCA9532_PINFUNC_PWM1,                   /**< Pin driven by PWM1 */
} PCA9532_PinFunction;

struct PCA9532_ConfigPinFunc {
    uint8_t number;                         /**< Pin number */
    PCA9532_PinFunction func;               /**< Function selector */
};


/** Descriptor to specify the configuration in a call to \ref PCA9532_ioctl. */
typedef struct PCA9532_Config {
    PCA9532_Opcode opcode;                  /**< Config action opcode */
    FunctionalState continued;                /**< Set if further config struct's follow in an array */

    union {
        uint16_t direction;                 /**< Config pin direction (all pins!): ENABLE/DISABLE */
        struct PCA9532_ConfigPwm pwm;       /**< PWM configuration */
        struct PCA9532_ConfigPinFunc pinFunc;   /**< Pin function */
    };
} PCA9532_Config;


const PCA9532_Config portsLcd7Config[] = {
    {.opcode = PCA9532_OPCODE_SET_PIN_FUNCTION, .continued = ENABLE,
        {.pinFunc = {.number = PCA9532_PIN0,  .func = PCA9532_PINFUNC_OFF}}},
    {.opcode = PCA9532_OPCODE_SET_PIN_FUNCTION, .continued = ENABLE,
        {.pinFunc = {.number = PCA9532_PIN1,  .func = PCA9532_PINFUNC_OFF}}},
    {.opcode = PCA9532_OPCODE_SET_PIN_FUNCTION, .continued = ENABLE,
        {.pinFunc = {.number = PCA9532_PIN4,  .func = PCA9532_PINFUNC_OFF}}},
    {.opcode = PCA9532_OPCODE_SET_PIN_FUNCTION, .continued = ENABLE,
        {.pinFunc = {.number = PCA9532_PIN6,  .func = PCA9532_PINFUNC_OFF}}},
    {.opcode = PCA9532_OPCODE_SET_PIN_FUNCTION, .continued = DISABLE,
        {.pinFunc = {.number = PCA9532_PIN8,  .func = PCA9532_PINFUNC_OFF}}},
};

I2C_Handle i2cTft;
typedef struct PCA9532_Context *PCA9532_Handle;
PCA9532_Handle portsLcd7;

LPCLIB_Result PCA9532_open (I2C_Handle bus, const uint8_t address, PCA9532_Handle *pHandle);
#define PCA9532_LCD_ADDRESS                         0x64
LPCLIB_Result PCA9532_ioctl (PCA9532_Handle handle, const PCA9532_Config *pConfig);
void PCA9532_writeBit (PCA9532_Handle handle, PCA9532_Pin pin, uint8_t value);
const unsigned short master_palette[256];

int c_entry (void) {                       /* Main Program                       */
	uint32_t i;

	SystemInit();
	Hitex_CGU_Init();

	CGU_EntityConnect(CGU_CLKSRC_PLL1, CGU_BASE_LCD);

	// Set up I/O expander on EA board
	scu_pinmux(0x04, 8, MD_PUP|MD_EZI, FUNC4);		// Configure P4_8 as GPIO5[12] as bitbanged SDA
	scu_pinmux(0x07, 5, MD_PUP|MD_EZI, FUNC0);		// Configure P7_5 as GPIO3[13] as bitbanged SCL
	GPIO_SetDir(0x5, 1<<12, 0); // set as input
	GPIO_SetDir(0x3, 1<<13, 0); // set as input
	GPIO_ClearValue(0x5, 1<<12); // make sure driven low
	GPIO_ClearValue(0x3, 1<<13); // make sure driven low

	scu_pinmux(0x07, 3, MD_PUP, FUNC0); 	// P7_3 as GPIO3[11]- Drive reset pin for I2C expander high
	GPIO_SetDir(0x3, 1<<11, 1); // set as output
	GPIO_SetValue(0x3, 1<<11); // make sure driven high

	for(i=0;i<1000;i++); // delay

	// Use bit-banged I2C to turn on LCD power supplies
    I2CEMU_open(&i2cTft);

    PCA9532_open(i2cTft, PCA9532_LCD_ADDRESS, &portsLcd7);
    PCA9532_ioctl(portsLcd7, &portsLcd7Config[0]);

    PCA9532_writeBit(portsLcd7, PCA9532_PIN0, 1);
    PCA9532_writeBit(portsLcd7, PCA9532_PIN1, 1);
    PCA9532_writeBit(portsLcd7, PCA9532_PIN4, 0);
    PCA9532_writeBit(portsLcd7, PCA9532_PIN6, 0);
    PCA9532_writeBit(portsLcd7, PCA9532_PIN8, 0);

	for(i=0;i<10000;i++); // delay

	/*pin set up for LCD */
	scu_pinmux(0x07, 7, MD_PUP, FUNC3); 	/* LCD_PWR @ P7.7 */

	scu_pinmux(0x04, 7, MD_PUP, FUNC0);		/* LCD_DCLK @ P4.7 */
	scu_pinmux(0x04, 5, MD_PUP, FUNC2);		/* LCD_FP @ P4.5 */
	scu_pinmux(0x04, 6, MD_PUP, FUNC2); 	/* LCD_ENAB_M @ P4.6 */
	scu_pinmux(0x07, 6, MD_PUP, FUNC3);		/* LCD_LP @ P7.6 */
	scu_pinmux(0x04, 3, MD_PUP, FUNC2);		/* LCD_VD_2 @ P4.3 */
	scu_pinmux(0x04, 2, MD_PUP, FUNC2);		/* LCD_VD_3 @ P4.2 */
	scu_pinmux(0x08, 7, MD_PUP, FUNC3);		/* LCD_VD_4 @ P8.7 */
	scu_pinmux(0x08, 6, MD_PUP, FUNC3);		/* LCD_VD_5 @ P8.6 */
	scu_pinmux(0x08, 5, MD_PUP, FUNC3);		/* LCD_VD_6 @ P8.5 */
	scu_pinmux(0x08, 4, MD_PUP, FUNC3);		/* LCD_VD_7 @ P8.4 */
	scu_pinmux(0x04, 10, MD_PUP, FUNC2);	/* LCD_VD_10 @ P4.10 */
	scu_pinmux(0x04, 9, MD_PUP, FUNC2); 	/* LCD_VD_11 @ P4.9 */
	scu_pinmux(0x08, 3, MD_PUP, FUNC3); 	/* LCD_VD_12 @ P8.3 */
	scu_pinmux(0x0B, 6, MD_PUP, FUNC2); 	/* LCD_VD_13 @ PB.6 */
	scu_pinmux(0x0B, 5, MD_PUP, FUNC2); 	/* LCD_VD_14 @ PB.5 */
	scu_pinmux(0x0B, 4, MD_PUP, FUNC2); 	/* LCD_VD_15 @ PB.4 */
	scu_pinmux(0x07, 2, MD_PUP, FUNC3); 	/* LCD_VD_18 @ P7.2 */
	scu_pinmux(0x07, 1, MD_PUP, FUNC3); 	/* LCD_VD_19 @ P7.1 */
	scu_pinmux(0x0B, 3, MD_PUP, FUNC2); 	/* LCD_VD_20 @ PB.3 */
	scu_pinmux(0x0B, 2, MD_PUP, FUNC2); 	/* LCD_VD_21 @ PB.2 */
	scu_pinmux(0x0B, 1, MD_PUP, FUNC2); 	/* LCD_VD_22 @ PB.1 */
	scu_pinmux(0x0B, 0, MD_PUP, FUNC2); 	/* LCD_VD_23 @ PB.0 */

	scu_pinmux(0x07, 0, MD_PUP, FUNC3);		/* LCD_LE @ P7.0 */
	scu_pinmux(0x04, 4, MD_PUP, FUNC2);		/* LCD_VD_1 @ P4.4 */
	scu_pinmux(0x04, 1, MD_PUP, FUNC2);		/* LCD_VD_0 @ P4.1 */
	scu_pinmux(0x0D, 0, MD_PUP, FUNC4);		/* LCD_RST @ PD.0 */

	LPC_RGU->RESET_CTRL0 = (1UL << 16); // Reset LCD peripheral

	for(i=0;i<1000000;i++);
	LCD_Init(LPC_LCD, (LCD_CFG_Type*)&ILux);

	LCD_SetFrameBuffer(LPC_LCD, (void*)framebuffer);
	for(i=0;i<sizeof(master_palette)/sizeof(master_palette[0]);i+=2)
	{
		LPC_LCD->PAL[i/2] = master_palette[i];
		LPC_LCD->PAL[i/2] |= master_palette[i+1]<<16;
	}

	LCD_Power(LPC_LCD, ENABLE);

	ADC_Init(LPC_ADC0, 200000, 10);
	ADC_ChannelCmd(LPC_ADC0, 2, ENABLE);

	//Select function ADC1_2
	LPC_SCU->ENAIO1 |= 0x04;

	// M4Frequency is automatically set when SetClock(BASE_M4_CLK... was called.
	SysTick_Config(CGU_GetPCLKFrequency(CGU_PERIPHERAL_M4CORE)/1000);  				// Generate interrupt @ 1000 Hz

	while (1);
}


/* With ARM and GHS toolsets, the entry point is main() - this will
   allow the linker to generate wrapper code to setup stacks, allocate
   heap area, and initialize and copy code and data segments. For GNU
   toolsets, the entry point is through __start() in the crt0_gnu.asm
   file, and that startup code will setup stacks and data */
int main(void)
{
    return c_entry();
}

#ifdef  DEBUG
/*******************************************************************************
* @brief		Reports the name of the source file and the source line number
* 				where the CHECK_PARAM error has occurred.
* @param[in]	file Pointer to the source file name
* @param[in]    line assert_param error line source number
* @return		None
*******************************************************************************/
void check_failed(uint8_t *file, uint32_t line)
{
	/* User can add his own implementation to report the file name and line number,
	 ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

	/* Infinite loop */
	while(1);
}
#endif

/*******************************************************************************/
/***** Bit-banged I2C Library *****/
/*******************************************************************************/

typedef enum {
    I2CEMU_BIT0 = 1,
    I2CEMU_BIT1 = 0,
} I2CEMU_Level;

static void I2CEMU_SDA_WRITE(I2C_Handle handle, I2CEMU_Level val)
{
	GPIO_SetDir(5, 1<<12, (val == I2CEMU_BIT0) ? 1 : 0);
}

static uint32_t I2CEMU_SDA_READ(I2C_Handle handle)
{
    return ((GPIO_ReadValue(5)>>12)&1);
}

static void I2CEMU_SCL_WRITE(I2C_Handle handle, I2CEMU_Level val)
{
	GPIO_SetDir(3, 1<<13, (val == I2CEMU_BIT0) ? 1 : 0);
}

static uint32_t I2CEMU_SCL_READ(I2C_Handle handle)
{
    return ((GPIO_ReadValue(3)>>13)&1);
}


/** Constant that determines the I²C bit rate. (Higher value --> slower rate). */
#define I2CEMU_BIT_DELAY        (100)

/** Constant that determines an I²C timeout (Higher value --> longer timeout). */
#define I2CEMU_TIMEOUT          (20)



/** Software delay that determines the I2C bit rate.
 * @param multi Integer multiple of the delay quantum.
 */
static void I2CEMU_delay (uint8_t multi)
{
    volatile uint32_t i;


    while (multi)
    {
        /* A bit of delay... */
        for (i = 0; i < I2CEMU_BIT_DELAY; i++)
        {
        }

        multi--;
    }
}

/** Send a start condition. */
static void I2CEMU_start (I2C_Handle handle)
{
    I2CEMU_SCL_WRITE(handle, I2CEMU_BIT1);
    I2CEMU_delay(1);
    I2CEMU_SDA_WRITE(handle, I2CEMU_BIT0);
    I2CEMU_delay(1);
}


/** Send a start condition. */
static void I2CEMU_stop (I2C_Handle handle)
{
    I2CEMU_SDA_WRITE(handle, I2CEMU_BIT0);
    I2CEMU_delay(1);
    I2CEMU_SCL_WRITE(handle, I2CEMU_BIT1);
    I2CEMU_delay(1);
    I2CEMU_SDA_WRITE(handle, I2CEMU_BIT1);
    I2CEMU_delay(1);
}

/** Send one byte, and return the answer (ACK/NAK).
 *  @param data Byte to be sent
 *  @return true=ACK, false=NAK
 */
static bool I2CEMU_sendByte (I2C_Handle handle, uint8_t data)
{
    bool result = false;
    uint32_t i;
    uint32_t timeout;


    /* Send eight bits plus the clock to request the acknowledge. */
    for (i = 0; i < 9; i++)
    {
        I2CEMU_SCL_WRITE(handle, I2CEMU_BIT0);
        I2CEMU_delay(1);
        I2CEMU_SDA_WRITE(handle, (data & 0x80) ? I2CEMU_BIT1 : I2CEMU_BIT0);
        data = (data << 1) | 1; /* (Make sure the ninth bit is a 1!) */
        I2CEMU_delay(1);

        I2CEMU_SCL_WRITE(handle, I2CEMU_BIT1);
        I2CEMU_delay(2);
    }

    /* Read acknowledge bit */
    timeout = I2CEMU_TIMEOUT;
    do {
        /* Possible clock stretching ended? */
        if (I2CEMU_SCL_READ(handle) == 1) {
            /* ACK status is valid now */
            if (I2CEMU_SDA_READ(handle) == 0) {
                result = true;
                break;
            }
        }

        I2CEMU_delay(1);

        if (timeout) {
            timeout--;
        }
    } while (timeout);

    /* SCL=0 ends transfer of this byte. */
    I2CEMU_SCL_WRITE(handle, I2CEMU_BIT0);
    I2CEMU_delay(1);

    return result;
}


/** Receive one byte, and send ACK.
 *  @return Received byte
 */
static uint8_t I2CEMU_receiveByte (I2C_Handle handle, bool send_ack)
{
    uint8_t data = 0;
    uint32_t i;
    uint32_t timeout;

    /* Receive eight bits. */
    for (i = 0; i < 8; i++)
    {
        I2CEMU_SCL_WRITE(handle, I2CEMU_BIT0);
        I2CEMU_delay(1);
        I2CEMU_SCL_WRITE(handle, I2CEMU_BIT1);
        timeout = I2CEMU_TIMEOUT;
        do {
            /* Possible clock stretching ended? */
            if (I2CEMU_SCL_READ(handle) == 1)
            {
                /* ACK status is valid now */
                data = (data << 1) | I2CEMU_SDA_READ(handle);
                break;
            }
            else
            {
                I2CEMU_delay(1);
                if (timeout)
                {
                    timeout--;
                }
            }
        } while (timeout);
    }

    /* Send acknowledge */
    I2CEMU_SCL_WRITE(handle, I2CEMU_BIT0);
    I2CEMU_delay(1);
    I2CEMU_SDA_WRITE(handle, send_ack ? I2CEMU_BIT0 : I2CEMU_BIT1);
    I2CEMU_delay(1);
    I2CEMU_SCL_WRITE(handle, I2CEMU_BIT1);
    I2CEMU_delay(2);
    I2CEMU_SCL_WRITE(handle, I2CEMU_BIT0);
    I2CEMU_SDA_WRITE(handle, I2CEMU_BIT1);
    I2CEMU_delay(1);

    return data;
}


/* Open an I2C emulation. */
LPCLIB_Result I2CEMU_open (I2C_Handle *pHandle)
{
        *pHandle = &i2cemu[0];

//TODO
//i2cemu[0].pinSDA = GPIO_5_12;
//i2cemu[0].pinSCL = GPIO_3_13;

//        GPIO_writeBit((*pHandle)->pinSDA, 0);
//        GPIO_writeBit((*pHandle)->pinSCL, 0);
        I2CEMU_SDA_WRITE(*pHandle, I2CEMU_BIT1);
        I2CEMU_SCL_WRITE(*pHandle, I2CEMU_BIT1);
        I2CEMU_start(*pHandle);
        I2CEMU_stop(*pHandle);

        return LPCLIB_SUCCESS;
}



/* Close an I2C bus. */
void I2CEMU_close (I2C_Handle *pHandle)
{
    I2CEMU_SDA_WRITE(*pHandle, I2CEMU_BIT1);
    I2CEMU_SCL_WRITE(*pHandle, I2CEMU_BIT1);
}


LPCLIB_Result I2CEMU_submitJob(I2C_Handle handle, I2C_Job *pJob)
{
    uint8_t data;
    int transferred;
    LPCLIB_Result result = LPCLIB_SUCCESS;
    _Bool ack;
    int sendAddress = 1;

    handle->job = pJob; //needed?

    handle->phase = handle->job->firstPhase;
    while (handle->phase) {
        ack = 1;
        if (sendAddress) {
            I2CEMU_start(handle);

            data = handle->job->slaveAddress << 1;              /* SLA+W */
            if (handle->phase->option == I2C_PHASE_RECEIVE) {
                data |= 1;                                      /* SLA+R */
            }

            ack = I2CEMU_sendByte(handle, data);                /* Send SLA+R/SLA+W */
            sendAddress = 0;
        }

        if (!ack) {
            result = LPCLIB_NO_RESPONSE;
            break;
        }

        transferred = 0;                            /* Reset counter */
        while (transferred < handle->phase->length) {
            if (handle->phase->option == I2C_PHASE_SEND) {
                ack = I2CEMU_sendByte(handle, handle->phase->txstart[transferred]);
            }
            if (handle->phase->option == I2C_PHASE_RECEIVE) {
                data = I2CEMU_receiveByte(handle,
                                          ((transferred + 1) < handle->phase->length) ? true : false);
                handle->phase->rxstart[transferred] = data;
                ack = 1;
            }

            ++transferred;

            if (!ack && (transferred < handle->phase->length)) {
                /* NAK before last byte --> error */
                result = LPCLIB_ERROR;
                break;
            }
        }

        if (!ack) {
            break;
        }

        if (handle->phase->next) {
            if (handle->phase->option != handle->phase->next->option) {
                sendAddress = 1;
            }
        }
        handle->phase = handle->phase->next;
    }

    I2CEMU_stop(handle);

    return result;
}

/* Single buffer write operation */
LPCLIB_Result I2CDEV_write (I2C_Handle bus, uint8_t address, int numBytes, const void *txBuffer)
{
    I2C_Job job;
    I2C_JobPhase phase;

    phase.next = NULL;
    phase.option = I2C_PHASE_SEND;
    phase.txstart = txBuffer;
    phase.length = numBytes;

    job.firstPhase = &phase;
    job.slaveAddress = address;

    return I2CEMU_submitJob(bus, &job);
}



/* Single buffer read operation. */
LPCLIB_Result I2CDEV_read (I2C_Handle bus, uint8_t address, int numBytes, void *rxBuffer)
{
    I2C_Job job;
    I2C_JobPhase phase;

    phase.next = NULL;
    phase.option = I2C_PHASE_RECEIVE;
    phase.rxstart = rxBuffer;
    phase.length = numBytes;

    job.firstPhase = &phase;
    job.slaveAddress = address;

    return I2CEMU_submitJob(bus, &job);
}



/* Single buffer write+read operation. */
LPCLIB_Result I2CDEV_writeAndRead (I2C_Handle bus,
                                   uint8_t address,
                                   int numTxBytes,
                                   const void *txBuffer,
                                   int numRxBytes,
                                   void *rxBuffer)
{
    I2C_Job job;
    LPCLIB_Result result;
#if LPCLIB_RTOS
    I2C_JobPhase *pPhase;

    if (LPCLIB_OSALMemAlloc(2 * sizeof(I2C_JobPhase), (LPCLIB_OSAL_MEMORY *)&pPhase)
            != LPCLIB_OSAL_ERROR_NONE) {
        return LPCLIB_OUT_OF_MEMORY;
    }
#else
    I2C_JobPhase pPhase[2];
#endif

    pPhase[0].next = &pPhase[1];
    pPhase[0].option = I2C_PHASE_SEND;
    pPhase[0].txstart = txBuffer;
    pPhase[0].length = numTxBytes;
    pPhase[1].next = NULL;
    pPhase[1].option = I2C_PHASE_RECEIVE;
    pPhase[1].txstart = rxBuffer;
    pPhase[1].length = numRxBytes;

    job.firstPhase = &pPhase[0];
    job.slaveAddress = address;

    result = I2CEMU_submitJob(bus, &job);

#if LPCLIB_RTOS
    LPCLIB_OSALMemRelease(pPhase);
#endif

    return result;
}

/** Local device context. */
typedef struct PCA9532_Context {
    I2C_Handle bus;                         /**< Bus to which device is connected. */
    uint8_t slaveAddress;                   /**< Slave address on I2C bus. */
} PCA9532_Context;

#define PCA9532_NUM_INSTANCES       1
static PCA9532_Context globalContext[PCA9532_NUM_INSTANCES]; //TODO: Must allocate memory dynamically!!


/** Read a single register.
 *
 *  \param[in] handle Handle of port expander.
 *  \param[in] theReg Register from which to read.
 *  \param[out] pReg Points to buffer that takes register content.
 *  \retval LPCLIB_SUCCESS Ok!
 *  \retval .... Error
 */
static LPCLIB_Result PCA9532_readReg (PCA9532_Handle handle,
                                      enum PCA9532_RegisterNames theReg,
                                      uint8_t *pReg)
{
    uint8_t buf;

    buf = (1u << 4) + theReg;
    return I2CDEV_writeAndRead(handle->bus, handle->slaveAddress, 1, &buf, 1, pReg);
}



/** Write one register.
 *
 *  \param[in] handle Handle of port expander.
 *  \param[in] theReg Register to be written to.
 *  \param[out] pRegs Points to buffer that takes register content.
 *  \retval LPCLIB_SUCCESS Ok!
 *  \retval .... Error
 */
static LPCLIB_Result PCA9532_writeReg (PCA9532_Handle handle,
                                       enum PCA9532_RegisterNames theReg,
                                       const uint8_t *pReg)
{
    uint8_t buf[2];

    buf[0] = (1u << 4) + theReg;
    buf[1] = *pReg;
    return I2CDEV_write(handle->bus, handle->slaveAddress, 2, &buf);
}



LPCLIB_Result PCA9532_open (I2C_Handle bus, const uint8_t address, PCA9532_Handle *pHandle)
{
    PCA9532_Context *pContext;
    int n;


    //TODO!!
    pContext = &globalContext[0];
    pContext->bus = bus;
    pContext->slaveAddress = address;

    *pHandle = pContext;

    return LPCLIB_SUCCESS;
}



/* Close the device. */
LPCLIB_Result PCA9532_close (PCA9532_Handle *pHandle)
{
    return LPCLIB_SUCCESS;
}



LPCLIB_Result PCA9532_ioctl (PCA9532_Handle handle, const PCA9532_Config *pConfig)
{
    enum PCA9532_RegisterNames regNum;
    LPCLIB_Result result = LPCLIB_SUCCESS;
    uint8_t reg;


    while (1) {
        switch (pConfig->opcode) {
        case PCA9532_OPCODE_SET_PWM0:
        case PCA9532_OPCODE_SET_PWM1:
            regNum = (enum PCA9532_RegisterNames)(pConfig->opcode);     /* The opcode is conveniently encoded! */
            result = PCA9532_writeReg(handle, regNum, &pConfig->pwm.period);
            if (result == LPCLIB_SUCCESS) {
                result = PCA9532_writeReg(handle,
                                          (enum PCA9532_RegisterNames)(regNum + 1),
                                          &pConfig->pwm.dutyCycle);  //TODO write both regs in one call!
            }
            break;

        case PCA9532_OPCODE_SET_PIN_FUNCTION:
            if (pConfig->pinFunc.number < 4) {
                regNum = PCA9532_REG_LS0;
            }
            else if (pConfig->pinFunc.number < 8) {
                regNum = PCA9532_REG_LS1;
            }
            else if (pConfig->pinFunc.number < 12) {
                regNum = PCA9532_REG_LS2;
            }
            else {
                regNum = PCA9532_REG_LS3;
            }
            result = PCA9532_readReg(handle, regNum, &reg);
            if (result == LPCLIB_SUCCESS) {
                reg &= ~(3u << (2 * (pConfig->pinFunc.number % 4)));
                reg |= (pConfig->pinFunc.func << (2 * (pConfig->pinFunc.number % 4)));
                result = PCA9532_writeReg(handle, regNum, &reg);
            }
            break;
        }

        if (!pConfig->continued) {
            break;
        }

        ++pConfig;                                      /* More config's follow */
    }

    return result;
}



LPCLIB_Result PCA9532_read (PCA9532_Handle handle, uint16_t *pPortState)
{
    uint8_t buf;
    uint8_t regs[2];
    LPCLIB_Result result;


    buf = PCA9532_REG_INPUT0;
    result = I2CDEV_writeAndRead(handle->bus, handle->slaveAddress, 1, &buf, 1, &regs[0]);
    if (result == LPCLIB_SUCCESS) {
        buf = PCA9532_REG_INPUT1;
        result = I2CDEV_writeAndRead(handle->bus, handle->slaveAddress, 1, &buf, 1, &regs[1]);
    }

    *pPortState = (regs[1] << 8) | regs[0];

    return result;
}


/* Write value to the 16-bit port through a mask. */
void PCA9532_write16WithMask (PCA9532_Handle handle, uint16_t value, uint16_t mask)
{
    int i;
    PCA9532_Config config;

    for (i = 0; i < 16; i++) {
        if (mask & (1u << i)) {
            config.opcode = PCA9532_OPCODE_SET_PIN_FUNCTION;
            config.continued = DISABLE;
            config.pinFunc.number = i;
            config.pinFunc.func = value & (1u << i) ?
                    PCA9532_PINFUNC_ON :
                    PCA9532_PINFUNC_OFF;
            PCA9532_ioctl(handle, &config);
        }
    }
}


/* Write value (0/1) to a PCA9532 port pin. */
void PCA9532_writeBit (PCA9532_Handle handle, PCA9532_Pin pin, uint8_t value)
{
    PCA9532_Config config;


    if (pin < 16) {
        config.opcode = PCA9532_OPCODE_SET_PIN_FUNCTION;
        config.continued = DISABLE;
        config.pinFunc.number = pin;
        config.pinFunc.func = value ? PCA9532_PINFUNC_ON : PCA9532_PINFUNC_OFF;
        PCA9532_ioctl(handle, &config);
    }
}


/* Return current state of a single port pin. */
uint32_t PCA9532_readBit (PCA9532_Handle handle, PCA9532_Pin pin)
{
    uint16_t port;

    if (PCA9532_read(handle, &port) == LPCLIB_SUCCESS) {
        return (port >> pin) & 1;                       /* Extract requested pin */
    }

    return 0;                                           /* Error. Return something anyway */
}

const unsigned short master_palette[256] =
{

/* Free palette entries (colors) remaining: 3
 * Note that palette colors have been converted from 24 bits to 15 bits each.
 *
 *        R    G    B
 *      -------------*/
/*   0) 165, 197, 226 -> */ 0x7314,
/*   1) 156, 195, 226 -> */ 0x7313,
/*   2) 163, 194, 220 -> */ 0x6f14,
/*   3) 154, 186, 213 -> */ 0x6af3,
/*   4) 154, 190, 225 -> */ 0x72f3,
/*   5) 156, 193, 219 -> */ 0x6f13,
/*   6) 162, 190, 225 -> */ 0x72f4,
/*   7) 172, 209, 234 -> */ 0x7755,
/*   8) 162, 188, 214 -> */ 0x6af4,
/*   9) 177, 203, 230 -> */ 0x7336,
/*  10) 169, 205, 240 -> */ 0x7b35,
/*  11) 171, 210, 241 -> */ 0x7b55,
/*  12) 141, 183, 211 -> */ 0x6ad1,
/*  13) 141, 178, 204 -> */ 0x66d1,
/*  14) 182, 214, 242 -> */ 0x7b56,
/*  15) 178, 206, 240 -> */ 0x7b36,
/*  16) 142, 188, 224 -> */ 0x72f1,
/*  17) 142, 192, 225 -> */ 0x7311,
/*  18) 158, 208, 233 -> */ 0x7753,
/*  19) 148, 174, 208 -> */ 0x6ab2,
/*  20) 142, 192, 218 -> */ 0x6f11,
/*  21) 139, 172, 199 -> */ 0x62b1,
/*  22)   7,   4,   6 -> */ 0x0000,
/*  23) 108, 137, 163 -> */ 0x522d,
/*  24)  69,  85, 101 -> */ 0x3148,
/*  25)  57,  80, 105 -> */ 0x3547,
/*  26)  89, 103, 134 -> */ 0x418b,
/*  27)  38,  42,  52 -> */ 0x18a4,
/*  28)  91, 115, 136 -> */ 0x45cb,
/*  29) 120, 146, 168 -> */ 0x564f,
/*  30) 121, 137, 163 -> */ 0x522f,
/*  31) 137, 165, 185 -> */ 0x5e91,
/*  32) 105, 119, 148 -> */ 0x49cd,
/*  33)  24,  23,  24 -> */ 0x0c43,
/*  34) 126, 164, 194 -> */ 0x628f,
/*  35) 108, 146, 171 -> */ 0x564d,
/*  36) 122, 153, 180 -> */ 0x5a6f,
/*  37)  70,  83,  90 -> */ 0x2d48,
/*  38) 125, 161, 182 -> */ 0x5a8f,
/*  39) 139, 173, 208 -> */ 0x6ab1,
/*  40)  16,  20,  37 -> */ 0x1042,
/*  41)  85, 102, 119 -> */ 0x398a,
/*  42)  53,  74,  81 -> */ 0x2926,
/*  43) 106, 115, 138 -> */ 0x45cd,
/*  44) 149, 173, 200 -> */ 0x66b2,
/*  45)  17,  41,  46 -> */ 0x14a2,
/*  46)  10,  10,  22 -> */ 0x0821,
/*  47) 122, 132, 149 -> */ 0x4a0f,
/*  48)  48,  53,  72 -> */ 0x24c6,
/*  49)  41,  52,  57 -> */ 0x1cc5,
/*  50) 149, 148, 164 -> */ 0x5252,
/*  51)  10,  19,  23 -> */ 0x0841,
/*  52)  91, 115, 146 -> */ 0x49cb,
/*  53)  86,  91, 119 -> */ 0x396a,
/*  54)  39,  37,  38 -> */ 0x1084,
/*  55)  73,  72,  87 -> */ 0x2929,
/*  56)  72,  89, 120 -> */ 0x3d69,
/*  57)  76,  99, 134 -> */ 0x4189,
/*  58) 188, 225, 245 -> */ 0x7b97,
/*  59) 206, 216, 230 -> */ 0x7379,
/*  60) 181, 210, 234 -> */ 0x7756,
/*  61) 152, 181, 204 -> */ 0x66d3,
/*  62) 152, 165, 183 -> */ 0x5a93,
/*  63) 252, 250, 247 -> */ 0x7bff,
/*  64) 210, 228, 240 -> */ 0x7b9a,
/*  65) 164, 184, 204 -> */ 0x66f4,
/*  66)  70,  74, 101 -> */ 0x3128,
/*  67) 142, 137, 150 -> */ 0x4a31,
/*  68)  88,  76, 100 -> */ 0x312b,
/*  69) 122, 119, 137 -> */ 0x45cf,
/*  70) 178, 189, 210 -> */ 0x6af6,
/*  71)  76,  57,  73 -> */ 0x24e9,
/*  72) 164, 172, 196 -> */ 0x62b4,
/*  73)  37,  26,  37 -> */ 0x1064,
/*  74) 106,  90, 103 -> */ 0x316d,
/*  75) 179, 173, 194 -> */ 0x62b6,
/*  76)  42,  29,  64 -> */ 0x2065,
/*  77) 103, 107, 134 -> */ 0x41ac,
/*  78) 181, 197, 214 -> */ 0x6b16,
/*  79) 184, 193, 204 -> */ 0x6717,
/*  80) 185, 208, 221 -> */ 0x6f57,
/*  81)  20,  10,  21 -> */ 0x0822,
/*  82) 168, 208, 220 -> */ 0x6f55,
/*  83) 161, 174, 208 -> */ 0x6ab4,
/*  84)  51,  44,  51 -> */ 0x18a6,
/*  85)  54,  52,  55 -> */ 0x18c6,
/*  86) 121, 123, 147 -> */ 0x49ef,
/*  87)  90,  87, 102 -> */ 0x314b,
/*  88) 176, 166, 173 -> */ 0x5696,
/*  89)  21,  52,  70 -> */ 0x20c2,
/*  90)  93, 130, 144 -> */ 0x4a0b,
/*  91)  27,  66,  78 -> */ 0x2503,
/*  92) 100, 110, 144 -> */ 0x49ac,
/*  93)  11,  18,  11 -> */ 0x0441,
/*  94)  37,  55,  44 -> */ 0x14c4,
/*  95)  68,  87,  72 -> */ 0x2548,
/*  96) 111, 139, 116 -> */ 0x3a2d,
/*  97) 138, 168, 144 -> */ 0x4ab1,
/*  98) 116, 107, 117 -> */ 0x39ae,
/*  99)  45,  74,  57 -> */ 0x1d25,
/* 100)  79, 106,  85 -> */ 0x29a9,
/* 101) 153, 181, 160 -> */ 0x52d3,
/* 102)  73,  68,  73 -> */ 0x2509,
/* 103) 122, 103,  78 -> */ 0x258f,
/* 104) 123, 163, 136 -> */ 0x468f,
/* 105) 136, 164, 169 -> */ 0x5691,
/* 106) 168, 192, 206 -> */ 0x6715,
/* 107)  93, 129, 117 -> */ 0x3a0b,
/* 108)  60, 100,  76 -> */ 0x2587,
/* 109) 156, 192, 205 -> */ 0x6713,
/* 110)  58,  97, 100 -> */ 0x3187,
/* 111)  27,  69,  53 -> */ 0x1903,
/* 112) 140, 177, 187 -> */ 0x5ed1,
/* 113)  78, 113, 134 -> */ 0x41c9,
/* 114) 153, 132, 105 -> */ 0x3613,
/* 115) 151, 178, 188 -> */ 0x5ed2,
/* 116) 188, 193, 172 -> */ 0x5717,
/* 117)  24,  33,  27 -> */ 0x0c83,
/* 118)  89,  68,  52 -> */ 0x190b,
/* 119) 155, 161, 171 -> */ 0x5693,
/* 120)  48,  37,  24 -> */ 0x0c86,
/* 121)  23,  18,  11 -> */ 0x0442,
/* 122)  44,  13,  10 -> */ 0x0425,
/* 123)  21,   5,   5 -> */ 0x0002,
/* 124)  52,  41,  39 -> */ 0x10a6,
/* 125) 116, 124, 160 -> */ 0x51ee,
/* 126) 207, 202, 208 -> */ 0x6b39,
/* 127) 116, 109, 133 -> */ 0x41ae,
/* 128) 102,  95, 129 -> */ 0x416c,
/* 129) 181, 186, 201 -> */ 0x66f6,
/* 130)  76,  92, 131 -> */ 0x4169,
/* 131)  84,  53,  42 -> */ 0x14ca,
/* 132)  91,  85,  70 -> */ 0x214b,
/* 133) 183, 165, 142 -> */ 0x4696,
/* 134) 249, 238, 210 -> */ 0x6bbf,
/* 135) 114,  83,  74 -> */ 0x254e,
/* 136) 115,  73,  47 -> */ 0x152e,
/* 137) 215, 198, 178 -> */ 0x5b1a,
/* 138)  87,  42,  13 -> */ 0x04aa,
/* 139) 252, 242, 178 -> */ 0x5bdf,
/* 140) 145, 112,  83 -> */ 0x29d2,
/* 141) 172, 141, 111 -> */ 0x3635,
/* 142)  82,  18,   5 -> */ 0x004a,
/* 143) 122,  99,  55 -> */ 0x198f,
/* 144)  89,  67,  24 -> */ 0x0d0b,
/* 145) 184, 162, 121 -> */ 0x3e97,
/* 146)  79,  61,  96 -> */ 0x30e9,
/* 147) 207, 172, 143 -> */ 0x46b9,
/* 148)  89,  70,  71 -> */ 0x210b,
/* 149) 238, 206, 176 -> */ 0x5b3d,
/* 150) 217, 195, 152 -> */ 0x4f1b,
/* 151) 133, 124, 139 -> */ 0x45f0,
/* 152)  56,  50,  41 -> */ 0x14c7,
/* 153)  68,  77, 114 -> */ 0x3928,
/* 154)  90,  83,  90 -> */ 0x2d4b,
/* 155) 109,  19,   4 -> */ 0x004d,
/* 156) 169,  57,  37 -> */ 0x10f5,
/* 157) 168,  90,  70 -> */ 0x2175,
/* 158) 169,  87,  43 -> */ 0x1555,
/* 159) 139,  56,  39 -> */ 0x10f1,
/* 160) 199,  91,  69 -> */ 0x2178,
/* 161) 198,  88,  41 -> */ 0x1578,
/* 162) 170,  16,   3 -> */ 0x0055,
/* 163) 224,  83,  36 -> */ 0x115c,
/* 164) 138,  19,   3 -> */ 0x0051,
/* 165) 206,  50,   6 -> */ 0x00d9,
/* 166) 236,  86,   3 -> */ 0x015d,
/* 167)  70,  24,  39 -> */ 0x1068,
/* 168) 200,  87,   7 -> */ 0x0159,
/* 169) 145, 119, 106 -> */ 0x35d2,
/* 170)   2,  31,  71 -> */ 0x2060,
/* 171) 203,  14,   2 -> */ 0x0039,
/* 172) 234,  51,   4 -> */ 0x00dd,
/* 173)  57, 102, 136 -> */ 0x4587,
/* 174) 166, 155, 164 -> */ 0x5274,
/* 175) 108,  57,  39 -> */ 0x10ed,
/* 176) 168, 148, 149 -> */ 0x4a55,
/* 177) 231,  21,   3 -> */ 0x005c,
/* 178) 139,  89,  71 -> */ 0x2171,
/* 179) 200, 153, 137 -> */ 0x4679,
/* 180) 199, 187, 196 -> */ 0x62f8,
/* 181) 199, 155, 166 -> */ 0x5278,
/* 182) 169,  53,   8 -> */ 0x04d5,
/* 183) 229, 121,  43 -> */ 0x15fc,
/* 184) 238, 114,   8 -> */ 0x05dd,
/* 185) 168, 121, 103 -> */ 0x31f5,
/* 186) 202, 180, 176 -> */ 0x5ad9,
/* 187) 235, 214, 204 -> */ 0x675d,
/* 188) 197, 122, 102 -> */ 0x31f8,
/* 189) 239, 142,  48 -> */ 0x1a3d,
/* 190) 229, 122,  72 -> */ 0x25fc,
/* 191) 246, 172,  80 -> */ 0x2abe,
/* 192) 251, 206,  79 -> */ 0x273f,
/* 193) 247, 142,  11 -> */ 0x063e,
/* 194) 235, 147,  77 -> */ 0x265d,
/* 195) 204, 105,  13 -> */ 0x05b9,
/* 196) 250, 206,  47 -> */ 0x173f,
/* 197) 251, 176,   8 -> */ 0x06df,
/* 198) 250, 174,  48 -> */ 0x1abf,
/* 199) 231, 186, 168 -> */ 0x56fc,
/* 200) 240, 173, 111 -> */ 0x36be,
/* 201) 252, 246,  46 -> */ 0x17df,
/* 202) 253, 247,  80 -> */ 0x2bdf,
/* 203) 245, 202, 146 -> */ 0x4b3e,
/* 204) 253, 248, 110 -> */ 0x37ff,
/* 205) 254, 248, 143 -> */ 0x47ff,
/* 206) 251, 204, 112 -> */ 0x3b3f,
/* 207) 221, 170,  41 -> */ 0x16bb,
/* 208) 204, 114,  45 -> */ 0x15d9,
/* 209) 221, 167,  26 -> */ 0x0e9b,
/* 210) 251, 206,   7 -> */ 0x033f,
/* 211) 253, 249,   5 -> */ 0x03ff,
/* 212) 229, 220, 227 -> */ 0x737c,
/* 213) 217, 131,  22 -> */ 0x0a1b,
/* 214) 183, 100,  16 -> */ 0x0996,
/* 215) 235, 180, 141 -> */ 0x46dd,
/* 216) 203, 147, 111 -> */ 0x3659,
/* 217) 231, 152, 105 -> */ 0x367c,
/* 218) 201, 119,  74 -> */ 0x25d9,
/* 219) 175,  82,   9 -> */ 0x0555,
/* 220) 208, 138,  80 -> */ 0x2a3a,
/* 221) 175, 105,  49 -> */ 0x19b5,
/* 222) 171, 114,  79 -> */ 0x25d5,
/* 223) 139,  52,   8 -> */ 0x04d1,
/* 224) 143,  74,  16 -> */ 0x0931,
/* 225) 229, 154, 133 -> */ 0x427c,
/* 226) 207, 165, 118 -> */ 0x3a99,
/* 227) 214, 132,  52 -> */ 0x1a1a,
/* 228) 149, 101,  53 -> */ 0x1992,
/* 229) 174, 133,  87 -> */ 0x2a15,
/* 230) 142,  83,  46 -> */ 0x1551,
/* 231) 221, 226, 209 -> */ 0x6b9b,
/* 232) 111,  50,   9 -> */ 0x04cd,
/* 233) 180, 132,  55 -> */ 0x1a16,
/* 234) 119,  68,  19 -> */ 0x090e,
/* 235) 151, 130,  91 -> */ 0x2e12,
/* 236)  87, 108, 145 -> */ 0x49aa,
/* 237) 115, 111, 145 -> */ 0x49ae,
/* 238) 227, 187, 201 -> */ 0x66fc,
/* 239)  56,  91, 129 -> */ 0x4167,
/* 240)  87,  74,  86 -> */ 0x292a,
/* 241) 167, 122, 134 -> */ 0x41f4,
/* 242)  28,  71, 108 -> */ 0x3503,
/* 243) 135,  91, 104 -> */ 0x3570,
/* 244) 108,  57,  73 -> */ 0x24ed,
/* 245) 151,  99,  24 -> */ 0x0d92,
/* 246) 135,  56,  64 -> */ 0x20f0,
/* 247) 161,  89, 108 -> */ 0x3574,
/* 248) 103,  21,  53 -> */ 0x184c,
/* 249) 216, 165,  87 -> */ 0x2a9b,
/* 250) 153, 134,  60 -> */ 0x1e13,
/* 251) 122, 102,  24 -> */ 0x0d8f,
/* 252) 210, 192, 127 -> */ 0x3f1a,
/* 253)   0,   0,   0 -> */ 0x0000,
/* 254)   0,   0,   0 -> */ 0x0000,
/* 255)   0,   0,   0 -> */ 0x0000
};

/**
 * @}
 */
