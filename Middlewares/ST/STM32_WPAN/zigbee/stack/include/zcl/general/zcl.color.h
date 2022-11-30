/**
 * This cluster supports scenes functionality. When a scene is recalled that includes this cluster,
 * the attributes listed in the Scenes Table Extensions for this cluster will be set to the value
 * in the scene. ColorTemperatureMireds may also be coupled to the Level Control cluster's
 * CurrentLevel, however, this feature is not implemented in the stack so additional code in the
 * application will be needed.
 * @file zcl.color.h
 * @heading Color Control
 * @brief ZCL Color Control cluster header
 * ZCL 7 Section 5.2
 * ZCL 8 Section 5.2
 * @copyright Copyright [2016 - 2022] Exegin Technologies Limited. All rights reserved.
 */

#ifndef ZCL_COLOR_H
#define ZCL_COLOR_H

#include "zcl/zcl.h"

/* @PICS.ZCL.Color Control(0x0300)
*
* CC.S | Server | True
* CC.C | Client | True
*
* Server Attributes
* CC.S.A0000 | CurrentHue | True
* CC.S.A0000.Report.Tx | CurrentHue | True
* CC.S.A0001 | CurrentSaturation | True
* CC.S.A0001.Scene | CurrentSaturation | True
* CC.S.A0001.Report.Tx | CurrentSaturation | True
* CC.S.A0002 | RemainingTime | False | Optional, but is used in mandatory commands
* CC.S.A0003 | CurrentX | True
* CC.S.A0003.Scene  | CurrentX | True
* CC.S.A0003.Report.Tx | CurrentX | True
* CC.S.A0004 | CurrentY | True
* CC.S.A0004.Scene | CurrentY | True
* CC.S.A0004.Report.Tx  | CurrentY | True
* CC.S.A0005 | DriftCompensation | False | Optional
* CC.S.A0006 | CompensationText | False | Optional
* CC.S.A0007 | ColorTemperatureMireds | True
* CC.S.A0007.Scene | ColorTemperatureMireds | True
* CC.S.A0007.Report.Tx | ColorTemperatureMireds | True
* CC.S.A0008 | ColorMode | True
* CC.S.A000f | Options | True
* CC.S.A0010 | NumberOfPrimaries | True
* CC.S.A0011 | Primary1X | False | Mandatory if the value of the NumberOfPrimaries
* attribute is greater than 0, otherwise optional.
* CC.S.A0012 | Primary1Y | False | Mandatory if the value of the NumberOfPrimaries
* attribute is greater than 0, otherwise optional.
* CC.S.A0013 | Primary1Intensity | False | Mandatory if the value of the NumberOfPrimaries
* attribute is greater than 0, otherwise optional.
* CC.S.A0015 | Primary2X | False | Mandatory if the value of the NumberOfPrimaries
* attribute is greater than 1, otherwise optional.
* CC.S.A0016 | Primary2Y | False | Mandatory if the value of the NumberOfPrimaries
* attribute is greater than 1, otherwise optional.
* CC.S.A0017 | Primary2Intensity | False | Mandatory if the value of the NumberOfPrimaries
* attribute is greater than 1, otherwise optional.
* CC.S.A0019 | Primary3X | False | Mandatory if the value of the NumberOfPrimaries
* attribute is greater than 2, otherwise optional.
* CC.S.A001a | Primary3Y | False | Mandatory if the value of the NumberOfPrimaries
* attribute is greater than 2, otherwise optional.
* CC.S.A001b | Primary3Intensity | False | Mandatory if the value of the NumberOfPrimaries
* attribute is greater than 2, otherwise optional.
* CC.S.A0020 | Primary4X | False | Mandatory if the value of the NumberOfPrimaries
* attribute is greater than 3, otherwise optional.
* CC.S.A0021 | Primary4Y | False | Mandatory if the value of the NumberOfPrimaries
* attribute is greater than 3, otherwise optional.
* CC.S.A0022 | Primary4Intensity | False | Mandatory if the value of the NumberOfPrimaries
* attribute is greater than 3, otherwise optional.
* CC.S.A0024 | Primary5X | False | Mandatory if the value of the NumberOfPrimaries
* attribute is greater than 4, otherwise optional.
* CC.S.A0025 | Primary5Y | False | Mandatory if the value of the NumberOfPrimaries
* attribute is greater than 4, otherwise optional.
* CC.S.A0026 | Primary5Intensity | False | Mandatory if the value of the NumberOfPrimaries
* attribute is greater than 4, otherwise optional.
* CC.S.A0028 | Primary6X | False | Mandatory if the value of the NumberOfPrimaries
* attribute is greater than 5, otherwise optional.
* CC.S.A0029 | Primary6Y | False | Mandatory if the value of the NumberOfPrimaries
* attribute is greater than 5, otherwise optional.
* CC.S.A002a | Primary6Intensity | False | Mandatory if the value of the NumberOfPrimaries
* attribute is greater than 5, otherwise optional.
* CC.S.A0030 | WhitePointX | False | Optional
* CC.S.A0031 | WhitePointY | False | Optional
* CC.S.A0032 | ColorPointRX | False | Optional
* CC.S.A0033 | ColorPointRY | False | Optional
* CC.S.A0034 | ColorPointRIntensity | False | Optional
* CC.S.A0036 | ColorPointGX | False | Optional
* CC.S.A0037 | ColorPointGY | False | Optional
* CC.S.A0038 | ColorPointGIntensity | False | Optional
* CC.S.A003a | ColorPointBX | False | Optional
* CC.S.A003b | ColorPointBY | False | Optional
* CC.S.A003c | ColorPointBIntensity | False | Optional
* CC.S.A4000 | EnhancedCurrentHue | True
* CC.S.A4000.Scene | EnhancedCurrentHue | True
* CC.S.A4001 | EnhancedColorMode | True
* CC.S.A4002 | ColorLoopActive | True
* CC.S.A4002.Scene | ColorLoopActive | True
* CC.S.A4003 | ColorLoopDirection | True
* CC.S.A4003.Scene | ColorLoopDirection | True
* CC.S.A4004 | ColorLoopTime | True
* CC.S.A4004.Scene | ColorLoopTime | True
* CC.S.A4005 | ColorLoopStartEnhancedHue | True
* CC.S.A4006 | ColorLoopStoredEnhancedHue | True
* CC.S.A400a | ColorCapabilities | True
* CC.S.A400b | ColorTempPhysicalMinMireds | True
* CC.S.A400c | ColorTempPhysicalMaxMireds | True
* CC.S.A400d | ColorTempPhysicalMaxMireds | True
* CC.S.A4010 | StartUpColorTemperatureMireds | True
* CC.S.Afffd | ClusterRevision | True
* CC.S.Afffe | AttributeReportingStatus | True
*
* Commands Received
* CC.S.C00.Rsp | Move to Hue| True
* CC.S.C01.Rsp | Move Hue | True
* CC.S.C02.Rsp | Step Hue | True
* CC.S.C03.Rsp | Move to Saturation | True
* CC.S.C04.Rsp | Move Saturation | True
* CC.S.C05.Rsp | Step Saturation |True
* CC.S.C06.Rsp | Move to Hue and Saturation | True
* CC.S.C07.Rsp | Move to Color | True
* CC.S.C08.Rsp | Move Color | True
* CC.S.C09.Rsp | Step Color | True
* CC.S.C0a.Rsp | Move to Color Temperature | True
* CC.S.C40.Rsp | Enhanced Move to Hue | True
* CC.S.C41.Rsp | Enhanced Move Hue | True
* CC.S.C42.Rsp | Enhanced Step Hue | True
* CC.S.C43.Rsp | Enhanced Move to Hue and Saturation | True
* CC.S.C44.Rsp | Color Loop Set | True
* CC.S.C47.Rsp | Stop Move Step | True
* CC.S.C4b.Rsp | Move Color Temperature | True
* CC.S.C4c.Rsp | Step Color Temperature | True
*
* Client Attributes
* CC.C.A0000.Report.Rsp | CurrentHue | False
* CC.C.A0001.Report.Rsp | CurrentSaturation | False
* CC.C.A0003.Report.Rsp | CurrentX | False
* CC.C.A0004.Report.Rsp | CurrentY | False
* CC.C.A0007.Report.Rsp | ColorTemperatureMireds | False
* CC.C.Afffd | ClusterRevision | True
* CC.C.Afffe | AttributeReportingStatus | True
*
* Commands Generated
* CC.C.C00.Tx | Move to Hue| True
* CC.C.C01.Tx | Move Hue | True
* CC.C.C02.Tx | Step Hue | True
* CC.C.C03.Tx | Move to Saturation | True
* CC.C.C04.Tx | Move Saturation | True
* CC.C.C05.Tx | Step Saturation |True
* CC.C.C06.Tx | Move to Hue and Saturation | True
* CC.C.C07.Tx | Move to Color | True
* CC.C.C08.Tx | Move Color | True
* CC.C.C09.Tx | Step Color | True
* CC.C.C0a.Tx | Move to Color Temperature | True
* CC.C.C40.Tx | Enhanced Move to Hue | True
* CC.C.C41.Tx | Enhanced Move Hue | True
* CC.C.C42.Tx | Enhanced Step Hue | True
* CC.C.C43.Tx | Enhanced Move to Hue and Saturation | True
* CC.C.C44.Tx | Color Loop Set | True
* CC.C.C47.Tx | Stop Move Step | True
* CC.C.C4b.Tx | Move Color Temperature | True
* CC.C.C4c.Tx | Step Color Temperature |  True
*/

/** Color Control Server Attribute IDs */
enum ZbZclColorSvrAttrT {
    ZCL_COLOR_ATTR_CURRENT_HUE = 0x0000, /**< CurrentHue */
    ZCL_COLOR_ATTR_CURRENT_SAT = 0x0001, /**< CurrentSaturation */
    ZCL_COLOR_ATTR_REMAINING_TIME = 0x0002, /**< RemainingTime (Optional but is used in mandatory commands) */
    ZCL_COLOR_ATTR_CURRENT_X = 0x0003, /**< CurrentX */
    ZCL_COLOR_ATTR_CURRENT_Y = 0x0004, /**< CurrentY */
    ZCL_COLOR_ATTR_DRIFT_COMP = 0x0005, /**< DriftCompensation (Optional) */
    ZCL_COLOR_ATTR_COMPENSATION_TEXT = 0x0006, /**< CompensationText (Optional) */
    ZCL_COLOR_ATTR_COLOR_TEMP_MIREDS = 0x0007, /**< ColorTemperatureMireds */
    ZCL_COLOR_ATTR_COLOR_MODE = 0x0008, /**< ColorMode */
    ZCL_COLOR_ATTR_OPTIONS = 0x000f, /**< Options */
    ZCL_COLOR_ATTR_NUM_PRIMARIES = 0x0010, /**< NumberOfPrimaries */
    ZCL_COLOR_ATTR_PRIMARY_1X = 0x0011, /**< Primary1X */
    ZCL_COLOR_ATTR_PRIMARY_1Y = 0x0012, /**< Primary1Y */
    ZCL_COLOR_ATTR_PRIMARY_1_INTENS = 0x0013, /**< Primary1Intensity */
    ZCL_COLOR_ATTR_PRIMARY_2X = 0x0015, /**< Primary2X */
    ZCL_COLOR_ATTR_PRIMARY_2Y = 0x0016, /**< Primary2Y */
    ZCL_COLOR_ATTR_PRIMARY_2_INTENS = 0x0017, /**< Primary2Intensity */
    ZCL_COLOR_ATTR_PRIMARY_3X = 0x0019, /**< Primary3X */
    ZCL_COLOR_ATTR_PRIMARY_3Y = 0x001a, /**< Primary3Y */
    ZCL_COLOR_ATTR_PRIMARY_3_INTENS = 0x001b, /**< Primary3Intensity */
    ZCL_COLOR_ATTR_PRIMARY_4X = 0x0020, /**< Primary4X */
    ZCL_COLOR_ATTR_PRIMARY_4Y = 0x0021, /**< Primary4Y */
    ZCL_COLOR_ATTR_PRIMARY_4_INTENS = 0x0022, /**< Primary4Intensity */
    ZCL_COLOR_ATTR_PRIMARY_5X = 0x0024, /**< Primary5X */
    ZCL_COLOR_ATTR_PRIMARY_5Y = 0x0025, /**< Primary5Y */
    ZCL_COLOR_ATTR_PRIMARY_5_INTENS = 0x0026, /**< Primary5Intensity */
    ZCL_COLOR_ATTR_PRIMARY_6X = 0x0028, /**< Primary6X */
    ZCL_COLOR_ATTR_PRIMARY_6Y = 0x0029, /**< Primary6Y */
    ZCL_COLOR_ATTR_PRIMARY_6_INTENS = 0x002a, /**< Primary6Intensity */
    ZCL_COLOR_ATTR_WHITE_POINT_X = 0x0030, /**< WhitePointX (Optional) */
    ZCL_COLOR_ATTR_WHITE_POINT_Y = 0x0031, /**< WhitePointY (Optional) */
    ZCL_COLOR_ATTR_COLOR_POINT_RX = 0x0032, /**< ColorPointRX (Optional) */
    ZCL_COLOR_ATTR_COLOR_POINT_RY = 0x0033, /**< ColorPointRY (Optional) */
    ZCL_COLOR_ATTR_COLOR_POINT_R_INTENS = 0x0034, /**< ColorPointRIntensity (Optional) */
    ZCL_COLOR_ATTR_COLOR_POINT_GX = 0x0036, /**< ColorPointGX (Optional) */
    ZCL_COLOR_ATTR_COLOR_POINT_GY = 0x0037, /**< ColorPointGY (Optional) */
    ZCL_COLOR_ATTR_COLOR_POINT_G_INTENS = 0x0038, /**< ColorPointGIntensity (Optional) */
    ZCL_COLOR_ATTR_COLOR_POINT_BX = 0x003a, /**< ColorPointBX (Optional) */
    ZCL_COLOR_ATTR_COLOR_POINT_BY = 0x003b, /**< ColorPointBY (Optional) */
    ZCL_COLOR_ATTR_COLOR_POINT_B_INTENS = 0x003c, /**< ColorPointBIntensity (Optional) */
    ZCL_COLOR_ATTR_ENH_CURR_HUE = 0x4000, /**< EnhancedCurrentHue */
    ZCL_COLOR_ATTR_ENH_COLOR_MODE = 0x4001, /**< EnhancedColorMode */
    ZCL_COLOR_ATTR_COLOR_LOOP_ACTIVE = 0x4002, /**< ColorLoopActive */
    ZCL_COLOR_ATTR_COLOR_LOOP_DIR = 0x4003, /**< ColorLoopDirection */
    ZCL_COLOR_ATTR_COLOR_LOOP_TIME = 0x4004, /**< ColorLoopTime */
    ZCL_COLOR_ATTR_COLOR_LOOP_START_HUE = 0x4005, /**< ColorLoopStartEnhancedHue */
    ZCL_COLOR_ATTR_COLOR_LOOP_STORE_HUE = 0x4006, /**< ColorLoopStoredEnhancedHue */
    ZCL_COLOR_ATTR_COLOR_CAPABILITIES = 0x400A, /**< ColorCapabilities */
    ZCL_COLOR_ATTR_COLOR_TEMP_MIN = 0x400B, /**< ColorTempPhysicalMinMireds */
    ZCL_COLOR_ATTR_COLOR_TEMP_MAX = 0x400C, /**< ColorTempPhysicalMaxMireds */
    ZCL_COLOR_ATTR_COUPLE_COLOR_TL_MIN = 0x400d, /**< ColorTempPhysicalMaxMireds */
    ZCL_COLOR_ATTR_STARTUP_COLOR_TEMP = 0x4010 /**< StartUpColorTemperatureMireds */
};

/* Color Control Command Identifiers  */
enum ZbZclColorCommandT {
    ZCL_COLOR_COMMAND_MOVE_TO_HUE = 0x00,
    ZCL_COLOR_COMMAND_MOVE_HUE = 0x01,
    ZCL_COLOR_COMMAND_STEP_HUE = 0x02,
    ZCL_COLOR_COMMAND_MOVE_TO_SAT = 0x03,
    ZCL_COLOR_COMMAND_MOVE_SAT = 0x04,
    ZCL_COLOR_COMMAND_STEP_SAT = 0x05,
    ZCL_COLOR_COMMAND_MOVE_TO_HS = 0x06,
    ZCL_COLOR_COMMAND_MOVE_TO_COLOR = 0x07,
    ZCL_COLOR_COMMAND_MOVE_COLOR = 0x08,
    ZCL_COLOR_COMMAND_STEP_COLOR = 0x09,
    ZCL_COLOR_COMMAND_MOVE_TO_COLOR_TEMP = 0x0a,
    ZCL_COLOR_COMMAND_ENH_MOVE_TO_HUE = 0x40,
    ZCL_COLOR_COMMAND_ENH_MOVE_HUE = 0x41,
    ZCL_COLOR_COMMAND_ENH_STEP_HUE = 0x42,
    ZCL_COLOR_COMMAND_ENH_MOVE_TO_HS = 0x43,
    ZCL_COLOR_COMMAND_COLOR_LOOP_SET = 0x44,
    ZCL_COLOR_COMMAND_STOP_MOVE_STEP = 0x47,
    ZCL_COLOR_COMMAND_MOVE_COLOR_TEMP = 0x4b,
    ZCL_COLOR_COMMAND_STEP_COLOR_TEMP = 0x4c,
};

/* Drift Compensation */
enum ZbZclColorDriftComp {
    ZCL_COLOR_DRIFT_NONE = 0x00,
    ZCL_COLOR_DRIFT_OTHER_UKN = 0x01,
    ZCL_COLOR_DRIFT_TEMP_MON = 0x02,
    ZCL_COLOR_DRIFT_OPTIC_LUM = 0x03,
    ZCL_COLOR_DRIFT_OPTIC_COLOR = 0x04,
};

/* Color Mode */
enum ZbZclColorMode {
    ZCL_COLOR_MODE_HS = 0x00,
    ZCL_COLOR_MODE_XY = 0x01,
    ZCL_COLOR_MODE_TEMP = 0x02,
};

/* Enhanced Color Mode */
enum ZbZclColorModeEnh {
    ZCL_COLOR_ENH_MODE_CURR_HS = 0x00,
    ZCL_COLOR_ENH_MODE_CURR_XY = 0x01,
    ZCL_COLOR_ENH_MODE_COLOR_TEMP = 0x02,
    ZCL_COLOR_ENH_MODE_ENH_HUE_SAT = 0x03,
};

/* Color Capabilities - bit field */
#define ZCL_COLOR_CAP_HS                    0x01U /* Mandatory if ZCL_COLOR_CAP_ENH_HUE */
#define ZCL_COLOR_CAP_ENH_HUE               0x02U
#define ZCL_COLOR_CAP_COLOR_LOOP            0x04U
#define ZCL_COLOR_CAP_XY                    0x08U /* Mandatory */
#define ZCL_COLOR_CAP_COLOR_TEMP            0x10U

/* Move direction field */
enum ZbZclColorMoveDir {
    ZCL_COLOR_MOVE_TO_DIR_SHORTEST = 0x00,
    ZCL_COLOR_MOVE_TO_DIR_LONGEST = 0x01,
    ZCL_COLOR_MOVE_TO_DIR_UP = 0x02,
    ZCL_COLOR_MOVE_TO_DIR_DOWN = 0x03,
};

/* Move mode */
enum ZbZclColorMoveMode {
    ZCL_COLOR_MOVE_MODE_STOP = 0x00,
    ZCL_COLOR_MOVE_MODE_UP = 0x01,
    ZCL_COLOR_MOVE_MODE_DOWN = 0x03,
};

/* Step mode */
enum ZbZclColorStepMode {
    ZCL_COLOR_STEP_MODE_UP = 0x01,
    ZCL_COLOR_STEP_MODE_DOWN = 0X03,
};

/* Color Loop Set action */
enum ZbZclColorLoopAction {
    ZCL_COLOR_LOOP_ACTION_DEACTIVATE = 0x00,
    ZCL_COLOR_LOOP_ACTION_ACTIVATE_FIELD = 0x01,
    ZCL_COLOR_LOOP_ACTION_ACTIVATE_ATTR = 0x02,
};

/* Color Loop Set direction */
enum ZbZclColorLoopDir {
    ZCL_COLOR_LOOP_DIR_DECREMENT_HUE = 0x00,
    ZCL_COLOR_LOOP_DIR_INCREMENT_HUE = 0x01,
};

/* Color Loop Set update flags */
enum ZbZclColorLoopFlags {
    ZCL_COLOR_LOOP_FLAG_UPDATE_ACTION = 0x01,
    ZCL_COLOR_LOOP_FLAG_UPDATE_DIRECTION = 0x02,
    ZCL_COLOR_LOOP_FLAG_UPDATE_TIME = 0x04,
    ZCL_COLOR_LOOP_FLAG_UPDATE_START_HUE = 0x08,
};

/* Options Attribute - bit field */
#define ZCL_COLOR_OPTIONS_EXECUTE_IF_OFF            0x01

#define ZCL_COLOR_MODE_DEFAULT                      0x01
#define ZCL_COLOR_OPTIONS_DEFAULT                   0x00
#define ZCL_COLOR_ILLEGAL_HUE                       0xFFFF
#define ZCL_COLOR_MAX_HUE_SAT                       0xFE
#define ZCL_COLOR_X_DEFAULT                         0x616B
#define ZCL_COLOR_X_MAX                             0xFEFF
#define ZCL_COLOR_Y_DEFAULT                         0x607D
#define ZCL_COLOR_Y_MAX                             0xFEFF
#define ZCL_COLOR_TEMP_DEFAULT                      0x00FA
#define ZCL_COLOR_TEMP_MAX                          0xFEFF
#define ZCL_COLOR_LOOP_TIME_DEFAULT                 0x0019
#define ZCL_COLOR_LOOP_START_HUE_DEFAULT            0x2300
#define ZCL_COLOR_STARTUP_COLOR_TEMP_MAX            0xFEFF

/*-----------------------------------------------------------------------------
 * Structures
 *-----------------------------------------------------------------------------
 */

/** Move to Hue command structure */
struct ZbZclColorClientMoveToHueReqT {
    uint8_t hue; /**< Hue */
    uint8_t direction; /**< Direction */
    uint16_t transition_time; /**< Transition Time */
    uint8_t mask; /**< OptionsMask */
    uint8_t override; /**< OptionsOverride */
};

/** Move Hue command structure */
struct ZbZclColorClientMoveHueReqT {
    uint8_t move_mode; /**< Move Mode */
    uint8_t rate; /**< Rate */
    uint8_t mask; /**< OptionsMask */
    uint8_t override; /**< OptionsOverride */
};

/** Step Hue command structure */
struct ZbZclColorClientStepHueReqT {
    uint8_t step_mode; /**< Step Mode */
    uint8_t step_size; /**< Step Size */
    uint8_t transition_time; /**< Transition Time */
    uint8_t mask; /**< OptionsMask */
    uint8_t override; /* OptionsOverride */
};

/** Move to Saturation command structure */
struct ZbZclColorClientMoveToSatReqT {
    uint8_t sat; /**< Saturation */
    uint16_t transition_time; /**< Transition Time */
    uint8_t mask; /**< OptionsMask */
    uint8_t override; /**< OptionsOverride */
};

/** Move Saturation command structure */
struct ZbZclColorClientMoveSatReqT {
    uint8_t move_mode; /**< Move Mode */
    uint8_t rate; /**< Rate */
    uint8_t mask; /**< OptionsMask */
    uint8_t override; /**< OptionsOverride */
};

/** Step Saturation command structure */
struct ZbZclColorClientStepSatReqT {
    uint8_t step_mode; /**< Step Mode */
    uint8_t step_size; /**< Step Size */
    uint8_t transition_time; /**< Transition Time */
    uint8_t mask; /**< OptionsMask */
    uint8_t override; /**< OptionsOverride */
};

/** Move to Hue and Saturation command structure */
struct ZbZclColorClientMoveToHueSatReqT {
    uint8_t hue; /**< Hue */
    uint8_t sat; /**< Saturation */
    uint16_t transition_time; /**< Transition Time */
    uint8_t mask; /**< OptionsMask */
    uint8_t override; /**< OptionsOverride */
};

/** Move to Color command structure */
struct ZbZclColorClientMoveToColorXYReqT {
    uint16_t color_x; /**< ColorX */
    uint16_t color_y; /**< ColorY */
    uint16_t transition_time; /**< Transition Time */
    uint8_t mask; /**< OptionsMask */
    uint8_t override; /**< OptionsOverride */
};

/** Move Color command structure */
struct ZbZclColorClientMoveColorXYReqT {
    uint16_t rate_x; /**< RateX */
    uint16_t rate_y; /**< RateY */
    uint8_t mask; /**< OptionsMask */
    uint8_t override; /**< OptionsOverride */
};

/** Step Color command structure */
struct ZbZclColorClientStepColorXYReqT {
    uint16_t step_x; /**< StepX */
    uint16_t step_y; /**< StepY */
    uint16_t transition_time; /**< Transition Time */
    uint8_t mask; /**< OptionsMask */
    uint8_t override; /**< OptionsOverride */
};

/** Move to Color Temperature command structure */
struct ZbZclColorClientMoveToColorTempReqT {
    uint16_t color_temp; /**< Color Temperature Mireds */
    uint16_t transition_time; /**< Transition Time */
    uint8_t mask; /**< OptionsMask */
    uint8_t override; /**< OptionsOverride */
};

/** Enhanced Move to Hue command structure */
struct ZbZclColorClientMoveToHueEnhReqT {
    uint16_t enh_hue; /**< Enhanced Hue */
    uint8_t direction; /**< Direction */
    uint16_t transition_time; /**< Transition Time */
    uint8_t mask; /**< OptionsMask */
    uint8_t override; /**< OptionsOverride */
};

/** Enhanced Move Hue command structure */
struct ZbZclColorClientMoveHueEnhReqT {
    uint8_t move_mode; /**< Move Mode */
    uint16_t rate; /**< Rate */
    uint8_t mask; /**< OptionsMask */
    uint8_t override; /**< OptionsOverride */
};

/** Enhanced Step Hue command structure */
struct ZbZclColorClientStepHueEnhReqT {
    uint8_t step_mode; /**< Step Mode */
    uint16_t step_size; /**< Step Size */
    uint16_t transition_time; /**< Transition Time */
    uint8_t mask; /**< OptionsMask */
    uint8_t override; /**< OptionsOverride */
};

/** Enhanced Move to Hue and Saturation command structure */
struct ZbZclColorClientMoveToHueSatEnhReqT {
    uint16_t enh_hue; /**< Enhanced Hue */
    uint8_t sat; /**< Saturation */
    uint16_t transition_time; /**< Transition Time */
    uint8_t mask; /**< OptionsMask */
    uint8_t override; /**< OptionsOverride */
};

/** Color Loop Set command structure */
struct ZbZclColorClientColorLoopSetReqT {
    uint8_t update_flags; /**< Update Flags */
    uint8_t action; /**< Action */
    uint8_t direction; /**< Direction  */
    uint16_t transition_time; /**< Time */
    uint16_t start_hue; /**< Start Hue */
    uint8_t mask; /**< OptionsMask */
    uint8_t override; /**< OptionsOverride */
};

/** Stop Move Step command structure */
struct ZbZclColorClientStopMoveStepReqT {
    uint8_t mask; /**< OptionsMask */
    uint8_t override; /**< OptionsOverride */
};

/** Move Color Temperature command structure */
struct ZbZclColorClientMoveColorTempReqT {
    uint8_t move_mode; /**< Move Mode */
    uint16_t rate; /**< Rate */
    uint16_t color_temp_min; /**< Color Temperature Minimum Mireds */
    uint16_t color_temp_max; /**< Color Temperature Maximum Mireds */
    uint8_t mask; /**< OptionsMask */
    uint8_t override; /**< OptionsOverride */
};

/** Step Color Temperature command structure */
struct ZbZclColorClientStepColorTempReqT {
    uint8_t step_mode; /**< Step Mode */
    uint16_t step_size; /**< Step Size */
    uint16_t transition_time; /**< Transition Time */
    uint16_t color_temp_min; /**< Color Temperature Minimum Mireds */
    uint16_t color_temp_max; /**< Color Temperature Maximum Mireds */
    uint8_t mask; /**< OptionsMask */
    uint8_t override; /**< OptionsOverride */
};

/*-----------------------------------------------------------------------------
 * Server
 *-----------------------------------------------------------------------------
 */

/** Color Control Server callbacks configuration */
struct ZbZclColorServerCallbacksT {
    /* Returns a ZCL Status code to send in the Default Response, or
     * ZCL_STATUS_SUCCESS_NO_DEFAULT_RESPONSE if no Default Response to be generated. */

    enum ZclStatusCodeT (*move_to_hue)(struct ZbZclClusterT *cluster,
        struct ZbZclColorClientMoveToHueReqT *req,
        struct ZbZclAddrInfoT *srcInfo, void *arg);
    /**< Callback to application, invoked on receipt of Move to Hue command.
     * The application is expected to update ZCL_COLOR_ATTR_CURRENT_HUE,
     * and ZCL_COLOR_ATTR_REMAINING_TIME */

    enum ZclStatusCodeT (*move_hue)(struct ZbZclClusterT *cluster,
        struct ZbZclColorClientMoveHueReqT *req,
        struct ZbZclAddrInfoT *srcInfo, void *arg);
    /**< Callback to application, invoked on receipt of Move Hue command.
     * The application is expected to update ZCL_COLOR_ATTR_CURRENT_HUE,
     * and ZCL_COLOR_ATTR_REMAINING_TIME */

    enum ZclStatusCodeT (*step_hue)(struct ZbZclClusterT *cluster,
        struct ZbZclColorClientStepHueReqT *req,
        struct ZbZclAddrInfoT *srcInfo, void *arg);
    /**< Callback to application, invoked on receipt of Step Hue command.
     * The application is expected to update ZCL_COLOR_ATTR_CURRENT_HUE,
     * and ZCL_COLOR_ATTR_REMAINING_TIME */

    enum ZclStatusCodeT (*move_to_sat)(struct ZbZclClusterT *cluster,
        struct ZbZclColorClientMoveToSatReqT *req,
        struct ZbZclAddrInfoT *srcInfo, void *arg);
    /**< Callback to application, invoked on receipt of Move to Saturation command.
     * The application is expected to update ZCL_COLOR_ATTR_CURRENT_SAT,
     * and ZCL_COLOR_ATTR_REMAINING_TIME */

    enum ZclStatusCodeT (*move_sat)(struct ZbZclClusterT *cluster,
        struct ZbZclColorClientMoveSatReqT *req,
        struct ZbZclAddrInfoT *srcInfo, void *arg);
    /**< Callback to application, invoked on receipt of Move Saturation command.
     * The application is expected to update ZCL_COLOR_ATTR_CURRENT_SAT,
     * and ZCL_COLOR_ATTR_REMAINING_TIME */

    enum ZclStatusCodeT (*step_sat)(struct ZbZclClusterT *cluster,
        struct ZbZclColorClientStepSatReqT *req,
        struct ZbZclAddrInfoT *srcInfo, void *arg);
    /**< Callback to application, invoked on receipt of Step Saturation command.
     * The application is expected to update ZCL_COLOR_ATTR_CURRENT_SAT,
     * and ZCL_COLOR_ATTR_REMAINING_TIME */

    enum ZclStatusCodeT (*move_to_hue_sat)(struct ZbZclClusterT *cluster,
        struct ZbZclColorClientMoveToHueSatReqT *req,
        struct ZbZclAddrInfoT *srcInfo, void *arg);
    /**< Callback to application, invoked on receipt of Move to Hue and Saturation command.
     * The application is expected to update ZCL_COLOR_ATTR_CURRENT_HUE,
     * ZCL_COLOR_ATTR_CURRENT_SAT, and ZCL_COLOR_ATTR_REMAINING_TIME */

    enum ZclStatusCodeT (*move_to_color_xy)(struct ZbZclClusterT *cluster,
        struct ZbZclColorClientMoveToColorXYReqT *req,
        struct ZbZclAddrInfoT *srcInfo, void *arg);
    /**< Callback to application, invoked on receipt of Move to Color command.
     * The application is expected to update ZCL_COLOR_ATTR_CURRENT_X,
     * ZCL_COLOR_ATTR_CURRENT_Y, and ZCL_COLOR_ATTR_REMAINING_TIME */

    enum ZclStatusCodeT (*move_color_xy)(struct ZbZclClusterT *cluster,
        struct ZbZclColorClientMoveColorXYReqT *req,
        struct ZbZclAddrInfoT *srcInfo, void *arg);
    /**< Callback to application, invoked on receipt of Move Color command.
     * The application is expected to update ZCL_COLOR_ATTR_CURRENT_X,
     * ZCL_COLOR_ATTR_CURRENT_Y, and ZCL_COLOR_ATTR_REMAINING_TIME */

    enum ZclStatusCodeT (*step_color_xy)(struct ZbZclClusterT *cluster,
        struct ZbZclColorClientStepColorXYReqT *req,
        struct ZbZclAddrInfoT *srcInfo, void *arg);
    /**< Callback to application, invoked on receipt of Step Color command.
     * The application is expected to update ZCL_COLOR_ATTR_CURRENT_X,
     * ZCL_COLOR_ATTR_CURRENT_Y, and ZCL_COLOR_ATTR_REMAINING_TIME */

    enum ZclStatusCodeT (*move_to_color_temp)(struct ZbZclClusterT *cluster,
        struct ZbZclColorClientMoveToColorTempReqT *req,
        struct ZbZclAddrInfoT *srcInfo, void *arg);
    /**< Callback to application, invoked on receipt of Move to Color Temperature command.
     * The application is expected to update ZCL_COLOR_ATTR_COLOR_TEMP_MIREDS,
     * and ZCL_COLOR_ATTR_REMAINING_TIME */

    enum ZclStatusCodeT (*move_to_hue_enh)(struct ZbZclClusterT *cluster,
        struct ZbZclColorClientMoveToHueEnhReqT *req,
        struct ZbZclAddrInfoT *srcInfo, void *arg);
    /**< Callback to application, invoked on receipt of Enhanced Move to Hue command.
     * The application is expected to update ZCL_COLOR_ATTR_ENH_CURR_HUE,
     * and ZCL_COLOR_ATTR_REMAINING_TIME */

    enum ZclStatusCodeT (*move_hue_enh)(struct ZbZclClusterT *cluster,
        struct ZbZclColorClientMoveHueEnhReqT *req,
        struct ZbZclAddrInfoT *srcInfo, void *arg);
    /**< Callback to application, invoked on receipt of Move Hue command.
     * The application is expected to update ZCL_COLOR_ATTR_ENH_CURR_HUE,
     * and ZCL_COLOR_ATTR_REMAINING_TIME */

    enum ZclStatusCodeT (*step_hue_enh)(struct ZbZclClusterT *cluster,
        struct ZbZclColorClientStepHueEnhReqT *req,
        struct ZbZclAddrInfoT *srcInfo, void *arg);
    /**< Callback to application, invoked on receipt of Enhanced Step Hue command.
     * The application is expected to update ZCL_COLOR_ATTR_ENH_CURR_HUE,
     * and ZCL_COLOR_ATTR_REMAINING_TIME */

    enum ZclStatusCodeT (*move_to_hue_sat_enh)(struct ZbZclClusterT *cluster,
        struct ZbZclColorClientMoveToHueSatEnhReqT *req,
        struct ZbZclAddrInfoT *srcInfo, void *arg);
    /**< Callback to application, invoked on receipt of Enhanced Move to Hue and Saturation command.
     * The application is expected to update ZCL_COLOR_ATTR_ENH_CURR_HUE,
     * ZCL_COLOR_ATTR_CURRENT_SAT, and ZCL_COLOR_ATTR_REMAINING_TIME */

    enum ZclStatusCodeT (*color_loop_set)(struct ZbZclClusterT *cluster,
        struct ZbZclColorClientColorLoopSetReqT *req,
        struct ZbZclAddrInfoT *srcInfo, void *arg);
    /**< Callback to application, invoked on receipt of Color Loop Set command.
     * The application is expected to update the following attributes according to the
     * update flags and action fields: ZCL_COLOR_ATTR_COLOR_LOOP_ACTIVE,
     * ZCL_COLOR_ATTR_COLOR_LOOP_DIR, ZCL_COLOR_ATTR_COLOR_LOOP_TIME,
     * ZCL_COLOR_ATTR_COLOR_LOOP_START_HUE, ZCL_COLOR_ATTR_ENH_CURR_HUE,
     * ZCL_COLOR_ATTR_COLOR_LOOP_STORE_HUE, and ZCL_COLOR_ATTR_REMAINING_TIME */

    enum ZclStatusCodeT (*stop_move_step)(struct ZbZclClusterT *cluster,
        struct ZbZclColorClientStopMoveStepReqT *req,
        struct ZbZclAddrInfoT *srcInfo, void *arg);
    /**< Callback to application, invoked on receipt of Stop Move Step command.
     * The application is expected to update ZCL_COLOR_ATTR_REMAINING_TIME */

    enum ZclStatusCodeT (*move_color_temp)(struct ZbZclClusterT *cluster,
        struct ZbZclColorClientMoveColorTempReqT *req,
        struct ZbZclAddrInfoT *srcInfo, void *arg);
    /**< Callback to application, invoked on receipt of Move Color Temperature command.
     * The application is expected to update ZCL_COLOR_ATTR_COLOR_TEMP_MIREDS,
     * and ZCL_COLOR_ATTR_REMAINING_TIME */

    enum ZclStatusCodeT (*step_color_temp)(struct ZbZclClusterT *cluster,
        struct ZbZclColorClientStepColorTempReqT *req,
        struct ZbZclAddrInfoT *srcInfo, void *arg);
    /**< Callback to application, invoked on receipt of Step Color Temperature command.
     * The application is expected to update ZCL_COLOR_ATTR_COLOR_TEMP_MIREDS,
     * and ZCL_COLOR_ATTR_REMAINING_TIME */
};

struct ZbColorClusterConfig {
    struct ZbZclColorServerCallbacksT callbacks;
    uint8_t capabilities; /* e.g. ZCL_COLOR_CAP_HS */
};

/**
 * Instantiate a new instance of the Color Control server cluster
 * @param zb Zigbee stack instance
 * @param endpoint Endpoint on which to create cluster
 * @param onoff_server OnOff Server cluster pointer for processing commands with the Options fields, may be NULL
 * @param attribute_list List of application defined attributes to be appended, may be NULL
 * @param num_attrs Number of application defined attributes to be added, may be 0 if attribute_list is NULL
 * @param config Configuration containing Color Control capabilities and callbacks for handling requests
 * @param arg Pointer to application data that will later be provided back to the callback functions when invoked
 * @return Cluster pointer, or NULL if there is an error
 */
struct ZbZclClusterT * ZbZclColorServerAlloc(struct ZigBeeT *zb, uint8_t endpoint, struct ZbZclClusterT *onoff_server,
    const struct ZbZclAttrT *attribute_list, unsigned int num_attrs, struct ZbColorClusterConfig *config, void *arg);

/*-----------------------------------------------------------------------------
 * Client
 *-----------------------------------------------------------------------------
 */

/**
 * Instantiate a new instance of the Color Control client cluster
 * @param zb Zigbee stack instance
 * @param endpoint Endpoint on which to create cluster
 * @return Cluster pointer, or NULL if there is an error
 */
struct ZbZclClusterT * ZbZclColorClientAlloc(struct ZigBeeT *zb, uint8_t endpoint);

/**
 * Send a Move to Hue command
 * @param clusterPtr Cluster instance from which to send this command
 * @param dst Destination address for request
 * @param req Move to Hue command request structure
 * @param callback Callback function that will be invoked later when the response is received
 * @param arg Pointer to application data that will later be provided back to the callback function when invoked
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT ZbZclColorClientMoveToHueReq(struct ZbZclClusterT *clusterPtr,
    const struct ZbApsAddrT *dst, struct ZbZclColorClientMoveToHueReqT *req,
    void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

/**
 * Send a Move Hue command
 * @param clusterPtr Cluster instance from which to send this command
 * @param dst Destination address for request
 * @param req Move Hue command request structure
 * @param callback Callback function that will be invoked later when the response is received
 * @param arg Pointer to application data that will later be provided back to the callback function when invoked
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT ZbZclColorClientMoveHueReq(struct ZbZclClusterT *clusterPtr,
    const struct ZbApsAddrT *dst, struct ZbZclColorClientMoveHueReqT *req,
    void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

/**
 * Send a Step Hue command
 * @param clusterPtr Cluster instance from which to send this command
 * @param dst Destination address for request
 * @param req Step Hue command request structure
 * @param callback Callback function that will be invoked later when the response is received
 * @param arg Pointer to application data that will later be provided back to the callback function when invoked
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT ZbZclColorClientStepHueReq(struct ZbZclClusterT *clusterPtr,
    const struct ZbApsAddrT *dst, struct ZbZclColorClientStepHueReqT *req,
    void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

/**
 * Send a Move to Saturation command
 * @param clusterPtr Cluster instance from which to send this command
 * @param dst Destination address for request
 * @param req Move to Saturation command command request structure
 * @param callback Callback function that will be invoked later when the response is received
 * @param arg Pointer to application data that will later be provided back to the callback function when invoked
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT ZbZclColorClientMoveToSatReq(struct ZbZclClusterT *clusterPtr,
    const struct ZbApsAddrT *dst, struct ZbZclColorClientMoveToSatReqT *req,
    void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

/**
 * Send a Move Saturation command
 * @param clusterPtr Cluster instance from which to send this command
 * @param dst Destination address for request
 * @param req Move Saturation command request structure
 * @param callback Callback function that will be invoked later when the response is received
 * @param arg Pointer to application data that will later be provided back to the callback function when invoked
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT ZbZclColorClientMoveSatReq(struct ZbZclClusterT *clusterPtr,
    const struct ZbApsAddrT *dst, struct ZbZclColorClientMoveSatReqT *req,
    void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

/**
 * Send a Step Saturation command
 * @param clusterPtr Cluster instance from which to send this command
 * @param dst Destination address for request
 * @param req Step Saturation command command request structure
 * @param callback Callback function that will be invoked later when the response is received
 * @param arg Pointer to application data that will later be provided back to the callback function when invoked
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT ZbZclColorClientStepSatReq(struct ZbZclClusterT *clusterPtr,
    const struct ZbApsAddrT *dst, struct ZbZclColorClientStepSatReqT *req,
    void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

/**
 * Send a Move to Hue and Saturation command
 * @param clusterPtr Cluster instance from which to send this command
 * @param dst Destination address for request
 * @param req  Move to Hue and Saturation command request structure
 * @param callback Callback function that will be invoked later when the response is received
 * @param arg Pointer to application data that will later be provided back to the callback function when invoked
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT ZbZclColorClientMoveToHueSatReq(struct ZbZclClusterT *clusterPtr,
    const struct ZbApsAddrT *dst, struct ZbZclColorClientMoveToHueSatReqT *req,
    void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

/**
 * Send a Move to Color command
 * @param clusterPtr Cluster instance from which to send this command
 * @param dst Destination address for request
 * @param req Move to Color command request structure
 * @param callback Callback function that will be invoked later when the response is received
 * @param arg Pointer to application data that will later be provided back to the callback function when invoked
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT ZbZclColorClientMoveToColorXYReq(struct ZbZclClusterT *clusterPtr,
    const struct ZbApsAddrT *dst, struct ZbZclColorClientMoveToColorXYReqT *req,
    void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

/**
 * Send a Move Color command
 * @param clusterPtr Cluster instance from which to send this command
 * @param dst Destination address for request
 * @param req Move Color command request structure
 * @param callback Callback function that will be invoked later when the response is received
 * @param arg Pointer to application data that will later be provided back to the callback function when invoked
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT ZbZclColorClientMoveColorXYReq(struct ZbZclClusterT *clusterPtr,
    const struct ZbApsAddrT *dst, struct ZbZclColorClientMoveColorXYReqT *req,
    void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

/**
 * Send a Step Color command
 * @param clusterPtr Cluster instance from which to send this command
 * @param dst Destination address for request
 * @param req Step Color command request structure
 * @param callback Callback function that will be invoked later when the response is received
 * @param arg Pointer to application data that will later be provided back to the callback function when invoked
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT ZbZclColorClientStepColorXYReq(struct ZbZclClusterT *clusterPtr,
    const struct ZbApsAddrT *dst, struct ZbZclColorClientStepColorXYReqT *req,
    void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

/**
 * Send a Move to Color Temperature command
 * @param clusterPtr Cluster instance from which to send this command
 * @param dst Destination address for request
 * @param req Move to Color Temperature command request structure
 * @param callback Callback function that will be invoked later when the response is received
 * @param arg Pointer to application data that will later be provided back to the callback function when invoked
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT ZbZclColorClientMoveToColorTempReq(struct ZbZclClusterT *clusterPtr,
    const struct ZbApsAddrT *dst, struct ZbZclColorClientMoveToColorTempReqT *req,
    void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

/**
 * Send an Enhanced Move to Hue command
 * @param clusterPtr Cluster instance from which to send this command
 * @param dst Destination address for request
 * @param req Enhanced Move to Hue command request structure
 * @param callback Callback function that will be invoked later when the response is received
 * @param arg Pointer to application data that will later be provided back to the callback function when invoked
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT ZbZclColorClientMoveToHueEnhReq(struct ZbZclClusterT *clusterPtr,
    const struct ZbApsAddrT *dst, struct ZbZclColorClientMoveToHueEnhReqT *req,
    void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

/**
 * Send an Enhanced Move Hue command
 * @param clusterPtr Cluster instance from which to send this command
 * @param dst Destination address for request
 * @param req Enhanced Move Hue command request structure
 * @param callback Callback function that will be invoked later when the response is received
 * @param arg Pointer to application data that will later be provided back to the callback function when invoked
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT ZbZclColorClientMoveHueEnhReq(struct ZbZclClusterT *clusterPtr,
    const struct ZbApsAddrT *dst, struct ZbZclColorClientMoveHueEnhReqT *req,
    void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

/**
 * Send an Enhanced Step Hue command
 * @param clusterPtr Cluster instance from which to send this command
 * @param dst Destination address for request
 * @param req Enhanced Step Hue command request structure
 * @param callback Callback function that will be invoked later when the response is received
 * @param arg Pointer to application data that will later be provided back to the callback function when invoked
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT ZbZclColorClientStepHueEnhReq(struct ZbZclClusterT *clusterPtr,
    const struct ZbApsAddrT *dst, struct ZbZclColorClientStepHueEnhReqT *req,
    void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

/**
 * Send an Enhanced Move to Hue and Saturation command
 * @param clusterPtr Cluster instance from which to send this command
 * @param dst Destination address for request
 * @param req Enhanced Move to Hue and Saturation command request structure
 * @param callback Callback function that will be invoked later when the response is received
 * @param arg Pointer to application data that will later be provided back to the callback function when invoked
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT ZbZclColorClientMoveToHueSatEnhReq(struct ZbZclClusterT *clusterPtr,
    const struct ZbApsAddrT *dst, struct ZbZclColorClientMoveToHueSatEnhReqT *req,
    void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

/**
 * Send a Color Loop Set command
 * @param cluster Cluster instance from which to send this command
 * @param dst Destination address for request
 * @param req Color Loop Set command request structure
 * @param callback Callback function that will be invoked later when the response is received
 * @param arg Pointer to application data that will later be provided back to the callback function when invoked
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT ZbZclColorClientColorLoopSetReq(struct ZbZclClusterT *clusterPtr,
    const struct ZbApsAddrT *dst, struct ZbZclColorClientColorLoopSetReqT *req,
    void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

/**
 * Send a Stop Move Step command
 * @param cluster Cluster instance from which to send this command
 * @param dst Destination address for request
 * @param req Stop Move Step command request structure
 * @param callback Callback function that will be invoked later when the response is received
 * @param arg Pointer to application data that will later be provided back to the callback function when invoked
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT ZbZclColorClientStopMoveStepReq(struct ZbZclClusterT *clusterPtr,
    const struct ZbApsAddrT *dst, struct ZbZclColorClientStopMoveStepReqT *req,
    void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

/**
 * Send a Move Color Temperature command
 * @param clusterPtr Cluster instance from which to send this command
 * @param dst Destination address for request
 * @param req Move Color Temperature command request structure
 * @param callback Callback function that will be invoked later when the response is received
 * @param arg Pointer to application data that will later be provided back to the callback function when invoked
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT ZbZclColorClientMoveColorTempReq(struct ZbZclClusterT *clusterPtr,
    const struct ZbApsAddrT *dst, struct ZbZclColorClientMoveColorTempReqT *req,
    void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

/**
 * Send a Step Color Temperature command
 * @param cluster Cluster instance from which to send this command
 * @param dst Destination address for request
 * @param req Step Color Temperature command request structure
 * @param callback Callback function that will be invoked later when the response is received
 * @param arg Pointer to application data that will later be provided back to the callback function when invoked
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT ZbZclColorClientStepColorTempReq(struct ZbZclClusterT *clusterPtr,
    const struct ZbApsAddrT *dst, struct ZbZclColorClientStepColorTempReqT *req,
    void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

/*-----------------------------------------------------------------------------
 * Cluster Payload Helpers
 *-----------------------------------------------------------------------------
 */
int ZbZclColorClientMoveToHueBuild(uint8_t *payload, unsigned int length, struct ZbZclColorClientMoveToHueReqT *req);
int ZbZclColorClientMoveHueBuild(uint8_t *payload, unsigned int length, struct ZbZclColorClientMoveHueReqT *req);
int ZbZclColorClientStepHueBuild(uint8_t *payload, unsigned int length, struct ZbZclColorClientStepHueReqT *req);
int ZbZclColorClientMoveToSatBuild(uint8_t *payload, unsigned int length, struct ZbZclColorClientMoveToSatReqT *req);
int ZbZclColorClientMoveSatBuild(uint8_t *payload, unsigned int length, struct ZbZclColorClientMoveSatReqT *req);
int ZbZclColorClientStepSatBuild(uint8_t *payload, unsigned int length, struct ZbZclColorClientStepSatReqT *req);
int ZbZclColorClientMoveToHueSatBuild(uint8_t *payload, unsigned int length, struct ZbZclColorClientMoveToHueSatReqT *req);
int ZbZclColorClientMoveToColorXYBuild(uint8_t *payload, unsigned int length, struct ZbZclColorClientMoveToColorXYReqT *req);
int ZbZclColorClientMoveColorXYBuild(uint8_t *payload, unsigned int length, struct ZbZclColorClientMoveColorXYReqT *req);
int ZbZclColorClientStepColorXYBuild(uint8_t *payload, unsigned int length, struct ZbZclColorClientStepColorXYReqT *req);
int ZbZclColorClientMoveToColorTempBuild(uint8_t *payload, unsigned int length, struct ZbZclColorClientMoveToColorTempReqT *req);
int ZbZclColorClientMoveToHueEnhBuild(uint8_t *payload, unsigned int length, struct ZbZclColorClientMoveToHueEnhReqT *req);
int ZbZclColorClientMoveHueEnhBuild(uint8_t *payload, unsigned int length, struct ZbZclColorClientMoveHueEnhReqT *req);
int ZbZclColorClientStepHueEnhBuild(uint8_t *payload, unsigned int length, struct ZbZclColorClientStepHueEnhReqT *req);
int ZbZclColorClientMoveToHueSatEnhBuild(uint8_t *payload, unsigned int length, struct ZbZclColorClientMoveToHueSatEnhReqT *req);
int ZbZclColorClientColorLoopSetBuild(uint8_t *payload, unsigned int length, struct ZbZclColorClientColorLoopSetReqT *req);
int ZbZclColorClientStopMoveStepBuild(uint8_t *payload, unsigned int length, struct ZbZclColorClientStopMoveStepReqT *req);
int ZbZclColorClientMoveColorTempBuild(uint8_t *payload, unsigned int length, struct ZbZclColorClientMoveColorTempReqT *req);
int ZbZclColorClientStepColorTempBuild(uint8_t *payload, unsigned int length, struct ZbZclColorClientStepColorTempReqT *req);

#endif
