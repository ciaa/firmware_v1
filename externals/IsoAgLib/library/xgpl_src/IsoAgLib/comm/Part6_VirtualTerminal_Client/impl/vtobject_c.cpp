/*
  vtobject_c.cpp

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/

#include "vtobject_c.h"
#include "../ivtobject_c.h"

#include "vtclient_c.h"
#include "vtclientconnection_c.h"
#include <IsoAgLib/util/impl/util_funcs.h>
#include <IsoAgLib/util/iassert.h>


namespace IsoAgLib {
  // implement here a normal constructor and functions, as the compiler dislikes inlining of that simple
  // constructor/functions direct in scope of iVtObject_c
  iVtObject_c::iVtObject_c() :
    vtObject_a(NULL),
    p_parentButtonObject(NULL)
  {
    s_properties.flags = 0;
    s_properties.clientId = 0;
  }

  uint16_t iVtObject_c::getID() const
  {
    isoaglib_assert(vtObject_a);
    return vtObject_a->ID;
  }
}


namespace __IsoAgLib {

vtObject_c::vtObject_c( ) {}

vtObject_c::~vtObject_c() {}


void
vtObject_c::setAttribute(uint8_t attrID, uint32_t newValue, bool b_enableReplaceOfCmd)
{
  __IsoAgLib::getVtClientInstance4Comm().getClientByID (s_properties.clientId).commandHandler().sendCommandChangeAttribute(
    this, attrID, newValue & 0xFF, (newValue >> 8) & 0xFF, (newValue >> 16) & 0xFF, newValue >> 24, b_enableReplaceOfCmd );
}


void
vtObject_c::setAttributeFloat(uint8_t attrID, float newValue, bool b_enableReplaceOfCmd)
{
#ifdef OPTIMIZE_NUMBER_CONVERSIONS_FOR_LITTLE_ENDIAN
  uint32_t ui32_convertedFloat;
  floatVar2LittleEndianStream (&newValue, &ui32_convertedFloat);
#else
  uint8_t ui8_convertedFloat[4];
  floatVar2LittleEndianStream (&newValue, &ui8_convertedFloat);
  uint32_t ui32_convertedFloat = __IsoAgLib::convertLittleEndianStringUi32(ui8_convertedFloat);
#endif

  setAttribute (attrID, ui32_convertedFloat, b_enableReplaceOfCmd);
}

#ifdef USE_ISO_TERMINAL_GETATTRIBUTES
void
vtObject_c::getAttribute(uint8_t attrID, bool b_enableReplaceOfCmd)
{
  __IsoAgLib::getVtClientInstance4Comm().getClientByID (s_properties.clientId).sendCommandGetAttributeValue (this, attrID, b_enableReplaceOfCmd);
}
#endif


void
vtObject_c::createRamStructIfNotYet (uint16_t ui16_structLen)
{ // Do we have to generate a RAM copy of our struct (to save the value), or has this already be done?
  if (!(s_properties.flags & FLAG_IN_RAM)) {
    void* old=vtObject_a;
    vtObject_a = (iVtObject_s*) new uint8_t [ui16_structLen];
    CNAMESPACE::memcpy (vtObject_a, old, ui16_structLen);
    s_properties.flags |= FLAG_IN_RAM;
  }
}


// //////////////////////////////// saveValue(8/16/32)
void
vtObject_c::saveValue8 (uint16_t ui16_structOffset, uint16_t ui16_structLen, uint8_t ui8_newValue)
{
  createRamStructIfNotYet (ui16_structLen);
  ((uint8_t *)vtObject_a) [ui16_structOffset] = ui8_newValue;
}

void
vtObject_c::saveValue16 (uint16_t ui16_structOffset, uint16_t ui16_structLen, uint16_t ui16_newValue)
{
  createRamStructIfNotYet (ui16_structLen);
  * ((uint16_t*) (((uint8_t *)vtObject_a)+ui16_structOffset)) = ui16_newValue;
}

void
vtObject_c::saveValue32 (uint16_t ui16_structOffset, uint16_t ui16_structLen, uint32_t ui32_newValue)
{
  createRamStructIfNotYet (ui16_structLen);
  * ((uint32_t*) (((uint8_t *)vtObject_a)+ui16_structOffset)) = ui32_newValue;
}

void
vtObject_c::saveSignedValue8 (uint16_t ui16_structOffset, uint16_t ui16_structLen, int8_t i8_newValue)
{
  createRamStructIfNotYet (ui16_structLen);
  ((int8_t *)vtObject_a) [ui16_structOffset] = i8_newValue;
}

void
vtObject_c::saveSignedValue16 (uint16_t ui16_structOffset, uint16_t ui16_structLen, int16_t i16_newValue)
{
  createRamStructIfNotYet (ui16_structLen);
  * ((int16_t*) (((uint8_t *)vtObject_a)+ui16_structOffset)) = i16_newValue;
}

void
vtObject_c::saveSignedValue32 (uint16_t ui16_structOffset, uint16_t ui16_structLen, int32_t i32_newValue)
{
  createRamStructIfNotYet (ui16_structLen);
  * ((int32_t*) (((uint8_t *)vtObject_a)+ui16_structOffset)) = i32_newValue;
}

void
vtObject_c::saveValueFloat (uint16_t ui16_structOffset, uint16_t ui16_structLen, float f_newValue)
{
  createRamStructIfNotYet (ui16_structLen);
  * ((float*) (((uint8_t *)vtObject_a)+ui16_structOffset)) = f_newValue;
}

void
vtObject_c::saveValueP (uint16_t ui16_structOffset, uint16_t ui16_structLen, const IsoAgLib::iVtObject_c* const p_newValue)
{
  createRamStructIfNotYet (ui16_structLen);
  * ((const IsoAgLib::iVtObject_c**) (((uint8_t *)vtObject_a)+ui16_structOffset)) = p_newValue;
}

void
vtObject_c::saveValueISOName (const uint16_t ui16_structOffset, const uint16_t ui16_structLen, const IsoAgLib::iIsoName_c& ar_newIsoName)
{
  createRamStructIfNotYet (ui16_structLen);
  *((IsoAgLib::iIsoName_c*) (((uint8_t *)vtObject_a)+ui16_structOffset)) = ar_newIsoName;
}

void
vtObject_c::saveValueBool (const uint16_t ui16_structOffset, const uint16_t ui16_structLen, bool b_newValue)
{
  createRamStructIfNotYet (ui16_structLen);
  *((bool*) (((uint8_t *)vtObject_a)+ui16_structOffset)) = b_newValue;
}


// //////////////////////////////// saveValue(8/16/32)SetAttribute
void
vtObject_c::saveValue8SetAttribute (uint16_t ui16_structOffset, uint16_t ui16_structLen, uint8_t ui8_ind, uint8_t ui8_newValue, uint8_t ui8_newValueSend, bool b_enableReplaceOfCmd) {
  if (ui16_structOffset != 0) saveValue8 (ui16_structOffset, ui16_structLen, ui8_newValue);
  setAttribute (ui8_ind, (uint32_t) ui8_newValueSend, b_enableReplaceOfCmd);
}

void
vtObject_c::saveValue16SetAttribute (uint16_t ui16_structOffset, uint16_t ui16_structLen, uint8_t ui8_ind, uint16_t ui16_newValue, bool b_enableReplaceOfCmd) {
  if (ui16_structOffset != 0) saveValue16 (ui16_structOffset, ui16_structLen, ui16_newValue);
  setAttribute (ui8_ind, (uint32_t) ui16_newValue, b_enableReplaceOfCmd);
}

void
vtObject_c::saveValue32SetAttribute (uint16_t ui16_structOffset, uint16_t ui16_structLen, uint8_t ui8_ind, uint32_t ui32_newValue, bool b_enableReplaceOfCmd) {
  if (ui16_structOffset != 0) saveValue32 (ui16_structOffset, ui16_structLen, ui32_newValue);
  setAttribute (ui8_ind, ui32_newValue, b_enableReplaceOfCmd);
}

void
vtObject_c::saveValueFloatSetAttribute (uint16_t ui16_structOffset, uint16_t ui16_structLen, uint8_t ui8_ind, float f_newValue, bool b_enableReplaceOfCmd) {
  if (ui16_structOffset != 0) saveValueFloat (ui16_structOffset, ui16_structLen, f_newValue);
  setAttributeFloat (ui8_ind, f_newValue, b_enableReplaceOfCmd);
}

void
vtObject_c::saveValuePSetAttribute (uint16_t ui16_structOffset, uint16_t ui16_structLen, uint8_t ui8_ind, IsoAgLib::iVtObject_c* p_newValue, bool b_enableReplaceOfCmd) {
  if (ui16_structOffset != 0) saveValueP (ui16_structOffset, ui16_structLen, p_newValue);
  setAttribute (ui8_ind, (p_newValue == NULL) ? 65535 : p_newValue->getID(), b_enableReplaceOfCmd);
}


// //////////////////////////////// GetAttributeValue 8/16/32
uint8_t
vtObject_c::getValue8 (uint16_t ui16_structOffset, uint16_t ui16_structLen, bool b_createRamStructIfNeeded)
{
  if (b_createRamStructIfNeeded) createRamStructIfNotYet (ui16_structLen);
  return * (((uint8_t *)vtObject_a)+ui16_structOffset);
}

uint16_t
vtObject_c::getValue16 (uint16_t ui16_structOffset, uint16_t ui16_structLen, bool b_createRamStructIfNeeded)
{
  if (b_createRamStructIfNeeded) createRamStructIfNotYet (ui16_structLen);
  return * ((uint16_t*) (((uint8_t *)vtObject_a)+ui16_structOffset));
}

uint32_t
vtObject_c::getValue32 (uint16_t ui16_structOffset, uint16_t ui16_structLen, bool b_createRamStructIfNeeded)
{
  if (b_createRamStructIfNeeded) createRamStructIfNotYet (ui16_structLen);
  return * ((uint32_t*) (((uint8_t *)vtObject_a)+ui16_structOffset));
}

int8_t
vtObject_c::getSignedValue8 (uint16_t ui16_structOffset, uint16_t ui16_structLen, bool b_createRamStructIfNeeded)
{
  if (b_createRamStructIfNeeded) createRamStructIfNotYet (ui16_structLen);
  return * (((int8_t *)vtObject_a)+ui16_structOffset);
}

int16_t
vtObject_c::getSignedValue16 (uint16_t ui16_structOffset, uint16_t ui16_structLen, bool b_createRamStructIfNeeded)
{
  if (b_createRamStructIfNeeded)  createRamStructIfNotYet (ui16_structLen);
  return * ((int16_t*) (((uint8_t *)vtObject_a)+ui16_structOffset));
}

int32_t
vtObject_c::getSignedValue32 (uint16_t ui16_structOffset, uint16_t ui16_structLen, bool b_createRamStructIfNeeded)
{
  if (b_createRamStructIfNeeded) createRamStructIfNotYet (ui16_structLen);
  return * ((int32_t*) (((uint8_t *)vtObject_a)+ui16_structOffset));
}

float
vtObject_c::getValueFloat (uint16_t ui16_structOffset, uint16_t ui16_structLen, bool b_createRamStructIfNeeded)
{
  if (b_createRamStructIfNeeded) createRamStructIfNotYet (ui16_structLen);
  return * ((float*) (((uint8_t *)vtObject_a)+ui16_structOffset));
}

#ifdef USE_ISO_TERMINAL_GETATTRIBUTES
// //////////////////////////////// get(Signed)Value(8/16/32)GetAttribute
uint8_t
vtObject_c::getValue8GetAttribute (uint16_t ui16_structOffset, uint16_t ui16_structLen, uint8_t ui8_ind, bool b_enableReplaceOfCmd) {
  getAttribute (ui8_ind, b_enableReplaceOfCmd);
  return getValue8 (ui16_structOffset, ui16_structLen);
}
uint16_t
vtObject_c::getValue16GetAttribute (uint16_t ui16_structOffset, uint16_t ui16_structLen, uint8_t ui8_ind, bool b_enableReplaceOfCmd) {
  getAttribute (ui8_ind, b_enableReplaceOfCmd);
  return getValue16 (ui16_structOffset, ui16_structLen);
}
uint32_t
vtObject_c::getValue32GetAttribute (uint16_t ui16_structOffset, uint16_t ui16_structLen, uint8_t ui8_ind, bool b_enableReplaceOfCmd) {
  getAttribute (ui8_ind, b_enableReplaceOfCmd);
  return getValue32 (ui16_structOffset, ui16_structLen);
}
int8_t
vtObject_c::getSignedValue8GetAttribute (uint16_t ui16_structOffset, uint16_t ui16_structLen, uint8_t ui8_ind, bool b_enableReplaceOfCmd) {
  getAttribute (ui8_ind, b_enableReplaceOfCmd);
  return getSignedValue8 (ui16_structOffset, ui16_structLen);
}
int16_t
vtObject_c::getSignedValue16GetAttribute (uint16_t ui16_structOffset, uint16_t ui16_structLen, uint8_t ui8_ind, bool b_enableReplaceOfCmd) {
  getAttribute (ui8_ind, b_enableReplaceOfCmd);
  return getSignedValue16 (ui16_structOffset, ui16_structLen);
}
int32_t
vtObject_c::getSignedValue32GetAttribute (uint16_t ui16_structOffset, uint16_t ui16_structLen, uint8_t ui8_ind, bool b_enableReplaceOfCmd) {
  getAttribute (ui8_ind, b_enableReplaceOfCmd);
  return getSignedValue32 (ui16_structOffset, ui16_structLen);
}
float
vtObject_c::getValueFloatGetAttribute (uint16_t ui16_structOffset, uint16_t ui16_structLen, uint8_t ui8_ind, bool b_enableReplaceOfCmd) {
  getAttribute (ui8_ind, b_enableReplaceOfCmd);
  return getValueFloat (ui16_structOffset, ui16_structLen);
}
#endif


bool
vtObject_c::genericChangeChildLocationPosition (bool ab_isLocation, IsoAgLib::iVtObject_c* childObject, int16_t dx, int16_t dy, bool b_updateObject, uint8_t numObjectsToFollow, IsoAgLib::repeat_iVtObject_x_y_iVtObjectFontAttributes_row_col_s* objectsToFollow, uint16_t ui16_structOffset, uint16_t ui16_structLen)
{
  // Find the child object in question
  for(int8_t i = 0; i < numObjectsToFollow; i++) {
    if (childObject->getID() == objectsToFollow[i].vtObject->getID()) {
      if (b_updateObject) {
        // Check if RAM version of objectsToFollow already exists?
        /**** he following could be called "createObjectsToFollowRamStructIfNotYet" ****/
        if (!(s_properties.flags & FLAG_OBJECTS2FOLLOW_IN_RAM)) {
          // Copy objectsToFollow structure! and use new pointer afterwards!!
          void* romObjectsToFollow = objectsToFollow;
          objectsToFollow = new IsoAgLib::repeat_iVtObject_x_y_iVtObjectFontAttributes_row_col_s [numObjectsToFollow];
          CNAMESPACE::memcpy (objectsToFollow, romObjectsToFollow, sizeof (IsoAgLib::repeat_iVtObject_x_y_iVtObjectFontAttributes_row_col_s) * numObjectsToFollow);
          // saveValue will check itself if general structure is already in RAM and can be altered
          saveValueP(ui16_structOffset, ui16_structLen, (IsoAgLib::iVtObject_c *)objectsToFollow);
          s_properties.flags |= FLAG_OBJECTS2FOLLOW_IN_RAM;
        }
        if (ab_isLocation) {
          objectsToFollow[i].x = objectsToFollow[i].x + dx;
          objectsToFollow[i].y = objectsToFollow[i].y + dy;
        } else {
          objectsToFollow[i].x = dx;
          objectsToFollow[i].y = dy;
        }
      }
      return true; // Object was child object, so its position could be changed
    }
  }
  return false; // Object was not child object
}


// only using int16_t because ISO's offset is -127 and hence the range is -127..0..+128 :-(((
bool
vtObject_c::genericChangeChildLocation (IsoAgLib::iVtObject_c* childObject, int16_t dx, int16_t dy, bool b_updateObject, uint8_t numObjectsToFollow, IsoAgLib::repeat_iVtObject_x_y_iVtObjectFontAttributes_row_col_s* objectsToFollow, uint16_t ui16_structOffset, uint16_t ui16_structLen, bool b_enableReplaceOfCmd)
{
  if ((dx < -127) || (dx > 128)
   || (dy < -127) || (dy > 128)) return false;

  bool b_result = genericChangeChildLocationPosition (true, childObject, dx, dy, b_updateObject, numObjectsToFollow, objectsToFollow, ui16_structOffset, ui16_structLen);
  if (b_result)
    __IsoAgLib::getVtClientInstance4Comm().getClientByID (s_properties.clientId).commandHandler().sendCommandChangeChildLocation(
      this, childObject, dx, dy, b_enableReplaceOfCmd );

  return b_result;
}


bool
vtObject_c::genericChangeChildPosition (IsoAgLib::iVtObject_c* childObject, int16_t x, int16_t y, bool b_updateObject, uint8_t numObjectsToFollow, IsoAgLib::repeat_iVtObject_x_y_iVtObjectFontAttributes_row_col_s* objectsToFollow, uint16_t ui16_structOffset, uint16_t ui16_structLen, bool b_enableReplaceOfCmd)
{
  bool b_result = genericChangeChildLocationPosition (false, childObject, x, y, b_updateObject, numObjectsToFollow, objectsToFollow, ui16_structOffset, ui16_structLen);
  if (b_result)
    __IsoAgLib::getVtClientInstance4Comm().getClientByID (s_properties.clientId).commandHandler().sendCommandChangeChildPosition(
      this, childObject, x, y, b_enableReplaceOfCmd );

  return b_result;
}


bool
vtObject_c::able (uint8_t enOrDis, bool b_updateObject, bool b_enableReplaceOfCmd)
{
  if (b_updateObject)
    updateEnable (enOrDis);

  return __IsoAgLib::getVtClientInstance4Comm().getClientByID (s_properties.clientId).commandHandler().sendCommand(
    0xA1 /* Command: Command --- Parameter: Enable/Disable Object */,
    vtObject_a->ID & 0xFF, vtObject_a->ID >> 8, enOrDis, 0xFF, 0xFF, 0xFF, 0xFF, b_enableReplaceOfCmd );
}


bool
vtObject_c::select(uint8_t selectOrActivate)
{
  return __IsoAgLib::getVtClientInstance4Comm().getClientByID (s_properties.clientId).commandHandler().sendCommand(
    0xA2 /* Command: Command --- Parameter: Select Input Object */,
    vtObject_a->ID & 0xFF, vtObject_a->ID >> 8, selectOrActivate, 0xFF, 0xFF, 0xFF, 0xFF, true );
}


} // __IsoAgLib
