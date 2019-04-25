
/**
 ******************************************************************************
 * @file    dt_client_app.h
 * @author  MCD Application Team
 * @brief   Header for dt_client_app.c module
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
 * All rights reserved.</center></h2>
 *
 * This software component is licensed by ST under Ultimate Liberty license
 * SLA0044, the "License"; You may not use this file except in compliance with
 * the License. You may obtain a copy of the License at:
 *                             www.st.com/SLA0044
 *
 ******************************************************************************
 */


/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __DTC_APPLICATION_H
#define __DTC_APPLICATION_H

#ifdef __cplusplus
extern "C" {
#endif

  typedef enum
{
  DTC_UNINITIALIZED = 0,
  DTC_INITIALIZED = 1,
  DTC_IDLE = 2,
  DTC_DISCOVER_SERVICES = 3,
  DTC_DISCOVER_CHARACS = 4,
  DTC_DISCOVER_TX_CHAR_DESC = 5,
  DTC_DISCOVER_RX_CHAR_DESC = 6,
  DTC_ENABLE_NOTIFICATION_TX_DESC = 7,
  DTC_DISABLE_NOTIFICATION_TX_DESC = 8,
  DTC_CONNECTED = 9,
  DTC_CONNECTED_ADVERTISING = 10,
  DTC_DISCOVER_NOTIFICATION_CHAR_DESC = 11,
} DTC_ProfileState;

  typedef struct
{
  /**
   * state of the Data Transfer
   * state machine
   */
  DTC_ProfileState state;

  /**
   * connection handle
   */
  uint16_t connHandle;

  /**
   * bitmask of the attributes
   * discovered
   */
  uint8_t featuresFound;

  /**
   * flag which maintains
   * the connection status
   */
  uint8_t disconnected;

  /**
   * handle of the Data Transfer service
   */
  uint16_t DataTransferServiceHandle;

  /**
   * end handle of the Data Transfer service
   */
  uint16_t DataTransferServiceEndHandle;

  /**
   * handle of the Tx characteristic
   *
   */
  uint16_t DataTransferTxCharHdle;

  /**
   * handle of the client configuration
   * descriptor of Tx characteristic
   */
  uint16_t DataTransferTxDescHandle;

  /**
   * handle of the client configuration
   * descriptor of new alert characteristic
   */
  uint16_t DataTransferRxCharHdle; /**< Characteristic handle */
  uint16_t DataTransferRxDescHandle; /**< Characteristic handle */
  uint16_t DTNotificationCharHdle;
  uint16_t ButtonDescHandle;
} DataTransferContext_t;

typedef enum
{  
  DT_IDLE                         = 0,
  DT_DISCOVER_SERVICES	           = 1,
  DT_DISCOVER_CHARACS		   = 2,
  DT_DISCOVER_LED_CHAR_DESC	   = 3,
  DT_DISCOVER_BUTTON_CHAR_DESC	   = 4,
  DT_ENABLE_NOTIFICATION_BUTTON_DESC  = 5,
  DT_DISABLE_NOTIFICATION_TX_DESC = 6,
  DT_ADV                          =7,
  DT_SCAN                         =8,
  DT_CONNECTING                   =9,
  DT_CONNECTED		           = 10,		
} DT_Gap_Gatt_State;  
typedef struct{
  /**
   * state of the Data Transfer
   * state machine
   */ 
  DT_Gap_Gatt_State state;

  /**
   * connection handle 
   */ 
  uint16_t connHandle;

   /**
   * handle of the P2P service
   */ 
  uint16_t P2PServiceHandle;

  /**
   * end handle of the P2P service
   */ 
  uint16_t P2PServiceEndHandle;

  /**
   * handle of the Tx characteristic - Write To Server
   * 
   */ 
  uint16_t P2PWriteToServerCharHdle;

  /**
   * handle of the client configuration
   * descriptor of Tx characteristic
   */ 
  uint16_t P2PWriteToServerDescHandle;

   /**
   * handle of the Rx characteristic - Notification From Server
   * 
   */ 
  uint16_t P2PNotificationCharHdle;

  /**
   * handle of the client configuration
   * descriptor of Rx characteristic
   */ 
  uint16_t P2PNotificationDescHandle;
  uint8_t  NotificationStatus;
  uint8_t WriteStatus;

}DT_ClientContext_t;

typedef enum
{
  DT_CONNECTION_ESTABLISHED,
  DT_PASSKEY_REQ_EVT,
  DT_PAIRING_COMPLETE,
  DT_READ_PHY,
  DT_SET_PHY,
  DT_MTU_EXCHANGED,
  DT_DISCONN_COMPLETE,
} Connection_Evt_t;

typedef struct
{
Connection_Evt_t Evt_Opcode;
uint16_t connection_handle;

} Connection_Context_t;
  /* Includes ------------------------------------------------------------------*/
  /* Exported types ------------------------------------------------------------*/
  /* Exported constants --------------------------------------------------------*/
  /* External variables --------------------------------------------------------*/
  /* Exported macros -----------------------------------------------------------*/
  /* Exported functions ------------------------------------------------------- */
  
void DT_Client_App_Init(void);
void enable_notification(void);
void DT_App_Notification( Connection_Context_t *pNotification );

#ifdef __cplusplus
}
#endif

#endif /*__DTC_APPLICATION_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
