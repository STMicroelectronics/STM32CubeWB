/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    appli_mesh.h
  * @author  MCD Application Team
  * @brief   Header file for the user application file 
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2021 STMicroelectronics.
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
#ifndef APPLI_MESH_H
#define APPLI_MESH_H

/* Includes ------------------------------------------------------------------*/
#include "types.h"
#include "mesh_cfg_usr.h"
#include "mesh_cfg.h"

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
#define POWEROFF_PIN                                                 GPIO_PIN_12
#define POWEROFF_GPIO_PORT                                                 GPIOB
#define POWEROFF_GPIO_CLK_ENABLE()                  __HAL_RCC_GPIOB_CLK_ENABLE()
#define POWEROFF_GPIO_CLK_DISABLE()                __HAL_RCC_GPIOB_CLK_DISABLE()
#define POWEROFF_EXTI_LINE                                           GPIO_PIN_12
#define POWEROFF_EXTI_IRQn                                        EXTI15_10_IRQn
#endif

typedef enum _prvn_nvm_state
{
  NVM_INIT = 0,
  NVM_SAVE_PENDING,
} prvn_nvm_state;

/* Exported variables  ------------------------------------------------------- */

extern MOBLEUINT8 bdaddr[]; 

typedef struct
{
  MOBLEUINT16 nodeAddress;
  MOBLEUINT8  numOfElements;
  MOBLEUINT8  Reserved1;   /* For Alignment by 4*/
  MOBLEUINT16 newNodeAddress;
  MOBLEUINT8  newNodeElements;
  prvn_nvm_state  saveToNVM; 
  MOBLEUINT8 NewProvNodeDevKey[DEVICE_KEY_SIZE];  
  MOBLEUINT8 NewProvNodeAppKey[APP_KEY_SIZE];
  MOBLEUINT8 NewProvNodeUUID[UUID_SIZE];
} NodeUnderProvisionParam_t;

extern NodeUnderProvisionParam_t NodeUnderProvisionParam;
extern MOBLEUINT16 Appli_SIG_Models[APPLICATION_NUMBER_OF_ELEMENTS][APPLICATION_SIG_MODELS_MAX_COUNT];
#ifdef ENABLE_VENDOR_MODEL_SERVER  
extern MOBLEUINT32 Appli_Vendor_Models[APPLICATION_NUMBER_OF_ELEMENTS][APPLICATION_VENDOR_MODELS_MAX_COUNT];
#endif
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
void Appli_OobAuthenticationProcess(void);
void Appli_BleOutputOOBAuthCb(MOBLEUINT8* output_oob, MOBLEUINT8 size);
MOBLEUINT8* Appli_BleInputOOBAuthCb(MOBLEUINT8 size);
void Appli_BleSerialInputOOBValue(char *rcvdStringBuff, uint16_t rcvdStringSize);
MOBLEUINT8 Appli_BleDisableFilterCb(void);
void BLEMesh_PbAdvLinkCloseCb(void);
void Appli_IntensityControlPublishing(MOBLEUINT8* value);

int Appli_CheckBdMacAddr(void);
MOBLE_RESULT Appli_LedBlink(void);
MOBLE_RESULT Appli_LedStateCtrlCb(MOBLEUINT16 ctrl);

void Appli_CheckForUnprovision(void);
void Appli_Unprovision(void);
void Appli_Process(void);
void Appli_LedCtrl(void);
void Appli_Init(MOBLEUINT8 *flag);

MOBLE_ADDRESS GetAddressToConfigure(void);
MOBLE_ADDRESS GetNewAddressToProvision(void);
void SaveProvisionedNodeAddress(void);
MOBLEUINT8* GetNewProvNodeDevKey(void);
MOBLEUINT8* GetNewProvNodeAppKey(void);
void IntensityPublish(void);
void Start_SelfConfiguration (void);
void Appli_ProvisionerInit(void);
void Appli_StartProvisionerMode(MOBLEUINT8 mode);

void Appli_GetPublicationParamsCb(model_publicationparams_t*);

MOBLEUINT16 PWM_CoolValue(float colourValue ,float brightValue);
MOBLEUINT16 PWM_WarmValue(float colourValue ,float brightValue);
MOBLEUINT16 PwmValueMapping(MOBLEUINT16 setValue , MOBLEUINT16 maxRange , MOBLEINT16 minRange);

MOBLE_RESULT BLEMesh_ScanDevices(neighbor_params_t *unprovDeviceArray, MOBLEUINT8 *noOfUnprovDevices);
MOBLE_RESULT BLEMesh_ProvisionDevice(neighbor_params_t *unprovDeviceArray, MOBLEUINT16 index);

#endif /* APPLI_MESH_H */


