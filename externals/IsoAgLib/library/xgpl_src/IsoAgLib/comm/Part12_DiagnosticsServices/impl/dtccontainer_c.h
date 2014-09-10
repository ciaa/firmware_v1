/*
  dtccontainer.h - Stores and manages array of DTC elements

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/
#ifndef DTC_CONTAINER_H
#define DTC_CONTAINER_H

#include <IsoAgLib/isoaglib_config.h>
#include <IsoAgLib/util/iliberr_c.h>
#include <IsoAgLib/driver/system/impl/system_c.h>

#include "../idiagnosticstypes.h"

// Begin Namespace __IsoAgLib
namespace __IsoAgLib {

/**
  This class stores and manages array of DTC elements
  @author Antoine Kandera, reviewed by Martin Wodok
*/
class DtcContainer_c
{
public:
  static const uint16_t scui16_sizeDTCList = (CONFIG_MAX_ACTIVE_DTCS) + (CONFIG_MAX_PREVIOUSLY_ACTIVE_DTCS);
public:
  struct Dtc_s
  {
    Dtc_s() :
      ui32_spn(spiNone),
      en_fmi(IsoAgLib::FMIConditionExists), // no applicable FMI exists, not relevant
      i32_timeLastStateChange(-1),
      i32_timeLastStateChangeSent(-1), 
      b_active(false),
      ui16_occurrenceCount(0)
    {}
    Dtc_s(uint32_t SPN, IsoAgLib::FailureModeIndicator_en FMI) :
      ui32_spn(SPN),
      en_fmi(FMI),
      i32_timeLastStateChange(System_c::getTime()),
      i32_timeLastStateChangeSent(System_c::getTime()), 
      b_active(true),
      ui16_occurrenceCount(1)
    {}
    Dtc_s& operator=(const Dtc_s& acrc_src)
    {
      if ( this != &acrc_src)
      {   
        ui32_spn = acrc_src.ui32_spn;
        en_fmi = acrc_src.en_fmi;

        i32_timeLastStateChange = acrc_src.i32_timeLastStateChange;
        i32_timeLastStateChangeSent = acrc_src.i32_timeLastStateChangeSent;

        b_active = acrc_src.b_active;
        ui16_occurrenceCount = acrc_src.ui16_occurrenceCount;
      }
      return *this;
    }

    // Special SPN scui32_spiNone indicates empty-placeholder
    static const uint32_t spiNone = 0xFFFFFFFF;

    uint32_t ui32_spn;
    IsoAgLib::FailureModeIndicator_en en_fmi;

    int32_t i32_timeLastStateChange;
    int32_t i32_timeLastStateChangeSent;

    bool b_active;

    uint16_t ui16_occurrenceCount;

  private:
    Dtc_s(const Dtc_s& acrc_src); // intentionally undefined
  }; // struct Dtc_s

public:

  DtcContainer_c() {}
  ~DtcContainer_c() {}

  /**
    Get index of the DTC with key (SPN,FMI)
    @return index in [0..scui16_sizeDTCList-1] if found
            scui16_sizeDTCList if not found
  */
  uint16_t getDTCIndex(uint32_t SPN, IsoAgLib::FailureModeIndicator_en FMI) const;

  /**
    Get index of first not defined DTC
    @return index in [0..scui16_sizeDTCList-1] if found
            scui16_sizeDTCList if not found
  */
  uint16_t getFreeDTCIndex() const;

  /**
    @param ab_searchForActiveDtc : true for number of active DTC, false for number of previously active DTC
    @return number of active / previously active DTC
  */
  uint16_t getNumberOfDtc(bool ab_searchForActiveDtc) const;

  /** Clear the Previous Active Trouble Codes */
  void dtcClearPrevious();

  /**
    deliver a Dtc_s reference from a specific index with operator[]
    @param aui16_index index of delivered DTC [0..scui16_sizeDTCList-1]
    NOTE: no test if the index is valid !
  */
  Dtc_s& operator[](uint16_t aui16_index) { isoaglib_header_assert(aui16_index < scui16_sizeDTCList); return marr_dtc[aui16_index];}

  /**
    deliver a Dtc_s const reference from a specific index with operator[]
    @param aui16_index index of delivered DTC [0..scui16_sizeDTCList-1]
    NOTE: no test if the index is valid !
  */
  const Dtc_s& operator[](uint16_t aui16_index) const { isoaglib_header_assert(aui16_index < scui16_sizeDTCList); return marr_dtc[aui16_index];}

private:
  /// "List" of all DTCs
  /// For the real amount of the active / prev. active DTCs,
  /// iterate through "marr_dtc" and count all entries with "ui32_spn != spiNone"
  Dtc_s marr_dtc [scui16_sizeDTCList];
};

}  // __IsoAgLib
#endif
