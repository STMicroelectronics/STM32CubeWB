/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    models_if.h
  * @author  MCD Application Team
  * @brief   Mesh Modes interface file header 
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
#ifndef MODELS_H
#define MODELS_H

/* Includes ------------------------------------------------------------------*/
#include "types.h"


/* Exported macro ------------------------------------------------------------*/
/* Exported variables  ------------------------------------------------------- */

/* Exported Functions Prototypes ---------------------------------------------*/

void BLEMesh_ModelsInit(void);
void BLEMesh_ModelsProcess(void);
void BLEMesh_ModelsCommand(void);

MOBLEUINT8 BLEMesh_ModelsGetElementNumber(MOBLE_ADDRESS dst_peer);
MOBLEUINT8 BLEMesh_ModelsGetElementIndex(MOBLE_ADDRESS dst_peer);
void BLEMesh_ModelsDelayPacket(MODEL_MessageHeader_t *pmsgParams, MOBLEUINT8 command, 
                               MOBLEUINT8 const * data, MOBLEUINT32 length);
void BLEMesh_ModelsSendDelayedPacket(void);
MOBLEUINT8 BLEMesh_ModelsASCII_To_Char(MOBLEUINT8 tempValue);
MOBLE_RESULT MeshClient_SetRemotePublication(MOBLEUINT32 modelId, MOBLEUINT8 elementIdx,
                                              MOBLEUINT16 msg_opcode, MOBLEUINT8 const *msg_buff, 
                                              MOBLEUINT32 length, MOBLEBOOL ack_flag,
                                              MOBLEUINT8 isVendor);

#endif /* MODELS_H */


