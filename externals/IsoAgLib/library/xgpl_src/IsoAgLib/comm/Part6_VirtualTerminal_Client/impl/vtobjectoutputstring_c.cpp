/*
  vtobjectoutputstring_c.cpp

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/

#include "../ivtobjectoutputstring_c.h"
#include "vtobjectoutputstring_c.h"

#ifdef USE_VTOBJECT_outputstring
#include "../ivtobjectfontattributes_c.h"
#include "../ivtobjectbutton_c.h"
#include "../ivtobjectstringvariable_c.h"
#include "../ivtobjectmacro_c.h"
#include <IsoAgLib/util/impl/util_funcs.h>
#include "vtclient_c.h"


namespace IsoAgLib {
  // implement here a normal constructor/destructor, as the compiler dislikes inlining of that simple
  // constructor/destructor direct in scope of iVtObjectOutputString_c
  iVtObjectOutputString_c::iVtObjectOutputString_c() {}
  iVtObjectOutputString_c::~iVtObjectOutputString_c(){}
}


namespace __IsoAgLib {

int16_t
vtObjectOutputString_c::stream(uint8_t* destMemory,
                               uint16_t maxBytes,
                               objRange_t sourceOffset)
{
#define MACRO_vtObjectTypeA vtObjectOutputString_a
#define MACRO_vtObjectTypeS iVtObjectOutputString_s
    MACRO_streamLocalVars;
    MACRO_scaleLocalVars;
    MACRO_scaleSKLocalVars;

    if (sourceOffset == 0) { // dump out constant sized stuff
      destMemory [0] = vtObject_a->ID & 0xFF;
      destMemory [1] = vtObject_a->ID >> 8;
      destMemory [2] = 11; // Object Type = Output String
      if ((s_properties.flags & FLAG_ORIGIN_SKM) || p_parentButtonObject) {
        destMemory [3] = (((uint32_t) vtObjectOutputString_a->width*factorM)/factorD) & 0xFF;
        destMemory [4] = (((uint32_t) vtObjectOutputString_a->width*factorM)/factorD) >> 8;
        destMemory [5] = (((uint32_t) vtObjectOutputString_a->height*factorM)/factorD) & 0xFF;
        destMemory [6] = (((uint32_t) vtObjectOutputString_a->height*factorM)/factorD) >> 8;
      } else {
        destMemory [3] = (((uint32_t) vtObjectOutputString_a->width*vtDimension)/opDimension) & 0xFF;
        destMemory [4] = (((uint32_t) vtObjectOutputString_a->width*vtDimension)/opDimension) >> 8;
        destMemory [5] = (((uint32_t) vtObjectOutputString_a->height*vtDimension)/opDimension) & 0xFF;
        destMemory [6] = (((uint32_t) vtObjectOutputString_a->height*vtDimension)/opDimension) >> 8;
      }
      destMemory [7] = __IsoAgLib::getVtClientInstance4Comm().getClientByID (s_properties.clientId).getUserClippedColor (vtObjectOutputString_a->backgroundColour, this, IsoAgLib::BackgroundColour);
      destMemory [8] = vtObjectOutputString_a->fontAttributes->getID() & 0xFF;
      destMemory [9] = vtObjectOutputString_a->fontAttributes->getID() >> 8;
      destMemory [10] = vtObjectOutputString_a->options;
      if (vtObjectOutputString_a->variableReference != NULL) {
        destMemory [11] = vtObjectOutputString_a->variableReference->getID() & 0xFF;
        destMemory [12] = vtObjectOutputString_a->variableReference->getID() >> 8;
      } else {
        destMemory [11] = 0xFF;
        destMemory [12] = 0xFF;
      }
      destMemory [13] = vtObjectOutputString_a->horizontalJustification;
      destMemory [14] = vtObjectOutputString_a->length & 0xFF;
      destMemory [15] = vtObjectOutputString_a->length >> 8;
      sourceOffset += 16;
      curBytes += 16;
    }

    while ((sourceOffset >= 16U) && (sourceOffset < (16U+vtObjectOutputString_a->length)) && ((curBytes+1) <= maxBytes)) {
      if (vtObjectOutputString_a->value == NULL)
        destMemory [curBytes] = 0x00;
      else
        destMemory [curBytes] = vtObjectOutputString_a->value [sourceOffset-16];
      curBytes++;
      sourceOffset++;
    }

    if ((sourceOffset == (16U + vtObjectOutputString_a->length)) && ((curBytes+1) <= maxBytes)) {
      destMemory [curBytes] = vtObjectOutputString_a->numberOfMacrosToFollow;
      curBytes++;
      sourceOffset++;
    }

    MACRO_streamEventMacro(17U+vtObjectOutputString_a->length);
    return curBytes;
}


vtObjectOutputString_c::vtObjectOutputString_c() {}
vtObjectOutputString_c::~vtObjectOutputString_c() {}


uint32_t
vtObjectOutputString_c::fitTerminal() const
{
  MACRO_localVars;
  return 17+vtObjectOutputString_a->length+vtObjectOutputString_a->numberOfMacrosToFollow*2;
}


void
vtObjectOutputString_c::setOriginSKM(bool b_SKM)
{
  MACRO_localVars;
  if (b_SKM) {
    s_properties.flags |= FLAG_ORIGIN_SKM;
    vtObjectOutputString_a->fontAttributes->setOriginSKM (b_SKM);
  }
}

void
vtObjectOutputString_c::setOriginBTN(IsoAgLib::iVtObjectButton_c* p_btn)
{
  MACRO_localVars;
  if (p_btn)
  {
    p_parentButtonObject = p_btn;
    vtObjectOutputString_a->fontAttributes->setOriginBTN (p_btn);
  }
}

#ifdef USE_VT_UNICODE_SUPPORT
void
vtObjectOutputString_c::setValueCopyUTF8 (const char* newValue, bool b_updateObject, bool b_enableReplaceOfCmd)
{
  const uint16_t cui16_strLen = (uint16_t)CNAMESPACE::strlen (newValue);
  char* pc_iso8859 = new char [cui16_strLen+1];

  const uint8_t cui8_fontType = get_vtObjectOutputString_a()->fontAttributes->get_vtObjectFontAttributes_a().fontType;

  convertStringUnicodeTo8859 (newValue, cui16_strLen, pc_iso8859, cui8_fontType);

  setValueCopy (pc_iso8859, b_updateObject, b_enableReplaceOfCmd);
  delete[] pc_iso8859;
}

void
vtObjectOutputString_c::setValueCopyUTF16 (const char* newValue, uint16_t length, bool b_updateObject, bool b_enableReplaceOfCmd)
{
  int copyLen = ( length <= get_vtObjectOutputString_a()->length) ? length : get_vtObjectOutputString_a()->length;

  // UTF-16 string must be a multiple of 2 bytes long.
  if( copyLen % 2 )
   copyLen--;

  if (b_updateObject) {
    // check if not already RAM string buffer?
    if (!(s_properties.flags & FLAG_STRING_IN_RAM)) {
      s_properties.flags |= FLAG_STRING_IN_RAM;
      // create new String buffer with same length as original one, as the size can't be changed !!
      char *newStringBuffer = new char [get_vtObjectOutputString_a()->length+1];
      saveValueP (MACRO_getStructOffset(get_vtObjectOutputString_a(), value), sizeof(iVtObjectOutputString_s), (IsoAgLib::iVtObject_c*) newStringBuffer);
    }
    char *dest = get_vtObjectOutputString_a()->value;
    const char *src = newValue;

    int i=0; for (; i<copyLen; i++) *dest++ = *src++;
    spacePadBomUTF16( dest, copyLen, get_vtObjectOutputString_a()->length );
  }

  __IsoAgLib::getVtClientInstance4Comm().getClientByID (s_properties.clientId).commandHandler().sendCommandChangeStringValueUTF16 (getID(), newValue, copyLen, b_enableReplaceOfCmd);

}
#endif


void
vtObjectOutputString_c::setValueCopy(const char* newValue, bool b_updateObject, bool b_enableReplaceOfCmd)
{
  MACRO_localVars;
  if (vtObjectOutputString_a->variableReference != NULL) {
    // register error!!
    return;
  }

  if (b_updateObject) {
    // check if not already RAM string buffer?
    if (!(s_properties.flags & FLAG_STRING_IN_RAM)) {
      s_properties.flags |= FLAG_STRING_IN_RAM;
      // create new String buffer with same length as original one, as the size can't be changed !!
      char *newStringBuffer = new char [get_vtObjectOutputString_a()->length+1];
      saveValueP (MACRO_getStructOffset(get_vtObjectOutputString_a(), value), sizeof(iVtObjectOutputString_s), (IsoAgLib::iVtObject_c*) newStringBuffer);
    }
    char *dest = get_vtObjectOutputString_a()->value;
    const char *src = newValue;
    int copyLen = (CNAMESPACE::strlen (newValue) <= get_vtObjectOutputString_a()->length) ? CNAMESPACE::strlen (newValue) : get_vtObjectOutputString_a()->length;
    int i=0; for (; i<copyLen; i++) *dest++ = *src++;
    for (; i<get_vtObjectOutputString_a()->length; i++) *dest++ = ' ';
    *dest = 0x00; // 0-termiante!
  }

  __IsoAgLib::getVtClientInstance4Comm().getClientByID (s_properties.clientId).commandHandler().sendCommandChangeStringValue (this, newValue, get_vtObjectOutputString_a()->length, b_enableReplaceOfCmd);
}


void
vtObjectOutputString_c::setValueRef(const char* newValue, bool b_updateObject, bool b_enableReplaceOfCmd)
{
  MACRO_localVars;
  if (vtObjectOutputString_a->variableReference != NULL) {
    // register error!!
    return;
  }

  if (b_updateObject) {
    // delete RAM_String first, before we lose the pointer!
    if (s_properties.flags & FLAG_STRING_IN_RAM) {
      delete (get_vtObjectOutputString_a()->value);
      s_properties.flags &= ~FLAG_STRING_IN_RAM;
    }

    saveValueP (MACRO_getStructOffset(get_vtObjectOutputString_a(), value), sizeof(iVtObjectOutputString_s), (IsoAgLib::iVtObject_c*) newValue);
  }

  setStringToStream( newValue ); // use MultiSendStreamer with mpc_stringToStream set!
  uint16_t ui16_tempLen = 0;
  if (newValue != NULL ) ui16_tempLen = (CNAMESPACE::strlen (newValue) <= get_vtObjectOutputString_a()->length) ? CNAMESPACE::strlen (newValue) : get_vtObjectOutputString_a()->length;
  setStrLenToSend( ui16_tempLen );
  __IsoAgLib::getVtClientInstance4Comm().getClientByID (s_properties.clientId).commandHandler().sendCommandChangeStringValue (this, b_enableReplaceOfCmd);
}

/** obsolete, now no more checking for length != 0, set VaRef anyway!
void
vtObjectOutputString_c::setVariableReference(IsoAgLib::iVtObjectStringVariable_c* newVariable, bool b_updateObject, bool b_enableReplaceOfCmd)
{
  MACRO_localVars;
  if (vtObjectOutputString_a->length != 0) {
    // register error!!
    return;
  }

  if (b_updateObject) saveValueP (MACRO_getStructOffset(get_vtObjectOutputString_a(), variableReference),  sizeof(iVtObjectOutputString_s), newVariable);

  uint16_t newVariableID = newVariable ? (newVariable->getID()) : 0xFFFF;
  __IsoAgLib::getVtClientInstance4Comm().getClientByID (s_properties.clientId).sendCommandChangeAttribute (this, 6, (newVariableID & 0xFF), (newVariableID >> 8), 0, 0, b_enableReplaceOfCmd);
}
*/

void
vtObjectOutputString_c::setSize(uint16_t newWidth, uint16_t newHeight, bool b_updateObject, bool b_enableReplaceOfCmd)
{
  if (b_updateObject) {
    saveValue16 (MACRO_getStructOffset(get_vtObjectOutputString_a(), width),  sizeof(iVtObjectOutputString_s), newWidth);
    saveValue16 (MACRO_getStructOffset(get_vtObjectOutputString_a(), height), sizeof(iVtObjectOutputString_s), newHeight);
  }

  __IsoAgLib::getVtClientInstance4Comm().getClientByID (s_properties.clientId).commandHandler().sendCommandChangeSize (this, newWidth, newHeight, b_enableReplaceOfCmd);
}


const char*
vtObjectOutputString_c::getString()
{
  return get_vtObjectOutputString_a()->value;
}

#ifdef USE_ISO_TERMINAL_GETATTRIBUTES
uint16_t
vtObjectOutputString_c::updateWidth(bool b_SendRequest)
{
  if (b_SendRequest)
    return getValue16GetAttribute(MACRO_getStructOffset(get_vtObjectOutputString_a(), width), sizeof(iVtObjectOutputString_s), 1);
  else
    return getValue16(MACRO_getStructOffset(get_vtObjectOutputString_a(), width), sizeof(iVtObjectOutputString_s));
}

uint16_t
vtObjectOutputString_c::updateHeight(bool b_SendRequest)
{
  if (b_SendRequest)
    return getValue16GetAttribute(MACRO_getStructOffset(get_vtObjectOutputString_a(), height), sizeof(iVtObjectOutputString_s), 2);
  else
    return getValue16(MACRO_getStructOffset(get_vtObjectOutputString_a(), height), sizeof(iVtObjectOutputString_s));
}

uint8_t
vtObjectOutputString_c::updateBackgroundColour(bool b_SendRequest)
{
  if (b_SendRequest)
    return getValue8GetAttribute(MACRO_getStructOffset(get_vtObjectOutputString_a(), backgroundColour), sizeof(iVtObjectOutputString_s), 3);
  else
    return getValue8(MACRO_getStructOffset(get_vtObjectOutputString_a(), backgroundColour), sizeof(iVtObjectOutputString_s));
}

uint16_t
vtObjectOutputString_c::updateFontAttributes(bool b_SendRequest)
{
  if (b_SendRequest)
    return getValue16GetAttribute(MACRO_getStructOffset(get_vtObjectOutputString_a(), fontAttributes), sizeof(iVtObjectOutputString_s), 4);
  else
    return getValue16(MACRO_getStructOffset(get_vtObjectOutputString_a(), fontAttributes), sizeof(iVtObjectOutputString_s));
}

uint8_t
vtObjectOutputString_c::updateOptions(bool b_SendRequest)
{
  if (b_SendRequest)
    return getValue8GetAttribute(MACRO_getStructOffset(get_vtObjectOutputString_a(), options), sizeof(iVtObjectOutputString_s), 5);
  else
    return getValue8(MACRO_getStructOffset(get_vtObjectOutputString_a(), options), sizeof(iVtObjectOutputString_s));
}

uint16_t
vtObjectOutputString_c::updateVariableReference(bool b_SendRequest)
{
  if (b_SendRequest)
    return getValue16GetAttribute(MACRO_getStructOffset(get_vtObjectOutputString_a(), variableReference), sizeof(iVtObjectOutputString_s), 6);
  else
    return getValue16(MACRO_getStructOffset(get_vtObjectOutputString_a(), variableReference), sizeof(iVtObjectOutputString_s));
}

uint8_t
vtObjectOutputString_c::updateJustification(bool b_SendRequest)
{
  if (b_SendRequest)
    return getValue8GetAttribute(MACRO_getStructOffset(get_vtObjectOutputString_a(), horizontalJustification), sizeof(iVtObjectOutputString_s), 7);
  else
    return getValue8(MACRO_getStructOffset(get_vtObjectOutputString_a(), horizontalJustification), sizeof(iVtObjectOutputString_s));
}

void
vtObjectOutputString_c::saveReceivedAttribute(uint8_t attrID, uint8_t* pui8_attributeValue)
{
  switch (attrID)
  {
    case 1: saveValue16(MACRO_getStructOffset(get_vtObjectOutputString_a(), width), sizeof(iVtObjectOutputString_s), convertLittleEndianStringUi16(pui8_attributeValue)); break;
    case 2: saveValue16(MACRO_getStructOffset(get_vtObjectOutputString_a(), height), sizeof(iVtObjectOutputString_s), convertLittleEndianStringUi16(pui8_attributeValue)); break;
    case 3: saveValue8(MACRO_getStructOffset(get_vtObjectOutputString_a(), backgroundColour), sizeof(iVtObjectOutputString_s), convertLittleEndianStringUi8(pui8_attributeValue)); break;
    case 4: saveValue16(MACRO_getStructOffset(get_vtObjectOutputString_a(), fontAttributes), sizeof(iVtObjectOutputString_s), convertLittleEndianStringUi16(pui8_attributeValue)); break;
    case 5: saveValue8(MACRO_getStructOffset(get_vtObjectOutputString_a(), options), sizeof(iVtObjectOutputString_s), convertLittleEndianStringUi8(pui8_attributeValue)); break;
    case 6: saveValue16(MACRO_getStructOffset(get_vtObjectOutputString_a(), variableReference), sizeof(iVtObjectOutputString_s), convertLittleEndianStringUi16(pui8_attributeValue)); break;
    case 7: saveValue8(MACRO_getStructOffset(get_vtObjectOutputString_a(), horizontalJustification), sizeof(iVtObjectOutputString_s), convertLittleEndianStringUi8(pui8_attributeValue)); break;
    default: break;
  }
}
#endif

} //__IsoAgLib

#endif
