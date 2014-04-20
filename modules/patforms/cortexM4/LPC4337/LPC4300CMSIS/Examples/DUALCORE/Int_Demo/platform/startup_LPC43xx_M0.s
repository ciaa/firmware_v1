;/******************************************************************************
; * @file:    startup_LPC43xx_M0.s (M0  core inside LPC43xx)
; * @purpose: CMSIS ARM Cortex-M0 Core Device Startup File
;
; * @version: V1.02
; * @date:    27. July 2009
; *------- <<< Use Configuration Wizard in Context Menu >>> ------------------
; *
; * Copyright (C) 2009 ARM Limited. All rights reserved.
; * ARM Limited (ARM) is supplying this software for use with Cortex-M3
; * processor based microcontrollers.  This file can be freely distributed
; * within development tools that are supporting such ARM based processors.
; *
; * THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
; * OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
; * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
; * ARM SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL, OR
; * CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
; *
; *****************************************************************************/


; <h> Stack Configuration
;   <o> Stack Size (in Bytes) <0x0-0xFFFFFFFF:8>
; </h>

Stack_Size      EQU     0x00000400

                AREA    STACK, NOINIT, READWRITE, ALIGN=3
Stack_Mem       SPACE   Stack_Size
__initial_sp


; <h> Heap Configuration
;   <o>  Heap Size (in Bytes) <0x0-0xFFFFFFFF:8>
; </h>

Heap_Size       EQU     0x00000000

                AREA    HEAP, NOINIT, READWRITE, ALIGN=3
__heap_base
Heap_Mem        SPACE   Heap_Size
__heap_limit


                PRESERVE8
                THUMB


; Vector Table Mapped to Address 0 at Reset

                AREA    RESET, DATA, READONLY
                EXPORT  __Vectors

__Vectors       DCD     __initial_sp              ; 0 Top of Stack
                DCD     Reset_Handler             ; 1 Reset Handler
                DCD     NMI_Handler               ; 2 NMI Handler
                DCD     HardFault_Handler         ; 3 Hard Fault Handler
                DCD     0                         ; 4 Reserved
                DCD     0                         ; 5 Reserved
                DCD     0                         ; 6 Reserved
                DCD     0                         ; 7 Reserved
                DCD     0                         ; 8 Reserved
                DCD     0                         ; 9 Reserved
                DCD     0                         ; 10 Reserved
                DCD     SVC_Handler               ; 11 SVCall Handler 
                DCD     0          				  ; 12 Reserved
                DCD     0                         ; 13 Reserved
                DCD     PendSV_Handler            ; 14 PendSV Handler 
                DCD     0				       	  ; 15 SysTick Handler not implemented on this core

                ; External Interrupts				
				DCD		RTC_IRQHandler	 			; 16 RTC
				DCD		IPC_Master2Slave_IRQHandler	; 17 M4
				DCD		DMA_IRQHandler				; 18 General Purpose DMA
				DCD		0							; 19 Reserved
				DCD		ATIMER_FLASH_EEPROM_IRQHandler	; 20 ATIMER, FLASH0/1 and EEPROM or'ed
				DCD		ETH_IRQHandler				; 21 ETHERNET
				DCD		SDIO_IRQHandler				; 22 SD/MMC
				DCD		LCD_IRQHandler				; 23 LCD
				DCD		USB0_IRQHandler				; 24 USB0
				DCD		USB1_IRQHandler				; 25 USB1
				DCD		SCT_IRQHandler				; 26 State Configurable Timer
				DCD		RIT_WDT_IRQHandler			; 27 Repetitive Interrupt Timer and WDT or'ed - used for FreeRTOS as SysTick
				DCD		TIMER0_IRQHandler			; 28 Timer0
				DCD		GINT1_IRQHandler			; 28 GINT1
				DCD		GPIO4_IRQHandler			; 29 GPIO4
				DCD		TIMER3_IRQHandler			; 30 Timer3
				DCD		MCPWM_IRQHandler			; 32 Motor Control PWM
				DCD		ADC0_IRQHandler				; 33 A/D Converter 0
				DCD		I2C0_I2C1_IRQHandler		; 34 I2C0 / I2C1 or'ed
				DCD		SGPIO_IRQHandler			; 35 SGPIO
				DCD		SPI_DAC_IRQHandler			; 36 SPI and DAC
				DCD		ADC1_IRQHandler				; 37 A/D Converter 1
				DCD		SSP0_SSP1_IRQHandler		; 38 SSP0 and SSP1 or'ed
				DCD		EVRT_IRQHandler				; 39 Event Router
				DCD		UART0_IRQHandler			; 40 UART0
				DCD		UART1_IRQHandler			; 41 UART1
				DCD		UART2_CAN1_IRQHandler		; 42 UART2 and C_CAN1 or'ed
				DCD		UART3_IRQHandler			; 43 UART3
				DCD		I2S0_I2S1_QEI_IRQHandler	; 44 I2S0 / I2S1 / QEI or'ed
				DCD		CAN0_IRQHandler				; 45 C_CAN0
				DCD		SPIFI_VADC_IRQHandler		; 46 SPIFI and VADC or'ed
				DCD		0							; 47 Reserved

                AREA    |.text|, CODE, READONLY
				

; Reset Handler

Reset_Handler   PROC
                EXPORT  Reset_Handler             [WEAK]
                IMPORT  __main
                LDR     R0, =__main
                BX      R0
                ENDP


; Dummy Exception Handlers (infinite loops which can be modified)

NMI_Handler     PROC
                EXPORT  NMI_Handler               [WEAK]
                B       .
                ENDP
HardFault_Handler\
                PROC
                EXPORT  HardFault_Handler         [WEAK]
                B       .
                ENDP

; FreeRTOS handler
vPortSVCHandler\
     			PROC
				EXPORT 	vPortSVCHandler		[WEAK]
                B       .
                ENDP

SVC_Handler     PROC
                EXPORT  SVC_Handler               [WEAK]
                B       .
                ENDP

; FreeRTOS handler
xPortPendSVHandler\
			  	PROC
                EXPORT  xPortPendSVHandler      [WEAK]
                B       .
                ENDP

PendSV_Handler  PROC
                EXPORT  PendSV_Handler            [WEAK]
                B       .
                ENDP

; FreeRTOS handlers
xPortSysTickHandler\
				PROC
                EXPORT  xPortSysTickHandler		[WEAK]
                B       .
                ENDP

xPortRiTimerHandler\
				PROC
                EXPORT  xPortRiTimerHandler		[WEAK]
                B       .
                ENDP

SysTick_Handler PROC
                EXPORT  SysTick_Handler           [WEAK]
                B       .
                ENDP

Default_Handler PROC

				EXPORT	RTC_IRQHandler	 		[WEAK]
				EXPORT  IPC_Master2Slave_IRQHandler [WEAK]
				EXPORT	M4_IRQHandler			[WEAK]
				EXPORT	DMA_IRQHandler			[WEAK]
				EXPORT	ATIMER_FLASH_EEPROM_IRQHandler	[WEAK]
				EXPORT	ETH_IRQHandler			[WEAK]
				EXPORT	SDIO_IRQHandler			[WEAK]
				EXPORT	LCD_IRQHandler			[WEAK]
				EXPORT	USB0_IRQHandler			[WEAK]
				EXPORT	USB1_IRQHandler			[WEAK]
				EXPORT	SCT_IRQHandler			[WEAK]
				EXPORT	RIT_WDT_IRQHandler		[WEAK]
				EXPORT	TIMER0_IRQHandler		[WEAK]
				EXPORT	GINT1_IRQHandler		[WEAK]
				EXPORT	GPIO4_IRQHandler		[WEAK]
				EXPORT	TIMER3_IRQHandler		[WEAK]
				EXPORT	MCPWM_IRQHandler		[WEAK]
				EXPORT	ADC0_IRQHandler			[WEAK]
				EXPORT	I2C0_I2C1_IRQHandler	[WEAK]
				EXPORT	SGPIO_IRQHandler		[WEAK]
				EXPORT	SPI_DAC_IRQHandler		[WEAK]
				EXPORT	ADC1_IRQHandler			[WEAK]
				EXPORT	SSP0_SSP1_IRQHandler	[WEAK]
				EXPORT	EVRT_IRQHandler			[WEAK]
				EXPORT	UART0_IRQHandler		[WEAK]
				EXPORT	UART1_IRQHandler		[WEAK]
				EXPORT	UART2_CAN1_IRQHandler	[WEAK]
				EXPORT	UART3_IRQHandler		[WEAK]
				EXPORT	I2S0_I2S1_QEI_IRQHandler	[WEAK]
				EXPORT	CAN0_IRQHandler			[WEAK]
				EXPORT	SPIFI_VADC_IRQHandler	[WEAK]

RTC_IRQHandler
M4_IRQHandler
IPC_Master2Slave_IRQHandler
DMA_IRQHandler
ATIMER_FLASH_EEPROM_IRQHandler
ETH_IRQHandler
SDIO_IRQHandler
LCD_IRQHandler
USB0_IRQHandler
USB1_IRQHandler
SCT_IRQHandler
RIT_WDT_IRQHandler
TIMER0_IRQHandler
GINT1_IRQHandler
GPIO4_IRQHandler
TIMER3_IRQHandler
MCPWM_IRQHandler
ADC0_IRQHandler
I2C0_I2C1_IRQHandler
SGPIO_IRQHandler
SPI_DAC_IRQHandler
ADC1_IRQHandler
SSP0_SSP1_IRQHandler
EVRT_IRQHandler
UART0_IRQHandler
UART1_IRQHandler
UART2_CAN1_IRQHandler
UART3_IRQHandler
I2S0_I2S1_QEI_IRQHandler
CAN0_IRQHandler
SPIFI_VADC_IRQHandler


				B       .
				ENDP


                ALIGN


; User Initial Stack & Heap

                IF      :DEF:__MICROLIB

                EXPORT  __initial_sp
                EXPORT  __heap_base
                EXPORT  __heap_limit

                ELSE

                IMPORT  __use_two_region_memory
                EXPORT  __user_initial_stackheap
__user_initial_stackheap

                LDR     R0, =  Heap_Mem
                LDR     R1, =(Stack_Mem + Stack_Size)
                LDR     R2, = (Heap_Mem +  Heap_Size)
                LDR     R3, = Stack_Mem
                BX      LR

                ALIGN

                ENDIF


                END
