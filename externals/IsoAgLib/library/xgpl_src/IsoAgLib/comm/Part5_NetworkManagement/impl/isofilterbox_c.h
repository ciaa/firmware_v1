/*
  isofilterbox_c.h

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/
#ifndef ISO_FILTER_BOX_C_H
#define ISO_FILTER_BOX_C_H

#include <IsoAgLib/isoaglib_config.h>
#include <IsoAgLib/util/impl/singleton.h>
#include <IsoAgLib/driver/can/imaskfilter_c.h>
#include <IsoAgLib/driver/can/impl/ident_c.h>
#include <IsoAgLib/comm/Part5_NetworkManagement/impl/isoname_c.h>


namespace __IsoAgLib {

class FilterBox_c;
class CanCustomer_c;


struct IsoFilter_s
{
  // dlcForce == -1: don't check dlc. value of 0..8: force to be exactly this dlc!
  IsoFilter_s (CanCustomer_c& arc_canCustomer, const IsoAgLib::iMaskFilter_c& arc_maskFilter, const IsoName_c* apc_isoNameDa = NULL, const IsoName_c* apc_isoNameSa = NULL, int8_t ai8_dlcForce=-1 );
  ~IsoFilter_s();

  uint32_t         getMask()      const { return mc_maskFilter.getMask(); }
  uint32_t         getFilter()    const { return mc_maskFilter.getFilter(); }
  const IsoName_c& getIsoNameDa() const { return mc_isoNameDa; }
  const IsoName_c& getIsoNameSa() const { return mc_isoNameSa; }

  /** operator== does INTENTIONALLY NOT compare the dlcForce field as you can't have the same filter for
      the same customer just with another dlcForce! do NEVER do this! */
  bool operator == (const IsoFilter_s& acrc_isoFilter) const
  { return equalMaskAndFilter (acrc_isoFilter) && (mpc_canCustomer == acrc_isoFilter.mpc_canCustomer) /*&& (mi8_dlcForce == acrc_isoFilter.mi8_dlcForce) */;}
  bool operator != (const IsoFilter_s& acrc_isoFilter) const
  { return !operator == (acrc_isoFilter); }

private:
  bool equalMaskAndFilter (const IsoFilter_s& acrc_isoFilter) const;

private:
  IsoAgLib::iMaskFilter_c mc_maskFilter;
  IsoName_c mc_isoNameDa;
  IsoName_c mc_isoNameSa;

  /** Pointer to a CanCustomer_c instance. Assume this like a reference to be always valid! */
  CanCustomer_c* mpc_canCustomer;

  int8_t mi8_dlcForce; // 0..8 to force the DLC, -1 to X (don't care)

  friend class IsoFilterBox_c;
};


class IsoFilterBox_c : public ClientBase
{
public:
  IsoFilterBox_c (const IsoFilter_s& rrefcs_isoFilter MULTITON_INST_PARAMETER_DEF_WITH_COMMA);

  bool hasIsoFilter (const IsoFilter_s& arcs_isoFilter) { return (ms_isoFilter == arcs_isoFilter); }

  void updateOnAdd();
  void updateOnRemove (const IsoName_c* acrc_isoName);

private:
  IsoFilter_s ms_isoFilter;
  FilterBox_c* mpc_filterBox;
  IsoAgLib::iMaskFilter_c mc_adaptedMaskFilter;
};

} // __IsoAgLib

#endif
