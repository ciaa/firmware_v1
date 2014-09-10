/*
  vtobjectpicturegraphic_c.cpp

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/

#include "vtobjectpicturegraphic_c.h"

#include "../ivtobjectbutton_c.h"
#include "../ivtobjectmacro_c.h"
#include "vtclient_c.h"


namespace __IsoAgLib {
#if 0
#define helperForDifferentSizes(a,b,c,f,g,h,x,y,z,optionander) \
        if (vtDimension < 400) { \
          numberOfBytesInRawData = vtObjectPictureGraphic_a->a; \
          rawData = vtObjectPictureGraphic_a->x; \
          options = (vtObjectPictureGraphic_a->f & 0x03) + ((vtObjectPictureGraphic_a->f & optionander) ? 0x04 : 0x00); \
        } else if (vtDimension < 480) { \
          numberOfBytesInRawData = vtObjectPictureGraphic_a->b; \
          rawData = vtObjectPictureGraphic_a->y; \
          options = (vtObjectPictureGraphic_a->g & 0x03) + ((vtObjectPictureGraphic_a->g & optionander) ? 0x04 : 0x00); \
        } else { \
          numberOfBytesInRawData = vtObjectPictureGraphic_a->c; \
          rawData = vtObjectPictureGraphic_a->z; \
          options = (vtObjectPictureGraphic_a->h & 0x03) + ((vtObjectPictureGraphic_a->h & optionander) ? 0x04 : 0x00); \
        }

#define helperForDifferentSizesSizeOnly(a,b,c) \
        if (vtDimension < 400) { \
          numberOfBytesInRawData = vtObjectPictureGraphic_a->a; \
        } else if (vtDimension < 480) { \
          numberOfBytesInRawData = vtObjectPictureGraphic_a->b; \
        } else { \
          numberOfBytesInRawData = vtObjectPictureGraphic_a->c; \
        }
//#else
#define helperForDifferentSizes(a,b,f,g,x,y,optionander) \
        if ( ( vtDimension < 480) || ( optionander == 0x04 ) ) { \
          numberOfBytesInRawData = vtObjectPictureGraphic_a->a; \
          rawData = vtObjectPictureGraphic_a->x; \
          options = (vtObjectPictureGraphic_a->f & 0x03) + ((vtObjectPictureGraphic_a->f & optionander) ? 0x04 : 0x00); \
        } else { \
          numberOfBytesInRawData = vtObjectPictureGraphic_a->b; \
          rawData = vtObjectPictureGraphic_a->y; \
          options = (vtObjectPictureGraphic_a->g & 0x03) + ((vtObjectPictureGraphic_a->g & optionander) ? 0x04 : 0x00); \
        }

#define helperForDifferentSizesSizeOnly(a,b,col) \
        if ( ( vtDimension < 480) || ( col == 0x0 ) ) { \
          numberOfBytesInRawData = vtObjectPictureGraphic_a->a; \
        } else { \
          numberOfBytesInRawData = vtObjectPictureGraphic_a->b; \
        }
#endif

#define min(a,b) (a<b)?a:b

#define MACRO_helperForDifferentSizesSizeOnly(a) \
          numberOfBytesInRawData = vtObjectPictureGraphic_a->a;

#define MACRO_helperForDifferentSizes(a,f,x,optionander) \
          MACRO_helperForDifferentSizesSizeOnly(a) \
          rawData = vtObjectPictureGraphic_a->x; \
          options = (vtObjectPictureGraphic_a->f & 0x03) + ((vtObjectPictureGraphic_a->f & optionander) ? 0x04 : 0x00); /* get the right RLE 1/4/8 bit to bit 2 when streaming! */

#define MACRO_calculate_ui8_graphicType \
          ui8_graphicType = ( min (__IsoAgLib::getVtClientInstance4Comm().getClientByID (s_properties.clientId).getVtServerInst().getVtCapabilities()->hwGraphicType, vtObjectPictureGraphic_a->format) ); \
          /* If 16-color bitmap is not specified, take the 2-color version. -That's the only exception! */ \
          if ((ui8_graphicType == 1) && (vtObjectPictureGraphic_a->rawData1 == NULL)) ui8_graphicType = 0;

#define MACRO_CheckFixedBitmapsLoop_start \
    /* See if we have colorDepth of VT */ \
    uint8_t vtDepth = __IsoAgLib::getVtClientInstance4Comm().getClientByID (s_properties.clientId).getVtServerInst().getVtCapabilities()->hwGraphicType; \
    /* Check for 100%-matching fixedBitmaps first */ \
    bool b_foundFixedBitmap = false; \
    for (int fixNr=0; fixNr<vtObjectPictureGraphic_a->numberOfFixedBitmapsToFollow; fixNr++) { \
      /* Matching means only match in WIDTH, not (yet) HEIGHT - and in Colourdepth! */ \
      if ( (width == vtObjectPictureGraphic_a->fixedBitmapsToFollow [fixNr].actualWidth) \
      && (vtObjectPictureGraphic_a->fixedBitmapsToFollow [fixNr].formatoptions >> 6 == vtDepth) ) {
// here user can insert code into the loop.
#define MACRO_CheckFixedBitmapsLoop_end \
        b_foundFixedBitmap = true; \
        break; \
      } \
    }

#define MACRO_calculateRequestedSize \
  uint16_t width; \
  if ((s_properties.flags & FLAG_ORIGIN_SKM) || p_parentButtonObject) { \
    width = (((uint32_t) vtObjectPictureGraphic_a->width * factorM)/factorD); \
  } else { \
    width = (((uint32_t) vtObjectPictureGraphic_a->width * vtDimension) /opDimension); \
  }


int16_t
vtObjectPictureGraphic_c::stream(uint8_t* destMemory, uint16_t maxBytes, objRange_t sourceOffset)
{
#define MACRO_vtObjectTypeA vtObjectPictureGraphic_a
#define MACRO_vtObjectTypeS iVtObjectPictureGraphic_s
    MACRO_streamLocalVars;
    MACRO_scaleLocalVars;
    MACRO_scaleSKLocalVars;

    uint8_t ui8_graphicType = 0;

    const HUGE_MEM uint8_t* rawData = NULL;
    uint32_t numberOfBytesInRawData = 0;
    uint16_t actualWidth = 0;
    uint16_t actualHeight = 0;
    uint8_t options = 0;

    MACRO_calculateRequestedSize

    MACRO_CheckFixedBitmapsLoop_start
        rawData = vtObjectPictureGraphic_a->fixedBitmapsToFollow [fixNr].rawData;
        numberOfBytesInRawData = vtObjectPictureGraphic_a->fixedBitmapsToFollow [fixNr].numberOfBytesInRawData;
        actualWidth = vtObjectPictureGraphic_a->fixedBitmapsToFollow [fixNr].actualWidth;
        actualHeight = vtObjectPictureGraphic_a->fixedBitmapsToFollow [fixNr].actualHeight;
        /* format is bit 8+7, options is bit 2-0 (NO RLE1/4/8 stuff here!! */
        options = vtObjectPictureGraphic_a->fixedBitmapsToFollow [fixNr].formatoptions & 0x7;
        ui8_graphicType = vtDepth;
    MACRO_CheckFixedBitmapsLoop_end

    if (!b_foundFixedBitmap) {
      // See what we have as standard...
      actualWidth = vtObjectPictureGraphic_a->actualWidth;
      actualHeight = vtObjectPictureGraphic_a->actualHeight;
      MACRO_calculate_ui8_graphicType
      switch (ui8_graphicType) {
        case 2:  MACRO_helperForDifferentSizes (numberOfBytesInRawData2, options, rawData2, 0x10) break;
        case 1:  MACRO_helperForDifferentSizes (numberOfBytesInRawData1, options, rawData1, 0x08) break;
        case 0:
        default: MACRO_helperForDifferentSizes (numberOfBytesInRawData0, options, rawData0, 0x04) break;
      }
    }

    if (sourceOffset == 0) { // dump out constant sized stuff
      destMemory [0] = vtObject_a->ID & 0xFF;
      destMemory [1] = vtObject_a->ID >> 8;
      destMemory [2] = 20; // Object Type = Picture Graphic
      destMemory [3] = width & 0xFF; // calculated above
      destMemory [4] = width >> 8;   // dependent on FLAG_ORIGIN_SKM
      destMemory [5] = actualWidth & 0xFF;
      destMemory [6] = actualWidth >> 8;
      destMemory [7] = actualHeight & 0xFF;
      destMemory [8] = actualHeight >> 8;
      destMemory [9] = ui8_graphicType;
      destMemory [10] = options;
      destMemory [11] = __IsoAgLib::getVtClientInstance4Comm().getClientByID(s_properties.clientId).getUserClippedColor(
                          vtObjectPictureGraphic_a->transparencyColour, this, IsoAgLib::TransparencyColour);
      destMemory [12] = (numberOfBytesInRawData) & 0xFF;
      destMemory [13] = (numberOfBytesInRawData >> 8) & 0xFF;
      destMemory [14] = (numberOfBytesInRawData >> 16) & 0xFF;
      destMemory [15] = (numberOfBytesInRawData >> 24) & 0xFF;
      destMemory [16] = vtObjectPictureGraphic_a->numberOfMacrosToFollow;

      sourceOffset += 17;
      curBytes += 17;
    }

    while ((sourceOffset >= 17) && (sourceOffset < (17+numberOfBytesInRawData)) && ((curBytes+1) <= maxBytes)) {
      destMemory [curBytes] = rawData [sourceOffset-17];
      curBytes++;
      sourceOffset++;
    }

    MACRO_streamEventMacro(17+numberOfBytesInRawData);

    return curBytes;
}

vtObjectPictureGraphic_c::vtObjectPictureGraphic_c() {}

uint32_t
vtObjectPictureGraphic_c::fitTerminal() const
{
  MACRO_localVars;
  MACRO_scaleLocalVars;
  MACRO_scaleSKLocalVars;

  uint32_t numberOfBytesInRawData = 0;
  uint8_t ui8_graphicType;

  MACRO_calculateRequestedSize

  MACRO_CheckFixedBitmapsLoop_start
      numberOfBytesInRawData = vtObjectPictureGraphic_a->fixedBitmapsToFollow [fixNr].numberOfBytesInRawData;
  MACRO_CheckFixedBitmapsLoop_end

  if (!b_foundFixedBitmap) {
    MACRO_calculate_ui8_graphicType
    switch (ui8_graphicType) {
      case 2:  MACRO_helperForDifferentSizesSizeOnly (numberOfBytesInRawData2) break;
      case 1:  MACRO_helperForDifferentSizesSizeOnly (numberOfBytesInRawData1) break;
      case 0:
      default: MACRO_helperForDifferentSizesSizeOnly (numberOfBytesInRawData0) break;
    }
  }

  return 17+numberOfBytesInRawData+vtObjectPictureGraphic_a->numberOfMacrosToFollow*2;
}

#ifdef USE_ISO_TERMINAL_GETATTRIBUTES
uint16_t
vtObjectPictureGraphic_c::updateWidth(bool b_SendRequest)
{
  if (b_SendRequest)
    return getValue16GetAttribute(MACRO_getStructOffset(get_vtObjectPictureGraphic_a(), width), sizeof(iVtObjectPictureGraphic_s), 1);
  else
    return getValue16(MACRO_getStructOffset(get_vtObjectPictureGraphic_a(), width), sizeof(iVtObjectPictureGraphic_s));
}

uint8_t
vtObjectPictureGraphic_c::updateOptions(bool b_SendRequest)
{
  if (b_SendRequest)
    return getValue8GetAttribute(MACRO_getStructOffset(get_vtObjectPictureGraphic_a(), options), sizeof(iVtObjectPictureGraphic_s), 2);
  else
    return getValue8(MACRO_getStructOffset(get_vtObjectPictureGraphic_a(), options), sizeof(iVtObjectPictureGraphic_s));
}

uint8_t
vtObjectPictureGraphic_c::updateTransparencyColour(bool b_SendRequest)
{
  if (b_SendRequest)
    return getValue8GetAttribute(MACRO_getStructOffset(get_vtObjectPictureGraphic_a(), transparencyColour), sizeof(iVtObjectPictureGraphic_s), 3);
  else
    return getValue8(MACRO_getStructOffset(get_vtObjectPictureGraphic_a(), transparencyColour), sizeof(iVtObjectPictureGraphic_s));
}

/** these attributes are in parentheses in the spec, so commented out here
uint16_t
vtObjectPictureGraphic_c::updateActualWidth(bool b_SendRequest)
{
  if (b_SendRequest)
    return getValue16GetAttribute(MACRO_getStructOffset(get_vtObjectPictureGraphic_a(), actualWidth), sizeof(iVtObjectPictureGraphic_s), 4);
  else
    return getValue16(MACRO_getStructOffset(get_vtObjectPictureGraphic_a(), actualWidth), sizeof(iVtObjectPictureGraphic_s));
}

uint16_t
vtObjectPictureGraphic_c::updateActualHeight(bool b_SendRequest)
{
  if (b_SendRequest)
    return getValue16GetAttribute(MACRO_getStructOffset(get_vtObjectPictureGraphic_a(), actualHeight), sizeof(iVtObjectPictureGraphic_s), 5);
  else
    return getValue16(MACRO_getStructOffset(get_vtObjectPictureGraphic_a(), actualHeight), sizeof(iVtObjectPictureGraphic_s));
}

uint8_t
vtObjectPictureGraphic_c::updateFormat(bool b_SendRequest)
{
  if (b_SendRequest)
    return getValue8GetAttribute(MACRO_getStructOffset(get_vtObjectPictureGraphic_a(), format), sizeof(iVtObjectPictureGraphic_s), 6);
  else
    return getValue8(MACRO_getStructOffset(get_vtObjectPictureGraphic_a(), format), sizeof(iVtObjectPictureGraphic_s));
}
*/

void
vtObjectPictureGraphic_c::saveReceivedAttribute(uint8_t attrID, uint8_t* pui8_attributeValue)
{
  switch (attrID)
  {
    case 1: saveValue16(MACRO_getStructOffset(get_vtObjectPictureGraphic_a(), width), sizeof(iVtObjectPictureGraphic_s), convertLittleEndianStringUi16(pui8_attributeValue)); break;
    case 2: saveValue8(MACRO_getStructOffset(get_vtObjectPictureGraphic_a(), options), sizeof(iVtObjectPictureGraphic_s), convertLittleEndianStringUi8(pui8_attributeValue)); break;
    case 3: saveValue8(MACRO_getStructOffset(get_vtObjectPictureGraphic_a(), transparencyColour), sizeof(iVtObjectPictureGraphic_s), convertLittleEndianStringUi8(pui8_attributeValue)); break;
    /** these attributes are in parentheses in the spec, so commented out here
    case 4: saveValue16(MACRO_getStructOffset(get_vtObjectPictureGraphic_a(), actualWidth), sizeof(iVtObjectPictureGraphic_s), convertLittleEndianStringUi16(pui8_attributeValue)); break;
    case 5: saveValue16(MACRO_getStructOffset(get_vtObjectPictureGraphic_a(), actualHeight), sizeof(iVtObjectPictureGraphic_s), convertLittleEndianStringUi16(pui8_attributeValue)); break;
    case 6: saveValue8(MACRO_getStructOffset(get_vtObjectPictureGraphic_a(), format), sizeof(iVtObjectPictureGraphic_s), convertLittleEndianStringUi8(pui8_attributeValue)); break;
    */
    default: break;
  }
}
#endif

} // __IsoAgLib
