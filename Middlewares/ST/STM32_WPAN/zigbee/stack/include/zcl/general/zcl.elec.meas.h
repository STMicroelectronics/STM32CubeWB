/**
 * @file zcl.elec.meas.h
 * @heading Electrical Measurement
 * @brief ZCL Electrical Measurement cluster header
 * ZCL 7 section 4.9
 * ZCL 8 section 4.9
 * @copyright Copyright [2009 - 2022] Exegin Technologies Limited. All rights reserved.
 */

#ifndef ZCL_ELEC_MEAS_H
#define ZCL_ELEC_MEAS_H

#include "zcl/zcl.h"

/* @PICS.ZCL.Electrical.Measurement
 * EMR.S | Server | True
 * EMR.C | Client | True
 *
 * Server Attributes
 * EMR.S.A0000 | MeasurementType | True
 * EMR.S.A0100 | DCVoltage | True | Optional
 * EMR.S.A0101 | DCVoltageMin | True | Optional
 * EMR.S.A0102 | DCVoltageMax | True | Optional
 * EMR.S.A0103 | DCCurrent | False | Optional
 * EMR.S.A0104 | DCCurrentMin | False | Optional
 * EMR.S.A0105 | DCCurrentMax | False | Optional
 * EMR.S.A0106 | DCPower | False | Optional
 * EMR.S.A0107 | DCPowerMin | False | Optional
 * EMR.S.A0108 | DCPowerMax | False | Optional
 * EMR.S.A0200 | DCVoltageMultiplier | True | Optional
 * EMR.S.A0201 | DCVoltageDivisor | True | Optional
 * EMR.S.A0202 | DCCurrentMultiplier | False | Optional
 * EMR.S.A0203 | DCCurrentDivisor | False | Optional
 * EMR.S.A0204 | DCPowerMultiplier | False | Optional
 * EMR.S.A0205 | DCPowerDivisor | False | Optional
 * EMR.S.A0300 | ACFrequency | False | Optional
 * EMR.S.A0301 | ACFrequencyMin | False | Optional
 * EMR.S.A0302 | ACFrequencyMax | False | Optional
 * EMR.S.A0303 | NeutralCurrent | False | Optional
 * EMR.S.A0304 | TotalActivePower | False | Optional
 * EMR.S.A0305 | TotalReactivePower | False | Optional
 * EMR.S.A0306 | TotalApparentPower | False | Optional
 * EMR.S.A0307 | Measured1stHarmonicCurrent | False | Optional
 * EMR.S.A0308 | Measured3rdHarmonicCurrent | False | Optional
 * EMR.S.A0309 | Measured5thHarmonicCurrent | False | Optional
 * EMR.S.A030a | Measured7thHarmonicCurrent | False | Optional
 * EMR.S.A030b | Measured9thHarmonicCurrent | False | Optional
 * EMR.S.A030c | Measured11thHarmonicCurrent | False | Optional
 * EMR.S.A030d | MeasuredPhase1stHarmonicCurrent | False | Optional
 * EMR.S.A030e | MeasuredPhase3rdHarmonicCurrent | False | Optional
 * EMR.S.A030f | MeasuredPhase5thHarmonicCurrent | False | Optional
 * EMR.S.A0310 | MeasuredPhase7thHarmonicCurrent | False | Optional
 * EMR.S.A0311 | MeasuredPhase9thHarmonicCurrent | False | Optional
 * EMR.S.A0312 | MeasuredPhase11thHarmonicCurrent | False | Optional
 * EMR.S.A0400 | ACFrequencyMultiplier | False | Optional
 * EMR.S.A0401 | ACFrequencyDivisor | False | Optional
 * EMR.S.A0402 | PowerMultiplier | False | Optional
 * EMR.S.A0403 | PowerDivisor | False | Optional
 * EMR.S.A0404 | HarmonicCurrentMultiplier | False | Optional
 * EMR.S.A0405 | PhaseHarmonicCurrentMultiplier | False | Optional
 * EMR.S.A0501 | LineCurrent | False | Optional
 * EMR.S.A0502 | ActiveCurrent | False | Optional
 * EMR.S.A0503 | ReactiveCurrent | False | Optional
 * EMR.S.A0505 | RMSVoltage | False | Optional
 * EMR.S.A0506 | RMSVoltageMin | False | Optional
 * EMR.S.A0507 | RMSVoltageMax | False | Optional
 * EMR.S.A0508 | RMSCurrent | False | Optional
 * EMR.S.A0509 | RMSCurrentMin | False | Optional
 * EMR.S.A050a | RMSCurrentMax | False | Optional
 * EMR.S.A050b | ActivePower | False | Optional
 * EMR.S.A050c | ActivePowerMin | False | Optional
 * EMR.S.A050d | ActivePowerMax | False | Optional
 * EMR.S.A050e | ReactivePower | False | Optional
 * EMR.S.A050f | ApparentPower | False | Optional
 * EMR.S.A0510 | PowerFactor | False | Optional
 * EMR.S.A0511 | AverageRMSVoltageMeasurementPeriod | False | Optional
 * EMR.S.A0512 | AverageRMSOverVoltageCounter | False | Optional
 * EMR.S.A0513 | AverageRMSUnderVoltageCounter | False | Optional
 * EMR.S.A0514 | RMSExtremeOverVoltagePeriod | False | Optional
 * EMR.S.A0515 | RMSExtremeUnderVoltagePeriod | False | Optional
 * EMR.S.A0516 | RMSVoltageSagPeriod | False | Optional
 * EMR.S.A0517 | RMSVoltageSwellPeriod | False | Optional
 * EMR.S.A0600 | ACVoltageMultiplier | False | Optional
 * EMR.S.A0601 | ACVoltageDivisor | False | Optional
 * EMR.S.A0602 | ACCurrentMultiplier | False | Optional
 * EMR.S.A0603 | ACCurrentDivisor | False | Optional
 * EMR.S.A0604 | ACPowerMultiplier | False | Optional
 * EMR.S.A0605 | ACPowerDivisor | False | Optional
 * EMR.S.A0700 | DCOverloadAlarmsMask | True | Optional
 * EMR.S.A0701 | DCVoltageOverload | True | Optional
 * EMR.S.A0702 | DCCurrentOverload | False | Optional
 * EMR.S.A0800 | ACAlarmsMask | False | Optional
 * EMR.S.A0801 | ACVoltageOverload | False | Optional
 * EMR.S.A0802 | ACCurrentOverload | False | Optional
 * EMR.S.A0803 | ACActivePowerOverload | False | Optional
 * EMR.S.A0804 | ACReactivePowerOverload | False | Optional
 * EMR.S.A0805 | AverageRMSOverVoltage | False | Optional
 * EMR.S.A0806 | AverageRMSUnderVoltage | False | Optional
 * EMR.S.A0807 | RMSExtremeOverVoltage | False | Optional
 * EMR.S.A0808 | RMSExtremeUnderVoltage | False | Optional
 * EMR.S.A0809 | RMSVoltageSag | False | Optional
 * EMR.S.A080a | RMSVoltageSwell | False | Optional
 * EMR.S.A0901 | LineCurrentPhB | False | Optional
 * EMR.S.A0902 | ActiveCurrentPhB | False | Optional
 * EMR.S.A0903 | ReactiveCurrentPhB | False | Optional
 * EMR.S.A0905 | RMSVoltagePhB | False | Optional
 * EMR.S.A0906 | RMSVoltageMinPhB | False | Optional
 * EMR.S.A0907 | RMSVoltageMaxPhB | False | Optional
 * EMR.S.A0908 | RMSCurrentPhB | False | Optional
 * EMR.S.A0909 | RMSCurrentMinPhB | False | Optional
 * EMR.S.A090a | RMSCurrentMaxPhB | False | Optional
 * EMR.S.A090b | ActivePowerPhB | False | Optional
 * EMR.S.A090c | ActivePowerMinPhB | False | Optional
 * EMR.S.A090d | ActivePowerMaxPhB | False | Optional
 * EMR.S.A090e | ReactivePowerPhB | False | Optional
 * EMR.S.A090f | ApparentPowerPhB | False | Optional
 * EMR.S.A0910 | PowerFactorPhB | False | Optional
 * EMR.S.A0911 | AverageRMSVoltageMeasurementPeriodPhB | False | Optional
 * EMR.S.A0912 | AverageRMSOverVoltageCounterPhB | False | Optional
 * EMR.S.A0913 | AverageRMSUnderVoltageCounterPhB | False | Optional
 * EMR.S.A0914 | RMSExtremeOverVoltagePeriodPhB | False | Optional
 * EMR.S.A0915 | RMSExtremeUnderVoltagePeriodPhB | False | Optional
 * EMR.S.A0916 | RMSVoltageSagPeriodPhB | False | Optional
 * EMR.S.A0917 | RMSVoltageSwellPeriodPhB | False | Optional
 * EMR.S.A0a01 | LineCurrentPhC | False | Optional
 * EMR.S.A0a02 | ActiveCurrentPhC | False | Optional
 * EMR.S.A0a03 | ReactiveCurrentPhC | False | Optional
 * EMR.S.A0a05 | RMSVoltagePhC | False | Optional
 * EMR.S.A0a06 | RMSVoltageMinPhC | False | Optional
 * EMR.S.A0a07 | RMSVoltageMaxPhC | False | Optional
 * EMR.S.A0a08 | RMSCurrentPhC | False | Optional
 * EMR.S.A0a09 | RMSCurrentMinPhC | False | Optional
 * EMR.S.A0a0a | RMSCurrentMaxPhC | False | Optional
 * EMR.S.A0a0b | ActivePowerPhC | False | Optional
 * EMR.S.A0a0c | ActivePowerMinPhC | False | Optional
 * EMR.S.A0a0d | ActivePowerMaxPhC | False | Optional
 * EMR.S.A0a0e | ReactivePowerPhC | False | Optional
 * EMR.S.A0a0f | ApparentPowerPhC | False | Optional
 * EMR.S.A0a10 | PowerFactorPhC | False | Optional
 * EMR.S.A0a11 | AverageRMSVoltageMeasurementPeriodPhC | False | Optional
 * EMR.S.A0a12 | AverageRMSOverVoltageCounterPhC | False | Optional
 * EMR.S.A0a13 | AverageRMSUnderVoltageCounterPhC | False | Optional
 * EMR.S.A0a14 | RMSExtremeOverVoltagePeriodPhC | False | Optional
 * EMR.S.A0a15 | RMSExtremeUnderVoltagePeriodPhC | False | Optional
 * EMR.S.A0a16 | RMSVoltageSagPeriodPhC | False | Optional
 * EMR.S.A0a17 | RMSVoltageSwellPeriodPhC | False | Optional
 * EMR.S.Afffd | ClusterRevision | True
 * EMR.S.Afffe | AttributeReportingStatus | True

 * Commands Received
 * EMR.S.C00.Rsp | Get Profile Info Command | Depends on customer application
 * EMR.S.C01.Rsp | Get Measurement Profile Command | Depends on customer application

 * Commands Generated
 * EMR.S.C00.Tx | Get Profile Info Response Command | Depends on customer application
 * EMR.S.C01.Tx | Get Measurement Profile Response Command | Depends on customer application
 *
 * Client Attributes
 * DRLK.C.Afffd | ClusterRevision | True
 * DRLK.C.Afffe | AttributeReportingStatus | True
 *
 * Commands Received
 * EMR.C.C00.Rsp | Get Profile Info Response Command | Depends on customer application
 * EMR.C.C01.Rsp | Get Measurement Profile Response Command | Depends on customer application
 *
 * Commands Generated
 * EMR.C.C00.Tx | Get Profile Info Command | True
 * EMR.C.C01.Tx | Get Measurement Profile Command | True
 */

/** Electrical Measurement Server Attribute IDs */
enum ZbZclElecMeasSvrAttrT {
    ZCL_ELEC_MEAS_ATTR_MEAS_TYPE = 0x0000, /**< MeasurementType */
    ZCL_ELEC_MEAS_ATTR_DC_VOLT = 0x0100, /**< DCVoltage (Optional) */
    ZCL_ELEC_MEAS_ATTR_DC_VOLT_MIN = 0x0101, /**< DCVoltageMin (Optional) */
    ZCL_ELEC_MEAS_ATTR_DC_VOLT_MAX = 0x0102, /**< DCVoltageMax (Optional) */
    ZCL_ELEC_MEAS_ATTR_DC_CURRENT = 0x0103, /**< DCCurrent (Optional) */
    ZCL_ELEC_MEAS_ATTR_DC_CURRENT_MIN = 0x0104, /**< DCCurrentMin (Optional) */
    ZCL_ELEC_MEAS_ATTR_DC_CURRENT_MAX = 0x0105, /**< DCCurrentMax (Optional) */
    ZCL_ELEC_MEAS_ATTR_DC_POWER = 0x0106, /**< DCPower (Optional) */
    ZCL_ELEC_MEAS_ATTR_DC_POWER_MIN = 0x0107, /**< DCPowerMin (Optional) */
    ZCL_ELEC_MEAS_ATTR_DC_POWER_MAX = 0x0108, /**< DCPowerMax (Optional) */
    ZCL_ELEC_MEAS_ATTR_DC_VOLT_MULTIPLIER = 0x0200, /**< DCVoltageMultiplier (Optional) */
    ZCL_ELEC_MEAS_ATTR_DC_VOLT_DIVISOR = 0x0201, /**< DCVoltageDivisor (Optional) */
    ZCL_ELEC_MEAS_ATTR_DC_CURR_MULTIPLIER = 0x0202, /**< DCCurrentMultiplier (Optional) */
    ZCL_ELEC_MEAS_ATTR_DC_CURR_DIVISOR = 0x0203, /**< DCCurrentDivisor (Optional) */
    ZCL_ELEC_MEAS_ATTR_DC_PWR_MULTIPLIER = 0x0204, /**< DCPowerMultiplier (Optional) */
    ZCL_ELEC_MEAS_ATTR_DC_PWR_DIVISOR = 0x0205, /**< DCPowerDivisor (Optional) */
    ZCL_ELEC_MEAS_ATTR_AC_FREQ = 0x0300, /**< ACFrequency (Optional) */
    ZCL_ELEC_MEAS_ATTR_AC_FREQ_MIN = 0x0301, /**< ACFrequencyMin (Optional) */
    ZCL_ELEC_MEAS_ATTR_AC_FREQ_MAX = 0x0302, /**< ACFrequencyMax (Optional) */
    ZCL_ELEC_MEAS_ATTR_NEUTRAL_CURR = 0x0303, /**< NeutralCurrent (Optional) */
    ZCL_ELEC_MEAS_ATTR_TOTAL_ACTIVE_PWR = 0x0304, /**< TotalActivePower (Optional) */
    ZCL_ELEC_MEAS_ATTR_TOTAL_REACTIVE_PWR = 0x0305, /**< TotalReactivePower (Optional) */
    ZCL_ELEC_MEAS_ATTR_TOTAL_APPARENT_PWR = 0x0306, /**< TotalApparentPower (Optional) */
    ZCL_ELEC_MEAS_ATTR_1ST_HARM_CURR = 0x0307, /**< Measured1stHarmonicCurrent (Optional) */
    ZCL_ELEC_MEAS_ATTR_3RD_HARM_CURR = 0x0308, /**< Measured3rdHarmonicCurrent (Optional) */
    ZCL_ELEC_MEAS_ATTR_5TH_HARM_CURR = 0x0309, /**< Measured5thHarmonicCurrent (Optional) */
    ZCL_ELEC_MEAS_ATTR_7TH_HARM_CURR = 0x030a, /**< Measured7thHarmonicCurrent (Optional) */
    ZCL_ELEC_MEAS_ATTR_9TH_HARM_CURR = 0x030b, /**< Measured9thHarmonicCurrent (Optional) */
    ZCL_ELEC_MEAS_ATTR_11TH_HARM_CURR = 0x030c, /**< Measured11thHarmonicCurrent (Optional) */
    ZCL_ELEC_MEAS_ATTR_PHASE_1ST_HARM_CURR = 0x030d, /**< MeasuredPhase1stHarmonicCurrent (Optional) */
    ZCL_ELEC_MEAS_ATTR_PHASE_3RD_HARM_CURR = 0x030e, /**< MeasuredPhase3rdHarmonicCurrent (Optional) */
    ZCL_ELEC_MEAS_ATTR_PHASE_5TH_HARM_CURR = 0x030f, /**< MeasuredPhase5thHarmonicCurrent (Optional) */
    ZCL_ELEC_MEAS_ATTR_PHASE_7TH_HARM_CURR = 0x0310, /**< MeasuredPhase7thHarmonicCurrent (Optional) */
    ZCL_ELEC_MEAS_ATTR_PHASE_9TH_HARM_CURR = 0x0311, /**< MeasuredPhase9thHarmonicCurrent (Optional) */
    ZCL_ELEC_MEAS_ATTR_PHASE_11TH_HARM_CURR = 0x0312, /**< MeasuredPhase11thHarmonicCurrent (Optional) */
    ZCL_ELEC_MEAS_ATTR_AC_FREQ_MULTIPLIER = 0x0400, /**< ACFrequencyMultiplier (Optional) */
    ZCL_ELEC_MEAS_ATTR_AC_FREQ_DIVISOR = 0x0401, /**< ACFrequencyDivisor (Optional) */
    ZCL_ELEC_MEAS_ATTR_PWR_MULTIPLIER = 0x0402, /**< PowerMultiplier (Optional) */
    ZCL_ELEC_MEAS_ATTR_PWR_DIVISOR = 0x0403, /**< PowerDivisor (Optional) */
    ZCL_ELEC_MEAS_ATTR_HARM_CURR_MULTIPLIER = 0x0404, /**< HarmonicCurrentMultiplier (Optional) */
    ZCL_ELEC_MEAS_ATTR_PHASE_CURR_MULTIPLIER = 0x0405, /**< PhaseHarmonicCurrentMultiplier (Optional) */
    ZCL_ELEC_MEAS_ATTR_LINE_CURR = 0x0501, /**< LineCurrent (Optional) */
    ZCL_ELEC_MEAS_ATTR_ACTIVE_CURR = 0x0502, /**< ActiveCurrent (Optional) */
    ZCL_ELEC_MEAS_ATTR_REACTIVE_CURR = 0x0503, /**< ReactiveCurrent (Optional) */
    ZCL_ELEC_MEAS_ATTR_RMS_VOLT = 0x0505, /**< RMSVoltage (Optional) */
    ZCL_ELEC_MEAS_ATTR_RMS_VOLT_MIN = 0x0506, /**< RMSVoltageMin (Optional) */
    ZCL_ELEC_MEAS_ATTR_RMS_VOLT_MAX = 0x0507, /**< RMSVoltageMax (Optional) */
    ZCL_ELEC_MEAS_ATTR_RMS_CURR = 0x0508, /**< RMSCurrent (Optional) */
    ZCL_ELEC_MEAS_ATTR_RMS_CURR_MIN = 0x0509, /**< RMSCurrentMin (Optional) */
    ZCL_ELEC_MEAS_ATTR_RMS_CURR_MAX = 0x050a, /**< RMSCurrentMax (Optional) */
    ZCL_ELEC_MEAS_ATTR_ACTIVE_PWR = 0x050b, /**< ActivePower (Optional) */
    ZCL_ELEC_MEAS_ATTR_ACTIVE_PWR_MIN = 0x050c, /**< ActivePowerMin (Optional) */
    ZCL_ELEC_MEAS_ATTR_ACTIVE_PWR_MAX = 0x050d, /**< ActivePowerMax (Optional) */
    ZCL_ELEC_MEAS_ATTR_REACTIVE_PWR = 0x050e, /**< ReactivePower (Optional) */
    ZCL_ELEC_MEAS_ATTR_APPARENT_PWR = 0x050f, /**< ApparentPower (Optional) */
    ZCL_ELEC_MEAS_ATTR_PWR_FACTOR = 0x0510, /**< PowerFactor (Optional) */
    ZCL_ELEC_MEAS_ATTR_AVG_RMS_VOLT_PERIOD = 0x0511, /**< AverageRMSVoltageMeasurementPeriod (Optional) */
    ZCL_ELEC_MEAS_ATTR_AVG_RMS_OV_COUNT = 0x0512, /**< AverageRMSOverVoltageCounter (Optional) */
    ZCL_ELEC_MEAS_ATTR_AVG_RMS_UV_COUNT = 0x0513, /**< AverageRMSUnderVoltageCounter (Optional) */
    ZCL_ELEC_MEAS_ATTR_RMS_EXT_OVER_PERIOD = 0x0514, /**< RMSExtremeOverVoltagePeriod (Optional) */
    ZCL_ELEC_MEAS_ATTR_RMS_EXT_UNDER_PERIOD = 0x0515, /**< RMSExtremeUnderVoltagePeriod (Optional) */
    ZCL_ELEC_MEAS_ATTR_VOLT_SAG_PERIOD = 0x0516, /**< RMSVoltageSagPeriod (Optional) */
    ZCL_ELEC_MEAS_ATTR_VOLT_SWELL_PERIOD = 0x0517, /**< RMSVoltageSwellPeriod (Optional) */
    ZCL_ELEC_MEAS_ATTR_AC_VOLT_MULTIPLIER = 0x0600, /**< ACVoltageMultiplier (Optional) */
    ZCL_ELEC_MEAS_ATTR_AC_VOLT_DIVISOR = 0x0601, /**< ACVoltageDivisor (Optional) */
    ZCL_ELEC_MEAS_ATTR_AC_CURR_MULT = 0x0602, /**< ACCurrentMultiplier (Optional) */
    ZCL_ELEC_MEAS_ATTR_AC_CURR_DIVISOR = 0x0603, /**< ACCurrentDivisor (Optional) */
    ZCL_ELEC_MEAS_ATTR_AC_PWR_MULTIPLIER = 0x0604, /**< ACPowerMultiplier (Optional) */
    ZCL_ELEC_MEAS_ATTR_AC_PWR_DIVISOR = 0x0605, /**< ACPowerDivisor (Optional) */
    ZCL_ELEC_MEAS_ATTR_DC_OL_ALARMS_MASK = 0x0700, /**< DCOverloadAlarmsMask (Optional) */
    ZCL_ELEC_MEAS_ATTR_DC_VOLT_OL = 0x0701, /**< DCVoltageOverload (Optional) */
    ZCL_ELEC_MEAS_ATTR_DC_CURR_OL = 0x0702, /**< DCCurrentOverload (Optional) */
    ZCL_ELEC_MEAS_ATTR_AC_ALARMS_MASK = 0x0800, /**< ACAlarmsMask (Optional) */
    ZCL_ELEC_MEAS_ATTR_AC_VOLT_OL = 0x0801, /**< ACVoltageOverload (Optional) */
    ZCL_ELEC_MEAS_ATTR_AC_CURR_OL = 0x0802, /**< ACCurrentOverload (Optional) */
    ZCL_ELEC_MEAS_ATTR_AC_ACTIVE_PWR_OL = 0x0803, /**< ACActivePowerOverload (Optional) */
    ZCL_ELEC_MEAS_ATTR_AC_REACTIVE_PWR_OL = 0x0804, /**< ACReactivePowerOverload (Optional) */
    ZCL_ELEC_MEAS_ATTR_AVG_RMS_OV = 0x0805, /**< AverageRMSOverVoltage (Optional) */
    ZCL_ELEC_MEAS_ATTR_AVG_RMS_UV = 0x0806, /**< AverageRMSUnderVoltage (Optional) */
    ZCL_ELEC_MEAS_ATTR_RMS_EXT_OV = 0x0807, /**< RMSExtremeOverVoltage (Optional) */
    ZCL_ELEC_MEAS_ATTR_RMS_EXT_UV = 0x0808, /**< RMSExtremeUnderVoltage (Optional) */
    ZCL_ELEC_MEAS_ATTR_RMS_VOLT_SAG = 0x0809, /**< RMSVoltageSag (Optional) */
    ZCL_ELEC_MEAS_ATTR_RMS_VOLT_SWELL = 0x080a, /**< RMSVoltageSwell (Optional) */
    ZCL_ELEC_MEAS_ATTR_LINE_CURR_B = 0x0901, /**< LineCurrentPhB (Optional) */
    ZCL_ELEC_MEAS_ATTR_ACTIVE_CURR_B = 0x0902, /**< ActiveCurrentPhB (Optional) */
    ZCL_ELEC_MEAS_ATTR_REACTIVE_CURR_B = 0x0903, /**< ReactiveCurrentPhB (Optional) */
    ZCL_ELEC_MEAS_ATTR_RMS_VOLT_B = 0x0905, /**< RMSVoltagePhB (Optional) */
    ZCL_ELEC_MEAS_ATTR_RMS_VOLT_MIN_B = 0x0906, /**< RMSVoltageMinPhB (Optional) */
    ZCL_ELEC_MEAS_ATTR_RMS_VOLT_MAX_B = 0x0907, /**< RMSVoltageMaxPhB (Optional) */
    ZCL_ELEC_MEAS_ATTR_RMS_CURR_B = 0x0908, /**< RMSCurrentPhB (Optional) */
    ZCL_ELEC_MEAS_ATTR_RMS_CURR_MIN_B = 0x0909, /**< RMSCurrentMinPhB (Optional) */
    ZCL_ELEC_MEAS_ATTR_RMS_CURR_MAX_B = 0x090a, /**< RMSCurrentMaxPhB (Optional) */
    ZCL_ELEC_MEAS_ATTR_ACTIVE_PWR_B = 0x090b, /**< ActivePowerPhB (Optional) */
    ZCL_ELEC_MEAS_ATTR_ACTIVE_PWR_MIN_B = 0x090c, /**< ActivePowerMinPhB (Optional) */
    ZCL_ELEC_MEAS_ATTR_ACTIVE_PWR_MAX_B = 0x090d, /**< ActivePowerMaxPhB (Optional) */
    ZCL_ELEC_MEAS_ATTR_REACTIVE_PWR_B = 0x090e, /**< ReactivePowerPhB (Optional) */
    ZCL_ELEC_MEAS_ATTR_APPARENT_PWR_B = 0x090f, /**< ApparentPowerPhB (Optional) */
    ZCL_ELEC_MEAS_ATTR_PWR_FACTOR_B = 0x0910, /**< PowerFactorPhB (Optional) */
    ZCL_ELEC_MEAS_ATTR_AVG_RMS_VOLT_PERIOD_B = 0x0911, /**< AverageRMSVoltageMeasurementPeriodPhB (Optional) */
    ZCL_ELEC_MEAS_ATTR_AVG_RMS_OV_B = 0x0912, /**< AverageRMSOverVoltageCounterPhB (Optional) */
    ZCL_ELEC_MEAS_ATTR_AVG_RMS_UV_B = 0x0913, /**< AverageRMSUnderVoltageCounterPhB (Optional) */
    ZCL_ELEC_MEAS_ATTR_RMS_EXT_OVER_B = 0x0914, /**< RMSExtremeOverVoltagePeriodPhB (Optional) */
    ZCL_ELEC_MEAS_ATTR_RMS_EXT_UNDER_B = 0x0915, /**< RMSExtremeUnderVoltagePeriodPhB (Optional) */
    ZCL_ELEC_MEAS_ATTR_RMS_SAG_PERIOD_B = 0x0916, /**< RMSVoltageSagPeriodPhB (Optional) */
    ZCL_ELEC_MEAS_ATTR_RMS_SWELL_PERIOD_B = 0x0917, /**< RMSVoltageSwellPeriodPhB (Optional) */
    ZCL_ELEC_MEAS_ATTR_LINE_CURR_C = 0x0a01, /**< LineCurrentPhC (Optional) */
    ZCL_ELEC_MEAS_ATTR_ACTIVE_CURR_C = 0x0a02, /**< ActiveCurrentPhC (Optional) */
    ZCL_ELEC_MEAS_ATTR_REACTIVE_CURR_C = 0x0a03, /**< ReactiveCurrentPhC (Optional) */
    ZCL_ELEC_MEAS_ATTR_RMS_VOLT_C = 0x0a05, /**< RMSVoltagePhC (Optional) */
    ZCL_ELEC_MEAS_ATTR_RMS_VOLT_MIN_C = 0x0a06, /**< RMSVoltageMinPhC (Optional) */
    ZCL_ELEC_MEAS_ATTR_RMS_VOLT_MAX_C = 0x0a07, /**< RMSVoltageMaxPhC (Optional) */
    ZCL_ELEC_MEAS_ATTR_RMS_CURR_C = 0x0a08, /**< RMSCurrentPhC (Optional) */
    ZCL_ELEC_MEAS_ATTR_RMS_CURR_MIN_C = 0x0a09, /**< RMSCurrentMinPhC (Optional) */
    ZCL_ELEC_MEAS_ATTR_RMS_CURR_MAX_C = 0x0a0a, /**< RMSCurrentMaxPhC (Optional) */
    ZCL_ELEC_MEAS_ATTR_ACTIVE_PWR_C = 0x0a0b, /**< ActivePowerPhC (Optional) */
    ZCL_ELEC_MEAS_ATTR_ACTIVE_PWR_MIN_C = 0x0a0c, /**< ActivePowerMinPhC (Optional) */
    ZCL_ELEC_MEAS_ATTR_ACTIVE_PWR_MAX_C = 0x0a0d, /**< ActivePowerMaxPhC (Optional) */
    ZCL_ELEC_MEAS_ATTR_REACTIVE_PWR_C = 0x0a0e, /**< ReactivePowerPhC (Optional) */
    ZCL_ELEC_MEAS_ATTR_APPARENT_PWR_C = 0x0a0f, /**< ApparentPowerPhC (Optional) */
    ZCL_ELEC_MEAS_ATTR_PWR_FACTOR_C = 0x0a10, /**< PowerFactorPhC (Optional) */
    ZCL_ELEC_MEAS_ATTR_AVG_RMS_VOLT_PERIOD_C = 0x0a11, /**< AverageRMSVoltageMeasurementPeriodPhC (Optional) */
    ZCL_ELEC_MEAS_ATTR_AVG_RMS_OV_C = 0x0a12, /**< AverageRMSOverVoltageCounterPhC (Optional) */
    ZCL_ELEC_MEAS_ATTR_AVG_RMS_UV_C = 0x0a13, /**< AverageRMSUnderVoltageCounterPhC (Optional) */
    ZCL_ELEC_MEAS_ATTR_RMS_EXT_OVER_C = 0x0a14, /**< RMSExtremeOverVoltagePeriodPhC (Optional) */
    ZCL_ELEC_MEAS_ATTR_RMS_EXT_UNDER_C = 0x0a15, /**< RMSExtremeUnderVoltagePeriodPhC (Optional) */
    ZCL_ELEC_MEAS_ATTR_RMS_SAG_PERIOD_C = 0x0a16, /**< RMSVoltageSagPeriodPhC (Optional) */
    ZCL_ELEC_MEAS_ATTR_RMS_SWELL_PERIOD_C = 0x0a17, /**< RMSVoltageSwellPeriodPhC (Optional) */
};

/* Default Values */
#define ZCL_ELEC_MEAS_DEFAULT_MEAS_TYPE         0x00000000
#define ZCL_ELEC_MEAS_DEFAULT_DC_MEAS           ZCL_INVALID_SIGNED_16BIT
#define ZCL_ELEC_MEAS_DEFAULT_DC_FORMATTING     0x0001
#define ZCL_ELEC_MEAS_DEFAULT_DC_OL_ALARMS_MASK 0x00
#define ZCL_ELEC_MEAS_DEFAULT_DC_VOLT_OL        0xffff

/* Min and Max Values */
#define ZCL_ELEC_MEAS_MIN_DC_MEAS               (-32767)
#define ZCL_ELEC_MEAS_MAX_DC_MEAS               32767
#define ZCL_ELEC_MEAS_MIN_DC_FORMATTING         0x0001
#define ZCL_ELEC_MEAS_MAX_DC_FORMATTING         0xffff
#define ZCL_ELEC_MEAS_MIN_OL_ALARMS_MASK        0x00
#define ZCL_ELEC_MEAS_MAX_OL_ALARMS_MASK        0x03

/* Electrical Measurement Client Generated Commands */
enum {
    ZCL_ELEC_MEAS_CLI_GET_PROFILE_INFO = 0x00, /* Get Profile Info Command (Optional) */
    ZCL_ELEC_MEAS_CLI_GET_MEAS_PROFILE = 0x01 /* Get Measurement Profile Command (Optional) */
};

/* Electrical Measurement Server Generated Commands */
enum {
    ZCL_ELEC_MEAS_SVR_GET_PROFILE_INFO_RSP = 0x00, /* Get Profile Info Response Command (Optional */
    ZCL_ELEC_MEAS_SVR_GET_MEAS_PROFILE_RSP = 0x01 /* Get Measurement Profile Response Command (Optional) */
};

/** Get Measurement Profile command structure */
struct ZbZclElecMeasClientGetMeasProfileReqT {
    uint16_t attr_id; /**< Attribute ID */
    uint32_t start_time; /**< Start Time */
    uint8_t num_intervals; /**< NumberOfIntervals */
};

/** Get Profile Info response structure */
struct ZbZclElecMeasSvrGetProfileInfoRspT {
    /*
     *  Profile count is the total number for supported profile, and the list of
     * attributes is the list of attributes being profiled. This relationship has
     * been interpreted as profile count holding the number of attributes in
     * attribute list.
     */
    uint8_t profile_count; /**< Profile Count */
    uint8_t profile_interval_period; /**< ProfileIntervalPeriod */
    uint8_t max_num_intervals; /**< MaxNumberOfIntervals */
    uint16_t *attr_list; /**< ListOfAttributes */
};

/** Get Measurement Profile response structure */
struct ZbZclElecMeasSvrGetMeasProfileRspT {
    uint32_t start_time; /**< StartTime */
    uint8_t status; /**< Status */
    uint8_t profile_interval_period; /**< ProfileIntervalPeriod */
    uint8_t num_intervals_delivered; /**< NumberOfIntervalsDelivered */
    uint16_t attr_id; /**< Attribute ID */
    /*
     *  Pointer to buffer of interval attribute data in the format as sent over the air
     * (endian conversion already done). If an interval is invalid, it should be set to
     * all f's (e.g. 0xffff for an int16 type).
     */
    uint8_t *interval_data; /**< Intervals */
    uint16_t interval_len; /**< Number of Intervals */
};

/** Electrical Measurement Server callbacks configuration */
struct ZbZclElecMeasSvrCallbacksT {
    enum ZclStatusCodeT (*get_profile_info)(struct ZbZclClusterT *clusterPtr, struct ZbZclAddrInfoT *src_info, void *arg);
    /**< Callback to application, invoked on receipt of Get Profile Info command. */
    enum ZclStatusCodeT (*get_meas_profile)(struct ZbZclClusterT *clusterPtr, struct ZbZclElecMeasClientGetMeasProfileReqT *cmd_req,
        struct ZbZclAddrInfoT *src_info, void *arg);
    /**< Callback to application, invoked on receipt of Get Measurement Profile command. */
};

/**
 * Create a new instance of the Electrical Measurement Client cluster
 * @param zb Zigbee stack instance
 * @param endpoint Endpoint on which to create cluster
 * @return Cluster pointer, or NULL if there is an error
 */
struct ZbZclClusterT * ZbZclElecMeasClientAlloc(struct ZigBeeT *zb, uint8_t endpoint);

/**
 * Create a new instance of the Electrical Measurement Server cluster
 * @param zb Zigbee stack instance
 * @param endpoint Endpoint on which to create cluster
 * @param callbacks Structure containing any callback function pointers for this cluster
 * @param arg Pointer to application data that will later be provided back to the callback functions when invoked
 * @return Cluster pointer, or NULL if there is an error
 */
struct ZbZclClusterT * ZbZclElecMeasServerAlloc(struct ZigBeeT *zb, uint8_t endpoint,
    struct ZbZclElecMeasSvrCallbacksT *callbacks, void *arg);

/**
 * Send a Get Profile Info command
 * @param cluster Cluster instance from which to send this command
 * @param dst Destination address for request
 * @param callback Callback function that will be invoked later when the response is received
 * @param arg Pointer to application data that will later be provided back to the callback function when invoked
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT ZbZclElecMeasClientGetProfileInfoReq(struct ZbZclClusterT *cluster, const struct ZbApsAddrT *dst,
    void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

/**
 * Send a Get Measurement Profile command
 * @param cluster Cluster instance from which to send this command
 * @param dst Destination address for request
 * @param req Get Measurement Profile command structure
 * @param callback Callback function that will be invoked later when the response is received
 * @param arg Pointer to application data that will later be provided back to the callback function when invoked
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT ZbZclElecMeasClientGetMeasProfileReq(struct ZbZclClusterT *cluster, const struct ZbApsAddrT *dst,
    struct ZbZclElecMeasClientGetMeasProfileReqT *req,
    void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

/**
 * Send a Get Profile Info response
 * @param cluster Cluster instance from which to send this command
 * @param dst Destination address for request
 * @param rsp Get Profile Info response structure
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT ZbZclElecMeasServerSendProfileInfoRsp(struct ZbZclClusterT *cluster, struct ZbZclAddrInfoT *dst,
    struct ZbZclElecMeasSvrGetProfileInfoRspT *rsp);

/**
 * Send a Get Measurement Profile response
 * @param cluster Cluster instance from which to send this command
 * @param dst Destination address for request
 * @param rsp Get Measurement Profile response structure
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT ZbZclElecMeasServerSendMeasProfileRsp(struct ZbZclClusterT *cluster, struct ZbZclAddrInfoT *dst,
    struct ZbZclElecMeasSvrGetMeasProfileRspT *rsp);

#endif
