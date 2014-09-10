/*
  hal_config.h: include dependent on used target (defined in
    IsoAgLib/isoaglib_config.h) the suitable HAL specific header for
    central configuration settings

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/

/* ************************************************************ */
/** \file IsoAgLib/hal/hal_config.h
  * include dependent on used target (defined in
    IsoAgLib/isoaglib_config.h) the suitable HAL
    specific header for central configuration settings.
*/
/* ************************************************************ */
#ifndef _HAL_INDEPENDENT_CONFIG_H_
#define _HAL_INDEPENDENT_CONFIG_H_


#ifdef HAL_PATH_ISOAGLIB
#  define _hal_config_header_ <HAL_PATH_ISOAGLIB/config.h>
#  include _hal_config_header_
#else
#  error "No HAL_PATH_ISOAGLIB set. This is mandatory! Make sure you don't include 'hal_config.h' directly. Include 'isoaglib_config.h' instead!"
#endif


#endif
