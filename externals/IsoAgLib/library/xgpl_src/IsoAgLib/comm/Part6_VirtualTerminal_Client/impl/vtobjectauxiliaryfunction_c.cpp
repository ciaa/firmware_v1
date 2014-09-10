/*
  vtobjectauxiliaryfunction_c.cpp

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/

#include "vtobjectauxiliaryfunction_c.h"

#ifdef USE_VTOBJECT_auxiliaryfunction
#include "../ivtobjectfontattributes_c.h"
#include "../ivtobjectbutton_c.h"
#include "vtclient_c.h"


namespace __IsoAgLib {

int16_t
vtObjectAuxiliaryFunction_c::stream(uint8_t* destMemory, uint16_t maxBytes, objRange_t sourceOffset)
{
#define MACRO_vtObjectTypeA vtObjectAuxiliaryFunction_a
#define MACRO_vtObjectTypeS iVtObjectAuxiliaryFunction_s
  MACRO_streamLocalVars;
  MACRO_scaleLocalVars;
  if (sourceOffset == 0) { // dump out constant sized stuff
    destMemory [0] = vtObject_a->ID & 0xFF;
    destMemory [1] = vtObject_a->ID >> 8;
    destMemory [2] = 29; // Object Type = Auxiliary Function
    destMemory [3] = __IsoAgLib::getVtClientInstance4Comm().getClientByID (s_properties.clientId).getUserClippedColor (vtObjectAuxiliaryFunction_a->backgroundColour, this, IsoAgLib::BackgroundColour);
    destMemory [4] = vtObjectAuxiliaryFunction_a->functionType;
    destMemory [5] = vtObjectAuxiliaryFunction_a->numberOfObjectsToFollow;
    sourceOffset += 6;
    curBytes += 6;
  }

  MACRO_streamObjectXYcenteredInSoftKey(6);

  return curBytes;
}


vtObjectAuxiliaryFunction_c::vtObjectAuxiliaryFunction_c() {}


uint32_t
vtObjectAuxiliaryFunction_c::fitTerminal() const
{
  MACRO_localVars;
  return 6+vtObjectAuxiliaryFunction_a->numberOfObjectsToFollow*6;
}

bool
vtObjectAuxiliaryFunction_c::moveChildLocation(IsoAgLib::iVtObject_c* apc_childObject, int8_t dx, int8_t dy, bool b_updateObject, bool b_enableReplaceOfCmd)
{
  MACRO_localVars;
  return genericChangeChildLocation (apc_childObject, dx, dy, b_updateObject, vtObjectAuxiliaryFunction_a->numberOfObjectsToFollow, const_cast<IsoAgLib::repeat_iVtObject_x_y_iVtObjectFontAttributes_row_col_s *> (vtObjectAuxiliaryFunction_a->objectsToFollow), MACRO_getStructOffset(get_vtObjectAuxiliaryFunction_a(), objectsToFollow), sizeof(iVtObjectAuxiliaryFunction_s), b_enableReplaceOfCmd);
}

bool
vtObjectAuxiliaryFunction_c::setChildPosition(IsoAgLib::iVtObject_c* apc_childObject, int16_t x, int16_t y, bool b_updateObject, bool b_enableReplaceOfCmd)
{
  MACRO_localVars;
  return genericChangeChildPosition (apc_childObject, x, y, b_updateObject, vtObjectAuxiliaryFunction_a->numberOfObjectsToFollow, const_cast<IsoAgLib::repeat_iVtObject_x_y_iVtObjectFontAttributes_row_col_s *> (vtObjectAuxiliaryFunction_a->objectsToFollow), MACRO_getStructOffset(get_vtObjectAuxiliaryFunction_a(), objectsToFollow), sizeof(iVtObjectAuxiliaryFunction_s), b_enableReplaceOfCmd);
}

// Operation : setOriginSKM
//! @param b_SKM:
void
vtObjectAuxiliaryFunction_c::setOriginSKM(bool /* b_SKM */)
{
  MACRO_localVars;
  for (int i=0; i<vtObjectAuxiliaryFunction_a->numberOfObjectsToFollow; i++) {
    vtObjectAuxiliaryFunction_a->objectsToFollow[i].vtObject->setOriginSKM (true);
  }
}

} // __IsoAgLib
#endif
