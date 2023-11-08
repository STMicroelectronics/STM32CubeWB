/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : app_zigbee.h
  * Description        : Header for Zigbee Application.
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
#ifndef APP_ZIGBEE_H
#define APP_ZIGBEE_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
/* Private includes ----------------------------------------------------------*/
#include "tl.h"
#include "tl_zigbee_hci.h"
#include "zigbee.h"

/* Exported types ------------------------------------------------------------*/

/*
 *  List of all errors tracked by the Thread application
 *  running on M4. Some of these errors may be fatal
 *  or just warnings
 */
typedef enum {
    ERR_ZIGBE_CMD_TO_M0,
    ERR_ZIGBEE_CHECK_WIRELESS
} ErrAppliIdEnum_t;

/* Exported constants --------------------------------------------------------*/

/* Exported functions ------------------------------------------------------- */
void APP_ZIGBEE_Init(void);
void APP_ZIGBEE_Error(uint32_t ErrId, uint32_t ErrCode);
void APP_ZIGBEE_RegisterCmdBuffer(TL_CmdPacket_t *p_buffer);
void APP_ZIGBEE_TL_INIT(void);
void Pre_ZigbeeCmdProcessing(void);
void APP_ZIGBEE_Stop(void);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* APP_ZIGBEE_H */

