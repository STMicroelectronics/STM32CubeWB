/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    appli_sensor_client.h
  * @author  MCD Application Team
  * @brief   Application interface for Sensor Mesh Models  
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
#ifndef APPLI_SENSOR_CLIENT_H
#define APPLI_SENSOR_CLIENT_H

/* Includes ------------------------------------------------------------------*/
#include "types.h"
#include "sensors.h"
#include "mesh_cfg.h"


/* Exported macro ------------------------------------------------------------*/
/* Exported variables  ------------------------------------------------------- */
/* Application Variable-------------------------------------------------------*/
/* Exported Functions Prototypes ---------------------------------------------*/
MOBLE_RESULT Appli_ConfigClient_Set(void); 
MOBLE_RESULT Appli_SensorsClient_API(MOBLEUINT8 elementIndex, 
                                     MOBLEUINT16 msg_opcode, 
                                     MOBLEUINT8 *msg_params);
void Appli_Sensor_Descriptor_Status(const MOBLEUINT8 *pDescriptor,
                                    MOBLEUINT32 length,
                                    MOBLE_ADDRESS dstPeer,
                                    MOBLEUINT8 elementIndex);
void Appli_Sensor_Cadence_Status(const MOBLEUINT8 *pCadence,
                                 MOBLEUINT32 length,
                                 MOBLE_ADDRESS dstPeer,
                                 MOBLEUINT8 elementIndex);
void Appli_Sensor_Settings_Status(const MOBLEUINT8 *pSettings,
                                 MOBLEUINT32 length,
                                 MOBLE_ADDRESS dstPeer,
                                 MOBLEUINT8 elementIndex);
void Appli_Sensor_Setting_Status(const MOBLEUINT8 *pSetting,
                                 MOBLEUINT32 length,
                                 MOBLE_ADDRESS dstPeer,
                                 MOBLEUINT8 elementIndex);
void Appli_Sensor_Status(const MOBLEUINT8 *pStatus,
                         MOBLEUINT32 length,
                         MOBLE_ADDRESS dstPeer,
                         MOBLEUINT8 elementIndex);
void Appli_Sensor_Column_Status(const MOBLEUINT8 *pColumn,
                                MOBLEUINT32 length,
                                MOBLE_ADDRESS dstPeer,
                                MOBLEUINT8 elementIndex);
void Appli_Sensor_Series_Status(const MOBLEUINT8 *pSeries,
                                MOBLEUINT32 length,
                                MOBLE_ADDRESS dstPeer,
                                MOBLEUINT8 elementIndex);  




#endif /* APPLI_SENSOR_CLIENT_H */

