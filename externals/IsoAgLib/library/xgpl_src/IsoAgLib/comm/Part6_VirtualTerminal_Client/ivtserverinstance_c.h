/*
  vtserverinstance_c.h - interface class for wrapping one vtserver instance

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/
#ifndef IVT_SERVER_INSTANCE_H
#define IVT_SERVER_INSTANCE_H

// own
#include "impl/vtserverinstance_c.h"

// IsoAgLib
#include <IsoAgLib/comm/Part5_NetworkManagement/iisoitem_c.h>
#include <IsoAgLib/comm/Part5_NetworkManagement/iisoname_c.h>


namespace IsoAgLib
{

/** interface class for wrapped vtserver-instance
  */
class iVtServerInstance_c : private __IsoAgLib::VtServerInstance_c
{
public:

  bool isVtActive() const { return VtServerInstance_c::isVtActive(); }

  const iIsoName_c& getIsoName() const { return VtServerInstance_c::getIsoName().toConstIisoName_c(); }
  uint32_t          getVtHardwareDimension() const { return VtServerInstance_c::getVtHardwareDimension(); }
  uint16_t          getVtFontSizes() const { return VtServerInstance_c::getVtFontSizes(); }
  uint8_t           getVtIsoVersion() const { return VtServerInstance_c::getVtIsoVersion(); }
  uint8_t           getSkPhysical() const { return getConstVtCapabilities()->skPhysical; }
  uint8_t           getSkVirtual() const { return getConstVtCapabilities()->skVirtual; }
  uint8_t           getVtColorDepth() const { return getConstVtCapabilities()->hwGraphicType; }
  const vtState_s*  getVtState() const { return VtServerInstance_c::getVtState(); }
  const iIsoItem_c& getIsoItem() const { return VtServerInstance_c::getIsoItem().toConstIisoItem_c(); }

private:
  /** PRIVATE constructor to forbid instantiation of this interface class.
    * it can only be static_cast'ed to this class, not constructed!
    */
  iVtServerInstance_c ();

  friend class iVtClient_c;
  friend class __IsoAgLib::VtServerInstance_c;
};

} // IsoAgLib

#endif
