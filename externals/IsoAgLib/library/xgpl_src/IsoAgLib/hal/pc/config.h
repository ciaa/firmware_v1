/*
  config.h: system dependent configs, defines and includes

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/
#ifndef _HAL_PC_CONFIG_H_
#define _HAL_PC_CONFIG_H_


#define SYSTEM_PC
#define HAL_USE_SPECIFIC_FILTERS


#define HAL_CAN_BITRATE_LIST {10, 20, 40, 50, 100, 125, 250, 500, 1000}
#define HAL_CAN_BITRATE_CNT 9

/// define list of allowed speed settings
#define HAL_RS232_BAUDRATE_LIST {75, 600, 1200, 2400, 4800, 9600, 19200, 38400, 57600, 115200}
#define HAL_RS232_BITRATE_CNT 10


// IsoAgLib counting for BUS-NR and MsgObj starts both in C-Style with 0
// -> all needed offsets shall be added at the lowest possible layer
//    ( i.e. direct in the BIOS/OS call)
// NOTE: For PC (CAN-Server) we have basically unlimited channels,
//       hence the max. BUS number is set to some quite high value of 31.
//       Should be really enough for everybody - just like 640kB of RAM on the PC ;-)
#define HAL_CAN_MAX_BUS_NR 31

#if (!defined( OPTIMIZE_NUMBER_CONVERSIONS_FOR_BIG_ENDIAN ) && !defined( OPTIMIZE_NUMBER_CONVERSIONS_FOR_LITTLE_ENDIAN ) )
#  error "Please specify little or big endian!"
#endif

/** define uint16_t order of float: WORD_LO_HI, BYTE_HI_LO, WORD_HI_LO */
#ifndef FLOAT_WORD_ORDER
#  define FLOAT_WORD_ORDER WORD_LO_HI
#endif

#define HAL_SIZEOF_INT 4

#ifdef USE_HUGE_MEM
#  error "USE_HUGE_MEM must not be set!"
#endif
#define HUGE_MEM
#define NEAR_MEM
#define USE_NEAR_MEM


#define MAX_EEPROM_SEGMENT_SIZE 40

/** define max number of write try-cycle to EEPROM
    ( one cycle: write value, re-read, compare )
  */
#define MAX_EEPROM_WRITE_TRY_CYCLE_CNT 5

#ifndef CONFIG_HAL_PC_RTE_DEFAULT_SERVER
  #define CONFIG_HAL_PC_RTE_DEFAULT_SERVER "rte4"
#endif

/* Initialisierung Watchdog 0 */
#define WD_MAX_TIME      0//200        /* 128 ms                    */
#define WD_MIN_TIME      0      /* 0 ms                      */
#define UD_MAX           233    /* 16.7 V                    */
#define UD_MIN           100    /* 7.2 V                    */
#define CONFIG_RELAIS    255    /* Relais wird bei allen Fehlern abgeschaltet */
#define  CONFIG_RESET    0x10   /* (b 0001 0000) Reset bei WD Verzug      */


#endif
