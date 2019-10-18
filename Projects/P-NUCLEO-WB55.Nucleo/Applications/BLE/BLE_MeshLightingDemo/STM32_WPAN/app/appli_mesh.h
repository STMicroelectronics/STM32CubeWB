/**
  ******************************************************************************
  * @file    appli_mesh.h
  * @author  BLE Mesh Team
  * @brief   Header file for the user application file 
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
#ifndef __APPLI_MESH_H
#define __APPLI_MESH_H

/* Includes ------------------------------------------------------------------*/
#include "types.h"
#include "mesh_cfg_usr.h"

/* Exported macro ------------------------------------------------------------*/
/* MACROS for Power Level definitions */
#define POWER_LEVEL_LOW            0 
#define TX_POWER_LEVEL_MINUS_18DBM 0 /* = -18 dBm, */
#define TX_POWER_LEVEL_MINUS_15DBM 1 /* = -15 dBm, */
#define TX_POWER_LEVEL_MINUS_12DBM 2 /* = -12 dBm, */
#define TX_POWER_LEVEL_MINUS_9DBM  3 /* = -9 dBm, */
#define TX_POWER_LEVEL_MINUS_6DBM  4 /* = -6 dBm, */
#define TX_POWER_LEVEL_MINUS_2DBM  5 /* = -2 dBm, */
#define TX_POWER_LEVEL_0DBM        6 /* =  0 dBm, */
#define TX_POWER_LEVEL_PLUS_5DBM   7 /* =  5 dBm. */
#define POWER_LEVEL_HIGH           1
#define TX_POWER_LEVEL_MINUS_14DBM 0 /* = -14 dBm, */
#define TX_POWER_LEVEL_MINUS_11DBM 1 /* = -11 dBm, */
#define TX_POWER_LEVEL_MINUS_8DBM  2 /* = -8 dBm, */
#define TX_POWER_LEVEL_MINUS_5DBM  3 /* = -5 dBm, */
#define TX_POWER_LEVEL_PLUS_2DBM   5 /* =  2 dBm, */
#define TX_POWER_LEVEL_PLUS_4DBM   6 /* =  4 dBm, */
#define TX_POWER_LEVEL_PLUS_8DBM   7 /* =  8 dBm */

#ifdef SAVE_MODEL_STATE_POWER_FAILURE_DETECTION       
#define POWEROFF_PIN                                                  GPIO_PIN_4
#define POWEROFF_GPIO_PORT                                                 GPIOB
#define POWEROFF_GPIO_CLK_ENABLE()                  __HAL_RCC_GPIOB_CLK_ENABLE()
#define POWEROFF_GPIO_CLK_DISABLE()                __HAL_RCC_GPIOB_CLK_DISABLE()
#define POWEROFF_EXTI_LINE                                            GPIO_PIN_4
#define POWEROFF_EXTI_IRQn                                            EXTI2_IRQn
#endif

/* Exported variables  ------------------------------------------------------- */

extern MOBLEUINT8 bdaddr[]; 

/* Exported Functions Prototypes ---------------------------------------------*/
MOBLE_RESULT Appli_BleStackInitCb(void);
MOBLE_RESULT Appli_BleSetTxPowerCb(void);
MOBLE_RESULT Appli_BleSetUUIDCb(MOBLEUINT8 *uuid_prefix_data);
MOBLE_RESULT Appli_BleSetProductInfoCB(MOBLEUINT8 *company_product_info);
void Appli_BleUnprovisionedIdentifyCb(MOBLEUINT8 data);
void Appli_BleGattConnectionCompleteCb(void);
void Appli_BleGattDisconnectionCompleteCb(void);
MOBLEUINT8 Appli_BleSetNumberOfElementsCb(void);
MOBLE_RESULT Appli_BleAttentionTimerCb(void);
void Appli_BleOutputOOBAuthCb(MOBLEUINT8* output_oob, MOBLEUINT8 size);
MOBLEUINT8* Appli_BleInputOOBAuthCb(MOBLEUINT8 size);
void Appli_BleSerialInputOOBValue(char *rcvdStringBuff, uint16_t rcvdStringSize);
MOBLEUINT8 Appli_BleDisableFilterCb(void);

void Appli_IntensityControlPublishing(void);

int Appli_CheckBdMacAddr(void);
MOBLE_RESULT Appli_LedBlink(void);
MOBLE_RESULT Appli_LedStateCtrlCb(MOBLEUINT16 ctrl);

void Appli_CheckForUnprovision(void);
void Appli_Process(void);
void Appli_LedCtrl(void);
void Appli_Init(void);
#endif /* __APPLI_MESH_H */

/******************* (C) COPYRIGHT 2019 STMicroelectronics *****END OF FILE****/

