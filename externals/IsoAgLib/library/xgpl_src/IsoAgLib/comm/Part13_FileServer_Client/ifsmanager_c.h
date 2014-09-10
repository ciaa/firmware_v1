/*
  ifsmanager_c.h

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/
#ifndef IFSMANAGER_C_H
#define IFSMANAGER_C_H

//base class
#include "impl/fsmanager_c.h"

//IsoAgLib includes
#include "ifsclient_c.h"

// Begin Namespace IsoAgLib
namespace IsoAgLib {

class iFsManager_c : private __IsoAgLib::FsManager_c
{
  public :
    /**
      * Registered a client to the fsmanager and returnes the clients fsclientservercommunication used for interaction with a
      * fileserver.
      */
    iFsClientServerCommunication_c *registerFsClient(iIdentItem_c &rc_identItem, iFsClient_c &rc_fsClient, const iFsWhitelistList &v_fsWhitelist)
    {
      return __IsoAgLib::FsManager_c::registerFsClient(static_cast<__IsoAgLib::IdentItem_c&>(rc_identItem), (rc_fsClient), v_fsWhitelist)->toInterfacePointer();
    }

    void deregisterFsClient(iIdentItem_c &rc_identItem)
    {
      return __IsoAgLib::FsManager_c::deregisterFsClient(static_cast<__IsoAgLib::IdentItem_c&>(rc_identItem));
    }

  private:
    /** allow getIFsManagerInstance() access to shielded base class.
      *  otherwise __IsoAgLib::getfsManagerInstance() wouldn't be accepted by compiler
      */
    #if defined(PRT_INSTANCE_CNT) && (PRT_INSTANCE_CNT > 1)
      friend iFsManager_c& getIFsManagerInstance (uint8_t rui8_instance);
    #else
      friend iFsManager_c& getIFsManagerInstance (void);
    #endif
};

#if defined(PRT_INSTANCE_CNT) && (PRT_INSTANCE_CNT > 1)
  /** C-style function, to get access to the unique fsManagerInstance_c singleton instance
    * if more than one CAN BUS is used for IsoAgLib, an index must be given to select the wanted BUS
    */
  inline iFsManager_c& getIFsManagerInstance (uint8_t rui8_instance = 0)
  { return static_cast<iFsManager_c&>(__IsoAgLib::getFsManagerInstance(rui8_instance)); }
#else
  /** C-style function, to get access to the unique fsManagerInstance_c singleton instance */
  inline iFsManager_c& getIFsManagerInstance (void)
  { return static_cast<iFsManager_c&>(__IsoAgLib::getFsManagerInstance()); }
#endif

//End namespace
}


#endif
