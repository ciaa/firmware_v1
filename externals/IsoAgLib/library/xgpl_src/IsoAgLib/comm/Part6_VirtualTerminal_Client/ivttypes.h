/*
  ivttypes.h

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/
#ifndef IVTTYPES_H
#define IVTTYPES_H

#include <IsoAgLib/isoaglib_config.h>
#include <cstdlib>       // using abs()
#include <algorithm>     // using min() max()
#include "impl/vttypes.h"


#define BUTTON_HAS_BEEN_RELEASED 0
#define BUTTON_HAS_BEEN_PRESSED 1
#define BUTTON_IS_STILL_HELD 2

#define BUTTON_HAS_BEEN_UNLATCHED 0
#define BUTTON_HAS_BEEN_LATCHED 1

#define KEY_HAS_BEEN_RELEASED 0
#define KEY_HAS_BEEN_PRESSED 1
#define KEY_IS_STILL_HELD 2

#define VT_OBJECT_TYPE_WORKING_SET 0
#define VT_OBJECT_TYPE_DATA_MASK 1
#define VT_OBJECT_TYPE_ALARM_MASK 2
#define VT_OBJECT_TYPE_CONTAINER 3
#define VT_OBJECT_TYPE_WINDOW_MASK 34
#define VT_OBJECT_TYPE_SOFT_KEY_MASK 4
#define VT_OBJECT_TYPE_KEY 5
#define VT_OBJECT_TYPE_BUTTON 6
#define VT_OBJECT_TYPE_KEY_GROUP 35
#define VT_OBJECT_TYPE_INPUT_BOOLEAN 7
#define VT_OBJECT_TYPE_INPUT_STRING 8
#define VT_OBJECT_TYPE_INPUT_NUMBER 9
#define VT_OBJECT_TYPE_INPUT_LIST 10
#define VT_OBJECT_TYPE_OUTPUT_STRING 11
#define VT_OBJECT_TYPE_OUTPUT_NUMBER 12
#define VT_OBJECT_TYPE_OUTPUT_LIST 37
#define VT_OBJECT_TYPE_LINE 13
#define VT_OBJECT_TYPE_RECTANGLE 14
#define VT_OBJECT_TYPE_ELLIPSE 15
#define VT_OBJECT_TYPE_POLYGON 16
#define VT_OBJECT_TYPE_METER 17
#define VT_OBJECT_TYPE_LINEAR_BAR_GRAPH 18
#define VT_OBJECT_TYPE_ARCHED_BAR_GRAPH 19
#define VT_OBJECT_TYPE_GRAPHICS_CONTEXT 36
#define VT_OBJECT_TYPE_PICTURE_GRAPHIC 20
#define VT_OBJECT_TYPE_NUMBER_VARIABLE 21
#define VT_OBJECT_TYPE_STRING_VARIABLE 22
#define VT_OBJECT_TYPE_FONT_ATTRIBUTES 23
#define VT_OBJECT_TYPE_LINE_ATTRIBUTES 24
#define VT_OBJECT_TYPE_FILL_ATTRIBUTES 25
#define VT_OBJECT_TYPE_INPUT_ATTRIBUTES 26
#define VT_OBJECT_TYPE_EXTENDED_INPUT_ATTRIBUTES 38
#define VT_OBJECT_TYPE_COLOUR_MAP 39
#define VT_OBJECT_TYPE_OBJECT_POINTER 27
#define VT_OBJECT_TYPE_MACRO 28
#define VT_OBJECT_TYPE_AUXILIARY_FUNCTION_1 29
#define VT_OBJECT_TYPE_AUXILIARY_INPUT_1 30
#define VT_OBJECT_TYPE_AUXILIARY_FUNCTION_2 31
#define VT_OBJECT_TYPE_AUXILIARY_INPUT_2 32
#define VT_OBJECT_TYPE_AUXILIARY_POINTER 33

namespace IsoAgLib {

class iVtObject_c;
class iVtObjectWorkingSet_c;
class iVtObjectDataMask_c;
class iVtObjectAlarmMask_c;
class iVtObjectContainer_c;
class iVtObjectSoftKeyMask_c;
class iVtObjectKey_c;
class iVtObjectButton_c;
class iVtObjectInputBoolean_c;
class iVtObjectInputString_c;
class iVtObjectInputNumber_c;
class iVtObjectInputList_c;
class iVtObjectOutputString_c;
class iVtObjectOutputNumber_c;
class iVtObjectOutputList_c;
class iVtObjectLine_c;
class iVtObjectMask_c;
class iVtObjectRectangle_c;
class iVtObjectEllipse_c;
class iVtObjectPolygon_c;
class iVtObjectMeter_c;
class iVtObjectLinearBarGraph_c;
class iVtObjectArchedBarGraph_c;
class iVtObjectPictureGraphic_c;
class iVtObjectNumberVariable_c;
class iVtObjectStringVariable_c;
class iVtObjectFontAttributes_c;
class iVtObjectLineAttributes_c;
class iVtObjectFillAttributes_c;
class iVtObjectInputAttributes_c;
class iVtObjectObjectPointer_c;
class iVtObjectMacro_c;

class iVtObjectAuxiliaryFuntion_c;
class iVtObjectAuxiliaryInput_c;
class iVtObjectAuxiliaryFuntion2_c;
class iVtObjectAuxiliaryInput2_c;
class iVtObjectAuxiliaryControlDesignatorObjectPointer_c;

// +++ (attribute list) struct declarations
struct repeat_iVtObject_s {
  iVtObject_c* vtObject;
};

// +++ (attribute list) struct declarations
struct repeat_iVtObject_x_y_iVtObjectFontAttributes_row_col_s {
  iVtObject_c* vtObject;
  int16_t x;
  int16_t y;
  iVtObjectFontAttributes_c* vtObjectBlockFont;
  int16_t row;
  int16_t col;
};

struct repeat_event_iVtObjectMacro_s {
  uint8_t event;
  iVtObjectMacro_c* vtObjectMacro;
};

struct repeat_x_y_s {
  int16_t x;
  int16_t y;
};

struct repeat_vtLanguage_s {
  uint8_t language[2];
};

struct repeat_rawData_rawBytes_actWidth_actHeight_formatoptions_s {
  const HUGE_MEM uint8_t* rawData;
  uint32_t numberOfBytesInRawData;
  uint16_t actualWidth;
  uint16_t actualHeight;
  uint8_t formatoptions; /* format is bit 8+7, options is bit 2-0 (NO RLE1/4/8 stuff here!! */
};

enum e_vtColour { BackgroundColour=0, BorderColour=1, NeedleColour=2, ArcAndTickColour=3, BarGraphColour=4, Colour=4 /*intentionally!*/, TargetLineColour=5, TransparencyColour=6, FontColour=7, LineColour=8, FillColour=9 };

/** struct of the data contained in the "VT Status Message" */
typedef struct vtState_s {
  uint32_t lastReceived; /* Timestamp of last reception */

  /* the following data is extracted from one "VT Status Message" */
  uint8_t  saOfActiveWorkingSetMaster;
  uint16_t dataAlarmMask;
  uint16_t softKeyMask;
  uint8_t  busyCodes;
  uint8_t  functionBusy;
} vtState_s;

} // IsoAgLib

#endif

