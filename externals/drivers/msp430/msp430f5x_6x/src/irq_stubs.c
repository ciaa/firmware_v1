#include "msp430.h"




/**
Enble ALL the individual IRQ signals from the vector specified
The MSP430 has within each IRQ vector , many signals available.
However it doesn have a way of disabling all of them just changing one flag.
SEE SLAS590 (TABLE 4)
*/
void MSP430_EnableIRQ(unsigned int irQ_number)
{
	switch(irQ_number)
	{
		case 0: // => "RTC",
			/* RTCRDYIE, RTCTEVIE, RTCAIE, RT0PSIE, RT1PSIE */

			RTCCTL0   |= RTCTEVIE|RTCAIE|RTCRDYIE;
			RTCPS0CTL |= RT0PSIE;
			RTCPS1CTL |= RT1PSIE;
			break;

		case 1 : //=> "PORT2",
			/* P2IE.0 to P2IE.7*/
			P2IE = 0xFF;
			break;

		case 2 : //=> "TIMER2_A1",
			/* TA2CCR1 CCIE1 to TA2CCR2 CCIE2, TA2IE*/
			TA2CTL   |= TAIE;
			TA2CCTL1 |= CCIE;
			TA2CCTL2 |= CCIE;
			break;

		case 3 : //=> "TIMER2_A0",
			/* TA2CCR0 CCIE0 */
			TA2CCTL0 |= CCIE;
			break;

		case 4 : //=> "USCI_B1",
			/* UCB1RXIE, UCB1TXIE */
			UCB1IE |= UCRXIE|UCTXIE;
			break;

		case 5 : //=> "USCI_A1",
			/* UCA1RXIE, UCA1TXIE */
			UCA1IE |= UCRXIE|UCTXIE;
			break;

		case 6 : //=> "PORT1",
			/* P1IE.0 to P1IE.7  */
			P1IE = 0xFF;
			break;

		case 7 : //=> "TIMER1_A1",
			/*  TA1CCR1 CCIE1 to TA1CCR2 CCIE2 TA1IE*/
			TA1CTL |= TAIE;
			TA1CCTL1 |= CCIE;
			TA1CCTL2 |= CCIE;
			break;

		case 8 : //=> "TIMER1_A0",
			/* TA1CCR0 CCIE0 */
			TA1CCTL0 |= CCIE;
			break;

		case 9 : //=> "DMA",
			/* DMA0IE, DMA1IE, DMA2IE */
			DMA0CTL |=  DMAIE;
			DMA1CTL |=  DMAIE;
			DMA2CTL |=  DMAIE;
			break;

		case 10: // => "USB_UBM",
			/* USB interrupts */
			USBPWRCTL |= VBOFFIE|VBONIE|VUOVLIE;
			USBPLLIR |= USBOORIE|USBLOSIE|USBOOLIE; 
			USBIEPCNF_0 |=USBIIE;
			USBOEPCNF_0 |=USBIIE;
			USBIEPIE = 0XFF;
			USBOEPIE = 0XFF;
			USBMAINT |=UTIE;
			USBIE |= RSTRIE|SUSRIE|RESRIE|SETUPIE|STPOWIE;

			USBIEPCNF_0 |= USBIIE;
			USBOEPCNF_0 |= USBIIE;

			USBIEPCNF_1 |= USBIIE;
			USBOEPCNF_1 |= USBIIE;
			USBIEPCNF_2 |= USBIIE;
			USBOEPCNF_2 |= USBIIE;
			USBIEPCNF_3 |= USBIIE;
			USBOEPCNF_3 |= USBIIE;
			USBIEPCNF_4 |= USBIIE;
			USBOEPCNF_4 |= USBIIE;
			USBIEPCNF_5 |= USBIIE;
			USBOEPCNF_5 |= USBIIE;
         USBIEPCNF_6 |= USBIIE;
			USBOEPCNF_6 |= USBIIE;
         USBIEPCNF_7 |= USBIIE;
			USBOEPCNF_7 |= USBIIE;
			break;

		case 11: // => "TIMER0_A1",
			/* TA0CCR1 CCIE1 to TA0CCR4 CCIE4 TA0IE */
			TA0CTL |= TAIE;
			TA0CCTL1 |= CCIE;
			TA0CCTL2 |= CCIE;
			TA0CCTL3 |= CCIE;
			TA0CCTL4 |= CCIE;
			break;

		case 12: // => "TIMER0_A0",
			/* TA0CCR0 CCIE0 */
			TA0CCTL0 |= CCIE;
			break;

		case 13: // => "ADC12",
			/* ADC12IE0 to ADC12IE15 */
         ADC12CTL0 |= ADC12OVIE|ADC12TOVIE;
         ADC12IE = 0Xffff;
			break;

		case 14: // => "USCI_B0",
			/* UCB0RXIE, UCB0TXIE  */
			UCB0IE |= UCRXIE|UCTXIE;
			break;

		case 15: // => "USCI_A0",
			/* UCA0RXIE, UCA0TXIE */
			UCA0IE |= UCRXIE|UCTXIE;
			break;

		case 16: // => "WDT",
			/* WDTIE */
			break;

		case 17: // => "TIMER0_B1",
			/*	TB0CCR1 CCIE1 to TB0CCR6 CCIE6 TB0IE */
			TB0CTL |= TAIE;
			TB0CCTL1 |= CCIE;
			TB0CCTL2 |= CCIE;
			TB0CCTL3 |= CCIE;
			TB0CCTL4 |= CCIE;
			TB0CCTL5 |= CCIE;
			TB0CCTL6 |= CCIE;
			break;

		case 18: // => "TIMER0_B0",
			/* TB0CCR0 CCIE0 */
			TB0CCTL0 |= CCIE;
			break;

		case 19: // => "COMP_B",
			/* Comparator B interrupt flags */
			break;

		case 20: // => "UNMI",
			break;

		case 21: // => "SYSNMI",
			/* NMIIE, OFIE, ACCVIE, BUSIE SVMLIE, SVMHIE, DLYLIE, DLYHIE, VLRLIE, VLRHIE, VMAIE, JMBNIE, JMBOUTIE */
			break;

		case 22: // => "RESET"
			/* WDTIE, KEYV */
			break;
	}
}

/*
*/
void MSP430_DisableIRQ(unsigned int irQ_number)
{
   switch(irQ_number)
   {
      case 0: // => "RTC",
         /* RTCRDYIE, RTCTEVIE, RTCAIE, RT0PSIE, RT1PSIE */

         RTCCTL0   &=~( RTCTEVIE|RTCAIE|RTCRDYIE);
         RTCPS0CTL &=~ RT0PSIE;
         RTCPS1CTL &=~ RT1PSIE;
         break;

      case 1 : //=> "PORT2",
         /* P2IE.0 to P2IE.7*/
         P2IE = 0xFF;
         break;

      case 2 : //=> "TIMER2_A1",
         /* TA2CCR1 CCIE1 to TA2CCR2 CCIE2, TA2IE*/
         TA2CTL   &=~ TAIE;
         TA2CCTL1 &=~ CCIE;
         TA2CCTL2 &=~ CCIE;
         break;

      case 3 : //=> "TIMER2_A0",
         /* TA2CCR0 CCIE0 */
         TA2CCTL0 &=~ CCIE;
         break;

      case 4 : //=> "USCI_B1",
         /* UCB1RXIE, UCB1TXIE */
         UCB1IE &=~ (UCRXIE|UCTXIE);
         break;

      case 5 : //=> "USCI_A1",
         /* UCA1RXIE, UCA1TXIE */
         UCA1IE &=~ (UCRXIE|UCTXIE);
         break;

      case 6 : //=> "PORT1",
         /* P1IE.0 to P1IE.7  */
         P1IE = 0x00;
         break;

      case 7 : //=> "TIMER1_A1",
         /*  TA1CCR1 CCIE1 to TA1CCR2 CCIE2 TA1IE*/
         TA1CTL   &=~ TAIE;
         TA1CCTL1 &=~ CCIE;
         TA1CCTL2 &=~ CCIE;
         break;

      case 8 : //=> "TIMER1_A0",
         /* TA1CCR0 CCIE0 */
         TA1CCTL0 &=~ CCIE;
         break;

      case 9 : //=> "DMA",
         /* DMA0IE, DMA1IE, DMA2IE */
         DMA0CTL &=~  DMAIE;
         DMA1CTL &=~  DMAIE;
         DMA2CTL &=~  DMAIE;
         break;

      case 10: // => "USB_UBM",
         /* USB interrupts */
         USBPWRCTL   &=~ (VBOFFIE|VBONIE|VUOVLIE);
         USBPLLIR    &=~ (USBOORIE|USBLOSIE|USBOOLIE);
         USBIEPCNF_0 &=~ USBIIE;
         USBOEPCNF_0 &=~ USBIIE;
         USBIEPIE = 0XFF;
         USBOEPIE = 0XFF;
         USBMAINT    &=~ UTIE;
         USBIE       &=~ (RSTRIE|SUSRIE|RESRIE|SETUPIE|STPOWIE);


         USBIEPCNF_0 &=~ USBIIE;
         USBOEPCNF_0 &=~ USBIIE;

         USBIEPCNF_1 &=~ USBIIE;
         USBOEPCNF_1 &=~ USBIIE;
         USBIEPCNF_2 &=~ USBIIE;
         USBOEPCNF_2 &=~ USBIIE;
         USBIEPCNF_3 &=~ USBIIE;
         USBOEPCNF_3 &=~ USBIIE;
         USBIEPCNF_4 &=~ USBIIE;
         USBOEPCNF_4 &=~ USBIIE;
         USBIEPCNF_5 &=~ USBIIE;
         USBOEPCNF_5 &=~ USBIIE;
         USBIEPCNF_6 &=~ USBIIE;
         USBOEPCNF_6 &=~ USBIIE;
         USBIEPCNF_7 &=~ USBIIE;
         USBOEPCNF_7 &=~ USBIIE;
         break;

      case 11: // => "TIMER0_A1",
         /* TA0CCR1 CCIE1 to TA0CCR4 CCIE4 TA0IE */
         TA0CTL   &=~ TAIE;
         TA0CCTL1 &=~ CCIE;
         TA0CCTL2 &=~ CCIE;
         TA0CCTL3 &=~ CCIE;
         TA0CCTL4 &=~ CCIE;
         break;

      case 12: // => "TIMER0_A0",
         /* TA0CCR0 CCIE0 */
         TA0CCTL0 &=~ CCIE;
         break;

      case 13: // => "ADC12",
         /* ADC12IE0 to ADC12IE15 */
         ADC12CTL0 &=~ (ADC12OVIE|ADC12TOVIE);
         ADC12IE = 0x0000;
         break;

      case 14: // => "USCI_B0",
         /* UCB0RXIE, UCB0TXIE  */
         UCB0IE &=~ (UCRXIE|UCTXIE);
         break;

      case 15: // => "USCI_A0",
         /* UCA0RXIE, UCA0TXIE */
         UCA0IE  &=~ (UCRXIE|UCTXIE);
         break;

      case 16: // => "WDT",
         /* WDTIE */
         break;

      case 17: // => "TIMER0_B1",
         /*	TB0CCR1 CCIE1 to TB0CCR6 CCIE6 TB0IE */
         TB0CTL   &=~ TAIE;
         TB0CCTL1 &=~ CCIE;
         TB0CCTL2 &=~ CCIE;
         TB0CCTL3 &=~ CCIE;
         TB0CCTL4 &=~ CCIE;
         TB0CCTL5 &=~ CCIE;
         TB0CCTL6 &=~ CCIE;
         break;

      case 18: // => "TIMER0_B0",
         /* TB0CCR0 CCIE0 */
         TB0CCTL0 &=~ CCIE;
         break;

      case 19: // => "COMP_B",
         /* Comparator B interrupt flags */
         break;

      case 20: // => "UNMI",
         break;

      case 21: // => "SYSNMI",
         /* NMIIE, OFIE, ACCVIE, BUSIE SVMLIE, SVMHIE, DLYLIE, DLYHIE, VLRLIE, VLRHIE, VMAIE, JMBNIE, JMBOUTIE */
         break;

      case 22: // => "RESET"
         /* WDTIE, KEYV */
         break;
     }
}


