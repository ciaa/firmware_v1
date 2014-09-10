/*
  vttypes.h

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/
#ifndef VTTYPES_H
#define VTTYPES_H

#include <IsoAgLib/isoaglib_config.h>

#define FLAG_BIT_IN_RAM 0
#define FLAG_BIT_ORIGIN_SKM 1
#define FLAG_BIT_STRING_IN_RAM 2
#define FLAG_BIT_OBJECTS2FOLLOW_IN_RAM 3
#define FLAG_BIT_OMIT_OBJECT 4

#define FLAG_IN_RAM (1<<FLAG_BIT_IN_RAM)
#define FLAG_ORIGIN_SKM (1<<FLAG_BIT_ORIGIN_SKM)
#define FLAG_STRING_IN_RAM (1<<FLAG_BIT_STRING_IN_RAM)
#define FLAG_OBJECTS2FOLLOW_IN_RAM (1<<FLAG_BIT_OBJECTS2FOLLOW_IN_RAM)
#define FLAG_OMIT_OBJECT (1<<FLAG_BIT_OMIT_OBJECT)


namespace __IsoAgLib {

// Forward declarations
class vtObject_c;
class vtObjectWorkingSet_c;
class vtObjectDataMask_c;
class vtObjectAlarmMask_c;
class vtObjectContainer_c;
class vtObjectSoftKeyMask_c;
class vtObjectKey_c;
class vtObjectButton_c;
class vtObjectInputBoolean_c;
class vtObjectInputString_c;
class vtObjectInputNumber_c;
class vtObjectInputList_c;
class vtObjectOutputString_c;
class vtObjectOutputNumber_c;
class vtObjectLine_c;
class vtObjectRectangle_c;
class vtObjectEllipse_c;
class vtObjectPolygon_c;
class vtObjectMeter_c;
class vtObjectLinearBarGraph_c;
class vtObjectArchedBarGraph_c;
class vtObjectPictureGraphic_c;
class vtObjectNumberVariable_c;
class vtObjectStringVariable_c;
class vtObjectFontAttributes_c;
class vtObjectLineAttributes_c;
class vtObjectFillAttributes_c;
class vtObjectInputAttributes_c;
class vtObjectObjectPointer_c;
class vtObjectMacro_c;
class vtObjectArchedBarGraph_c;

class vtObjectAuxiliaryFuntion_c;
class vtObjectAuxiliaryInput_c;
class vtObjectAuxiliaryFuntion2_c;
class vtObjectAuxiliaryInput2_c;
class vtObjectAuxiliaryControlDesignatorObjectPointer_c;

} // end namespace __IsoAgLib

// Use the following define in your project's define-settings if you are using objects larger than 64KB
// If there are objects larger and this define is NOT set, a compile error will be issued automatically.
// vt2iso will generate this dependend error automatically when creating objects larger than 64KB
//#define USE_OBJECTS_LARGER_THAN_64K

#ifdef USE_OBJECTS_LARGER_THAN_64K
typedef uint32_t objRange_t;
#else
typedef uint16_t objRange_t;
#endif

#define MACRO_localVars \
    MACRO_vtObjectTypeS* MACRO_vtObjectTypeA = (MACRO_vtObjectTypeS *) vtObject_a;

#define MACRO_streamLocalVars \
    MACRO_localVars \
    /* current bytes written */ \
    uint16_t curBytes=0;

#define MACRO_scaleLocalVarVtDimension \
    int32_t vtDimension=__IsoAgLib::getVtClientInstance4Comm().getClientByID (s_properties.clientId).getVtServerInst().getVtHardwareDimension();

#define MACRO_scaleLocalVarOpDimension \
    int32_t opDimension=__IsoAgLib::getVtClientInstance4Comm().getClientByID (s_properties.clientId).getVtObjectPoolDimension();

#ifdef PROPRIETARY_NO_POOL_SCALING
  #define MACRO_scaleLocalVars \
      MACRO_scaleLocalVarOpDimension \
      MACRO_scaleLocalVarVtDimension \
      vtDimension = opDimension; /* prevent scaling by setting the vtDimension equal to opDimension */
#else
  #define MACRO_scaleLocalVars \
      MACRO_scaleLocalVarOpDimension \
      MACRO_scaleLocalVarVtDimension
#endif


#ifdef PROPRIETARY_NO_POOL_SCALING
  #define MACRO_getSkDimension \
      opSoftKeyWidth  = __IsoAgLib::getVtClientInstance4Comm().getClientByID (s_properties.clientId).getVtObjectPoolSoftKeyWidth(); \
      opSoftKeyHeight = __IsoAgLib::getVtClientInstance4Comm().getClientByID (s_properties.clientId).getVtObjectPoolSoftKeyHeight(); \
      /* prevent scaling by setting the vtDimension equal to opDimension */ \
      vtSoftKeyWidth  = opSoftKeyWidth; \
      vtSoftKeyHeight = opSoftKeyHeight; 
#else
  #define MACRO_getSkDimension \
      opSoftKeyWidth  = __IsoAgLib::getVtClientInstance4Comm().getClientByID (s_properties.clientId).getVtObjectPoolSoftKeyWidth(); \
      opSoftKeyHeight = __IsoAgLib::getVtClientInstance4Comm().getClientByID (s_properties.clientId).getVtObjectPoolSoftKeyHeight(); \
      vtSoftKeyWidth  = __IsoAgLib::getVtClientInstance4Comm().getClientByID (s_properties.clientId).getVtServerInst().getVtCapabilities ()->skWidth; \
      vtSoftKeyHeight = __IsoAgLib::getVtClientInstance4Comm().getClientByID (s_properties.clientId).getVtServerInst().getVtCapabilities ()->skHeight;
#endif

#define MACRO_scaleSKLocalVars \
    int32_t opSoftKeyWidth,  opSoftKeyHeight, vtSoftKeyWidth, vtSoftKeyHeight; \
    int32_t opButtonWidth, opButtonHeight, vtButtonWidth, vtButtonHeight;\
    int32_t factorM, factorD; /* zaehler, nenner */ \
    if (p_parentButtonObject != NULL) { \
      opButtonWidth  = p_parentButtonObject->get_vtObjectButton_a().width-8; \
      opButtonHeight = p_parentButtonObject->get_vtObjectButton_a().height-8; \
      vtButtonWidth  = ((opButtonWidth+8) * vtDimension) / opDimension - 8; \
      vtButtonHeight = ((opButtonHeight+8) * vtDimension) / opDimension - 8; \
      const int32_t ci_factorX = ((vtButtonWidth << 20) / opButtonWidth); \
      const int32_t ci_factorY = ((vtButtonHeight << 20) / opButtonHeight); \
      if (ci_factorX < ci_factorY) { \
        factorM = vtButtonWidth; \
        factorD = opButtonWidth; \
      } else { \
        factorM = vtButtonHeight; \
        factorD = opButtonHeight; \
      } \
      /* initialize variables which are normally only used in the else branch */ \
      opSoftKeyWidth = opSoftKeyHeight = vtSoftKeyWidth = vtSoftKeyHeight = 0; \
    } else {  \
      MACRO_getSkDimension \
      /* set defaults for button sizes to avoid compiler warning */ \
      opButtonWidth = opButtonHeight = vtButtonWidth = vtButtonHeight = 0; \
      const int32_t ci_factorX = (vtSoftKeyWidth  << 20) / opSoftKeyWidth; \
      const int32_t ci_factorY = (vtSoftKeyHeight << 20) / opSoftKeyHeight; \
      if (ci_factorX < ci_factorY) { \
        factorM = vtSoftKeyWidth; \
        factorD = opSoftKeyWidth; \
      } else { \
        factorM = vtSoftKeyHeight; \
        factorD = opSoftKeyHeight; \
      } \
    } \

#define MACRO_getBlockfont(index)  \
    int32_t xBlock, yBlock; \
    if (MACRO_vtObjectTypeA->objectsToFollow [index].vtObjectBlockFont != NULL) { \
      xBlock = MACRO_vtObjectTypeA->objectsToFollow [index].col * (MACRO_vtObjectTypeA->objectsToFollow [index].vtObjectBlockFont->getScaledWidthHeight () >> 8); \
      yBlock = MACRO_vtObjectTypeA->objectsToFollow [index].row * (MACRO_vtObjectTypeA->objectsToFollow [index].vtObjectBlockFont->getScaledWidthHeight () & 0xFF); \
    } else { \
      xBlock = 0; \
      yBlock = 0; \
    }

#define MACRO_streamObject(bytesBefore) \
    uint16_t nrObject = (sourceOffset-(bytesBefore)) / 2; \
    while ((sourceOffset >= (bytesBefore)) && (sourceOffset < ((bytesBefore)+2U*MACRO_vtObjectTypeA->numberOfObjectsToFollow)) && ((curBytes+2) <= maxBytes)) { \
      /* write out an object */ \
      if(MACRO_vtObjectTypeA->objectsToFollow [nrObject].vtObject != NULL) { \
        destMemory [curBytes]   = MACRO_vtObjectTypeA->objectsToFollow [nrObject].vtObject->getID() & 0xFF; \
        destMemory [curBytes+1] = MACRO_vtObjectTypeA->objectsToFollow [nrObject].vtObject->getID() >> 8; \
      } else { \
        destMemory [curBytes]   = 0xFF; \
        destMemory [curBytes+1] = 0xFF; \
      } \
      nrObject++; \
      curBytes += 2; \
      sourceOffset += 2; \
    }

#define MACRO_streamObjectXY(bytesBefore) \
    uint16_t nrObjectXY = (sourceOffset-(bytesBefore)) / 6; \
    while ((sourceOffset >= (bytesBefore)) && (sourceOffset < ((bytesBefore)+6U*MACRO_vtObjectTypeA->numberOfObjectsToFollow)) && ((curBytes+6) <= maxBytes)) { \
      /* write out an object_X_Y pair */ \
      destMemory [curBytes]   = MACRO_vtObjectTypeA->objectsToFollow [nrObjectXY].vtObject->getID() & 0xFF; \
      destMemory [curBytes+1] = MACRO_vtObjectTypeA->objectsToFollow [nrObjectXY].vtObject->getID() >> 8; \
      MACRO_getBlockfont(nrObjectXY)  \
      if ((s_properties.flags & FLAG_ORIGIN_SKM) || p_parentButtonObject) { \
        destMemory [curBytes+2] = ((((MACRO_vtObjectTypeA->objectsToFollow [nrObjectXY].x)*factorM)/factorD)+xBlock) & 0xFF; \
        destMemory [curBytes+3] = ((((MACRO_vtObjectTypeA->objectsToFollow [nrObjectXY].x)*factorM)/factorD)+xBlock) >> 8; \
        destMemory [curBytes+4] = ((((MACRO_vtObjectTypeA->objectsToFollow [nrObjectXY].y)*factorM)/factorD)+yBlock) & 0xFF; \
        destMemory [curBytes+5] = ((((MACRO_vtObjectTypeA->objectsToFollow [nrObjectXY].y)*factorM)/factorD)+yBlock) >> 8; \
      } else { \
        destMemory [curBytes+2] = ((((MACRO_vtObjectTypeA->objectsToFollow [nrObjectXY].x)*vtDimension)/opDimension)+xBlock) & 0xFF; \
        destMemory [curBytes+3] = ((((MACRO_vtObjectTypeA->objectsToFollow [nrObjectXY].x)*vtDimension)/opDimension)+xBlock) >> 8; \
        destMemory [curBytes+4] = ((((MACRO_vtObjectTypeA->objectsToFollow [nrObjectXY].y)*vtDimension)/opDimension)+yBlock) & 0xFF; \
        destMemory [curBytes+5] = ((((MACRO_vtObjectTypeA->objectsToFollow [nrObjectXY].y)*vtDimension)/opDimension)+yBlock) >> 8; \
      } \
      nrObjectXY++; \
      curBytes += 6; \
      sourceOffset += 6; \
    }

#define MACRO_streamObjectXYcenteredInButton(bytesBefore) \
    uint16_t nrObjectXY = (sourceOffset-(bytesBefore)) / 6; \
    MACRO_scaleSKLocalVars \
    int16_t centerX = (vtButtonWidth -  ((opButtonWidth *factorM)/factorD)) >>1; \
    int16_t centerY = (vtButtonHeight - ((opButtonHeight*factorM)/factorD)) >>1; \
    while ((sourceOffset >= (bytesBefore)) && (sourceOffset < ((bytesBefore)+6U*MACRO_vtObjectTypeA->numberOfObjectsToFollow)) && ((curBytes+6) <= maxBytes)) { \
      MACRO_getBlockfont(nrObjectXY)  \
      /* write out an objectX_y pair */ \
      destMemory [curBytes]   = MACRO_vtObjectTypeA->objectsToFollow [nrObjectXY].vtObject->getID() & 0xFF; \
      destMemory [curBytes+1] = MACRO_vtObjectTypeA->objectsToFollow [nrObjectXY].vtObject->getID() >> 8; \
      destMemory [curBytes+2] = (((MACRO_vtObjectTypeA->objectsToFollow [nrObjectXY].x*factorM)/factorD)+xBlock+centerX) & 0xFF; \
      destMemory [curBytes+3] = (((MACRO_vtObjectTypeA->objectsToFollow [nrObjectXY].x*factorM)/factorD)+xBlock+centerX) >> 8; \
      destMemory [curBytes+4] = (((MACRO_vtObjectTypeA->objectsToFollow [nrObjectXY].y*factorM)/factorD)+yBlock+centerY) & 0xFF; \
      destMemory [curBytes+5] = (((MACRO_vtObjectTypeA->objectsToFollow [nrObjectXY].y*factorM)/factorD)+yBlock+centerY) >> 8; \
      nrObjectXY++; \
      curBytes += 6; \
      sourceOffset += 6; \
    }

#define MACRO_streamObjectXYcenteredInSoftKey(bytesBefore) \
    uint16_t nrObjectXY = (sourceOffset-(bytesBefore)) / 6; \
    MACRO_scaleSKLocalVars \
    int16_t centerX = (vtSoftKeyWidth -  ((opSoftKeyWidth *factorM)/factorD)) >>1; \
    int16_t centerY = (vtSoftKeyHeight - ((opSoftKeyHeight*factorM)/factorD)) >>1; \
    while ((sourceOffset >= (bytesBefore)) && (sourceOffset < ((bytesBefore)+6U*MACRO_vtObjectTypeA->numberOfObjectsToFollow)) && ((curBytes+6) <= maxBytes)) { \
      MACRO_getBlockfont(nrObjectXY)  \
      /* write out an objectX_y pair */ \
      destMemory [curBytes]   = MACRO_vtObjectTypeA->objectsToFollow [nrObjectXY].vtObject->getID() & 0xFF; \
      destMemory [curBytes+1] = MACRO_vtObjectTypeA->objectsToFollow [nrObjectXY].vtObject->getID() >> 8; \
      destMemory [curBytes+2] = (((MACRO_vtObjectTypeA->objectsToFollow [nrObjectXY].x*factorM)/factorD)+xBlock+centerX) & 0xFF; \
      destMemory [curBytes+3] = (((MACRO_vtObjectTypeA->objectsToFollow [nrObjectXY].x*factorM)/factorD)+xBlock+centerX) >> 8; \
      destMemory [curBytes+4] = (((MACRO_vtObjectTypeA->objectsToFollow [nrObjectXY].y*factorM)/factorD)+yBlock+centerY) & 0xFF; \
      destMemory [curBytes+5] = (((MACRO_vtObjectTypeA->objectsToFollow [nrObjectXY].y*factorM)/factorD)+yBlock+centerY) >> 8; \
      nrObjectXY++; \
      curBytes += 6; \
      sourceOffset += 6; \
    }

#define MACRO_streamEventMacro(bytesBefore) \
    uint16_t nrEventMacro = (sourceOffset-(bytesBefore)) / 2; \
    while ((sourceOffset >= (bytesBefore)) && (sourceOffset < ((bytesBefore)+2U*MACRO_vtObjectTypeA->numberOfMacrosToFollow)) && ((curBytes+2) <= maxBytes)) { \
      /* write out an objectX_y pair */ \
      destMemory [curBytes]   = MACRO_vtObjectTypeA->macrosToFollow [nrEventMacro].event; \
      destMemory [curBytes+1] = MACRO_vtObjectTypeA->macrosToFollow [nrEventMacro].vtObjectMacro->getID() & 0xFF; /* Macro ObjID must be 0-255 !! */ \
      nrEventMacro++; \
      curBytes += 2; \
      sourceOffset += 2; \
    }

#define MACRO_streamLanguages(bytesBefore)\
    uint16_t nrLanguageCode = (sourceOffset-(bytesBefore)) / 2; \
    while ((sourceOffset >= (bytesBefore)) && (sourceOffset < ((bytesBefore)+2U*MACRO_vtObjectTypeA->numberOfLanguagesToFollow)) && ((curBytes+2) <= maxBytes)) { \
      /* write out a two character Language Code pair */ \
      destMemory [curBytes]   = MACRO_vtObjectTypeA->languagesToFollow [nrLanguageCode].language[0]; \
      destMemory [curBytes+1] = MACRO_vtObjectTypeA->languagesToFollow [nrLanguageCode].language[1]; \
      nrLanguageCode++; \
      curBytes += 2; \
      sourceOffset += 2; \
    }

/** @todo SOON-174 polygon positions with x/yBlock!?
      if (MACRO_vtObjectTypeA->objectsToFollow [nrObjectXY].vtObjectBlockFont != NULL) { \
        xBlock = MACRO_vtObjectTypeA->objectsToFollow [nrObjectXY].col * (MACRO_vtObjectTypeA->objectsToFollow [nrObjectXY].vtObjectBlockFont->getScaledWidthHeight () >> 8); \
        yBlock = MACRO_vtObjectTypeA->objectsToFollow [nrObjectXY].row * (MACRO_vtObjectTypeA->objectsToFollow [nrObjectXY].vtObjectBlockFont->getScaledWidthHeight () & 0xFF); \
      } else { \
      } \
*/
/* M.Wodok's Version... */
#define MACRO_streamPolygonPoints(bytesBefore) \
    uint16_t nrPointXY = (sourceOffset-(bytesBefore)) >>2; \
    while ((sourceOffset >= (bytesBefore)) && (sourceOffset < ((bytesBefore)+4U*MACRO_vtObjectTypeA->numberOfPoints)) && ((curBytes+4) <= maxBytes)) { \
      int32_t xBlock, yBlock; \
        xBlock = 0; \
        yBlock = 0; \
      if ((s_properties.flags & FLAG_ORIGIN_SKM) || p_parentButtonObject) { \
        destMemory [curBytes  ] = ((((uint32_t) (MACRO_vtObjectTypeA->pointsToFollow [nrPointXY].x)*factorM)/factorD)+xBlock) & 0xFF; \
        destMemory [curBytes+1] = ((((uint32_t) (MACRO_vtObjectTypeA->pointsToFollow [nrPointXY].x)*factorM)/factorD)+xBlock) >> 8; \
        destMemory [curBytes+2] = ((((uint32_t) (MACRO_vtObjectTypeA->pointsToFollow [nrPointXY].y)*factorM)/factorD)+yBlock) & 0xFF; \
        destMemory [curBytes+3] = ((((uint32_t) (MACRO_vtObjectTypeA->pointsToFollow [nrPointXY].y)*factorM)/factorD)+yBlock) >> 8; \
      } else { \
        destMemory [curBytes  ] = ((((uint32_t) (MACRO_vtObjectTypeA->pointsToFollow [nrPointXY].x)*vtDimension)/opDimension)+xBlock) & 0xFF; \
        destMemory [curBytes+1] = ((((uint32_t) (MACRO_vtObjectTypeA->pointsToFollow [nrPointXY].x)*vtDimension)/opDimension)+xBlock) >> 8; \
        destMemory [curBytes+2] = ((((uint32_t) (MACRO_vtObjectTypeA->pointsToFollow [nrPointXY].y)*vtDimension)/opDimension)+yBlock) & 0xFF; \
        destMemory [curBytes+3] = ((((uint32_t) (MACRO_vtObjectTypeA->pointsToFollow [nrPointXY].y)*vtDimension)/opDimension)+yBlock) >> 8; \
      } \
      nrPointXY++; \
      curBytes += 4; \
      sourceOffset += 4; \
    }


#define MACRO_getStructOffset(structPointer,structElement) \
  ((uint16_t) ((uint8_t*)(&structPointer->structElement) - ((uint8_t*)structPointer)))

#define MACRO_streamUi16(msg, value)  \
  (msg)[0] = value & 0xFF;  \
  (msg)[1] = value >> 8;

#define MACRO_streamId(msg, ds) \
  if (ds == NULL) { \
    (msg)[0] = 0xFF;  \
    (msg)[1] = 0xFF; \
  } \
  else {  \
    (msg)[0] = ds->getID() & 0xFF;  \
    (msg)[1] = ds->getID() >> 8;  \
  }

#endif // VTTYPES_H

