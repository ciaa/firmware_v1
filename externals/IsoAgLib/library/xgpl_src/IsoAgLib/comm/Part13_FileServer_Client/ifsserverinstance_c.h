/*
  ifsserverinstance_c.h

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/
#ifndef IFSSERVERINSTANCE_C_H
#define IFSSERVERINSTANCE_C_H

#include "impl/fsserverinstance_c.h"

//start namespace IsoAgLib
namespace IsoAgLib
{

class iFsClientServerCommunication_c;

class iFsServerInstance_c : private __IsoAgLib::FsServerInstance_c
{
private:
  /** only declared, never defined -> not constructable */
  iFsServerInstance_c();

  friend class __IsoAgLib::FsServerInstance_c;
  friend class iFsClientServerCommunication_c;
};

//end namespace
}

#endif
