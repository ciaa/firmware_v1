/*
  vtobjectlineattributes_c.cpp

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/

#include "vtobjectlineattributes_c.h"

#include "vtclient_c.h"
#include "../ivtobjectbutton_c.h"
#include "../ivtobjectmacro_c.h"


namespace __IsoAgLib {

int16_t
vtObjectLineAttributes_c::stream(uint8_t* destMemory,
                                 uint16_t maxBytes,
                                 objRange_t sourceOffset)
{
#define MACRO_vtObjectTypeA vtObjectLineAttributes_a
#define MACRO_vtObjectTypeS iVtObjectLineAttributes_s
    MACRO_streamLocalVars;
    MACRO_scaleLocalVars;
    MACRO_scaleSKLocalVars;

    if (sourceOffset == 0) { // dump out constant sized stuff
      destMemory [0] = vtObject_a->ID & 0xFF;
      destMemory [1] = vtObject_a->ID >> 8;
      destMemory [2] = 24; // Object Type = Line Attributes
      destMemory [3] = __IsoAgLib::getVtClientInstance4Comm().getClientByID (s_properties.clientId).getUserClippedColor (vtObjectLineAttributes_a->lineColour, this, IsoAgLib::LineColour);
      /** @todo SOON-174 not yet supporting different lineAttributes scaling for buttons!! */
      if (s_properties.flags & FLAG_ORIGIN_SKM) {
        destMemory [4] = (((uint32_t) vtObjectLineAttributes_a->lineWidth*factorM)/factorD) & 0xFF;
      } else {
        destMemory [4] = (((uint32_t) vtObjectLineAttributes_a->lineWidth*vtDimension)/opDimension) & 0xFF;
      }
      if ((vtObjectLineAttributes_a->lineWidth >= 1) && (destMemory [4] == 0))
      { // if downscaling line-widths of at least 1, do NEVER send 0, at least always send 1!
        destMemory [4] = 1;
      }
      destMemory [5] = vtObjectLineAttributes_a->lineArt & 0xFF;
      destMemory [6] = vtObjectLineAttributes_a->lineArt >> 8;

      destMemory [7] = vtObjectLineAttributes_a->numberOfMacrosToFollow;
      sourceOffset += 8;
      curBytes += 8;
    }

    MACRO_streamEventMacro(8);
    return curBytes;
}


vtObjectLineAttributes_c::vtObjectLineAttributes_c() {}


uint32_t
vtObjectLineAttributes_c::fitTerminal() const
{
  MACRO_localVars;
  return 8+vtObjectLineAttributes_a->numberOfMacrosToFollow*2;
}

void
vtObjectLineAttributes_c::setLineAttributes(uint8_t newLineColour, uint8_t newLineWidth, uint16_t newLineArt, bool b_updateObject, bool b_enableReplaceOfCmd) {
  if (b_updateObject) {
    saveValue8 (MACRO_getStructOffset(get_vtObjectLineAttributes_a(), lineColour), sizeof(iVtObjectLineAttributes_s), __IsoAgLib::getVtClientInstance4Comm().getClientByID (s_properties.clientId).getUserClippedColor (newLineColour, this, IsoAgLib::LineColour));
    saveValue8 (MACRO_getStructOffset(get_vtObjectLineAttributes_a(), lineWidth), sizeof(iVtObjectLineAttributes_s), newLineWidth);
    saveValue16 (MACRO_getStructOffset(get_vtObjectLineAttributes_a(), lineArt), sizeof(iVtObjectLineAttributes_s), newLineArt);
  }
  __IsoAgLib::getVtClientInstance4Comm().getClientByID (s_properties.clientId).commandHandler().sendCommandChangeLineAttributes (this, __IsoAgLib::getVtClientInstance4Comm().getClientByID (s_properties.clientId).getUserClippedColor (newLineColour, this, IsoAgLib::LineColour), newLineWidth, newLineArt, b_enableReplaceOfCmd);
}

#ifdef USE_ISO_TERMINAL_GETATTRIBUTES
uint8_t
vtObjectLineAttributes_c::updateLineColour(bool b_SendRequest)
{
  if (b_SendRequest)
    return getValue8GetAttribute(MACRO_getStructOffset(get_vtObjectLineAttributes_a(), lineColour), sizeof(iVtObjectLineAttributes_s), 1);
  else
    return getValue8(MACRO_getStructOffset(get_vtObjectLineAttributes_a(), lineColour), sizeof(iVtObjectLineAttributes_s));
}

uint8_t
vtObjectLineAttributes_c::updateLineWidth(bool b_SendRequest)
{
  if (b_SendRequest)
    return getValue8GetAttribute(MACRO_getStructOffset(get_vtObjectLineAttributes_a(), lineWidth), sizeof(iVtObjectLineAttributes_s), 2);
  else
    return getValue8(MACRO_getStructOffset(get_vtObjectLineAttributes_a(), lineWidth), sizeof(iVtObjectLineAttributes_s));
}

uint16_t
vtObjectLineAttributes_c::updateLineArt(bool b_SendRequest)
{
  if (b_SendRequest)
    return getValue16GetAttribute(MACRO_getStructOffset(get_vtObjectLineAttributes_a(), lineArt), sizeof(iVtObjectLineAttributes_s), 3);
  else
    return getValue16(MACRO_getStructOffset(get_vtObjectLineAttributes_a(), lineArt), sizeof(iVtObjectLineAttributes_s));
}

void
vtObjectLineAttributes_c::saveReceivedAttribute(uint8_t attrID, uint8_t* pui8_attributeValue)
{
  switch (attrID)
  {
    case 1: saveValue8(MACRO_getStructOffset(get_vtObjectLineAttributes_a(), lineColour), sizeof(iVtObjectLineAttributes_s), convertLittleEndianStringUi8(pui8_attributeValue)); break;
    case 2: saveValue8(MACRO_getStructOffset(get_vtObjectLineAttributes_a(), lineWidth), sizeof(iVtObjectLineAttributes_s), convertLittleEndianStringUi8(pui8_attributeValue)); break;
    case 3: saveValue16(MACRO_getStructOffset(get_vtObjectLineAttributes_a(), lineArt), sizeof(iVtObjectLineAttributes_s), convertLittleEndianStringUi16(pui8_attributeValue)); break;
    default: break;
  }
}
#endif

} // __IsoAgLib
