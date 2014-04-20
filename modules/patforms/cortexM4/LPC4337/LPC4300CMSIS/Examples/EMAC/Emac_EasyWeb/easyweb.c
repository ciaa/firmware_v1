/******************************************************************
 *****                                                        *****
 *****  Name: easyweb.c                                       *****
 *****  Ver.: 1.0                                             *****
 *****  Date: 07/05/2001                                      *****
 *****  Auth: Andreas Dannenberg                              *****
 *****        HTWK Leipzig                                    *****
 *****        university of applied sciences                  *****
 *****        Germany                                         *****
 *****  Func: implements a dynamic HTTP-server by using       *****
 *****        the easyWEB-API                                 *****
 *****                                                        *****
 ******************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "lpc43xx.H"                    /* lpc43xx definitions                */
#include "lpc43xx_scu.h"
#include "lpc43xx_adc.h"
#include "lpc43xx_cgu.h"
#include "lpc43xx_gpio.h"
#include "lpc43xx_scu.h"
#include "lpc43xx_libcfg.h"
#include "EMAC.h"         // Keil: *.c -> *.h    // ethernet packet driver

#define extern            // Keil: Line added for modular project management

#include "easyweb.h"
#include "tcpip.h"        // Keil: *.c -> *.h    // easyWEB TCP/IP stack
#include "webpage.h"                             // webside for our HTTP server (HTML)

/* Example group ----------------------------------------------------------- */
/** @defgroup EMAC_Easy_Web	Easy_Web
 * @ingroup EMAC_Examples
 * @{
 */
#define ADC_CLK             200000		/* set to 1Mhz */
#if defined(HITEX_BOARD)
#define _ADC_CHANNEL		ADC_CHANNEL_2
#elif defined(KEIL_BOARD)
#define _ADC_CHANNEL		ADC_CHANNEL_1
#endif

unsigned int res = 0;
unsigned int pagecounter = 100;
unsigned int adcValue    =   0;

#define IRC_OSC     (12000000UL)        /* Internal RC oscillator frequency   */
uint32_t SystemFrequency = IRC_OSC * 6; /*!< System Clock Frequency (Core Clock)  */
uint32_t AHBSystemFrequency = IRC_OSC * 6;


extern void TCPClockHandler(void);

volatile uint32_t TimeTick  = 0;

/* SysTick interrupt happens every 10 ms */
void SysTick_Handler (void) {
  TimeTick++;
  if (TimeTick >= 20) {
    TimeTick = 0;
//    LPC_GPIO1->FIOPIN ^= 1 << 28;
    TCPClockHandler();
  }
}

uint32_t ADC_Read(void){
	uint32_t adc_value;
	// Start conversion
	ADC_StartCmd(LPC_ADC1,ADC_START_NOW);
	//Wait conversion complete
	while (!(ADC_ChannelGetStatus(LPC_ADC1,_ADC_CHANNEL,ADC_DATA_DONE)));
	adc_value = ADC_ChannelGetData(LPC_ADC1,_ADC_CHANNEL);
	return adc_value;
}

void init_memory(void) {
   int i;
   char * pMem = (char *)0x20008000;

   for (i=0;i<10000;i++) {
	  (*pMem++) = 0x00;

   }
}

int main(void)
{
	SystemInit();
	CGU_Init();
	CGU_EnableEntity(CGU_CLKSRC_32KHZ_OSC, ENABLE);			// Enable 32 kHz & 1 kHz on osc32k

	CGU_EnableEntity(CGU_CLKSRC_ENET_RX_CLK, ENABLE);
	CGU_EnableEntity(CGU_CLKSRC_ENET_TX_CLK, ENABLE);
#if MII
	CGU_EntityConnect(CGU_CLKSRC_ENET_TX_CLK, CGU_BASE_PHY_TX);
	CGU_EntityConnect(CGU_CLKSRC_ENET_RX_CLK, CGU_BASE_PHY_RX);
#else
	CGU_EntityConnect(CGU_CLKSRC_ENET_TX_CLK, CGU_BASE_PHY_TX);
	CGU_EntityConnect(CGU_CLKSRC_ENET_TX_CLK, CGU_BASE_PHY_RX);
//	CGU_SetDIV(CGU_CLKSRC_IDIVA, 2);
//	CGU_EntityConnect(CGU_CLKSRC_ENET_TX_CLK, CGU_CLKSRC_IDIVA);
//	CGU_EntityConnect(CGU_CLKSRC_IDIVA, CGU_BASE_PHY_TX);
//	CGU_EntityConnect(CGU_CLKSRC_IDIVA, CGU_BASE_PHY_RX);
#endif

#if defined(HITEX_BOARD)
	/* Initialize ADC 1 Channel 2 (PF_9) */
	scu_pinmux(0xF ,9 , MD_PLN, FUNC4);
	GPIO_SetDir((1<<7),(1<<23), 0);
	LPC_SCU->SFSPF_9 &= ~(1<<6);
	LPC_SCU->SFSPF_9 |= (1<<4);
	LPC_SCU->SFSPF_9 &= ~(1<<3);
	LPC_SCU->ENAIO1 |= (1<<2);
    ADC_Init( LPC_ADC1, ADC_CLK, 10);
	ADC_ChannelCmd(LPC_ADC1,_ADC_CHANNEL,ENABLE);
#elif defined(KEIL_BOARD)
	/* Initialize ADC 1 Channel 1 */
	LPC_SCU->ENAIO1 |= (1<<1);
    ADC_Init( LPC_ADC1, ADC_CLK, 10);
	ADC_ChannelCmd(LPC_ADC1,_ADC_CHANNEL,ENABLE);
#endif


#if 0
 while(1){
	res = ADC_Read();	/* Polling */

 }
#endif

   SysTick_Config(CGU_GetPCLKFrequency(CGU_PERIPHERAL_M4CORE)/100);               /* Generate interrupt every 10 ms */

  TCPLowLevelInit();

  HTTPStatus = 0;                                // clear HTTP-server's flag register

  TCPLocalPort = TCP_PORT_HTTP;                  // set port we want to listen to

  while (1)                                      // repeat forever
  {
    if (!(SocketStatus & SOCK_ACTIVE)) TCPPassiveOpen();   // listen for incoming TCP-connection
    DoNetworkStuff();                                      // handle network and easyWEB-stack
                                                           // events
    HTTPServer();
  }
}

// This function implements a very simple dynamic HTTP-server.
// It waits until connected, then sends a HTTP-header and the
// HTML-code stored in memory. Before sending, it replaces
// some special strings with dynamic values.
// NOTE: For strings crossing page boundaries, replacing will
// not work. In this case, simply add some extra lines
// (e.g. CR and LFs) to the HTML-code.

void HTTPServer(void)
{
  if (SocketStatus & SOCK_CONNECTED)             // check if somebody has connected to our TCP
  {
    if (SocketStatus & SOCK_DATA_AVAILABLE)      // check if remote TCP sent data
      TCPReleaseRxBuffer();                      // and throw it away

    if (SocketStatus & SOCK_TX_BUF_RELEASED)     // check if buffer is free for TX
    {
      if (!(HTTPStatus & HTTP_SEND_PAGE))        // init byte-counter and pointer to webside
      {                                          // if called the 1st time
        HTTPBytesToSend = sizeof(WebSide) - 1;   // get HTML length, ignore trailing zero
        PWebSide = (unsigned char *)WebSide;     // pointer to HTML-code
      }

      if (HTTPBytesToSend > MAX_TCP_TX_DATA_SIZE)     // transmit a segment of MAX_SIZE
      {
        if (!(HTTPStatus & HTTP_SEND_PAGE))           // 1st time, include HTTP-header
        {
          memcpy(TCP_TX_BUF, GetResponse, sizeof(GetResponse) - 1);
          memcpy(TCP_TX_BUF + sizeof(GetResponse) - 1, PWebSide, MAX_TCP_TX_DATA_SIZE - sizeof(GetResponse) + 1);
          HTTPBytesToSend -= MAX_TCP_TX_DATA_SIZE - sizeof(GetResponse) + 1;
          PWebSide += MAX_TCP_TX_DATA_SIZE - sizeof(GetResponse) + 1;
        }
        else
        {
          memcpy(TCP_TX_BUF, PWebSide, MAX_TCP_TX_DATA_SIZE);
          HTTPBytesToSend -= MAX_TCP_TX_DATA_SIZE;
          PWebSide += MAX_TCP_TX_DATA_SIZE;
        }

        TCPTxDataCount = MAX_TCP_TX_DATA_SIZE;   // bytes to xfer
        InsertDynamicValues();                   // exchange some strings...
        TCPTransmitTxBuffer();                   // xfer buffer
      }
      else if (HTTPBytesToSend)                  // transmit leftover bytes
      {
        memcpy(TCP_TX_BUF, PWebSide, HTTPBytesToSend);
        TCPTxDataCount = HTTPBytesToSend;        // bytes to xfer
        InsertDynamicValues();                   // exchange some strings...
        TCPTransmitTxBuffer();                   // send last segment
        TCPClose();                              // and close connection
        HTTPBytesToSend = 0;                     // all data sent
      }

      HTTPStatus |= HTTP_SEND_PAGE;              // ok, 1st loop executed
    }
  }
  else
    HTTPStatus &= ~HTTP_SEND_PAGE;               // reset help-flag if not connected
}

// samples and returns the AD-converter value of channel 5

unsigned int GetAD7Val(void)			   //Finish
{
// Keil: function replaced to handle lpc43xx A/D converter.
  unsigned int val;
  val = ADC_Read();
  return(val);                                   /* result of A/D process */
}

// searches the TX-buffer for special strings and replaces them
// with dynamic values (AD-converter results)

void InsertDynamicValues(void)
{
  unsigned char *Key;
           char NewKey[5];
  unsigned int i;

  if (TCPTxDataCount < 4) return;                     // there can't be any special string

  Key = TCP_TX_BUF;

  for (i = 0; i < (TCPTxDataCount - 3); i++)
  {
    if (*Key == 'A')
     if (*(Key + 1) == 'D')
       if (*(Key + 3) == '%')
         switch (*(Key + 2))
         {
           case '8' :                                 // "AD8%"?
           {
             adcValue = GetAD7Val();                  // get AD value
             sprintf(NewKey, "0x%03X", adcValue);       // insert AD converter value
             memcpy(Key, NewKey, 5);
             break;
           }
           case '7' :                                 // "AD7%"?
           {
             sprintf(NewKey, "%3u", (adcValue*100)/1023);     // copy saved value from previous read
             memcpy(Key, NewKey, 3);
             break;
           }
		   case '1' :                                 // "AD1%"?
           {
 			 sprintf(NewKey, "%3u", ++pagecounter);    // increment and insert page counter
             memcpy(Key, NewKey, 3);
			 *(Key + 3) = ' ';
             break;
           }
         }
    Key++;
  }
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

/**
 * @}
 */
