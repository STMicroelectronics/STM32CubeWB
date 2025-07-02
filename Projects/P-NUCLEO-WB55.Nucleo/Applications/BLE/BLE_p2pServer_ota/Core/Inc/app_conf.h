/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    app_conf.h
  * @author  MCD Application Team
  * @brief   Application configuration file for STM32WPAN Middleware.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2020-2021 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef APP_CONF_H
#define APP_CONF_H

#include "hw.h"
#include "hw_conf.h"
#include "hw_if.h"
#include "ble_bufsize.h"

/******************************************************************************
 * Application Config
 ******************************************************************************/

/**< generic parameters ******************************************************/

/**
 * Define Tx Power
 */
#define CFG_TX_POWER                      (0x18) /* -0.15dBm */

/**
 * Define Advertising parameters
 */
#define CFG_ADV_BD_ADDRESS                (0x7257acd87a6c)

/* BD_ADDR type: define proper address. Can only be GAP_PUBLIC_ADDR (0x00) or GAP_STATIC_RANDOM_ADDR (0x01) */
#define CFG_IDENTITY_ADDRESS           GAP_PUBLIC_ADDR   /* GAP_STATIC_RANDOM_ADDR */
//#define CFG_STATIC_RANDOM_ADDRESS               (0x1234567890ab) /**< Static Random Address fixed for lifetime of the device */

/* Define privacy: PRIVACY_DISABLED or PRIVACY_ENABLED */
#define CFG_PRIVACY                       PRIVACY_DISABLED /* PRIVACY_ENABLED */

/* if CFG_PRIVACY equals PRIVACY_DISABLED,  CFG_BLE_ADDRESS_TYPE has 2 allowed values: GAP_PUBLIC_ADDR or GAP_STATIC_RANDOM_ADDR */
#define CFG_BLE_ADDRESS_TYPE              CFG_IDENTITY_ADDRESS /**< Bluetooth address types defined in ble_legacy.h */

/* if CFG_PRIVACY equals PRIVACY_ENABLED, CFG_BLE_ADDRESS_TYPE has 2 allowed values: GAP_RESOLVABLE_PRIVATE_ADDR or GAP_NON_RESOLVABLE_PRIVATE_ADDR */
//#define CFG_BLE_ADDRESS_TYPE              GAP_RESOLVABLE_PRIVATE_ADDR /* GAP_NON_RESOLVABLE_PRIVATE_ADDR */

#define CFG_FAST_CONN_ADV_INTERVAL_MIN    (0x80)      /**< 80ms */
#define CFG_FAST_CONN_ADV_INTERVAL_MAX    (0xa0)      /**< 100ms */
#define CFG_LP_CONN_ADV_INTERVAL_MIN      (0x640)     /**< 1s */
#define CFG_LP_CONN_ADV_INTERVAL_MAX      (0xfa0)     /**< 2.5s */

/**
 * Define IO Authentication
 */
#define CFG_BONDING_MODE                 (1)
#define CFG_FIXED_PIN                    (111111)
#define CFG_USED_FIXED_PIN               (0)
#define CFG_ENCRYPTION_KEY_SIZE_MAX      (16)
#define CFG_ENCRYPTION_KEY_SIZE_MIN      (8)

/**
 * Define IO capabilities
 */
#define CFG_IO_CAPABILITY_DISPLAY_ONLY        (0x00)
#define CFG_IO_CAPABILITY_DISPLAY_YES_NO      (0x01)
#define CFG_IO_CAPABILITY_KEYBOARD_ONLY       (0x02)
#define CFG_IO_CAPABILITY_NO_INPUT_NO_OUTPUT  (0x03)
#define CFG_IO_CAPABILITY_KEYBOARD_DISPLAY    (0x04)

#define CFG_IO_CAPABILITY                     CFG_IO_CAPABILITY_DISPLAY_YES_NO

/**
 * Define MITM modes
 */
#define CFG_MITM_PROTECTION_NOT_REQUIRED      (0x00)
#define CFG_MITM_PROTECTION_REQUIRED          (0x01)

#define CFG_MITM_PROTECTION                   CFG_MITM_PROTECTION_REQUIRED

/**
 * Define Secure Connections Support
 */
#define CFG_SECURE_NOT_SUPPORTED              (0x00)
#define CFG_SECURE_OPTIONAL                   (0x01)
#define CFG_SECURE_MANDATORY                  (0x02)

#define CFG_SC_SUPPORT                        CFG_SECURE_OPTIONAL

/**
 * Define Keypress Notification Support
 */
#define CFG_KEYPRESS_NOT_SUPPORTED            (0x00)
#define CFG_KEYPRESS_SUPPORTED                (0x01)

#define CFG_KEYPRESS_NOTIFICATION_SUPPORT     CFG_KEYPRESS_NOT_SUPPORTED

/**
 * Numeric Comparison Answers
 */
#define YES (0x01)
#define NO  (0x00)

/**
 * Device name configuration for Generic Access Service
 */
#define CFG_GAP_DEVICE_NAME             "TEMPLATE"
#define CFG_GAP_DEVICE_NAME_LENGTH      (8)

/**
 * Define PHY
 */
#define ALL_PHYS_PREFERENCE                             0x00
#define RX_2M_PREFERRED                                 0x02
#define TX_2M_PREFERRED                                 0x02
#define TX_1M                                           0x01
#define TX_2M                                           0x02
#define RX_1M                                           0x01
#define RX_2M                                           0x02

/**
*   Identity root key used to derive IRK and DHK(Legacy)
*/
#define CFG_BLE_IR     {0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC, 0xDE, 0xF0, 0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC, 0xDE, 0xF0}

/**
* Encryption root key used to derive LTK(Legacy) and CSRK
*/
#define CFG_BLE_ER     {0xFE, 0xDC, 0xBA, 0x09, 0x87, 0x65, 0x43, 0x21, 0xFE, 0xDC, 0xBA, 0x09, 0x87, 0x65, 0x43, 0x21}

/**
 * SMPS supply
 * SMPS not used when Set to 0
 * SMPS used when Set to 1
 */
#define CFG_USE_SMPS    0

/* USER CODE BEGIN Generic_Parameters */
/* USER CODE END Generic_Parameters */

/**< specific parameters */
/*****************************************************/

#define P2P_SERVER1    1    /*1 = Device is Peripherique*/
#define P2P_SERVER2    0
#define P2P_SERVER3    0
#define P2P_SERVER4    0
#define P2P_SERVER5    0
#define P2P_SERVER6    0

#define CFG_DEV_ID_P2P_SERVER1                  (0x83)
#define CFG_DEV_ID_P2P_SERVER2                  (0x84)
#define CFG_DEV_ID_P2P_SERVER3                  (0x87)
#define CFG_DEV_ID_P2P_SERVER4                  (0x88)
#define CFG_DEV_ID_P2P_SERVER5                  (0x89)
#define CFG_DEV_ID_P2P_SERVER6                  (0x8A)
#define CFG_DEV_ID_P2P_ROUTER                   (0x85)

#define  RADIO_ACTIVITY_EVENT   1          /* 1 for OOB Demo */

/**
* AD Element - Group B Feature
*/
/* LSB - First Byte */
#define CFG_FEATURE_THREAD_SWITCH               (0x40)

/* LSB - Second Byte */
#define CFG_FEATURE_OTA_REBOOT                  (0x20)

#define CONN_L(x) ((int)((x)/0.625f))
#define CONN_P(x) ((int)((x)/1.25f))

  /*  L2CAP Connection Update request parameters used for test only with smart Phone */
#define L2CAP_REQUEST_NEW_CONN_PARAM             1

#define L2CAP_INTERVAL_MIN              CONN_P(1000) /* 1s */
#define L2CAP_INTERVAL_MAX              CONN_P(1000) /* 1s */
#define L2CAP_PERIPHERAL_LATENCY             0x0000
#define L2CAP_TIMEOUT_MULTIPLIER        0x1F4

/* USER CODE BEGIN Specific_Parameters */

/* USER CODE END Specific_Parameters */

/******************************************************************************
 * BLE Stack
 ******************************************************************************/
/**
 * Maximum number of simultaneous connections that the device will support.
 * Valid values are from 1 to 8
 */
#define CFG_BLE_NUM_LINK            2

/**
 * Maximum number of Services that can be stored in the GATT database.
 * Note that the GAP and GATT services are automatically added so this parameter should be 2 plus the number of user services
 */
#define CFG_BLE_NUM_GATT_SERVICES   8

/**
 * Maximum number of Attributes
 * (i.e. the number of characteristic + the number of characteristic values + the number of descriptors, excluding the services)
 * that can be stored in the GATT database.
 * Note that certain characteristics and relative descriptors are added automatically during device initialization
 * so this parameters should be 9 plus the number of user Attributes
 */
#define CFG_BLE_NUM_GATT_ATTRIBUTES 68

/**
 * Maximum supported ATT_MTU size
 * This parameter is ignored by the CPU2 when CFG_BLE_OPTIONS has SHCI_C2_BLE_INIT_OPTIONS_LL_ONLY flag set
 */
#define CFG_BLE_MAX_ATT_MTU             (156)

/**
 * Size of the storage area for Attribute values
 *  This value depends on the number of attributes used by application. In particular the sum of the following quantities (in octets) should be made for each attribute:
 *  - attribute value length
 *  - 5, if UUID is 16 bit; 19, if UUID is 128 bit
 *  - 2, if server configuration descriptor is used
 *  - 2*DTM_NUM_LINK, if client configuration descriptor is used
 *  - 2, if extended properties is used
 *  The total amount of memory needed is the sum of the above quantities for each attribute.
 * This parameter is ignored by the CPU2 when CFG_BLE_OPTIONS has SHCI_C2_BLE_INIT_OPTIONS_LL_ONLY flag set
 */
#define CFG_BLE_ATT_VALUE_ARRAY_SIZE    (1344)

/**
 * Prepare Write List size in terms of number of packet
 * This parameter is ignored by the CPU2 when CFG_BLE_OPTIONS has SHCI_C2_BLE_INIT_OPTIONS_LL_ONLY flag set
 */
#define CFG_BLE_PREPARE_WRITE_LIST_SIZE         BLE_PREP_WRITE_X_ATT(CFG_BLE_MAX_ATT_MTU)

/**
 * Number of allocated memory blocks
 * This parameter is overwritten by the CPU2 with an hardcoded optimal value when the parameter CFG_BLE_OPTIONS has SHCI_C2_BLE_INIT_OPTIONS_LL_ONLY flag set
 */
#define CFG_BLE_MBLOCK_COUNT            (BLE_MBLOCKS_CALC(CFG_BLE_PREPARE_WRITE_LIST_SIZE, CFG_BLE_MAX_ATT_MTU, CFG_BLE_NUM_LINK))

/**
 * Enable or disable the Extended Packet length feature. Valid values are 0 or 1.
 */
#define CFG_BLE_DATA_LENGTH_EXTENSION   1

/**
 * Sleep clock accuracy in Peripheral mode (ppm value)
 */
#define CFG_BLE_PERIPHERAL_SCA   500

/**
 * Sleep clock accuracy in Central mode
 * 0 : 251 ppm to 500 ppm
 * 1 : 151 ppm to 250 ppm
 * 2 : 101 ppm to 150 ppm
 * 3 : 76 ppm to 100 ppm
 * 4 : 51 ppm to 75 ppm
 * 5 : 31 ppm to 50 ppm
 * 6 : 21 ppm to 30 ppm
 * 7 : 0 ppm to 20 ppm
 */
#define CFG_BLE_CENTRAL_SCA   0

/**
 * LsSource
 * Some information for Low speed clock mapped in bits field
 * - bit 0:   1: Calibration for the RF system wakeup clock source   0: No calibration for the RF system wakeup clock source
 * - bit 1:   1: STM32WB5M Module device                             0: Other devices as STM32WBxx SOC, STM32WB1M module
 * - bit 2:   1: HSE/1024 Clock config                               0: LSE Clock config   
 */
#if defined(STM32WB5Mxx)
  #define CFG_BLE_LS_SOURCE  (SHCI_C2_BLE_INIT_CFG_BLE_LS_NOCALIB | SHCI_C2_BLE_INIT_CFG_BLE_LS_MOD5MM_DEV | SHCI_C2_BLE_INIT_CFG_BLE_LS_CLK_LSE)
#else
  #define CFG_BLE_LS_SOURCE  (SHCI_C2_BLE_INIT_CFG_BLE_LS_NOCALIB | SHCI_C2_BLE_INIT_CFG_BLE_LS_OTHER_DEV | SHCI_C2_BLE_INIT_CFG_BLE_LS_CLK_LSE)
#endif

/**
 * Start up time of the high speed (16 or 32 MHz) crystal oscillator in units of 625/256 us (~2.44 us)
 */
#define CFG_BLE_HSE_STARTUP_TIME  0x148

/**
 * Maximum duration of the connection event when the device is in Peripheral mode in units of 625/256 us (~2.44 us)
 */
#define CFG_BLE_MAX_CONN_EVENT_LENGTH  (0xFFFFFFFF)

/**
 * Viterbi Mode
 * 1 : enabled
 * 0 : disabled
 */
#define CFG_BLE_VITERBI_MODE  1

/**
 * BLE stack Options flags to be configured with:
 * - SHCI_C2_BLE_INIT_OPTIONS_LL_ONLY
 * - SHCI_C2_BLE_INIT_OPTIONS_LL_HOST
 * - SHCI_C2_BLE_INIT_OPTIONS_NO_SVC_CHANGE_DESC
 * - SHCI_C2_BLE_INIT_OPTIONS_WITH_SVC_CHANGE_DESC
 * - SHCI_C2_BLE_INIT_OPTIONS_DEVICE_NAME_RO
 * - SHCI_C2_BLE_INIT_OPTIONS_DEVICE_NAME_RW
 * - SHCI_C2_BLE_INIT_OPTIONS_EXT_ADV
 * - SHCI_C2_BLE_INIT_OPTIONS_NO_EXT_ADV
 * - SHCI_C2_BLE_INIT_OPTIONS_CS_ALGO2
 * - SHCI_C2_BLE_INIT_OPTIONS_NO_CS_ALGO2
 * - SHCI_C2_BLE_INIT_OPTIONS_REDUC_GATTDB_NVM 
 * - SHCI_C2_BLE_INIT_OPTIONS_FULL_GATTDB_NVM
 * - SHCI_C2_BLE_INIT_OPTIONS_GATT_CACHING_USED
 * - SHCI_C2_BLE_INIT_OPTIONS_GATT_CACHING_NOTUSED
 * - SHCI_C2_BLE_INIT_OPTIONS_POWER_CLASS_1
 * - SHCI_C2_BLE_INIT_OPTIONS_POWER_CLASS_2_3
 * which are used to set following configuration bits:
 * (bit 0): 1: LL only
 *          0: LL + host
 * (bit 1): 1: no service change desc.
 *          0: with service change desc.
 * (bit 2): 1: device name Read-Only
 *          0: device name R/W
 * (bit 3): 1: extended advertizing supported
 *          0: extended advertizing not supported
 * (bit 4): 1: CS Algo #2 supported
 *          0: CS Algo #2 not supported
 * (bit 5): 1: Reduced GATT database in NVM 
 *          0: Full GATT database in NVM 
 * (bit 6): 1: GATT caching is used
 *          0: GATT caching is not used
 * (bit 7): 1: LE Power Class 1
 *          0: LE Power Class 2-3
 * other bits: complete with Options_extension flag
 */
#define CFG_BLE_OPTIONS  (SHCI_C2_BLE_INIT_OPTIONS_LL_HOST | SHCI_C2_BLE_INIT_OPTIONS_WITH_SVC_CHANGE_DESC | SHCI_C2_BLE_INIT_OPTIONS_DEVICE_NAME_RW | SHCI_C2_BLE_INIT_OPTIONS_NO_EXT_ADV | SHCI_C2_BLE_INIT_OPTIONS_NO_CS_ALGO2 | SHCI_C2_BLE_INIT_OPTIONS_FULL_GATTDB_NVM | SHCI_C2_BLE_INIT_OPTIONS_GATT_CACHING_NOTUSED | SHCI_C2_BLE_INIT_OPTIONS_POWER_CLASS_2_3)

/**
 * BLE stack Options_extension flags to be configured with:
 * - SHCI_C2_BLE_INIT_OPTIONS_APPEARANCE_WRITABLE
 * - SHCI_C2_BLE_INIT_OPTIONS_APPEARANCE_READONLY
 * - SHCI_C2_BLE_INIT_OPTIONS_ENHANCED_ATT_SUPPORTED
 * - SHCI_C2_BLE_INIT_OPTIONS_ENHANCED_ATT_NOTSUPPORTED
 * which are used to set following configuration bits:
 * (bit 0): 1: appearance Writable
 *          0: appearance Read-Only
 * (bit 1): 1: Enhanced ATT supported
 *          0: Enhanced ATT not supported
 * other bits: reserved (shall be set to 0)
 */
#define CFG_BLE_OPTIONS_EXT  (SHCI_C2_BLE_INIT_OPTIONS_APPEARANCE_READONLY | SHCI_C2_BLE_INIT_OPTIONS_ENHANCED_ATT_NOTSUPPORTED)

#define CFG_BLE_MAX_COC_INITIATOR_NBR   (32)

#define CFG_BLE_MIN_TX_POWER            (-40)

#define CFG_BLE_MAX_TX_POWER            (6)

/**
* BLE stack Maximum number of created Enhanced ATT bearers to be configured
* in addition to the number of links
*     - Range: 0 .. 4
*/ 
#define CFG_BLE_MAX_ADD_EATT_BEARERS    (4)
        

/**
 * BLE Rx model configuration flags to be configured with:
 * - SHCI_C2_BLE_INIT_RX_MODEL_AGC_RSSI_LEGACY
 * - SHCI_C2_BLE_INIT_RX_MODEL_AGC_RSSI_BLOCKER
 * which are used to set following configuration bits:
 * (bit 0): 1: agc_rssi model improved vs RF blockers
 *          0: Legacy agc_rssi model
 * other bits: reserved (shall be set to 0)
 */

#define CFG_BLE_RX_MODEL_CONFIG         SHCI_C2_BLE_INIT_RX_MODEL_AGC_RSSI_LEGACY

/* Maximum number of advertising sets.
 * Range: 1 .. 8 with limitation:
 * This parameter is linked to CFG_BLE_MAX_ADV_DATA_LEN such as both compliant with allocated Total memory computed with BLE_EXT_ADV_BUFFER_SIZE based
 * on Max Extended advertising configuration supported.
 * This parameter is considered by the CPU2 when CFG_BLE_OPTIONS has SHCI_C2_BLE_INIT_OPTIONS_EXT_ADV flag set
 */

#define CFG_BLE_MAX_ADV_SET_NBR     (3)

 /* Maximum advertising data length (in bytes)
 * Range: 31 .. 1650 with limitation:
 * This parameter is linked to CFG_BLE_MAX_ADV_SET_NBR such as both compliant with allocated Total memory computed with BLE_EXT_ADV_BUFFER_SIZE based
 * on Max Extended advertising configuration supported.
 * This parameter is considered by the CPU2 when CFG_BLE_OPTIONS has SHCI_C2_BLE_INIT_OPTIONS_EXT_ADV flag set
 */

#define CFG_BLE_MAX_ADV_DATA_LEN    (1650)

 /* RF TX Path Compensation Value (16-bit signed integer). Units: 0.1 dB.
  * Range: -1280 .. 1280
  */

#define CFG_BLE_TX_PATH_COMPENS    (0)

 /* RF RX Path Compensation Value (16-bit signed integer). Units: 0.1 dB.
  * Range: -1280 .. 1280
  */

#define CFG_BLE_RX_PATH_COMPENS    (0)

  /* BLE core version (16-bit signed integer).
   * - SHCI_C2_BLE_INIT_BLE_CORE_5_2
   * - SHCI_C2_BLE_INIT_BLE_CORE_5_3
   * - SHCI_C2_BLE_INIT_BLE_CORE_5_4
   * which are used to set: 11(5.2), 12(5.3), 13(5.4).
   */
   
#define CFG_BLE_CORE_VERSION   (SHCI_C2_BLE_INIT_BLE_CORE_5_4)


/******************************************************************************
 * Transport Layer
 ******************************************************************************/
/**
 * Queue length of BLE Event
 * This parameter defines the number of asynchronous events that can be stored in the HCI layer before
 * being reported to the application. When a command is sent to the BLE core coprocessor, the HCI layer
 * is waiting for the event with the Num_HCI_Command_Packets set to 1. The receive queue shall be large
 * enough to store all asynchronous events received in between.
 * When CFG_TLBLE_MOST_EVENT_PAYLOAD_SIZE is set to 27, this allow to store three 255 bytes long asynchronous events
 * between the HCI command and its event.
 * This parameter depends on the value given to CFG_TLBLE_MOST_EVENT_PAYLOAD_SIZE. When the queue size is to small,
 * the system may hang if the queue is full with asynchronous events and the HCI layer is still waiting
 * for a CC/CS event, In that case, the notification TL_BLE_HCI_ToNot() is called to indicate
 * to the application a HCI command did not receive its command event within 30s (Default HCI Timeout).
 */
#define CFG_TLBLE_EVT_QUEUE_LENGTH 5
/**
 * This parameter should be set to fit most events received by the HCI layer. It defines the buffer size of each element
 * allocated in the queue of received events and can be used to optimize the amount of RAM allocated by the Memory Manager.
 * It should not exceed 255 which is the maximum HCI packet payload size (a greater value is a lost of memory as it will
 * never be used)
 * It shall be at least 4 to receive the command status event in one frame.
 * The default value is set to 27 to allow receiving an event of MTU size in a single buffer. This value maybe reduced
 * further depending on the application.
 */
#define CFG_TLBLE_MOST_EVENT_PAYLOAD_SIZE 255   /**< Set to 255 with the memory manager and the mailbox */

#define TL_BLE_EVENT_FRAME_SIZE ( TL_EVT_HDR_SIZE + CFG_TLBLE_MOST_EVENT_PAYLOAD_SIZE )
/******************************************************************************
 * UART interfaces
 ******************************************************************************/

/**
 * Select UART interfaces
 */
#define CFG_DEBUG_TRACE_UART    hw_uart1
#define CFG_CONSOLE_MENU        hw_lpuart1
/******************************************************************************
 * USB interface
 ******************************************************************************/

/**
 * Enable/Disable USB interface
 */
#define CFG_USB_INTERFACE_ENABLE    0

/******************************************************************************
 * IPCC interface
 ******************************************************************************/

/**
 * The IPCC is dedicated to the communication between the CPU2 and the CPU1
 * and shall not be modified by the application
 * The two following definitions shall not be modified
 */
#define HAL_IPCC_TX_IRQHandler(...)  HW_IPCC_Tx_Handler( )
#define HAL_IPCC_RX_IRQHandler(...)  HW_IPCC_Rx_Handler( )

/******************************************************************************
 * Low Power
 ******************************************************************************/
/**
 *  When set to 1, the low power mode is enable
 *  When set to 0, the device stays in RUN mode
 */
#define CFG_LPM_SUPPORTED    1

/******************************************************************************
 * RTC interface
 ******************************************************************************/
#define HAL_RTCEx_WakeUpTimerIRQHandler(...)  HW_TS_RTC_Wakeup_Handler( )

/******************************************************************************
 * Timer Server
 ******************************************************************************/
/**
 *  CFG_RTC_WUCKSEL_DIVIDER:  This sets the RTCCLK divider to the wakeup timer.
 *  The lower is the value, the better is the power consumption and the accuracy of the timerserver
 *  The higher is the value, the finest is the granularity
 *
 *  CFG_RTC_ASYNCH_PRESCALER: This sets the asynchronous prescaler of the RTC. It should as high as possible ( to output
 *  clock as low as possible) but the output clock should be equal or higher frequency compare to the clock feeding
 *  the wakeup timer. A lower clock speed would impact the accuracy of the timer server.
 *
 *  CFG_RTC_SYNCH_PRESCALER: This sets the synchronous prescaler of the RTC.
 *  When the 1Hz calendar clock is required, it shall be sets according to other settings
 *  When the 1Hz calendar clock is not needed, CFG_RTC_SYNCH_PRESCALER should be set to 0x7FFF (MAX VALUE)
 *
 *  CFG_RTCCLK_DIVIDER_CONF:
 *  Shall be set to either 0,2,4,8,16
 *  When set to either 2,4,8,16, the 1Hhz calendar is supported
 *  When set to 0, the user sets its own configuration
 *
 *  The following settings are computed with LSI as input to the RTC
 */

#define CFG_RTCCLK_DIVIDER_CONF 0

#if (CFG_RTCCLK_DIVIDER_CONF == 0)
/**
 * Custom configuration
 * It does not support 1Hz calendar
 * It divides the RTC CLK by 16
 */

#define CFG_RTCCLK_DIV            (16)
#define CFG_RTC_WUCKSEL_DIVIDER   (0)
#define CFG_RTC_ASYNCH_PRESCALER  (0x0F)
#define CFG_RTC_SYNCH_PRESCALER   (0x7FFF)

#else

#if (CFG_RTCCLK_DIVIDER_CONF == 2)
/**
 * It divides the RTC CLK by 2
 */
#define CFG_RTC_WUCKSEL_DIVIDER (3)
#endif

#if (CFG_RTCCLK_DIVIDER_CONF == 4)
/**
 * It divides the RTC CLK by 4
 */
#define CFG_RTC_WUCKSEL_DIVIDER (2)
#endif

#if (CFG_RTCCLK_DIVIDER_CONF == 8)
/**
 * It divides the RTC CLK by 8
 */
#define CFG_RTC_WUCKSEL_DIVIDER (1)
#endif

#if (CFG_RTCCLK_DIVIDER_CONF == 16)
/**
 * It divides the RTC CLK by 16
 */
#define CFG_RTC_WUCKSEL_DIVIDER (0)
#endif

#define CFG_RTCCLK_DIV              CFG_RTCCLK_DIVIDER_CONF
#define CFG_RTC_ASYNCH_PRESCALER    (CFG_RTCCLK_DIV - 1)
#define CFG_RTC_SYNCH_PRESCALER     (DIVR( LSE_VALUE, (CFG_RTC_ASYNCH_PRESCALER+1) ) - 1 )

#endif

/** tick timer values */
#define CFG_TS_TICK_VAL           DIVR( (CFG_RTCCLK_DIV * 1000000), LSE_VALUE )
#define CFG_TS_TICK_VAL_PS        DIVR( ((uint64_t)CFG_RTCCLK_DIV * 1e12), (uint64_t)LSE_VALUE )

typedef enum
{
  CFG_TIM_PROC_ID_ISR,
  /* USER CODE BEGIN CFG_TimProcID_t */

  /* USER CODE END CFG_TimProcID_t */
} CFG_TimProcID_t;

/******************************************************************************
 * Debug
 ******************************************************************************/
/**
 * When set, this resets some hw resources to put the device in the same state as at power up.
 * It resets only register that may prevent the FW to run properly.
 *
 * This shall be set to 0 in a final product
 *
 */
#define CFG_HW_RESET_BY_FW         1

/**
 * keep debugger enabled while in any low power mode when set to 1
 * should be set to 0 in production
 */
#define CFG_DEBUGGER_SUPPORTED    1

/**
 * When set to 1, the traces are enabled in the BLE services
 */
#define CFG_DEBUG_BLE_TRACE     1

/**
 * Enable or Disable traces in application
 */
#define CFG_DEBUG_APP_TRACE     1

#if (CFG_DEBUG_APP_TRACE != 0)
#define APP_DBG_MSG                 PRINT_MESG_DBG
#else
#define APP_DBG_MSG                 PRINT_NO_MESG
#endif

#if ( (CFG_DEBUG_BLE_TRACE != 0) || (CFG_DEBUG_APP_TRACE != 0) )
#define CFG_DEBUG_TRACE             1
#endif

#if (CFG_DEBUG_TRACE != 0)
#undef CFG_LPM_SUPPORTED
#undef CFG_DEBUGGER_SUPPORTED
#define CFG_LPM_SUPPORTED           0
#define CFG_DEBUGGER_SUPPORTED      1
#endif

/**
 * When CFG_DEBUG_TRACE_FULL is set to 1, the trace are output with the API name, the file name and the line number
 * When CFG_DEBUG_TRACE_LIGHT is set to 1, only the debug message is output
 *
 * When both are set to 0, no trace are output
 * When both are set to 1,  CFG_DEBUG_TRACE_FULL is selected
 */
#define CFG_DEBUG_TRACE_LIGHT     1
#define CFG_DEBUG_TRACE_FULL      0

#if (( CFG_DEBUG_TRACE != 0 ) && ( CFG_DEBUG_TRACE_LIGHT == 0 ) && (CFG_DEBUG_TRACE_FULL == 0))
#undef CFG_DEBUG_TRACE_FULL
#undef CFG_DEBUG_TRACE_LIGHT
#define CFG_DEBUG_TRACE_FULL      0
#define CFG_DEBUG_TRACE_LIGHT     1
#endif

#if ( CFG_DEBUG_TRACE == 0 )
#undef CFG_DEBUG_TRACE_FULL
#undef CFG_DEBUG_TRACE_LIGHT
#define CFG_DEBUG_TRACE_FULL      0
#define CFG_DEBUG_TRACE_LIGHT     0
#endif

/**
 * When not set, the traces is looping on sending the trace over UART
 */
#define DBG_TRACE_USE_CIRCULAR_QUEUE 1

/**
 * max buffer Size to queue data traces and max data trace allowed.
 * Only Used if DBG_TRACE_USE_CIRCULAR_QUEUE is defined
 */
#define DBG_TRACE_MSG_QUEUE_SIZE 4096
#define MAX_DBG_TRACE_MSG_SIZE   1024

/* USER CODE BEGIN Defines */
#define CFG_LED_SUPPORTED         1
#define CFG_BUTTON_SUPPORTED      1

#define PUSH_BUTTON_SW1_EXTI_IRQHandler     EXTI4_IRQHandler
#define PUSH_BUTTON_SW2_EXTI_IRQHandler     EXTI0_IRQHandler
#define PUSH_BUTTON_SW3_EXTI_IRQHandler     EXTI1_IRQHandler
/* USER CODE END Defines */

/******************************************************************************
 * Scheduler
 ******************************************************************************/

/**
 * These are the lists of task id registered to the scheduler
 * Each task id shall be in the range [0:31]
 * This mechanism allows to implement a generic code in the API TL_BLE_HCI_StatusNot() to comply with
 * the requirement that a HCI/ACI command shall never be sent if there is already one pending
 */

/**< Add in that list all tasks that may send a ACI/HCI command */
typedef enum
{
  CFG_TASK_ADV_CANCEL_ID,
#if (L2CAP_REQUEST_NEW_CONN_PARAM != 0 )
  CFG_TASK_CONN_UPDATE_REG_ID,
#endif
  CFG_TASK_HCI_ASYNCH_EVT_ID,
  /* USER CODE BEGIN CFG_Task_Id_With_HCI_Cmd_t */
  CFG_TASK_SW1_BUTTON_PUSHED_ID,
  CFG_TASK_SW2_BUTTON_PUSHED_ID,
  CFG_TASK_SW3_BUTTON_PUSHED_ID,
  /* USER CODE END CFG_Task_Id_With_HCI_Cmd_t */
  CFG_LAST_TASK_ID_WITH_HCICMD,                                               /**< Shall be LAST in the list */
} CFG_Task_Id_With_HCI_Cmd_t;

/**< Add in that list all tasks that never send a ACI/HCI command */
typedef enum
{
  CFG_FIRST_TASK_ID_WITH_NO_HCICMD = CFG_LAST_TASK_ID_WITH_HCICMD - 1,        /**< Shall be FIRST in the list */
  CFG_TASK_SYSTEM_HCI_ASYNCH_EVT_ID,
  /* USER CODE BEGIN CFG_Task_Id_With_NO_HCI_Cmd_t */

  /* USER CODE END CFG_Task_Id_With_NO_HCI_Cmd_t */
  CFG_LAST_TASK_ID_WITH_NO_HCICMD                                            /**< Shall be LAST in the list */
} CFG_Task_Id_With_NO_HCI_Cmd_t;

#define CFG_TASK_NBR    CFG_LAST_TASK_ID_WITH_NO_HCICMD

/**
 * This is the list of priority required by the application
 * Each Id shall be in the range 0..31
 */
typedef enum
{
  CFG_SCH_PRIO_0,
  /* USER CODE BEGIN CFG_SCH_Prio_Id_t */

  /* USER CODE END CFG_SCH_Prio_Id_t */
  CFG_SCH_PRIO_NBR
} CFG_SCH_Prio_Id_t;

/**
 * This is a bit mapping over 32bits listing all events id supported in the application
 */
typedef enum
{
  CFG_IDLEEVT_HCI_CMD_EVT_RSP_ID,
  CFG_IDLEEVT_SYSTEM_HCI_CMD_EVT_RSP_ID,
  /* USER CODE BEGIN CFG_IdleEvt_Id_t */

  /* USER CODE END CFG_IdleEvt_Id_t */
} CFG_IdleEvt_Id_t;

/******************************************************************************
 * LOW POWER
 ******************************************************************************/
/**
 * Supported requester to the MCU Low Power Manager - can be increased up  to 32
 * It list a bit mapping of all user of the Low Power Manager
 */
typedef enum
{
  CFG_LPM_APP,
  CFG_LPM_APP_BLE,
  /* USER CODE BEGIN CFG_LPM_Id_t */

  /* USER CODE END CFG_LPM_Id_t */
} CFG_LPM_Id_t;

/******************************************************************************
 * OTP manager
 ******************************************************************************/
#define CFG_OTP_BASE_ADDRESS    OTP_AREA_BASE

#define CFG_OTP_END_ADRESS      OTP_AREA_END_ADDR

#endif /* APP_CONF_H */

