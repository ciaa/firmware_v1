/*
  vtobjectfontattributes_c.cpp

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/

#include "../ivtobjectfontattributes_c.h"
#include "vtobjectfontattributes_c.h"
#include "vtclient_c.h"
#include "../ivtobjectbutton_c.h"
#include "../ivtobjectmacro_c.h"

namespace IsoAgLib {
// implement here a normal constructor and functions, as the compiler dislikes inlining of that simple
// cconstructor/functions direct in scope of iVtObjectFontAttributes_c
  iVtObjectFontAttributes_c::iVtObjectFontAttributes_c() : vtObjectFontAttributes_c() {}

  iVtObjectFontAttributes_c::~iVtObjectFontAttributes_c() {}

  uint16_t
      iVtObjectFontAttributes_c::getScaledWidthHeight()
  {
    return vtObjectFontAttributes_c::getScaledWidthHeight();
  }
}


namespace __IsoAgLib {


uint8_t vtObjectFontAttributes_c::marr_font2PixelDimensionTableW [15] = {6,  8,  8, 12, 16, 16, 24, 32, 32, 48, 64, 64, 96,128,128};
uint8_t vtObjectFontAttributes_c::marr_font2PixelDimensionTableH [15] = {8,  8, 12, 16, 16, 24, 32, 32, 48, 64, 64, 96,128,128,192};



int16_t
vtObjectFontAttributes_c::stream(uint8_t* destMemory,
                                 uint16_t maxBytes,
                                 objRange_t sourceOffset)
{
#define MACRO_vtObjectTypeA vtObjectFontAttributes_a
#define MACRO_vtObjectTypeS iVtObjectFontAttributes_s
  MACRO_streamLocalVars

  if (sourceOffset == 0) { // dump out constant sized stuff
    destMemory [0] = vtObject_a->ID & 0xFF;
    destMemory [1] = vtObject_a->ID >> 8;
    destMemory [2] = 23; // Object Type = Font Attributes
    destMemory [3] = __IsoAgLib::getVtClientInstance4Comm().getClientByID (s_properties.clientId).getUserClippedColor (vtObjectFontAttributes_a->fontColour, this, IsoAgLib::FontColour);
    destMemory [4] = mui8_fontSizeScaled; // size() must have been called before to prepare!!!!
    destMemory [5] = vtObjectFontAttributes_a->fontType;
    destMemory [6] = vtObjectFontAttributes_a->fontStyle & __IsoAgLib::getVtClientInstance4Comm().getClientByID (s_properties.clientId).getVtServerInst().getVtCapabilities()->fontTypes;
    destMemory [7] = vtObjectFontAttributes_a->numberOfMacrosToFollow;
    sourceOffset += 8;
    curBytes += 8;
  }

  MACRO_streamEventMacro(8);
  return curBytes;
}


vtObjectFontAttributes_c::vtObjectFontAttributes_c()
: mui8_fontSizeScaled( 0xFF ) // set mui8_fontSizeScaled to "not yet calculated"
{}


vtObjectFontAttributes_c::~vtObjectFontAttributes_c()
{}


uint32_t
vtObjectFontAttributes_c::fitTerminal() const
{
  MACRO_localVars;

  mui8_fontSizeScaled = 0xFF;
  // Recalc mui8_fontSizeScaled (with 0xFF it will re-calc the font size)
  calcScaledFontDimension();

  return 8+vtObjectFontAttributes_a->numberOfMacrosToFollow*2;
}


uint16_t
vtObjectFontAttributes_c::getScaledWidthHeight()
{
  calcScaledFontDimension(); // idempotent! method doesn't calc more than once, so no problem to call...

  if (mui8_fontSizeScaled >= 15)
    return ((0<<6) | (0));
  else
    return ((marr_font2PixelDimensionTableW [mui8_fontSizeScaled] << 8) | (marr_font2PixelDimensionTableH [mui8_fontSizeScaled]));
}


void
vtObjectFontAttributes_c::calcScaledFontDimension() const
{
  MACRO_localVars;
  MACRO_scaleLocalVars;
  MACRO_scaleSKLocalVars;

  // you can call it idempotent!!
  if (mui8_fontSizeScaled != 0xFF)
    return; // already calculated

  mui8_fontSizeScaled = vtObjectFontAttributes_a->fontSize;
  if (mui8_fontSizeScaled > (15-1)) mui8_fontSizeScaled = (15-1);

  uint32_t width, height;
  uint8_t wIndex=0, hIndex=0;
  if ((s_properties.flags & FLAG_ORIGIN_SKM) || p_parentButtonObject) {
    width = (((uint32_t) factorM * (marr_font2PixelDimensionTableW [mui8_fontSizeScaled]) <<10)/factorD); // (8 bit shifted fixed floating)
    height= (((uint32_t) factorM * (marr_font2PixelDimensionTableH [mui8_fontSizeScaled]) <<10)/factorD); // (8 bit shifted fixed floating)
  } else {
    width = (((uint32_t) vtDimension * (marr_font2PixelDimensionTableW [mui8_fontSizeScaled]) <<10)/opDimension); // (8 bit shifted fixed floating)
    height= (((uint32_t) vtDimension * (marr_font2PixelDimensionTableH [mui8_fontSizeScaled]) <<10)/opDimension); // (8 bit shifted fixed floating)
  }

  /** @todo SOON-174 maybe keep aspect ratio?? Make it a user-flag on registerIsoObjectPool? Or put it into the objects itself?? */
  // now get the lower possible size...
  int i, j;
  for (i=14; i>=0; i--) {
    if (((uint32_t (marr_font2PixelDimensionTableW [i])) << 10) <= width) {
      wIndex = i;
      break;
    }
  }
  for (j=14; j>=0; j--) {
    if (((uint32_t (marr_font2PixelDimensionTableH [j])) << 10) <= height) {
      hIndex = j;
      break;
    }
  }
  if ((i < 0) || (j < 0))
  { // too small font, smaller than 6x8... ==> take 6x8
    mui8_fontSizeScaled = 0;
  }
  else
  { // match indices together... take the lowest one, that'll do!
    if (wIndex < hIndex)
      mui8_fontSizeScaled = wIndex;
    else
      mui8_fontSizeScaled = hIndex;
  }

  /// Always check if the font is available!
  while (!(__IsoAgLib::getVtClientInstance4Comm().getClientByID (s_properties.clientId).getVtServerInst().getVtFontSizes() & (1 << mui8_fontSizeScaled))) {
    mui8_fontSizeScaled--; // try a smaller font, but "6x8" should be there in any way, 'cause we set it in processMsg!!
  }
}

void
vtObjectFontAttributes_c::setFontAttributes(uint8_t newFontColour, uint8_t newFontSize, uint8_t newFontType, uint8_t newFontStyle, bool b_updateObject, bool b_enableReplaceOfCmd)
{
  if (b_updateObject) {
    saveValue8 (MACRO_getStructOffset(get_vtObjectFontAttributes_a(), fontColour), sizeof(iVtObjectFontAttributes_s), __IsoAgLib::getVtClientInstance4Comm().getClientByID (s_properties.clientId).getUserClippedColor (newFontColour, this, IsoAgLib::FontColour));
    saveValue8 (MACRO_getStructOffset(get_vtObjectFontAttributes_a(), fontSize),   sizeof(iVtObjectFontAttributes_s), newFontSize);
    saveValue8 (MACRO_getStructOffset(get_vtObjectFontAttributes_a(), fontType),   sizeof(iVtObjectFontAttributes_s), newFontType);
    saveValue8 (MACRO_getStructOffset(get_vtObjectFontAttributes_a(), fontStyle),  sizeof(iVtObjectFontAttributes_s), newFontStyle);
  }
  VtClientConnection_c& vtCSC = __IsoAgLib::getVtClientInstance4Comm().getClientByID (s_properties.clientId);
  vtCSC.commandHandler().sendCommandChangeFontAttributes(
    this, vtCSC.getUserClippedColor (newFontColour, this, IsoAgLib::FontColour),
    newFontSize, newFontType, newFontStyle, b_enableReplaceOfCmd );
}

#ifdef USE_ISO_TERMINAL_GETATTRIBUTES
uint8_t
vtObjectFontAttributes_c::updateFontColour(bool b_SendRequest)
{
  if (b_SendRequest)
    return getValue8GetAttribute(MACRO_getStructOffset(get_vtObjectFontAttributes_a(), fontColour), sizeof(iVtObjectFontAttributes_s), 1);
  else
    return getValue8(MACRO_getStructOffset(get_vtObjectFontAttributes_a(), fontColour), sizeof(iVtObjectFontAttributes_s));
}

uint8_t
vtObjectFontAttributes_c::updateFontSize(bool b_SendRequest)
{
  if (b_SendRequest)
    return getValue8GetAttribute(MACRO_getStructOffset(get_vtObjectFontAttributes_a(), fontSize), sizeof(iVtObjectFontAttributes_s), 2);
  else
    return getValue8(MACRO_getStructOffset(get_vtObjectFontAttributes_a(), fontSize), sizeof(iVtObjectFontAttributes_s));
}

uint8_t
vtObjectFontAttributes_c::updateFontType(bool b_SendRequest)
{
  if (b_SendRequest)
    return getValue8GetAttribute(MACRO_getStructOffset(get_vtObjectFontAttributes_a(), fontType), sizeof(iVtObjectFontAttributes_s), 3);
  else
    return getValue8(MACRO_getStructOffset(get_vtObjectFontAttributes_a(), fontType), sizeof(iVtObjectFontAttributes_s));
}

uint8_t
vtObjectFontAttributes_c::updateFontStyle(bool b_SendRequest)
{
  if (b_SendRequest)
    return getValue8GetAttribute(MACRO_getStructOffset(get_vtObjectFontAttributes_a(), fontStyle), sizeof(iVtObjectFontAttributes_s), 4);
  else
    return getValue8(MACRO_getStructOffset(get_vtObjectFontAttributes_a(), fontStyle), sizeof(iVtObjectFontAttributes_s));
}

void
vtObjectFontAttributes_c::saveReceivedAttribute(uint8_t attrID, uint8_t* pui8_attributeValue)
{
  switch (attrID)
  {
    case 1: saveValue8(MACRO_getStructOffset(get_vtObjectFontAttributes_a(), fontColour), sizeof(iVtObjectFontAttributes_s), convertLittleEndianStringUi8(pui8_attributeValue)); break;
    case 2: saveValue8(MACRO_getStructOffset(get_vtObjectFontAttributes_a(), fontSize), sizeof(iVtObjectFontAttributes_s), convertLittleEndianStringUi8(pui8_attributeValue)); break;
    case 3: saveValue8(MACRO_getStructOffset(get_vtObjectFontAttributes_a(), fontType), sizeof(iVtObjectFontAttributes_s), convertLittleEndianStringUi8(pui8_attributeValue)); break;
    case 4: saveValue8(MACRO_getStructOffset(get_vtObjectFontAttributes_a(), fontStyle), sizeof(iVtObjectFontAttributes_s), convertLittleEndianStringUi8(pui8_attributeValue)); break;
    default: break;
  }
}
#endif

} // __IsoAgLib

