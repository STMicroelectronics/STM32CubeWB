/* Copyright [2015 - 2023] Exegin Technologies Limited. All rights reserved. */

#ifndef MCP_ENUMS_H_
# define MCP_ENUMS_H_

/* *INDENT-OFF* */

/*lint -save -e726 [!MISRA - extra comma ignored (end of enum)] */
/* IEEE 802.15.4 Status Values */
enum mcp_status {
    mcp_STATUS_SUCCESS                      = 0x00,
    mcp_STATUS_BEACON_LOSS                  = 0xe0,
    mcp_STATUS_CHANNEL_ACCESS_FAILURE       = 0xe1,
    mcp_STATUS_COUNTER_ERROR                = 0xdb,
    mcp_STATUS_DENIED                       = 0xe2,
    mcp_STATUS_DISABLE_TRX_FAILURE          = 0xe3,
    mcp_STATUS_FRAME_TOO_LONG               = 0xe5,
    mcp_STATUS_IMPROPER_KEY_TYPE            = 0xdc,
    mcp_STATUS_IMPROPER_SECURITY_LEVEL      = 0xdd,
    mcp_STATUS_INVALID_ADDRESS              = 0xf5,
    mcp_STATUS_INVALID_GTS                  = 0xe6,
    mcp_STATUS_INVALID_HANDLE               = 0xe7,
    mcp_STATUS_INVALID_INDEX                = 0xf9,
    mcp_STATUS_INVALID_PARAMETER            = 0xe8,
    mcp_STATUS_LIMIT_REACHED                = 0xfa,
    mcp_STATUS_NO_ACK                       = 0xe9,
    mcp_STATUS_NO_BEACON                    = 0xea,
    mcp_STATUS_NO_DATA                      = 0xeb,
    mcp_STATUS_NO_SHORT_ADDRESS             = 0xec,
    mcp_STATUS_ON_TIME_TOO_LONG             = 0xf6,
    mcp_STATUS_OUT_OF_CAP                   = 0xed,
    mcp_STATUS_PAN_ID_CONFLICT              = 0xee,
    mcp_STATUS_PAST_TIME                    = 0xf7,
    mcp_STATUS_READ_ONLY                    = 0xfb,
    mcp_STATUS_REALIGNMENT                  = 0xef,
    mcp_STATUS_SCAN_IN_PROGRESS             = 0xfc,
    mcp_STATUS_SECURITY_ERROR               = 0xe4,
    mcp_STATUS_SUPERFRAME_OVERLAP           = 0xfd,
    mcp_STATUS_TRACKING_OFF                 = 0xf8,
    mcp_STATUS_TRANSACTION_EXPIRED          = 0xf0,
    mcp_STATUS_TRANSACTION_OVERFLOW         = 0xf1,
    mcp_STATUS_TX_ACTIVE                    = 0xf2,
    mcp_STATUS_UNAVAILABLE_KEY              = 0xf3,
    mcp_STATUS_UNSUPPORTED_ATTRIBUTE        = 0xf4,
    mcp_STATUS_UNSUPPORTED_LEGACY           = 0xde,
    mcp_STATUS_UNSUPPORTED_SECURITY         = 0xdf,
    mcp_STATUS_DRIVER_ERROR                 = 0xfe,
    mcp_STATUS_DEVICE_ERROR                 = 0xff,
};

enum {
    mcp_EVENT_NONE                          = 0x00,
    mcp_EVENT_ASN_NOTIFY_TIME               = 0x01,
    mcp_EVENT_LINK_EXPIRED                  = 0x02,
    mcp_EVENT_KEY_STALE                     = 0x03,
    mcp_EVENT_ACTIVE_KEY_CHANGED            = 0x04,
    mcp_EVENT_UNDEFINED                     = 0xff,
};

enum mcp_attr_id {
    /* PHY PIB Attributes. */
    mcp_phyCurrentChannel                   = 0x00,
    mcp_phyChannelsSupported                = 0x01,
    mcp_phyTransmitPower                    = 0x02,
    mcp_phyCcaMode                          = 0x03,
    mcp_phyCurrentPage                      = 0x04,
    mcp_phyMaxFrameDuration                 = 0x05,
    mcp_phyShrDuration                      = 0x06,
    mcp_phySymbolsPerOctet                  = 0x07,
    mcp_aMaxPhyPacketSize                   = 0x08,
    mcp_phyCcaThreshold                     = 0x09,
    mcp_phyCcaThresholdAbs                  = 0x0a,
    mcp_phyRadioSensitivity                 = 0x0b,

    /* MAC PIB Attributes.  */
    mcp_macAckWaitDuration                  = 0x40,
    mcp_macAssociatedPanCoord               = 0x56,
    mcp_macAssociationPermit                = 0x41,
    mcp_macAutoRequest                      = 0x42,
    mcp_macBattLifeExt                      = 0x43,
    mcp_macBattLifeExtPeriods               = 0x44,
    mcp_macBeaconPayload                    = 0x45,
    mcp_macBeaconPayloadLength              = 0x46,
    mcp_macBeaconOrder                      = 0x47,
    mcp_macBeaconTxTime                     = 0x48,
    mcp_macBsn                              = 0x49,
    mcp_macCoordExtendedAddress             = 0x4a,
    mcp_macCoordShortAddress                = 0x4b,
    mcp_macDsn                              = 0x4c,
    mcp_macGtsPermit                        = 0x4d,
    mcp_macMaxBE                            = 0x57,
    mcp_macMaxCsmaBackoffs                  = 0x4e,
    mcp_macMaxFrameTotalWaitTime            = 0x58,
    mcp_macMaxFrameRetries                  = 0x59,
    mcp_macMinBE                            = 0x4f,

    mcp_macPanId                            = 0x50,
    mcp_macPromiscuousMode                  = 0x51,
    mcp_macResponseWaitTime                 = 0x5a,
    mcp_macRxOnWhenIdle                     = 0x52,
    mcp_macSecurityEnabled                  = 0x5d,
    mcp_macShortAddress                     = 0x53,
    mcp_macExtendedAddress                  = 0x63, /* Read/Write - used to get/set the EUI-64 address of the device. */
    mcp_macSuperframeOrder                  = 0x54,
    mcp_macSyncSymbolOffset                 = 0x5b,
    mcp_macTimestampSupported               = 0x5c,
    mcp_macTransactionPersistenceTime       = 0x55,

    /* Source address whitelist lengths. */
    mcp_macAcceptSourceAddr64Entries        = 0x5e, /* Read/Write - Number of EUI-64 entries in the whitelist table. */
    mcp_macAcceptSourceAddr16Entries        = 0x5f, /* Read/Write - Number of short addr entries in the whitelist table. */

    /* Exegin Add-ons. */
    mcp_phySymbolRate                       = 0x60, /* Read only - rate in symbols/seconds of current phy. */
    mcp_phyClearChannel                     = 0x61, /* Read only - reading will trigger a CCA measurement (ZigBee testing). */
    mcp_phyEnergyDetect                     = 0x62, /* Read only - reading will trigger a phy ED measurement (ZigBee testing). */
    mcp_macPromiscuousAutoFcs               = 0x64, /* Optional - specifies if the FCS is checked when in promiscuous mode. */
    mcp_phyTestModulated                    = 0x65, /* Optional - Transmits a modulated carrier (FCC testing). */
    mcp_phyTestUnmodulated                  = 0x66, /* Optional - Transmits an unmodulated carrier (FCC testing). */
    mcp_phyTestClockOut                     = 0x67, /* Optional - Enables clock output to GPIO (Oscillator tuning). */
    mcp_macStatistics                       = 0x68, /* Read only - returns the mcp_statistics structure (if supported). */
    mcp_macKeySwitchIndex                   = 0x69, /* Read/Write - used by ZigBee-IP to trigger a key switch (i.e. receive a packet with a matching key ID). */
    mcp_phySendRaw                          = 0x6a, /* Write only - transmits a raw PSDU on attribute write. */
    mcp_macHardwareInd                      = 0x6b, /* Read/Write - boolean to toggle a hardware indication. */
    mcp_phyStatistics                       = 0x6c, /* Read only - returns an mcp_counters structure (if supported). */
    mcp_macLicenseCert                      = 0x6d, /* Read only - The license certificate. */
    mcp_phyHardwareRand                     = 0x6e, /* Read only - Retrieve a randomly generated byte from hardware (eg: RSSI noise). */
    mcp_phyPagesSupported                   = 0x6f, /* Read only - a 32bit mask of channel-pages supported. */
    mcp_macASNNotifyTime                    = 0x70, /* Read/Write - ASN Time for MLME-EVENT.indication */

    /* Security PIB Attributes */
    mcp_macKeyTable                         = 0x71,
    mcp_macKeyTableEntries                  = 0x72,
    mcp_macDeviceTable                      = 0x73,
    mcp_macDeviceTableEntries               = 0x74,
    mcp_macSecurityLevelTable               = 0x75,
    mcp_macSecurityLevelTableEntries        = 0x76,
    mcp_macFrameCounter                     = 0x77,
    mcp_macAutoRequestSecurityLevel         = 0x78,
    mcp_macAutoRequestKeyIdMode             = 0x79,
    mcp_macAutoRequestKeySource             = 0x7a,
    mcp_macAutoRequestKeyIndex              = 0x7b,
    mcp_macDefaultKeySource                 = 0x7c,

    /* Source address whitelist entries. */
    mcp_macAcceptSourceAddr64               = 0x7d, /* Read/Write - Get or set EUI-64 entries in the whitelist table. */
    mcp_macAcceptSourceAddr16               = 0x7e, /* Read/Write - Get or set short addr entries in the whitelist table. */

    /* TX duty-cycle attributes */
    mcp_macTxTotalDuration                  = 0x7f, /* Total number of transmitted symbols */
    mcp_macTxControlActiveDuration          = 0x80, /* Active phase of TX duty cycle in milliseconds */
    mcp_macTxControlPauseDuration           = 0x81, /* Paused phase of TX duty cycle in milliseconds */

    mcp_macPacketDiagInfoEnable             = 0x82, /* Controls if Diagnostic TLVs are added to packet indication messages */
    mcp_macTapType                          = 0x83, /* Type of data sent to the TAP interface */

    mcp_enhancedBeaconIEs                   = 0x0101, /* HIEs, MIEs, and PIEs to include in enhanced beacon */

    mcp_macImplicitBroadcast                = 0x0102,
    mcp_macGlobalTime                       = 0x0103, /* For Global Time IE, in seconds */
    mcp_macBeaconsScheduled                 = 0x0104, /* Count of beacons scheduled, not necessarily sent; zeroed on read. */

    mcp_macEnhancedBeaconSecurityLevel      = 0x0105, /* Security setting for transmitting enhanced beacons */
    mcp_macEnhancedBeaconKeyIdMode          = 0x0106,
    mcp_macEnhancedBeaconKeySource          = 0x0107,
    mcp_macEnhancedBeaconKeyIndex           = 0x0108,

    /* Custom Default Channel feature. */
    mcp_macRxOnIdleChannel                  = 0x0109,
    mcp_macRxOnIdleChannelEnable            = 0x010a,

    /* SUN PHY PIB Attributes. */
    mcp_phyFSKFEC                           = 0x0200,
    mcp_phyFSKFECInterleavingRSC            = 0x0201,
    mcp_phyFSKFECScheme                     = 0x0202,
    mcp_phySunChannelsSupported             = 0x0203,
    mcp_phyMaxSunChannelSupported           = 0x0204,
    mcp_phyNumSunPageEntriesSupported       = 0x0205,
    mcp_phySunPageEntriesSupported          = 0x0206,
    mcp_phyCurrentSunPageEntry              = 0x0207, /* struct mcp_sunpage entry */
    mcp_phySunNumGenericPHYDescriptors      = 0x0208,
    mcp_phySunGenericPHYDescriptors         = 0x0209,
    mcp_phyNumModeSwitchParameterEntries    = 0x020a,
    mcp_phyModeSwitchParameterEntries       = 0x020b,
    mcp_phyFSKPreambleLength                = 0x020c,
    mcp_phyMRFSKSFD                         = 0x020d,
    mcp_phyFSKScramblePSDU                  = 0x020e, /* Data whitening enable/disable */
    mcp_phyOFDMInterleaving                 = 0x020f,
    mcp_phyOFDMScrambleSeed                 = 0x0210, /* Absent in 802.15.4g spec, but still needed. */
    mcp_phyPHRDuration                      = 0x0211,
    mcp_phyChannelPlan                      = 0x0212,

    /* WiSUN PHY PIB Attributes */
    mcp_phyWisunRegion                      = 0x0220,
    mcp_phyWisunOperatingMode               = 0x0221,
    mcp_phyWisunChannelPlanDesc             = 0x0222, /* Wi-SUN FAN 1.1 */
    mcp_phyWisunNumModeSwitchEntries        = 0x0223,

    /* Exegin PHY Add-ons. */
    mcp_phyFSKDW                            = 0x0280, /* Deprecated */
    mcp_phyFreqBandSupported                = 0x0281, /* uint16_t "enum mcp_freq_band" */
    mcp_phyTxPowerReg                       = 0x0282, /* uint32_t - For TX power profiling, write to the radio's register directly. */

    /* SUN MAC PIB Attributes. */
    mcp_macEnhancedBeaconOrder              = 0x0300,
    mcp_macMPMIE                            = 0x0301,
    mcp_macNBPANEnhancedBeaconOrder         = 0x0302,
    mcp_macOffsetTimeSlot                   = 0x0303,
    mcp_macFCSType                          = 0x0304,

    /* TSCH MAC PIB Attributes */
    mcp_macTschEnabled                      = 0x0400,
    mcp_macDisconnectTime                   = 0x0401,
    mcp_macJoinMetric                       = 0x0402,
    mcp_macASN                              = 0x0403,
    mcp_macSlotframeTable                   = 0x0404,
    mcp_macSlotframeEntries                 = 0x0405,
    mcp_macLinkTable                        = 0x0406,
    mcp_macLinkEntries                      = 0x0407,

    /* WiSUN MAC PIB Attributes */
    mcp_macWisunEnabled                     = 0x0440,
    mcp_macWisunPanVersion                  = 0x0441,
    mcp_macWisunPanSize                     = 0x0442,
    mcp_macWisunRoutingCost                 = 0x0443,
    mcp_macWisunUseParentBSIE               = 0x0444,
    mcp_macWisunParentAddress               = 0x0445,
    mcp_macWisunGTKHash                     = 0x0446,
    mcp_macWisunParentBlacklist             = 0x0447,
    mcp_macWisunPasImin                     = 0x0448,   /* PAN Advertise Solicit trickle timer minimum interval, in seconds */
    mcp_macWisunPasImax                     = 0x0449,   /* PAN Advertise Solicit trickle timer maximum interval size, in #doublings */
    mcp_macWisunPasK                        = 0x044a,   /* PAN Advertise Solicit trickle timer redundancy constant */
    mcp_macWisunPaImin                      = 0x044b,   /* PAN Advertise trickle timer minimum interval, in seconds */
    mcp_macWisunPaImax                      = 0x044c,   /* PAN Advertise trickle timer maximum interval size, in # doublings */
    mcp_macWisunPaK                         = 0x044d,   /* PAN Advertise trickle timer redundancy constant */
    mcp_macWisunPcsImin                     = 0x044e,   /* PAN Configuration Solicit trickle timer minimum interval, in seconds */
    mcp_macWisunPcsImax                     = 0x044f,   /* PAN Configuration Solicit trickle timer maximum interval size, in # doublings */
    mcp_macWisunPcsK                        = 0x0450,   /* PAN Configuration Solicit trickle timer redundancy constant */
    mcp_macWisunPcImin                      = 0x0451,   /* PAN Configuration trickle timer minimum interval, in seconds */
    mcp_macWisunPcImax                      = 0x0452,   /* PAN Configuration trickle timer maximum interval size, in # doublings */
    mcp_macWisunPcK                         = 0x0453,   /* PAN Configuration trickle timer redundancy constant */
    mcp_macWisunPcsAttempts                 = 0x0454,   /* PAN Configuration Solicit Attempts */
    mcp_macWisunExcludedChannelBitmask      = 0x0455,   /* Excluded Channels as array of 8 64-bit masks */
    mcp_macWisunRoutingLimit                = 0x0456,   /* Limit how large the routing cost can become, as 32-bit int. */
    mcp_macWisunMeshId                      = 0x0457,   /* Vendor Specific Mesh Id (RPL DODAG) for network steering. */
    mcp_macWisunLFNWindowStyle              = 0x0458,   /* Wi-SUN FAN 1.1 */
    mcp_macWisunLFNDiscoverySlots           = 0x0459,   /* Wi-SUN FAN 1.1 - x DST (macHopDwellTime) for LFN Discovery Response Period */
    mcp_macWisunLFNListenInterval           = 0x045a,   /* Wi-SUN FAN 1.1 */
    mcp_macWisunNbrTable                    = 0x045b,   /* Information on a Wi-SUN neighbor */
    mcp_macWisunNbrEntries                  = 0x045c,   /* Number of Wi-SUN neighbor entries */
    mcp_macWisunJoinMetrics                 = 0x045d,   /* Information in the Join Metrics IE */

    /* Hopping Sequence PIB Attributes */
    mcp_macHoppingSequenceID                = 0x0480,
    mcp_macHoppingChannelPage               = 0x0481,   /* Called macChannelPage in IEEE 802.15.4-2015, confusing name. */
    mcp_macHoppingChannelList               = 0x0482,
    mcp_macHoppingChannelCount              = 0x0483,
    mcp_macHoppingSequenceLength            = 0x0484,
    mcp_macHoppingSequenceList              = 0x0485,
    mcp_macHopDwellTime                     = 0x0486,
    mcp_macTimeslotTemplateId               = 0x0487,
    mcp_macTsCcaOffset                      = 0x0488,
    mcp_macTsCca                            = 0x0489,
    mcp_macTsTxOffset                       = 0x048A,
    mcp_macTsRxOffset                       = 0x048B,
    mcp_macTsRxAckDelay                     = 0x048C,
    mcp_macTsTxAckDelay                     = 0x048D,
    mcp_macTsRxWait                         = 0x048E,
    mcp_macTsRxTx                           = 0x048F,
    mcp_macTsMaxAck                         = 0x0490,
    mcp_macTsMaxTx                          = 0x0491,
    mcp_macTsTimeslotLength                 = 0x0492,
    mcp_macTsAckWait                        = 0x0493,
    mcp_macExtendedBitmap                   = 0x0494,
    mcp_macNumberOfChannels                 = 0x0495,
    mcp_macBcastHopDwellTime                = 0x0496,
    mcp_macBcastHopInterval                 = 0x0497,
    mcp_macBcastHopId                       = 0x0498,
    mcp_macLFNBcastHopInterval              = 0x0499,
    mcp_macLFNBcastHopId                    = 0x049a,
    mcp_macLFNBcastSyncPeriod               = 0x049b,
    mcp_macNetworkExcludedChannels          = 0x049c,   /* Network-Wide Excluded Channels as array of 8 64-bit masks */

    /* Zigbee R22+ */
    mcp_macDCCurrentMeas                    = 0x0500, /* Current Duty Cycle over the last hour (hundredths of %) */
    mcp_macDCCurrentBytes                   = 0x0501, /* Current Duty Cycle over the last hour (bytes) */
    mcp_macJoinList                         = 0x0502, /* IEEE Joining List (used if mibJoiningPolicy == IEEELIST_JOIN) */
    mcp_macDCAccelerated                    = 0x0503, /* Governs whether the Duty Cycle timing is accelerated (1) or normal (0) */
    mcp_macLBTTxRetries                     = 0x0504,
    mcp_macTxPowerMgmtSupported             = 0x0505,
    mcp_macDutyCycleSupported               = 0x0506,
    mcp_macDiagClusterInfo                  = 0x0507, /* ZCL 7 Diagnostics Cluster Info */
    mcp_macZdRemoteEui64                    = 0x0508, /* Zigbee Direct EUI-64 of remote connected device */
    mcp_macSimDeafTxDiv                     = 0x0509, /* Control simulating deafness during Tx backoff (MAC CSMA-CA) */
    mcp_macZdSecureSession                  = 0x050a, /* Zigbee Direct BLE secure session currently setup between ZDD & ZVD. */
    mcp_macMaxCSMARetries                   = 0x050b,
    /**< Used if the MAC has decoupled CSMA retries from mcp_macMaxFrameRetries. It defines the number
     * of retries the MAC should perform for a CSMA-CA failure, which is after mcp_macMaxCsmaBackoffs
     * and CCA attempts. Normally, we just combine CSMA-CA failures and missed ACK failures, and use a
     * single counter for both, with the upper limit set to mcp_macMaxFrameRetries. */

    /* Note, large gap of IDs until 0x40a0. */

    /* Enhanced Beacon Request command PIB Attributes. */
    mcp_macEbrPermitJoining                 = 0x40A0,
    mcp_macEbrFilters                       = 0x40A1,
    mcp_macEbrLinkQuality                   = 0x40A2,
    mcp_macEbrPercentFilter                 = 0x40A3,
    mcp_macEbrAttributeList                 = 0x40A4,
    mcp_macBeaconAutoRespond                = 0x40A5,

    /* Beacon Appendix Length. It is the length of the beacon appendix, if any, within the
     * Beacon Payload. It is always the trailing part of the Beacon Payload and must be less
     * than or equal to mcp_macBeaconPayloadLength. */
    mcp_macBeaconAppendixLength             = 0x40A6,

    /*
     * IDs in this range are reserved for customers to add features
     * specific to their implementation.
     */
    mcp_customerFeatureStart                = 0xF000,
    mcp_customerFeatureEnd                  = 0xF7FF
};

/* Frequency Bands */
enum mcp_freq_band {
    mcp_169MHz_band     = 0,    /* Europe */
    mcp_450MHz_band     = 1,    /* US FCC Part 22/90 */
    mcp_470MHz_band     = 2,    /* China */
    mcp_780MHz_band     = 3,    /* China */
    mcp_863MHz_band     = 4,    /* Europe */
    mcp_896MHz_band     = 5,    /* US FCC Part 90 */
    mcp_901MHz_band     = 6,    /* US FCC Part 24 */
    mcp_915MHz_band     = 7,    /* US */
    mcp_917MHz_band     = 8,    /* Korea */
    mcp_920MHz_band     = 9,    /* Japan */
    mcp_928MHz_band     = 10,   /* US, non-contiguous */
    mcp_950MHz_band     = 11,   /* Was in IEEE 802.15.4g; removed in 802.15.4-2015. Overlaps the 928MHz band. */
    mcp_1427MHz_band    = 12,   /* US and Canada, non-contiguous */
    mcp_2450MHz_band    = 13,   /* US and Canada */
    /* Band added by IEEE 802.15.4u-2016 */
    mcp_866MHz_band     = 14,   /* India 865-867 */
    /* Bands added by IEEE 802.15.4v-2017 */
    mcp_870MHz_band     = 15,   /* Europe 870-876*/
    /* 16 to 23 are 915MHz sub-bands using 902-928MHz channel numbering */
    mcp_915MHz_a_band   = 16,   /* North America and Mexico 902-928 (alternate) */
    mcp_915MHz_b_band   = 17,   /* Brazil. Non-contiguous 902-907.5 & 915-928 MHz */
    mcp_915MHz_c_band   = 18,   /* Australia and New Zealand 915-928 */
    mcp_915MHz_d_band   = 19,   /* Europe 915-921*/
    mcp_915MHz_e_band   = 20,   /* Philippines 915-918 */
    mcp_919MHz_band     = 21,   /* Malaysia 919-923 */
    mcp_920MHz_a_band   = 22,   /* China 920.5-924.5 */
    mcp_920MHz_b_band   = 23,   /* HK, Singapore, Thailand, and Vietnam 920-925 */
    /* 863MHz sub-band using 863-870MHz channel numbering */
    mcp_867MHz_band     = 24,   /* 866-869 */
    /* Bands listed in IEEE 802-15.4-2015+revisions Table 10-1 Frequency band designations,
     * but not assigned a bit in Table 7-19 Frequency band identifier values */
    mcp_433MHz_band     = 32,
    mcp_868MHz_band     = 32+1,
    mcp_2380MHz_band    = 32+2,
};

/* Defined Channel Pages */
enum {
    mcp_PAGE_DEFAULT    = 0,    /* Default Channel Page (i.e. legacy 802.15.4 mode) */
    mcp_PAGE_ASK        = 1,    /* 868/915MHz ASK. */
    mcp_PAGE_OQPSK      = 2,    /* 868/915MHz O-QPSK. */
    mcp_PAGE_CSS        = 3,    /* CSS */
    mcp_PAGE_HRP_UWB    = 4,    /* HRP UWB */
    mcp_PAGE_780_MHZ    = 5,    /* 780 MHz Band (not SUN, RCC or LECIM) previously MPSK */
    mcp_PAGE_GFSK       = 6,    /* 920.8-928 MHz */
    mcp_PAGE_MSK        = 7,    /* 433MHz/2450MHz MSK */
    mcp_PAGE_LRP_UWB    = 8,    /* LRP UWB */
    mcp_PAGE_SUN        = 9,    /* SUN PHYs */
    mcp_PAGE_SUN_FSK    = 10,   /* SUN FSK Generic PHY */
    mcp_PAGE_2380_MHZ   = 11,   /* 2380 MHz band */
    mcp_PAGE_LECIM      = 12,   /* LECIM PHY */
    mcp_PAGE_RCC        = 13,   /* RCC PHY */
};

/* SUN PHY Types */
enum {
    mcp_FFSK_A          = 0,    /* Filtered FSK-A */
    mcp_FFSK_B          = 1,    /* Filtered FSK-B */
    mcp_OQPSK_A         = 2,    /* O-QPSK-A */
    mcp_OQPSK_B         = 3,    /* O-QPSK-B */
    mcp_OQPSK_C         = 4,    /* O-QPSK-C */
    mcp_OFDM_OPT1       = 5,    /* OFDM Option 1 */
    mcp_OFDM_OPT2       = 6,    /* OFDM Option 2 */
    mcp_OFDM_OPT3       = 7,    /* OFDM Option 3 */
    mcp_OFDM_OPT4       = 8,    /* OFDM Option 4 */

    /* Legacy (802.15.4-2011) PHY types mapped to . */
    mcp_OQPSK_L         = 256,  /* Legacy O-QPSK */
    mcp_BPSK_L          = 257,  /* Legacy BPSK */
};

/* PHR Types */
enum {
    mcp_PHR_RAW             = 0U,
    mcp_PHR_O_QPSK          = 1U,
    mcp_PHR_CSS             = 2U,
    mcp_PHR_HRP_UWB         = 3U,
    mcp_PHR_MSK             = 4U,
    mcp_PHR_LRP_UWB         = 5U,
    mcp_PHR_SUN_FSK         = 6U,
    mcp_PHR_SUN_OFDM        = 7U,
    mcp_PHR_SUN_O_QPSK      = 8U,
    mcp_PHR_LECIM_FSK       = 9U,
    mcp_PHR_TVWS_FSK        = 10U,
    mcp_PHR_TVWS_OFDM       = 11U,
    mcp_PHR_TVWS_NB_OFDM    = 12U,
    mcp_PHR_RCC_LMR         = 13U,
    mcp_PHR_CMB_O_QPSK      = 14U,
    mcp_PHR_CMB_GFSK        = 15U,
    mcp_PHR_TASK            = 16U,
    mcp_PHR_RS_GFSK         = 17U,
    mcp_PHR_WISUN_FSK_MS    = 18U,
};

/* FCS Type (mcp_macFCSType). */
enum {
    mcp_FCS_TYPE_LONG      = 0,     /* 4 byte FCS is automatically generated by the PHY. */
    mcp_FCS_TYPE_SHORT     = 1,     /* 2 byte FCS is automatically generated by the PHY. */
    mcp_FCS_TYPE_LONG_RAW  = 0x80,  /* 4 byte FCS is not generated by the PHY, but the PHR is modified appropriately. */
    mcp_FCS_TYPE_SHORT_RAW = 0x81,  /* 2 byte FCS is not generated by the PHY, but the PHR is modified appropriately. */
};

/* TAP Types */
enum {
    mcp_TAP_NONE                    = 0U,
    mcp_TAP_RAW_RX_TX               = 1U, /* RX and TX frames. */
    mcp_TAP_FILTERED_UNSECURED_RX   = 2U, /* For Wi-SUN TBU frame subscription. */
};

/* TSCH specific definitions. */

/* Header IE types. */
enum {
    mcp_IEEE802154_HIE_VENDOR_SPECIFIC = 0x00,
    mcp_IEEE802154_HIE_UNMANAGED_MIN = 0x01,
    mcp_IEEE802154_HIE_UNMANAGED_MAX = 0x19,
    mcp_IEEE802154_HIE_LE_CSL = 0x1a,
    mcp_IEEE802154_HIE_LE_RIT = 0x1b,
    mcp_IEEE802154_HIE_DSME_PAN_DESC = 0x1c,
    mcp_IEEE802154_HIE_RZ_TIME = 0x1d,
    mcp_IEEE802154_HIE_ACK_TIME_CORR = 0x1e,
    mcp_IEEE802154_HIE_GROUP_ACK = 0x1f,
    mcp_IEEE802154_HIE_LLDN_INFO = 0x20,
    mcp_IEEE802154_HIE_SIMPLIFIED_SUPERFRAME = 0x23,
    mcp_IEEE802154_HIE_GLOBAL_TIME = 0x29,
    mcp_IEEE802154_HIE_WISUN = 0x2A,
};

/* Payload IE group values. */
enum {
    mcp_IEEE802154_PIE_ESDU = 0x0,
    mcp_IEEE802154_PIE_MLME = 0x1,
    mcp_IEEE802154_PIE_VENDOR_SPECIFIC = 0x2,
    mcp_IEEE802154_PIE_MPX = 0x3,   /* IEEE 802.15.9 Support */
    mcp_IEEE802154_PIE_WISUN = 0x4, /* Wi-SUN Payload embedded IEs */
    mcp_IEEE802154_PIE_RESERVED_MIN = 0x5,
    mcp_IEEE802154_PIE_RESERVED_MAX = 0xe,
    mcp_IEEE802154_PIE_TERM = 0xf,
    };

/* Nested TSCH MLME IE types. */
enum {
    mcp_IEEE802154_MIE_TSCH_SYNC = 0x1a,
    mcp_IEEE802154_MIE_TSCH_LINK = 0x1b,
    mcp_IEEE802154_MIE_TSCH_TIMESLOT = 0x1c,
    mcp_IEEE802154_MIE_HOP_TIMING = 0x1d,
    mcp_IEEE802154_MIE_EB_FILTER = 0x1e,
    mcp_IEEE802154_MIE_METRICS_SINGLE = 0x1f,
    mcp_IEEE802154_MIE_METRICS_ALL = 0x20,
    mcp_IEEE802154_MIE_UNMANAGED_MIN = 0x40,
    mcp_IEEE802154_MIE_UNMANAGED_MAX = 0x7f,
    mcp_IEEE802154_MIE_HOP_SEQUENCE = 0x9,
    mcp_IEEE802154_MIE_VENDOR_SPECIFIC = 0x8,
};

enum {
    mcp_802154_FRAME_TYPE_BEACON = 0X00,
    mcp_802154_FRAME_TYPE_DATA = 0X01,
    mcp_802154_FRAME_TYPE_ACK = 0X02,
    mcp_802154_FRAME_TYPE_CMD = 0X03,
};

/*lint -save -e621 [!MISRA - identifier clash ASSOCIATIONRESP/REQ] */
enum {
    mcp_802154_CMD_TYPE_ASSOCIATIONREQ = 0X01,
    mcp_802154_CMD_TYPE_ASSOCIATIONRESP = 0X02,
    mcp_802154_CMD_TYPE_DISASSOCIATION = 0X03,
    mcp_802154_CMD_TYPE_DATAREQ = 0X04,
    mcp_802154_CMD_TYPE_PANIDCONFLICT = 0X05,
    mcp_802154_CMD_TYPE_ORPHAN = 0X06,
    mcp_802154_CMD_TYPE_BEACONREQ = 0X07,
    mcp_802154_CMD_TYPE_REALIGNMENT = 0X08,
    mcp_802154_CMD_TYPE_GTSREQ = 0X09,
    mcp_802154_CMD_TYPE_VENDOR = 0X24,
};
/*lint -restore */

/* Wi-SUN Channel Functions */
enum {
    mcp_WS_CHANNEL_FIXED = 0, /* Fixed Channel Operation. */
    mcp_WS_CHANNEL_TR51CF = 1, /* Channel Function defined by ASNITIA-4957.200 */
    mcp_WS_CHANNEL_DH1CF = 2, /* Direct Hash channel function defined by WiSUN. */
    mcp_WS_CHANNEL_VENDOR = 3, /* Use Vendor Defined channel function in macHoppingSequence */
};

/* Wi-SUN Regulatory Domains
 * Defined by Wi-SUN-PHY-Specification-1V00.pdf section 5.7
 * Frequency Bands and Channel Parameters
 */
enum {
    mcp_WS_DOMAIN_ISM2400 = 0x00,
    mcp_WS_DOMAIN_USA_CAN = 0x01,
    mcp_WS_DOMAIN_JAPAN = 0x02,
    mcp_WS_DOMAIN_EUROPE = 0x03,
    mcp_WS_DOMAIN_CHINA = 0x04,
    mcp_WS_DOMAIN_INDIA = 0x05,
    mcp_WS_DOMAIN_MEXICO = 0x06,
    mcp_WS_DOMAIN_BRAZIL = 0x07,
    mcp_WS_DOMAIN_AUSTRALIA = 0x08,
    mcp_WS_DOMAIN_KOREA = 0x09,
    mcp_WS_DOMAIN_PHILIPPINES = 0x0a,
    mcp_WS_DOMAIN_MALAYSIA = 0x0b,
    mcp_WS_DOMAIN_HONG_KONG = 0x0c,
    mcp_WS_DOMAIN_SINGAPORE = 0x0d,
    mcp_WS_DOMAIN_THAILAND = 0x0e,
    mcp_WS_DOMAIN_VIETNAM = 0x0f,
};

/* Wi-SUN FAN 1.0 FSK PHY Operating Modes */
enum {
    mcp_WS_OPMODE_1A = 0x00,
    mcp_WS_OPMODE_1B = 0x01,
    mcp_WS_OPMODE_2A = 0x02,
    mcp_WS_OPMODE_2B = 0x03,
    mcp_WS_OPMODE_3  = 0x04,
    mcp_WS_OPMODE_4A = 0x05,
    mcp_WS_OPMODE_4B = 0x06,
    mcp_WS_OPMODE_5  = 0x07,
};

/* Wi-SUN FAN 1.1 PHY Types */
#define WS_PHYTYPE_SHIFT        4U  /* PHY Type is in upper 4 bits of FAN 1.1 PhyModeID. */
enum {
    mcp_WS_MOD_FSK          = (0x00UL << (WS_PHYTYPE_SHIFT)),
    mcp_WS_MOD_FSK_FEC      = (0x01UL << (WS_PHYTYPE_SHIFT)),
    mcp_WS_MOD_OFDM_OPT1    = (0x02UL << (WS_PHYTYPE_SHIFT)),
    mcp_WS_MOD_OFDM_OPT2    = (0x03UL << (WS_PHYTYPE_SHIFT)),
    mcp_WS_MOD_OFDM_OPT3    = (0x04UL << (WS_PHYTYPE_SHIFT)),
    mcp_WS_MOD_OFDM_OPT4    = (0x05UL << (WS_PHYTYPE_SHIFT)),
};

/* Wi-SUN FAN 1.1 OFDM MCS levels */
enum {
    mcp_WS_OFDM_MCS_0   = (0x00UL),
    mcp_WS_OFDM_MCS_1   = (0x01UL),
    mcp_WS_OFDM_MCS_2   = (0x02UL),
    mcp_WS_OFDM_MCS_3   = (0x03UL),
    mcp_WS_OFDM_MCS_4   = (0x04UL),
    mcp_WS_OFDM_MCS_5   = (0x05UL),
    mcp_WS_OFDM_MCS_6   = (0x06UL),
};

/* Wi-SUN FAN 1.1 Channel Plan ID values */
enum {
    mcp_WS_CHPLANID_902_928_200 =   (1U),   /* ChanCenterFreq0 902.2 MHz, ChanSpacing 200 kHz */
    mcp_WS_CHPLANID_902_928_400 =   (2U),   /* ChanCenterFreq0 902.4 MHz, ChanSpacing 400 kHz */
    mcp_WS_CHPLANID_902_928_600 =   (3U),   /* ChanCenterFreq0 902.6 MHz, ChanSpacing 600 kHz */
    mcp_WS_CHPLANID_902_928_800 =   (4U),   /* ChanCenterFreq0 902.7 MHz, ChanSpacing 800 kHz */
    mcp_WS_CHPLANID_902_928_1200 =  (5U),   /* ChanCenterFreq0 903.2 MHz, ChanSpacing 1200 kHz */
    /* 6 - 20 reserved */
    mcp_WS_CHPLANID_920_928_200 =   (21U),   /* ChanCenterFreq0 920.6 MHz, ChanSpacing 200 kHz */
    mcp_WS_CHPLANID_920_928_400 =   (22U),   /* ChanCenterFreq0 920.9 MHz, ChanSpacing 400 kHz */
    mcp_WS_CHPLANID_920_928_600 =   (23U),   /* ChanCenterFreq0 920.8 MHz, ChanSpacing 600 kHz */
    mcp_WS_CHPLANID_920_928_800 =   (24U),   /* ChanCenterFreq0 921.1 MHz, ChanSpacing 800 kHz */
    /* 25 - 31 reserved */
    mcp_WS_CHPLANID_863_870_100 =   (32U),   /* ChanCenterFreq0 863.1 MHz, ChanSpacing 100 kHz */
    mcp_WS_CHPLANID_863_870_200 =   (33U),   /* ChanCenterFreq0 863.1 MHz, ChanSpacing 200 kHz */
    mcp_WS_CHPLANID_870_876_100 =   (34U),   /* ChanCenterFreq0 870.1 MHz, ChanSpacing 100 kHz */
    mcp_WS_CHPLANID_870_876_200 =   (35U),   /* ChanCenterFreq0 870.2 MHz, ChanSpacing 200 kHz */
    mcp_WS_CHPLANID_863_876_100 =   (36U),   /* ChanCenterFreq0 863.1 MHz, ChanSpacing 100 kHz */
    mcp_WS_CHPLANID_863_876_200 =   (37U),   /* ChanCenterFreq0 863.1 MHz, ChanSpacing 200 kHz */
    mcp_WS_CHPLANID_863_876_400 =   (38U),   /* ChanCenterFreq0 863.5 MHz, ChanSpacing 400 kHz */
    mcp_WS_CHPLANID_865_868_100 =   (39U),   /* ChanCenterFreq0 865.1 MHz, ChanSpacing 100 kHz */
    mcp_WS_CHPLANID_865_868_200 =   (40U),   /* ChanCenterFreq0 865.1 MHz, ChanSpacing 200 kHz */
    /* 41 - 63 reserved */
};

/* MLME-ASSOCIATE.request capabilities */
#define MCP_ASSOC_CAP_ALT_COORD     0x01U /* Deprecated by IEEE 802.15.4-2015 */
#define MCP_ASSOC_CAP_DEV_TYPE      0x02U /* 0 = RFD, 1 = FFD */
#define MCP_ASSOC_CAP_PWR_SRC       0x04U /* 0 = battery, 1 = mains */
#define MCP_ASSOC_CAP_RXONIDLE      0x08U
#define MCP_ASSOC_CAP_ASSOC_TYPE    0x10U /* 0 = normal, 1 = fast association */
/* Reserved bit: 0x20 */
#define MCP_ASSOC_CAP_SECURITY      0x40U
#define MCP_ASSOC_CAP_ALLOC_ADDR    0x80U /* 1 = coordinator (i.e. parent) allocates short address, 0 = otherwise */

/* Macro to compute the usage bit for security keys and levels. */
#define MCP_KEY_USAGE(_type_, _cmd_)   (1ULL << (((_type_) == (uint8_t)mcp_802154_FRAME_TYPE_CMD) ? ((_cmd_) + (uint8_t)mcp_802154_FRAME_TYPE_CMD) : (_type_)))
#define MCP_KEY_USAGE_ALL              ((1ULL << (mcp_802154_FRAME_TYPE_CMD + mcp_802154_CMD_TYPE_VENDOR + 1)) - 1)

/* Vendor-specific IE has different ID for HIE, PIE, MIE types. */
#define MCP_TLV_IE_IS_VENDOR(type, id)     \
    ((((type) == MCP_TLV_HIE) && ((id) == (uint8_t)mcp_IEEE802154_HIE_VENDOR_SPECIFIC)) \
    || (((type) == MCP_TLV_PIE) && ((id) == (uint8_t)mcp_IEEE802154_PIE_VENDOR_SPECIFIC)) \
    || (((type) == MCP_TLV_MIE) && ((id) == (uint8_t)mcp_IEEE802154_MIE_VENDOR_SPECIFIC)))

/*lint -restore */

#endif /* MCP_ENUMS_H_ */
