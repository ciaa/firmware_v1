/*
  hal_datastreams.h:
    include dependent on used target (defined in
    IsoAgLib/isoaglib_config.h) the suitable HAL specific header for
    datastreams input and output

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/

/* ************************************************************ */
/** \file
 * 
  * include dependent on used target (defined in
	  IsoAgLib/isoaglib_config.h) the suitable HAL
		specific header for datastreams output control.
*/
/* ************************************************************ */
#ifndef _HAL_INDEPENDENT_DATASTREAMS_H_
#define _HAL_INDEPENDENT_DATASTREAMS_H_

#include <IsoAgLib/isoaglib_config.h>
#include <IsoAgLib/hal/hal_config.h>


typedef enum
{
	StreamAte    = 0x01,
	StreamApp    = 0x02,
	StreamTrunc  = 0x04,
	StreamIn     = 0x08,
	StreamOut    = 0x10,
	StreamBinary = 0x20
} FileMode_t;


#ifdef HAL_PATH_SUPPLEMENTARY_DATASTREAMS
#  define _hal_datastreams_header_ <HAL_PATH_SUPPLEMENTARY_DATASTREAMS/datastreams.h>
#  include _hal_datastreams_header_
#endif


#endif
