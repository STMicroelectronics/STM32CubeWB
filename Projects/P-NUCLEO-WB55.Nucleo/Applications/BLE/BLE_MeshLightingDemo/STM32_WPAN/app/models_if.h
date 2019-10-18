/**
  ******************************************************************************
  * @file    models_if.h
  * @author  BLE Mesh Team
  * @brief   Mesh Modes interface file header 
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
#ifndef __MODELS_H
#define __MODELS_H

/* Includes ------------------------------------------------------------------*/
#include "types.h"


/* Exported macro ------------------------------------------------------------*/
/* Exported variables  ------------------------------------------------------- */

/* Exported Functions Prototypes ---------------------------------------------*/

void BLEMesh_ModelsInit(void);
void BLEMesh_ModelsProcess(void);
void BLEMesh_ModelsCommand(void);
MOBLE_RESULT BLEMesh_ModelsCheckSubscription(MOBLE_ADDRESS dst_peer, MOBLEUINT8 elementNumber);
MOBLEUINT8 BLEMesh_ModelsGetElementNumber(MOBLE_ADDRESS dst_peer);
void BLEMesh_ModelsDelayPacket(MOBLE_ADDRESS peer, MOBLE_ADDRESS dst, MOBLEUINT8 command, 
                               MOBLEUINT8 const * data, MOBLEUINT32 length);
void BLEMesh_ModelsSendDelayedPacket(void);
MOBLEUINT8 BLEMesh_ModelsASCII_To_Char(MOBLEUINT8 tempValue);
#endif /* __MODELS_H */

/******************* (C) COPYRIGHT 2019 STMicroelectronics *****END OF FILE****/

