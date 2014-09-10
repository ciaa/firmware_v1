/*
  ivtclientconnection.h: Class for managing a connection

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/
#ifndef IVTCLIENTCONNECTION_H
#define IVTCLIENTCONNECTION_H

#include "impl/vtclientconnection_c.h"
#include "ivtclient_c.h"
#include "ivtserverinstance_c.h"

namespace IsoAgLib {

class iVtClientConnection_c : private __IsoAgLib::VtClientConnection_c
{
public:

  bool isVtActive() { return VtClientConnection_c::isVtActive(); }

  /* enable/disable sending of commands to the VT over the ISOBUS.
     defaults to true, can be temporarily switched OFF (false) to locally
     modify the objects, partial pool update them and then switched
     ON (true) again. */     
  void sendCommandsToBus( bool commandsToBus );

  bool sendCommandChangeActiveMask       (uint16_t aui16_objectUid, uint16_t aui16_maskId, bool b_enableReplaceOfCmd = true );
  bool sendCommandChangeNumericValue     (uint16_t aui16_objectUid, uint8_t byte1, uint8_t byte2, uint8_t byte3, uint8_t byte4, bool b_enableReplaceOfCmd=true);
  bool sendCommandChangeAttribute        (uint16_t aui16_objectUid, uint8_t attrId, uint8_t byte1, uint8_t byte2, uint8_t byte3, uint8_t byte4, bool b_enableReplaceOfCmd=true);
  bool sendCommandChangeSoftKeyMask      (uint16_t aui16_objectUid, uint8_t maskType, uint16_t newSoftKeyMaskID, bool b_enableReplaceOfCmd=true);
  bool sendCommandChangeStringValue      (uint16_t aui16_objectUid, const char* apc_newValue, uint16_t overrideSendLength, bool b_enableReplaceOfCmd=true);
  bool sendCommandChangeChildPosition    (uint16_t aui16_objectUid, uint16_t aui16_childObjectUid, int16_t x, int16_t y, bool b_enableReplaceOfCmd=true);
  bool sendCommandChangeChildLocation    (uint16_t aui16_objectUid, uint16_t aui16_childObjectUid, int16_t dx, int16_t dy, bool b_enableReplaceOfCmd=true);
  bool sendCommandChangeBackgroundColour (uint16_t aui16_objectUid, uint8_t newColour,  bool b_enableReplaceOfCmd=true);
  bool sendCommandChangeSize             (uint16_t aui16_objectUid, uint16_t newWidth, uint16_t newHeight, bool b_enableReplaceOfCmd=true);
  bool sendCommandChangeFillAttributes   (uint16_t aui16_objectUid, uint8_t newFillType, uint8_t newFillColour, IsoAgLib::iVtObjectPictureGraphic_c* newFillPatternObject, bool b_enableReplaceOfCmd=true);
  bool sendCommandChangeFontAttributes   (uint16_t aui16_objectUid, uint8_t newFontColour, uint8_t newFontSize, uint8_t newFontType, uint8_t newFontStyle, bool b_enableReplaceOfCmd=true);
  bool sendCommandChangeLineAttributes   (uint16_t aui16_objectUid, uint8_t newLineColour, uint8_t newLineWidth, uint16_t newLineArt, bool b_enableReplaceOfCmd=true);
  bool sendCommandHideShow               (uint16_t aui16_objectUid, uint8_t b_hideOrShow, bool b_enableReplaceOfCmd=true);
  
  unsigned getCommandQueueSize() const { return commandHandler().getQueueSize(); }

  //! @param versionLabel7chars == NULL: Use VersionLabel used for Uploading/Loading (must be given at init!)
  //!                                    This includes the language-code for multi-language pools!
  //!        versionLabel7chars != NULL: Use VersionLabel given. Must be 7 characters!
  bool sendNonVolatileDeleteVersion( const char* versionLabel7chars );
  bool sendCommandEsc (bool b_enableReplaceOfCmd=true);
  bool sendCommandUpdateObjectPool (IsoAgLib::iVtObject_c** rppc_vtObjects, uint16_t aui16_numObjects);

  uint8_t getClientId() const { return VtClientConnection_c::getClientId(); }
  bool isClientActive() const { return VtClientConnection_c::isClientActive(); }

  iIdentItem_c& getIdentItem() const { return static_cast<IsoAgLib::iIdentItem_c&>(VtClientConnection_c::getIdentItem()); }
  bool connectedToVtServer() const   { return VtClientConnection_c::connectedToVtServer(); }
  /** ATTENTION: Please assure "connectedToVtServer()" before getting this reference */
  iVtServerInstance_c* getVtServerInstPtr() const { return (VtClientConnection_c::getVtServerInstPtr() != NULL) ? VtClientConnection_c::getVtServerInst().toIvtServerInstancePtr_c() : NULL; }
  iVtServerInstance_c& getVtServerInst() const    { return VtClientConnection_c::getVtServerInst().toIvtServerInstance_c(); }

  /**
   * Start "move to next VT" procedure by sending the "delete object pool"
   * 
   * return TRUE:  message sending was successfully
   * return FALSE: message sending failed
   *               reasons: a) sendCommandsToBus(false) is active
   *                        b) no connection to VT server
   **/
  bool moveToNextVt() { return VtClientConnection_c::moveToNextVt(); }

  // Note: This function will clear the stored timed out command after returning it!
  // @return 0x00: No command timed (or already checked and reset)
  //      != 0x00: The command that timed out and was the reason for a reconnect!
  uint8_t getAndResetLastTimedOutCommand() { return VtClientConnection_c::getAndResetLastTimedOutCommand(); }

private:
  iVtClientConnection_c();

  friend class iVtClient_c;
  friend class __IsoAgLib::VtClientConnection_c;
};


inline bool
iVtClientConnection_c::sendNonVolatileDeleteVersion( const char* versionLabel7chars )
{
  if( versionLabel7chars == NULL )
  {
    isoaglib_header_assert( uploadPoolState().versionLabel() );
    return commandHandler().sendNonVolatileDeleteVersion( uploadPoolState().versionLabel() );
  }
  else
    return commandHandler().sendNonVolatileDeleteVersion( versionLabel7chars ); 
}

inline void
iVtClientConnection_c::sendCommandsToBus( bool commandsToBus ) 
{
  return commandHandler().sendCommandsToBus( commandsToBus );
}

inline bool
iVtClientConnection_c::sendCommandUpdateObjectPool (IsoAgLib::iVtObject_c** rppc_vtObjects, uint16_t aui16_numObjects)
{
  return commandHandler().sendCommandUpdateObjectPool (rppc_vtObjects, aui16_numObjects);
}

inline bool 
iVtClientConnection_c::sendCommandChangeActiveMask (uint16_t aui16_objectUid, uint16_t aui16_maskId, bool b_enableReplaceOfCmd)
{
  return commandHandler().sendCommandChangeActiveMask( aui16_objectUid, aui16_maskId, b_enableReplaceOfCmd );
}

inline bool
iVtClientConnection_c::sendCommandChangeNumericValue (uint16_t aui16_objectUid, uint8_t byte1, uint8_t byte2, uint8_t byte3, uint8_t byte4, bool b_enableReplaceOfCmd)
{
  return commandHandler().sendCommandChangeNumericValue(aui16_objectUid, byte1, byte2, byte3, byte4, b_enableReplaceOfCmd);
}

inline bool 
iVtClientConnection_c::sendCommandChangeAttribute (uint16_t aui16_objectUid, uint8_t attrId, uint8_t byte1, uint8_t byte2, uint8_t byte3, uint8_t byte4, bool b_enableReplaceOfCmd)
{
  return commandHandler().sendCommandChangeAttribute(aui16_objectUid, attrId, byte1, byte2, byte3, byte4, b_enableReplaceOfCmd);
}

inline bool 
iVtClientConnection_c::sendCommandChangeSoftKeyMask  (uint16_t aui16_objectUid, uint8_t maskType, uint16_t newSoftKeyMaskID, bool b_enableReplaceOfCmd)
{
  return commandHandler().sendCommandChangeSoftKeyMask(aui16_objectUid, maskType, newSoftKeyMaskID, b_enableReplaceOfCmd);
}

inline bool
iVtClientConnection_c::sendCommandChangeStringValue  (uint16_t aui16_objectUid, const char* apc_newValue, uint16_t overrideSendLength, bool b_enableReplaceOfCmd)
{
  return commandHandler().sendCommandChangeStringValue(aui16_objectUid, apc_newValue, overrideSendLength, b_enableReplaceOfCmd);
}

inline bool
iVtClientConnection_c::sendCommandChangeChildPosition (uint16_t aui16_objectUid, uint16_t aui16_childObjectUid, int16_t x, int16_t y, bool b_enableReplaceOfCmd)
{
  return commandHandler().sendCommandChangeChildPosition(aui16_objectUid, aui16_childObjectUid, x, y, b_enableReplaceOfCmd);
}

inline bool
iVtClientConnection_c::sendCommandChangeChildLocation (uint16_t aui16_objectUid, uint16_t aui16_childObjectUid, int16_t dx, int16_t dy, bool b_enableReplaceOfCmd)
{
  return commandHandler().sendCommandChangeChildLocation(aui16_objectUid, aui16_childObjectUid, dx, dy, b_enableReplaceOfCmd);
}

inline bool
iVtClientConnection_c::sendCommandChangeBackgroundColour (uint16_t aui16_objectUid, uint8_t newColour,  bool b_enableReplaceOfCmd)
{
  return commandHandler().sendCommandChangeBackgroundColour(aui16_objectUid, newColour,  b_enableReplaceOfCmd);
}

inline bool
iVtClientConnection_c::sendCommandChangeSize             (uint16_t aui16_objectUid, uint16_t newWidth, uint16_t newHeight, bool b_enableReplaceOfCmd)
{
  return commandHandler().sendCommandChangeSize(aui16_objectUid, newWidth, newHeight, b_enableReplaceOfCmd);
}

inline bool
iVtClientConnection_c::sendCommandChangeFillAttributes (uint16_t aui16_objectUid, uint8_t newFillType, uint8_t newFillColour, IsoAgLib::iVtObjectPictureGraphic_c* newFillPatternObject, bool b_enableReplaceOfCmd)
{
  return commandHandler().sendCommandChangeFillAttributes(aui16_objectUid, newFillType, newFillColour, newFillPatternObject, b_enableReplaceOfCmd);
}

inline bool
iVtClientConnection_c::sendCommandChangeFontAttributes (uint16_t aui16_objectUid, uint8_t newFontColour, uint8_t newFontSize, uint8_t newFontType, uint8_t newFontStyle, bool b_enableReplaceOfCmd)
{
  return commandHandler().sendCommandChangeFontAttributes(aui16_objectUid, newFontColour, newFontSize, newFontType, newFontStyle, b_enableReplaceOfCmd);
}

inline bool
iVtClientConnection_c::sendCommandChangeLineAttributes (uint16_t aui16_objectUid, uint8_t newLineColour, uint8_t newLineWidth, uint16_t newLineArt, bool b_enableReplaceOfCmd)
{
  return commandHandler().sendCommandChangeLineAttributes(aui16_objectUid, newLineColour, newLineWidth, newLineArt, b_enableReplaceOfCmd);
}

inline bool
iVtClientConnection_c::sendCommandHideShow (uint16_t aui16_objectUid, uint8_t b_hideOrShow, bool b_enableReplaceOfCmd)
{
  return commandHandler().sendCommandHideShow(aui16_objectUid, b_hideOrShow, b_enableReplaceOfCmd);
}

inline bool
iVtClientConnection_c::sendCommandEsc (bool b_enableReplaceOfCmd)
{
  return commandHandler().sendCommandEsc(b_enableReplaceOfCmd);
}


} // IsoAgLib

#endif
