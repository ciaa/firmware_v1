/*
 * Copyright (c) 2013 - 2014, Freescale Semiconductor, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of Freescale Semiconductor, Inc. nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */


#include <string.h>
#include "lwip/mem.h"
#include "lwip/pbuf.h"
#include <lwip/stats.h>
#include <lwip/snmp.h>
#include "netif/etharp.h"
#include "netif/ppp_oe.h"
#include "ethernetif.h"

#include "fsl_enet_hal.h"
#include "fsl_phy_driver.h"

/* Ethernet Receive buffer descriptors */
#if defined ( __ICCARM__ ) /*!< IAR Compiler */
  #pragma data_alignment=ENET_BD_ALIGNMENT   
#endif
__ALIGN_BEGIN enet_bd_struct_t RxBuffDescrip[ENET_RXBD_NUM] __ALIGN_END;
/* Ethernet Transmit buffer descriptors */
#if defined ( __ICCARM__ ) /*!< IAR Compiler */
  #pragma data_alignment=ENET_BD_ALIGNMENT   
#endif
__ALIGN_BEGIN enet_bd_struct_t TxBuffDescrip[ENET_TXBD_NUM] __ALIGN_END;

/* Ethernet Receive data Buffer */
#if defined ( __ICCARM__ ) /*!< IAR Compiler */
  #pragma data_alignment=ENET_RX_BUFFER_ALIGNMENT   
#endif
__ALIGN_BEGIN uint8_t RxDataBuff[ENET_RXBD_NUM][ENET_RXBuffSizeAlign(ENET_RXBUFF_SIZE)] __ALIGN_END; 
/* Ethernet Transmit data Buffer */
#if defined ( __ICCARM__ ) /*!< IAR Compiler */
  #pragma data_alignment=ENET_TX_BUFFER_ALIGNMENT   
#endif
__ALIGN_BEGIN uint8_t TxDataBuff[ENET_TXBD_NUM][ENET_TXBuffSizeAlign(ENET_TXBUFF_SIZE)] __ALIGN_END; 
#if !ENET_RECEIVE_ALL_INTERRUPT
/* Ethernet Receive data Buffer */
#if defined ( __ICCARM__ ) /*!< IAR Compiler */
  #pragma data_alignment=ENET_RX_BUFFER_ALIGNMENT   
#endif
__ALIGN_BEGIN uint8_t ExtRxDataBuff[ENET_EXTRXBD_NUM][ENET_RXBuffSizeAlign(ENET_RXBUFF_SIZE)] __ALIGN_END; 
#endif

/* Define those to better describe your network interface. */
#define IFNAME0 'e'
#define IFNAME1 'n'

enet_config_rmii_t rmiiCfg = {kEnetCfgRmii, kEnetCfgSpeed100M, kEnetCfgFullDuplex, false, false};
enet_mac_config_t g_enetMacCfg[HW_ENET_INSTANCE_COUNT] = 
{
{
    kEnetMacNormalMode,  /*!< ENET normal mode*/
    kEnetMaxFrameSize,  /*!< ENET receive maximun frame length*/
    kEnetDefaultTruncLen,  /*!< ENET default frame truncation length*/
    kEnetDefaultIpg,       /*!< ENET default transmit inter packet gap*/
    0,                 /*!< ENET Pause duration*/
    NULL,      /*!< ENET mac address*/
    &rmiiCfg,  /*!< ENET rmii interface*/
    kEnetMdioHoldOneClkCycle,
    /*!< enet mac control flag recommended to use enet_mac_control_flag_t
      it is special control for loop mode, sleep mode, crc forward/terminate etc*/
    kEnetRxCrcFwdEnable | kEnetTxCrcBdEnable | kEnetMacEnhancedEnable,
    NULL,     /*!< ENET rxaccelerator config*/
    NULL,     /*!< ENET txaccelerator config*/
    {0, kEnetMinFifoAlmostEmpty, 0, kEnetMinFifoAlmostEmpty}, /*!< ENET Rx FIFO threshold*/
    {0, 0, 0, kEnetMinFifoAlmostEmpty, kEnetDefaultTxFifoAlmostFull}, /*!< ENET Tx FIFO threshold*/
#if FSL_FEATURE_ENET_SUPPORT_PTP
    false,
#endif
},
};
const enet_phy_config_t g_enetPhyCfg[HW_ENET_INSTANCE_COUNT] =
{{false, 0, false }};

#if FSL_FEATURE_ENET_SUPPORT_PTP
enet_mac_ptp_ts_data_t ptpTsRxData[ENET_PTP_RXTS_RING_LEN];
enet_mac_ptp_ts_data_t ptpTsTxData[ENET_PTP_TXTS_RING_LEN];
#endif

enet_dev_if_t enetDevIf[HW_ENET_INSTANCE_COUNT];

#if !ENET_RECEIVE_ALL_INTERRUPT&&!NO_SYS
OSA_TASK_DEFINE(Enet_receive, RECV_TASK_STACK_SIZE);
#endif
#if !ENET_RECEIVE_ALL_INTERRUPT
/*FUNCTION****************************************************************
 *
 * Function Name: ENET_receive
 * Description:  Net Receive interface.
 *
 *END*********************************************************************/
void ENET_receive(task_param_t param)
{
    uint16_t counter = 0;
    uint32_t result;
    event_flags_t flag = 0x1, flagCheck;
    enet_mac_packet_buffer_t packetBuffer[kEnetMaxFrameBdNumbers];

    /* Init the packet buffer*/
    memset(&packetBuffer[0], 0, kEnetMaxFrameBdNumbers * sizeof(enet_mac_packet_buffer_t));
    for(counter = 0; counter < kEnetMaxFrameBdNumbers - 1; counter++)
    {
        packetBuffer[counter].next = &packetBuffer[counter + 1];
    }
	packetBuffer[counter].next = NULL;
    counter = 0;

    /* Check input parameter*/
    if (!param)
    {
        return ;
    }
    enet_dev_if_t * enetIfPtr = (enet_dev_if_t *)param;

#if !USE_RTOS
    osa_status_t status;
    /*receive frame*/
#else  
    while(1)
#endif
    {
        /* Init the packet buffer*/
        for(counter = 0; counter < kEnetMaxFrameBdNumbers; counter++)
        {
            packetBuffer[counter].data = NULL;
        }    
               /* Receive frame*/
        result = ENET_DRV_ReceiveData(enetIfPtr, &packetBuffer[0]);
        if ((result == kStatus_ENET_RxbdEmpty) || (result == kStatus_ENET_InvalidInput))
        {
            OSA_EventWait(&enetIfPtr->enetReceiveSync, flag, false, OSA_WAIT_FOREVER, &flagCheck);
        }

        /* Process with the right packets*/
        if (packetBuffer[0].data != NULL)
        {
           ethernetif_input(enetIfPtr,packetBuffer);
           for(counter = 0; packetBuffer[counter].length != 0 ; counter ++)
           {
              *(uint32_t *)(packetBuffer[counter].data) = 0;
              enet_mac_enqueue_buffer((void **)&enetIfPtr->bdContext.extRxBuffQue, packetBuffer[counter].data);
              packetBuffer[counter].length = 0;
           }		
        }
    }
}
#endif

/**
 * In this function, the hardware should be initialized.
 * Called from ethernetif_init().
 *
 * @param netif the already initialized lwip network interface structure
 *        for this ethernetif
 */
err_t
low_level_init(struct netif *netif)
{
  enet_dev_if_t * enetIfPtr;
  uint32_t result;
  const enet_mac_config_t *macCfgPtr;
  enet_buff_config_t buffCfg;
  uint32_t devNumber = 0;
  osa_status_t osaFlag;
  
  #if LWIP_HAVE_LOOPIF
  devNumber = netif->num - 1;
  #else
  devNumber = netif->num;
  #endif
  /* set MAC hardware address length */
  netif->hwaddr_len = ETHARP_HWADDR_LEN;

  /* set MAC hardware address */
  netif->hwaddr[0] = configMAC_ADDR0;
  netif->hwaddr[1] = configMAC_ADDR1;
  netif->hwaddr[2] = configMAC_ADDR2;
  netif->hwaddr[3] = configMAC_ADDR3;
  netif->hwaddr[4] = configMAC_ADDR4;
  netif->hwaddr[5] = configMAC_ADDR5;


  /* maximum transfer unit */
  netif->mtu = kEnetMaxFrameDateSize;
  
  /* device capabilities */
  /* don't set NETIF_FLAG_ETHARP if this device is not an ethernet one */
  netif->flags = NETIF_FLAG_BROADCAST | NETIF_FLAG_ETHARP | NETIF_FLAG_LINK_UP;
  /* Do whatever else is needed to initialize interface. */ 
 
  if (devNumber >= HW_ENET_INSTANCE_COUNT)
  	{
  	  return kStatus_ENET_InvalidDevice;
    }
  /* Initialize device*/
    enetIfPtr = (enet_dev_if_t *)&enetDevIf[devNumber];
  	netif->state = (void *)enetIfPtr;
    if (HW_ENET_INSTANCE_COUNT - 1 == devNumber)
    {
        enetIfPtr->next = NULL;
    }   

    /* 	Configure strucutre*/
    enetIfPtr->deviceNumber = devNumber;
    g_enetMacCfg[devNumber].macAddr = (uint8_t *)&(netif->hwaddr[0]);
    macCfgPtr = &g_enetMacCfg[devNumber];
#if ENET_RECEIVE_ALL_INTERRUPT   
    enetIfPtr->enetNetifcall = ethernetif_input;
#endif
    /* Create sync signal*/
    OSA_MutexCreate(&enetIfPtr->enetContextSync);	

    memset(&buffCfg, 0,sizeof(enet_buff_config_t));
    buffCfg.rxBdNumber = ENET_RXBD_NUM;
    buffCfg.rxBdPtrAlign = RxBuffDescrip;
    buffCfg.rxBufferAlign = &RxDataBuff[0][0];
    buffCfg.rxBuffSizeAlign = ENET_RXBuffSizeAlign(ENET_RXBUFF_SIZE);
    buffCfg.txBdNumber = ENET_TXBD_NUM;
    buffCfg.txBdPtrAlign = TxBuffDescrip;
    buffCfg.txBufferAlign = &TxDataBuff[0][0];
    buffCfg.txBuffSizeAlign = ENET_TXBuffSizeAlign(ENET_TXBUFF_SIZE);
#if !ENET_RECEIVE_ALL_INTERRUPT
    buffCfg.extRxBuffQue = &ExtRxDataBuff[0][0];
    buffCfg.extRxBuffNum = ENET_EXTRXBD_NUM;
#endif
#if FSL_FEATURE_ENET_SUPPORT_PTP
    buffCfg.ptpTsRxDataPtr = &ptpTsRxData[0];
    buffCfg.ptpTsRxBuffNum = ENET_PTP_RXTS_RING_LEN;
    buffCfg.ptpTsTxDataPtr = &ptpTsTxData[0];
    buffCfg.ptpTsTxBuffNum = ENET_PTP_TXTS_RING_LEN;
#endif
     /* Initialize ENET device*/
    result = ENET_DRV_Init(enetIfPtr, macCfgPtr, &buffCfg);
    if (result == kStatus_ENET_Success)
    {
        /* Initialize PHY*/ 
        if(g_enetPhyCfg[devNumber].isAutodiscoverEnabled)
        {
            uint32_t phyAddr;
            result = PHY_DRV_Autodiscover(devNumber, &phyAddr);
            if(result != kStatus_ENET_Success)
            return result;
            enetIfPtr->phyAddr = phyAddr;
        }
        else
        { 
    	    enetIfPtr->phyAddr = g_enetPhyCfg[devNumber].phyAddr;
        }

        PHY_DRV_Init(devNumber, enetIfPtr->phyAddr, g_enetPhyCfg[devNumber].isLoopEnabled);
        enetIfPtr->isInitialized = true;
#if !ENET_RECEIVE_ALL_INTERRUPT
    osaFlag = OSA_EventCreate(&enetIfPtr->enetReceiveSync, kEventAutoClear);
    if(osaFlag != kStatus_OSA_Success)
    {
        return osaFlag;
    }
#if USE_RTOS
    /* Create receive task*/
    osaFlag = OSA_TaskCreate(ENET_receive, "receive", RECV_TASK_STACK_SIZE, Enet_receive_stack, ENET_RECV_TASK_PRIO, (task_param_t)enetIfPtr, false, &Enet_receive_task_handler);
    if(osaFlag != kStatus_OSA_Success)
    {
        return osaFlag;
    }
#endif
#endif
        return ENET_OK;
    }
    else
    {
        ENET_DRV_Deinit(enetIfPtr);
        OSA_MutexDestroy(&enetIfPtr->enetContextSync);
#if !ENET_RECEIVE_ALL_INTERRUPT
#if USE_RTOS
        OSA_TaskDestroy(Enet_receive_task_handler);
#endif
        OSA_EventDestroy(&enetIfPtr->enetReceiveSync);
#endif
        return ENET_ERROR;
    }
}

/**
 * This function should do the actual transmission of the packet. The packet is
 * contained in the pbuf that is passed to the function. This pbuf
 * might be chained.
 *
 * @param netif the lwip network interface structure for this ethernetif
 * @param p the MAC packet to send (e.g. IP packet including MAC addresses and type)
 * @return ERR_OK if the packet could be sent
 *         an err_t value if the packet couldn't be sent
 *
 * @note Returning ERR_MEM here if a DMA queue of your MAC is full can lead to
 *       strange results. You might consider waiting for space in the DMA queue
 *       to become availale since the stack doesn't retry to send a packet
 *       dropped because of memory failure (except for the TCP timers).
 */

static err_t
low_level_output(struct netif *netif, struct pbuf *p)
{
  enet_dev_if_t *enetIfPtr = (enet_dev_if_t *)netif->state;
  struct pbuf *q;
  uint16_t  lenTemp = 0, bdNumUsed = 0, bdNumUsedTmp,frameLen;
  volatile enet_bd_struct_t * curBd;
  uint8_t *frame;
  if ((!enetIfPtr) || (!p))
  {
    return  kStatus_ENET_InvalidInput;
  }
  if (p->tot_len> enetIfPtr->maxFrameSize)
  {
#if ENET_ENABLE_DETAIL_STATS 
    enetIfPtr->stats.statsTxLarge++;
#endif
    return kStatus_ENET_TxLarge;
  }
   /* Get the current transmit data buffer in buffer descriptor */
  curBd = enetIfPtr->bdContext.txBdCurPtr;
  frame = ENET_HAL_GetBuffDescripData(curBd);
  if(p->tot_len%enetIfPtr->bdContext.txBuffSizeAlign !=0)
  	bdNumUsed = p->tot_len/enetIfPtr->bdContext.txBuffSizeAlign + 1;
  else
  	bdNumUsed = p->tot_len/enetIfPtr->bdContext.txBuffSizeAlign;
  bdNumUsedTmp = bdNumUsed;
  frameLen = p->tot_len;
  while(bdNumUsedTmp--)
  {
    lenTemp = 0;
    for(q = p; q != NULL; q = q->next) {
	/* Send the data from the pbuf to the interface, one pbuf at a
	    time. The size of the data in each pbuf is kept in the ->len
	    variable. */
	  if(enetIfPtr->bdContext.txBuffSizeAlign - lenTemp >= q->len)
	  	{
	      memcpy(frame + lenTemp, q->payload, q->len);
          lenTemp += q->len;
	  	}
	  else
	  	{
	  	  memcpy(frame + lenTemp, q->payload, enetIfPtr->bdContext.txBuffSizeAlign - lenTemp); 
		  p->payload =(void *)((uint32_t )q->payload + enetIfPtr->bdContext.txBuffSizeAlign - lenTemp);
		  p->len = q->len + lenTemp - enetIfPtr->bdContext.txBuffSizeAlign;
		  p->tot_len = q->tot_len + lenTemp - enetIfPtr->bdContext.txBuffSizeAlign;
		  p->next = q->next;
		  break;
	  	}
    }
  }

    /* Send packet to the device*/
    return ENET_DRV_SendData(enetIfPtr, frameLen, bdNumUsed);
 
}

/**
 * Should allocate a pbuf and transfer the bytes of the incoming
 * packet from the interface into the pbuf.
 *
 * @param netif the lwip network interface structure for this ethernetif
 * @return a pbuf filled with the received packet (including MAC header)
 *         NULL on memory error
 */
static struct pbuf *
low_level_input(struct netif *netif, enet_mac_packet_buffer_t *packetBuffer)
{
  uint16_t location = 0;
  struct pbuf *head, *temphead, *p, *q;
  head = temphead = p = q = NULL;
  if (packetBuffer->length != 0)
  {
    head = temphead = p = pbuf_alloc(PBUF_RAW, packetBuffer->length, PBUF_POOL);
  }
  else return NULL;
  while(1)
    {
		if (packetBuffer->length != 0)
        {
		  location = 0;
          for(q = p; q != NULL; q = q->next)
		  {
		  memcpy((uint8_t*)q->payload, packetBuffer->data + location, q->len);
		  location = location + q->len;
		  }
		}
		packetBuffer = packetBuffer->next;
		if(packetBuffer != NULL)
		{
          if (packetBuffer->length != 0)
          {
        	p = pbuf_alloc(PBUF_RAW, packetBuffer->length, PBUF_POOL);
			pbuf_chain(temphead,p);
			temphead = p;
          }
		}else break;
     }
  return head;  
}
/**
 * This function should be called when a packet is ready to be read
 * from the interface. It uses the function low_level_input() that
 * should handle the actual reception of bytes from the network
 * interface. Then the type of the received packet is determined and
 * the appropriate input function is called.
 *
 * @param netif the lwip network interface structure for this ethernetif
 */
uint32_t
ethernetif_input(void *enetPtr, enet_mac_packet_buffer_t *packetBuffer)
{

   struct eth_hdr *ethhdr;
   struct pbuf *p;
   struct netif *netif;
  	/* Check input param*/
    if((!enetPtr) || (!packetBuffer)) 
    {
        return kStatus_ENET_InvalidInput;
	}
	for(netif = netif_list; netif != NULL; netif = netif->next)
	{
	    if(netif->state == enetPtr)
		break;
	}

  /* move received packet into a new pbuf */
  p = low_level_input(netif,packetBuffer);
  /* no packet could be read, silently ignore this */
  if (p == NULL) return ENET_ERROR;
  /* points to packet payload, which starts with an Ethernet header */
  ethhdr = p->payload;

  switch (htons(ethhdr->type)) {
  /* IP or ARP packet? */
  case ETHTYPE_IP:
  case ETHTYPE_ARP:
#if PPPOE_SUPPORT
  /* PPPoE packet? */
  case ETHTYPE_PPPOEDISC:
  case ETHTYPE_PPPOE:
#endif /* PPPOE_SUPPORT */
    /* full packet send to tcpip_thread to process */
    if (netif->input(p, netif)!=ERR_OK)
     { LWIP_DEBUGF(NETIF_DEBUG, ("ethernetif_input: IP input error\n"));
       pbuf_free(p);
       p = NULL;
       return ENET_ERROR;
     }
    break;

  default:
    pbuf_free(p);
    p = NULL;
    break;
  }
  return ENET_OK;
}

/**
 * Should be called at the beginning of the program to set up the
 * network interface. It calls the function low_level_init() to do the
 * actual setup of the hardware.
 *
 * This function should be passed as a parameter to netif_add().
 *
 * @param netif the lwip network interface structure for this ethernetif
 * @return ERR_OK if the loopif is initialized
 *         ERR_MEM if private data couldn't be allocated
 *         any other err_t on error
 */
err_t
ethernetif_init(struct netif *netif)
{
  err_t result;
  LWIP_ASSERT("netif != NULL", (netif != NULL));

#if LWIP_NETIF_HOSTNAME
  /* Initialize interface hostname */
  netif->hostname = "lwip";
#endif /* LWIP_NETIF_HOSTNAME */

  /*
   * Initialize the snmp variables and counters inside the struct netif.
   * The last argument should be replaced with your link speed, in units
   * of bits per second.
   */
  NETIF_INIT_SNMP(netif, snmp_ifType_ethernet_csmacd, LINK_SPEED_OF_YOUR_NETIF_IN_BPS);

  netif->name[0] = IFNAME0;
  netif->name[1] = IFNAME1;
  /* We directly use etharp_output() here to save a function call.
   * You can instead declare your own function an call etharp_output()
   * from it if you have to do some checks before sending (e.g. if link
   * is available...) */
  netif->output = etharp_output;
  netif->linkoutput = low_level_output;
  
  
  /* initialize the hardware */
  result = low_level_init(netif);

  return result;
}
