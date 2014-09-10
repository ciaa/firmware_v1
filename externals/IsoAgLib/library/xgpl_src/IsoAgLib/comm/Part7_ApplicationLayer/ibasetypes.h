/*
  ibasetypes.h: types for base data handling

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/
#ifndef IBASE_TYPES_H
#define IBASE_TYPES_H
/* *************************************** */
/* ********** include headers ************ */
/* *************************************** */
#include <IsoAgLib/isoaglib_config.h>

namespace IsoAgLib {

/** use an enum for the different states of ISO flags */
enum IsoActiveFlag_t {
  IsoInactive = 0,    ///< Flag is marked explicit as not active
  IsoActive = 1,      ///< corresponding function is active
  IsoError = 2,       ///< correspnding function is in error state, and thus not useable
  IsoNotAvailable = 3 ///< corresponding function is not available/not implemented in service provider
};

/** use an enum for the different states of ISO flags */
enum IsoDataReq_t {
  IsoNoDataRequested = 0, ///< no lighting data is requested from all implements
  IsoDataRequested = 1,   ///< lighting data is requested from all implements
  IsoReservedData = 2,        ///< reserved field
  IsoDontCare = 3         ///< corresponding function is not available/not implemented in service provider
};

/** GPS GNSS Method */
enum IsoGnssMethod_t {
  IsoNoGps           = 0,
  IsoGnssFix         = 1,
  IsoDgnssFix        = 2,
  IsoGnssPrecise     = 3,
  IsoRtkFixedInteger = 4,
  IsoRtkFloat        = 5,
  IsoDrEstimated     = 6,
  IsoGnssManual      = 7,
  IsoGnssSimulated   = 8,
  IsoGnssError       = 14,
  IsoGnssNull        = 15,
  IsoGnssMethodMAX   = 15
};

/** type of GNSS system */
enum IsoGnssType_t {
  IsoGnssGps            = 0,
  IsoGnssGlonass        = 1,
  IsoGnssGpsGlonass     = 2,
  IsoGnssGpsSbas        = 3,
  IsoGnssGpsSbasGlonass = 4,
  IsoGnssTypeMAX        = 4
};

/** different lighting types; implements and tractor*/
enum IsoCommandFlag_t {
  daytimeRunning =        1,
  alternateHead =         2,
  lowBeamHead =           3,
  highBeamHead =          4,
  frontFog =              5,
  beacon =                6,
  rightTurn =             7,
  leftTurn =              8,
  backUpLightAlarmHorn =  9,
  centerStop =           10,
  rightStop =            11,
  leftStop =             12,
  implClearance =        13,
  tracClearance =        14,
  implMarker =           15,
  tracMarker =           16,
  rearFog =              17,
  undersideWork =        18,
  rearLowWork =          19,
  rearHighWork =         20,
  sideLowWork =          21,
  sideHighWork =         22,
  frontLowWork =         23,
  frontHighWork =        24,
  implOEMOpt2 =          25,
  implOEMOpt1 =          26,
  implRightForwardWork = 27,
  implLeftForwardWork =  28,
  dataMsgReq =           29,    //reserved in Lighting data
  implRightFacingWork =  30,
  implLeftFacingWork =   31,
  implRearWork =         32,
};

/** different lighting types; implements and tractor*/
enum IsoFacility_t {
// Byte 1
  IsoFacilityPowerManagementKeySwitch =                       2,  // Byte 1, Bit6
  IsoFacilityPowerManagementMaxTimeTractorPower =             3,  // Byte 1, Bit5
  IsoFacilityPowerManagementMaintainPower =                   4,  // Byte 1, Bit4
  IsoFacilitySpeedInformationWheelBasedSpeed =                5,  // Byte 1, Bit3
  IsoFacilitySpeedInformationGroundBasedSpeed =               6,  // Byte 1, Bit2
  IsoFacilitySpeedInformationEngineSpeed =                    7,  // Byte 1, Bit1
// Byte 2
  IsoFacilityHitchInformationRearPosition =                   8,  // Byte 2, Bit8
  IsoFacilityHitchInformationRearInWork =                     9,  // Byte 2, Bit7
  IsoFacilityPTOInformationRearShaftSpeed =                   10, // Byte 2, Bit6
  IsoFacilityPTOInformationRearShaftEngagement =              11, // Byte 2, Bit5
  IsoFacilityLightingMinimalSetAsExistingTrailerConnector =   12, // Byte 2, Bit4
  IsoFacilityLanguageCommandStorageInTractorECU =             13, // Byte 2, Bit3
// Byte 3
  IsoFacilityTimeDate =                                       16, // Byte 3, Bit8
  IsoFacilitySpeedAndDistanceGroundBasedDistance =            17, // Byte 3, Bit7
  IsoFacilitySpeedAndDistanceGroundBasedDirection =           18, // Byte 3, Bit6
  IsoFacilitySpeedAndDistanceWheelBasedDistance =             19, // Byte 3, Bit5
  IsoFacilitySpeedAndDistanceWheelBasedDirection =            20, // Byte 3, Bit4
  IsoFacilityAdditionalHitchParametersRearDraft =             21, // Byte 3, Bit3
  IsoFacilityLightingFullImplementLightingMessageSet =        22, // Byte 3, Bit2
  IsoFacilityEstimatedOrMeasuredAuxiliaryValveStatus =        23, // Byte 3, Bit1
// Byte 4
  IsoFacilityHitchCommandsRearHitchPosition =                 24, // Byte 4, Bit8
  IsoFacilityPTOCommandsRearPTOSpeedCommand =                 25, // Byte 4, Bit7
  IsoFacilityPTOCommandsRearPTOEngagementCommand =            26, // Byte 4, Bit6
  IsoFacilityAuxiliaryValveCommands =                         27, // Byte 4, Bit5
  IsoFacilityLimitRequestStatusReporting =                    28, // Byte 4, Bit4
// Byte 5
  IsoFacilityNavigationalSystemHighOutputPosition =           32, // Byte 5, Bit8
  IsoFacilityNavigationalSystemPositionData =                 33, // Byte 5, Bit7
  IsoFacilityNavigationalPseudoRangeNoiseStatistics =         34, // Byte 5, Bit6
  IsoFacilityOperatorExternalLightControls =                  36, // Byte 5, Bit4
  IsoFacilitySelectedMachineSpeed =                           37, // Byte 5, Bit3
  IsoFacilitySelectedMachineSpeedControl =                    38, // Byte 5, Bit2
  IsoFacilityDirectionControl =                               39, // Byte 5, Bit1
// Byte 6
  IsoFacilityHitchInformationFrontPosition =                  40, // Byte 6, Bit8
  IsoFacilityHitchInformationFrontInWork =                    41, // Byte 6, Bit7
  IsoFacilityPTOInformationFrontShaftSpeed =                  42, // Byte 6, Bit6
  IsoFacilityPTOInformationFrontShaftEngagement =             43, // Byte 6, Bit5
  IsoFacilityAdditionalHitchParametersFrontDraft =            44, // Byte 6, Bit4
  IsoFacilityHitchCommandsFrontHitchPosition =                45, // Byte 6, Bit3
  IsoFacilityPTOCommandsFrontPTOSpeedCommand =                46, // Byte 6, Bit2
  IsoFacilityPTOCommandsFrontPTOEngagementCommand =           47, // Byte 6, Bit1
// Byte 8
  IsoFacilityReservedBitssetTo0 =                             63  // Byte 8, Bit1
};

/** use an extended enum for the different states of ISO flags for auxiliary valve */
enum IsoAuxFlagExtended_t {
  IsoBlocked = 0,            ///< corresponding valve is closed
  IsoExtend = 1,             ///< corresponding valve is controlled from valves extended port
  IsoRetract = 2,            ///< corresponding valve is controlled from valves retract port
  IsoFloating = 3,           ///< corresponding valve has no control from or to the valve
  IsoErrorAuxExt = 14,       ///< corresponding function is in error state, and thus not useable
  IsoNotAvailableAuxExt = 15 ///< corresponding function is not available/not implemented in service provider
};

/** use an enum for the different states of ISO flags for auxiliary valve */
enum IsoAuxFlag_t {
  IsoBlock = 0,          ///< corresponding valve is closed
  IsoFloat = 1,          ///< corresponding valve has no control from or to the valve
  IsoErrorAux = 2,       ///< corresponding function is in error state, and thus not useable
  IsoNotAvailableAux = 3 ///< corresponding function is not available/not implemented in service provider
};

/** use an enum for the different states of ISO flags for auxiliary valve limit status*/
enum IsoLimitFlag_t {
  IsoNotLimited = 0,          ///< corresponding limit status is not active
  IsoOperatorLimited = 1,     ///< request cannot be implemented
  IsoLimitedHigh = 2,         ///< corresponding function is in error state, and thus not useable
  IsoLimitedLow = 3,           ///< corresponding function is not available/not implemented in service provider
  IsoNonRecoverabelFault = 6, ///not recoverable from the viewpoint of the implemented
  IsoNotAvailableLimit = 7    ///paramter not supported
};

/** use an enum typedef for the different states of direction */
enum IsoOperatorDirectionFlag_t {
  IsoNotReversed = 0,         ///< reported direction is not reversed from the perspective of the operator
  IsoReversed = 1,            ///< reported direction is reversed from the perspective of the operator
  IsoErrorReversed = 2,       ///< correspnding function is in error state, and thus not useable
  IsoNotAvailableReversed = 3 ///< corresponding function is not available in service provider
};

/** use an enum typedef for the different states of a parameter whose requests are transient/temporary/one-shot */
enum IsoReqFlag_t {
  IsoExternalReqAccepted = 0, ///< External Request accepted. No subsequent operator intervention.
  IsoControlOverride = 1,     ///< most recent external request ignored because of an operator or tractor system override
  IsoErrorReq = 2,            ///< correspnding function is in error state, and thus not useable
  IsoNotAvailableReq = 3      ///< corresponding function is not available in service provider
};

/** use an enum typedef for the different states of the guidance system */
enum IsoSteerFlag_t {
  IsoNotIntendedToSteer = 0, ///< guidance system is not intending to control steering
  IsoIntendedToSteer = 1,    ///< guidance system is intending to control steering
  IsoReserved = 2,           ///< reserved field
  IsoNotAvailableSteer = 3   ///< corresponding function is not available in service provider
};

/** use an enum typedef for the different states of the request reset command status */
enum IsoResetFlag_t {
  IsoResetNotRequired = 0, ///< reset of command status is not required
  IsoResetRequired = 1,    ///< reset of command status is required
  IsoErrorReset = 2,       ///< correspnding function is in error state, and thus not useable
  IsoNotAvailableReset = 3 ///< corresponding function is not available in service provider
};

/** use an enum typedef for the steering input position status */
enum IsoSteerPosFlag_t {
  IsoNotCorrectPos = 0,       ///< manual steering is not in correct position for guidance to function
  IsoCorrectPos = 1,          ///< manual steering is in correct position for guidance to function
  IsoErrorSteerPos = 2,       ///< correspnding function is in error state, and thus not useable
  IsoNotAvailableSteerPos = 3 ///< corresponding function is not available in service provider
};

/** use an enum typedef for the steering system readiness */
enum IsoSteerReadinessFlag_t {
  IsoSystemNotReady = 0,            ///< machine steering system indicates that it is not free from faults that would preclude guidance sytstem operation
  IsoSystemReady = 1,               ///< machine steering system indicates that it is not free from faults that would preclude guidance sytstem operation
  IsoErrorSteerReadiness = 2,       ///< correspnding function is in error state, and thus not useable
  IsoNotAvailableSteerReadiness = 3 ///< corresponding function is not available in service provider
};

/** use an enum typedef for facilities type */
enum IsoFacilityFlag_t {
  IsoFacilityNotAvailable = 0,  ///< facility is not available
  IsoFacilityAvailable = 1      ///< facility is available
};

/** use an enum typedef for TECU type */
enum IsoTecuClassFlag_t {
  IsoTecuClass1 = 0,              ///< TECU class 1
  IsoTecuClass2 = 1,              ///< TECU class 2
  IsoTecuClass3 = 2,              ///< TECU class 3
  IsoTecuClassNotAvailable = 3,   ///< TECU class is not available
};

/** use an enum for the different states of ISO flags for the direction of travel of the chassis */
enum IsoDirectionFlag_t {
  IsoReverse = 0,              ///< direction of travel of the machine is reversed
  IsoForward = 1,              ///< direction of travel of the machine is forward
  IsoErrorDirection = 2,       ///< corresponding function is in error state, and thus not useable
  IsoNotAvailableDirection = 3 ///< corresponding function is not available/not implemented in service provider
};

/** use an enum for the different sources of speed*/
enum IsoSpeedSourceFlag_t {
  IsoWheelBasedSpeed = 0,      ///< speed source is wheel-based
  IsoGroundBasedSpeed = 1,     ///< speed source is ground-based
  IsoNavigationBasedSpeed = 2, ///< speed source is navigation-based
  IsoBlendedSpeed = 3,         ///< speed source is "blended"
  IsoSimulatedSpeed = 4,       ///< speed source is simulated
  IsoNotAvailableSpeed = 7     ///<corresponding function is not available in service provider
};

/** use an enum for the different states of ISO flags for the implement work state */
enum IsoImplWorkFlag_t {
  IsoNotReadyForFieldWork = 0, ///< Implement is not ready for field work
  IsoReadyForFieldWork = 1,    ///< Implement is ready for field work
  IsoErrorWork = 2,            ///< corresponding function is in error state, and thus not useable
  IsoNotAvailableWork = 3      ///< corresponding function is not available/not implemented in service provider
};

/** use an enum for the different states of ISO flags for the implement transport state */
enum IsoImplTransportFlag_t {
  IsoNotTransported = 0,       ///< Implement may not be transported
  IsoTransported = 1,          ///< Implement may be transported
  IsoErrorTransport = 2,       ///< corresponding function is in error state, and thus not useable
  IsoNotAvailableTransport = 3 ///< corresponding function is not available/not implemented in service provider
};

/** use an enum for the different states of ISO flags for the implement park state */
enum IsoImplParkFlag_t {
  IsoNotDisconnected = 0, ///< Implement may not be disconnected
  IsoDisconnect = 1,      ///< Implement may be disconnected
  IsoErrorPark = 2,       ///< corresponding function is in error state, and thus not useable
  IsoNotAvailablePark = 3 ///< corresponding function is not available/not implemented in service provider
};

/** use an enum for the different sources of the selected speed */
enum SpeedSource_t {
  NoSpeed = 0,
  WheelBasedSpeed = 1,
  GroundBasedSpeed = 2,
  GpsBasedSpeed = 3,
  SelectedSpeed = 4
};

/** use an enum for the different sources of distance and direction */
enum DistanceDirectionSource_t {
  NoDistDirec = 0,
  WheelBasedDistDirec = 1,
  GroundBasedDistDirec = 2,
  SelectedDistDirec = 3
};

/** define mode */
enum IdentMode_t {
  IdentModeTractor = 0,
  IdentModeImplement = 1
};

} // end namespace IsoAgLib


// formerly defined in isoaglib_config.h - but only used in Part 7
// subject to be changed/revised later on. please don't use!
#define NO_VAL_32S ((int32_t)0x80000000l)
#define NO_VAL_16S ((int16_t)0x8000)
#define NO_VAL_8S ((int8_t)0x80)
#define NO_VAL_32 ((uint32_t)0xFFFFFFFFUL)
#define NO_VAL_16 ((uint16_t)0xFFFFu)
#define NO_VAL_8 ((uint8_t)0xFFu)

#define ERROR_VAL_32S ((int32_t)0x80000001l)
#define ERROR_VAL_16S ((int16_t)0x8001)
#define ERROR_VAL_8S ((int8_t)0x81)
#define ERROR_VAL_32 0xFFFFFFFEUL
#define ERROR_VAL_16 0xFFFEu
#define ERROR_VAL_8 0xFEu


#endif

