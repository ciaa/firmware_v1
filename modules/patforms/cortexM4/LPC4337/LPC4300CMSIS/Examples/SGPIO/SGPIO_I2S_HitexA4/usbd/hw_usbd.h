/***********************************************************************
* $Id: hw_usbd.h 8242 2011-10-11 15:15:25Z nxp28536 $
*
* Project: IP9208 USB-OTG device controller definitions
*
* Description:
*     This file contains the structure definitions and manifest
*     constants for the IP9208 component:
*         Universal Serial Bus - On The Go (USB-OTG)
*
***********************************************************************
*   Copyright(C) 2011, NXP Semiconductor
*   All rights reserved.
*
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
**********************************************************************/

#ifndef HW_USBD_IP9208_H
#define HW_USBD_IP9208_H

#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif

#ifndef _BIT
/* _BIT(n) sets the bit at position "n"
 * _BIT(n) is intended to be used in "OR" and "AND" expressions:
 * e.g., "(_BIT(3) | _BIT(7))".
 */
/* Set bit macro */
#define _BIT(n)	(((uint32_t)(1ul)) << (n))

/* _SBF(f,v) sets the bit field starting at position "f" to value "v".
 * _SBF(f,v) is intended to be used in "OR" and "AND" expressions:
 * e.g., "((_SBF(5,7) | _SBF(12,0xF)) & 0xFFFF)"
 */
/* Set bit field macro */
#define _SBF(f,v) (((uint32_t)(v)) << (f))
#endif /*_BIT*/

/***********************************************************************
* USB Register Structures
**********************************************************************/

/* USB Module Register Structure */
typedef volatile struct
{
	volatile uint8_t RESERVED0[0xFF];
  /*Capability registers*/
  volatile uint8_t  caplength;     /*Capability register length*/
  volatile uint8_t  _otg1[1];
  volatile uint16_t hciversion;    /*Host interface version number*/
  volatile uint32_t hcsparams;     /*Host controller structural parameters*/
  volatile uint32_t hccparams;     /*Host controller capability parameters*/
  volatile uint32_t _otg2[5];      /*10Ch-11Fh 20 Reserved */
  volatile uint16_t dciversion;    /*Device interface version number*/
  volatile uint16_t _otg3[1];
  volatile uint32_t dccparams;     /*Device controller capability parameters*/
  volatile uint32_t _otg4a[4];     /*128h-134h 16 Reserved */
  volatile uint32_t usb_up_int;    /*USB interrupt test mode*/
  volatile uint32_t _otg4b[1];     /*13Ch 4 Reserved */

  /* Operational registers */
  volatile uint32_t usbcmd;        /*USB command*/
  volatile uint32_t usbsts;        /*USB status*/
  volatile uint32_t usbintr;       /*USB interrupt enable*/
  volatile uint32_t frindex;       /*USB frame index*/
  volatile uint32_t _otg5[1];
  volatile uint32_t periodiclistbase__deviceaddr;    /*Frame list base address__USB device address*/
  volatile uint32_t asynclistaddr__endpointlistaddr; /*Next asynchronous list address__Address of endpoint list in memory*/
  volatile uint32_t ttctrl;        /*Asynchronous buffer status for embedded TT*/
  volatile uint32_t burstsize;     /*Programmable burst size*/
  volatile uint32_t txfilltuning;  /*Host transmit pre-buffer packet tuning*/
  volatile uint32_t txttfilltuning;/*Host TT transmit pre-buffer packet tuning*/
  volatile uint32_t _otg6[1];
  volatile uint32_t _otg7[2];      /*170h-174h 8 Reserved n/a*/
  volatile uint32_t endptnak;
  volatile uint32_t endptnaken;
  volatile uint32_t configflag;    /*Configured flag register*/
  volatile uint32_t portsc1;       /*Port status/control 1*/
  volatile uint32_t _otg8[7];      /*190h-1A3h 20 Reserved n/a*/
  volatile uint32_t otgsc;         /*OTG status and control*/
  volatile uint32_t usbmode;       /*USB device mode*/
  volatile uint32_t endptsetupstat;/*Endpoint setup status*/
  volatile uint32_t endptprime;    /*Endpoint initialization*/
  volatile uint32_t endptflush;    /*Endpoint de-initialization*/
  volatile uint32_t endptstatus;   /*Endpoint status*/
  volatile uint32_t endptcomplete; /*Endpoint complete*/
  volatile uint32_t endptctrl[4];    /*Endpoint control 0 - 3*/

} USB_OTG_REGS_T;

/* dTD Transfer Description */
typedef volatile struct
{
  volatile uint32_t next_dTD;
  volatile uint32_t total_bytes ;
  volatile uint32_t buffer0;
  volatile uint32_t buffer1;
  volatile uint32_t buffer2;
  volatile uint32_t buffer3;
  volatile uint32_t buffer4;
  volatile uint32_t reserved;
}  DTD_T;

/* dQH  Queue Head */
typedef volatile struct
{
  volatile uint32_t cap;
  volatile uint32_t curr_dTD;
  volatile uint32_t next_dTD;
  volatile uint32_t total_bytes;
  volatile uint32_t buffer0;
  volatile uint32_t buffer1;
  volatile uint32_t buffer2;
  volatile uint32_t buffer3;
  volatile uint32_t buffer4;
  volatile uint32_t reserved;
  volatile uint32_t setup[2];
  volatile uint32_t gap[4];
}  DQH_T;


/* bit defines for USBCMD register */
#define USBCMD_RS     _BIT(0)
#define USBCMD_RST    _BIT(1)
#define USBCMD_ATDTW  _BIT(12)
#define USBCMD_SUTW   _BIT(13)

/* bit defines for USBSTS register */
#define USBSTS_UI   _BIT(0)
#define USBSTS_UEI  _BIT(1)
#define USBSTS_PCI  _BIT(2)
#define USBSTS_URI  _BIT(6)
#define USBSTS_SRI  _BIT(7)
#define USBSTS_SLI  _BIT(8)
#define USBSTS_NAKI _BIT(16)

/* bit defines for DEVICEADDR register */
#define USBDEV_ADDR_AD  _BIT(24)
#define USBDEV_ADDR(n)  _SBF(25, ((n) & 0x7F))

/* bit defines for PRTSC1 register */
#define USBPRTS_CCS     _BIT(0)
#define USBPRTS_PE      _BIT(2)
#define USBPRTS_FPR     _BIT(6)
#define USBPRTS_SUSP    _BIT(7)
#define USBPRTS_PR      _BIT(8)
#define USBPRTS_HSP     _BIT(9)
#define USBPRTS_PLPSCD  _BIT(23)
#define USBPRTS_PFSC    _BIT(24)

/* bit defines for USBMODE register */
#define USBMODE_CM_IDLE _SBF(0, 0x0)
#define USBMODE_CM_DEV  _SBF(0, 0x2)
#define USBMODE_CM_HOST _SBF(0, 0x3)
#define USBMODE_SLOM    _BIT(3)
#define USBMODE_SDIS    _BIT(4)

/* bit defines for EP registers*/
#define USB_EP_BITPOS(n) (((n) & 0x80)? (0x10 | ((n) & 0x7)) : ((n) & 0x7))

/* bit defines EPcontrol registers*/
#define EPCTRL_RXS        _BIT(0)
#define EPCTRL_RX_TYPE(n) _SBF(2,((n) & 0x3))
#define EPCTRL_RX_CTL     _SBF(2,0)
#define EPCTRL_RX_ISO     _SBF(2,1)
#define EPCTRL_RX_BLK     _SBF(2,2)
#define EPCTRL_RXI        _BIT(5)
#define EPCTRL_RXR        _BIT(6)
#define EPCTRL_RXE        _BIT(7)
#define EPCTRL_TXS        _BIT(16)
#define EPCTRL_TX_TYPE(n) _SBF(18,((n) & 0x3))
#define EPCTRL_TX_CTL     _SBF(18,0)
#define EPCTRL_TX_ISO     _SBF(18,1)
#define EPCTRL_TX_BLK     _SBF(18,2)
#define EPCTRL_TX_INT     _SBF(18,3)
#define EPCTRL_TXI        _BIT(21)
#define EPCTRL_TXR        _BIT(22)
#define EPCTRL_TXE        _BIT(23)

/* dQH field and bit defines */
/* Temp fixed on max, should be taken out of table */
#define QH_MAX_CTRL_PAYLOAD       0x03ff
#define QH_MAX_PKT_LEN_POS        16
#define QH_MAXP(n)                _SBF(16,((n) & 0x3FF))
#define QH_IOS                    _BIT(15)
#define QH_ZLT                    _BIT(29)
#define QH_MULT(n)               _SBF(30,((n) & 0x3))

/* dTD field and bit defines */
#define TD_NEXT_TERMINATE         _BIT(0)
#define TD_IOC                    _BIT(15)
#define TD_ACTIVE                 _BIT(7)
#define TD_MULTO(n)               _SBF(10,((n) & 0x3))

#ifdef __cplusplus
}
#endif

#endif /* HW_USBD_IP9208_H */
