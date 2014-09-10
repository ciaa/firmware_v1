/*
  diagnosticfunctionalitiestypes.h: types for CF Functionalities

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/
#ifndef DIAGNOSTIC_FUNCTIONALITIES_TYPES_H
#define DIAGNOSTIC_FUNCTIONALITIES_TYPES_H

#include <IsoAgLib/isoaglib_config.h>

namespace __IsoAgLib {

enum EcuDiagnosticProtocolIdentification_t
{
  ProtocolId_J1939_73 = 0,
  ProtocolId_ISO14230,
  ProtocolId_ISO15765_3,
  ProtocolId_BITSIZE = 8
};

struct EcuDiagnosticProtocolIdentification_s
{
  typedef EcuDiagnosticProtocolIdentification_t enum_type;
  enum { number_of_bits = ProtocolId_BITSIZE };
};
typedef BitFieldWrapperRight_c<EcuDiagnosticProtocolIdentification_s> EcuDiagnosticProtocolIdentificationBitMask_t;


enum FunctionalitiesCharacteristics_t
{
  MinimumControlFunction = 0,
  UniversalTerminal,
  UniversalTerminalWorkingSet,
  AuxOInputs,
  AuxOFunctions,
  AuxNInputs,
  AuxNFunctions,
  TaskControllerBasic,
  TaskControllerBasicWorkingSet,
  TaskControllerGeo,
  TaskControllerGeoWorkingSet,
  TaskControllerSectionControl,
  TaskControllerSectionControlWorkingSet,
  BasicTractorECU,
  BasicTractorECUImplement,
  StopAllImplementOperations = 21,
  StopAllImplementOperationsImplement,
  Functionality_ReservedForISO = 0xFF
};

// Min CF
enum MinimumControlFunctionOptions_t
{
  MinimumControlFunction_BITSIZE = 0
};

struct MinimumControlFunctionOptions_s
{
  typedef MinimumControlFunctionOptions_t enum_type;
  enum { number_of_bits = MinimumControlFunction_BITSIZE };
};
typedef BitFieldWrapperRight_c<MinimumControlFunctionOptions_s> MinimumControlFunctionOptionsBitMask_t;

// UT
enum UniversalTerminalOptions_t
{
#if 0
  // not yet defined, but sure to come...
  UniversalTerminal_UserLayoutScreens,
  UniversalTerminal_ColourMap,
  UniversalTerminal_GraphicsContext,
  UniversalTerminal_BITSIZE = 8
#endif
  UniversalTerminal_BITSIZE = 0
};

struct UniversalTerminalOptions_s
{
  typedef UniversalTerminalOptions_t enum_type;
  enum { number_of_bits = UniversalTerminal_BITSIZE };
};
typedef BitFieldWrapperRight_c<UniversalTerminalOptions_s> UniversalTerminalOptionsBitMask_t;

// Aux O
enum AuxOOptions_t
{
  AuxO_SupportsType0Function = 0,
  AuxO_SupportsType1Function,
  AuxO_SupportsType2Function,
  AuxO_BITSIZE = 8
};

struct AuxOOptions_s
{
  typedef AuxOOptions_t enum_type;
  enum { number_of_bits = AuxO_BITSIZE };
};
typedef BitFieldWrapperRight_c<AuxOOptions_s> AuxOOptionsBitMask_t;

// Aux N
enum AuxNOptions_t
{
  AuxN_SupportsType0Function = 0,
  AuxN_SupportsType1Function,
  AuxN_SupportsType2Function,
  AuxN_SupportsType3Function,
  AuxN_SupportsType4Function,
  AuxN_SupportsType5Function,
  AuxN_SupportsType6Function,
  AuxN_SupportsType7Function,
  AuxN_SupportsType8Function,
  AuxN_SupportsType9Function,
  AuxN_SupportsType10Function,
  AuxN_SupportsType11Function,
  AuxN_SupportsType12Function,
  AuxN_SupportsType13Function,
  AuxN_SupportsType14Function,
  AuxN_BITSIZE = 16
};

struct AuxNOptions_s
{
  typedef AuxNOptions_t enum_type;
  enum { number_of_bits = AuxN_BITSIZE };
};
typedef BitFieldWrapperRight_c<AuxNOptions_s> AuxNOptionsBitMask_t;

// TC Basic
enum TaskControllerBasicOptions_t
{
  TaskControllerBasic_BITSIZE = 0
};

struct TaskControllerBasicOptions_s
{
  typedef TaskControllerBasicOptions_t enum_type;
  enum { number_of_bits = TaskControllerBasic_BITSIZE };
};
typedef BitFieldWrapperRight_c<TaskControllerBasicOptions_s> TaskControllerBasicOptionsBitMask_t;

// TC Geo
enum TaskControllerGeoOptions_t
{
  TaskControllerGeo_PolygonPrescriptionMaps = 0,
  TaskControllerGeo_BITSIZE = 8
};

struct TaskControllerGeoOptions_s
{
  typedef TaskControllerGeoOptions_t enum_type;
  enum { number_of_bits = TaskControllerGeo_BITSIZE };
};
typedef BitFieldWrapperRight_c<TaskControllerGeoOptions_s> TaskControllerGeoOptionsBitMask_t;

// Basic TECU
enum BasicTractorECUOptions_t
{
  BasicTractorECU_Class1NoOptions = 0,
  BasicTractorECU_Class2NoOptions,
  BasicTractorECU_ClassRequiredLighting,
  BasicTractorECU_NavigationOption,
  BasicTractorECU_FrontHitchOption,
  BasicTractorECU_GuidanceOption,
  BasicTractorECU_BITSIZE = 8
};

struct BasicTractorECUOptions_s
{
  typedef BasicTractorECUOptions_t enum_type;
  enum { number_of_bits = BasicTractorECU_BITSIZE };
};
typedef BitFieldWrapperRight_c<BasicTractorECUOptions_s> BasicTractorECUOptionsBitMask_t;


// Stop all implement operations
enum StopAllImplementOperationsOptions_t
{
  StopAllImplementOperations_BITSIZE = 0
};

struct StopAllImplementOperationsOptions_s
{
  typedef StopAllImplementOperationsOptions_t enum_type;
  enum { number_of_bits = StopAllImplementOperations_BITSIZE };
};
typedef BitFieldWrapperRight_c<StopAllImplementOperationsOptions_s> StopAllImplementOperationsOptionsBitMask_t;


#if 0
// not specified in ISO yet...
// Advance TECU
enum AdvanceTractorECUOptions_t
{
  AdvanceTractorECU_Class3NoOptions = 0,
  AdvanceTractorECU_FullLightingMessageSet,
  AdvanceTractorECU_NavigationOption,
  AdvanceTractorECU_FrontHitchOption,
  AdvanceTractorECU_GuidanceOption,
  AdvanceTractorECU_PowertrainOption,
  AdvanceTractorECU_MotionInitiationOption,
  AdvanceTractorECU_BITSIZE = 8
};

struct AdvanceTractorECUOptions_s
{
  typedef AdvanceTractorECUOptions_t enum_type;
  enum { number_of_bits = AdvanceTractorECU_BITSIZE };
};
typedef BitFieldWrapperRight_c<AdvanceTractorECUOptions_s> AdvanceTractorECUOptionsBitMask_t;

// SC
enum SequenceControlOptions_t
{
  SequenceControl_BITSIZE = 0
};

struct SequenceControlOptions_s
{
  typedef SequenceControlOptions_t enum_type;
  enum { number_of_bits = SequenceControl_BITSIZE };
};
typedef BitFieldWrapperRight_c<SequenceControlOptions_s> SequenceControlOptionsBitMask_t;

// FS
enum FileServerOptions_t
{
  FileServer_BITSIZE = 0
};

struct FileServerOptions_s
{
  typedef FileServerOptions_t enum_type;
  enum { number_of_bits = FileServer_BITSIZE };
};
typedef BitFieldWrapperRight_c<FileServerOptions_s> FileServerOptionsBitMask_t;
#endif

} // end namespace IsoAgLib


#endif

