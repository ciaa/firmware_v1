/* Copyright 2016, Mariano cerdeiro
 * All rights reserved.
 *
 * This file is part of CIAA Firmware.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 */

/** \brief CIAA Eth Driver for LPC4337
 **
 ** Implements the Eth Driver for LPC4337
 **
 **/

/** \addtogroup CIAA_Firmware CIAA Firmware
 ** @{ */
/** \addtogroup Drivers CIAA Drivers
 ** @{ */
/** \addtogroup ETH Eth Drivers
 ** @{ */

/*==================[inclusions]=============================================*/
#ifdef CIAA_CFG_NET_IP
#include "ciaaDriverEth.h"
#include "chip.h"

/** from LWIP */
#include "lpc_phy.h"
#include "arch/lpc18xx_43xx_emac.h"
#include "arch/lpc_arch.h"
#include "netif/etharp.h"
#include "lwip/netif.h"
#include "lwip/init.h"
#include "lwip/timers.h"


/*==================[macros and definitions]=================================*/

/*==================[internal data declaration]==============================*/
/* NETIF data */
static struct netif lpc_netif;

static uint32_t physts;
static ip_addr_t ipaddr, netmask, gw;

static const PINMUX_GRP_T pinmuxing[] = {
   /* RMII pin group */
   {0x7, 7, MD_EHS | MD_PLN | MD_EZI | MD_ZI |FUNC6},
   {0x1 ,17 , MD_EHS | MD_PLN | MD_EZI | MD_ZI| FUNC3},  /* ENET_MDIO: P1_17 -> FUNC3 */
   {0x1 ,18 , MD_EHS | MD_PLN | MD_EZI | MD_ZI| FUNC3},  /* ENET_TXD0: P1_18 -> FUNC3 */
   {0x1 ,20 , MD_EHS | MD_PLN | MD_EZI | MD_ZI| FUNC3},  /* ENET_TXD1: P1_20 -> FUNC3 */
   {0x1 ,19 , MD_EHS | MD_PLN | MD_EZI | MD_ZI| FUNC0},  /* ENET_REF: P1_19 -> FUNC0 (default) */
   {0x0 ,1 , MD_EHS | MD_PLN | MD_EZI | MD_ZI| FUNC6},   /* ENET_TX_EN: P0_1 -> FUNC6 */
   {0x1 ,15 ,MD_EHS | MD_PLN | MD_EZI | MD_ZI| FUNC3},   /* ENET_RXD0: P1_15 -> FUNC3 */
   {0x0 ,0 , MD_EHS | MD_PLN | MD_EZI | MD_ZI| FUNC2},   /* ENET_RXD1: P0_0 -> FUNC2 */
   {0x1 ,16 ,MD_EHS | MD_PLN | MD_EZI | MD_ZI| FUNC7}
};

/*==================[internal functions declaration]=========================*/
/* Sets up system hardware */
static void prvSetupHardware(void)
{
   /* LED0 is used for the link status, on = PHY cable detected */
   SystemCoreClockUpdate();

   /* Setup system level pin muxing */
   Chip_SCU_SetPinMuxing(pinmuxing, sizeof(pinmuxing) / sizeof(PINMUX_GRP_T));

   Chip_ENET_RMIIEnable(LPC_ETHERNET);

   /* Setup a 1mS sysTick for the primary time base */
   lwipSysTick_Enable(1);
}
/*==================[internal data definition]===============================*/

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/

/*==================[external functions definition]==========================*/

void ciaaDriverEth_init(void)
{
   /* init Ethernet Hardware */
   prvSetupHardware();

   /* Initialize LWIP */
   lwip_init();

   /* Static IP assignment */
#if LWIP_DHCP
   IP4_ADDR(&gw, 0, 0, 0, 0);
   IP4_ADDR(&ipaddr, 0, 0, 0, 0);
   IP4_ADDR(&netmask, 0, 0, 0, 0);
#else
   IP4_ADDR(&gw, 10, 0, 0, 1);
   IP4_ADDR(&ipaddr, 10, 0, 0, 123);
   IP4_ADDR(&netmask, 255, 255, 255, 0);
#endif

   /* Add netif interface for lpc17xx_8x */
   netif_add(&lpc_netif, &ipaddr, &netmask, &gw, NULL, lpc_enetif_init,
           ethernet_input);
   netif_set_default(&lpc_netif);
   netif_set_up(&lpc_netif);

#if LWIP_DHCP
   dhcp_start(&lpc_netif);
#endif
}

void ciaaDriverEth_mainFunction(void)
{
   /* Handle packets as part of this loop, not in the IRQ handler */
   lpc_enetif_input(&lpc_netif);

   /* lpc_rx_queue will re-qeueu receive buffers. This normally occurs
      automatically, but in systems were memory is constrained, pbufs
      may not always be able to get allocated, so this function can be
      optionally enabled to re-queue receive buffers. */
#if 0
   while (lpc_rx_queue(&lpc_netif)) {}
#endif

   /* Free TX buffers that are done sending */
   lpc_tx_reclaim(&lpc_netif);

   /* LWIP timers - ARP, DHCP, TCP, etc. */
   sys_check_timeouts();

   /* Call the PHY status update state machine once in a while
      to keep the link status up-to-date */
   physts = lpcPHYStsPoll();

   /* Only check for connection state when the PHY status has changed */
   if (physts & PHY_LINK_CHANGED) {
      if (physts & PHY_LINK_CONNECTED) {
         /* Set interface speed and duplex */
         if (physts & PHY_LINK_SPEED100) {
            Chip_ENET_SetSpeed(LPC_ETHERNET, 1);
            NETIF_INIT_SNMP(&lpc_netif, snmp_ifType_ethernet_csmacd, 100000000);
         }
         else {
            Chip_ENET_SetSpeed(LPC_ETHERNET, 0);
            NETIF_INIT_SNMP(&lpc_netif, snmp_ifType_ethernet_csmacd, 10000000);
         }
         if (physts & PHY_LINK_FULLDUPLX) {
            Chip_ENET_SetDuplex(LPC_ETHERNET, true);
         }
         else {
            Chip_ENET_SetDuplex(LPC_ETHERNET, false);
         }

         netif_set_link_up(&lpc_netif);
      }
      else {
         netif_set_link_down(&lpc_netif);
      }
   }

}
/*==================[interrupt handlers]=====================================*/

#else /* #ifdef CIAA_CFG_NET_IP */
/* some C compilers may have problems by compiling an empty file, this is not
 * allowed in the standard */
typedef unsigned char dummy;
#endif /* #ifdef CIAA_CFG_NET_IP */

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/

