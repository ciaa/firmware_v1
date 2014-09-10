/*
  idiagnosticstypes.h: type for Failure Mode Indicators

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/
#ifndef IDIAGNOSTICS_TYPES_H
#define IDIAGNOSTICS_TYPES_H

#include <IsoAgLib/isoaglib_config.h>
#include <IsoAgLib/util/impl/bitfieldwrapper_c.h>

namespace IsoAgLib {

/** use an enum for the different Failure Mode Indicators */
enum FailureModeIndicator_en {

  FMIDataValidButAboveNormalOperationRangeMostSevereLevel = 0,
  FMIDataValidButBelowNormalOperationalRangeMostSevereLevel,
  FMIDataErraticIntermittentOrIncorrect,
  FMIVoltageAboveNormalOrShortedToHighSource,
  FMIVoltageBelowNormalOrShortedToLowSource,
  FMICurrentBelowNormalOrOpenCircuit,
  FMICurrentAboveNormalOrGroundedCircuit,
  FMIMechanicalSystemNotRespondingOrImproperlyAdjusted,
  FMIAbnormalFrequencyOrPulseWidthOrPeriod,
  FMIAbnormalUpdateRate,
  FMIAbnormalRateOfChange,
  FMIRootCauseNotKnown,
  FMIBadIntelligentUnitOrComponent,
  FMIOutOfCalibration,
  FMISpecialInstructions,
  FMIDataValidButAboveNormalOperatingRangeLeastSevereLevel,
  FMIDataValidButAboveNormalOperatingRangeModeratelySevereLevel,
  FMIDataValidButBelowNormalOperatingRangeLeastSevereLevel,
  FMIDataValidButBelowNormalOperatingRangeModeratelySevereLevel,
  FMIReceivedNetworkDataInError,
  FMIConditionExists = 31

};

enum CertificationRevision_t
{
  // for backward-compatibilty only (until removed after some time!)
  CertificationRevisionFirstRevision = 0,  // deprecated enum, use CertificationRevision1stRevision
  CertificationRevisionSecondRevision = 1, // deprecated enum, use CertificationRevision2ndRevision
  CertificationRevisionThirdRevision = 2,  // deprecated enum, use CertificationRevision3rdRevision
  CertificationRevisionFourthRevision = 3, // deprecated enum, use CertificationRevision4thRevision

  CertificationRevision1stRevision = 0,
  CertificationRevision2ndRevision = 1,
  CertificationRevision3rdRevision = 2,
  CertificationRevision4thRevision = 3,
  CertificationRevision5thRevision = 4,
  CertificationRevision6thRevision = 5,
  CertificationRevision7thRevision = 6,
  CertificationRevision8thRevision = 7,
  CertificationRevision9thRevision = 8,
  CertificationRevision10thRevision = 9,
  CertificationRevision11thRevision = 10,
  CertificationRevision12thRevision = 11,
  CertificationRevision13thRevision = 12,
  CertificationRevision14thRevision = 13,
  CertificationRevision15thRevision = 14,
  CertificationRevision16thRevision = 15,
  CertificationRevision17thRevision = 16,
  CertificationRevision18thRevision = 17,
  CertificationRevision19thRevision = 18,
  CertificationRevision20thRevision = 19,
  CertificationRevision21stRevision = 20,
  CertificationRevision22ndRevision = 21,
  CertificationRevision23rdRevision = 22,
  CertificationRevision24thRevision = 23,
  CertificationRevision25thRevision = 24,
  CertificationRevision26thRevision = 25,
  CertificationRevision27thRevision = 26,
  CertificationRevision28thRevision = 27,
  CertificationRevision29thRevision = 28,
  CertificationRevision30thRevision = 29,
  CertificationRevision31stRevision = 30,
  CertificationRevisionNotAvailable = 31
};


enum CertificationLabType_t
{
  CertificationLabTypeNonCertifiedLab = 0,
  CertificationLabTypeAEFCertifiedLab = 1,
  CertificationLabTypeNotAvailable = 3
};

} // IsoAgLib


#endif

