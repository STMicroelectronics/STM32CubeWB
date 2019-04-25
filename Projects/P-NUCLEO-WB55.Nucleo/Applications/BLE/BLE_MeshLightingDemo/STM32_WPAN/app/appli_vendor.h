/**
  ******************************************************************************
  * @file    appli_vendor.h
  * @author  BLE Mesh Team
  * @brief   Application interface for Vendor Mesh Models  
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
#ifndef __VENDOR_APPLI_H
#define __VENDOR_APPLI_H

/* Includes ------------------------------------------------------------------*/
#include "types.h"
#include "ble_mesh.h"

/* Exported macro ------------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

MOBLE_RESULT Appli_Vendor_LEDControl( MOBLEUINT8 const *data, MOBLEUINT32 length,
                                       MOBLEUINT8 elementNumber, MOBLE_ADDRESS dst_peer);
MOBLE_RESULT Appli_Vendor_DeviceInfo(MOBLEUINT8 const *data, MOBLEUINT32 length);
MOBLE_RESULT Appli_Vendor_Test(MOBLEUINT8 const *data, MOBLEUINT32 length);

#endif /* __VENDOR_APPLI_H */

/******************* (C) COPYRIGHT 2019 STMicroelectronics *****END OF FILE****/

