/**
 ******************************************************************************
 * @file    dts.h
 * @author  MCD Application Team
 * @brief   Header for ble_application.c module
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
#ifndef __DTS_STM_H
#define __DTS_STM_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "ble_types.h"
#include "ble_conf.h"

/* Includes ------------------------------------------------------------------*/
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
  DTS_STM__NOTIFICATION_ENABLED,
  DTS_STM_NOTIFICATION_DISABLED,
  DTS_STM_GATT_TX_POOL_AVAILABLE,
  DTC_NOTIFICATION_ENABLED,
  DTC_NOTIFICATION_DISABLED,
  DTS_STM_DATA_RECEIVED,
  DTS_MTU_EXCHANGED,
} DTS_STM_NotCode_t;

typedef struct
{
  uint8_t ATT_MTU_exchanged;
  DTS_STM_NotCode_t Evt_Opcode;
  DTS_STM_Payload_t DataTransfered;
} DTS_STM_App_Notification_evt_t;

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* External variables --------------------------------------------------------*/
/* Exported macros -----------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void DTS_STM_Init( void );
tBleStatus DTS_STM_UpdateChar( uint16_t UUID , uint8_t *pPayload );
void DTS_Notification( DTS_STM_App_Notification_evt_t *pNotification );
void BLE_SVC_GAP_Change_PHY(void);
void BLE_SVC_GAP_Security_Req(void);
void BLE_SVC_GAP_Clear_DataBase(void);
void Resume_Notification(void);
tBleStatus DTS_STM_UpdateCharThroughput(DTS_STM_Payload_t *pDataValue );

#ifdef __cplusplus
}
#endif

#endif /*__DTS_STM_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
