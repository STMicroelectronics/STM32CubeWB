/**
 ******************************************************************************
 * File Name          : App/eddystone_uid_service.h
 * Description        :
 ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
