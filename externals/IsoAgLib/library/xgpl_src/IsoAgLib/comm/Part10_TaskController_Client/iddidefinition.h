/*
  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/
#ifndef _IDDIDEFINITION_H_
#define _IDDIDEFINITION_H_

#define DDI_DATA_DICTIONARY_VERSION                0

#define DDI_APPRATE_VOLUME_PER_AREA_SETPOINT       1 // Units are 0.01 mm3/m2
#define DDI_APPRATE_VOLUME_PER_AREA_ACTUAL         2
#define DDI_APPRATE_VOLUME_PER_AREA_DEFAULT        3
#define DDI_APPRATE_VOLUME_PER_AREA_MINIMUM        4
#define DDI_APPRATE_VOLUME_PER_AREA_MAXIMUM        5

#define DDI_APPRATE_MASS_PER_AREA_SETPOINT         6 // Units are mg/m2
#define DDI_APPRATE_MASS_PER_AREA_ACTUAL           7
#define DDI_APPRATE_MASS_PER_AREA_DEFAULT          8
#define DDI_APPRATE_MASS_PER_AREA_MINIMUM          9
#define DDI_APPRATE_MASS_PER_AREA_MAXIMUM         10

#define DDI_APPRATE_COUNT_PER_AREA_SETPOINT       11 // Units are 0.001 count/m2
#define DDI_APPRATE_COUNT_PER_AREA_ACTUAL         12
#define DDI_APPRATE_COUNT_PER_AREA_DEFAULT        13
#define DDI_APPRATE_COUNT_PER_AREA_MINIMUM        14
#define DDI_APPRATE_COUNT_PER_AREA_MAXIMUM        15

#define DDI_APPRATE_SPACING_SETPOINT              16 // Units are mm
#define DDI_APPRATE_SPACING_ACTUAL                17
#define DDI_APPRATE_SPACING_DEFAULT               18
#define DDI_APPRATE_SPACING_MINIMUM               19
#define DDI_APPRATE_SPACING_MAXIMUM               20

#define DDI_APPRATE_VOLUME_PER_VOLUME_SETPOINT    21 // Units are mm3/m3
#define DDI_APPRATE_VOLUME_PER_VOLUME_ACTUAL      22
#define DDI_APPRATE_VOLUME_PER_VOLUME_DEFAULT     23
#define DDI_APPRATE_VOLUME_PER_VOLUME_MINIMUM     24
#define DDI_APPRATE_VOLUME_PER_VOLUME_MAXIMUM     25

#define DDI_APPRATE_MASS_PER_MASS_SETPOINT        26 // Units are mg/kg
#define DDI_APPRATE_MASS_PER_MASS_ACTUAL          27
#define DDI_APPRATE_MASS_PER_MASS_DEFAULT         28
#define DDI_APPRATE_MASS_PER_MASS_MINIMUM         29
#define DDI_APPRATE_MASS_PER_MASS_MAXIMUM         30

#define DDI_APPRATE_VOLUME_PER_MASS_SETPOINT      31 // Units are mm3/kg
#define DDI_APPRATE_VOLUME_PER_MASS_ACTUAL        32
#define DDI_APPRATE_VOLUME_PER_MASS_DEFAULT       33
#define DDI_APPRATE_VOLUME_PER_MASS_MINIMUM       34
#define DDI_APPRATE_VOLUME_PER_MASS_MAXIMUM       35

#define DDI_APPRATE_VOLUME_PER_TIME_SETPOINT      36 // Units are mm3/s
#define DDI_APPRATE_VOLUME_PER_TIME_ACTUAL        37
#define DDI_APPRATE_VOLUME_PER_TIME_DEFAULT       38
#define DDI_APPRATE_VOLUME_PER_TIME_MINIMUM       39
#define DDI_APPRATE_VOLUME_PER_TIME_MAXIMUM       40

#define DDI_APPRATE_MASS_PER_TIME_SETPOINT        41 // Units are mg/s
#define DDI_APPRATE_MASS_PER_TIME_ACTUAL          42
#define DDI_APPRATE_MASS_PER_TIME_DEFAULT         43
#define DDI_APPRATE_MASS_PER_TIME_MINIMUM         44
#define DDI_APPRATE_MASS_PER_TIME_MAXIMUM         45

#define DDI_APPRATE_COUNT_PER_TIME_SETPOINT       46 // Units are 0.001 count/s
#define DDI_APPRATE_COUNT_PER_TIME_ACTUAL         47
#define DDI_APPRATE_COUNT_PER_TIME_DEFAULT        48
#define DDI_APPRATE_COUNT_PER_TIME_MINIMUM        49
#define DDI_APPRATE_COUNT_PER_TIME_MAXIMUM        50

#define DDI_TILLAGE_DEPTH_SETPOINT                51 // Units are mm
#define DDI_TILLAGE_DEPTH_ACTUAL                  52
#define DDI_TILLAGE_DEPTH_DEFAULT                 53
#define DDI_TILLAGE_DEPTH_MINIMUM                 54
#define DDI_TILLAGE_DEPTH_MAXIMUM                 55

#define DDI_SEEDING_DEPTH_SETPOINT                56 // Units are mm
#define DDI_SEEDING_DEPTH_ACTUAL                  57
#define DDI_SEEDING_DEPTH_DEFAULT                 58
#define DDI_SEEDING_DEPTH_MINIMUM                 59
#define DDI_SEEDING_DEPTH_MAXIMUM                 60

#define DDI_WORKING_HEIGHT_SETPOINT               61 // Units are mm
#define DDI_WORKING_HEIGHT_ACTUAL                 62
#define DDI_WORKING_HEIGHT_DEFAULT                63
#define DDI_WORKING_HEIGHT_MINIMUM                64
#define DDI_WORKING_HEIGHT_MAXIMUM                65

#define DDI_WORKING_WIDTH_SETPOINT                66 // Units are mm
#define DDI_WORKING_WIDTH_ACTUAL                  67
#define DDI_WORKING_WIDTH_DEFAULT                 68
#define DDI_WORKING_WIDTH_MINIMUM                 69
#define DDI_WORKING_WIDTH_MAXIMUM                 70

#define DDI_CONTENT_VOLUME_SETPOINT               71 // Units are ml
#define DDI_CONTENT_VOLUME_ACTUAL                 72
#define DDI_CONTENT_VOLUME_MAXIMUM                73

#define DDI_CONTENT_MASS_SETPOINT                 74 // Units are g
#define DDI_CONTENT_MASS_ACTUAL                   75
#define DDI_CONTENT_MASS_MAXIMUM                  76

#define DDI_CONTENT_COUNT_SETPOINT                77 // Units are count
#define DDI_CONTENT_COUNT_ACTUAL                  78
#define DDI_CONTENT_COUNT_MAXIMUM                 79

#define DDI_APPLICATION_TOTAL_VOLUME              80 // l
#define DDI_APPLICATION_TOTAL_MASS                81 // kg
#define DDI_APPLICATION_TOTAL_COUNT               82 // count

#define DDI_YIELD_VOLUME_PER_AREA                 83 // ml/m2
#define DDI_YIELD_MASS_PER_AREA                   84 // mg/m2
#define DDI_YIELD_COUNT_PER_AREA                  85 // 0.001 count/m2

#define DDI_YIELD_VOLUME_PER_TIME                 86 // ml/s
#define DDI_YIELD_MASS_PER_TIME                   87 // mg/s
#define DDI_YIELD_COUNT_PER_TIME                  88 // 0.001 count/s

#define DDI_YIELD_TOTAL_VOLUME                    89 // ml
#define DDI_YIELD_TOTAL_MASS                      90 // g
#define DDI_YIELD_TOTAL_COUNT                     91 // count

#define DDI_CROP_LOSS_VOLUME_PER_AREA             92 // ml/m2
#define DDI_CROP_LOSS_MASS_PER_AREA               93 // mg/m2
#define DDI_CROP_LOSS_COUNT_PER_AREA              94 // 0.001 count/m2

#define DDI_CROP_LOSS_VOLUME_PER_TIME             95 // ml/s
#define DDI_CROP_LOSS_MASS_PER_TIME               96 // mg/s
#define DDI_CROP_LOSS_COUNT_PER_TIME              97 // 0.001 count/s

#define DDI_PERCENTAGE_CROP_LOSS                  98 // ppm
#define DDI_CROP_MOISTURE                         99 // ppm
#define DDI_CROP_CONTAMINATION                   100 // ppm

#define DDI_BALE_WIDTH_SETPOINT                  101 // Units are mm
#define DDI_BALE_WIDTH_ACTUAL                    102
#define DDI_BALE_WIDTH_DEFAULT                   103
#define DDI_BALE_WIDTH_MINIMUM                   104
#define DDI_BALE_WIDTH_MAXIMUM                   105

#define DDI_BALE_HEIGHT_SETPOINT                 106 // Units are mm
#define DDI_BALE_HEIGHT_ACTUAL                   107
#define DDI_BALE_HEIGHT_DEFAULT                  108
#define DDI_BALE_HEIGHT_MINIMUM                  109
#define DDI_BALE_HEIGHT_MAXIMUM                  110

#define DDI_BALE_SIZE_SETPOINT                   111 // Units are mm
#define DDI_BALE_SIZE_ACTUAL                     112
#define DDI_BALE_SIZE_DEFAULT                    113
#define DDI_BALE_SIZE_MINIMUM                    114
#define DDI_BALE_SIZE_MAXIMUM                    115

#define DDI_TOTAL_AREA                           116 // m2
#define DDI_TOTAL_EFFECTIVE_DISTANCE             117 // m
#define DDI_TOTAL_INEFFECTIVE_DISTANCE           118 // m
#define DDI_TOTAL_EFFECTIVE_TIME                 119 // s
#define DDI_TOTAL_INEFFECTIVE_TIME               120 // s

#define DDI_PRODUCT_DENSITY_MASS_PER_VOLUME      121 // g/l
#define DDI_PRODUCT_DENSITY_MASS_PER_COUNT       122 // mg/1000
#define DDI_PRODUCT_DENSITY_VOLUME_PER_COUNT     123 // ml/1000

#define DDI_AUXVALVE_EXTEND_SCALING              124 // 0.1 %
#define DDI_AUXVALVE_RETRACT_SCALING             125 // 0.1 %
#define DDI_AUXVALVE_EXTEND_RAMPUP               126 // ms
#define DDI_AUXVALVE_EXTEND_RAMPDOWN             127 // 0.1 ms
#define DDI_AUXVALVE_RETRACT_RAMPUP              128 // ms
#define DDI_AUXVALVE_RETRACT_RAMPDOWN            129 // ms
#define DDI_AUXVALVE_FLOAT_THRESHOLD             130 // 0.1 %
#define DDI_AUXVALVE_EXTEND_PROGRESSIVITY        131
#define DDI_AUXVALVE_RETRACT_PROGRESSIVITY       132
#define DDI_AUXVALVE_INVERT_PORTS                133

#define DDI_DEVICE_ELEMENT_OFFSET_X              134 // mm
#define DDI_DEVICE_ELEMENT_OFFSET_Y              135 // mm
#define DDI_DEVICE_ELEMENT_OFFSET_Z              136 // mm

#define DDI_DEVICE_CAPACITY_VOLUME               137 // ml
#define DDI_DEVICE_CAPACITY_MASS                 138 // g
#define DDI_DEVICE_CAPACITY_COUNT                139 // count

#define DDI_APPRATE_PERCENTAGE_SETPOINT          140 // ppm

#define DDI_WORKSTATE                            141 // 00 - disabled, 01 - enabled, 10 - error, 11 - not installed

#define DDI_TIME_LATENCY_SETPOINT                142 // ms
#define DDI_TIME_LATENCY_ACTUAL                  143 // ms
 
#define DDI_YAW_ANGLE                            144 // 0.001 degree
#define DDI_ROLL_ANGLE                           145 // 0.001 degree
#define DDI_PITCH_ANGLE                          146 // 0.001 degree

#define DDI_LOG_COUNT                            147

#define DDI_FUEL_CONSUMPTION_TOTAL               148 // ml
#define DDI_FUEL_CONSUMPTION_PER_TIME            149 // mm3/sec
#define DDI_FUEL_CONSUMPTION_PER_AREA            150 // mm3/m2
 
#define DDI_AREA_PER_TIME_CAPACITY               151 // mm2/s

#define DDI_ACTUAL_NORMALIZED_DIFFERENCE_VEGETATIVE_INDEX  153 // unit not defined
#define DDI_PHYSICAL_OBJECT_LENGTH               154 // mm
#define DDI_PHYSICAL_OBJECT_WIDTH                155 // mm
#define DDI_PHYSICAL_OBJECT_HEIGHT               156 // mm

#define DDI_CONNECTOR_TYPE                       157 // 0 - unknown (default), 1 - ISO 6489-3 Tractor drawbar, 2 - ISO 730 Three-point-hitch semi-mounted, 3 - ISO 730 Three-point-hitch mounted, 4 - ISO 6489-1 Hitch-hook, 5 - ISO 6489-2 Clevis coupling 40, 6 - ISO 6489-4 Piton type coupling, 7 - ISO 6489-5 CUNA hitch, or ISO 5692-2 Pivot wagon hitch, 8 - ISO 24347 Ball type hitch, all other values are reserved for future assignments
#define DDI_PRESCRIPTION_CONTROL_STATE           158 // 00 - disabled, 01 - enabled, 10 - error, 11 - not installed
#define DDI_NUMBER_OF_SUBUNITS_PER_SECTION       159 // count
#define DDI_SECTION_CONTROL_STATE                160 // 00 - disabled, 01 - enabled, 10 - error, 11 - not installed
#define DDI_CONDENSED_WORK_STATE1_16             161 // sections 1-16 bit fields 00 - disabled, 01 - enabled, 10 - error, 11 - not installed
#define DDI_CONDENSED_WORK_STATE17_32            162 // sections 17-32 bit fields 00 - disabled, 01 - enabled, 10 - error, 11 - not installed
#define DDI_CONDENSED_WORK_STATE33_48            163 // sections 33-48 bit fields 00 - disabled, 01 - enabled, 10 - error, 11 - not installed
#define DDI_CONDENSED_WORK_STATE49_64            164 // sections 49-64 bit fields 00 - disabled, 01 - enabled, 10 - error, 11 - not installed
#define DDI_CONDENSED_WORK_STATE65_80            165 // sections 65-80 bit fields 00 - disabled, 01 - enabled, 10 - error, 11 - not installed
#define DDI_CONDENSED_WORK_STATE81_96            166 // sections 81-96 bit fields 00 - disabled, 01 - enabled, 10 - error, 11 - not installed
#define DDI_CONDENSED_WORK_STATE97_112           167 // sections 97-112 bit fields 00 - disabled, 01 - enabled, 10 - error, 11 - not installed
#define DDI_CONDENSED_WORK_STATE113_128          168 // sections 113-128 bit fields 00 - disabled, 01 - enabled, 10 - error, 11 - not installed
#define DDI_CONDENSED_WORK_STATE129_144          169 // sections 129-144 bit fields 00 - disabled, 01 - enabled, 10 - error, 11 - not installed
#define DDI_CONDENSED_WORK_STATE145_160          170 // sections 145-160 bit fields 00 - disabled, 01 - enabled, 10 - error, 11 - not installed
#define DDI_CONDENSED_WORK_STATE161_176          171 // sections 161-176 bit fields 00 - disabled, 01 - enabled, 10 - error, 11 - not installed
#define DDI_CONDENSED_WORK_STATE177_192          172 // sections 177-192 bit fields 00 - disabled, 01 - enabled, 10 - error, 11 - not installed
#define DDI_CONDENSED_WORK_STATE193_208          173 // sections 193-208 bit fields 00 - disabled, 01 - enabled, 10 - error, 11 - not installed
#define DDI_CONDENSED_WORK_STATE209_224          174 // sections 209-224 bit fields 00 - disabled, 01 - enabled, 10 - error, 11 - not installed
#define DDI_CONDENSED_WORK_STATE225_240          175 // sections 225-240 bit fields 00 - disabled, 01 - enabled, 10 - error, 11 - not installed
#define DDI_CONDENSED_WORK_STATE241_256          176 // sections241-256 bit fields 00 - disabled, 01 - enabled, 10 - error, 11 - not installed

#define DDI_ACTUAL_LENGTH_OF_CUT                 177 // mm

#define DDI_ELEMENT_TYPE_INSTANCE                178
#define DDI_ACTUAL_CULTURAL_PRACTICE             179 // 0=Unknown, 1=Fertilizing, 2=Sowing and Planting, 3=Crop Protection, 4=Tillage
                                                     // 5=Baling, 6=Mowing, 7=Wrapping, 8=Harvesting, 9=Forage Harvesting, 10=Transport,
                                                     // 11-255=Reserved
                                                     
#define DDI_DEVICE_REFERENCE_POINT_TO_GROUND_DISTANCE  180 // mm

#define DDI_DRY_MASS_PER_AREA_YIELD              181 // mg/m2
#define DDI_DRY_MASS_PER_TIME_YIELD              182 // mg/s
#define DDI_YIELD_TOTAL_DRY_MASS                 183 // kg
#define DDI_REFERENCE_MOISTURE_FOR_DRY_MASS      184 // ppm

#define DDI_SEED_COTTON_MASS_PER_AREA_YIELD      185 // mg/m2
#define DDI_LINT_COTTON_MASS_PER_AREA_YIELD      186 // mg/m2
#define DDI_SEED_COTTON_MASS_PER_TIME_YIELD      187 // mg/s
#define DDI_LINT_COTTON_MASS_PER_TIME_YIELD      188 // mg/s

#define DDI_YIELD_TOTAL_SEED_COTTON_MASS         189 // kg
#define DDI_YIELD_TOTAL_LINT_COTTON_MASS         190 // kg

#define DDI_LINT_TURNOUT_PERCENTAGE              191 // ppm

#define DDI_AMBIENT_TEMPERATURE                  192 // mK

#define DDI_SETPOINT_PRODUCT_PRESSURE            193 // Units are Pa
#define DDI_ACTUAL_PRODUCT_PRESSURE              194
#define DDI_MINIMUM_PRODUCT_PRESSURE             195
#define DDI_MAXIMUM_PRODUCT_PRESSURE             196

#define DDI_SETPOINT_PUMP_OUTPUT_PRESSURE        197 // Units are Pa
#define DDI_ACTUAL_PUMP_OUTPUT_PRESSURE          198
#define DDI_MINIMUM_PUMP_OUTPUT_PRESSURE         199
#define DDI_MAXIMUM_PUMP_OUTPUT_PRESSURE         200

#define DDI_SETPOINT_TANK_AGITATION_PRESSURE     201 // Units are Pa
#define DDI_ACTUAL_TANK_AGITATION_PRESSURE       202
#define DDI_MINIMUM_TANK_AGITATION_PRESSURE      203
#define DDI_MAXIMUM_TANK_AGITATION_PRESSURE      204

#define DDI_SC_TURN_ON_TIME                      205 // ms
#define DDI_SC_TURN_OFF_TIME                     206 // ms

#define DDI_WIND_SPEED                           207 // mm/s
#define DDI_WIND_DIRECTION                       208 // degree
#define DDI_AIR_HUMIDITY                         209 // percent
#define DDI_SKY_CONDITIONS                       210

#define DDI_LAST_BALE_FLAKES_PER_BALE            211
#define DDI_LAST_BALE_AVERAGE_MOISTURE           212
#define DDI_LAST_BALE_AVERAGE_STROKES_PER_FLAKE  213
#define DDI_LIFETIME_BALE_COUNT                  214

#define DDI_LIFETIME_WORKING_HOURS               215

#define DDI_ACTUAL_BALE_HYDRAULIC_PRESSURE                       216
#define DDI_LAST_BALE_AVERAGE_HYDRAULIC_PRESSURE                 217

#define DDI_SETPOINT_BALE_COMPRESSION_PLUNGER_LOAD               218
#define DDI_ACTUAL_BALE_COMPRESSION_PLUNGER_LOAD                 219

#define DDI_LAST_BALE_AVERAGE_BALE_COMPRESSION_PLUNGER_LOAD      220
#define DDI_LAST_BALE_APPLIED_PRESERVATIVE       221
#define DDI_LAST_BALE_TAG_NUMBER                 222
#define DDI_LAST_BALE_MASS                       223

#define DDI_DELTA_T                              224

#define DDI_SETPOINT_WORKING_LENGTH              225
#define DDI_ACTUAL_WORKING_LENGTH                226
#define DDI_MINIMUM_WORKING_LENGTH               227
#define DDI_MAXIMUM_WORKING_LENGTH               228

#define DDI_ACTUAL_NET_WEIGHT                    229
#define DDI_NET_WEIGHT_STATE                     230

#define DDI_SETPOINT_NET_WEIGHT                  231
#define DDI_ACTUAL_GROSS_WEIGHT                  232
#define DDI_GROSS_WEIGHT_STATE                   233
#define DDI_MINIMUM_GROSS_WEIGHT                 234
#define DDI_MAXIMUM_GROSS_WEIGHT                 235

#define DDI_THRESHER_ENGAGEMENT_TOTAL_TIME       236
#define DDI_ACTUAL_HEADER_WORKING_HEIGHT_STATUS      237
#define DDI_ACTUAL_HEADER_ROTATIONAL_SPEED_STATUS    238
#define DDI_YIELD_HOLD_STATUS                    239
#define DDI_ACTUAL__UN_LOADING_SYSTEM_STATUS     240
#define DDI_CROP_TEMPERATURE                     241

#define DDI_SETPOINT_SIEVE_CLEARANCE             242
#define DDI_ACTUAL_SIEVE_CLEARANCE               243
#define DDI_MINIMUM_SIEVE_CLEARANCE              244
#define DDI_MAXIMUM_SIEVE_CLEARANCE              245

#define DDI_SETPOINT_CHAFFER_CLEARANCE           246
#define DDI_ACTUAL_CHAFFER_CLEARANCE             247
#define DDI_MINIMUM_CHAFFER_CLEARANCE            248
#define DDI_MAXIMUM_CHAFFER_CLEARANCE            249

#define DDI_SETPOINT_CONCAVE_CLEARANCE           250
#define DDI_ACTUAL_CONCAVE_CLEARANCE             251
#define DDI_MINIMUM_CONCAVE_CLEARANCE            252
#define DDI_MAXIMUM_CONCAVE_CLEARANCE            253

#define DDI_SETPOINT_SEPARATION_FAN_ROTATIONAL_SPEED     254
#define DDI_ACTUAL_SEPARATION_FAN_ROTATIONAL_SPEED       255
#define DDI_MINIMUM_SEPARATION_FAN_ROTATIONAL_SPEED      256
#define DDI_MAXIMUM_SEPARATION_FAN_ROTATIONAL_SPEED      257

#define DDI_HYDRAULIC_OIL_TEMPERATURE            258
#define DDI_YIELD_LAG_IGNORE_TIME                259
#define DDI_YIELD_LEAD_IGNORE_TIME               260
#define DDI_AVERAGE_YIELD_MASS_PER_TIME          261
#define DDI_AVERAGE_CROP_MOISTURE                262
#define DDI_AVERAGE_YIELD_MASS_PER_AREA          263
#define DDI_CONNECTOR_PIVOT_X                    264
#define DDI_REMAINING_AREA                       265

#define DDI_LIFETIME_APPLICATION_TOTAL_MASS      266
#define DDI_LIFETIME_APPLICATION_TOTAL_COUNT     267

#define DDI_LIFETIME_YIELD_TOTAL_VOLUME          268
#define DDI_LIFETIME_YIELD_TOTAL_MASS            269
#define DDI_LIFETIME_YIELD_TOTAL_COUNT           270

#define DDI_LIFETIME_TOTAL_AREA                  271
#define DDI_LIFETIME_TOTAL_DISTANCE              272

#define DDI_LIFETIME_INEFFECTIVE_TOTAL_DISTANCE  273
#define DDI_LIFETIME_EFFECTIVE_TOTAL_TIME        274
#define DDI_LIFETIME_INEFFECTIVE_TOTAL_TIME      275

#define DDI_LIFETIME_FUEL_CONSUMPTION            276

#define DDI_LIFETIME_AVERAGE_FUEL_CONSUMPTION_PER_TIME  277
#define DDI_LIFETIME_AVERAGE_FUEL_CONSUMPTION_PER_AREA  278

#define DDI_LIFETIME_YIELD_TOTAL_DRY_MASS               279
#define DDI_LIFETIME_YIELD_TOTAL_SEED_COTTON_MASS       280
#define DDI_LIFETIME_YIELD_TOTAL_LINT_COTTON_MASS       281

#define DDI_LIFETIME_THRESHING_ENGAGEMENT_TOTAL_TIME    282

#define DDI_PRECUT_TOTAL_COUNT                   283
#define DDI_UNCUT_TOTAL_COUNT                    284

#define DDI_LIFETIME_PRECUT_TOTAL_COUNT          285
#define DDI_LIFETIME_UNCUT_TOTAL_COUNT           286

#define DDI_SETPOINT_WORK_STATE                  289 // 00 - disabled, 01 - enabled, 10 - error, 11 - not installed
#define DDI_SETPOINT_CONDENSED_WORK_STATE1_16    290 // sections 1-16 bit fields 00 - disabled, 01 - enabled, 10 - error, 11 - not installed
#define DDI_SETPOINT_CONDENSED_WORK_STATE17_32   291 // sections 17-32 bit fields 00 - disabled, 01 - enabled, 10 - error, 11 - not installed
#define DDI_SETPOINT_CONDENSED_WORK_STATE33_48   292 // sections 33-48 bit fields 00 - disabled, 01 - enabled, 10 - error, 11 - not installed
#define DDI_SETPOINT_CONDENSED_WORK_STATE49_64   293 // sections 49-64 bit fields 00 - disabled, 01 - enabled, 10 - error, 11 - not installed
#define DDI_SETPOINT_CONDENSED_WORK_STATE65_80   294 // sections 65-80 bit fields 00 - disabled, 01 - enabled, 10 - error, 11 - not installed
#define DDI_SETPOINT_CONDENSED_WORK_STATE81_96   295 // sections 81-96 bit fields 00 - disabled, 01 - enabled, 10 - error, 11 - not installed
#define DDI_SETPOINT_CONDENSED_WORK_STATE97_112  296 // sections 97-112 bit fields 00 - disabled, 01 - enabled, 10 - error, 11 - not installed
#define DDI_SETPOINT_CONDENSED_WORK_STATE113_128 297 // sections 112-128 bit fields 00 - disabled, 01 - enabled, 10 - error, 11 - not installed
#define DDI_SETPOINT_CONDENSED_WORK_STATE129_144 298 // sections 129-144 bit fields 00 - disabled, 01 - enabled, 10 - error, 11 - not installed
#define DDI_SETPOINT_CONDENSED_WORK_STATE145_160 299 // sections 145-160 bit fields 00 - disabled, 01 - enabled, 10 - error, 11 - not installed
#define DDI_SETPOINT_CONDENSED_WORK_STATE161_176 300 // sections 161-176 bit fields 00 - disabled, 01 - enabled, 10 - error, 11 - not installed
#define DDI_SETPOINT_CONDENSED_WORK_STATE177_192 301 // sections 177-192 bit fields 00 - disabled, 01 - enabled, 10 - error, 11 - not installed
#define DDI_SETPOINT_CONDENSED_WORK_STATE193_208 302 // sections 193-208 bit fields 00 - disabled, 01 - enabled, 10 - error, 11 - not installed
#define DDI_SETPOINT_CONDENSED_WORK_STATE209_224 303 // sections 209-224 bit fields 00 - disabled, 01 - enabled, 10 - error, 11 - not installed
#define DDI_SETPOINT_CONDENSED_WORK_STATE225_240 304 // sections 225-240 bit fields 00 - disabled, 01 - enabled, 10 - error, 11 - not installed
#define DDI_SETPOINT_CONDENSED_WORK_STATE241_256 305 // sections 241-256 bit fields 00 - disabled, 01 - enabled, 10 - error, 11 - not installed

#define DDI_TRUE_ROTATION_POINT_X                306
#define DDI_TRUE_ROTATION_POINT_Y                307

#define DDI_ACTUAL_PERCENTAGE_APPLICATION_RATE   308
#define DDI_MINIMUM_PERCENTAGE_APPLICATION_RATE  309
#define DDI_MAXIMUM_PERCENTAGE_APPLICATION_RATE  310

#define DDI_RELATIVE_YIELD_POTENTIAL             311
#define DDI_MINIMUM_RELATIVE_YIELD_POTENTIAL     312
#define DDI_MAXIMUM_RELATIVE_YIELD_POTENTIAL     313

#define DDI_ACTUAL_PERCENTAGE_CROP_DRY_MATTER    314
#define DDI_AVERAGE_PERCENTAGE_CROP_DRY_MATTER_  315

#define DDI_EFFECTIVE_TOTAL_FUEL_CONSUMPTION     316
#define DDI_INEFFECTIVE_TOTAL_FUEL_CONSUMPTION   317

#define DDI_EFFECTIVE_TOTAL_DIESEL_EXHAUST_FLUID_CONSUMPTION    318
#define DDI_INEFFECTIVE_TOTAL_DIESEL_EXHAUST_FLUID_CONSUMPTION  319

#define DDI_LAST_LOADED_WEIGHT                   320
#define DDI_LAST_UNLOADED_WEIGHT                 321
#define DDI_LOAD_IDENTIFICATION_NUMBER           322
#define DDI_UNLOAD_IDENTIFICATION_NUMBER         323

#define DDI_CHOPPER_ENGAGEMENT_TOTAL_TIME        324

#define DDI_PGN_BASED_LOGGING                    0xDFFE
#define DDI_DEFAULT_PROCESS_DATA                 0xDFFF

#define DDI_INVALID_DDI                          0xFFFF

#endif // _IDDIDEFINITION_H_
