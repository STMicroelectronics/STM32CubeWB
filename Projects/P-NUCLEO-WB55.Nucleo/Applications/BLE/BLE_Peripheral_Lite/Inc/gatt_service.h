/**
  ******************************************************************************
  * @file    gatt_service.h
  * @author  MCD Application Team
  * @brief   Header for gatt_service.c module
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
#ifndef __GATT_SERVICE_H
#define __GATT_SERVICE_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
/* Exported defines ----------------------------------------------------------*/
#define MY_VERY_OWN_SERVICE_UUID                                      (0xFE40)
#define MY_VERY_OWN_WRITE_CHARACTERISTIC_UUID                         (0xFE41)
#define MY_VERY_OWN_NOTIFY_CHARACTERISTIC_UUID                        (0xFE42)
    
#define MY_VERY_OWN_NOTIFY_CHARACTERISTIC_VALUE_LENGTH     2
#define MY_VERY_OWN_WRITE_CHARACTERISTIC_VALUE_LENGTH      2

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* External variables --------------------------------------------------------*/
/* Exported macros -----------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void MyVeryOwnService_Init(void);
tBleStatus MyVeryOwnWriteCharacteristic_Update(uint16_t UUID, uint16_t newValueLength, uint8_t *pNewValue);

#ifdef __cplusplus
}
#endif

#endif /* __GATT_SERVICE_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
