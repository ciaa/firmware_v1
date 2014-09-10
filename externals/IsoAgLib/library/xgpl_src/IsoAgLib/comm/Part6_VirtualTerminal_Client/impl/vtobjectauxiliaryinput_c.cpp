/*
  vtobjectauxiliaryinput_c.cpp

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/
#include "vtobjectauxiliaryinput_c.h"

#ifdef USE_VTOBJECT_auxiliaryinput
#include "../ivtobjectfontattributes_c.h"
#include "../ivtobjectbutton_c.h"
#include "vtclient_c.h"


namespace __IsoAgLib {

int16_t
vtObjectAuxiliaryInput_c::stream(uint8_t* destMemory, uint16_t maxBytes, objRange_t sourceOffset)
{
#define MACRO_vtObjectTypeA vtObjectAuxiliaryInput_a
#define MACRO_vtObjectTypeS iVtObjectAuxiliaryInput_s
  MACRO_streamLocalVars;
  MACRO_scaleLocalVars;
  if (sourceOffset == 0) { // dump out constant sized stuff
    destMemory [0] = vtObject_a->ID & 0xFF;
    destMemory [1] = vtObject_a->ID >> 8;
    destMemory [2] = 30; // Object Type = Auxiliary Input
    destMemory [3] = __IsoAgLib::getVtClientInstance4Comm().getClientByID (s_properties.clientId).getUserClippedColor (vtObjectAuxiliaryInput_a->backgroundColour, this, IsoAgLib::BackgroundColour);
    destMemory [4] = vtObjectAuxiliaryInput_a->functionType;
    destMemory [5] = vtObjectAuxiliaryInput_a->inputId;
    destMemory [6] = vtObjectAuxiliaryInput_a->numberOfObjectsToFollow;
    sourceOffset += 7;
    curBytes += 7;
  }

  MACRO_streamObjectXYcenteredInSoftKey(7);

  return curBytes;
}


vtObjectAuxiliaryInput_c::vtObjectAuxiliaryInput_c() {}


uint32_t
vtObjectAuxiliaryInput_c::fitTerminal() const
{
  MACRO_localVars;
  return 7+vtObjectAuxiliaryInput_a->numberOfObjectsToFollow*6;
}

bool
vtObjectAuxiliaryInput_c::moveChildLocation(IsoAgLib::iVtObject_c* apc_childObject, int8_t dx, int8_t dy, bool b_updateObject, bool b_enableReplaceOfCmd)
{
  MACRO_localVars;
  return genericChangeChildLocation (apc_childObject, dx, dy, b_updateObject, vtObjectAuxiliaryInput_a->numberOfObjectsToFollow, const_cast<IsoAgLib::repeat_iVtObject_x_y_iVtObjectFontAttributes_row_col_s *> (vtObjectAuxiliaryInput_a->objectsToFollow), MACRO_getStructOffset(get_vtObjectAuxiliaryInput_a(), objectsToFollow), sizeof(iVtObjectAuxiliaryInput_s), b_enableReplaceOfCmd);
}

bool
vtObjectAuxiliaryInput_c::setChildPosition(IsoAgLib::iVtObject_c* apc_childObject, int16_t x, int16_t y, bool b_updateObject, bool b_enableReplaceOfCmd)
{
  MACRO_localVars;
  return genericChangeChildPosition (apc_childObject, x, y, b_updateObject, vtObjectAuxiliaryInput_a->numberOfObjectsToFollow, const_cast<IsoAgLib::repeat_iVtObject_x_y_iVtObjectFontAttributes_row_col_s *> (vtObjectAuxiliaryInput_a->objectsToFollow), MACRO_getStructOffset(get_vtObjectAuxiliaryInput_a(), objectsToFollow), sizeof(iVtObjectAuxiliaryInput_s), b_enableReplaceOfCmd);
}

// Operation : setOriginSKM
//! @param b_SKM:
void
vtObjectAuxiliaryInput_c::setOriginSKM(bool /* b_SKM */)
{
  MACRO_localVars;
  for (int i=0; i<vtObjectAuxiliaryInput_a->numberOfObjectsToFollow; i++) {
    vtObjectAuxiliaryInput_a->objectsToFollow[i].vtObject->setOriginSKM (true);
  }
}

} // __IsoAgLib
#endif
