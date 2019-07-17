
/**
  ******************************************************************************
  *  @file   crs_stm.h
  * @author  MCD Application Team
  * @version V0.0.1.alpha
  * @date    04-September-2018
  * @brief   Header for crs_stm.c module
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
#ifndef __STM32XX_CRS_H
#define __STM32XX_CRS_H

#ifdef __cplusplus
extern "C" 
{
#endif

/* Includes ------------------------------------------------------------------*/ 
/* Exported types ------------------------------------------------------------*/
typedef enum
{
  CRS_NOTIFY_ENABLED_EVT,
  CRS_NOTIFY_DISABLED_EVT,
  CRS_READ_EVT,
  CRS_WRITE_EVT,
} CRS_Opcode_evt_t;

typedef struct
{
  uint8_t * pPayload;
  uint8_t   Length;
}CRS_Data_t;  

typedef struct
{
  CRS_Opcode_evt_t                              CRS_Evt_Opcode;
  CRS_Data_t                                    DataTransfered;
  uint16_t                                      ConnectionHandle;
  uint8_t                                       ServiceInstance;
}CRSAPP_Notification_evt_t;


/* Exported constants --------------------------------------------------------*/
#define CRS_MAX_DATA_LEN                                           (ATT_MTU - 3) /**< Maximum length of data (in bytes) that can be transmitted to the peer. */

/* External variables --------------------------------------------------------*/
/* Exported macros -----------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void CRS_STM_Init(void);
void CRSAPP_Notification(CRSAPP_Notification_evt_t *pNotification);
tBleStatus CRSAPP_Update_Char(uint16_t UUID,  uint8_t *pPayload);


#ifdef __cplusplus
}
#endif

#endif /*__STM32XX_CRS_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
