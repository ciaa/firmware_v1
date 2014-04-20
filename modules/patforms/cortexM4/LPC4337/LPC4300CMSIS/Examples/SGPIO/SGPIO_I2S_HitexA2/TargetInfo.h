/**
*	@addtogroup Framework
*	@{
*/

/****************************************************************************
*
*		Audio Framework
*		---------------
*
****************************************************************************
*	TargetInfo.h
****************************************************************************
*
*	Description:	All target specific information
*
*	Copyright:
*	Owner:
*
****************************************************************************
*
*	$Revision: 1.10 $
*
*	$Log: TargetInfo.h,v $
*	Revision 1.10  2008/02/29 01:36:25  pbeckmann
*	Removed wrap_module and rebuilt.
*	
*	Revision 1.9  2008/02/27 05:49:38  pbeckmann
*	Added SecondOrderFilterSmoothed
*	
*	Revision 1.8  2008/02/22 19:16:19  pbeckmann
*	Added MathFloat32 library
*	
*	Revision 1.7  2008/02/20 01:12:20  pbeckmann
*	Fixed build on the 21369 EZ-KIT
*	
*	Revision 1.6  2007/12/17 19:41:15  pbeckmann
*	Large number of modules added to executable.
*	
*	Revision 1.5  2007/12/01 01:24:57  pbeckmann
*	These files inadvertently were erased.
*	
*	Revision 1.4  2007/11/24 19:11:48  CHPerry
*	Added definition for packet buffer size
*	
*	Revision 1.3  2007/11/06 19:53:21  pbeckmann
*	Fixed build.
*	
*	Revision 1.2  2007/10/30 01:07:27  pbeckmann
*	Updated target to use target lirbaries built locally by the library.
*	
*	Revision 1.1  2007/09/07 20:24:12  pbeckmann
*	Reorganized target specific information.
*	
*
***************************************************************************/

/*
* @file TargetInfo.h
*/


#ifndef _TARGETINFO_H
#define _TARGETINFO_H

/* ----------------------------------------------------------------------
** Default audio I/O mode
** ------------------------------------------------------------------- */

#define DEFAULT_AUDIO_IO_MODE AudioIOMode_ADC


  /* ----------------------------------------------------------------------
  ** Specifies the sizes of each of the heaps on the target
  ** ------------------------------------------------------------------- */

#define MASTER_HEAP_SIZE		(1024*5)
#define SLOW_HEAP_SIZE			(1024*8)
#define FASTB_HEAP_SIZE			(1024*2)
#define MAX_COMMAND_BUFFER_LEN  128

  /* ----------------------------------------------------------------------
  ** Specifies the audio modules that will be compiled into the target
  ** ------------------------------------------------------------------- */

extern const ModClassModule awe_modButterworthFilter40Class;
extern const ModClassModule awe_modSecondOrderFilterSmoothed40Class;
extern const ModClassModule awe_modAllpassDelayClass;
extern const ModClassModule awe_modAllpassDampedDelayOptimizedClass;
extern const ModClassModule awe_modAllpassDampedDelayciOptimizedClass;
extern const ModClassModule awe_modAllpassDelayciClass;
extern const ModClassModule awe_modAllpassDelayiClass;
extern const ModClassModule awe_modDelayciClass;
extern const ModClassModule awe_modDelayiClass;
extern const ModClassModule awe_modLatticeDelayClass;
extern const ModClassModule awe_modLMSClass;
extern const ModClassModule awe_modSecondOrderFilter40SmoothedCascadeClass;
extern const ModClassModule awe_modGraphicEQBandClass;
extern const ModClassModule awe_modAbsClass;
extern const ModClassModule awe_modAdderClass;
extern const ModClassModule awe_modAGCAutoAttackReleaseClass;
extern const ModClassModule awe_modAGCCoreARClass;
extern const ModClassModule awe_modAGCCoreClass;
extern const ModClassModule awe_modAGCGainComputerClass;
extern const ModClassModule awe_modAGCLimiterCoreClass;
extern const ModClassModule awe_modAGCMultiplierClass;
extern const ModClassModule awe_modAGCNoiseGateCoreClass;
extern const ModClassModule awe_modAGCReleaseClass;
extern const ModClassModule awe_modBalanceClass;
extern const ModClassModule awe_modBiquadCascadeClass;
extern const ModClassModule awe_modBiquadClass;
extern const ModClassModule awe_modBiquadSmoothedClass;
extern const ModClassModule awe_modButterworthFilterClass;
extern const ModClassModule awe_modClipIndicatorClass;
extern const ModClassModule awe_modClipAsymClass;
extern const ModClassModule awe_modDCSourceClass;
extern const ModClassModule awe_modFIRDecimatorClass;
extern const ModClassModule awe_modFIRInterpolatorClass;
extern const ModClassModule awe_modFIRClass;
extern const ModClassModule awe_modLogicCompareClass;
extern const ModClassModule awe_modMaxAbsClass;
extern const ModClassModule awe_modMeterClass;
extern const ModClassModule awe_modMixerClass;
extern const ModClassModule awe_modMixerSmoothedClass;
extern const ModClassModule awe_modMultiplexorSmoothedClass;
extern const ModClassModule awe_modMultiplierClass;
extern const ModClassModule awe_modMuteSmoothedClass;
extern const ModClassModule awe_modPeriodicSourceClass;
extern const ModClassModule awe_modPolynomialClass;
extern const ModClassModule awe_modProbeClass;
extern const ModClassModule awe_modRandiClass;
extern const ModClassModule awe_modRMSClass;
extern const ModClassModule awe_modRouterSmoothedClass;
extern const ModClassModule awe_modSampleAndHoldClass;
extern const ModClassModule awe_modSawtoothClass;
extern const ModClassModule awe_modScaleOffsetClass;
extern const ModClassModule awe_modScalerDBClass;
extern const ModClassModule awe_modScalerDBSmoothedClass;
extern const ModClassModule awe_modScalerClass;
extern const ModClassModule awe_modScalerSmoothedClass;
extern const ModClassModule awe_modScalerNDBSmoothedClass;
extern const ModClassModule awe_modScalerNClass;
extern const ModClassModule awe_modScalerNSmoothedClass;
extern const ModClassModule awe_modSecondOrderFilterSmoothedClass;
extern const ModClassModule awe_modSineGenClass;
extern const ModClassModule awe_modSineSmoothedGenClass;
extern const ModClassModule awe_modSinkClass;
extern const ModClassModule awe_modSMixer2x1Class;
extern const ModClassModule awe_modSourceClass;
extern const ModClassModule awe_modSquareAddClass;
extern const ModClassModule awe_modSquareClass;
extern const ModClassModule awe_modSubtractClass;
extern const ModClassModule awe_modSumDiffClass;
extern const ModClassModule awe_modTableInterpClass;
extern const ModClassModule awe_modTableLookupClass;
extern const ModClassModule awe_modVolumeControlClass;
extern const ModClassModule awe_modWhiteNoiseClass;
extern const ModClassModule awe_modZeroCrossingDetectorClass;
extern const ModClassModule awe_modBlockStatisticsClass;
extern const ModClassModule awe_modWithinRangeClass;
extern const ModClassModule awe_modThreeBandToneControlClass;
extern const ModClassModule awe_modSecondOrderFilterSmoothedCascadeClass;
extern const ModClassModule awe_modAdderFract32Class;
extern const ModClassModule awe_modAGCMultiplierFract32Class;
extern const ModClassModule awe_modBiquadCascadeFract32Class;
extern const ModClassModule awe_modBiquadFract32Class;
extern const ModClassModule awe_modBiquadSmoothedFract32Class;
extern const ModClassModule awe_modClipIndicatorFract32Class;
extern const ModClassModule awe_modClipAsymFract32Class;
extern const ModClassModule awe_modFIRDecimatorFract32Class;
extern const ModClassModule awe_modFIRFract32Class;
extern const ModClassModule awe_modFIRInterpolatorFract32Class;
extern const ModClassModule awe_modLogicAndConstFract32Class;
extern const ModClassModule awe_modLogicCompareConstFract32Class;
extern const ModClassModule awe_modLogicCompareFract32Class;
extern const ModClassModule awe_modMaxAbsFract32Class;
extern const ModClassModule awe_modMeterFract32Class;
extern const ModClassModule awe_modMixerFract32Class;
extern const ModClassModule awe_modMixerSmoothedFract32Class;
extern const ModClassModule awe_modMultiplexorSmoothedFract32Class;
extern const ModClassModule awe_modMultiplierFract32Class;
extern const ModClassModule awe_modMuteSmoothedFract32Class;
extern const ModClassModule awe_modPeriodicSourceFract32Class;
extern const ModClassModule awe_modPolynomialFract32Class;
extern const ModClassModule awe_modRMSFract32Class;
extern const ModClassModule awe_modRouterSmoothedFract32Class;
extern const ModClassModule awe_modSawtoothFract32Class;
extern const ModClassModule awe_modScaleOffsetFract32Class;
extern const ModClassModule awe_modScalerDBFract32Class;
extern const ModClassModule awe_modScalerDBSmoothedFract32Class;
extern const ModClassModule awe_modScalerFract32Class;
extern const ModClassModule awe_modScalerSmoothedFract32Class;
extern const ModClassModule awe_modScalerNDBSmoothedFract32Class;
extern const ModClassModule awe_modScalerNFract32Class;
extern const ModClassModule awe_modScalerNSmoothedFract32Class;
extern const ModClassModule awe_modSecondOrderFilterSmoothedFract32Class;
extern const ModClassModule awe_modShiftFract32Class;
extern const ModClassModule awe_modSineGenFract32Class;
extern const ModClassModule awe_modSineSmoothedGenFract32Class;
extern const ModClassModule awe_modSinkFract32Class;
extern const ModClassModule awe_modSMixer2x1Fract32Class;
extern const ModClassModule awe_modSOFSmoothedCascadeFract32Class;
extern const ModClassModule awe_modSourceFract32Class;
extern const ModClassModule awe_modSquareAddFract32Class;
extern const ModClassModule awe_modSquareFract32Class;
extern const ModClassModule awe_modSubtractFract32Class;
extern const ModClassModule awe_modSumDiffFract32Class;
extern const ModClassModule awe_modZeroCrossingDetectorFract32Class;
extern const ModClassModule awe_modBlockStatisticsFract32Class;
extern const ModClassModule awe_modButterworthFilterFract32Class;
extern const ModClassModule awe_modVolumeControlFract32Class;
extern const ModClassModule awe_modBalanceFract32Class;
extern const ModClassModule awe_modWhiteNoiseFract32Class;
extern const ModClassModule awe_modWithinRangeFract32Class;
extern const ModClassModule awe_modGraphicEQBandFract32Class;
extern const ModClassModule awe_modAbsFract32Class;
extern const ModClassModule awe_modSqrtFract32Class;
extern const ModClassModule awe_modThreeBandToneControlFract32Class;
extern const ModClassModule awe_modAGCLimiterCoreFract32Class;
extern const ModClassModule awe_modDCSourceFract32Class;
extern const ModClassModule awe_modProbeFract32Class;
extern const ModClassModule awe_modCopierClass;
extern const ModClassModule awe_modDeinterleaveClass;
extern const ModClassModule awe_modDelayClass;
extern const ModClassModule awe_modDelayMsecClass;
extern const ModClassModule awe_modDelayNTapClass;
extern const ModClassModule awe_modDownsamplerClass;
extern const ModClassModule awe_modFifoInClass;
extern const ModClassModule awe_modFifoOutClass;
extern const ModClassModule awe_modFileStreamingClass;
extern const ModClassModule awe_modFloatToIntClass;
extern const ModClassModule awe_modInterleaveClass;
extern const ModClassModule awe_modIntToFloatClass;
extern const ModClassModule awe_modMatlabStreamingClass;
extern const ModClassModule awe_modMultiplexorClass;
extern const ModClassModule awe_modRawStreamingSourceClass;
extern const ModClassModule awe_modRebufferClass;
extern const ModClassModule awe_modRouterClass;
extern const ModClassModule awe_modUpsamplerClass;
extern const ModClassModule awe_modFloatToFract32Class;
extern const ModClassModule awe_modFract32ToFloatClass;
extern const ModClassModule awe_modIntToFract32Class;
extern const ModClassModule awe_modZeroSourceClass;
extern const ModClassModule awe_modNullSinkClass;
extern const ModClassModule awe_modRandClass;
extern const ModClassModule awe_modDeMultiplexorClass;
extern const ModClassModule awe_modFaderExampleFract32Class;
extern const ModClassModule awe_modPeakHoldExampleFract32Class;
extern const ModClassModule awe_modAcosClass;
extern const ModClassModule awe_modAsinClass;
extern const ModClassModule awe_modAtanClass;
extern const ModClassModule awe_modAtan2Class;
extern const ModClassModule awe_modCeilClass;
extern const ModClassModule awe_modCosClass;
extern const ModClassModule awe_modCoshClass;
extern const ModClassModule awe_modDb10Class;
extern const ModClassModule awe_modDb20Class;
extern const ModClassModule awe_modDivideClass;
extern const ModClassModule awe_modExpClass;
extern const ModClassModule awe_modFloorClass;
extern const ModClassModule awe_modFmodClass;
extern const ModClassModule awe_modFrexpClass;
extern const ModClassModule awe_modLdexpClass;
extern const ModClassModule awe_modLogClass;
extern const ModClassModule awe_modLog2Class;
extern const ModClassModule awe_modLog10Class;
extern const ModClassModule awe_modModfClass;
extern const ModClassModule awe_modPowClass;
extern const ModClassModule awe_modPow10toXClass;
extern const ModClassModule awe_modReciprocalClass;
extern const ModClassModule awe_modRoundClass;
extern const ModClassModule awe_modSinClass;
extern const ModClassModule awe_modSinhClass;
extern const ModClassModule awe_modSqrtClass;
extern const ModClassModule awe_modTanClass;
extern const ModClassModule awe_modTanhClass;
extern const ModClassModule awe_modUndb10Class;
extern const ModClassModule awe_modUndb20Class;
extern const ModClassModule awe_modCycleBurnerClass;
extern const ModClassModule awe_modSampleRateClass;
extern const ModClassModule awe_modLogicBinaryOpClass;
extern const ModClassModule awe_modLogicAnyClass;
extern const ModClassModule awe_modLogicAllClass;
extern const ModClassModule awe_modConsecutiveCountIntClass;
extern const ModClassModule awe_modSinkIntClass;
extern const ModClassModule awe_modAdderInt32Class;
extern const ModClassModule awe_modSubtractInt32Class;
extern const ModClassModule awe_modSumDiffInt32Class;
extern const ModClassModule awe_modSampleAndHoldInt32Class;
extern const ModClassModule awe_modAllpassDelayFract32Class;
extern const ModClassModule awe_modDelayciFract32Class;
extern const ModClassModule awe_modScl1add2Fract32Class;
extern const ModClassModule awe_modFractionalDelayFract32Class;
extern const ModClassModule awe_modBiquadFract32X64Class;
extern const ModClassModule awe_modBiquadCascadeFract32X64Class;
extern const ModClassModule awe_modSecondOrderFilterSmoothedFract32X64Class;
extern const ModClassModule awe_modGraphicEQBandFract32X64Class;
extern const ModClassModule awe_modButterworthFilterFract32X64Class;
extern const ModClassModule awe_modBiquadSmoothedFract32X64Class;
extern const ModClassModule awe_modVolumeControlFract32X64Class;
extern const ModClassModule awe_modBiquadFract32X48Class;
extern const ModClassModule awe_modBiquadCascadeFract32X48Class;
extern const ModClassModule awe_modSecondOrderFilterSmoothedFract32X48Class;
extern const ModClassModule awe_modGraphicEQBandFract32X48Class;
extern const ModClassModule awe_modButterworthFilterFract32X48Class;
extern const ModClassModule awe_modBiquadSmoothedFract32X48Class;
extern const ModClassModule awe_modVolumeControlFract32X48Class;
extern const ModClassModule awe_modLatticeDelayFract32Class;
extern const ModClassModule awe_modLMSFract32Class;
extern const ModClassModule awe_modFftFract32Class;
extern const ModClassModule awe_modCfftFract32Class;
extern const ModClassModule awe_modIfftFract32Class;
extern const ModClassModule awe_modWindowAliasFract32Class;
extern const ModClassModule awe_modMP3DecoderClass;
extern const ModClassModule awe_modAllBlackfinDecodersClass;
extern const ModClassModule awe_modAC3DecoderClass;
extern const ModClassModule awe_modAACDecoderClass;
extern const ModClassModule awe_modDPL2XBlackfinFract32Class;
extern const ModClassModule awe_modDHBlackfinFract32Class;
extern const ModClassModule awe_modDVSBlackfinFract32Class;
extern const ModClassModule awe_modNEO6BlackfinFract32Class;
extern const ModClassModule awe_modFlangerFract32Class;
extern const ModClassModule awe_modChorusFract32Class;
extern const ModClassModule awe_modPitchShiftFract32Class;
extern const ModClassModule awe_modWindowOverlapFract32Class;
extern const ModClassModule awe_modOverlapAddFract32Class;
extern const ModClassModule awe_modRealImagToComplexFract32Class;
extern const ModClassModule awe_modComplexToRealImagFract32Class;
extern const ModClassModule awe_modZeroPaddingFract32Class;
extern const ModClassModule awe_modIntegrityCheckClass;
extern const ModClassModule awe_modBlockConcatenateClass;
extern const ModClassModule awe_modBlockDelayClass;
extern const ModClassModule awe_modBlockExtractClass;
extern const ModClassModule awe_modShiftSamplesClass;
extern const ModClassModule awe_modComplexMagnitudeFract32Class;
extern const ModClassModule awe_modBalanceFract32Class;
extern const ModClassModule awe_modAGCCoreFract32Class;
extern const ModClassModule awe_modLMSNormFract32Class;
extern const ModClassModule awe_modFreqScopeFract32Class;
extern const ModClassModule awe_modAGCCompressorCoreClass;
extern const ModClassModule awe_modBiquadNCascadeClass;
extern const ModClassModule awe_modImpulseSourceClass;
extern const ModClassModule awe_modPulseGenClass;
extern const ModClassModule awe_modBlockFlipClass;
extern const ModClassModule awe_modFract32ToIntClass;
extern const ModClassModule awe_modInvertClass;
extern const ModClassModule awe_modInvertFract32Class;
extern const ModClassModule awe_modImpulseSourceFract32Class;
extern const ModClassModule awe_modMathExceptionClass;
extern const ModClassModule awe_modScalerDBControlClass;


#define LISTOFCLASSOBJECTS \
&awe_modFloatToFract32Class, \
&awe_modFract32ToFloatClass, \
&awe_modDeinterleaveClass, \
&awe_modInterleaveClass, \
&awe_modRMSClass, \
&awe_modDb20Class, \
&awe_modMathExceptionClass, \
&awe_modSubtractClass, \
&awe_modClipAsymClass, \
&awe_modScalerClass, \
&awe_modScalerDBControlClass, \
&awe_modButterworthFilterClass, \
&awe_modBiquadCascadeClass, \
&awe_modSumDiffClass, \
&awe_modDelayClass, \
&awe_modDelayMsecClass, \
&awe_modAdderClass, \
&awe_modScalerDBClass, \
&awe_modRouterClass, \
&awe_modMeterClass, \
&awe_modSecondOrderFilterSmoothedClass


#endif	// _TARGETINFO_H

/**
* @}
*
* End of file.
*/
