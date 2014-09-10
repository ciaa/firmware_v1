/*
  vtserverinstance_c.h: class for wrapping one vtserver instance

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/
#ifndef VTSERVERINSTANCE_C_H
#define VTSERVERINSTANCE_C_H

#include <IsoAgLib/comm/Part5_NetworkManagement/iidentitem_c.h>
#include "../ivttypes.h"

namespace IsoAgLib {
  class iVtServerInstance_c;
}
namespace __IsoAgLib {
  class VtClient_c;
  class VtClientConnection_c;
}


/** struct that stores the Language and Units information
  received from the LANGUAGE_PGN
*/
typedef struct localSettings_s{
  uint32_t lastReceived; /* Timestamp of last reception */
  uint32_t lastRequested;

  /* the following data is extracted from one "VT Status Message" */
  uint16_t languageCode; /* 2 ASCII chars packed into 16 bit! "en", "de", etc. */
  // Number Formats
  uint8_t  nDecimalPoint;
  uint8_t  nTimeFormat;
  // Date Formats
  uint8_t  dFormat;
  // Unit Formats
  uint8_t  uDistance;
  uint8_t  uArea;
  uint8_t  uVolume;
  uint8_t  uMass;
  uint8_t  uTemperature;
  uint8_t  uPressure;
  uint8_t  uForce;
  uint8_t  uUnitsSystem;
} localSettings_s;


namespace __IsoAgLib
{

/** class for wrapping one vtserver instance */
class VtServerInstance_c
{
private:
  /** class not copyable */
  VtServerInstance_c(const VtServerInstance_c&);
  VtServerInstance_c& operator=(const VtServerInstance_c&); 

public:
  VtServerInstance_c( const IsoItem_c& r_newItem, VtClient_c& r_isoTerminal );

  /** struct that stores the "Get Number Of Soft Keys Response",
    "Get Text Font Data Response" and "Get Hardware Response"
  */
  typedef struct vtCapabilities_s {
    uint32_t lastReceivedHardware;
    uint32_t lastRequestedHardware;
    uint8_t  hwGraphicType; // 0, 1 or 2 (2, 16 or 256 colors)

    //Bit 0 = 1 = VT has a touch screen or pointing device and supports Pointing Event message.
    //Bit 1 = 1 = VT reports drag operation via Pointing Event message (Bit 0 shall be set to 1)
    //Bit 2 = 1 = VT has multiple frequency audio output
    //Bit 3 = 1 = VT has adjustable volume audio output
    //Bit 4 = 1 = VT supports simultaneous activations of all combinations of physical soft keys (See Clause 4.6.15 Soft key and button activation)
    //Bit 5 = 1 = VT supports intermediate coordinates during a drag operation (Bit 1 shall be set to a 1)
    uint8_t  hwHardware;
    uint16_t hwWidth;   // hwWidth and
    uint16_t hwHeight;  // hwHeight have to be the same (only square mask dimension!)

    uint32_t lastReceivedFont;
    uint32_t lastRequestedFont;
    uint16_t fontSizes;
    uint8_t  fontTypes;

    uint32_t lastReceivedVersion;
    uint8_t  iso11783version;  // here the version number of the terminal gets stored as soon as the "VT Get Memory Response" was received

    uint32_t lastReceivedSoftkeys;
    uint32_t lastRequestedSoftkeys;
    uint8_t  skPhysical;
    uint8_t  skVirtual;
    uint8_t  skWidth;
    uint8_t  skHeight;

    uint8_t  bootTime;
  } vtCapabilities_s;

  virtual ~VtServerInstance_c();

  /** check if there's already been at least one vt_statusMessage in the last 3 seconds
    @return true if at least one vt_statusMessage - false if there's not yet been one or the last one is more than 3 seconds old
  */
  bool isVtActive() const;

  /** setter */
  void setLatestVtStatusData( const CanPkgExt_c& arc_data );
  void setLocalSettings( const CanPkgExt_c& arc_data );
  void setVersion( const CanPkgExt_c& arc_data );
  void setSoftKeyData( const CanPkgExt_c& arc_data );
  void setTextFontData( const CanPkgExt_c& arc_data );
  void setHardwareData( const CanPkgExt_c& arc_data );

  /** getter */
  const IsoName_c&           getIsoName()             const { return m_isoItem.isoName(); }
  uint8_t                    getVtSourceAddress()     const { return m_isoItem.nr(); }
  uint32_t                   getVtHardwareDimension() const;
  uint16_t                   getVtFontSizes()         const;
  uint8_t                    getVtIsoVersion()        const;
  vtCapabilities_s*          getVtCapabilities()      { return &ms_vtCapabilitiesA; }
  const vtCapabilities_s*    getConstVtCapabilities() const  { return &ms_vtCapabilitiesA; }
  const IsoAgLib::vtState_s* getVtState()             const  { return &ms_vtStateA; }
  localSettings_s*           getLocalSettings()       { return &ms_localSettingsA; }
  const IsoItem_c&           getIsoItem()             const { return m_isoItem; }
  bool                       isPrimaryVt()            const { return (getIsoName().funcInst() == 0); }
  
  bool receivedLocalSettings() const { return( ms_localSettingsA.lastReceived != 0 ); }
  void requestLocalSettings( IdentItem_c& identItem );

// the following define should be globally defined in the project settings...
// (currently not supported, due to multi VT enhancements)
#if 0
#ifdef USE_IOP_GENERATOR_FAKE_VT_PROPERTIES
  void fakeVtProperties (uint16_t aui16_dimension, uint16_t aui16_skWidth, uint16_t aui16_skHeight, uint8_t aui16_colorDepth, uint16_t aui16_fontSizes)
  {
    ms_vtCapabilitiesA.hwWidth = aui16_dimension;
    ms_vtCapabilitiesA.skWidth = aui16_skWidth;
    ms_vtCapabilitiesA.skHeight = aui16_skHeight;
    ms_vtCapabilitiesA.hwGraphicType = aui16_colorDepth;
    ms_vtCapabilitiesA.fontSizes = aui16_fontSizes;
  }
#endif
#endif

  /** interface convert function - avoids lots of explicit static_casts */
  IsoAgLib::iVtServerInstance_c* toIvtServerInstancePtr_c();

  /** interface convert function - avoids lots of explicit static_casts */
  IsoAgLib::iVtServerInstance_c& toIvtServerInstance_c();

private:
  const IsoItem_c& m_isoItem;

  VtClient_c& mrc_isoTerminal; // back ref.

  IsoAgLib::vtState_s ms_vtStateA; /** last "VT Status Message" */
  vtCapabilities_s ms_vtCapabilitiesA; /** color-depth, #SKs, etc. */
  localSettings_s ms_localSettingsA; /** Language, Units, Date Format */
};

} // __IsoAgLib

#endif
