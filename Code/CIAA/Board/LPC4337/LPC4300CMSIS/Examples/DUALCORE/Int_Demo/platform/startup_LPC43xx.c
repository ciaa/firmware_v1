
#include "platform_init.h"

/* External common functions */
extern int __main (void);
extern void SystemInit (void);

#define ALIAS(def) __attribute__((weak,alias(#def)))

void Default_IRQHandler (void);

void Reset_Handler (void)		__attribute__ ((section("SYSTEM_RESET_VECTORS"), used));
void NMI_Handler (void) 		__attribute__ ((section("SYSTEM_RESET_VECTORS"), used));
void HardFault_Handler(void)  	__attribute__ ((section("SYSTEM_RESET_VECTORS"), used));
void DebugMon_Handler(void)  	__attribute__ ((section("SYSTEM_RESET_VECTORS"), used));

void SVC_Handler (void) ALIAS(Default_IRQHandler);
void PendSV_Handler (void) ALIAS(Default_IRQHandler);
void SysTick_Handler (void) ALIAS(Default_IRQHandler);

/* freeRTOS versions of the OS calls */
void vPortSVCHandler (void) ALIAS(Default_IRQHandler);
void xPortPendSVHandler (void) ALIAS(Default_IRQHandler);
void xPortSysTickHandler (void) ALIAS(Default_IRQHandler);

/* common default interrupt handler definitions */

void NMI_Handler (void)	{ 

    while (1);
}

void HardFault_Handler (void) {
    
 	while (1);
}

void DebugMon_Handler (void) {

    while (1);
}

void Default_IRQHandler (void) {

    while (1);
}

/*****************************************************************************/
/* M4 Core specifics */
/*****************************************************************************/


#if defined(CORE_M4)

extern void fpuEnable(void);
extern void platformInit(void);

extern void copyStartupCode2IntRam(void);


/* Default interrupt service routines.
 * Defined as weak symbols. Can be redefined in the application.
 */
void MemManage_Handler(void)  	__attribute__ ((section("SYSTEM_RESET_VECTORS"), used));
void BusFault_Handler(void)  	__attribute__ ((section("SYSTEM_RESET_VECTORS"), used));
void UsageFault_Handler(void)  	__attribute__ ((section("SYSTEM_RESET_VECTORS"), used));
void Sign_Value (void) 			__attribute__ ((section("SYSTEM_RESET_VECTORS"), used));

/* */
void DAC_IRQHandler (void) ALIAS(Default_IRQHandler);
void M0_IRQHandler (void) ALIAS(Default_IRQHandler);
void DMA_IRQHandler (void) ALIAS(Default_IRQHandler);
void FLASH_EEPROM_IRQHandler (void) ALIAS(Default_IRQHandler);
void ETH_IRQHandler (void) ALIAS(Default_IRQHandler);
void SDIO_IRQHandler (void) ALIAS(Default_IRQHandler);
void LCD_IRQHandler (void) ALIAS(Default_IRQHandler);
void USB0_IRQHandler (void) ALIAS(Default_IRQHandler);
void USB1_IRQHandler (void) ALIAS(Default_IRQHandler);
void SCT_IRQHandler (void) ALIAS(Default_IRQHandler);
void RIT_IRQHandler (void) ALIAS(Default_IRQHandler);
void TIMER0_IRQHandler (void) ALIAS(Default_IRQHandler);
void TIMER1_IRQHandler (void) ALIAS(Default_IRQHandler);
void TIMER2_IRQHandler (void) ALIAS(Default_IRQHandler);
void TIMER3_IRQHandler (void) ALIAS(Default_IRQHandler);
void MCPWM_IRQHandler (void) ALIAS(Default_IRQHandler);
void ADC0_IRQHandler (void) ALIAS(Default_IRQHandler);
void I2C0_IRQHandler (void) ALIAS(Default_IRQHandler);
void I2C1_IRQHandler (void) ALIAS(Default_IRQHandler);
void SPI_IRQHandler (void) ALIAS(Default_IRQHandler);
void ADC1_IRQHandler (void) ALIAS(Default_IRQHandler);
void SSP0_IRQHandler (void) ALIAS(Default_IRQHandler);
void SSP1_IRQHandler (void) ALIAS(Default_IRQHandler);
void UART0_IRQHandler (void) ALIAS(Default_IRQHandler);
void UART1_IRQHandler (void) ALIAS(Default_IRQHandler);
void UART2_IRQHandler (void) ALIAS(Default_IRQHandler);
void UART3_IRQHandler (void) ALIAS(Default_IRQHandler);
void I2S0_IRQHandler (void) ALIAS(Default_IRQHandler);
void I2S1_IRQHandler (void) ALIAS(Default_IRQHandler);
void SPIFI_IRQHandler (void) ALIAS(Default_IRQHandler);
void SGPIO_IRQHandler (void) ALIAS(Default_IRQHandler);
void GPIO0_IRQHandler (void) ALIAS(Default_IRQHandler);
void GPIO1_IRQHandler (void) ALIAS(Default_IRQHandler);
void GPIO2_IRQHandler (void) ALIAS(Default_IRQHandler);
void GPIO3_IRQHandler (void) ALIAS(Default_IRQHandler);
void GPIO4_IRQHandler (void) ALIAS(Default_IRQHandler);
void GPIO5_IRQHandler (void) ALIAS(Default_IRQHandler);
void GPIO6_IRQHandler (void) ALIAS(Default_IRQHandler);
void GPIO7_IRQHandler (void) ALIAS(Default_IRQHandler);
void GINT0_IRQHandler (void) ALIAS(Default_IRQHandler);
void GINT1_IRQHandler (void) ALIAS(Default_IRQHandler);
void EVRT_IRQHandler (void) ALIAS(Default_IRQHandler);
void CAN1_IRQHandler (void) ALIAS(Default_IRQHandler);
void VADC_IRQHandler (void) ALIAS(Default_IRQHandler);
void ATIMER_IRQHandler (void) ALIAS(Default_IRQHandler);
void RTC_IRQHandler (void) ALIAS(Default_IRQHandler);
void WDT_IRQHandler (void) ALIAS(Default_IRQHandler);
void CAN0_IRQHandler (void) ALIAS(Default_IRQHandler);
void QEI_IRQHandler (void) ALIAS(Default_IRQHandler);

/* hook for the IPC interrupt routine */
void IPC_Slave2Master_IRQHandler(void) ALIAS(Default_IRQHandler);


#define SIGN_VALUE      0x5A5A5A5A

extern void Image$$ARM_LIB_STACK$$ZI$$Limit;


/** Vector table. Placed in section called RESET
 */
void (* const vector_table[])(void) __attribute__((section("RESET"), used)) =
{
    (void(*)(void)) &Image$$ARM_LIB_STACK$$ZI$$Limit,
    Reset_Handler,
    NMI_Handler,
    HardFault_Handler,
    MemManage_Handler,
    BusFault_Handler,
    UsageFault_Handler,
    (void(*)(void)) SIGN_VALUE,
    0,
    0,
    0,
    SVC_Handler,
    DebugMon_Handler,
    0,
    PendSV_Handler,
    SysTick_Handler,

    DAC_IRQHandler,                         /* 16 */
    IPC_Slave2Master_IRQHandler,            /* 17 */
    DMA_IRQHandler,                         /* 18 */
    0,
    FLASH_EEPROM_IRQHandler,                /* 20 Reserved for Typhoon */
    ETH_IRQHandler,                         /* 21 Ethernet */
    SDIO_IRQHandler,                        /* 22 SD/MMC */
    LCD_IRQHandler,                         /* 23 LCD */
    USB0_IRQHandler,                        /* 24 USB0 */
    USB1_IRQHandler,                        /* 25 USB1 */
    SCT_IRQHandler,                         /* 26 State Configurable Timer */
    RIT_IRQHandler,                         /* 27 Repetitive Interrupt Timer */
    TIMER0_IRQHandler,                      /* 28 Timer0 */
    TIMER1_IRQHandler,                      /* 29 Timer1 */
    TIMER2_IRQHandler,                      /* 30 Timer2 */
    TIMER3_IRQHandler,                      /* 31 Timer3 */
    MCPWM_IRQHandler,                       /* 32 Motor Control PWM */
    ADC0_IRQHandler,                        /* 33 A/D Converter 0 */
    I2C0_IRQHandler,                        /* 34 I2C0 */
    I2C1_IRQHandler,                        /* 35 I2C1 */
    SPI_IRQHandler,                         /* 36 SPI */
    ADC1_IRQHandler,                        /* 37 A/D Converter 1 */
    SSP0_IRQHandler,                        /* 38 SSP0 */
    SSP1_IRQHandler,                        /* 39 SSP1 */
    UART0_IRQHandler,                       /* 40 UART0 */
    UART1_IRQHandler,                       /* 41 UART1 */
    UART2_IRQHandler,                       /* 42 UART2 */
    UART3_IRQHandler,                       /* 43 UART3 */
    I2S0_IRQHandler,                        /* 44 I2S0 */
    I2S1_IRQHandler,                        /* 45 I2S1 */
    SPIFI_IRQHandler,                       /* 46 SPI Flash Interface */
    SGPIO_IRQHandler,                       /* 47 SGPIO */
    GPIO0_IRQHandler,                       /* 48 GPIO0 */
    GPIO1_IRQHandler,                       /* 49 GPIO1 */
    GPIO2_IRQHandler,                       /* 50 GPIO2 */
    GPIO3_IRQHandler,                       /* 51 GPIO3 */
    GPIO4_IRQHandler,                       /* 52 GPIO4 */
    GPIO5_IRQHandler,                       /* 53 GPIO5 */
    GPIO6_IRQHandler,                       /* 54 GPIO6 */
    GPIO7_IRQHandler,                       /* 55 GPIO7 */
    GINT0_IRQHandler,                       /* 56 GINT0 */
    GINT1_IRQHandler,                       /* 57 GINT1 */
    EVRT_IRQHandler,                        /* 58 Event Router */
    CAN1_IRQHandler,                        /* 59 C_CAN1 */
    0,
    VADC_IRQHandler,                        /* 61 VADC */
    ATIMER_IRQHandler,                      /* 62 ATIMER */
    RTC_IRQHandler,                         /* 63 RTC */
    0,
    WDT_IRQHandler,                         /* 65 WDT */
    0,
    CAN0_IRQHandler,                        /* 67 C_CAN0 */
    QEI_IRQHandler,                         /* 68 QEI */
};


/* CRP key placed in section CRP */
__attribute__ ((section("CRP"), used))
const unsigned long CRP_Key = 0xFFFFFFFF;



void Reset_Handler (void) {
	


	/* checks for presence of an FPU unit and enables */
	fpuEnable();
	
	/* copy the EMC initialization code within ram to execute */
	//#if (INITIALIZE_EMC == YES)
	copyStartupCode2IntRam();
	//#endif
	/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!  */
	/* Globals not yet initialized           !! */
	/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!  */	
    /* call system init, just configures the SystemCoreClock */
	SystemInit();   

	/* initialize the platform before main */
	platformInit();


	/* Jump to the library entry point __main() */
    __main();

    /* Loop here forever, in case main() ever returns. */
    while (1) {
    }
}




/* cortex M4 specific system interrupt handlers */
void MemManage_Handler (void) {

    while (1);
}

void BusFault_Handler (void) {
    
	while (1);
}

void UsageFault_Handler (void) {

    while (1);
}



#endif





/*****************************************************************************/
/* M0 Core */
/*****************************************************************************/
#if defined(CORE_M0)

/* External functions */
extern int __main (void);

/* Default interrupt service routines.
 * Defined as weak symbols. Can be redefined in the application.
 */
#define ALIAS(def) __attribute__((weak,alias(#def)))

void Default_IRQHandler (void);

void Reset_Handler (void);
void NMI_Handler (void);
void HardFault_Handler (void);
void BusFault_Handler (void);
void UsageFault_Handler (void);
void Sign_Value (void);
void DebugMon_Handler (void);

void SVC_Handler (void) ALIAS(Default_IRQHandler);
void PendSV_Handler (void) ALIAS(Default_IRQHandler);
void SysTick_Handler (void) ALIAS(Default_IRQHandler);

/* freeRTOS versions of the OS calls */
void vPortSVCHandler (void) ALIAS(Default_IRQHandler);
void xPortPendSVHandler (void) ALIAS(Default_IRQHandler);
void xPortSysTickHandler (void) ALIAS(Default_IRQHandler);

/* hook for the IPC interrupt routine */
void IPC_Master2Slave_IRQHandler(void) ALIAS(Default_IRQHandler);

extern void Image$$ARM_LIB_STACK$$ZI$$Limit;

void DAC_IRQHandler (void) ALIAS(Default_IRQHandler);
void M4_IRQHandler (void) ALIAS(Default_IRQHandler);
void DMA_IRQHandler (void) ALIAS(Default_IRQHandler);
void FLASH_EEPROM_IRQHandler (void) ALIAS(Default_IRQHandler);
void ETH_IRQHandler (void) ALIAS(Default_IRQHandler);
void SDIO_IRQHandler (void) ALIAS(Default_IRQHandler);
void LCD_IRQHandler (void) ALIAS(Default_IRQHandler);
void USB0_IRQHandler (void) ALIAS(Default_IRQHandler);
void USB1_IRQHandler (void) ALIAS(Default_IRQHandler);
void SCT_IRQHandler (void) ALIAS(Default_IRQHandler);
void RIT_IRQHandler (void) ALIAS(Default_IRQHandler);
void TIMER0_IRQHandler (void) ALIAS(Default_IRQHandler);
void TIMER1_IRQHandler (void) ALIAS(Default_IRQHandler);
void TIMER2_IRQHandler (void) ALIAS(Default_IRQHandler);
void TIMER3_IRQHandler (void) ALIAS(Default_IRQHandler);
void MCPWM_IRQHandler (void) ALIAS(Default_IRQHandler);
void ADC0_IRQHandler (void) ALIAS(Default_IRQHandler);
void I2C0_IRQHandler (void) ALIAS(Default_IRQHandler);
void I2C1_IRQHandler (void) ALIAS(Default_IRQHandler);
void SPI_IRQHandler (void) ALIAS(Default_IRQHandler);
void ADC1_IRQHandler (void) ALIAS(Default_IRQHandler);
void SSP0_IRQHandler (void) ALIAS(Default_IRQHandler);
void SSP1_IRQHandler (void) ALIAS(Default_IRQHandler);
void UART0_IRQHandler (void) ALIAS(Default_IRQHandler);
void UART1_IRQHandler (void) ALIAS(Default_IRQHandler);
void UART2_IRQHandler (void) ALIAS(Default_IRQHandler);
void UART3_IRQHandler (void) ALIAS(Default_IRQHandler);
void I2S0_IRQHandler (void) ALIAS(Default_IRQHandler);
void I2S1_IRQHandler (void) ALIAS(Default_IRQHandler);
void SPIFI_IRQHandler (void) ALIAS(Default_IRQHandler);
void SGPIO_IRQHandler (void) ALIAS(Default_IRQHandler);
void GPIO0_IRQHandler (void) ALIAS(Default_IRQHandler);
void GPIO1_IRQHandler (void) ALIAS(Default_IRQHandler);
void GPIO2_IRQHandler (void) ALIAS(Default_IRQHandler);
void GPIO3_IRQHandler (void) ALIAS(Default_IRQHandler);
void GPIO4_IRQHandler (void) ALIAS(Default_IRQHandler);
void GPIO5_IRQHandler (void) ALIAS(Default_IRQHandler);
void GPIO6_IRQHandler (void) ALIAS(Default_IRQHandler);
void GPIO7_IRQHandler (void) ALIAS(Default_IRQHandler);
void GINT0_IRQHandler (void) ALIAS(Default_IRQHandler);
void GINT1_IRQHandler (void) ALIAS(Default_IRQHandler);
void EVRT_IRQHandler (void) ALIAS(Default_IRQHandler);
void CAN1_IRQHandler (void) ALIAS(Default_IRQHandler);
void VADC_IRQHandler (void) ALIAS(Default_IRQHandler);
void ATIMER_IRQHandler (void) ALIAS(Default_IRQHandler);
void RTC_IRQHandler (void) ALIAS(Default_IRQHandler);
void WDT_IRQHandler (void) ALIAS(Default_IRQHandler);
void CAN0_IRQHandler (void) ALIAS(Default_IRQHandler);
void QEI_IRQHandler (void) ALIAS(Default_IRQHandler);



#define SIGN_VALUE      0x5A5A5A5A

/** Vector table. Placed in section called RESET
 */
void (* const vector_table[])(void) __attribute__ ((section("RESET"), used)) =
{
    (void(*)(void)) &Image$$ARM_LIB_STACK$$ZI$$Limit,
    Reset_Handler,
    NMI_Handler,
    HardFault_Handler,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    SVC_Handler,
    DebugMon_Handler,
    0,
    PendSV_Handler,
    SysTick_Handler,

    RTC_IRQHandler,
    IPC_Master2Slave_IRQHandler,
    DMA_IRQHandler,
    0,
    0,
    ETH_IRQHandler,                         /* 21 Ethernet */
    SDIO_IRQHandler,                        /* 22 SD/MMC */
    LCD_IRQHandler,                         /* 23 LCD */
    USB0_IRQHandler,                        /* 24 USB0 */
    USB1_IRQHandler,                        /* 25 USB1 */
    SCT_IRQHandler,                         /* 26 State Configurable Timer */
    RIT_IRQHandler,                         /* 27 Repetitive Interrupt Timer - WWDT timer */ 
    TIMER0_IRQHandler,                      /* 28 Timer0 */
    GINT1_IRQHandler,
    TIMER3_IRQHandler,
    0,
    0,
    MCPWM_IRQHandler,                       /* 32 Motor Control PWM */
    ADC0_IRQHandler,                        /* 33 A/D Converter 0 */
    I2C0_IRQHandler,                        /* 34 I2C0 */ //TODO Optional I2C1
    SGPIO_IRQHandler,                       /* 47 SGPIO */
    SPI_IRQHandler,                         /* 36 SPI */ //optional DAC
    ADC1_IRQHandler,                        /* 37 A/D Converter 1 */
    SSP0_IRQHandler,                        /* 38 SSP0 */ //TODO optional SSP1
    EVRT_IRQHandler,                        /* 58 Event Router */
    UART0_IRQHandler,                       /* 40 UART0 */
    UART2_IRQHandler,                       /* 42 UART2 */ //TODO optinbal CAN1
    UART3_IRQHandler,
    I2S0_IRQHandler,                        /* 44 I2S0 */ //TODO optional I2S1 or QEI
    CAN0_IRQHandler,
};


void Reset_Handler (void) {

    /* Jump to the library entry point __main() */
    __main();

    /* Loop here forever, in case main() ever returns. */
    while (1) {
    }
}


#endif





