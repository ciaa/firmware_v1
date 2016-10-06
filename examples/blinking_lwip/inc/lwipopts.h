/*
 * @brief LWIP build option override file
 *
 * @note
 * Copyright(C) NXP Semiconductors, 2012
 * All rights reserved.
 *
 * @par
 * Software that is described herein is for illustrative purposes only
 * which provides customers with programming information regarding the
 * LPC products.  This software is supplied "AS IS" without any warranties of
 * any kind, and NXP Semiconductors and its licensor disclaim any and
 * all warranties, express or implied, including all implied warranties of
 * merchantability, fitness for a particular purpose and non-infringement of
 * intellectual property rights.  NXP Semiconductors assumes no responsibility
 * or liability for the use of the software, conveys no license or rights under any
 * patent, copyright, mask work right, or any other intellectual property rights in
 * or to any products. NXP Semiconductors reserves the right to make changes
 * in the software without notification. NXP Semiconductors also makes no
 * representation or warranty that such application will be suitable for the
 * specified use without further testing or modification.
 *
 * @par
 * Permission to use, copy, modify, and distribute this software and its
 * documentation is hereby granted, under NXP Semiconductors' and its
 * licensor's relevant copyrights in the software, without fee, provided that it
 * is used in conjunction with NXP Semiconductors microcontrollers.  This
 * copyright, permission, and disclaimer notice must appear in all copies of
 * this code.
 *
 * Copyright (c) 2001-2004 Swedish Institute of Computer Science.
 * All rights reserved. 
 *
 * Redistribution and use in source and binary forms, with or without modification, 
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission. 
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED 
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT 
 * SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, 
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT 
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING 
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY 
 * OF SUCH DAMAGE.
 *
 * This file is part of the lwIP TCP/IP stack.
 *
 * Author: Adam Dunkels <adam@sics.se>
 * */

#ifndef __LWIPOPTS_H_
#define __LWIPOPTS_H_


/*
   -----------------------------------------------
   ---------- Platform specific locking ----------
   -----------------------------------------------
*/

/**
 * SYS_LIGHTWEIGHT_PROT==1: if you want inter-task protection for certain
 * critical regions during buffer allocation, deallocation and memory
 * allocation and deallocation.
 */
/* Need for memory protection */
#define SYS_LIGHTWEIGHT_PROT            0

/**
 * NO_SYS==1: Provides VERY minimal functionality. Otherwise,
 * use lwIP facilities.
 */
 /* Standalone build */
#define NO_SYS                          1

/**
 * NO_SYS_NO_TIMERS==1: Drop support for sys_timeout when NO_SYS==1
 * Mainly for compatibility to old versions.
 */
/* Use LWIP timers */
#define NO_SYS_NO_TIMERS                0

/**
 * MEMCPY: override this if you have a faster implementation at hand than the
 * one included in your C library
 */
//#define MEMCPY(dst,src,len)             memcpy(dst,src,len)

/**
 * SMEMCPY: override this with care! Some compilers (e.g. gcc) can inline a
 * call to memcpy() if the length is known at compile time and is small.
 */
//#define SMEMCPY(dst,src,len)            memcpy(dst,src,len)

/*
   ------------------------------------
   ---------- Memory options ----------
   ------------------------------------
*/
/**
 * MEM_LIBC_MALLOC==1: Use malloc/free/realloc provided by your C-library
 * instead of the lwip internal allocator. Can save code size if you
 * already use it.
 */
#define MEM_LIBC_MALLOC                 1

/**
* MEMP_MEM_MALLOC==1: Use mem_malloc/mem_free instead of the lwip pool allocator.
* Especially useful with MEM_LIBC_MALLOC but handle with care regarding execution
* speed and usage from interrupts!
*/
#define MEMP_MEM_MALLOC                 1

/**
 * MEM_ALIGNMENT: should be set to the alignment of the CPU
 *    4 byte alignment -> #define MEM_ALIGNMENT 4
 *    2 byte alignment -> #define MEM_ALIGNMENT 2
 */
/* 32-bit alignment */
#define MEM_ALIGNMENT                   4

/**
 * MEM_SIZE: the size of the heap memory. If the application will send
 * a lot of data that needs to be copied, this should be set high.
 */
/* Non-static memory, used with DMA pool */
#define MEM_SIZE                        (12 * 1024)


/**
 * MEMP_SEPARATE_POOLS: if defined to 1, each pool is placed in its own array.
 * This can be used to individually change the location of each pool.
 * Default is one big array for all pools
 */
//#define MEMP_SEPARATE_POOLS             0

/**
 * MEMP_OVERFLOW_CHECK: memp overflow protection reserves a configurable
 * amount of bytes before and after each memp element in every pool and fills
 * it with a prominent default value.
 *    MEMP_OVERFLOW_CHECK == 0 no checking
 *    MEMP_OVERFLOW_CHECK == 1 checks each element when it is freed
 *    MEMP_OVERFLOW_CHECK >= 2 checks each element in every pool every time
 *      memp_malloc() or memp_free() is called (useful but slow!)
 */
//#define MEMP_OVERFLOW_CHECK             0

/**
 * MEMP_SANITY_CHECK==1: run a sanity check after each memp_free() to make
 * sure that there are no cycles in the linked lists.
 */
//#define MEMP_SANITY_CHECK               0

/**
 * MEM_USE_POOLS==1: Use an alternative to malloc() by allocating from a set
 * of memory pools of various sizes. When mem_malloc is called, an element of
 * the smallest pool that can provide the length needed is returned.
 * To use this, MEMP_USE_CUSTOM_POOLS also has to be enabled.
 */
//#define MEM_USE_POOLS                   0

/**
 * MEM_USE_POOLS_TRY_BIGGER_POOL==1: if one malloc-pool is empty, try the next
 * bigger pool - WARNING: THIS MIGHT WASTE MEMORY but it can make a system more
 * reliable. */
//#define MEM_USE_POOLS_TRY_BIGGER_POOL   0

/**
 * MEMP_USE_CUSTOM_POOLS==1: whether to include a user file lwippools.h
 * that defines additional pools beyond the "standard" ones required
 * by lwIP. If you set this to 1, you must have lwippools.h in your
 * inlude path somewhere.
 */
//#define MEMP_USE_CUSTOM_POOLS           0

/**
 * Set this to 1 if you want to free PBUF_RAM pbufs (or call mem_free()) from
 * interrupt context (or another context that doesn't allow waiting for a
 * semaphore).
 * If set to 1, mem_malloc will be protected by a semaphore and SYS_ARCH_PROTECT,
 * while mem_free will only use SYS_ARCH_PROTECT. mem_malloc SYS_ARCH_UNPROTECTs
 * with each loop so that mem_free can run.
 *
 * ATTENTION: As you can see from the above description, this leads to dis-/
 * enabling interrupts often, which can be slow! Also, on low memory, mem_malloc
 * can need longer.
 *
 * If you don't want that, at least for NO_SYS=0, you can still use the following
 * functions to enqueue a deallocation call which then runs in the tcpip_thread
 * context:
 * - pbuf_free_callback(p);
 * - mem_free_callback(m);
 */
//#define LWIP_ALLOW_MEM_FREE_FROM_OTHER_CONTEXT 0

/*
   ------------------------------------------------
   ---------- Internal Memory Pool Sizes ----------
   ------------------------------------------------
*/
/**
 * MEMP_NUM_PBUF: the number of memp struct pbufs (used for PBUF_ROM and PBUF_REF).
 * If the application sends a lot of data out of ROM (or other static memory),
 * this should be set high.
 */
//#define MEMP_NUM_PBUF                   16

/**
 * MEMP_NUM_RAW_PCB: Number of raw connection PCBs
 * (requires the LWIP_RAW option)
 */
//#define MEMP_NUM_RAW_PCB                4

/**
 * MEMP_NUM_UDP_PCB: the number of UDP protocol control blocks. One
 * per active UDP "connection".
 * (requires the LWIP_UDP option)
 */
//#define MEMP_NUM_UDP_PCB                4

/**
 * MEMP_NUM_TCP_PCB: the number of simulatenously active TCP connections.
 * (requires the LWIP_TCP option)
 */
//#define MEMP_NUM_TCP_PCB                5

/**
 * MEMP_NUM_TCP_PCB_LISTEN: the number of listening TCP connections.
 * (requires the LWIP_TCP option)
 */
//#define MEMP_NUM_TCP_PCB_LISTEN         8

/**
 * MEMP_NUM_TCP_SEG: the number of simultaneously queued TCP segments.
 * (requires the LWIP_TCP option)
 */
//#define MEMP_NUM_TCP_SEG                16

/**
 * MEMP_NUM_REASSDATA: the number of IP packets simultaneously queued for
 * reassembly (whole packets, not fragments!)
 */
//#define MEMP_NUM_REASSDATA              5

/**
 * MEMP_NUM_FRAG_PBUF: the number of IP fragments simultaneously sent
 * (fragments, not whole packets!).
 * This is only used with IP_FRAG_USES_STATIC_BUF==0 and
 * LWIP_NETIF_TX_SINGLE_PBUF==0 and only has to be > 1 with DMA-enabled MACs
 * where the packet is not yet sent when netif->output returns.
 */
//#define MEMP_NUM_FRAG_PBUF              15

/**
 * MEMP_NUM_ARP_QUEUE: the number of simulateously queued outgoing
 * packets (pbufs) that are waiting for an ARP request (to resolve
 * their destination address) to finish.
 * (requires the ARP_QUEUEING option)
 */
//#define MEMP_NUM_ARP_QUEUE              30

/**
 * MEMP_NUM_IGMP_GROUP: The number of multicast groups whose network interfaces
 * can be members et the same time (one per netif - allsystems group -, plus one
 * per netif membership).
 * (requires the LWIP_IGMP option)
 */
//#define MEMP_NUM_IGMP_GROUP             8

/**
 * MEMP_NUM_SYS_TIMEOUT: the number of simulateously active timeouts.
 * (requires NO_SYS==0)
 * The default number of timeouts is calculated here for all enabled modules.
 * The formula expects settings to be either '0' or '1'.
 */
/* #define MEMP_NUM_SYS_TIMEOUT            (LWIP_TCP + IP_REASSEMBLY + LWIP_ARP + (2*LWIP_DHCP) + LWIP_AUTOIP + LWIP_IGMP + LWIP_DNS + PPP_SUPPORT) */
#define MEMP_NUM_SYS_TIMEOUT            300

/**
 * MEMP_NUM_NETBUF: the number of struct netbufs.
 * (only needed if you use the sequential API, like api_lib.c)
 */
//#define MEMP_NUM_NETBUF                 2

/**
 * MEMP_NUM_NETCONN: the number of struct netconns.
 * (only needed if you use the sequential API, like api_lib.c)
 */
//#define MEMP_NUM_NETCONN                4

/**
 * MEMP_NUM_TCPIP_MSG_API: the number of struct tcpip_msg, which are used
 * for callback/timeout API communication.
 * (only needed if you use tcpip.c)
 */
//#define MEMP_NUM_TCPIP_MSG_API          8

/**
 * MEMP_NUM_TCPIP_MSG_INPKT: the number of struct tcpip_msg, which are used
 * for incoming packets.
 * (only needed if you use tcpip.c)
 */
//#define MEMP_NUM_TCPIP_MSG_INPKT        8

/* MEMP_NUM_SNMP_NODE: the number of leafs in the SNMP tree. */
//#define MEMP_NUM_SNMP_NODE              50

/**
 * MEMP_NUM_SNMP_ROOTNODE: the number of branches in the SNMP tree.
 * Every branch has one leaf (MEMP_NUM_SNMP_NODE) at least!
 */
//#define MEMP_NUM_SNMP_ROOTNODE          30

/**
 * MEMP_NUM_SNMP_VARBIND: the number of concurrent requests (does not have to
 * be changed normally) - 2 of these are used per request (1 for input,
 * 1 for output)
 */
//#define MEMP_NUM_SNMP_VARBIND           2

/**
 * MEMP_NUM_SNMP_VALUE: the number of OID or values concurrently used
 * (does not have to be changed normally) - 3 of these are used per request
 * (1 for the value read and 2 for OIDs - input and output)
 */
//#define MEMP_NUM_SNMP_VALUE             3

/**
 * MEMP_NUM_NETDB: the number of concurrently running lwip_addrinfo() calls
 * (before freeing the corresponding memory using lwip_freeaddrinfo()).
 */
//#define MEMP_NUM_NETDB                  1

/**
 * MEMP_NUM_LOCALHOSTLIST: the number of host entries in the local host list
 * if DNS_LOCAL_HOSTLIST_IS_DYNAMIC==1.
 */
//#define MEMP_NUM_LOCALHOSTLIST          1

/**
 * MEMP_NUM_PPPOE_INTERFACES: the number of concurrently active PPPoE
 * interfaces (only used with PPPOE_SUPPORT==1)
 */
//#define MEMP_NUM_PPPOE_INTERFACES       1

/* PBUF_POOL_SIZE: the number of buffers in the pbuf pool.  */
/* pbuf buffers in pool. In zero-copy mode, these buffers are
   located in peripheral RAM. In copied mode, they are located in
   internal IRAM */
#define PBUF_POOL_SIZE                  7

/*
   ---------------------------------
   ---------- ARP options ----------
   ---------------------------------
*/
/* LWIP_ARP==1: Enable ARP functionality. */
// #define LWIP_ARP                        1

/* ARP_TABLE_SIZE: Number of active MAC-IP address pairs cached. */
// #define ARP_TABLE_SIZE                  10

/**
 * ARP_QUEUEING==1: Multiple outgoing packets are queued during hardware address
 * resolution. By default, only the most recent packet is queued per IP address.
 * This is sufficient for most protocols and mainly reduces TCP connection
 * startup time. Set this to 1 if you know your application sends more than one
 * packet in a row to an IP address that is not in the ARP cache.
 */
//#define ARP_QUEUEING                    0

/**
 * ETHARP_TRUST_IP_MAC==1: Incoming IP packets cause the ARP table to be
 * updated with the source MAC and IP addresses supplied in the packet.
 * You may want to disable this if you do not trust LAN peers to have the
 * correct addresses, or as a limited approach to attempt to handle
 * spoofing. If disabled, lwIP will need to make a new ARP request if
 * the peer is not already in the ARP table, adding a little latency.
 * The peer *is* in the ARP table if it requested our address before.
 * Also notice that this slows down input processing of every IP packet!
 */
//#define ETHARP_TRUST_IP_MAC             0

/**
 * ETHARP_SUPPORT_VLAN==1: support receiving ethernet packets with VLAN header.
 * Additionally, you can define ETHARP_VLAN_CHECK to an u16_t VLAN ID to check.
 * If ETHARP_VLAN_CHECK is defined, only VLAN-traffic for this VLAN is accepted.
 * If ETHARP_VLAN_CHECK is not defined, all traffic is accepted.
 * Alternatively, define a function/define ETHARP_VLAN_CHECK_FN(eth_hdr, vlan)
 * that returns 1 to accept a packet or 0 to drop a packet.
 */
//#define ETHARP_SUPPORT_VLAN             0

/** LWIP_ETHERNET==1: enable ethernet support for PPPoE even though ARP
 * might be disabled
 */
//#define LWIP_ETHERNET                   (LWIP_ARP || PPPOE_SUPPORT)

/** ETH_PAD_SIZE: number of bytes added before the ethernet header to ensure
 * alignment of payload after that header. Since the header is 14 bytes long,
 * without this padding e.g. addresses in the IP header will not be aligned
 * on a 32-bit boundary, so setting this to 2 can speed up 32-bit-platforms.
 */
 /* No padding needed */
#define ETH_PAD_SIZE                    0

/** ETHARP_SUPPORT_STATIC_ENTRIES==1: enable code to support static ARP table
 * entries (using etharp_add_static_entry/etharp_remove_static_entry).
 */
//#define ETHARP_SUPPORT_STATIC_ENTRIES   0


/*
   --------------------------------
   ---------- IP options ----------
   --------------------------------
*/
/**
 * IP_FORWARD==1: Enables the ability to forward IP packets across network
 * interfaces. If you are going to run lwIP on a device with only one network
 * interface, define this to 0.
 */
//#define IP_FORWARD                      0

/**
 * IP_OPTIONS_ALLOWED: Defines the behavior for IP options.
 *      IP_OPTIONS_ALLOWED==0: All packets with IP options are dropped.
 *      IP_OPTIONS_ALLOWED==1: IP options are allowed (but not parsed).
 */
//#define IP_OPTIONS_ALLOWED              1

/**
 * IP_REASSEMBLY==1: Reassemble incoming fragmented IP packets. Note that
 * this option does not affect outgoing packet sizes, which can be controlled
 * via IP_FRAG.
 */
//#define IP_REASSEMBLY                   1

/**
 * IP_FRAG==1: Fragment outgoing IP packets if their size exceeds MTU. Note
 * that this option does not affect incoming packet sizes, which can be
 * controlled via IP_REASSEMBLY.
 */
//#define IP_FRAG                         1

/**
 * IP_REASS_MAXAGE: Maximum time (in multiples of IP_TMR_INTERVAL - so seconds, normally)
 * a fragmented IP packet waits for all fragments to arrive. If not all fragments arrived
 * in this time, the whole packet is discarded.
 */
//#define IP_REASS_MAXAGE                 3

/**
 * IP_REASS_MAX_PBUFS: Total maximum amount of pbufs waiting to be reassembled.
 * Since the received pbufs are enqueued, be sure to configure
 * PBUF_POOL_SIZE > IP_REASS_MAX_PBUFS so that the stack is still able to receive
 * packets even if the maximum amount of fragments is enqueued for reassembly!
 */
//#define IP_REASS_MAX_PBUFS              10

/**
 * IP_FRAG_USES_STATIC_BUF==1: Use a static MTU-sized buffer for IP
 * fragmentation. Otherwise pbufs are allocated and reference the original
 * packet data to be fragmented (or with LWIP_NETIF_TX_SINGLE_PBUF==1,
 * new PBUF_RAM pbufs are used for fragments).
 * ATTENTION: IP_FRAG_USES_STATIC_BUF==1 may not be used for DMA-enabled MACs!
 */
//#define IP_FRAG_USES_STATIC_BUF         0

/**
 * IP_FRAG_MAX_MTU: Assumed max MTU on any interface for IP frag buffer
 * (requires IP_FRAG_USES_STATIC_BUF==1)
 */
//#define IP_FRAG_MAX_MTU                 1500

/* IP_DEFAULT_TTL: Default value for Time-To-Live used by transport layers. */
//#define IP_DEFAULT_TTL                  255

/**
 * IP_SOF_BROADCAST=1: Use the SOF_BROADCAST field to enable broadcast
 * filter per pcb on udp and raw send operations. To enable broadcast filter
 * on recv operations, you also have to set IP_SOF_BROADCAST_RECV=1.
 */
#define IP_SOF_BROADCAST                1

/**
 * IP_SOF_BROADCAST_RECV (requires IP_SOF_BROADCAST=1) enable the broadcast
 * filter on recv operations.
 */
#define IP_SOF_BROADCAST_RECV           1

/**
 * IP_FORWARD_ALLOW_TX_ON_RX_NETIF==1: allow ip_forward() to send packets back
 * out on the netif where it was received. This should only be used for
 * wireless networks.
 * ATTENTION: When this is 1, make sure your netif driver correctly marks incoming
 * link-layer-broadcast/multicast packets as such using the corresponding pbuf flags!
 */
//#define IP_FORWARD_ALLOW_TX_ON_RX_NETIF 0

/**
 * LWIP_RANDOMIZE_INITIAL_LOCAL_PORTS==1: randomize the local port for the first
 * local TCP/UDP pcb (default==0). This can prevent creating predictable port
 * numbers after booting a device.
 */
//#define LWIP_RANDOMIZE_INITIAL_LOCAL_PORTS 0

/*
   ----------------------------------
   ---------- ICMP options ----------
   ----------------------------------
*/
/**
 * LWIP_ICMP==1: Enable ICMP module inside the IP stack.
 * Be careful, disable that make your product non-compliant to RFC1122
 */
//#define LWIP_ICMP                       1

/* ICMP_TTL: Default value for Time-To-Live used by ICMP packets. */
//#define ICMP_TTL                       (IP_DEFAULT_TTL)

/* respond to broadcast pings (default is unicast only) */
#define LWIP_BROADCAST_PING             1

/**
 * LWIP_MULTICAST_PING==1: respond to multicast pings (default is unicast only)
 */
//#define LWIP_MULTICAST_PING             0

/*
   ---------------------------------
   ---------- RAW options ----------
   ---------------------------------
*/
/* LWIP_RAW==1: Enable application layer to hook into the IP layer itself. */
/* Raw interface not needed */
#define LWIP_RAW                        1

/* LWIP_RAW==1: Enable application layer to hook into the IP layer itself. */
//#define RAW_TTL                        (IP_DEFAULT_TTL)

/*
   ----------------------------------
   ---------- DHCP options ----------
   ----------------------------------
*/
/* LWIP_DHCP==1: Enable DHCP module. */
/* DHCP is ok, UDP is required with DHCP */
#define LWIP_DHCP                       0

/* DHCP_DOES_ARP_CHECK==1: Do an ARP check on the offered address. */
//#define DHCP_DOES_ARP_CHECK             ((LWIP_DHCP) && (LWIP_ARP))

/*
   ------------------------------------
   ---------- AUTOIP options ----------
   ------------------------------------
*/
/* LWIP_AUTOIP==1: Enable AUTOIP module. */
//#define LWIP_AUTOIP                     0

/**
 * LWIP_DHCP_AUTOIP_COOP==1: Allow DHCP and AUTOIP to be both enabled on
 * the same interface at the same time.
 */
//#define LWIP_DHCP_AUTOIP_COOP           0

/**
 * LWIP_DHCP_AUTOIP_COOP_TRIES: Set to the number of DHCP DISCOVER probes
 * that should be sent before falling back on AUTOIP. This can be set
 * as low as 1 to get an AutoIP address very quickly, but you should
 * be prepared to handle a changing IP address when DHCP overrides
 * AutoIP.
 */
//#define LWIP_DHCP_AUTOIP_COOP_TRIES     9

/*
   ----------------------------------
   ---------- SNMP options ----------
   ----------------------------------
*/
/* LWIP_SNMP==1: Turn on SNMP module.UDP must be available for SNMP transport */
//#define LWIP_SNMP                       0

/**
 * SNMP_CONCURRENT_REQUESTS: Number of concurrent requests the module will
 * allow. At least one request buffer is required.
 * Does not have to be changed unless external MIBs answer request asynchronously
 */
//#define SNMP_CONCURRENT_REQUESTS        1

/**
 * SNMP_TRAP_DESTINATIONS: Number of trap destinations. At least one trap
 * destination is required
 */
//#define SNMP_TRAP_DESTINATIONS          1

/**
 * SNMP_PRIVATE_MIB: 
 * When using a private MIB, you have to create a file 'private_mib.h' that contains
 * a 'struct mib_array_node mib_private' which contains your MIB.
 */
//#define SNMP_PRIVATE_MIB                0

/**
 * Only allow SNMP write actions that are 'safe' (e.g. disabeling netifs is not
 * a safe action and disabled when SNMP_SAFE_REQUESTS = 1).
 * Unsafe requests are disabled by default!
 */
//#define SNMP_SAFE_REQUESTS              1

/**
 * The maximum length of strings used. This affects the size of
 * MEMP_SNMP_VALUE elements.
 */
//#define SNMP_MAX_OCTET_STRING_LEN       127

/**
 * The maximum depth of the SNMP tree.
 * With private MIBs enabled, this depends on your MIB!
 * This affects the size of MEMP_SNMP_VALUE elements.
 */
//#define SNMP_MAX_TREE_DEPTH             15

/**
 * The size of the MEMP_SNMP_VALUE elements, normally calculated from
 * SNMP_MAX_OCTET_STRING_LEN and SNMP_MAX_TREE_DEPTH.
 */
//#define SNMP_MAX_VALUE_SIZE             LWIP_MAX((SNMP_MAX_OCTET_STRING_LEN)+1, sizeof(s32_t)*(SNMP_MAX_TREE_DEPTH))

/*
   ----------------------------------
   ---------- IGMP options ----------
   ----------------------------------
*/
/* LWIP_IGMP==1: Turn on IGMP module.  */
//#define LWIP_IGMP                       0

/*
   ----------------------------------
   ---------- DNS options -----------
   ----------------------------------
*/
/* LWIP_DNS==1: Turn on DNS module. UDP must be available for DNS transport. */
//#define LWIP_DNS                        0

/** DNS maximum number of entries to maintain locally. */
//#define DNS_TABLE_SIZE                  4

/** DNS maximum host name length supported in the name table. */
//#define DNS_MAX_NAME_LENGTH             256

/** The maximum of DNS servers */
//#define DNS_MAX_SERVERS                 2

/** DNS do a name checking between the query and the response. */
//#define DNS_DOES_NAME_CHECK             1

/** DNS message max. size. Default value is RFC compliant. */
//#define DNS_MSG_SIZE                    512

/** DNS_LOCAL_HOSTLIST: Implements a local host-to-address list. If enabled,
 *  you have to define
 *    #define DNS_LOCAL_HOSTLIST_INIT {{"host1", 0x123}, {"host2", 0x234}}
 *  (an array of structs name/address, where address is an u32_t in network
 *  byte order).
 *
 *  Instead, you can also use an external function:
 *  #define DNS_LOOKUP_LOCAL_EXTERN(x) extern u32_t my_lookup_function(const char *name)
 *  that returns the IP address or INADDR_NONE if not found.
 */
//#define DNS_LOCAL_HOSTLIST              0

/** If this is turned on, the local host-list can be dynamically changed
 *  at runtime. */
//#define DNS_LOCAL_HOSTLIST_IS_DYNAMIC   0

/*
   ---------------------------------
   ---------- UDP options ----------
   ---------------------------------
*/
/* LWIP_UDP==1: Turn on UDP. */
#define LWIP_UDP                        1

/* LWIP_UDPLITE==1: Turn on UDP-Lite. (Requires LWIP_UDP) */
//#define LWIP_UDPLITE                    0

/* UDP_TTL: Default Time-To-Live value. */
//#define UDP_TTL                         (IP_DEFAULT_TTL)

/* LWIP_NETBUF_RECVINFO==1: append destination addr and port to every netbuf. */
//#define LWIP_NETBUF_RECVINFO            0

/*
   ---------------------------------
   ---------- TCP options ----------
   ---------------------------------
*/
/* LWIP_TCP==1: Turn on TCP. */
//#define LWIP_TCP                        1

/* TCP_TTL: Default Time-To-Live value. */
//#define TCP_TTL                         (IP_DEFAULT_TTL)

/**
 * TCP_WND: The size of a TCP window.  This must be at least 
 * (2 * TCP_MSS) for things to work well
 */
//#define TCP_WND                         (4 * TCP_MSS)

/* TCP_MAXRTX: Maximum number of retransmissions of data segments. */
//#define TCP_MAXRTX                      12

/* TCP_SYNMAXRTX: Maximum number of retransmissions of SYN segments. */
//#define TCP_SYNMAXRTX                   6

/**
 * TCP_QUEUE_OOSEQ==1: TCP will queue segments that arrive out of order.
 * Define to 0 if your device is low on memory.
 */
//#define TCP_QUEUE_OOSEQ                 (LWIP_TCP)

/**
 * TCP_MSS: TCP Maximum segment size. (default is 536, a conservative default,
 * you might want to increase this.)
 * For the receive side, this MSS is advertised to the remote side
 * when opening a connection. For the transmit size, this MSS sets
 * an upper limit on the MSS advertised by the remote host.
 */
/* MSS should match the hardware packet size */
#define TCP_MSS                         1460

/**
 * TCP_CALCULATE_EFF_SEND_MSS: "The maximum size of a segment that TCP really
 * sends, the 'effective send MSS,' MUST be the smaller of the send MSS (which
 * reflects the available reassembly buffer size at the remote host) and the
 * largest size permitted by the IP layer" (RFC 1122)
 * Setting this to 1 enables code that checks TCP_MSS against the MTU of the
 * netif used for a connection and limits the MSS if it would be too big otherwise.
 */
//#define TCP_CALCULATE_EFF_SEND_MSS      1


/**
 * TCP_SND_BUF: TCP sender buffer space (bytes).
 * To achieve good performance, this should be at least 2 * TCP_MSS.
 */
#define TCP_SND_BUF                     (2 * TCP_MSS)

/**
 * TCP_SND_QUEUELEN: TCP sender buffer space (pbufs). This must be at least
 * as much as (2 * TCP_SND_BUF/TCP_MSS) for things to work.
 */
//#define TCP_SND_QUEUELEN                ((4 * (TCP_SND_BUF) + (TCP_MSS - 1))/(TCP_MSS))

/**
 * TCP_SNDLOWAT: TCP writable space (bytes). This must be less than
 * TCP_SND_BUF. It is the amount of space which must be available in the
 * TCP snd_buf for select to return writable (combined with TCP_SNDQUEUELOWAT).
 */
#define TCP_SNDLOWAT                    LWIP_MIN(LWIP_MAX(((TCP_SND_BUF)/2), (2 * TCP_MSS) + 1), (TCP_SND_BUF) - 1)

/**
 * TCP_SNDQUEUELOWAT: TCP writable bufs (pbuf count). This must be less
 * than TCP_SND_QUEUELEN. If the number of pbufs queued on a pcb drops below
 * this number, select returns writable (combined with TCP_SNDLOWAT).
 */
//#define TCP_SNDQUEUELOWAT               LWIP_MAX(((TCP_SND_QUEUELEN)/2), 5)

/**
 * TCP_OOSEQ_MAX_BYTES: The maximum number of bytes queued on ooseq per pcb.
 * Default is 0 (no limit). Only valid for TCP_QUEUE_OOSEQ==0.
 */
//#define TCP_OOSEQ_MAX_BYTES             0

/**
 * TCP_OOSEQ_MAX_PBUFS: The maximum number of pbufs queued on ooseq per pcb.
 * Default is 0 (no limit). Only valid for TCP_QUEUE_OOSEQ==0.
 */
//#define TCP_OOSEQ_MAX_PBUFS             0

/* TCP_LISTEN_BACKLOG: Enable the backlog option for tcp listen pcb. */
//#define TCP_LISTEN_BACKLOG              0

/**
 * The maximum allowed backlog for TCP listen netconns.
 * This backlog is used unless another is explicitly specified.
 * 0xff is the maximum (u8_t).
 */
//#define TCP_DEFAULT_LISTEN_BACKLOG      0xff

/**
 * TCP_OVERSIZE: The maximum number of bytes that tcp_write may
 * allocate ahead of time in an attempt to create shorter pbuf chains
 * for transmission. The meaningful range is 0 to TCP_MSS. Some
 * suggested values are:
 *
 * 0:         Disable oversized allocation. Each tcp_write() allocates a new
              pbuf (old behaviour).
 * 1:         Allocate size-aligned pbufs with minimal excess. Use this if your
 *            scatter-gather DMA requires aligned fragments.
 * 128:       Limit the pbuf/memory overhead to 20%.
 * TCP_MSS:   Try to create unfragmented TCP packets.
 * TCP_MSS/4: Try to create 4 fragments or less per TCP packet.
 */
//#define TCP_OVERSIZE                    TCP_MSS

/* LWIP_TCP_TIMESTAMPS==1: support the TCP timestamp option. */
//#define LWIP_TCP_TIMESTAMPS             0

/**
 * TCP_WND_UPDATE_THRESHOLD: difference in window to trigger an
 * explicit window update
 */
//#define TCP_WND_UPDATE_THRESHOLD   (TCP_WND / 4)

/**
 * LWIP_EVENT_API and LWIP_CALLBACK_API: Only one of these should be set to 1.
 *     LWIP_EVENT_API==1: The user defines lwip_tcp_event() to receive all
 *         events (accept, sent, etc) that happen in the system.
 *     LWIP_CALLBACK_API==1: The PCB callback function is called directly
 *         for the event. This is the default.
 */
//#define LWIP_EVENT_API                  0
//#define LWIP_CALLBACK_API               1


/*
   ----------------------------------
   ---------- Pbuf options ----------
   ----------------------------------
*/
/**
 * PBUF_LINK_HLEN: the number of bytes that should be allocated for a
 * link level header. The default is 14, the standard value for
 * Ethernet.
 */
//#define PBUF_LINK_HLEN                  (14 + ETH_PAD_SIZE)

/**
 * PBUF_POOL_BUFSIZE: the size of each pbuf in the pbuf pool. The default is
 * designed to accomodate single full size TCP frame in one pbuf, including
 * TCP_MSS, IP header, and link header.
 */
//#define PBUF_POOL_BUFSIZE               LWIP_MEM_ALIGN_SIZE(TCP_MSS+40+PBUF_LINK_HLEN)

/*
   ------------------------------------------------
   ---------- Network Interfaces options ----------
   ------------------------------------------------
*/
/**
 * LWIP_NETIF_HOSTNAME==1: use DHCP_OPTION_HOSTNAME with netif's hostname
 * field.
 */
/* Hostname can be used */
#define LWIP_NETIF_HOSTNAME             1

/* LWIP_NETIF_API==1: Support netif api (in netifapi.c) */
//#define LWIP_NETIF_API                  0

/**
 * LWIP_NETIF_STATUS_CALLBACK==1: Support a callback function whenever an interface
 * changes its up/down status (i.e., due to DHCP IP acquistion)
 */
//#define LWIP_NETIF_STATUS_CALLBACK      0

/**
 * LWIP_NETIF_LINK_CALLBACK==1: Support a callback function from an interface
 * whenever the link changes (i.e., link down)
 */
//#define LWIP_NETIF_LINK_CALLBACK        0

/**
 * LWIP_NETIF_REMOVE_CALLBACK==1: Support a callback function that is called
 * when a netif has been removed
 */
//#define LWIP_NETIF_REMOVE_CALLBACK      0

/**
 * LWIP_NETIF_HWADDRHINT==1: Cache link-layer-address hints (e.g. table
 * indices) in struct netif. TCP and UDP can make use of this to prevent
 * scanning the ARP table for every sent packet. While this is faster for big
 * ARP tables or many concurrent connections, it might be counterproductive
 * if you have a tiny ARP table or if there never are concurrent connections.
 */
//#define LWIP_NETIF_HWADDRHINT           0

/**
 * LWIP_NETIF_LOOPBACK==1: Support sending packets with a destination IP
 * address equal to the netif IP address, looping them back up the stack.
 */
//#define LWIP_NETIF_LOOPBACK             0

/**
 * LWIP_LOOPBACK_MAX_PBUFS: Maximum number of pbufs on queue for loopback
 * sending for each netif (0 = disabled)
 */
//#define LWIP_LOOPBACK_MAX_PBUFS         0

/**
 * LWIP_NETIF_LOOPBACK_MULTITHREADING: Indicates whether threading is enabled in
 * the system, as netifs must change how they behave depending on this setting
 * for the LWIP_NETIF_LOOPBACK option to work.
 * Setting this is needed to avoid reentering non-reentrant functions like
 * tcp_input().
 *    LWIP_NETIF_LOOPBACK_MULTITHREADING==1: Indicates that the user is using a
 *       multithreaded environment like tcpip.c. In this case, netif->input()
 *       is called directly.
 *    LWIP_NETIF_LOOPBACK_MULTITHREADING==0: Indicates a polling (or NO_SYS) setup.
 *       The packets are put on a list and netif_poll() must be called in
 *       the main application loop.
 */
//#define LWIP_NETIF_LOOPBACK_MULTITHREADING    (!NO_SYS)

/**
 * LWIP_NETIF_TX_SINGLE_PBUF: if this is set to 1, lwIP tries to put all data
 * to be sent into one single pbuf. This is for compatibility with DMA-enabled
 * MACs that do not support scatter-gather.
 * Beware that this might involve CPU-memcpy before transmitting that would not
 * be needed without this flag! Use this only if you need to!
 *
 * @todo: TCP and IP-frag do not work with this, yet:
 */
//#define LWIP_NETIF_TX_SINGLE_PBUF             0

/*
   ------------------------------------
   ---------- LOOPIF options ----------
   ------------------------------------
*/
/* LWIP_HAVE_LOOPIF==1: Support loop interface (127.0.0.1) and loopif.c  */
//#define LWIP_HAVE_LOOPIF                0

/* ------------------------------------
   ---------- SLIPIF options ----------
   ------------------------------------ */
/* LWIP_HAVE_SLIPIF==1: Support slip interface and slipif.c */
//#define LWIP_HAVE_SLIPIF                0

/* ------------------------------------
   ---------- Thread options ----------
   ------------------------------------ */
/* TCPIP_THREAD_NAME: The name assigned to the main tcpip thread. */
//#define TCPIP_THREAD_NAME              "tcpip_thread"

/**
 * TCPIP_THREAD_STACKSIZE: The stack size used by the main tcpip thread.
 * The stack size value itself is platform-dependent, but is passed to
 * sys_thread_new() when the thread is created.
 */
//#define TCPIP_THREAD_STACKSIZE          0

/**
 * TCPIP_THREAD_PRIO: The priority assigned to the main tcpip thread.
 * The priority value itself is platform-dependent, but is passed to
 * sys_thread_new() when the thread is created.
 */
//#define TCPIP_THREAD_PRIO               1

/**
 * TCPIP_MBOX_SIZE: The mailbox size for the tcpip thread messages
 * The queue size value itself is platform-dependent, but is passed to
 * sys_mbox_new() when tcpip_init is called.
 */
//#define TCPIP_MBOX_SIZE                 0

/* SLIPIF_THREAD_NAME: The name assigned to the slipif_loop thread. */
//#define SLIPIF_THREAD_NAME             "slipif_loop"

/**
 * SLIP_THREAD_STACKSIZE: The stack size used by the slipif_loop thread.
 * The stack size value itself is platform-dependent, but is passed to
 * sys_thread_new() when the thread is created.
 */
//#define SLIPIF_THREAD_STACKSIZE         0

/**
 * SLIPIF_THREAD_PRIO: The priority assigned to the slipif_loop thread.
 * The priority value itself is platform-dependent, but is passed to
 * sys_thread_new() when the thread is created. 
 */
//#define SLIPIF_THREAD_PRIO              1

/* PPP_THREAD_NAME: The name assigned to the pppInputThread. */
//#define PPP_THREAD_NAME                "pppInputThread"

/**
 * PPP_THREAD_STACKSIZE: The stack size used by the pppInputThread.
 * The stack size value itself is platform-dependent, but is passed to
 * sys_thread_new() when the thread is created.
 */
//#define PPP_THREAD_STACKSIZE            0

/**
 * PPP_THREAD_PRIO: The priority assigned to the pppInputThread.
 * The priority value itself is platform-dependent, but is passed to
 * sys_thread_new() when the thread is created.
 */
//#define PPP_THREAD_PRIO                 1

/* DEFAULT_THREAD_NAME: The name assigned to any other lwIP thread. */
//#define DEFAULT_THREAD_NAME            "lwIP"

/**
 * DEFAULT_THREAD_STACKSIZE: The stack size used by any other lwIP thread.
 * The stack size value itself is platform-dependent, but is passed to
 * sys_thread_new() when the thread is created.
 */
//#define DEFAULT_THREAD_STACKSIZE        0

/**
 * DEFAULT_THREAD_PRIO: The priority assigned to any other lwIP thread.
 * The priority value itself is platform-dependent, but is passed to
 * sys_thread_new() when the thread is created.
 */
//#define DEFAULT_THREAD_PRIO             1

/**
 * DEFAULT_RAW_RECVMBOX_SIZE: The mailbox size for the incoming packets on a
 * NETCONN_RAW. The queue size value itself is platform-dependent, but is passed
 * to sys_mbox_new() when the recvmbox is created.
 */
//#define DEFAULT_RAW_RECVMBOX_SIZE       0

/**
 * DEFAULT_UDP_RECVMBOX_SIZE: The mailbox size for the incoming packets on a
 * NETCONN_UDP. The queue size value itself is platform-dependent, but is passed
 * to sys_mbox_new() when the recvmbox is created.
 */
//#define DEFAULT_UDP_RECVMBOX_SIZE       0

/**
 * DEFAULT_TCP_RECVMBOX_SIZE: The mailbox size for the incoming packets on a
 * NETCONN_TCP. The queue size value itself is platform-dependent, but is passed
 * to sys_mbox_new() when the recvmbox is created.
 */
//#define DEFAULT_TCP_RECVMBOX_SIZE       0

/**
 * DEFAULT_ACCEPTMBOX_SIZE: The mailbox size for the incoming connections.
 * The queue size value itself is platform-dependent, but is passed to
 * sys_mbox_new() when the acceptmbox is created.
 */
//#define DEFAULT_ACCEPTMBOX_SIZE         0

/*
   ----------------------------------------------
   ---------- Sequential layer options ----------
   ----------------------------------------------
*/
/**
 * LWIP_TCPIP_CORE_LOCKING: (EXPERIMENTAL!)
 * Don't use it if you're not an active lwIP project member
 */
//#define LWIP_TCPIP_CORE_LOCKING         0

/**
 * LWIP_TCPIP_CORE_LOCKING_INPUT: (EXPERIMENTAL!)
 * Don't use it if you're not an active lwIP project member
 */
//#define LWIP_TCPIP_CORE_LOCKING_INPUT   0

/* LWIP_NETCONN==1: Enable Netconn API (require to use api_lib.c) */
#define LWIP_NETCONN                    0

/** LWIP_TCPIP_TIMEOUT==1: Enable tcpip_timeout/tcpip_untimeout tod create
 * timers running in tcpip_thread from another thread.
 */
//#define LWIP_TCPIP_TIMEOUT              1

/*
   ------------------------------------
   ---------- Socket options ----------
   ------------------------------------
*/
/* LWIP_SOCKET==1: Enable Socket API (require to use sockets.c)  */
#define LWIP_SOCKET                     0

/**
 * LWIP_COMPAT_SOCKETS==1: Enable BSD-style sockets functions names.
 * (only used if you use sockets.c)
 */
//#define LWIP_COMPAT_SOCKETS             1

/**
 * LWIP_POSIX_SOCKETS_IO_NAMES==1: Enable POSIX-style sockets functions names.
 * Disable this option if you use a POSIX operating system that uses the same
 * names (read, write & close). (only used if you use sockets.c)
 */
//#define LWIP_POSIX_SOCKETS_IO_NAMES     1

/**
 * LWIP_TCP_KEEPALIVE==1: Enable TCP_KEEPIDLE, TCP_KEEPINTVL and TCP_KEEPCNT
 * options processing. Note that TCP_KEEPIDLE and TCP_KEEPINTVL have to be set
 * in seconds. (does not require sockets.c, and will affect tcp.c)
 */
//#define LWIP_TCP_KEEPALIVE              0

/* LWIP_SO_SNDTIMEO==1: Enable send timeout for sockets/netconns and
 * SO_SNDTIMEO processing. */
//#define LWIP_SO_SNDTIMEO                0

/* LWIP_SO_RCVTIMEO==1: Enable receive timeout for sockets/netconns and
 * SO_RCVTIMEO processing. */
//#define LWIP_SO_RCVTIMEO                0

/* LWIP_SO_RCVBUF==1: Enable SO_RCVBUF processing. */
//#define LWIP_SO_RCVBUF                  0

/* If LWIP_SO_RCVBUF is used, this is the default value for recv_bufsize */
//#define RECV_BUFSIZE_DEFAULT            INT_MAX

/* SO_REUSE==1: Enable SO_REUSEADDR option. */
//#define SO_REUSE                        0

/**
 * SO_REUSE_RXTOALL==1: Pass a copy of incoming broadcast/multicast packets
 * to all local matches if SO_REUSEADDR is turned on.
 * WARNING: Adds a memcpy for every packet if passing to more than one pcb!
 */
//#define SO_REUSE_RXTOALL                0

/*
   ----------------------------------------
   ---------- Statistics options ----------
   ----------------------------------------
*/

/* LWIP_STATS==1: Enable statistics collection in lwip_stats. */
#define LWIP_STATS                      0


/* LWIP_STATS_DISPLAY==1: Compile in the statistics output functions */
#define LWIP_STATS_DISPLAY              0

/* LINK_STATS==1: Enable link stats.  */
#define LINK_STATS                      0

/* ETHARP_STATS==1: Enable etharp stats. */
//#define ETHARP_STATS                    (LWIP_ARP)

/* IP_STATS==1: Enable IP stats. */
//#define IP_STATS                        1

/* IPFRAG_STATS==1: Enable IP fragmentation stats. Default is
 * on if using either frag or reass. */
//#define IPFRAG_STATS                    (IP_REASSEMBLY || IP_FRAG)

/* ICMP_STATS==1: Enable ICMP stats. */
//#define ICMP_STATS                      1

/* IGMP_STATS==1: Enable IGMP stats. */
//#define IGMP_STATS                      (LWIP_IGMP)

/* UDP_STATS==1: Enable UDP stats.Default is on if UDP enabled, otherwise off */
//#define UDP_STATS                       (LWIP_UDP)

/* TCP_STATS==1: Enable TCP stats. Default is on if TCP 
 * enabled, otherwise off. */
//#define TCP_STATS                       (LWIP_TCP)

/* MEM_STATS==1: Enable mem.c stats. */
//#define MEM_STATS                       ((MEM_LIBC_MALLOC == 0) && (MEM_USE_POOLS == 0))

/* MEMP_STATS==1: Enable memp.c pool stats. */
//#define MEMP_STATS                      (MEMP_MEM_MALLOC == 0)

/* SYS_STATS==1: Enable system stats (sem and mbox counts, etc). */
//#define SYS_STATS                       (NO_SYS == 0)


/*
   ---------------------------------
   ---------- PPP options ----------
   ---------------------------------
*/

//#define PPP_SUPPORT                     0 /* <= PPP_SUPPORT==1: Enable PPP. */

//#define PPPOE_SUPPORT                   0 /* <= PPPOE_SUPPORT==1: Enable PPP Over Ethernet  */

//#define PPPOS_SUPPORT                   PPP_SUPPORT /* <= PPPOS_SUPPORT==1: Enable PPP Over Serial */


//#if PPP_SUPPORT

//#define NUM_PPP                         1 /* NUM_PPP: Max PPP sessions.  */

//#define PAP_SUPPORT                     0 /* PAP_SUPPORT==1: Support PAP. */

//#define CHAP_SUPPORT                    0 /* CHAP_SUPPORT==1: Support CHAP.  */

/* MSCHAP_SUPPORT==1: Support MSCHAP. CURRENTLY NOT SUPPORTED! DO NOT SET!  */
//#define MSCHAP_SUPPORT                  0

/*  CBCP_SUPPORT==1: Support CBCP. CURRENTLY NOT SUPPORTED! DO NOT SET!  */
//#define CBCP_SUPPORT                    0

/*  CCP_SUPPORT==1: Support CCP. CURRENTLY NOT SUPPORTED! DO NOT SET!  */
//#define CCP_SUPPORT                     0

/* VJ_SUPPORT==1: Support VJ header compression */
//#define VJ_SUPPORT                      0

/* MD5_SUPPORT==1: Support MD5 (see also CHAP). */
//#define MD5_SUPPORT                     0

/*
 * Timeouts
 */
//#define FSM_DEFTIMEOUT                  6       /* Timeout time in seconds */
//#define FSM_DEFMAXTERMREQS              2       /* Maximum Terminate-Request transmissions */
//#define FSM_DEFMAXCONFREQS              10      /* Maximum Configure-Request transmissions */
//#define FSM_DEFMAXNAKLOOPS              5       /* Maximum number of nak loops */
//#define UPAP_DEFTIMEOUT                 6       /* Timeout (seconds) for retransmitting req */
//#define UPAP_DEFREQTIME                 30      /* Time to wait for auth-req from peer */
//#define CHAP_DEFTIMEOUT                 6       /* Timeout time in seconds */
//#define CHAP_DEFTRANSMITS               10      /* max # times to send challenge */

/* Interval in seconds between keepalive echo requests, 0 to disable. */
//#define LCP_ECHOINTERVAL                0

/* Number of unanswered echo requests before failure. */
//#define LCP_MAXECHOFAILS                3

/* Max Xmit idle time (in jiffies) before resend flag char. */
//#define PPP_MAXIDLEFLAG                 100

/*
 * Packet sizes
 *
 * Note - lcp shouldn't be allowed to negotiate stuff outside these
 *    limits.  See lcp.h in the pppd directory.
 * (XXX - these constants should simply be shared by lcp.c instead
 *    of living in lcp.h)
 */
//#define PPP_MTU                         1500     /* Default MTU (size of Info field) */
/* #define PPP_MAXMTU  65535 - (PPP_HDRLEN + PPP_FCSLEN) */
//#define PPP_MAXMTU                      1500 /* Largest MTU we allow */
//#define PPP_MINMTU                      64
//#define PPP_MRU                         1500 /* default MRU = max length of info field */
//#define PPP_MAXMRU                      1500 /* Largest MRU we allow */
//#define PPP_DEFMRU                      296  /* Try for this */
//#define PPP_MINMRU                      128  /* No MRUs below this */
//#define MAXNAMELEN                      256  /* max length of hostname or name for auth */
//#define MAXSECRETLEN                    256  /* max length of password or secret */

//#endif /* PPP_SUPPORT */

/*
   --------------------------------------
   ---------- Checksum options ----------
   --------------------------------------
*/
/* CHECKSUM_GEN_IP==1: Generate checksums in software for outgoing IP packet */
#define CHECKSUM_GEN_IP                 1

/* CHECKSUM_GEN_UDP==1: Generate checksums in software for outgoing UDP pckts */
#define CHECKSUM_GEN_UDP                1

/* CHECKSUM_GEN_TCP==1: Generate checksums in software for outgoing TCP pckts */
#define CHECKSUM_GEN_TCP                1

/* CHECKSUM_GEN_ICMP==1: Generate checksums in softe for outgoing ICMP pckts */
//#define CHECKSUM_GEN_ICMP               1

/* CHECKSUM_CHECK_IP==1: Check checksums in software for incoming IP packets. */
#define CHECKSUM_CHECK_IP               1

/* CHECKSUM_CHECK_UDP==1: Check checksums in software for incoming UDP pckts */
#define CHECKSUM_CHECK_UDP              1

/* CHECKSUM_CHECK_TCP==1: Check checksums in software for incoming TCP pckts */
#define CHECKSUM_CHECK_TCP              1

/**
 * LWIP_CHECKSUM_ON_COPY==1: Calculate checksum when copying data from
 * application buffers to pbufs.
 */
#define LWIP_CHECKSUM_ON_COPY           1

/* Use LWIP version of htonx() to allow generic functionality across
   all platforms. If you are using the Cortex Mx devices, you might
   be able to use the Cortex __rev instruction instead. */
/*FIXME should be in cc.h*/
#define LWIP_PLATFORM_BYTESWAP          0

/*
   ---------------------------------------
   ---------- Hook options ---------------
   ---------------------------------------
*/

/* Hooks are undefined by default, define them to a function if you need them. */

/**
 * LWIP_HOOK_IP4_INPUT(pbuf, input_netif):
 * - called from ip_input() (IPv4)
 * - pbuf: received struct pbuf passed to ip_input()
 * - input_netif: struct netif on which the packet has been received
 * Return values:
 * - 0: Hook has not consumed the packet, packet is processed as normal
 * - != 0: Hook has consumed the packet.
 * If the hook consumed the packet, 'pbuf' is in the responsibility of the hook
 * (i.e. free it when done).
 */

/**
 * LWIP_HOOK_IP4_ROUTE(dest):
 * - called from ip_route() (IPv4)
 * - dest: destination IPv4 address
 * Returns the destination netif or NULL if no destination netif is found. In
 * that case, ip_route() continues as normal.
 */

/*
   ---------------------------------------
   ---------- Debugging options ----------
   ---------------------------------------
*/
/**
 * LWIP_DBG_MIN_LEVEL: After masking, the value of the debug is
 * compared against this value. If it is smaller, then debugging
 * messages are written.
 */
//#define LWIP_DBG_MIN_LEVEL              LWIP_DBG_LEVEL_ALL

/**
 * LWIP_DBG_TYPES_ON: A mask that can be used to globally enable/disable
 * debug messages of certain types.
 */
//#define LWIP_DBG_TYPES_ON               LWIP_DBG_ON


#define ETHARP_DEBUG                    LWIP_DBG_OFF /*  <= etharp.c */
//#define NETIF_DEBUG                     LWIP_DBG_OFF /* <= netif.c */
#define PBUF_DEBUG                      LWIP_DBG_OFF /* <= pbuf.c */
//#define API_LIB_DEBUG                   LWIP_DBG_OFF /* <= api_lib.c */
//#define API_MSG_DEBUG                   LWIP_DBG_OFF /* <= api_msg.c */
//#define SOCKETS_DEBUG                   LWIP_DBG_OFF /* <= sockets.c */
//#define ICMP_DEBUG                      LWIP_DBG_OFF /* <= icmp.c */
//#define IGMP_DEBUG                      LWIP_DBG_OFF /* <= igmp.c. */
//#define INET_DEBUG                      LWIP_DBG_OFF /* <= inet.c. */
#define IP_DEBUG                        LWIP_DBG_OFF /* <= debugging for IP. */
//#define IP_REASS_DEBUG                  LWIP_DBG_OFF /* <= ip_frag.c for both frag & reass.*/
//#define RAW_DEBUG                       LWIP_DBG_OFF /* <= raw.c */
//#define MEM_DEBUG                       LWIP_DBG_OFF /* <= mem.c */
//#define MEMP_DEBUG                      LWIP_DBG_OFF /* <= memp.c */
//#define SYS_DEBUG                       LWIP_DBG_OFF /* <= sys.c */
//#define TIMERS_DEBUG                    LWIP_DBG_OFF /* <= timers.c */
#define TCP_DEBUG                       LWIP_DBG_OFF /* <= debugging for TCP */
//#define TCP_INPUT_DEBUG                 LWIP_DBG_OFF /* <= tcp_in.c */
//#define TCP_FR_DEBUG                    LWIP_DBG_OFF /* <= tcp_in.c for fast retransmit */
//#define TCP_RTO_DEBUG                   LWIP_DBG_OFF /* <= TCP for retransmit timeout */
//#define TCP_CWND_DEBUG                  LWIP_DBG_OFF /* <= TCP congestion window */
//#define TCP_WND_DEBUG                   LWIP_DBG_OFF /* <= tcp_in.c for window updating */
//#define TCP_OUTPUT_DEBUG                LWIP_DBG_OFF /* <= tcp_out.c output functions */
//#define TCP_RST_DEBUG                   LWIP_DBG_OFF /* <= TCP with the RST message */
//#define TCP_QLEN_DEBUG                  LWIP_DBG_OFF /* <= TCP queue lengths */
#define UDP_DEBUG                       LWIP_DBG_OFF /* <= debugging in UDP. */
#define TCPIP_DEBUG                     LWIP_DBG_OFF /* <= tcpip.c. */
//#define PPP_DEBUG                       LWIP_DBG_OFF /* <= debugging for PPP */
//#define SLIP_DEBUG                      LWIP_DBG_OFF /* <= slipif.c */
#define DHCP_DEBUG                      LWIP_DBG_OFF /* <= dhcp.c */
//#define AUTOIP_DEBUG                    LWIP_DBG_OFF /* <= autoip.c */
//#define SNMP_MSG_DEBUG                  LWIP_DBG_OFF /* <= SNMP messages */
//#define SNMP_MIB_DEBUG                  LWIP_DBG_OFF /* <= SNMP MIBs */
//#define DNS_DEBUG                       LWIP_DBG_OFF /* <= debugging for DNS */

/* This define is custom for the LPC EMAC driver. Enabled it to
   get debug messages for the driver. */
#define EMAC_DEBUG                    LWIP_DBG_OFF


/* FIXME replace with mem_malloc macros and his family acording to docs */
/* Needed for malloc/free */
#include "ciaaPOSIX_stdlib.h"
#define malloc ciaaPOSIX_malloc
#define free ciaaPOSIX_free

#endif /* __LWIPOPTS_H_ */
