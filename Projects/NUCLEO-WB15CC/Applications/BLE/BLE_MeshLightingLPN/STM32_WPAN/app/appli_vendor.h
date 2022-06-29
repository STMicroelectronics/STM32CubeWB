/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    appli_vendor.h
  * @author  MCD Application Team
  * @brief   Application interface for Vendor Mesh Models  
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
#ifndef APPLI_VENDOR_H
#define APPLI_VENDOR_H

/* Includes ------------------------------------------------------------------*/
#include "types.h"
#include "ble_mesh.h"

/* Exported macro ------------------------------------------------------------*/
#define LED_OFF_VALUE          1U
#define LED_ON_VALUE           32000U
/* Private define ------------------------------------------------------------*/

MOBLE_RESULT Appli_Vendor_LEDControl( MOBLEUINT8 const *data, MOBLEUINT32 length,
                                       MOBLEUINT8 elementNumber, MOBLE_ADDRESS dst_peer);
MOBLE_RESULT Appli_Vendor_DeviceInfo(MOBLEUINT8 const *data, MOBLEUINT32 length);
MOBLE_RESULT Appli_Vendor_Test(MOBLEUINT8 const *data, MOBLEUINT32 length);
MOBLE_RESULT Appli_Vendor_Data_write(MOBLEUINT8 const *data, MOBLEUINT32 length);
void Appli_GetTestValue (MOBLEUINT8 *responseValue);
MOBLE_RESULT Appli_Vendor_SendBigDataPacket(void);
MOBLE_RESULT Appli_Vendor_SetBigDataPacket(MOBLEUINT8 *data, MOBLEUINT32 length,
                                       MOBLEUINT8 elementIndex , MOBLE_ADDRESS dst_peer);
void Appli_Vendor_Publish(MOBLE_ADDRESS srcAddress);
#endif /* APPLI_VENDOR_H */


