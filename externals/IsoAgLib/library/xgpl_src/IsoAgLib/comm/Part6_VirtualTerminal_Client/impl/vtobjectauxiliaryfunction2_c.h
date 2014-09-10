/*
  vtobjectauxiliaryfunction2_c.h

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/
#ifndef VTOBJECTAUXILIARYFUNCTION2_C_H
#define VTOBJECTAUXILIARYFUNCTION2_C_H

#include <IsoAgLib/isoaglib_config.h>


#ifdef USE_VTOBJECT_auxiliaryfunction2

#include "vtobject_c.h"
#include "vttypes.h"

namespace __IsoAgLib {

class Aux2Functions_c;

class vtObjectAuxiliaryFunction2_c : public vtObject_c
{
public:
  vtObjectAuxiliaryFunction2_c();

  void init(const iVtObjectAuxiliaryFunction2_s* vtObjectAuxiliaryFunction2SROM MULTITON_INST_PARAMETER_DEF_WITH_COMMA)
  {vtObject_c::init((iVtObject_s *)vtObjectAuxiliaryFunction2SROM MULTITON_INST_PARAMETER_USE_WITH_COMMA);}

  int16_t stream(uint8_t* destMemory, uint16_t maxBytes, objRange_t sourceOffset);

  iVtObjectAuxiliaryFunction2_s* get_vtObjectAuxiliaryFunction2_a() {return (iVtObjectAuxiliaryFunction2_s *)&(get_vtObject_a());}

  uint32_t fitTerminal() const;
  void setOriginSKM(bool b_SKM);
  bool moveChildLocation(IsoAgLib::iVtObject_c* apc_childObject, int8_t dx, int8_t dy, bool b_updateObject=false, bool b_enableReplaceOfCmd=false);
  bool setChildPosition(IsoAgLib::iVtObject_c* apc_childObject, int16_t dx, int16_t dy, bool b_updateObject=false, bool b_enableReplaceOfCmd=false);
  bool isAssigned() const;

#ifdef USE_ISO_TERMINAL_GETATTRIBUTES
  /** that attribute is in parentheses in the spec, so commented out here
  uint8_t updateObjectType() const { return 31; }
  */
  void saveReceivedAttribute (uint8_t /*attrID*/, uint8_t* /*pui8_attributeValue*/) {};
#endif

private:

  // store assigned AUX inputs and preferred assigned AUX inputs in this structure
  struct AssignedInput_s
  {
    AssignedInput_s()
      : mc_inputIsoName(),
        mui16_inputUid(0xFFFF)
    {}

    bool operator==(const AssignedInput_s& arc_ref) const
    {
      if ( (mc_inputIsoName != arc_ref.mc_inputIsoName) ||
           (mui16_inputUid != arc_ref.mui16_inputUid) )
        return false;
      else
        return true;
    }

    IsoName_c mc_inputIsoName;
    uint16_t mui16_inputUid;
  };

  struct PreferredAssignedInput_s
  {
    PreferredAssignedInput_s()
      : mc_inputIsoName(),
        mui16_inputModelIdentificationCode(0xFFFF),
        mui16_inputUid(0xFFFF)
    {}

    bool operator==(const PreferredAssignedInput_s& arc_ref) const
    {
      if ( (mc_inputIsoName != arc_ref.mc_inputIsoName) ||
           (mui16_inputModelIdentificationCode != arc_ref.mui16_inputModelIdentificationCode) ||
           (mui16_inputUid != arc_ref.mui16_inputUid) )
        return false;
      else
        return true;
    }

    IsoName_c mc_inputIsoName;
    uint16_t mui16_inputModelIdentificationCode;
    uint16_t mui16_inputUid;
  };

  void getAssignedInput(IsoName_c& arc_isoName, uint16_t& arui16_inputUid) const;
  void getPreferredAssignedInput(IsoName_c& arc_isoName, uint16_t& arui16_inputModelIdentificationCode, uint16_t& arui16_inputUid) const;
  bool setAssignedInput(const IsoName_c& arc_isoName, uint16_t aui16_inputUid);
  bool unassignInputIfIsoNameMatches(const IsoName_c& arc_isoName);
  bool setPreferredAssignedInput(const IsoName_c& arc_isoName, uint16_t aui16_inputModelIdentificationCode, uint16_t aui16_inputUid);
  bool hasPreferredAssigment() const;
  void setMatchingPreferredAssignedInputReady(bool a_isReady) { m_matchingPreferredAssignedInputReady = a_isReady; }
  bool getMatchingPreferredAssignedInputReady() { return m_matchingPreferredAssignedInputReady; }

  AssignedInput_s ms_assignedInput;
  PreferredAssignedInput_s ms_preferredAssignedInput;

  bool m_matchingPreferredAssignedInputReady;

  friend class Aux2Functions_c;
};

}

#endif

#endif
