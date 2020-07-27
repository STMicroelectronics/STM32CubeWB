/**
 ******************************************************************************
  * File Name          : App/eddystone_url_service.h
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
#ifndef EDDYSTONE_URL_SERVICE_H
#define EDDYSTONE_URL_SERVICE_H

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
  uint8_t UrlScheme;           /*!< Specifies the URL Encoded Scheme Prefix. */
  uint8_t * Url;               /*!< Specifies the Encoded URL (max 17 characters). */
  uint8_t UrlLength;           /*!< Specifies the length of the Encoded URL (max 17 characters). */
} EddystoneURL_InitTypeDef;

/* Exported constants --------------------------------------------------------*/
#define HTTP_WWW          (0x00u)
#define HTTPS_WWW         (0x01u)
#define HTTP              (0x02u)
#define HTTPS             (0x03u)

#define DOT_COM_SLASH     (0x00u)
#define DOT_ORG_SLASH     (0x01u)
#define DOT_EDU_SLASH     (0x02u)
#define DOT_NET_SLASH     (0x03u)
#define DOT_INFO_SLASH    (0x04u)
#define DOT_BIZ_SLASH     (0x05u)
#define DOT_GOV_SLASH     (0x06u)
#define DOT_COM           (0x07u)
#define DOT_ORG           (0x08u)
#define DOT_EDU           (0x09u)
#define DOT_NET           (0x0Au)
#define DOT_INFO          (0x0Bu)
#define DOT_BIZ           (0x0Cu)
#define DOT_GOV           (0x0Du)

/* Exported Macros -----------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
tBleStatus EddystoneURL_Init(EddystoneURL_InitTypeDef *EddystoneURL_Init);
void EddystoneURL_Process(void);

#ifdef __cplusplus
}
#endif

#endif /* EDDYSTONE_URL_SERVICE_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
