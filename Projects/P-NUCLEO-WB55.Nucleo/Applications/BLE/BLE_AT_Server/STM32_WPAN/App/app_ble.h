/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : App/app_ble.h
  * Description        : Application configuration file for BLE Middleware.
  *
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2019-2021 STMicroelectronics.
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
#ifndef APP_BLE_H
#define APP_BLE_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "hci_tl.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/

    typedef enum
    {
      APP_BLE_IDLE,
      APP_BLE_FAST_ADV,
      APP_BLE_LP_ADV,
      APP_BLE_SCAN,
      APP_BLE_LP_CONNECTING,
      APP_BLE_CONNECTED_SERVER,
      APP_BLE_CONNECTED_CLIENT
    } APP_BLE_ConnStatus_t;

    typedef struct
    {
      uint8_t *pPayload;
      uint8_t Length;
    } CUSTOMS_Payload_t;

    typedef enum
    {
      CUSTOM_NOTIFICATION_ENABLED,
      CUSTOM_NOTIFICATION_DISABLED,
      CUSTOM_INDICATION_ENABLED,
      CUSTOM_INDICATION_DISABLED,
      CUSTOM_WRITE_EVT,
      CUSTOMS_STM_GATT_TX_POOL_AVAILABLE,
      CUSTOMS_STM_DATA_RECEIVED,
      CUSTOMS_MTU_EXCHANGED,
    } CUSTOMS_NotCode_t;

    typedef struct
    {
      uint8_t ATT_MTU_exchanged;
      uint8_t service_index;
      uint8_t charac_index;
      uint8_t length;
      uint8_t charac_value[245];
      CUSTOMS_NotCode_t Evt_Opcode;
      CUSTOMS_Payload_t DataTransfered;
    } CUSTOMS_App_Notification_evt_t;

    /**
 * security parameters structure
 */
typedef struct _tSecurityParams
{
  /**
   * IO capability of the device
   */
  uint8_t ioCapability;

  /**
   * Authentication requirement of the device
   * Man In the Middle protection required?
   */
  uint8_t mitm_mode;

  /**
   * bonding mode of the device
   */
  uint8_t bonding_mode;
  
  uint8_t sc_support;

  /**
   * this variable indicates whether to use a fixed pin
   * during the pairing process or a passkey has to be
   * requested to the application during the pairing process
   * 0 implies use fixed pin and 1 implies request for passkey
   */
  uint8_t Use_Fixed_Pin;

  /**
   * minimum encryption key size requirement
   */
  uint8_t encryptionKeySizeMin;

  /**
   * maximum encryption key size requirement
   */
  uint8_t encryptionKeySizeMax;

  /**
   * fixed pin to be used in the pairing process if
   * Use_Fixed_Pin is set to 1
   */
  uint32_t Fixed_Pin;

  /**
   * this flag indicates whether the host has to initiate
   * the security, wait for pairing or does not have any security
   * requirements.\n
   * 0x00 : no security required
   * 0x01 : host should initiate security by sending the slave security
   *        request command
   * 0x02 : host need not send the clave security request but it
   * has to wait for paiirng to complete before doing any other
   * processing
   */
  uint8_t initiateSecurity;
}tSecurityParams;

    /**
 * global context
 * contains the variables common to all
 * services
 */
typedef struct _tBLEProfileGlobalContext
{

  /**
   * security requirements of the host
   */
  tSecurityParams bleSecurityParam;

  /**
   * gap service handle
   */
  uint16_t gapServiceHandle;

  /**
   * device name characteristic handle
   */
  uint16_t devNameCharHandle;

  /**
   * appearance characteristic handle
   */
  uint16_t appearanceCharHandle;

  /**
   * connection handle of the current active connection
   * When not in connection, the handle is set to 0xFFFF
   */
  uint16_t connectionHandle;

  /**
   * length of the UUID list to be used while advertising
   */
  uint8_t advtServUUIDlen;

  /**
   * the UUID list to be used while advertising
   */
  uint8_t advtServUUID[100];

}BleGlobalContext_t;

    typedef struct
    {
      BleGlobalContext_t BleApplicationContext_legacy;
      APP_BLE_ConnStatus_t Device_Connection_Status;
      
      uint8_t SwitchOffGPIO_timer_Id;
    }BleApplicationContext_t;
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* External variables --------------------------------------------------------*/
/* USER CODE BEGIN EV */

/* USER CODE END EV */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions ---------------------------------------------*/
  void APP_BLE_Init( void );

  APP_BLE_ConnStatus_t APP_BLE_Get_Server_Connection_Status(void);
  void CUSTOMS_Notification( CUSTOMS_App_Notification_evt_t *pNotification );

/* USER CODE BEGIN EF */
void APP_BLE_Key_Button1_Action(void);
void APP_BLE_Key_Button2_Action(void);
void APP_BLE_Key_Button3_Action(void);

/* USER CODE END EF */

#ifdef __cplusplus
}
#endif

#endif /* APP_BLE_H */

