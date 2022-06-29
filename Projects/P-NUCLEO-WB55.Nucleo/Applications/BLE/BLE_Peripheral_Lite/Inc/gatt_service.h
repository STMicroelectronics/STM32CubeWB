/**
  ******************************************************************************
  * @file    gatt_service.h
  * @author  MCD Application Team
  * @brief   Header for gatt_service.c module
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
#ifndef GATT_SERVICE_H
#define GATT_SERVICE_H

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

#endif /* GATT_SERVICE_H */
