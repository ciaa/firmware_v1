/*
  hal_typedef.h: central typedef

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/
#ifndef __HAL_TYPEDEF_H__
#define __HAL_TYPEDEF_H__


#ifndef _ISOAGLIB_CONFIG_H_
#error Please include isoaglib_config.h!
#endif

#ifdef HAL_PATH_ISOAGLIB
#  define _hal_typedef_header_ <HAL_PATH_ISOAGLIB/typedef.h>
#  include _hal_typedef_header_
#else
#  error "No HAL_PATH_ISOAGLIB set. This is a mandatory module!"
#endif


#endif
