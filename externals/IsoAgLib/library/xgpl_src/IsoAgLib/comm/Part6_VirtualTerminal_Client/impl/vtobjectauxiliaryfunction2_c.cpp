/*
  vtobjectauxiliaryfunction2_c.cpp

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/

#include "vtobjectauxiliaryfunction2_c.h"

#ifdef USE_VTOBJECT_auxiliaryfunction2
#include "../ivtobjectfontattributes_c.h"
#include "../ivtobjectbutton_c.h"
#include "vtclient_c.h"


namespace __IsoAgLib {

int16_t
vtObjectAuxiliaryFunction2_c::stream(uint8_t* destMemory, uint16_t maxBytes, objRange_t sourceOffset)
{
#define MACRO_vtObjectTypeA vtObjectAuxiliaryFunction2_a
#define MACRO_vtObjectTypeS iVtObjectAuxiliaryFunction2_s
  MACRO_streamLocalVars;
  MACRO_scaleLocalVars;
  if (sourceOffset == 0) { // dump out constant sized stuff
    destMemory [0] = vtObject_a->ID & 0xFF;
    destMemory [1] = vtObject_a->ID >> 8;
    destMemory [2] = 31; // Object Type = Auxiliary Function 2
    destMemory [3] = __IsoAgLib::getVtClientInstance4Comm().getClientByID (s_properties.clientId).getUserClippedColor (vtObjectAuxiliaryFunction2_a->backgroundColour, this, IsoAgLib::BackgroundColour);
    destMemory [4] = vtObjectAuxiliaryFunction2_a->functionType;
    destMemory [5] = vtObjectAuxiliaryFunction2_a->numberOfObjectsToFollow;
    sourceOffset += 6;
    curBytes += 6;
  }
  
  MACRO_streamObjectXYcenteredInSoftKey(6);
  
  return curBytes;
}


vtObjectAuxiliaryFunction2_c::vtObjectAuxiliaryFunction2_c()
  : m_matchingPreferredAssignedInputReady(false)
{}

uint32_t
vtObjectAuxiliaryFunction2_c::fitTerminal() const
{
  MACRO_localVars;
  return 6+vtObjectAuxiliaryFunction2_a->numberOfObjectsToFollow*6;
}

bool
vtObjectAuxiliaryFunction2_c::moveChildLocation(IsoAgLib::iVtObject_c* apc_childObject, int8_t dx, int8_t dy, bool b_updateObject, bool b_enableReplaceOfCmd)
{
  MACRO_localVars;
  return genericChangeChildLocation (apc_childObject, dx, dy, b_updateObject, vtObjectAuxiliaryFunction2_a->numberOfObjectsToFollow, const_cast<IsoAgLib::repeat_iVtObject_x_y_iVtObjectFontAttributes_row_col_s *> (vtObjectAuxiliaryFunction2_a->objectsToFollow), MACRO_getStructOffset(get_vtObjectAuxiliaryFunction2_a(), objectsToFollow), sizeof(iVtObjectAuxiliaryFunction2_s), b_enableReplaceOfCmd);
}

bool
vtObjectAuxiliaryFunction2_c::setChildPosition(IsoAgLib::iVtObject_c* apc_childObject, int16_t x, int16_t y, bool b_updateObject, bool b_enableReplaceOfCmd)
{
  MACRO_localVars;
  return genericChangeChildPosition (apc_childObject, x, y, b_updateObject, vtObjectAuxiliaryFunction2_a->numberOfObjectsToFollow, const_cast<IsoAgLib::repeat_iVtObject_x_y_iVtObjectFontAttributes_row_col_s *> (vtObjectAuxiliaryFunction2_a->objectsToFollow), MACRO_getStructOffset(get_vtObjectAuxiliaryFunction2_a(), objectsToFollow), sizeof(iVtObjectAuxiliaryFunction2_s), b_enableReplaceOfCmd);
}


void
vtObjectAuxiliaryFunction2_c::setOriginSKM(bool /* b_SKM */)
{
  MACRO_localVars;
  for (int i=0; i<vtObjectAuxiliaryFunction2_a->numberOfObjectsToFollow; i++) {
    vtObjectAuxiliaryFunction2_a->objectsToFollow[i].vtObject->setOriginSKM (true);
  }
}


bool
vtObjectAuxiliaryFunction2_c::setAssignedInput(const IsoName_c& arc_isoName, uint16_t aui16_inputUid)
{
  const AssignedInput_s assignedInputBackup = ms_assignedInput;

  if (aui16_inputUid != 0xFFFF)
  { // store assignment
    ms_assignedInput.mc_inputIsoName = arc_isoName;
  }
  else
  { // remove assignment
    ms_assignedInput.mc_inputIsoName = IsoName_c::IsoNameUnspecified();
    // reset timestamp
  }
  ms_assignedInput.mui16_inputUid = aui16_inputUid;

  // true if changed
  return !(assignedInputBackup == ms_assignedInput);
}

bool
vtObjectAuxiliaryFunction2_c::unassignInputIfIsoNameMatches(const IsoName_c& arc_isoName)
{
  if (arc_isoName != ms_assignedInput.mc_inputIsoName)
    return false; 
  else
    return setAssignedInput(arc_isoName, 0xFFFF); // unassign;
}


bool
vtObjectAuxiliaryFunction2_c::setPreferredAssignedInput(const IsoName_c& arc_isoName, uint16_t aui16_inputModelIdentificationCode, uint16_t aui16_inputUid)
{
  const PreferredAssignedInput_s preferredAssignedInputBackup = ms_preferredAssignedInput;

  if (aui16_inputUid != 0xFFFF)
  { // store assignment
    ms_preferredAssignedInput.mc_inputIsoName = arc_isoName;
  }
  else
  {
    ms_preferredAssignedInput.mc_inputIsoName = IsoName_c::IsoNameUnspecified();
  }
  ms_preferredAssignedInput.mui16_inputModelIdentificationCode = aui16_inputModelIdentificationCode;
  ms_preferredAssignedInput.mui16_inputUid = aui16_inputUid;

  // true if changed
  return !(preferredAssignedInputBackup == ms_preferredAssignedInput);
}

void
vtObjectAuxiliaryFunction2_c::getPreferredAssignedInput(IsoName_c& arc_isoName, uint16_t& arui16_inputModelIdentificationCode, uint16_t& arui16_inputUid) const
{
  arc_isoName = ms_preferredAssignedInput.mc_inputIsoName;
  arui16_inputModelIdentificationCode = ms_preferredAssignedInput.mui16_inputModelIdentificationCode;
  arui16_inputUid = ms_preferredAssignedInput.mui16_inputUid;
}

bool
vtObjectAuxiliaryFunction2_c::isAssigned() const
{
  return (0xFFFF != ms_assignedInput.mui16_inputUid);
}

void
vtObjectAuxiliaryFunction2_c::getAssignedInput(IsoName_c& arc_isoName, uint16_t& arui16_inputUid) const
{
  arc_isoName = ms_assignedInput.mc_inputIsoName;
  arui16_inputUid = ms_assignedInput.mui16_inputUid;
}


bool
vtObjectAuxiliaryFunction2_c::hasPreferredAssigment() const
{
  return (IsoName_c::IsoNameUnspecified() != ms_preferredAssignedInput.mc_inputIsoName);
}


} // __IsoAgLib
#endif
