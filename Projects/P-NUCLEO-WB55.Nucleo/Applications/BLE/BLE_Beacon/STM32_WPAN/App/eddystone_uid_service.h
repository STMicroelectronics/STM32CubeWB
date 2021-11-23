/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    eddystone_uid_service.h
  * @author  MCD Application Team
  * @brief   
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2020-2021 STMicroelectronics.
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
#ifndef EDDYSTONE_UID_SERVICE_H
#define EDDYSTONE_UID_SERVICE_H

#ifdef __cplusplus
extern "C"
{
#endif

/* Includes ------------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
typedef struct
{
  uint16_t AdvertisingInterval;/*!< Specifies the desired advertising interval. */
  uint8_t CalibratedTxPower;   /*!< Specifies the power at 0m. */
  uint8_t * NamespaceID;       /*!< Specifies the 10-byte namespace to which the beacon belongs. */
  uint8_t * BeaconID;          /*!< Specifies the unique 6-byte beacon ID within the specified namespace. */
} EddystoneUID_InitTypeDef;

/* Exported constants --------------------------------------------------------*/
/* Exported Macros -----------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
tBleStatus EddystoneUID_Init(EddystoneUID_InitTypeDef *EddystoneUID_Init);
void EddystoneUID_Process(void);

#ifdef __cplusplus
}
#endif

#endif /* EDDYSTONE_UID_SERVICE_H */
