/*
  iisbstatehandler_c.h: callbacks for ISB state changes

  (C) Copyright 2013 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/
#ifndef ISBSTATEHANDLER_C_H
#define ISBSTATEHANDLER_C_H

#include <IsoAgLib/isoaglib_config.h>


namespace IsoAgLib {


class iIsbStateHandler_c
{
public:
  iIsbStateHandler_c() {}
  virtual ~iIsbStateHandler_c() {}

  //! Note: A call with "false" does not mean to re-activate the operations
  //!       It only notifies that the operations can be re-eactivated via "other" means now.
  virtual void stopAllImplementOperations( bool ) = 0;

  virtual void numberOfServers( unsigned ) = 0;
};


}

#endif
 