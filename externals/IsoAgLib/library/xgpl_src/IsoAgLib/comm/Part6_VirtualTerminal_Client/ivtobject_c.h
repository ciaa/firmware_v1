/*
  ivtobject_c.h

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/
#ifndef IVTOBJECT_C_H
#define IVTOBJECT_C_H

#include "ivttypes.h"
#include <IsoAgLib/isoaglib_config.h>
#include <IsoAgLib/util/impl/singleton.h>
#include <IsoAgLib/util/iassert.h>

namespace IsoAgLib {

class iVtObject_c : public ClientBase
{
public:
  typedef struct iVtObject_s {
    uint16_t ID;
  } iVtObject_s;

  typedef struct iVtObjectAlarmMask_s {
    uint16_t ID;
    uint8_t backgroundColour;
    iVtObjectSoftKeyMask_c* softKeyMask;
    uint8_t priority;
    uint8_t acousticSignal;
    uint8_t numberOfObjectsToFollow;
    const repeat_iVtObject_x_y_iVtObjectFontAttributes_row_col_s* objectsToFollow;
    uint8_t numberOfMacrosToFollow;
    const repeat_event_iVtObjectMacro_s* macrosToFollow;
  } iVtObjectAlarmMask_s;

  typedef struct iVtObjectArchedBarGraph_s {
    uint16_t ID;
    uint16_t width;
    uint16_t height;
    uint8_t colour;
    uint8_t targetLineColour;
    uint8_t options;
    uint8_t startAngle;
    uint8_t endAngle;
    uint16_t barGraphWidth;
    uint16_t minValue;
    uint16_t maxValue;
    iVtObject_c* variableReference;
    uint16_t value;
    iVtObject_c* targetValueVariableReference;
    uint16_t targetValue;
    uint8_t numberOfMacrosToFollow;
    const repeat_event_iVtObjectMacro_s* macrosToFollow;
  } iVtObjectArchedBarGraph_s;

  typedef struct iVtObjectButton_s {
    uint16_t ID;
    uint16_t width;
    uint16_t height;
    uint8_t backgroundColour;
    uint8_t borderColour;
    uint8_t keyCode;
    uint8_t options;
    uint8_t numberOfObjectsToFollow;
    const repeat_iVtObject_x_y_iVtObjectFontAttributes_row_col_s* objectsToFollow;
    uint8_t numberOfMacrosToFollow;
    const repeat_event_iVtObjectMacro_s* macrosToFollow;
  } iVtObjectButton_s;

  typedef struct iVtObjectContainer_s {
    uint16_t ID;
    uint16_t width;
    uint16_t height;
    uint8_t hidden;
    uint8_t numberOfObjectsToFollow;
    const repeat_iVtObject_x_y_iVtObjectFontAttributes_row_col_s* objectsToFollow;
    uint8_t numberOfMacrosToFollow;
    const repeat_event_iVtObjectMacro_s* macrosToFollow;
  } iVtObjectContainer_s;

  typedef struct iVtObjectDataMask_s {
    uint16_t ID;
    uint8_t backgroundColour;
    iVtObjectSoftKeyMask_c* softKeyMask;
    uint8_t numberOfObjectsToFollow;
    const repeat_iVtObject_x_y_iVtObjectFontAttributes_row_col_s* objectsToFollow;
    uint8_t numberOfMacrosToFollow;
    const repeat_event_iVtObjectMacro_s* macrosToFollow;
  } iVtObjectDataMask_s;

  typedef struct iVtObjectEllipse_s {
    uint16_t ID;
    iVtObjectLineAttributes_c* lineAttributes;
    uint16_t width;
    uint16_t height;
    uint8_t ellipseType;
    uint8_t startAngle;
    uint8_t endAngle;
    iVtObjectFillAttributes_c* fillAttributes;
    uint8_t numberOfMacrosToFollow;
    const repeat_event_iVtObjectMacro_s* macrosToFollow;
  } iVtObjectEllipse_s;

  typedef struct iVtObjectFillAttributes_s {
    uint16_t ID;
    uint8_t fillType;
    uint8_t fillColour;
    iVtObjectPictureGraphic_c* fillPatternObject;
    uint8_t numberOfMacrosToFollow;
    const repeat_event_iVtObjectMacro_s* macrosToFollow;
  } iVtObjectFillAttributes_s;

  typedef struct iVtObjectFontAttributes_s {
    uint16_t ID;
    uint8_t fontColour;
    uint8_t fontSize;
    uint8_t fontType; // always =0 ISO_LATIN_1
    uint8_t fontStyle;
    uint8_t numberOfMacrosToFollow;
    const repeat_event_iVtObjectMacro_s* macrosToFollow;
  } iVtObjectFontAttributes_s;

  typedef struct iVtObjectInputAttributes_s {
    uint16_t ID;
    uint8_t validationType;
    uint8_t length;
    char* validationString;
    uint8_t numberOfMacrosToFollow;
    const repeat_event_iVtObjectMacro_s* macrosToFollow;
  } iVtObjectInputAttributes_s;

  typedef struct iVtObjectInputBoolean_s {
    uint16_t ID;
    uint8_t backgroundColour;
    uint16_t width;
    iVtObject_c* foregroundColour;
    iVtObject_c* variableReference;
    uint8_t value;
    uint8_t enabled;
    uint8_t numberOfMacrosToFollow;
    const repeat_event_iVtObjectMacro_s* macrosToFollow;
  } iVtObjectInputBoolean_s;

  typedef struct iVtObjectInputList_s {
    uint16_t ID;
    uint16_t width;
    uint16_t height;
    iVtObject_c* variableReference;
    uint8_t value;
    uint8_t enabled;
    uint8_t numberOfObjectsToFollow;
    const repeat_iVtObject_s* objectsToFollow;
    uint8_t numberOfMacrosToFollow;
    const repeat_event_iVtObjectMacro_s* macrosToFollow;
  } iVtObjectInputList_s;

  typedef struct iVtObjectInputNumber_s {
    uint16_t ID;
    uint16_t width;
    uint16_t height;
    uint8_t backgroundColour;
    iVtObject_c* fontAttributes;
    uint8_t options;
    iVtObject_c* variableReference;
    uint32_t value;
    uint32_t minValue;
    uint32_t maxValue;
    int32_t offset;
    float scale;
    uint8_t numberOfDecimals;
    uint8_t format;
    uint8_t horizontalJustification;
    uint8_t secondOptionsByte;
    uint8_t numberOfMacrosToFollow;
    const repeat_event_iVtObjectMacro_s* macrosToFollow;
  } iVtObjectInputNumber_s;

  typedef struct iVtObjectInputString_s {
    uint16_t ID;
    uint16_t width;
    uint16_t height;
    uint8_t backgroundColour;
    iVtObjectFontAttributes_c* fontAttributes;
    iVtObjectInputAttributes_c* inputAttributes;
    uint8_t options;
    iVtObjectStringVariable_c* variableReference;
    uint8_t horizontalJustification;
    uint16_t length;
    char* value;
    uint8_t enabled;
    uint8_t numberOfMacrosToFollow;
    const repeat_event_iVtObjectMacro_s* macrosToFollow;
  } iVtObjectInputString_s;

  typedef struct iVtObjectKey_s {
    uint16_t ID;
    uint8_t backgroundColour;
    uint8_t keyCode;
    uint8_t numberOfObjectsToFollow;
    const repeat_iVtObject_x_y_iVtObjectFontAttributes_row_col_s* objectsToFollow;
    uint8_t numberOfMacrosToFollow;
    const repeat_event_iVtObjectMacro_s* macrosToFollow;
  } iVtObjectKey_s;

  typedef struct iVtObjectLinearBarGraph_s {
    uint16_t ID;
    uint16_t width;
    uint16_t height;
    uint8_t colour;
    uint8_t targetLineColour;
    uint8_t options;
    uint8_t numberOfTicks;
    uint16_t minValue;
    uint16_t maxValue;
    iVtObject_c* variableReference;
    uint16_t value;
    iVtObject_c* targetValueVariableReference;
    uint16_t targetValue;
    uint8_t numberOfMacrosToFollow;
    const repeat_event_iVtObjectMacro_s* macrosToFollow;
  } iVtObjectLinearBarGraph_s;

  typedef struct iVtObjectLineAttributes_s {
    uint16_t ID;
    uint8_t lineColour;
    uint8_t lineWidth;
    uint16_t lineArt;
    uint8_t numberOfMacrosToFollow;
    const repeat_event_iVtObjectMacro_s* macrosToFollow;
  } iVtObjectLineAttributes_s;

  typedef struct iVtObjectLine_s {
    uint16_t ID;
    iVtObjectLineAttributes_c* lineAttributes;
    uint16_t width;
    uint16_t height;
    uint8_t lineDirection;
    uint8_t numberOfMacrosToFollow;
    const repeat_event_iVtObjectMacro_s* macrosToFollow;
  } iVtObjectLine_s;

  typedef struct iVtObjectMacro_s {
    uint16_t ID;
    uint16_t numBytesToFollow;
    const uint8_t* commandsToFollow;
  } iVtObjectMacro_s;

  typedef struct iVtObjectMeter_s {
    uint16_t ID;
    uint16_t width;
    uint8_t needleColour;
    uint8_t borderColour;
    uint8_t arcAndTickColour;
    uint8_t options;
    uint8_t numberOfTicks;
    uint8_t startAngle;
    uint8_t endAngle;
    uint16_t minValue;
    uint16_t maxValue;
    iVtObject_c* variableReference;
    uint16_t value;
    uint8_t numberOfMacrosToFollow;
    const repeat_event_iVtObjectMacro_s* macrosToFollow;
  } iVtObjectMeter_s;

  typedef struct iVtObjectNumberVariable_s {
    uint16_t ID;
    uint32_t value;
  } iVtObjectNumberVariable_s;

  typedef struct iVtObjectObjectPointer_s {
    uint16_t ID;
    iVtObject_c* value;
  } iVtObjectObjectPointer_s;

  typedef struct iVtObjectOutputList_s {
    uint16_t ID;
    uint16_t width;
    uint16_t height;
    iVtObject_c* variableReference;
    uint8_t value;
    uint8_t numberOfObjectsToFollow;
    const repeat_iVtObject_s* objectsToFollow;
    uint8_t numberOfMacrosToFollow;
    const repeat_event_iVtObjectMacro_s* macrosToFollow;
  } iVtObjectOutputList_s;

  typedef struct iVtObjectOutputNumber_s {
    uint16_t ID;
    uint16_t width;
    uint16_t height;
    uint8_t backgroundColour;
    iVtObjectFontAttributes_c* fontAttributes;
    uint8_t options;
    iVtObject_c* variableReference;
    uint32_t value;
    int32_t offset;
    float scale;
    uint8_t numberOfDecimals;
    uint8_t format;
    uint8_t horizontalJustification;
    uint8_t numberOfMacrosToFollow;
    const repeat_event_iVtObjectMacro_s* macrosToFollow;
  } iVtObjectOutputNumber_s;

  typedef struct iVtObjectOutputString_s {
    uint16_t ID;
    uint16_t width;
    uint16_t height;
    uint8_t backgroundColour;
    iVtObjectFontAttributes_c* fontAttributes;
    uint8_t options;
    iVtObjectStringVariable_c* variableReference;
    uint8_t horizontalJustification;
    uint16_t length;
    char* value; /* size length+1 (0-termination intern!) */
    uint8_t numberOfMacrosToFollow;
    const repeat_event_iVtObjectMacro_s* macrosToFollow;
  } iVtObjectOutputString_s;

  typedef struct iVtObjectPictureGraphic_s {
    uint16_t ID;
    uint16_t width;
    uint16_t actualWidth;
    uint16_t actualHeight;
    uint8_t format;
    uint8_t options;
    uint8_t transparencyColour;
    uint32_t numberOfBytesInRawData0;
    const HUGE_MEM uint8_t* rawData0;
    uint32_t numberOfBytesInRawData1;
    const HUGE_MEM uint8_t* rawData1;
    uint32_t numberOfBytesInRawData2;
    const HUGE_MEM uint8_t* rawData2;
    uint8_t numberOfFixedBitmapsToFollow;
    const repeat_rawData_rawBytes_actWidth_actHeight_formatoptions_s* fixedBitmapsToFollow;
    uint8_t numberOfMacrosToFollow;
    const repeat_event_iVtObjectMacro_s* macrosToFollow;
  } iVtObjectPictureGraphic_s;

  typedef struct iVtObjectPolygon_s {
    uint16_t ID;
    uint16_t width;
    uint16_t height;
    iVtObjectLineAttributes_c* lineAttributes;
    iVtObjectFillAttributes_c* fillAttributes;
    uint8_t polygonType;
    uint8_t numberOfPoints;
    const repeat_x_y_s* pointsToFollow;
    uint8_t numberOfMacrosToFollow;
    const repeat_event_iVtObjectMacro_s* macrosToFollow;
  } iVtObjectPolygon_s;

  typedef struct iVtObjectRectangle_s {
    uint16_t ID;
    iVtObjectLineAttributes_c* lineAttributes;
    uint16_t width;
    uint16_t height;
    uint8_t lineSuppression;
    iVtObjectFillAttributes_c* fillAttributes;
    uint8_t numberOfMacrosToFollow;
    const repeat_event_iVtObjectMacro_s* macrosToFollow;
  } iVtObjectRectangle_s;

  typedef struct iVtObjectSoftKeyMask_s {
    uint16_t ID;
    uint8_t backgroundColour;
    uint8_t numberOfObjectsToFollow;
    const repeat_iVtObject_s* objectsToFollow;
    uint8_t numberOfMacrosToFollow;
    const repeat_event_iVtObjectMacro_s* macrosToFollow;
  } iVtObjectSoftKeyMask_s;

  typedef struct iVtObjectStringVariable_s {
    uint16_t ID;
    uint16_t length;
    char* value;
  } iVtObjectStringVariable_s;

  typedef struct iVtObjectWorkingSet_s {
    uint16_t ID;
    uint8_t backgroundColour;
    uint8_t selectable;
    iVtObjectMask_c* activeMask; // data or alarm mask
    uint8_t numberOfObjectsToFollow;
    const repeat_iVtObject_x_y_iVtObjectFontAttributes_row_col_s* objectsToFollow;
    uint8_t numberOfMacrosToFollow;
    const repeat_event_iVtObjectMacro_s* macrosToFollow;
    uint8_t numberOfLanguagesToFollow;
    const repeat_vtLanguage_s* languagesToFollow;
  } iVtObjectWorkingSet_s;

  typedef struct iVtObjectAuxiliaryInput_s {
    uint16_t ID;
    uint8_t backgroundColour;
    uint8_t functionType;
    uint8_t inputId;
    uint8_t numberOfObjectsToFollow;
    const repeat_iVtObject_x_y_iVtObjectFontAttributes_row_col_s* objectsToFollow;
  } iVtObjectAuxiliaryInput_s;

  typedef struct iVtObjectAuxiliaryFunction_s {
    uint16_t ID;
    uint8_t backgroundColour;
    uint8_t functionType;
    uint8_t numberOfObjectsToFollow;
    const repeat_iVtObject_x_y_iVtObjectFontAttributes_row_col_s* objectsToFollow;
  } iVtObjectAuxiliaryFunction_s;

  typedef struct iVtObjectGraphicsContext_s {
    uint16_t ID;
    uint16_t viewportWidth;
    uint16_t viewportHeight;
    int16_t  viewportX;       //!< Upper left corner of the viewport
    int16_t  viewportY;
    uint16_t canvasWidth;
    uint16_t canvasHeight;
    int8_t   viewportZoom;    //!< Zoom value [-32 ... 32]
    int16_t  cursorX;
    int16_t  cursorY;
    uint8_t  foregroundColour;
    uint8_t  backgroundColour;
    iVtObjectFontAttributes_c* fontAttributes;
    iVtObjectLineAttributes_c* lineAttributes;
    iVtObjectFillAttributes_c* fillAttributes;
    uint8_t  format;          //!< 0 => 1 bit/pixel, 1 => 4 bit/pixel, 2 => 8 bit/pixel
    uint8_t  options;         //!< Bit 0 transparent flag; Bit 1 line, font, fill colour usage
    uint8_t  transparencyColour;  //!< Colour to be shown transparent.
  } iVtObjectGraphicsContext_s;

  typedef struct iVtObjectAuxiliaryInput2_s {
    uint16_t ID;
    uint8_t backgroundColour;
    uint8_t functionType;
    uint8_t numberOfObjectsToFollow;
    const repeat_iVtObject_x_y_iVtObjectFontAttributes_row_col_s* objectsToFollow;
  } iVtObjectAuxiliaryInput2_s;

  typedef struct iVtObjectAuxiliaryFunction2_s {
    uint16_t ID;
    uint8_t backgroundColour;
    uint8_t functionType;
    uint8_t numberOfObjectsToFollow;
    const repeat_iVtObject_x_y_iVtObjectFontAttributes_row_col_s* objectsToFollow;
  } iVtObjectAuxiliaryFunction2_s;

  typedef struct iVtObjectAuxiliaryControlDesignatorObjectPointer_s {
    uint16_t ID;
    uint8_t pointerType;
    iVtObject_c* value;
  } iVtObjectAuxiliaryControlDesignatorObjectPointer_s;

// METHODS
public:
  // Constructor
  iVtObject_c();

  virtual ~iVtObject_c() {}

  //  Operation: getID
  uint16_t getID() const;

  //! *** API-function ***
  //! Per default, all objects are uploaded.
  //! This function inhibts objects from being uploaded
  //! to the VT. Only use this function if you know what
  //! you're doing and if there's no other way.
  //! This can be useful for omitting e.g. AuxFunction
  //! objects that should not be available due to configuration.
  void omitFromUpload();

  virtual void setOriginSKM(bool b_SKM);
  virtual void setOriginBTN(iVtObjectButton_c* p_btn);

  void setClientID (uint8_t ui8_clientID);

  /** return object type as described in the standard.
      please note that the upper byte may be used for proprietary objects.
   */
  virtual uint16_t getObjectType() const = 0;

protected: 
  iVtObject_s* vtObject_a;

  iVtObjectButton_c* p_parentButtonObject;

  struct {
    uint8_t flags:5;
    uint8_t clientId:3; // when changing, adapt the assertion in "setClientID(..)"!
  } s_properties;
};

inline
void
iVtObject_c::omitFromUpload()
{
  s_properties.flags = uint8_t(s_properties.flags | FLAG_OMIT_OBJECT);
}


inline
void
iVtObject_c::setClientID( uint8_t ui8_clientID )
{
  isoaglib_header_assert( ui8_clientID < 8 ); // 3 bits only, see struct s_clientId
  s_properties.clientId = (ui8_clientID & 0x07); // "&" to avoid -Wconversion warning
}


} // IsoAgLib

#endif
