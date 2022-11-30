/**
  ******************************************************************************
 * @file    dts.h
 * @author  MCD Application Team
 * @brief   Header for ble_application.c module
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


/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef DTS_STM_H
#define DTS_STM_H

#ifdef __cplusplus
extern "C"
{
#endif

/* Includes ------------------------------------------------------------------*/
#include "ble_types.h"
#include "ble_conf.h"

/* Exported types ------------------------------------------------------------*/
/* Exported types LBR Client Implementation-----------------------------------*/
typedef struct
{
  uint8_t *pPayload;
//  uint32_t *pPayload;
  uint32_t pPayload_n_1;
  uint32_t pPayload_n;
  uint32_t Length;
} DTS_STM_Payload_t;

typedef enum
{
  DTS_TX_NOTIFICATION_ENABLED,
  DTS_TX_NOTIFICATION_DISABLED,
  DTS_STM_GATT_TX_POOL_AVAILABLE,
  DTS_THROUGHPUT_NOTIFICATION_ENABLED,
  DTS_THROUGHPUT_NOTIFICATION_DISABLED,
  DTS_DATA_RECEIVED,
  DTS_MTU_EXCHANGED,
} DTS_STM_NotCode_t;

typedef struct
{
  uint8_t ATT_MTU_exchanged;
  DTS_STM_NotCode_t Evt_Opcode;
  DTS_STM_Payload_t DataTransfered;
} DTS_STM_App_Notification_evt_t;

/* Exported constants --------------------------------------------------------*/
/* External variables --------------------------------------------------------*/
/* Exported macros -----------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void DTS_STM_Init( void );
tBleStatus DTS_STM_UpdateChar( uint16_t UUID , uint8_t *pPayload );
void DTS_Notification( DTS_STM_App_Notification_evt_t *pNotification );
void BLE_SVC_GAP_Change_PHY(void);
void BLE_SVC_GAP_Security_Req(void);
void BLE_GAP_Pairing_Req(void);
void BLE_SVC_GAP_Clear_DataBase(void);
void Resume_Notification(void);
//tBleStatus DTS_STM_UpdateCharThroughput(DTS_STM_Payload_t *pDataValue );

#ifdef __cplusplus
}
#endif

#endif /* DTS_STM_H */
