/**
 * @file zcl.meter.h
 * @heading Metering
 * @brief ZCL Metering cluster header
 * ZCL 7 section 10.4
 * ZCL 8 section 10.4
 * @copyright Copyright [2009 - 2021] Exegin Technologies Limited. All rights reserved.
 */

/* EXEGIN - removed '@'PICS escape sequence, since these are not the
 * official PICS codes. These were added with the assumption that
 * SE clusters are being brought into the Zigbee 3.0 fold and will
 * use the same PICS codes and PICS format style. However, this has
 * yet to occur.
 * E.g. SE 1.4 defines the PICS for the Metering Server Cluster as
 * MECS1. The ZCL way would be to define it as SEMT.S. */

/* ZCL.Meter
 * SEMT.S | Server | True
 * SEMT.C | Client | True
 *
 * Server Attributes
 * SEMT.S.A0000 | CurrentSummationDelivered | True
 * SEMT.S.A0001 | CurrentSummationReceived | False | Optional
 * SEMT.S.A0002 | CurrentMaxDemandDelivered | False | Optional
 * SEMT.S.A0003 | CurrentMaxDemandReceived | False | Optional
 * SEMT.S.A0004 | DFTSummation | False | Optional
 * SEMT.S.A0005 | DailyFreezeTime | False | Optional
 * SEMT.S.A0006 | PowerFactor | False| Optional
 * SEMT.S.A0007 | ReadingSnapShotTime | False | Optional
 * SEMT.S.A0008 | CurrentMaxDemandDeliveredTime | False | Optional
 * SEMT.S.A0009 | CurrentMaxDemandReceivedTime | False | Optional
 * SEMT.S.A000a | DefaultUpdatePeriod | False | Optional
 * SEMT.S.A000b | FastPollUpdatePeriod | False | Optional
 * SEMT.S.A000c | CurrentBlockPeriodConsumptionDelivered | False | Optional
 * SEMT.S.A000d | DailyConsumptionTarget | False | Optional
 * SEMT.S.A000e | CurrentBlock | False | Optional
 * SEMT.S.A000f | ProfileIntervalPeriod | False | Optional
 * SEMT.S.A0010 | Deprecated | False | Optional
 * SEMT.S.A0011 | PresetReadingTime | False | Optional
 * SEMT.S.A0012 | SummationDeliveredPerReport | False | Optional
 * SEMT.S.A0013 | FlowRestriction | False | Optional
 * SEMT.S.A0014 | Supply Status | False | Optional
 * SEMT.S.A0015 | CurrentInletEnergyCarrierSummation | False | Optional
 * SEMT.S.A0016 | CurrentOutletEnergyCarrierSummation | False | Optional
 * SEMT.S.A0017 | InletTemperature | False | Optional
 * SEMT.S.A0018 | OutletTemperature | False | Optional
 * SEMT.S.A0019 | ControlTemperature | False | Optional
 * SEMT.S.A001a | CurrentInletEnergyCarrierDemand | False | Optional
 * SEMT.S.A001b | CurrentOutletEnergyCarrierDemand | False | Optional
 * SEMT.S.A001c | PreviousBlockPeriodConsumptionDelivered | False | Optional
 * SEMT.S.A001d | CurrentBlockPeriod ConsumptionReceived | False | Optional
 * SEMT.S.A001e | CurrentBlockReceived | False | Optional
 * SEMT.S.A001f | DFTSummation Received | False | Optional
 * SEMT.S.A0020 | ActiveRegisterTier Delivered | False | Optional
 * SEMT.S.A0021 | ActiveRegisterTier Received | False | Optional
 * SEMT.S.A0022 | LastBlockSwitchTime | False | Optional
 * SEMT.S.A0100 | CurrentTier(1-48)SummationDelivered | False | Optional
 * SEMT.S.A0101 | CurrentTier(1-48)SummationReceived | False | Optional
 * SEMT.S.A01fc | CPP1SummationDelivered | False | Optional
 * SEMT.S.A01fe | CPP2SummationDelivered | False | Optional
 * SEMT.S.A0200 | Status | True
 * SEMT.S.A0201 | RemainingBatteryLife | False | Optional
 * SEMT.S.A0202 | HoursInOperation | False | Optional
 * SEMT.S.A0203 | HoursInFault | False | Optional
 * SEMT.S.A0204 | Extended Status | False | Optional
 * SEMT.S.A0205 | Remaining BatteryLife in Days | False | Optional
 * SEMT.S.A0206 | CurrentMeterID | False | Optional
 * SEMT.S.A0207 | AmbientConsumptionIndicator | False | Optional
 * SEMT.S.A0300 | UnitofMeasure | True
 * SEMT.S.A0301 | Multiplier | False| Optional
 * SEMT.S.A0302 | Divisor | False | Optional
 * SEMT.S.A0303 | SummationFormatting | True | Optional
 * SEMT.S.A0304 | DemandFormatting | False | Optional
 * SEMT.S.A0305 | HistoricalConsumptionFormatting | False | Optional
 * SEMT.S.A0306 | MeteringDeviceType | True
 * SEMT.S.A0307 | SiteID | False | Optional
 * SEMT.S.A0308 | MeterSerialNumber | False | Optional
 * SEMT.S.A0309 | EnergyCarrierUnitOfMeasure | False | Optional
 * SEMT.S.A030a | EnergyCarrierSummationFormatting | False | Optional
 * SEMT.S.A030b | EnergyCarrierDemandFormatting | False | Optional
 * SEMT.S.A030c | TemperatureUnitOfMeasure | False | Optional
 * SEMT.S.A030d | TemperatureFormatting | False | Optional
 * SEMT.S.A030e | ModuleSerialNumber | False | Optional
 * SEMT.S.A030f | OperatingTariffLabel Delivered | False | Optional
 * SEMT.S.A0310 | OperatingTariffLabel Received | False | Optional
 * SEMT.S.A0311 | CustomerIDNumber | False | Optional
 * SEMT.S.A0312 | AlternativeUnitof Measure | False | Optional
 * SEMT.S.A0313 | AlternativeDemandFormatting | False | Optional
 * SEMT.S.A0314 | AlternativeDemandFormatting | False | Optional
 * SEMT.S.A0400 | InstantaneousDemand | False | Optional
 * SEMT.S.A0401 | CurrentDayConsumptionDelivered | False | Optional
 * SEMT.S.A0402 | CurrentDayConsumptionReceived | False | Optional
 * SEMT.S.A0403 | PreviousDayConsumptionDelivered | False | Optional
 * SEMT.S.A0404 | PreviousDayConsumptionReceived | False | Optional
 * SEMT.S.A0405 | CurrentPartialProfileIntervalStartTimeDelivered | False | Optional
 * SEMT.S.A0406 | CurrentPartialProfileIntervalStartTimeReceived | False | Optional
 * SEMT.S.A0407 | CurrentPartialProfileIntervalValueDelivered | False | Optional
 * SEMT.S.A0408 | CurrentPartialProfileIntervalValueReceived | False | Optional
 * SEMT.S.A0409 | CurrentDayMaxPressure | False | Optional
 * SEMT.S.A040a | CurrentDayMinPressure | False | Optional
 * SEMT.S.A040b | PreviousDayMaxPressure | False | Optional
 * SEMT.S.A040c | PreviousDayMinPressure | False | Optional
 * SEMT.S.A040d | CurrentDayMaxDemand | False | Optional
 * SEMT.S.A040e | PreviousDayMaxDemand | False | Optional
 * SEMT.S.A040f | CurrentMonthMaxDemand | False | Optional
 * SEMT.S.A0410 | CurrentYearMaxDemand | False | Optional
 * SEMT.S.A0411 | CurrentDayMaxEnergyCarrierDemand | False | Optional
 * SEMT.S.A0412 | PreviousDayMaxEnergyCarrierDemand | False | Optional
 * SEMT.S.A0413 | CurrentMonthMaxEnergyCarrierDemand | False | Optional
 * SEMT.S.A0414 | CurrentMonthMinEnergyCarrierDemand | False | Optional
 * SEMT.S.A0415 | CurrentYearMaxEnergyCarrierDemand | False | Optional
 * SEMT.S.A0416 | CurrentYearMinEnergyCarrierDemand | False | Optional
 * SEMT.S.A0420 | PreviousDay(2-8)ConsumptionDelivered | False | Optional
 * SEMT.S.A0421 | PreviousDay(2-8)ConsumptionReceived | False | Optional
 * SEMT.S.A0430 | CurrentWeekConsumptionDelivered | False | Optional
 * SEMT.S.A0431 | CurrentWeekConsumptionReceived | False | Optional
 * SEMT.S.A0432 | PreviousWeekConsumptionDelivered | False | Optional
 * SEMT.S.A0433 | PreviousWeekConsumptionReceived | False | Optional
 * SEMT.S.A0434 | PreviousWeek(2-5)ConsumptionDelivered | False | Optional
 * SEMT.S.A0435 | PreviousWeek(2-5)ConsumptionReceived | False | Optional
 * SEMT.S.A0440 | CurrentMonthConsumptionDelivered | False | Optional
 * SEMT.S.A0441 | CurrentMonthConsumptionReceived | False | Optional
 * SEMT.S.A0442 | PreviousMonthConsumptionDelivered | False | Optional
 * SEMT.S.A0443 | PreviousMonthConsumptionReceived | False | Optional
 * SEMT.S.A0444 | PreviousMonth(2-13)ConsumptionDelivered | False | Optional
 * SEMT.S.A0445 | PreviousMonth(2-13)ConsumptionReceived | False | Optional
 * SEMT.S.A045c | Historical Freeze Time | False | Optional
 * SEMT.S.A0500 | MaxNumberOfPeriodsDelivered | False | Optional
 * SEMT.S.A0600 | CurrentDemandDelivered | False | Optional
 * SEMT.S.A0601 | DemandLimit | False | Optional
 * SEMT.S.A0602 | DemandIntegrationPeriod | False | Optional
 * SEMT.S.A0603 | NumberOfDemandSubintervals | False | Optional
 * SEMT.S.A0604 | DemandLimitArmDuration | False | Optional
 * SEMT.S.A0605 | LoadLimitSupplyState | False | Optional
 * SEMT.S.A0606 | LoadLimitCounter | False | Optional
 * SEMT.S.A0607 | SupplyTamperState | False | Optional
 * SEMT.S.A0608 | SupplyDepletionState | False | Optional
 * SEMT.S.A0609 | SupplyUncontrolled FlowState | False | Optional
 * SEMT.S.A0700 | CurrentNoTierBlock(1-16)SummationDelivered | False | Optional
 * SEMT.S.A0710 | CurrentTier(1-15)Block(1-16)SummationDelivered | False | Optional
 * SEMT.S.A0800 | Generic Alarm Mask | False | Optional
 * SEMT.S.A0801 | Electricity Alarm Mask | False | Optional
 * SEMT.S.A0802 | Generic Flow/Pressure Alarm Mask | False | Optional
 * SEMT.S.A0803 | Water Specific Alarm Mask | False |Optional
 * SEMT.S.A0804 | Heat and Cooling Specific Alarm Mask | False | Optional
 * SEMT.S.A0805 | Gas Specific AlarmMask | False | Optional
 * SEMT.S.A0806 | ExtendedGenericAlarmMask | False | Optional
 * SEMT.S.A0807 | ManufacturerAlarmMask | False | Optional
 * SEMT.S.A0900 | CurrentNoTierBlock(1-16)SummationReceived | False | Optional
 * SEMT.S.A0910 | CurrentTier(1-15)Block(1-16)SummationReceived | False | Optional
 * SEMT.S.A0a00 | BillToDateDelivered | False | Optional
 * SEMT.S.A0a01 | BillToDateTimeStampDelivered | False | Optional
 * SEMT.S.A0a02 | ProjectedBillDelivered | False | Optional
 * SEMT.S.A0a03 | ProjectedBillTimeStampDelivered | False | Optional
 * SEMT.S.A0a04 | BillDeliveredTrailingDigit | False | Optional
 * SEMT.S.A0a10 | BillToDateReceived | False | Optional
 * SEMT.S.A0a11 | BillToDateTimeStampReceived | False | Optional
 * SEMT.S.A0a12 | ProjectedBillReceived | False | Optional
 * SEMT.S.A0a13 | ProjectedBillTimeStampReceived | False | Optional
 * SEMT.S.A0a14 | BillReceivedTrailingDigit | False | Optional
 * SEMT.S.A0b00 | ProposedChangeSupply ImplementationTime | False | Optional
 * SEMT.S.A0b01 | ProposedChange SupplyStatus | False | Optional
 * SEMT.S.A0b10 | Uncontrolled Flow Threshold | False | Optional
 * SEMT.S.A0b11 | Uncontrolled Flow Threshold Unit of Measure | False | Optional
 * SEMT.S.A0b12 | Uncontrolled Flow Multiplier | False | Optional
 * SEMT.S.A0b13 | Uncontrolled Flow Divisor | False | Optional
 * SEMT.S.A0b14 | Flow Stabilisation Period | False | Optional
 * SEMT.S.A0b15 | Flow Measurement Period | False | Optional
 * SEMT.S.A0c00 | AlternativeInstantaneousDemand | False | Optional
 * SEMT.S.Afffd | ClusterRevision | True
 * SEMT.S.Afffe | AttributeReportingStatus | True
 *
 * Commands Received
 * SEMT.S.C00.Rsp | Get Profile | False
 * SEMT.S.C01.Rsp | Request Mirror Response | False
 * SEMT.S.C02.Rsp | Mirror Removed | False
 * SEMT.S.C03.Rsp | Request Fast Poll Mode | False
 * SEMT.S.C04.Rsp | ScheduleSnapshot | False
 * SEMT.S.C05.Rsp | TakeSnapshot | False
 * SEMT.S.C06.Rsp | GetSnapshot | False
 * SEMT.S.C07.Rsp | StartSampling | False
 * SEMT.S.C08.Rsp | GetSampledData | False
 * SEMT.S.C09.Rsp | MirrorReport AttributeResponse | False
 * SEMT.S.C0a.Rsp | ResetLoadLimit Counter | False
 * SEMT.S.C0b.Rsp | Change Supply | False
 * SEMT.S.C0c.Rsp | Local Change Supply | False
 * SEMT.S.C0d.Rsp | SetSupplyStatus | False
 * SEMT.S.C0e.Rsp | SetUncontrolledFlowThreshold | False
 *
 * Commands Generated
 * SEMT.S.C00.Tx | Get Profile Response | True
 * SEMT.S.C01.Tx | Request Mirror | False
 * SEMT.S.C02.Tx | Remove Mirror | False
 * SEMT.S.C03.Tx | Request Fast Poll Mode Response | False
 * SEMT.S.C04.Tx | ScheduleSnapshot Response | False
 * SEMT.S.C05.Tx | TakeSnapshotResponse | False
 * SEMT.S.C06.Tx | Publish Snapshot | False
 * SEMT.S.C07.Tx | GetSampledData Response | True
 * SEMT.S.C08.Tx | ConfigureMirror | False
 * SEMT.S.C09.Tx | ConfigureNotification Scheme | False
 * SEMT.S.C0a.Tx | ConfigureNotification Flag | False
 * SEMT.S.C0b.Tx | GetNotifiedMessage | False
 * SEMT.S.C0c.Tx | Supply Status Response | False
 * SEMT.S.C0d.Tx | StartSamplingResponse | False
 *
 * Client Attributes
 * SEMT.C.A0000 | FunctionalNotificationFlags | False | Optional
 * SEMT.C.A0001 | NotificationFlags2 | False | Optional
 * SEMT.C.A0002 | NotificationFlags3 | False | Optional
 * SEMT.C.A0003 | NotificationFlags4 | False | Optional
 * SEMT.C.A0004 | NotificationFlags5 | False | Optional
 * SEMT.C.A0005 | NotificationFlags6 | False | Optional
 * SEMT.C.A0006 | NotificationFlags7 | False | Optional
 * SEMT.C.A0007 | NotificationFlags8 | False | Optional
 * SEMT.C.Afffd | ClusterRevision | True
 * SEMT.C.Afffe | AttributeReportingStatus | True
 *
 * Commands Received
 * SEMT.C.C00.Rsp | Get Profile Response | False
 * SEMT.C.C01.Rsp | Request Mirror | False
 * SEMT.C.C02.Rsp | Remove Mirror | False
 * SEMT.C.C03.Rsp | Request Fast Poll Mode Response | False
 * SEMT.C.C04.Rsp | ScheduleSnapshot Response | False
 * SEMT.C.C05.Rsp | TakeSnapshotResponse | False
 * SEMT.C.C06.Rsp | Publish Snapshot | False
 * SEMT.C.C07.Rsp | GetSampledData Response | False
 * SEMT.C.C08.Rsp | ConfigureMirror | False
 * SEMT.C.C09.Rsp | ConfigureNotification Scheme | False
 * SEMT.C.C0a.Rsp | ConfigureNotification Flag | False
 * SEMT.C.C0b.Rsp | GetNotifiedMessage | False
 * SEMT.C.C0c.Rsp | Supply Status Response | False
 * SEMT.C.C0d.Rsp | StartSamplingResponse | False
 *
 * Commands Generated
 * SEMT.C.C00.Tx | Get Profile | True
 * SEMT.C.C01.Tx | Request Mirror Response | False
 * SEMT.C.C02.Tx | Mirror Removed | False
 * SEMT.C.C03.Tx | Request Fast Poll Mode | False
 * SEMT.C.C04.Tx | ScheduleSnapshot | False
 * SEMT.C.C05.Tx | TakeSnapshot | False
 * SEMT.C.C06.Tx | GetSnapshot | False
 * SEMT.C.C07.Tx | StartSampling | False
 * SEMT.C.C08.Tx | GetSampledData | True
 * SEMT.C.C09.Tx | MirrorReport AttributeResponse | False
 * SEMT.C.C0a.Tx | ResetLoadLimit Counter | False
 * SEMT.C.C0b.Tx | Change Supply | False
 * SEMT.C.C0c.Tx | Local Change Supply | True
 * SEMT.C.C0d.Tx | SetSupplyStatus | False
 * SEMT.C.C0e.Tx | SetUncontrolledFlowThreshold | False
 */

#ifndef ZCL_METER_H
# define ZCL_METER_H

#include "zcl/zcl.h"

/*lint -save -e621 [ identifier clash within 31 chars <Rule 5.1, REQUIRED> ] */

/** Metering Server Attribute IDs */
enum ZbZclMeteringSvrAttrT {
    /* Reading Information Set (0x00) */
    ZCL_METER_SVR_ATTR_CURSUM_DELIV = 0x0000, /**< CurrentSummationDelivered */
    ZCL_METER_SVR_ATTR_CURSUM_RECV = 0x0001, /**< CurrentSummationReceived (Optional) */
    ZCL_METER_SVR_ATTR_MAX_DMND_DELIV = 0x0002, /**< CurrentMaxDemandDelivered (Optional) */
    ZCL_METER_SVR_ATTR_MAX_DMND_RECV = 0x0003, /**< CurrentMaxDemandReceived (Optional) */
    ZCL_METER_SVR_ATTR_DFT_SUM = 0x0004, /**< DFTSummation (Optional) */
    ZCL_METER_SVR_ATTR_DAILY_FREEZE_TIME = 0x0005, /**< DailyFreezeTime (Optional) */
    ZCL_METER_SVR_ATTR_POWER_FACTOR = 0x0006, /**< PowerFactor (Optional) */
    ZCL_METER_SVR_ATTR_READ_SNAPSHOT_TIME = 0x0007, /**< ReadingSnapShotTime (Optional) */
    ZCL_METER_SVR_ATTR_MAX_DMND_DELIV_TIME = 0x0008, /**< CurrentMaxDemandDeliveredTime (Optional) */
    ZCL_METER_SVR_ATTR_MAX_DMND_RECV_TIME = 0x0009, /**< CurrentMaxDemandReceivedTime (Optional) */
    ZCL_METER_SVR_ATTR_DEFAULT_UPDATE_PERIOD = 0x000a, /**< DefaultUpdatePeriod (Optional) */
    ZCL_METER_SVR_ATTR_FAST_POLL_UPDATE_PERIOD = 0x000b, /**< FastPollUpdatePeriod (Optional) */
    ZCL_METER_SVR_ATTR_BLOCK_CONSUMP_DELIV = 0x000c, /**< CurrentBlockPeriodConsumptionDelivered (Optional) */
    ZCL_METER_SVR_ATTR_DAILY_CONSUMPTION_TARGET = 0x000d, /**< DailyConsumptionTarget (Optional) */
    ZCL_METER_SVR_ATTR_CURBLOCK = 0x000e, /**< CurrentBlock (Optional) */
    ZCL_METER_SVR_ATTR_PROFILE_INTERVAL_PERIOD = 0x000f, /**< ProfileIntervalPeriod (Optional) */
    ZCL_METER_SVR_ATTR_INTERVAL_REPORTING_PERIOD = 0x0010, /**< (Deprecated) (Optional) */
    ZCL_METER_SVR_ATTR_PRESET_READING_TIME = 0x0011, /**< PresetReadingTime (Optional) */
    ZCL_METER_SVR_ATTR_VOLUME_PER_REPORT = 0x0012, /**< SummationDeliveredPerReport (Optional) */
    ZCL_METER_SVR_ATTR_FLOW_RESTRICTION = 0x0013, /**< FlowRestriction (Optional) */
    ZCL_METER_SVR_ATTR_SUPPLY_STATUS = 0x0014, /**< Supply Status (Optional) */
    ZCL_METER_SVR_ATTR_INLET_EC_SUMMATION = 0x0015, /**< CurrentInletEnergyCarrierSummation (Optional) */
    ZCL_METER_SVR_ATTR_OUTLET_EC_SUMMATION = 0x0016, /**< CurrentOutletEnergyCarrierSummation (Optional) */
    ZCL_METER_SVR_ATTR_INLET_TEMPERATURE = 0x0017, /**< InletTemperature (Optional) */
    ZCL_METER_SVR_ATTR_OUTLET_TEMPERATURE = 0x0018, /**< OutletTemperature (Optional) */
    ZCL_METER_SVR_ATTR_CONTROL_TEMPERATURE = 0x0019, /**< ControlTemperature (Optional) */
    ZCL_METER_SVR_ATTR_INLET_EC_DEMAND = 0x001a, /**< CurrentInletEnergyCarrierDemand (Optional) */
    ZCL_METER_SVR_ATTR_OUTLET_EC_DEMAND = 0x001b, /**< CurrentOutletEnergyCarrierDemand (Optional) */
    ZCL_METER_SVR_ATTR_PREVIOUS_BLOCK_DELIV = 0x001c, /**< PreviousBlockPeriodConsumptionDelivered (Optional) */
    ZCL_METER_SVR_ATTR_CURBLOCK_CONSUMPTION_RECIEVED = 0x001d, /**< CurrentBlockPeriod ConsumptionReceived (Optional) */
    ZCL_METER_SVR_ATTR_CURBLOCK_RECV = 0x001e, /**< CurrentBlockReceived (Optional) */
    ZCL_METER_SVR_ATTR_DFT_SUM_RECV = 0x001f, /**< DFTSummation Received (Optional) */
    ZCL_METER_SVR_ATTR_ACTREGTIER_DELIV = 0x0020, /**< ActiveRegisterTier Delivered (Optional) */
    ZCL_METER_SVR_ATTR_ACTREGTIER_RECV = 0x0021, /**< ActiveRegisterTier Received (Optional) */
    ZCL_METER_SVR_ATTR_LAST_BLOCK_SWITCH_TIME = 0x0022, /**< LastBlockSwitchTime (Optional) */

    /* Summation TOU Information Set (0x01) */
    ZCL_METER_SVR_ATTR_TIER1_DELIV = 0x0100,
    /**< CurrentTier1SummationDelivered (Optional)
     * For all the tiers, use the ZCL_METER_SVR_ATTR_TIERN_DELIV(tier) macro. */
    ZCL_METER_SVR_ATTR_TIER1_RECV = 0x0101,
    /**< CurrentTier1SummationReceived (Optional)
     * For all the tiers, use the ZCL_METER_SVR_ATTR_TIERN_RECV(tier) macro. */

    /* Meter Status Set (0x02) */
    ZCL_METER_SVR_ATTR_METER_STATUS = 0x0200, /**< Status */
    ZCL_METER_SVR_ATTR_RMN_BATT_LIFE = 0x0201, /**< RemainingBatteryLife (Optional) */
    ZCL_METER_SVR_ATTR_HOURS_IN_OPERATION = 0x0202, /**< HoursInOperation (Optional) */
    ZCL_METER_SVR_ATTR_HOURS_IN_FAULT = 0x0203, /**< HoursInFault (Optional) */
    ZCL_METER_SVR_ATTR_EXTENDED_STATUS = 0x0204, /**< Extended Status (Optional) */
    ZCL_METER_SVR_ATTR_RMN_BATT_DAYS = 0x0205, /**< Remaining BatteryLife in Days (Optional) */
    ZCL_METER_SVR_ATTR_CURMETER_ID = 0x0206, /**< CurrentMeterID (Optional) */
    ZCL_METER_SVR_ATTR_AMBIENT_CONSUMPTION_INDICATOR = 0x0207, /**< AmbientConsumptionIndicator (Optional) */

    /* Formatting Set (0x03) */
    ZCL_METER_SVR_ATTR_UNIT_OF_MEASURE = 0x0300, /**< UnitofMeasure */
    ZCL_METER_SVR_ATTR_MULTIPLIER = 0x0301, /**< Multiplier (Optional) */
    ZCL_METER_SVR_ATTR_DIVISOR = 0x0302, /**< Divisor (Optional) */
    ZCL_METER_SVR_ATTR_SUMMATION_FORMAT = 0x0303, /**< SummationFormatting */
    ZCL_METER_SVR_ATTR_DEMAND_FORMAT = 0x0304, /**< DemandFormatting (Optional) */
    ZCL_METER_SVR_ATTR_HISTORICAL_CONSUMPTION_FORMAT = 0x0305, /**< HistoricalConsumptionFormatting (Optional) */
    ZCL_METER_SVR_ATTR_DEVICE_TYPE = 0x0306, /**< MeteringDeviceType */
    ZCL_METER_SVR_ATTR_SITE_ID = 0x0307, /**< SiteID (Optional) */
    ZCL_METER_SVR_ATTR_SERIAL_NUMBER = 0x0308, /**< MeterSerialNumber (Optional) */
    ZCL_METER_SVR_ATTR_EC_UNIT_OF_MEASURE = 0x0309, /**< EnergyCarrierUnitOfMeasure (Optional) */
    ZCL_METER_SVR_ATTR_EC_SUMMATION_FORMAT = 0x030a, /**< EnergyCarrierSummationFormatting (Optional) */
    ZCL_METER_SVR_ATTR_EC_DEMAND_FORMAT = 0x030b, /**< EnergyCarrierDemandFormatting (Optional) */
    ZCL_METER_SVR_ATTR_TEMP_UNIT_OF_MEASURE = 0x030c, /**< TemperatureUnitOfMeasure (Optional) */
    ZCL_METER_SVR_ATTR_TEMP_FORMAT = 0x030d, /**< TemperatureFormatting (Optional) */
    ZCL_METER_SVR_ATTR_MODULE_SERIAL_NUMBER = 0x030e, /**< ModuleSerialNumber (Optional) */
    ZCL_METER_SVR_ATTR_OPERTRFLBL_DELIV = 0x030f, /**< OperatingTariffLabel Delivered (Optional) */
    ZCL_METER_SVR_ATTR_OPERTRFLBL_RECIEVED = 0x0310, /**< OperatingTariffLabel Received (Optional) */
    ZCL_METER_SVR_ATTR_CUSTOMER_ID_NUMBER = 0x0311, /**< CustomerIDNumber (Optional) */
    ZCL_METER_SVR_ATTR_ALT_UNIT_OF_MEASURE = 0x0312, /**< AlternativeUnitof Measure (Optional) */
    ZCL_METER_SVR_ATTR_ALT_DEMAND_FORMATTING = 0x0313, /**< AlternativeDemandFormatting (Optional) */
    ZCL_METER_SVR_ATTR_ALT_CONSUMPTION_FORMATTING = 0x0314, /**< AlternativeDemandFormatting (Optional) */

    /* Historical Consumption Set (0x04) */
    ZCL_METER_SVR_ATTR_INSTANTANEOUS_DEMAND = 0x0400, /**< InstantaneousDemand (Optional) */
    ZCL_METER_SVR_ATTR_CURDAY_DM_DELIV = 0x0401, /**< CurrentDayConsumptionDelivered (Optional) */
    ZCL_METER_SVR_ATTR_CURDAY_DM_RECV = 0x0402, /**< CurrentDayConsumptionReceived (Optional) */
    ZCL_METER_SVR_ATTR_PRVDAY_DM_DELIV = 0x0403, /**< PreviousDayConsumptionDelivered (Optional) */
    ZCL_METER_SVR_ATTR_PRVDAY_DM_RECV = 0x0404, /**< PreviousDayConsumptionReceived (Optional) */
    ZCL_METER_SVR_ATTR_CURPRTL_DELIV_START = 0x0405, /**< CurrentPartialProfileIntervalStartTimeDelivered (Optional) */
    ZCL_METER_SVR_ATTR_CURPRTL_RECV_START = 0x0406, /**< CurrentPartialProfileIntervalStartTimeReceived (Optional) */
    ZCL_METER_SVR_ATTR_CURPRTL_DELIV = 0x0407, /**< CurrentPartialProfileIntervalValueDelivered (Optional) */
    ZCL_METER_SVR_ATTR_CURPRTL_RECV = 0x0408, /**< CurrentPartialProfileIntervalValueReceived (Optional) */
    ZCL_METER_SVR_ATTR_CURDAY_MAX_PRESSURE = 0x0409, /**< CurrentDayMaxPressure (Optional) */
    ZCL_METER_SVR_ATTR_CURDAY_MIN_PRESSURE = 0x040a, /**< CurrentDayMinPressure (Optional) */
    ZCL_METER_SVR_ATTR_PRVDAY_MAX_PRESSURE = 0x040b, /**< PreviousDayMaxPressure (Optional) */
    ZCL_METER_SVR_ATTR_PRVDAY_MIN_PRESSURE = 0x040c, /**< PreviousDayMinPressure (Optional) */
    ZCL_METER_SVR_ATTR_CURDAY_MAX_DEMAND = 0x040d, /**< CurrentDayMaxDemand (Optional) */
    ZCL_METER_SVR_ATTR_PRVDAY_MAX_DEMAND = 0x040e, /**< PreviousDayMaxDemand (Optional) */
    ZCL_METER_SVR_ATTR_CURMTH_MAX_DEMAND = 0x040f, /**< CurrentMonthMaxDemand (Optional) */
    ZCL_METER_SVR_ATTR_CURYR_MAX_DEMAND = 0x0410, /**< CurrentYearMaxDemand (Optional) */
    ZCL_METER_SVR_ATTR_CURDAY_EC_MAX_DEMAND = 0x0411, /**< CurrentDayMaxEnergyCarrierDemand (Optional) */
    ZCL_METER_SVR_ATTR_PRVDAY_EC_MAX_DEMAND = 0x0412, /**< PreviousDayMaxEnergyCarrierDemand (Optional) */
    ZCL_METER_SVR_ATTR_CURMTH_EC_MAX_DEMAND = 0x0413, /**< CurrentMonthMaxEnergyCarrierDemand (Optional) */
    ZCL_METER_SVR_ATTR_CURMTH_EC_MIN_DEMAND = 0x0414, /**< CurrentMonthMinEnergyCarrierDemand (Optional) */
    ZCL_METER_SVR_ATTR_CURYR_EC_MAX_DEMAND = 0x0415, /**< CurrentYearMaxEnergyCarrierDemand (Optional) */
    ZCL_METER_SVR_ATTR_CURYR_EC_MIN_DEMAND = 0x0416, /**< CurrentYearMinEnergyCarrierDemand (Optional) */

    ZCL_METER_SVR_ATTR_PRVDAY_2_DELIV_CONSUMP = 0x0420,
    /**< PreviousDay2ConsumptionDelivered (Optional)
    * ZCL_METER_SVR_ATTR_PRVDAY_N_DELIV_CONSUMP(2)
    * For all the previous days, use the ZCL_METER_SVR_ATTR_PRVDAY_N_DELIV_CONSUMP(prev_day) macro. */
    ZCL_METER_SVR_ATTR_PRVDAY_2_RECV_CONSUMP = 0x0421,
    /**< PreviousDay2ConsumptionReceived (Optional)
    * ZCL_METER_SVR_ATTR_PRVDAY_N_RECV_CONSUMP(2)
    * For all the previous days, use the ZCL_METER_SVR_ATTR_PRVDAY_N_RECV_CONSUMP(prev_day) macro. */

    ZCL_METER_SVR_ATTR_WEEK_0_CONSUMP_DELIV = 0x0430,
    /**< CurrentWeekConsumptionDelivered (Optional) - ZCL_METER_SVR_ATTR_WEEK_N_CONSUMP_DELIV(0) */
    ZCL_METER_SVR_ATTR_WEEK_0_CONSUMP_RECV = 0x0431,
    /**< CurrentWeekConsumptionReceived (Optional) - ZCL_METER_SVR_ATTR_WEEK_N_CONSUMP_RECV(0) */
    ZCL_METER_SVR_ATTR_WEEK_1_CONSUMP_DELIV = 0x0432,
    /**< PreviousWeekConsumptionDelivered (Optional) - ZCL_METER_SVR_ATTR_WEEK_N_CONSUMP_DELIV(1) */
    ZCL_METER_SVR_ATTR_WEEK_1_CONSUMP_RECV = 0x0433,
    /**< PreviousWeekConsumptionReceived (Optional) - ZCL_METER_SVR_ATTR_WEEK_N_CONSUMP_RECV(1) */
    ZCL_METER_SVR_ATTR_WEEK_2_CONSUMP_DELIV = 0x0434,
    /**< PreviousWeek2ConsumptionDelivered (Optional)
     * ZCL_METER_SVR_ATTR_WEEK_N_CONSUMP_DELIV(2)
     * For all the week consumption delivered, use the ZCL_METER_SVR_ATTR_WEEK_N_CONSUMP_DELIV(prev_week) macro. */
    ZCL_METER_SVR_ATTR_WEEK_2_CONSUMP_RECV = 0x0435,
    /**< PreviousWeek2ConsumptionReceived (Optional)
     * ZCL_METER_SVR_ATTR_WEEK_N_CONSUMP_RECV(2)
     * For all the week consumption received, use the ZCL_METER_SVR_ATTR_WEEK_N_CONSUMP_RECV(prev_week) macro. */

    ZCL_METER_SVR_ATTR_MONTH_0_CONSUMP_DELIV = 0x0440,
    /**< CurrentMonthConsumptionDelivered (Optional) - ZCL_METER_SVR_ATTR_MONTH_N_CONSUMP_DELIV(0) */
    ZCL_METER_SVR_ATTR_MONTH_0_CONSUMP_RECV = 0x0441,
    /**< CurrentMonthConsumptionReceived (Optional) - ZCL_METER_SVR_ATTR_MONTH_N_CONSUMP_RECV(0) */
    ZCL_METER_SVR_ATTR_MONTH_1_CONSUMP_DELIV = 0x0442,
    /**< PreviousMonthConsumptionDelivered (Optional) - ZCL_METER_SVR_ATTR_MONTH_N_CONSUMP_DELIV(1) */
    ZCL_METER_SVR_ATTR_MONTH_1_CONSUMP_RECV = 0x0443,
    /**< PreviousMonthConsumptionReceived (Optional) - ZCL_METER_SVR_ATTR_MONTH_N_CONSUMP_RECV(1) */
    ZCL_METER_SVR_ATTR_MONTH_2_CONSUMP_DELIV = 0x0444,
    /**< PreviousMonth2ConsumptionDelivered (Optional)
     * ZCL_METER_SVR_ATTR_MONTH_N_CONSUMP_DELIV(2)
     * For all the month consumption delivered, use the ZCL_METER_SVR_ATTR_MONTH_N_CONSUMP_DELIV(prev_month) macro. */
    ZCL_METER_SVR_ATTR_MONTH_2_CONSUMP_RECV = 0x0445,
    /**< PreviousMonth2ConsumptionReceived (Optional)
     * ZCL_METER_SVR_ATTR_MONTH_N_CONSUMP_RECV(2)
     * For all the prev_month consumption received, use the ZCL_METER_SVR_ATTR_MONTH_N_CONSUMP_RECV(prev_month) macro. */

    ZCL_METER_SVR_ATTR_HISTORICAL_FREEZE_TIME = 0x045C, /**< Historical Freeze Time (Optional) */

    /* Load Profile Configuration Set (0x05) */
    ZCL_METER_SVR_ATTR_MAX_PERIODS_DELIV = 0x0500, /**< MaxNumberOfPeriodsDelivered (Optional) */

    /* Supply Limit Set (0x06) */
    ZCL_METER_SVR_ATTR_CURDEMAND_DELIV = 0x0600, /**< CurrentDemandDelivered (Optional) */
    ZCL_METER_SVR_ATTR_DEMAND_LIMIT = 0x0601, /**< DemandLimit (Optional) */
    ZCL_METER_SVR_ATTR_DEMAND_INTEGRATION_PERIOD = 0x0602, /**< DemandIntegrationPeriod (Optional) */
    ZCL_METER_SVR_ATTR_DEMAND_SUBINTERVALS = 0x0603, /**< NumberOfDemandSubintervals (Optional) */
    ZCL_METER_SVR_ATTR_SUPPLY_LIMIT_ARM_DURATION = 0x0604, /**< DemandLimitArmDuration (Optional) */
    ZCL_METER_SVR_ATTR_LOAD_LIMIT_SUPPLY_STATE = 0x0605, /**< LoadLimitSupplyState (Optional) */
    ZCL_METER_SVR_ATTR_LOAD_LIMIT_COUNTER = 0x0606, /**< LoadLimitCounter (Optional) */
    ZCL_METER_SVR_ATTR_SUPPLY_TAMPER_STATE = 0x0607, /**< SupplyTamperState (Optional) */
    ZCL_METER_SVR_ATTR_SUPPLY_DEPLETION_STATE = 0x0608, /**< SupplyDepletionState (Optional) */
    ZCL_METER_SVR_ATTR_SUPPLY_UNCONTROLLED_FLOW_STATE = 0x0609, /**< SupplyUncontrolled FlowState (Optional) */

    /* Block Information Set (Delivered) (0x07) */
    ZCL_METER_SVR_ATTR_NO_TIER_BLOCK1_DELIV = 0x0700,
    /**< CurrentNoTierBlock1SummationDelivered (Optional)
     * ZCL_METER_SVR_ATTR_NO_TIER_BLOCKN_DELIV(1)
     * For all CurrentNoTierBlockNSummationDelivered, use the ZCL_METER_SVR_ATTR_NO_TIER_BLOCKN_DELIV(block) macro. */
    ZCL_METER_SVR_ATTR_TIER1_BLOCK1_DELIV = 0x0710,
    /**< CurrentTier1Block1SummationDelivered (Optional)
     * ZCL_METER_SVR_ATTR_TIERN_BLOCKN_DELIV(1,1)
     * For all CurrentTierNBlockNSummationDelivered, use the ZCL_METER_SVR_ATTR_TIERN_BLOCKN_DELIV(tier, block) macro. */

    /* Alarms Set (0x08) */
    ZCL_METER_SVR_ATTR_GENERIC_ALARM_MASK = 0x0800, /**< Generic Alarm Mask */
    ZCL_METER_SVR_ATTR_ELECTRICITY_ALARM_MASK = 0x0801, /**< Electricity Alarm Mask */
    ZCL_METER_SVR_ATTR_FLOW_PRESSURE_ALARM_MASK = 0x0802, /**< Generic Flow/Pressure Alarm Mask */
    ZCL_METER_SVR_ATTR_WATER_ALARM_MASK = 0x0803, /**< Water Specific Alarm Mask */
    ZCL_METER_SVR_ATTR_HEAT_COOLING_ALARM_MASK = 0x0804, /**< Heat and Cooling Specific Alarm Mask */
    ZCL_METER_SVR_ATTR_GAS_ALARM_MASK = 0x0805, /**< Gas Specific AlarmMask */
    ZCL_METER_SVR_ATTR_EXTENDED_GENERIC_ALARM_MASK = 0x0806, /**< ExtendedGenericAlarmMask */
    ZCL_METER_SVR_ATTR_MANUFACTURER_ALARM_MASK = 0x0807, /**< ManufacturerAlarmMask */

    /* Block Information Set (Received) (0x09) */
    ZCL_METER_SVR_ATTR_NO_TIER_BLOCK1_RECV = 0x0900,
    /**< CurrentNoTierBlock1SummationReceived (Optional)
     * ZCL_METER_SVR_ATTR_NO_TIER_BLOCKN_RECV(1)
     * For all CurrentNoTierBlockNSummationReceived, use the ZCL_METER_SVR_ATTR_NO_TIER_BLOCKN_RECV(block) macro. */
    ZCL_METER_SVR_ATTR_TIER1_BLOCK1_RECV = 0x0910,
    /**< CurrentTier1Block1SummationReceived (Optional)
     * ZCL_METER_SVR_ATTR_TIERN_BLOCKN_RECV(1,1)
     * For all CurrentTierNBlockNSummationReceived, use the ZCL_METER_SVR_ATTR_TIERN_BLOCKN_RECV(tier, block) macro. */

    /* Meter Billing Set (0x0a) */
    ZCL_METER_SVR_ATTR_BTD_DELIV = 0x0a00, /**< BillToDateDelivered (Optional) */
    ZCL_METER_SVR_ATTR_BTD_TIMESTAMP_DELIV = 0x0a01, /**< BillToDateTimeStampDelivered (Optional) */
    ZCL_METER_SVR_ATTR_PROJ_BILL_DELIV = 0x0a02, /**< ProjectedBillDelivered (Optional) */
    ZCL_METER_SVR_ATTR_PROJ_BILL_TIMESTAMP_DELIV = 0x0a03, /**< ProjectedBillTimeStampDelivered (Optional) */
    ZCL_METER_SVR_ATTR_BILL_DELIV_TRAILING_DIGIT = 0x0a04, /**< BillDeliveredTrailingDigit (Optional) */
    ZCL_METER_SVR_ATTR_BTD_RECV = 0x0a10, /**< BillToDateReceived (Optional) */
    ZCL_METER_SVR_ATTR_BTD_TIMESTAMP_RECV = 0x0a11, /**< BillToDateTimeStampReceived (Optional) */
    ZCL_METER_SVR_ATTR_PROJ_BILL_RECV = 0x0a12, /**< ProjectedBillReceived (Optional) */
    ZCL_METER_SVR_ATTR_PROJ_BILL_TIMESTAMP_RECV = 0x0a13, /**< ProjectedBillTimeStampReceived (Optional) */
    ZCL_METER_SVR_ATTR_BILL_RECV_TRAILING_DIGIT = 0x0a14, /**< ProjectedBillTimeStampReceived (Optional) */

    /* Supply Control Set (0x0B) */
    ZCL_METER_SVR_ATTR_PROP_CHG_SUPPLY_IMPL_TIME = 0x0b00, /**< ProposedChangeSupply ImplementationTime (Optional) */
    ZCL_METER_SVR_ATTR_PROP_CHG_SUPPLY_STATUS = 0x0b01, /**< ProposedChange SupplyStatus (Optional) */
    ZCL_METER_SVR_ATTR_UNCTLFLOW_THRESHOLD = 0x0b10, /**< Uncontrolled Flow Threshold (Optional) */
    ZCL_METER_SVR_ATTR_UNCTLFLOW_UNIT_OF_MEASURE = 0x0b11, /**< Uncontrolled Flow Threshold Unit of Measure (Optional) */
    ZCL_METER_SVR_ATTR_UNCTLFLOW_MULTIPLIER = 0x0b12, /**< Uncontrolled Flow Multiplier (Optional) */
    ZCL_METER_SVR_ATTR_UNCTLFLOW_DIVISOR = 0x0b13, /**< Uncontrolled Flow Divisor (Optional) */
    ZCL_METER_SVR_ATTR_FLOW_STABILISATION_PERIOD = 0x0b14, /**< Flow Stabilisation Period (Optional) */
    ZCL_METER_SVR_ATTR_FLOW_MEASUREMENT_PERIOD = 0x0b15, /**< Flow Measurement Period (Optional) */

    /* Alternative Historical Consumption (0x0C) */
    ZCL_METER_SVR_ATTR_ALT_INSTANTANEOUS_DEMAND = 0x0c00 /**< AlternativeInstantaneousDemand (Optional) */
        /* ! Remaining enum values or macros not supported */
};

/* Simple Metering Time of Use Information Attributes */

/* Tiers start at 1 and go to a max of 48.
 * They should have started at an index of 0, but oh well */
#define ZCL_METER_SVR_ATTR_TIERN_MAX                                48U
#define ZCL_METER_SVR_ATTR_TIERN_DELIV(tier)                        (0x0100U + (2U * ((tier) - 1U)))
#define ZCL_METER_SVR_ATTR_TIERN_RECV(tier)                         (0x0101U + (2U * ((tier) - 1U)))

/* Simple Metering Block Information Attributes (Delivered) */
#define ZCL_METER_SVR_ATTR_TIERN_BLOCKN_DELIV(tier, block)          (0x0700U + ((block) - 1U) + (0x10U * (tier)))
#define ZCL_METER_SVR_ATTR_NO_TIER_BLOCKN_DELIV(block)              ZCL_METER_SVR_ATTR_TIERN_BLOCKN_DELIV(0U, (block))

/* Simple Metering Block Information Attributes (Received) */
#define ZCL_METER_SVR_ATTR_TIERN_BLOCKN_RECV(tier, block)           (0x0900U + ((block) - 1U) + (0x10U * (tier)))
#define ZCL_METER_SVR_ATTR_NO_TIER_BLOCKN_RECV(block)               ZCL_METER_SVR_ATTR_TIERN_BLOCKN_RECV(0U, block)

/* Simple Metering ESP Historical Attributes */
#define ZCL_METER_SVR_ATTR_PRVDAY_N_DELIV_CONSUMP(prev_day)         (0x0420U + (2U * ((prev_day) - 2U)))
#define ZCL_METER_SVR_ATTR_PRVDAY_N_RECV_CONSUMP(prev_day)          (0x0421U + (2U * ((prev_day) - 2U)))

#define ZCL_METER_SVR_ATTR_WEEK_FIRST_CONSUMP_DELIV                 0U
#define ZCL_METER_SVR_ATTR_WEEK_MAX_CONSUMP_DELIV                   6U /* less than, i.e. 0..5 */
#define ZCL_METER_SVR_ATTR_WEEK_N_CONSUMP_DELIV(prev_week)          (0x0430U + (2U * (prev_week)))
#define ZCL_METER_SVR_ATTR_WEEK_N_CONSUMP_RECV(prev_week)           (0x0431U + (2U * (prev_week)))
#define ZCL_METER_SVR_ATTR_MONTH_FIRST_CONSUMP_DELIV                0U
#define ZCL_METER_SVR_ATTR_MONTH_MAX_CONSUMP_DELIV                  14U /* less than, i.e. 0..13 */
#define ZCL_METER_SVR_ATTR_MONTH_N_CONSUMP_DELIV(prev_month)        (0x0440U + (2U * (prev_month)))
#define ZCL_METER_SVR_ATTR_MONTH_N_CONSUMP_RECV(prev_month)         (0x0441U + (2U * (prev_month)))

/** Metering Client Attribute IDs */
enum ZbZclMeterCliAttrT {
    /* Notification Set (0x00) */
    ZCL_METER_CLI_ATTR_FUNC_NOTIF_FLAGS = 0x0000, /**< FunctionalNotificationFlags */
    ZCL_METER_CLI_ATTR_NOTIF_FLAGS_2 = 0x0001, /**< NotificationFlags2 (Price) */
    ZCL_METER_CLI_ATTR_NOTIF_FLAGS_3 = 0x0002, /**< NotificationFlags3 (Calendar) */
    ZCL_METER_CLI_ATTR_NOTIF_FLAGS_4 = 0x0003, /**< NotificationFlags4 (Prepayment) */
    ZCL_METER_CLI_ATTR_NOTIF_FLAGS_5 = 0x0004, /**< NotificationFlags5 (Device Management) */
    ZCL_METER_CLI_ATTR_NOTIF_FLAGS_6 = 0x0005, /**< NotificationFlags6 (Not supported) */
    ZCL_METER_CLI_ATTR_NOTIF_FLAGS_7 = 0x0006, /**< NotificationFlags7 (Not supported) */
    ZCL_METER_CLI_ATTR_NOTIF_FLAGS_8 = 0x0007, /**< NotificationFlags8 (Not supported) */
};

/* Functional Notification Flags (ZCL_METER_CLI_ATTR_FUNC_NOTIF_FLAGS) */
#define ZCL_METER_FUNC_NOTIF_FLAG_NEW_OTA_FW                        0x00000001U /* bit  0 */
#define ZCL_METER_FUNC_NOTIF_FLAG_CBKE_UPDATE_REQ                   0x00000002U /* bit  1 */
#define ZCL_METER_FUNC_NOTIF_FLAG_TIME_SYNC                         0x00000004U /* bit  2 */
/* Reserved - 0x00000008U */
#define ZCL_METER_FUNC_NOTIF_FLAG_STAY_AWAKE_HAN                    0x00000010U /* bit  4 */
#define ZCL_METER_FUNC_NOTIF_FLAG_STAY_AWAKE_WAN                    0x00000020U /* bit  5 */
#define ZCL_METER_FUNC_NOTIF_FLAG_PUSH_HIST_METER_MASK              0x000001C0U /* bits 6-8 */
#define ZCL_METER_FUNC_NOTIF_FLAG_PUSH_HIST_PREPAY_SET              0x00000E00U /* bits 9-11 */
#define ZCL_METER_FUNC_NOTIF_FLAG_PUSH_STATIC_BASIC                 0x00001000U /* bit 12 */
#define ZCL_METER_FUNC_NOTIF_FLAG_PUSH_STATIC_METER                 0x00002000U /* bit 13 */
#define ZCL_METER_FUNC_NOTIF_FLAG_PUSH_STATIC_PREPAY                0x00004000U /* bit 14 */
#define ZCL_METER_FUNC_NOTIF_FLAG_NETWORK_KEY_ACTIVE                0x00008000U /* bit 15 */
#define ZCL_METER_FUNC_NOTIF_FLAG_DISPLAY_MESSAGE                   0x00010000U /* bit 16 */
#define ZCL_METER_FUNC_NOTIF_FLAG_CANCEL_ALL_MESSAGES               0x00020000U /* bit 17 */
#define ZCL_METER_FUNC_NOTIF_FLAG_CHANGE_SUPPLY                     0x00040000U /* bit 18 */
#define ZCL_METER_FUNC_NOTIF_FLAG_LOCAL_CHANGE_SUPPLY               0x00080000U /* bit 19 */
#define ZCL_METER_FUNC_NOTIF_FLAG_SET_UNCONTROLLED_FLOW_THRESH      0x00100000U /* bit 20 */
#define ZCL_METER_FUNC_NOTIF_FLAG_TUNNEL_MESSAGE_PEND               0x00200000U /* bit 21 */
#define ZCL_METER_FUNC_NOTIF_FLAG_GET_SNAPSHOT                      0x00400000U /* bit 22 */
#define ZCL_METER_FUNC_NOTIF_FLAG_GET_SAMPLED_DATA                  0x00800000U /* bit 23 */
#define ZCL_METER_FUNC_NOTIF_FLAG_NEW_SUBGHZ_CHANNEL_MASKS          0x01000000U /* bit 24 */
#define ZCL_METER_FUNC_NOTIF_FLAG_ENERGY_SCAN_PEND                  0x02000000U /* bit 25 */
#define ZCL_METER_FUNC_NOTIF_FLAG_CHANNEL_CHANGE_PEND               0x04000000U /* bit 26 */

/* NotificationFlags2 (ZCL_METER_CLI_ATTR_NOTIF_FLAGS_2, Price) */
#define ZCL_METER_NOTIF_FLAG_2_PUBLISH_PRICE                        0x00000001U /* bit  0 */
#define ZCL_METER_NOTIF_FLAG_2_PUBLISH_BLOCK_PERIOD                 0x00000002U /* bit  1 */
#define ZCL_METER_NOTIF_FLAG_2_PUBLISH_TARIFF_INFO                  0x00000004U /* bit  2 */
#define ZCL_METER_NOTIF_FLAG_2_PUBLISH_CONV_FACTOR                  0x00000008U /* bit  3 */
#define ZCL_METER_NOTIF_FLAG_2_PUBLISH_CAL_VAL                      0x00000010U /* bit  4 */
#define ZCL_METER_NOTIF_FLAG_2_PUBLISH_CO2_VAL                      0x00000020U /* bit  5 */
#define ZCL_METER_NOTIF_FLAG_2_PUBLISH_BILL_PERIOD                  0x00000040U /* bit  6 */
#define ZCL_METER_NOTIF_FLAG_2_PUBLISH_CONS_BILL                    0x00000080U /* bit  7 */
#define ZCL_METER_NOTIF_FLAG_2_PUBLISH_PRICE_MATRIX                 0x00000100U /* bit  8 */
#define ZCL_METER_NOTIF_FLAG_2_PUBLISH_BLOCK_THRESH                 0x00000200U /* bit  9 */
#define ZCL_METER_NOTIF_FLAG_2_PUBLISH_CURRENCY_CONV                0x00000400U /* bit 10 */
/* Reserved */
#define ZCL_METER_NOTIF_FLAG_2_PUBLISH_CREDIT_PAY_INFO              0x00001000U /* bit 12 */
#define ZCL_METER_NOTIF_FLAG_2_PUBLISH_CPP_EVENT                    0x00002000U /* bit 13 */
#define ZCL_METER_NOTIF_FLAG_2_PUBLISH_TIER_LABELS                  0x00004000U /* bit 14 */
#define ZCL_METER_NOTIF_FLAG_2_PUBLISH_CANCEL_TARIFF                0x00008000U /* bit 15 */

/* NotificationFlags3 (ZCL_METER_CLI_ATTR_NOTIF_FLAGS_3, Calendar) */
#define ZCL_METER_NOTIF_FLAG_3_PUBLISH_CALENDAR                     0x00000001U /* bit  0 */
#define ZCL_METER_NOTIF_FLAG_3_PUBLISH_SPECIAL_DAYS                 0x00000002U /* bit  1 */
#define ZCL_METER_NOTIF_FLAG_3_PUBLISH_SEASONS                      0x00000004U /* bit  2 */
#define ZCL_METER_NOTIF_FLAG_3_PUBLISH_WEEK                         0x00000008U /* bit  3 */
#define ZCL_METER_NOTIF_FLAG_3_PUBLISH_DAY                          0x00000010U /* bit  4 */
#define ZCL_METER_NOTIF_FLAG_3_PUBLISH_CANCEL_CALENDAR              0x00000020U /* bit  5 */

/* NotificationFlags4 (ZCL_METER_CLI_ATTR_NOTIF_FLAGS_4, Prepayment) */
#define ZCL_METER_NOTIF_FLAG_4_SELECT_AVAIL_EMERG_CREDIT            0x00000001U /* bit  0 */
#define ZCL_METER_NOTIF_FLAG_4_CHANGE_DEBT                          0x00000002U /* bit  1 */
#define ZCL_METER_NOTIF_FLAG_4_EMERG_CREDIT_SETUP                   0x00000004U /* bit  2 */
#define ZCL_METER_NOTIF_FLAG_4_CONSUMER_TOP_UP                      0x00000008U /* bit  3 */
#define ZCL_METER_NOTIF_FLAG_4_CREDIT_ADJUST                        0x00000010U /* bit  4 */
#define ZCL_METER_NOTIF_FLAG_4_CHANGE_PAY_MODE                      0x00000020U /* bit  5 */
#define ZCL_METER_NOTIF_FLAG_4_GET_PREPAY_SNAPSHOT                  0x00000040U /* bit  6 */
#define ZCL_METER_NOTIF_FLAG_4_GET_TOP_UP_LOG                       0x00000080U /* bit  7 */
#define ZCL_METER_NOTIF_FLAG_4_SET_LOW_CREDIT_WARN_LEVEL            0x00000100U /* bit  8 */
#define ZCL_METER_NOTIF_FLAG_4_GET_DEBT_REPAY_LOG                   0x00000200U /* bit  9 */
#define ZCL_METER_NOTIF_FLAG_4_SET_MAX_CREDIT_LIMIT                 0x00000400U /* bit 10 */
#define ZCL_METER_NOTIF_FLAG_4_SET_OVERALL_DEBT_CAP                 0x00000800U /* bit 11 */

/* NotificationFlags5 (ZCL_METER_CLI_ATTR_NOTIF_FLAGS_5, Device Management) */
#define ZCL_METER_NOTIF_FLAG_5_PUBLISH_CHANGE_TENANCY               0x00000001U /* bit  0 */
#define ZCL_METER_NOTIF_FLAG_5_PUBLISH_CHANGE_SUPPLIER              0x00000002U /* bit  1 */
#define ZCL_METER_NOTIF_FLAG_5_REQUEST_NEW_PW_1_RSP                 0x00000004U /* bit  2 */
#define ZCL_METER_NOTIF_FLAG_5_REQUEST_NEW_PW_2_RSP                 0x00000008U /* bit  3 */
#define ZCL_METER_NOTIF_FLAG_5_REQUEST_NEW_PW_3_RSP                 0x00000010U /* bit  4 */
#define ZCL_METER_NOTIF_FLAG_5_REQUEST_NEW_PW_4_RSP                 0x00000020U /* bit  5 */
#define ZCL_METER_NOTIF_FLAG_5_UPDATE_SITE_ID                       0x00000040U /* bit  6 */
#define ZCL_METER_NOTIF_FLAG_5_RESET_BATTERY_COUNTER                0x00000080U /* bit  7 */
#define ZCL_METER_NOTIF_FLAG_5_UPDATE_CIN                           0x00000100U /* bit  8 */

/* Metering Status flags (ZCL_METER_SVR_ATTR_METER_STATUS) */
#define ZCL_METER_STATUS_CHECK_METER                0x01U
#define ZCL_METER_STATUS_LOW_BATTERY                0x02U
#define ZCL_METER_STATUS_TAMPER_DETECT              0x04U
#define ZCL_METER_STATUS_POWER_FAILURE              0x08U
#define ZCL_METER_STATUS_PIPE_EMPTY                 0x08U
#define ZCL_METER_STATUS_TEMPERATURE_SENSOR         0x08U
#define ZCL_METER_STATUS_POWER_QUALITY              0x10U
#define ZCL_METER_STATUS_LOW_PRESSURE               0x10U
#define ZCL_METER_STATUS_BURST_DETECT               0x10U
#define ZCL_METER_STATUS_LEAK_DETECT                0x20U
#define ZCL_METER_STATUS_SERVICE_DISCONNECT         0x40U
#define ZCL_METER_STATUS_REVERSE_FLOW               0x80U
#define ZCL_METER_STATUS_FLOW_SENSOR                0x80U

#define ZCL_METER_FORMAT_FRACTIONAL                 0x07U
#define ZCL_METER_FORMAT_INTEGER                    0x78U
#define ZCL_METER_FORMAT_SUPPRESS_LEADING_ZERO      0x80U
#define ZCL_METER_FORMAT_OFFSET_FRACTIONAL          0U
#define ZCL_METER_FORMAT_OFFSET_INTEGER             3U

/* Metering Units of Measure enumeration (ZCL_METER_SVR_ATTR_UNIT_OF_MEASURE / UnitofMeasure) */
enum ZbZclMeterUnitsT {
    ZCL_METER_UNITS_BINARY_KWH = 0x00,
    ZCL_METER_UNITS_BINARY_CUBIC_METERS = 0x01,
    ZCL_METER_UNITS_BINARY_CUBIC_FEET = 0x02,
    ZCL_METER_UNITS_BINARY_CCF = 0x03,
    ZCL_METER_UNITS_BINARY_US_GALLON = 0x04,
    ZCL_METER_UNITS_BINARY_IMPERIAL_GALLON = 0x05,
    ZCL_METER_UNITS_BINARY_BTU = 0x06,
    ZCL_METER_UNITS_BINARY_LITERS = 0x07,
    ZCL_METER_UNITS_BINARY_RELATIVE_KPA = 0x08,
    ZCL_METER_UNITS_BINARY_ABSOLUTE_KPA = 0x09,
    ZCL_METER_UNITS_BINARY_MCF = 0x0a, /* SE 1.1 */
    ZCL_METER_UNITS_BINARY_UNITLE = 0x0b, /* SE 1.1 */
    ZCL_METER_UNITS_BINARY_MEGA_JOULE = 0x0c, /* SE 1.1 */
    ZCL_METER_UNITS_BINARY_KVAR = 0x0d, /* SE 1.2a */

    ZCL_METER_UNITS_BCD_KWH = 0x80,
    ZCL_METER_UNITS_BCD_CUBIC_METERS = 0x81,
    ZCL_METER_UNITS_BCD_CUBIC_FEET = 0x82,
    ZCL_METER_UNITS_BCD_CCF = 0x83,
    ZCL_METER_UNITS_BCD_US_GALLON = 0x84,
    ZCL_METER_UNITS_BCD_IMPERIAL_GALLON = 0x85,
    ZCL_METER_UNITS_BCD_BTU = 0x86,
    ZCL_METER_UNITS_BCD_LITERS = 0x87,
    ZCL_METER_UNITS_BCD_RELATIVE_KPA = 0x88,
    ZCL_METER_UNITS_BCD_ABSOLUTE_KPA = 0x89,
    ZCL_METER_UNITS_BCD_MCF = 0x8a, /* SE 1.1 */
    ZCL_METER_UNITS_BCD_UNITLE = 0x8b, /* SE 1.1 */
    ZCL_METER_UNITS_BCD_MEGA_JOULE = 0x8c, /* SE 1.1 */
    ZCL_METER_UNITS_BCD_KVAR = 0x8d /* SE 1.2a */
};

/* Metering Device Type enumerations (ZCL_METER_SVR_ATTR_DEVICE_TYPE) */
enum ZbZclMeterTypeT {
    ZCL_METER_TYPE_ELECTRIC = 0x00,
    ZCL_METER_TYPE_GAS = 0x01,
    ZCL_METER_TYPE_WATER = 0x02,
    /* ZCL_METER_TYPE_THERMAL = 0x03, Deprecated as of SE 1.1 */
    ZCL_METER_TYPE_PRESSURE = 0x04,
    ZCL_METER_TYPE_HEAT = 0x05,
    ZCL_METER_TYPE_COOLING = 0x06,

    ZCL_METER_TYPE_END_USE_METER = 0x07, /* SE 1.1b */
    ZCL_METER_TYPE_PV_GEN = 0x08, /* SE 1.1b */
    ZCL_METER_TYPE_WIND_GEN = 0x09, /* SE 1.1b */
    ZCL_METER_TYPE_WATER_GEN = 0x0a, /* SE 1.1b */
    ZCL_METER_TYPE_MICRO_GEN = 0x0b, /* SE 1.1b */
    ZCL_METER_TYPE_SOLAR_HW = 0x0c, /* SE 1.1b */
    ZCL_METER_TYPE_ELECTRIC_PHASE_1 = 0x0d, /* SE 1.1b */
    ZCL_METER_TYPE_ELECTRIC_PHASE_2 = 0x0e, /* SE 1.1b */
    ZCL_METER_TYPE_ELECTRIC_PHASE_3 = 0x0f, /* SE 1.1b */

    ZCL_METER_TYPE_MIRROR_ELECTRIC = 0x7f, /**< Mirrored Electric Metering */
    ZCL_METER_TYPE_MIRROR_GAS = 0x80,
    ZCL_METER_TYPE_MIRROR_WATER = 0x81,
    /* ZCL_METER_TYPE_MIRROR_THERMAL = 0x82, Deprecated as of SE 1.1 */
    ZCL_METER_TYPE_MIRROR_PRESSURE = 0x83,
    ZCL_METER_TYPE_MIRROR_HEAT = 0x84,
    ZCL_METER_TYPE_MIRROR_COOLING = 0x85,

    ZCL_METER_TYPE_MIRROR_END_USE_METER = 0x86, /* SE 1.1b */
    ZCL_METER_TYPE_MIRROR_PV_GEN = 0x87, /* SE 1.1b */
    ZCL_METER_TYPE_MIRROR_WIND_GEN = 0x88, /* SE 1.1b */
    ZCL_METER_TYPE_MIRROR_WATER_GEN = 0x89, /* SE 1.1b */
    ZCL_METER_TYPE_MIRROR_MICRO_GEN = 0x8a, /* SE 1.1b */
    ZCL_METER_TYPE_MIRROR_SOLAR_HW = 0x8b, /* SE 1.1b */
    ZCL_METER_TYPE_MIRROR_ELECPHS_1 = 0x8c, /* SE 1.1b */
    ZCL_METER_TYPE_MIRROR_ELECPHS_2 = 0x8d, /* SE 1.1b */
    ZCL_METER_TYPE_MIRROR_ELECPHS_3 = 0x8e /* SE 1.1b */
};

/*lint -save -e849 [ !MISRA - multiple string with same enum value. ] */
/* In the infinite wisdom of zigbee, they use the same enumeration values
 *  for different meter types (e.g. alarm code 0x03 means power failure for
 *  electric meter , but means pipe empty for water meter).
 */
/* Metering Status Attribute bit locations */
enum ZbZclMeterAlarmT {
    /* Simple Metering alarm codes */
    ZCL_METER_ALARM_CHECK_METER = 0x00,
    ZCL_METER_ALARM_LOW_BATTERY = 0x01,
    ZCL_METER_ALARM_TAMPER_DETECT = 0x02,
    ZCL_METER_ALARM_POWER_FAILURE = 0x03, /* Electricity alarm only */
    ZCL_METER_ALARM_PIPE_EMPTY = 0x03, /* Water alarm only.  */
    ZCL_METER_ALARM_TEMPERATURE_SENSOR = 0x03, /* Heating/Cooling alarm only */
    ZCL_METER_ALARM_POWER_QUALITY = 0x04, /* Electricity alarm only */
    ZCL_METER_ALARM_LOW_PRESSURE = 0x04, /* Gas and Water alarms only */
    ZCL_METER_ALARM_BURST_DETECT = 0x04, /* Heating/Cooling alarm only */
    ZCL_METER_ALARM_LEAK_DETECT = 0x05,
    ZCL_METER_ALARM_SERVICE_DISCONNECT = 0x06,
    ZCL_METER_ALARM_REVERSE_FLOW = 0x07, /* Gas and Water alarms only */
    ZCL_METER_ALARM_FLOW_SENSOR = 0x07, /* Heating/Cooling alarm only */

    /* Simple Metering Electricity alarm codes */
    ZCL_METER_ALARM_LOW_VOLTAGE_L1 = 0x10,
    ZCL_METER_ALARM_HIGH_VOLTAGE_L1 = 0x11,
    ZCL_METER_ALARM_LOW_VOLTAGE_L2 = 0x12,
    ZCL_METER_ALARM_HIGH_VOLTAGE_L2 = 0x13,
    ZCL_METER_ALARM_LOW_VOLTAGE_L3 = 0x14,
    ZCL_METER_ALARM_HIGH_VOLTAGE_L4 = 0x15,
    ZCL_METER_ALARM_OVER_CURRENT_L1 = 0x16,
    ZCL_METER_ALARM_OVER_CURRENT_L2 = 0x17,
    ZCL_METER_ALARM_OVER_CURRENT_L3 = 0x18,
    ZCL_METER_ALARM_FREQ_LOW_L1 = 0x19,
    ZCL_METER_ALARM_FREQ_HIGH_L1 = 0x1a,
    ZCL_METER_ALARM_FREQ_LOW_L2 = 0x1b,
    ZCL_METER_ALARM_FREQ_HIGH_L2 = 0x1c,
    ZCL_METER_ALARM_FREQ_LOW_L3 = 0x1d,
    ZCL_METER_ALARM_FREQ_HIGH_L3 = 0x1e,
    ZCL_METER_ALARM_GROUND_FAULT = 0x1f,
    ZCL_METER_ALARM_ELECTRIC_TAMPER_DETECT = 0x20,

    /* Simple Metering Pressure/Flow alarm codes */
    ZCL_METER_ALARM_PRESSURE_BURST_DETECT = 0x30,
    ZCL_METER_ALARM_PRESSURE_TOO_LOW = 0x31,
    ZCL_METER_ALARM_PRESSURE_TOO_HIGH = 0x32,
    ZCL_METER_ALARM_FLOW_COMM_ERROR = 0x33,
    ZCL_METER_ALARM_FLOW_MEASUREMENT_FAULT = 0x34,
    ZCL_METER_ALARM_FLOW_REVERSE = 0x35,
    ZCL_METER_ALARM_FLOW_AIR_DETECT = 0x36,
    ZCL_METER_ALARM_FLOW_PIPE_EMPTY = 0x37,

    /* Simple Metering Heat and Cooling alarm codes */
    ZCL_METER_ALARM_INLET_TEMPERATURE_SENSOR = 0x50,
    ZCL_METER_ALARM_OUTLET_TEMPERATURE_SENSOR = 0x51
};
/*lint -restore */

/* Metering Profile intervals */
enum ZbZclMeterProfileIntervalT {
    ZCL_METER_PRFL_PERIOD_DAILY = 0x00,
    ZCL_METER_PRFL_PERIOD_HOURLY = 0x01,
    ZCL_METER_PRFL_PERIOD_30MIN = 0x02,
    ZCL_METER_PRFL_PERIOD_15MIN = 0x03,
    ZCL_METER_PRFL_PERIOD_10MIN = 0x04,
    ZCL_METER_PRFL_PERIOD_450SEC = 0x05,
    ZCL_METER_PRFL_PERIOD_300SEC = 0x06,
    ZCL_METER_PRFL_PERIOD_150SEC = 0x07
};

/* Metering Get Profile Response Status */
enum ZbZclMeterGetProfileStatusT {
    ZCL_METER_PRFL_STATUS_SUCCESS = 0x00,
    ZCL_METER_PRFL_STATUS_CHAN_UNDEFINED = 0x01,
    ZCL_METER_PRFL_STATUS_CHAN_UNSUPPORTED = 0x02,
    ZCL_METER_PRFL_STATUS_INVALID_END_TIME = 0x03,
    ZCL_METER_PRFL_STATUS_TOO_MANY_INTERVALS = 0x04,
    ZCL_METER_PRFL_STATUS_NO_INTERVALS = 0x05
};

/* Metering Server Generated Commands */
enum ZbZclMeterServerCommandsT {
    ZCL_METER_SVR_CMD_GET_PROFILE_RESPONSE = 0x00, /**< Get Profile Response */
    ZCL_METER_SVR_CMD_REQUEST_MIRROR = 0x01, /**< Request Mirror */
    ZCL_METER_SVR_CMD_REMOVE_MIRROR = 0x02, /**< Remove Mirror */
    ZCL_METER_SVR_CMD_FAST_POLL_RESPONSE = 0x03, /**< Request Fast Poll Mode Response */
    ZCL_METER_SVR_CMD_SCHEDULE_SNAPSHOT_RESPONSE = 0x04, /**< ScheduleSnapshot Response */
    ZCL_METER_SVR_CMD_TAKE_SNAPSHOT_RESPONSE = 0x05, /**<  TakeSnapshotResponse*/
    ZCL_METER_SVR_CMD_PUBLISH_SNAPSHOT = 0x06, /**< Publish Snapshot */
    ZCL_METER_SVR_CMD_GET_SAMPLED_DATA_RESPONSE = 0x07, /**< GetSampledData Response */
    ZCL_METER_SVR_CMD_CONFIGURE_MIRROR = 0x08, /**< ConfigureMirror */
    ZCL_METER_SVR_CMD_CONFIGURE_NOTIFICATION_SCHEME = 0x09, /**< ConfigureNotification Scheme */
    ZCL_METER_SVR_CMD_CONFIGURE_NOTIFICATION_FLAG = 0x0a, /**< ConfigureNotification Flag */
    ZCL_METER_SVR_CMD_GET_NOTIFIED_MESSAGE = 0x0b, /**< GetNotifiedMessage */
    ZCL_METER_SVR_CMD_SUPPLY_STATUS_RESPONSE = 0x0c, /**< Supply Status Response */
    ZCL_METER_SVR_CMD_START_SAMPLING_RESPONSE_RESERVED = 0x0d /**< StartSamplingResponse */
};

/* Metering Client Generated Commands */
enum ZbZclMeterClientCommandsT {
    ZCL_METER_CLI_CMD_GET_PROFILE = 0x00, /**< Get Profile */
    ZCL_METER_CLI_CMD_REQUEST_MIRROR_RESPONSE = 0x01, /**< Request Mirror Response */
    ZCL_METER_CLI_CMD_MIRROR_REMOVED = 0x02, /**< Mirror Removed */
    ZCL_METER_CLI_CMD_REQUEST_FAST_POLL_MODE = 0x03, /**< Request Fast Poll Mode */
    ZCL_METER_CLI_CMD_SCHEDULE_SNAPSHOT = 0x04, /**< ScheduleSnapshot */
    ZCL_METER_CLI_CMD_TAKE_SNAPSHOT = 0x05, /**< TakeSnapshot */
    ZCL_METER_CLI_CMD_GET_SNAPSHOT = 0x06, /**< GetSnapshot */
    ZCL_METER_CLI_CMD_START_SAMPLING = 0x07, /**< StartSampling */
    ZCL_METER_CLI_CMD_GET_SAMPLED_DATA = 0x08, /**< GetSampledData */
    ZCL_METER_CLI_CMD_MIRROR_REPORT_ATTRIBUTE_RESPONSE = 0x09, /**< MirrorReport AttributeResponse */
    ZCL_METER_CLI_CMD_RESET_LOADLIMIT_COUNTER = 0x0a, /**< ResetLoadLimit Counter */
    ZCL_METER_CLI_CMD_CHANGE_SUPPLY = 0x0b, /**< Change Supply */
    ZCL_METER_CLI_CMD_LOCAL_CHANGE_SUPPLY = 0x0c, /**< Local Change Supply */
    ZCL_METER_CLI_CMD_SET_SUPPLY_STATUS = 0x0d, /**< SetSupplyStatus */
    ZCL_METER_CLI_CMD_SET_UNCONTROLLED_FLOW_THRESHOLD = 0x0e /**< SetUncontrolledFlowThreshold */
};

#define ZCL_METER_MIRROR_INVALID_ENDPOINT           0xffffU

/* Metering Sample Type Enumerations. These are the same as Interval Channel Values */
enum ZbZclMeterSampleTypeT {
    ZCL_METER_SAMPLE_TYPE_CONSUMP_DELIV = 0x00,
    ZCL_METER_SAMPLE_TYPE_CONSUMP_RECV, /* 0x01 */
    ZCL_METER_SAMPLE_TYPE_REACTIVE_CONSUMP_DELIV, /* 0x02 */
    ZCL_METER_SAMPLE_TYPE_REACTIVE_CONSUMP_RECV, /* 0x03 */
    ZCL_METER_SAMPLE_TYPE_INSTANTANEOUS_DEMAND /* 0x04 */
};

#if 0 /* not used */
struct ZbZclMeterSampleT {
    unsigned long long sumDelivered;
    unsigned long long sumReceived;
    unsigned long long demandDelivered;
    unsigned long long demandReceived;
    signed char powerFactor;
    int currentTier;
    int currentBlock;
};
#endif

/**< NotificationScheme Enumerations */
enum ZbZclMeterNotifSchemesT {
    ZCL_METER_NOTIF_SCHEME_NONE = 0x00,
    /**< No Notification Scheme Defined */

    ZCL_METER_NOTIF_SCHEME_PREDEF_A = 0x01,
    /**< Predefined Notification Scheme A. The notification flags equal 0x0FFFFFFF,
     * meaning that only the FunctionalNotificationFlags is included in the
     * MirrorReportAttributeResponse command. */

    ZCL_METER_NOTIF_SCHEME_PREDEF_B = 0x02,
    /**< Predefined Notification Scheme B. The notification flags equal 0x01234FFF,
     * meaning that the first notification flag included in the list will be the
     * FunctionalNotificationFlags followed by NotificationFlags2 to NotificationFlags5. */

    /* 0x03 to 0x80 is Reserved */
    /* 0x81 to 0xfe is For MSP Requirements */
    /* 0xff is Reserved */
};

/**< Notification Flags Order Enumerations. These are the nibble values within
 * the 32-bit Notification Flag Order field. There are 8 nibbles in total. */
enum ZbZclMeterNotifFlagsOrderT {
    ZCL_METER_NOTIF_FO_1 = 0x00, /**< NotificationFlag1 */
    ZCL_METER_NOTIF_FO_2 = 0x01, /**< NotificationFlag2 */
    ZCL_METER_NOTIF_FO_3 = 0x02, /**< NotificationFlag3 */
    ZCL_METER_NOTIF_FO_4 = 0x03, /**< NotificationFlag4 */
    ZCL_METER_NOTIF_FO_5 = 0x04, /**< NotificationFlag5 */
    ZCL_METER_NOTIF_FO_6 = 0x05, /**< NotificationFlag6 */
    ZCL_METER_NOTIF_FO_7 = 0x06, /**< NotificationFlag7 */
    ZCL_METER_NOTIF_FO_8 = 0x07, /**< NotificationFlag8 */
    /* 0x08 to 0x0e is Reserved */
    ZCL_METER_NOTIF_NONE = 0x0f, /**< Blank / No Notification Flag */
};

/* There can be a maximum of 8 notification flags (NotificationFlag1-8) */
#define ZCL_METER_NOTIF_FLAGS_MAX                   8U

#define ZCL_METER_BIT_FIELD_ALLOC_MAX_COMMANDS      32U

/**< Bit Field Allocation Command Sub Payload */
struct ZbZclMeterBitFieldAllocT {
    uint16_t cluster_id;
    uint16_t manuf_id;
    uint8_t num_commands;
    uint8_t command_ids[ZCL_METER_BIT_FIELD_ALLOC_MAX_COMMANDS];
};

/** ConfigureMirror command structure (ZCL_METER_SVR_CMD_CONFIGURE_MIRROR) */
struct ZbZclMeterServerConfigMirrorT {
    uint32_t issuer_event_id;
    /**< Issuer Event ID. This should be a UTC Time Stamp value of when the command
     * was issued. */
    uint32_t reporting_interval; /**< Reporting Interval */
    bool mirror_notif_reporting; /**< Mirror Notification Reporting */
    enum ZbZclMeterNotifSchemesT notif_scheme; /**< Notification Scheme */
};

/** ConfigureNotificationScheme command structure (ZCL_METER_SVR_CMD_CONFIGURE_NOTIFICATION_SCHEME) */
struct ZbZclMeterServerConfigNotifSchemeT {
    uint32_t issuer_event_id; /**< Issuer Event ID */
    enum ZbZclMeterNotifSchemesT notif_scheme; /**< Notification Scheme */
    enum ZbZclMeterNotifFlagsOrderT notif_flag_order[8U];
    /**< Notification Flag Order. This is presented to the application as a list of nibbles,
     * where the first item in the list is the most-significant nibble of the 32-bit value. */
};

/** ConfigureNotificationFlags command structure (ZCL_METER_SVR_CMD_CONFIGURE_NOTIFICATION_FLAG) */
struct ZbZclMeterServerConfigNotifFlagsT {
    uint32_t issuer_event_id; /**< Issuer Event ID */
    enum ZbZclMeterNotifSchemesT notif_scheme; /**< Notification Scheme */
    uint16_t notif_flag_attrid; /**< Notification Flag Attribute ID */
    struct ZbZclMeterBitFieldAllocT bit_field_alloc; /**< Bit Field Allocation. */
};

/** Get Profile command structure */
struct ZbZclMeterClientGetProfileReqT {
    enum ZbZclMeterSampleTypeT interval_channel; /**< Interval Channel */
    uint32_t end_time; /**< End Time */
    uint8_t number_of_periods; /**< NumberOfPeriods */
};

/** Get Profile Response command structure */
struct ZbZclMeterServerGetProfileRspT {
    uint32_t end_time; /**< End Time */
    enum ZbZclMeterGetProfileStatusT status; /**< Status */
    uint8_t profile_interval_period; /**< ProfileIntervalPeriod */
    uint8_t number_of_periods; /**< NumberOfPeriodsDelivered */
    uint8_t *profile_data; /**< Profile buffer */
    unsigned int profile_length; /**< Profile buffer length */
};

/** Get Sampled Data command structure */
struct ZbZclMeterClientGetSampledDataReqT {
    uint16_t sample_id; /**< SampleID */
    uint32_t earliest_sample_time; /**< EarliestSampleTime */
    enum ZbZclMeterSampleTypeT sample_type; /**< SampleType */
    uint16_t number_of_samples; /**< NumberOfSamples */
};

/** Get Sampled Data Response command structure */
struct ZbZclMeterServerGetSampledDataRspT {
    uint16_t sample_id; /**< SampleID */
    uint32_t sample_start_time; /**< SampleStart Time */
    enum ZbZclMeterSampleTypeT sample_type; /**< SampleType */
    uint16_t sample_request_interval; /**< SampleRequest Interval */
    uint16_t number_of_samples; /**< NumberOfSamples */
    uint8_t *sample_data; /**< Sampled Data buffer */
    unsigned int sample_length; /**< Sampled Data buffer length */
};

enum ZbZclMeterSupplyStatusT {
    ZCL_METER_SUPPLY_STATUS_OFF = 0x00,
    ZCL_METER_SUPPLY_STATUS_OFF_ARMED = 0x01,
    ZCL_METER_SUPPLY_STATUS_ON = 0x02
};

/** Metering Local Change Supply command structure */
struct ZbZclMeterClientLocalChangeSupplyReqT {
    enum ZbZclMeterSupplyStatusT prop_supply_status; /**< Proposed Supply Status */
};

/** MirrorReportAttributeResponse Command */
struct ZbZclMeterClientMirrorReportAttrRspT {
    enum ZbZclMeterNotifSchemesT notif_scheme; /**< Notification Scheme */
    uint8_t num_flags;
    uint32_t notif_flags[ZCL_METER_NOTIF_FLAGS_MAX]; /**< Notification Flags */
};

/** Metering Server callbacks configuration */
struct ZbZclMeterServerCallbacksT {
    enum ZclStatusCodeT (*get_profile)(struct ZbZclClusterT *cluster, void *arg,
        struct ZbZclMeterClientGetProfileReqT *req, struct ZbZclAddrInfoT *srcInfo);
    /**< Configure callback to handle ZCL_METER_CLI_CMD_GET_PROFILE.
     * In handling of get_profile(), the application should call
     * ZbZclMeterServerSendGetProfileRsp to send the response if successful.
     * Return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error.
     */

    enum ZclStatusCodeT (*get_sampled_data)(struct ZbZclClusterT *cluster, void *arg,
        struct ZbZclMeterClientGetSampledDataReqT *req, struct ZbZclAddrInfoT *srcInfo);
    /**< Configure callback to handle ZCL_METER_CLI_CMD_GET_SAMPLED_DATA.
     * In handling of get_sampled_data(), the application should call
     * ZbZclMeterServerSendGetSampledDataRsp to send the response if successful.
     * Return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error.
     */

    enum ZclStatusCodeT (*local_change_supply)(struct ZbZclClusterT *cluster, void *arg,
        struct ZbZclMeterClientLocalChangeSupplyReqT *req, struct ZbZclAddrInfoT *srcInfo);
    /**< Configure callback to handle ZCL_METER_CLI_CMD_LOCAL_CHANGE_SUPPLY.
     * Return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error.
     */

    enum ZclStatusCodeT (*mirror_report_attr_rsp)(struct ZbZclClusterT *cluster, void *arg,
        struct ZbZclMeterClientMirrorReportAttrRspT *notify, struct ZbZclAddrInfoT *srcInfo);
    /**< Configure callback to handle ZCL_METER_CLI_CMD_MIRROR_REPORT_ATTRIBUTE_RESPONSE.
     * Return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error. */

    enum ZclStatusCodeT (*optional)(struct ZbZclClusterT *cluster,
        struct ZbZclHeaderT *zclHdrPtr, struct ZbApsdeDataIndT *dataIndPtr);
    /**< Optional commands not parsed into a struct or specific callback. If this is NULL,
     * the cluster will return a Default Response with status of ZCL_STATUS_UNSUPP_COMMAND.
     * ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error.
     */
};

/**
 * Create a new instance of the Metering Server cluster
 * After initialization, ZbZclClusterSetCallbackArg is called with 'arg' to configure the application's callback argument
 * @param zb Zigbee stack instance
 * @param endpoint Endpoint on which to create cluster
 * @param callbacks Structure containing any callback function pointers for this cluster
 * @param arg Pointer to application data that will later be provided back to the callback functions when invoked
 * @return Cluster pointer, or NULL if there is an error
 */
struct ZbZclClusterT * ZbZclMeterServerAlloc(struct ZigBeeT *zb, uint8_t endpoint,
    struct ZbZclMeterServerCallbacksT *callbacks, void *arg);

/**
 * Send a Request Mirror Command (ZCL_METER_SVR_CMD_REQUEST_MIRROR)
 * @param cluster Cluster instance from which to send this command
 * @param dst Destination of command. Should be the Coordinator / TC / Comms Hub in this case.
 * @param callback Callback function that will be invoked later when the response is received
 * @param arg Pointer to application data that will later be provided back to the callback function when invoked
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT ZbZclMeterServerSendRequestMirror(struct ZbZclClusterT *cluster, struct ZbApsAddrT *dst,
    void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

/**
 * Send a Remove Mirror Command (ZCL_METER_SVR_CMD_REMOVE_MIRROR)
 * @param cluster Cluster instance from which to send this command
 * @param dst Destination of command. Should be the Coordinator / TC / Comms Hub in this case.
 * @param callback Callback function that will be invoked later when the response is received
 * @param arg Pointer to application data that will later be provided back to the callback function when invoked
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT ZbZclMeterServerSendRemoveMirror(struct ZbZclClusterT *cluster, struct ZbApsAddrT *dst,
    void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

/**
 * Send a ConfigureMirror Command (ZCL_METER_SVR_CMD_CONFIGURE_MIRROR)
 * @param cluster Cluster instance from which to send this command
 * @param dst Destination of command. Should be the Coordinator / TC / Comms Hub in this case.
 * @param req ConfigureMirror parameters
 * @param callback Callback function that will be invoked later when the response is received
 * @param arg Pointer to application data that will later be provided back to the callback function when invoked
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT ZbZclMeterServerSendConfigMirror(struct ZbZclClusterT *cluster, struct ZbApsAddrT *dst,
    struct ZbZclMeterServerConfigMirrorT *req,
    void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

/**
 * Convert the profile interval period enumerated value to a time in seconds
 * @param profile_interval_id Zigbee stack instance
 * @return Converted time in seconds or -1 on error
 */
int ZbZclMeterGetProfileIntervalPeriod(uint8_t profile_interval_id);

/**
 * Send a Get Profile Response command
 * @param cluster Cluster instance from which to send this command
 * @param dst Destination address for response, including sequence number and tx options
 * @param rsp Get Profile Response command structure
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT ZbZclMeterServerSendGetProfileRsp(struct ZbZclClusterT *cluster,
    struct ZbZclAddrInfoT *dst, struct ZbZclMeterServerGetProfileRspT *rsp);

/**
 * Convert an array of 24-bit integers to the Zigbee frame format
 * @param sample_data Converted data for Zigbee frame
 * @param max_len Maximum length
 * @param samples Array of 24-bit integers to convert
 * @param num_samples Number of integers to convert
 * @return Returns the number of octets written to sample_data or -1 on error
 */
int ZbZclMeterFormSampledData(uint8_t *sample_data, unsigned int max_len, uint32_t *samples, uint16_t num_samples);

/**
 * Send a Get Sampled Data Response command
 * @param cluster Cluster instance from which to send this command
 * @param dst Destination address for response, including sequence number and tx options
 * @param rsp Get Sampled Data Response command structure
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT ZbZclMeterServerSendGetSampledDataRsp(struct ZbZclClusterT *cluster,
    struct ZbZclAddrInfoT *dst, struct ZbZclMeterServerGetSampledDataRspT *rsp);

/** Metering Server Mirror Functions */

/**
 * Create a new instance of the Metering Server Mirror cluster
 * @param zb Zigbee stack instance
 * @param endpoint Endpoint on which to create cluster
 * @param mirrorAddr Address of the Server Mirror
 * @param mirrorEndpoint Endpoint on which to create Mirror cluster
 * @return Cluster pointer, or NULL if there is an error
 */
struct ZbZclClusterT * ZbZclMeterServerMirrorAlloc(struct ZigBeeT *zb, uint8_t endpoint,
    uint64_t mirrorAddr, uint8_t mirrorEndpt);

/* Metering Client */

/** Metering Client callbacks configuration */
struct ZbZclMeterClientCallbacksT {
    uint16_t (*request_mirror)(struct ZbZclClusterT *cluster, struct ZbZclAddrInfoT *srcInfo, void *arg);
    /**< Callback to handle a ZCL_METER_SVR_CMD_REQUEST_MIRROR command.
     * Callback will create a Meter Server cluster to act as the mirror (ZbZclMeterServerAlloc).
     * It should also call ZbZclMeterClientMirrorRegister to register the Meter Server with the
     * client so any ZCL Reports received from the meter will automatically update the local
     * Meter Server mirror.
     *
     * Return value is the endpoint ID, which is typically the same endpoint this local
     * Meter Client cluster is operating on (ZbZclClusterGetEndpoint), or
     * ZCL_METER_MIRROR_INVALID_ENDPOINT (0xffff) on error.
     */

    uint16_t (*remove_mirror)(struct ZbZclClusterT *cluster, struct ZbZclAddrInfoT *srcInfo, void *arg);
    /**< Callback to handle a ZCL_METER_SVR_CMD_REMOVE_MIRROR command.
     * Callback will delete the Meter Server cluster that is acting as a mirror (ZbZclClusterFree).
     * If the Meter Server was previously registered through ZbZclMeterClientMirrorRegister,
     * this registration will be removed if a valid endpoint ID is returned.
     *
     * Return value is the endpoint ID the Mete Server was operating on, or
     * ZCL_METER_MIRROR_INVALID_ENDPOINT (0xffff) on error.
     */

    enum ZclStatusCodeT (*config_mirror)(struct ZbZclClusterT *cluster,
        struct ZbZclMeterServerConfigMirrorT *req,
        struct ZbZclAddrInfoT *srcInfo, void *arg);
    /**< Callback to handle a ZCL_METER_SVR_CMD_CONFIGURE_MIRROR command. */

#if 0
    /* Not certifiable (and not supported) */
    /* enum ZclStatusCodeT (*config_notif_scheme)(struct ZbZclClusterT *cluster,
          struct ZbZclMeterServerConfigNotifSchemeT *req,
          struct ZbZclAddrInfoT *srcInfo, void *arg); */

    /* enum ZclStatusCodeT (*config_notif_flags)(struct ZbZclClusterT *cluster,
        struct ZbZclMeterServerConfigNotifFlagsT *req,
        struct ZbZclAddrInfoT *srcInfo, void *arg); */
#endif

    enum ZclStatusCodeT (*optional)(struct ZbZclClusterT *cluster,
        struct ZbZclHeaderT *zclHdrPtr, struct ZbApsdeDataIndT *dataIndPtr);
    /**< Optional commands not parsed into a struct or specific callback. If this is NULL,
     * the cluster will return a Default Response with status of ZCL_STATUS_UNSUPP_COMMAND.
     * ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error.
     */
};

/**
 * Create a new instance of the Metering Client cluster
 * @param zb Zigbee stack instance
 * @param endpoint Endpoint on which to create cluster
 * @param callbacks Structure containing any callback function pointers for this cluster
 * @param arg Pointer to application data that will later be provided back to the callback functions when invoked
 * @return Cluster pointer, or NULL if there is an error
 */
struct ZbZclClusterT * ZbZclMeterClientAlloc(struct ZigBeeT *zb, uint8_t endpoint,
    struct ZbZclMeterClientCallbacksT *callbacks, void *arg);

/**
 * Register a Meter Server (Mirror) with this client, so any ZCL Attribute Report commands
 * will update our local Meter Server.
 * @param cluster Meter Client instance
 * @param server Meter Server (Mirror) instance
 * @param rmt_addr The extended address of the remote Meter Server (real meter)
 * @param rmt_endpoint The source endpoint of the remote Meter Server (real meter)
 * @return None
 */
void ZbZclMeterClientMirrorRegister(struct ZbZclClusterT *cluster, struct ZbZclClusterT *server,
    uint64_t rmt_addr, uint8_t rmt_endpoint);

/**
 * Send a Get Profile command
 * @param cluster Cluster instance from which to send this command
 * @param dst Destination address for response, including sequence number and tx options
 * @param cmd_req Get Profile command structure
 * @param callback Callback function that will be invoked later when the response is received
 * @param arg Pointer to application data that will later be provided back to the callback function when invoked
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT ZbZclMeterClientCommandGetProfileReq(struct ZbZclClusterT *cluster,
    const struct ZbApsAddrT *dst, struct ZbZclMeterClientGetProfileReqT *cmd_req,
    void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

/**
 * Send a Get Sampled Data command
 * @param cluster Cluster instance from which to send this command
 * @param dst Destination address for response, including sequence number and tx options
 * @param cmd_req Get Sampled Data command structure
 * @param callback Callback function that will be invoked later when the response is received
 * @param arg Pointer to application data that will later be provided back to the callback function when invoked
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT ZbZclMeterClientCommandGetSampledDataReq(struct ZbZclClusterT *cluster,
    const struct ZbApsAddrT *dst, struct ZbZclMeterClientGetSampledDataReqT *cmd_req,
    void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

/**
 * Send a Local Change Supply command
 * @param cluster Cluster instance from which to send this command
 * @param dst Destination address for response, including sequence number and tx options
 * @param cmd_req Get Profile command structure
 * @param callback Callback function that will be invoked later when the response is received
 * @param arg Pointer to application data that will later be provided back to the callback function when invoked
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT ZbZclMeterClientCommandLocalChangeSupplyReq(struct ZbZclClusterT *cluster,
    const struct ZbApsAddrT *dst, struct ZbZclMeterClientLocalChangeSupplyReqT *cmd_req,
    void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

/*lint -restore */

#endif
