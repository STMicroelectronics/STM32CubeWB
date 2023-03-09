/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    eddystone_url_service.c
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

/* Includes ------------------------------------------------------------------*/
#include "app_common.h"
#include "ble.h"
#include "eddystone_beacon.h"
#include "eddystone_url_service.h"

/* Exported types ------------------------------------------------------------*/
/* Private types -------------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define ADVERTISING_INTERVAL_INCREMENT (16)

/* Private variables ---------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Private constants ---------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
tBleStatus EddystoneURL_Init(EddystoneURL_InitTypeDef *EddystoneURL_Init)
{
  tBleStatus ret;
  uint16_t AdvertisingInterval = (EddystoneURL_Init->AdvertisingInterval * ADVERTISING_INTERVAL_INCREMENT / 10);
  uint8_t service_data[24] =
  {
    6 + EddystoneURL_Init->UrlLength,                                       /*< Length. */
    AD_TYPE_SERVICE_DATA,                                                   /*< Service Data data type value. */
    0xAA, 0xFE,                                                             /*< 16-bit Eddystone UUID. */
    0x10,                                                                   /*< URL frame type. */
    EddystoneURL_Init->CalibratedTxPower,                                   /*< Ranging data. */
    EddystoneURL_Init->UrlScheme,                                           /*< URL Scheme Prefix is http://www. */
    0x00,                                                                   /*< URL */
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00
  };
  uint8_t service_uuid_list[] =
  {
    3,                                                                      /*< Length. */
    AD_TYPE_16_BIT_SERV_UUID_CMPLT_LIST,                                    /*< Complete list of 16-bit Service UUIDs data type value. */
    0xAA, 0xFE                                                              /*< 16-bit Eddystone UUID. */
  };
  uint8_t flags[] =
  {
    2,                                                                      /*< Length. */
    AD_TYPE_FLAGS,                                                          /*< Flags data type value. */
    (FLAG_BIT_LE_GENERAL_DISCOVERABLE_MODE | FLAG_BIT_BR_EDR_NOT_SUPPORTED) /*< BLE general discoverable, without BR/EDR support. */
  };

  /* Disable scan response. */
  hci_le_set_scan_response_data(0, NULL);

  /* Put the device in a non-connectable mode. */
  ret = aci_gap_set_discoverable(ADV_NONCONN_IND,                          /*< Advertise as non-connectable, undirected. */
                                 AdvertisingInterval, AdvertisingInterval, /*< Set the advertising interval as 700 ms (0.625 us increment). */
                                 GAP_PUBLIC_ADDR, NO_WHITE_LIST_USE,           /*< Use the public address, with no white list. */
                                 0, NULL,                                  /*< Do not use a local name. */
                                 0, NULL,                                  /*< Do not include the service UUID list. */
                                 0, 0);                                    /*< Do not set a slave connection interval. */

  if (ret != BLE_STATUS_SUCCESS)
  {
    return ret;
  }

  /* Remove the TX power level advertisement (this is done to decrease the packet size). */
  ret = aci_gap_delete_ad_type(AD_TYPE_TX_POWER_LEVEL);

  if (ret != BLE_STATUS_SUCCESS)
  {
    return ret;
  }

  for (uint8_t i = 0; i < EddystoneURL_Init->UrlLength; ++i)
  {
    service_data[7 + i] = EddystoneURL_Init->Url[i];
  }

  /* Update the service data. */
  ret = aci_gap_update_adv_data(sizeof(service_data), service_data);

  if (ret != BLE_STATUS_SUCCESS)
  {
    return ret;
  }

  /* Update the service UUID list. */
  ret = aci_gap_update_adv_data(sizeof(service_uuid_list), service_uuid_list);

  if (ret != BLE_STATUS_SUCCESS)
  {
    return ret;
  }

  /* Update the adverstising flags. */
  ret = aci_gap_update_adv_data(sizeof(flags), flags);

  if (ret != BLE_STATUS_SUCCESS)
  {
    return ret;
  }

  return ret;
}

void EddystoneURL_Process(void)
{
#ifdef USE_OTA
  uint32_t data_address = OTA_BEACON_DATA_ADDRESS + OFFSET_PAYLOAD_LENGTH; /* 0x8006009 */

  if(((*(uint8_t *)(OTA_BEACON_DATA_ADDRESS)) !=  0xFF) &&
     ((*(uint8_t *)(data_address + 9)) ==  0x10))
  {
    /* Eddystone URL beacon User Data download */
    EddystoneURL_InitTypeDef EddystoneURL_InitStruct;
    uint8_t Url[100];
    uint8_t i;

    EddystoneURL_InitStruct.Url                 = Url;
    EddystoneURL_InitStruct.AdvertisingInterval = ADVERTISING_INTERVAL_IN_MS;

    data_address += 5; /* 0x800600e */
    EddystoneURL_InitStruct.UrlLength           = *(uint8_t *)(data_address);
    data_address += 5; /* 0x8006013 */
    EddystoneURL_InitStruct.CalibratedTxPower   = *(uint8_t *)(data_address);
    data_address += 1; /* 0x8006014 */
    EddystoneURL_InitStruct.UrlScheme           = *(uint8_t *)(data_address);
    data_address += 1; /* 0x8006015 */
    for(i = 0; i < EddystoneURL_InitStruct.UrlLength - 6; i++)
      EddystoneURL_InitStruct.Url[i] = *(uint8_t *)(data_address + i);

    EddystoneURL_Init(&EddystoneURL_InitStruct);
  }
  else
  {
    uint8_t UrlScheme = URL_PREFIX;
    uint8_t Url[]     = PHYSICAL_WEB_URL;
    EddystoneURL_InitTypeDef EddystoneURL_InitStruct =
    {
      .AdvertisingInterval = ADVERTISING_INTERVAL_IN_MS,
      .CalibratedTxPower = CALIBRATED_TX_POWER_AT_0_M,
      .UrlScheme = UrlScheme,
      .Url = Url,
      .UrlLength = sizeof(Url) - 1
    };

    EddystoneURL_Init(&EddystoneURL_InitStruct);
  }
#else
  uint8_t UrlScheme = URL_PREFIX;
  uint8_t Url[]     = PHYSICAL_WEB_URL;
  EddystoneURL_InitTypeDef EddystoneURL_InitStruct =
  {
    .AdvertisingInterval = ADVERTISING_INTERVAL_IN_MS,
    .CalibratedTxPower = CALIBRATED_TX_POWER_AT_0_M,
    .UrlScheme = UrlScheme,
    .Url = Url,
    .UrlLength = sizeof(Url) - 1
  };

  EddystoneURL_Init(&EddystoneURL_InitStruct);
#endif
}
