/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : app_zigbee.h
  * Description        : Header for Zigbee Application.
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
#ifndef APP_ZIGBEE_H
#define APP_ZIGBEE_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
/* Private includes ----------------------------------------------------------*/
#include "tl.h"
#include "tl_zigbee_hci.h"

/* Exported types ------------------------------------------------------------*/

/*
 *  List of all errors tracked by the Thread application
 *  running on M4. Some of these errors may be fatal
 *  or just warnings
 */
typedef enum {
    ERR_REC_MULTI_MSG_FROM_M0,
    ERR_ZIGBE_CMD_TO_M0,
    ERR_ZIGBEE_CHECK_WIRELESS
} ErrAppliIdEnum_t;

/* Exported constants --------------------------------------------------------*/

/* Exported functions ------------------------------------------------------- */
void APP_ZIGBEE_Init(void);
void APP_ZIGBEE_Error(uint32_t ErrId, uint32_t ErrCode);
void APP_ZIGBEE_RegisterCmdBuffer(TL_CmdPacket_t *p_buffer);
void APP_ZIGBEE_ProcessMsgM0ToM4(void);
void APP_ZIGBEE_TL_INIT(void);
void Pre_ZigbeeCmdProcessing(void);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* APP_ZIGBEE_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
