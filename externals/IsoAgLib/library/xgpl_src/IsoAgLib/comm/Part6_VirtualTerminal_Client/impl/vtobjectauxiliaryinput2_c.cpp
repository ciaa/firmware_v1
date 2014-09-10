/*
  vtobjectauxiliaryinput2_c.cpp

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/
#include "vtobjectauxiliaryinput2_c.h"

#ifdef USE_VTOBJECT_auxiliaryinput2
#include "../ivtobjectfontattributes_c.h"
#include "../ivtobjectbutton_c.h"
#include "vtclient_c.h"

#include <IsoAgLib/driver/system/isystem_c.h>


namespace __IsoAgLib {

int16_t
vtObjectAuxiliaryInput2_c::stream(uint8_t* destMemory, uint16_t maxBytes, objRange_t sourceOffset)
{
#define MACRO_vtObjectTypeA vtObjectAuxiliaryInput2_a
#define MACRO_vtObjectTypeS iVtObjectAuxiliaryInput2_s
  MACRO_streamLocalVars;
  MACRO_scaleLocalVars;
  if (sourceOffset == 0) { // dump out constant sized stuff
    destMemory [0] = vtObject_a->ID & 0xFF;
    destMemory [1] = vtObject_a->ID >> 8;
    destMemory [2] = 32; // Object Type = Auxiliary Input 2
    destMemory [3] = __IsoAgLib::getVtClientInstance4Comm().getClientByID (s_properties.clientId).getUserClippedColor (vtObjectAuxiliaryInput2_a->backgroundColour, this, IsoAgLib::BackgroundColour);
    destMemory [4] = vtObjectAuxiliaryInput2_a->functionType;
    destMemory [5] = vtObjectAuxiliaryInput2_a->numberOfObjectsToFollow;
    sourceOffset += 6;
    curBytes += 6;
  }

  MACRO_streamObjectXYcenteredInSoftKey(6);

  return curBytes;
}


// Operation : vtObjectAuxiliaryInput2_c
vtObjectAuxiliaryInput2_c::vtObjectAuxiliaryInput2_c()
  : m_inputState(),
    mui16_value1(0),
    mui16_value2(0),
    mb_inputActivatedInLearnMode(false),
    mb_highStatusUpdateRate(false),
    mb_valueChangeToHandle(false)
{}

void vtObjectAuxiliaryInput2_c::init(const iVtObjectAuxiliaryInput2_s* vtObjectAuxiliaryInput2SROM MULTITON_INST_PARAMETER_DEF_WITH_COMMA)
{
  vtObject_c::init((iVtObject_s *)vtObjectAuxiliaryInput2SROM MULTITON_INST_PARAMETER_USE_WITH_COMMA);

  // set reserved value 0xFFFF for mui16_value2 for some function types
  switch (getFunctionType())
  {
    case FunctionType_Analog:
    case FunctionType_AnalogReturnTo50:
    case FunctionType_AnalogReturnTo0:
      mui16_value2 = 0xFFFF;
      break;
    case FunctionType_LatchingBoolean:
    case FunctionType_NonLatchingBoolean:
    case FunctionType_BothLatchingDualBoolean:
    case FunctionType_BothNonLatchingDualBoolean:
    case FunctionType_LatchingDualBooleanUp:
    case FunctionType_LatchingDualBooleanDown:
    case FunctionType_CombinedAnalogReturnTo50DualLatchingBoolean:
    case FunctionType_CombinedAnalogDualLatchingBoolean:
    case FunctionType_NonLatchingQuadBoolean:
    case FunctionType_AnalogQuad:
    case FunctionType_AnalogReturnTo50Quad:
    case FunctionType_Encoder:
      mui16_value2 = 0;
      break;
  }
}


uint32_t
vtObjectAuxiliaryInput2_c::fitTerminal() const
{
  MACRO_localVars;
  return 6+vtObjectAuxiliaryInput2_a->numberOfObjectsToFollow*6;
}


bool
vtObjectAuxiliaryInput2_c::moveChildLocation(IsoAgLib::iVtObject_c* apc_childObject, int8_t dx, int8_t dy, bool b_updateObject, bool b_enableReplaceOfCmd)
{
  MACRO_localVars;
  return genericChangeChildLocation (apc_childObject, dx, dy, b_updateObject, vtObjectAuxiliaryInput2_a->numberOfObjectsToFollow, const_cast<IsoAgLib::repeat_iVtObject_x_y_iVtObjectFontAttributes_row_col_s *> (vtObjectAuxiliaryInput2_a->objectsToFollow), MACRO_getStructOffset(get_vtObjectAuxiliaryInput2_a(), objectsToFollow), sizeof(iVtObjectAuxiliaryInput2_s), b_enableReplaceOfCmd);
}

bool
vtObjectAuxiliaryInput2_c::setChildPosition(IsoAgLib::iVtObject_c* apc_childObject, int16_t x, int16_t y, bool b_updateObject, bool b_enableReplaceOfCmd)
{
  MACRO_localVars;
  return genericChangeChildPosition (apc_childObject, x, y, b_updateObject, vtObjectAuxiliaryInput2_a->numberOfObjectsToFollow, const_cast<IsoAgLib::repeat_iVtObject_x_y_iVtObjectFontAttributes_row_col_s *> (vtObjectAuxiliaryInput2_a->objectsToFollow), MACRO_getStructOffset(get_vtObjectAuxiliaryInput2_a(), objectsToFollow), sizeof(iVtObjectAuxiliaryInput2_s), b_enableReplaceOfCmd);
}


void
vtObjectAuxiliaryInput2_c::setOriginSKM(bool /* b_SKM */)
{
  MACRO_localVars;
  for (int i=0; i<vtObjectAuxiliaryInput2_a->numberOfObjectsToFollow; i++) {
    vtObjectAuxiliaryInput2_a->objectsToFollow[i].vtObject->setOriginSKM (true);
  }
}


bool
vtObjectAuxiliaryInput2_c::setValue(uint16_t aui16_value1, uint16_t aui16_value2, StateForLearnMode_en aen_stateForLearnMode)
{
  uint16_t oldValue1 = mui16_value1;
  uint16_t oldValue2 = mui16_value2;

#ifndef NDEBUG
  // skip these checks for release builds

  // range check
  switch (getFunctionType())
  {
    case FunctionType_LatchingBoolean:
      isoaglib_assert(aui16_value1 <= 1);
      break;

    case FunctionType_Analog:
    case FunctionType_AnalogReturnTo50:
    case FunctionType_AnalogReturnTo0:
      isoaglib_assert(aui16_value1 <= 0xFAFF);
      break;

    case FunctionType_NonLatchingBoolean:
      isoaglib_assert(aui16_value1 <= 2);
      break;

    case FunctionType_BothLatchingDualBoolean:
      isoaglib_assert((aui16_value1 == 0) || (aui16_value1 == 1) || (aui16_value1 == 4));
      break;

    case FunctionType_BothNonLatchingDualBoolean:
      isoaglib_assert((aui16_value1 == 0) || (aui16_value1 == 1) || (aui16_value1 == 2) || (aui16_value1 == 4) || (aui16_value1 == 8));
      break;

    case FunctionType_LatchingDualBooleanUp:
      isoaglib_assert((aui16_value1 == 0) || (aui16_value1 == 1) || (aui16_value1 == 4) || (aui16_value1 == 8));
      break;

    case FunctionType_LatchingDualBooleanDown:
      isoaglib_assert((aui16_value1 == 0) || (aui16_value1 == 1) || (aui16_value1 == 2) || (aui16_value1 == 4));
      break;

    case FunctionType_CombinedAnalogReturnTo50DualLatchingBoolean:
    case FunctionType_CombinedAnalogDualLatchingBoolean:
      isoaglib_assert((aui16_value1 <= 0xFAFF) || (aui16_value1 == 0xFB00) || (aui16_value1 == 0xFB01));
      break;

    case FunctionType_NonLatchingQuadBoolean:
      isoaglib_assert( ((aui16_value1 & 0x03) != 0x03) && ((aui16_value1 & 0x0C) != 0x0C) && ((aui16_value1 & 0x30) != 0x30) && ((aui16_value1 & 0xC0) != 0xC0) );
      break;

    case FunctionType_AnalogQuad:
    case FunctionType_AnalogReturnTo50Quad:
      isoaglib_assert((aui16_value1 <= 0xFAFF) && (aui16_value2 <= 0xFAFF));
      break;

    case FunctionType_Encoder:
      isoaglib_assert(0 != aui16_value2);
      break;
  }

#endif

  switch (aen_stateForLearnMode)
  {
    case StateForLearnMode_CalculateInternally:
      if (0 != aui16_value1)
        mb_inputActivatedInLearnMode = true;
      else
        mb_inputActivatedInLearnMode = false;
      break;
    case StateForLearnMode_SetActive:
      mb_inputActivatedInLearnMode = true;
      break;
    case StateForLearnMode_SetInactive:
      mb_inputActivatedInLearnMode = false;
      break;
  }

  // set mui16_value2 for some cases
  switch (getFunctionType())
  {
    case FunctionType_LatchingBoolean:
    case FunctionType_NonLatchingBoolean:
      if ((oldValue1 != 0) && (aui16_value1 == 0))
      { // on -> off ==> fine, it's a change!
        break;
      }
      if ((oldValue1 == 0) && (aui16_value1 == 1))
      { // off -> on ==> fine, it's a change!
        mui16_value2++; // and count the transition
        break;
      }
      // no real change, do not trigger any (unneeded) updates!
      return true; 
    case FunctionType_BothLatchingDualBoolean:
    case FunctionType_BothNonLatchingDualBoolean:
    case FunctionType_LatchingDualBooleanUp:
    case FunctionType_LatchingDualBooleanDown:
    case FunctionType_CombinedAnalogReturnTo50DualLatchingBoolean:
    case FunctionType_CombinedAnalogDualLatchingBoolean:
    case FunctionType_NonLatchingQuadBoolean:
      // @todo the following is maybe not correct for all of these types
      // but it's not very precisely specified in the standard, so we
      // don't care to much right now until someone uses these...
      if (oldValue1 != aui16_value1)
        mui16_value2++; // increase transition counter
      break;
    case FunctionType_AnalogQuad:
    case FunctionType_AnalogReturnTo50Quad:
    case FunctionType_Encoder:
      mui16_value2 = aui16_value2;
      break;

    case FunctionType_Analog:
    case FunctionType_AnalogReturnTo50:
    case FunctionType_AnalogReturnTo0:
      break;
  }

  mui16_value1 = aui16_value1;

  // special handling for non latched boolean
  switch (getFunctionType())
  {
    case FunctionType_NonLatchingBoolean:
    case FunctionType_BothNonLatchingDualBoolean:
    case FunctionType_NonLatchingQuadBoolean:
      mb_highStatusUpdateRate = (mui16_value1 != 0);
      break;

    case FunctionType_LatchingDualBooleanUp:
      mb_highStatusUpdateRate = (mui16_value1 == 4);
      break;

    case FunctionType_LatchingDualBooleanDown:
      mb_highStatusUpdateRate = (mui16_value1 == 1);
      break;

    case FunctionType_LatchingBoolean:
    case FunctionType_Analog:
    case FunctionType_AnalogReturnTo50:
    case FunctionType_AnalogReturnTo0:
    case FunctionType_BothLatchingDualBoolean:
    case FunctionType_CombinedAnalogReturnTo50DualLatchingBoolean:
    case FunctionType_CombinedAnalogDualLatchingBoolean:
    case FunctionType_AnalogQuad:
    case FunctionType_AnalogReturnTo50Quad:
    case FunctionType_Encoder:
      break;
  }

  if ((oldValue1 != mui16_value1) || (oldValue2 != mui16_value2))
  {
    // mb_valueChangeToHandle is set to false in updateValueForHeldPositionAfterFirstMsg(), 
    // called by triggerAux2InputStatusMsg() when the state message is sent successfully (no timing restrictions: 200msec minimum time diff)
    // As long as the message can not be sent, mb_valueChangeToHandle stays true 
    // => message sending is tried in Aux2Inputs_c::timeEventInputStateMsg() until timing allows message sending
    mb_valueChangeToHandle = true;
    __IsoAgLib::getVtClientInstance4Comm().getClientByID (s_properties.clientId).triggerAux2InputStatusMsg(this);
  }

  return true;
      
}


void
vtObjectAuxiliaryInput2_c::updateValueForHeldPositionAfterFirstMsg()
{
  // message successfully sent => reset flag
  mb_valueChangeToHandle = false;

  switch (getFunctionType())
  {
    case FunctionType_NonLatchingBoolean:
      switch (mui16_value1)
      {
        case 1: mui16_value1 = 2; break;
        default: break;
      }
      break;

    case FunctionType_BothNonLatchingDualBoolean:
      switch (mui16_value1)
      {
        case 1: mui16_value1 = 2; break;
        case 4: mui16_value1 = 8; break;
        default: break;
      }
      break;

    case FunctionType_LatchingDualBooleanUp:
      switch (mui16_value1)
      {
        case 4: mui16_value1 = 8; break;
        default: break;
      }
      break;

    case FunctionType_LatchingDualBooleanDown:
      switch (mui16_value1)
      {
        case 1: mui16_value1 = 2; break;
        default: break;
      }
      break;

    case FunctionType_NonLatchingQuadBoolean:
      // transition from 0x01 to 0x02
      if ((mui16_value1 & 0x03) == 0x01)
        mui16_value1 = mui16_value1 - 0x01 + 0x02;
      if ((mui16_value1 & 0x0C) == 0x04)
        mui16_value1 = mui16_value1 - 0x04 + 0x08;
      if ((mui16_value1 & 0x30) == 0x10)
        mui16_value1 = mui16_value1 - 0x10 + 0x20;
      if ((mui16_value1 & 0xC0) == 0x40)
        mui16_value1 = mui16_value1 - 0x40 + 0x80;
      break;

    case FunctionType_LatchingBoolean:
    case FunctionType_Analog:
    case FunctionType_AnalogReturnTo50:
    case FunctionType_AnalogReturnTo0:
    case FunctionType_BothLatchingDualBoolean:
    case FunctionType_CombinedAnalogReturnTo50DualLatchingBoolean:
    case FunctionType_CombinedAnalogDualLatchingBoolean:
    case FunctionType_AnalogQuad:
    case FunctionType_AnalogReturnTo50Quad:
    case FunctionType_Encoder:
      break;
  }

}


void
vtObjectAuxiliaryInput2_c::setInputStateEnabled(bool ab_enabled)
{
  m_inputState.mb_enabled = ab_enabled;
}

void vtObjectAuxiliaryInput2_c::setTimeStampLastStateMsg()
{
  m_inputState.mui32_timeStampLastStateMsg = HAL::getTime();
}


} // __IsoAgLib
#endif
