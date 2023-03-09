/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    eddystone_tlm_service.c
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
#include "eddystone_uid_service.h"
#include "system_stm32wbxx.h"
#include "eddystone_tlm_service.h"

/* Exported types ------------------------------------------------------------*/
typedef struct
{
  uint16_t AdvertisingInterval;/*!< Specifies the desired advertising interval. */
  uint8_t  TLM_Version;        /*!< Specifies the version of the TLM frame. */
  uint16_t BatteryVoltage;     /*!< Specifies the battery voltage, in 1 mV/bit. */
  uint16_t BeaconTemperature;  /*!< Specifies the beacon temperature, in Signed 8.8 Fixed Point notation. */
  uint32_t AdvertisingCount;   /*!< Specifies the running count of all advertisement frames. */
  uint32_t Uptime;             /*!< Specifies the time sinbe the beacon was powered-up or rebooted. */
} EddystoneTLM_InitTypeDef;

/* Private types -------------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define ADVERTISING_INTERVAL_INCREMENT (16)
#define DEFAULT_BEACON_SEC                 (1000000/CFG_TS_TICK_VAL)  /**< 1s */

/* Private variables ---------------------------------------------------------*/
uint8_t tlm_adv;
uint8_t TimerTLM_Id;
EddystoneURL_InitTypeDef EddystoneURL_InitStruct;
EddystoneUID_InitTypeDef EddystoneUID_InitStruct;
EddystoneTLM_InitTypeDef EddystoneTLM_InitStruct;

/* Private constants ---------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/* Global variables ----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
static tBleStatus EddystoneTLM_Init(EddystoneTLM_InitTypeDef *EddystoneTLM_Init)
{
  tBleStatus ret = BLE_STATUS_SUCCESS;
  uint16_t AdvertisingInterval = (EddystoneTLM_Init->AdvertisingInterval * ADVERTISING_INTERVAL_INCREMENT / 10);
  uint8_t service_data[] =
  {
    17,                                                                      /*< Length. */
    AD_TYPE_SERVICE_DATA,                                                    /*< Service Data data type value. */
    0xAA, 0xFE,                                                              /*< 16-bit Eddystone UUID. */
    0x20,                                                                    /*< TLM frame type. */
    (EddystoneTLM_Init->TLM_Version),                                        /*< TLM version. */
    (EddystoneTLM_Init->BatteryVoltage & 0xFF00) >> 8,                       /*< Battery voltage. */
    (EddystoneTLM_Init->BatteryVoltage & 0x00FF),
    (EddystoneTLM_Init->BeaconTemperature & 0xFF00) >> 8,                    /*< Beacon temperature. */
    (EddystoneTLM_Init->BeaconTemperature & 0x00FF),
    (EddystoneTLM_Init->AdvertisingCount & 0xFF000000) >> 24,                /*< Advertising PDU count. */
    (EddystoneTLM_Init->AdvertisingCount & 0x00FF0000) >> 16,
    (EddystoneTLM_Init->AdvertisingCount & 0x0000FF00) >> 8,
    (EddystoneTLM_Init->AdvertisingCount & 0x000000FF),
    (EddystoneTLM_Init->Uptime & 0xFF000000) >> 24,                          /*< Time since power-on or reboot. */
    (EddystoneTLM_Init->Uptime & 0x00FF0000) >> 16,
    (EddystoneTLM_Init->Uptime & 0x0000FF00) >> 8,
    (EddystoneTLM_Init->Uptime & 0x000000FF)
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

static void EddystoneTLM(void)
{
  tBleStatus ret;
#ifdef USE_OTA
  uint32_t data_address = OTA_BEACON_DATA_ADDRESS + OFFSET_PAYLOAD_LENGTH; /* 0x8006009 */
#endif

  if(tlm_adv == TRUE)
  { /* Advertising of TLM */
    ret = aci_gap_set_non_discoverable();

    if (ret != BLE_STATUS_SUCCESS)
    {
      while(1);
    }

#ifdef USE_OTA
    if(((*(uint8_t *)OTA_BEACON_DATA_ADDRESS) != 0xFF) &&
       ((*(uint8_t *)(OTA_BEACON_DATA_ADDRESS + 8)) == 0x01))
    { /* TLM present and User Data downloaded */
      EddystoneTLM_InitStruct.AdvertisingInterval = ADVERTISING_INTERVAL_IN_MS;

      if((*(uint8_t *)(data_address + 9) == 0x00))
      { /* 0x8006012: EDDYSTONE UUID => TLM Data start at 0x8006027 */
        data_address += 0x09 + 0x15; /* 0x8006009 -> 0x8006027: 0x1E */
      }
      else
      { /* 0x8006012: EDDYSTONE URL => TLM Data start at 0x8006012 + 3 + (URL length - 6) */
        data_address += 0x09 + 0x03 + (EddystoneURL_InitStruct.UrlLength - 6);
      }

      EddystoneTLM_InitStruct.TLM_Version = *(uint8_t *)data_address;
      if((*(uint8_t *)data_address) != 0xFF)
      { /* Battery Voltage downloaded */
        EddystoneTLM_InitStruct.BatteryVoltage = (*(uint8_t *)data_address) +
                                                 ((*(uint8_t *)(data_address + 1)) << 8);
        data_address += 2;
        if((*(uint8_t *)data_address) != 0xFF)
        { /* Temperature downloaded */
          EddystoneTLM_InitStruct.BeaconTemperature = (*(uint8_t *)data_address) +
                                                      ((*(uint8_t *)(data_address + 1)) << 8);
          data_address += 2;
          if((*(uint8_t *)data_address) != 0xFF)
          { /* Uptime downloaded */
            EddystoneTLM_InitStruct.Uptime = (*(uint8_t *)data_address) +
                                             ((*(uint8_t *)(data_address + 1)) << 8)  +
                                             ((*(uint8_t *)(data_address + 2)) << 16) +
                                             ((*(uint8_t *)(data_address + 3)) << 24);
            data_address += 4;
            if((*(uint8_t *)data_address) != 0xFF)
            { /* Advertising count downloaded */
              EddystoneTLM_InitStruct.AdvertisingCount = (*(uint8_t *)data_address) +
                                                         ((*(uint8_t *)(data_address + 1)) << 8)  +
                                                         ((*(uint8_t *)(data_address + 2)) << 16) +
                                                         ((*(uint8_t *)(data_address + 3)) << 24);
            }
          }
        }
      }
      EddystoneTLM_Init(&EddystoneTLM_InitStruct);

      tlm_adv = FALSE;

      /* Wait 1s */
      HW_TS_Start(TimerTLM_Id, DEFAULT_BEACON_SEC);
    }
    else
    { /* TLM not present or User Data not downloaded */
      EddystoneTLM_Init(&EddystoneTLM_InitStruct);

      tlm_adv = FALSE;

      /* Wait 1s */
      HW_TS_Start(TimerTLM_Id, DEFAULT_BEACON_SEC);
    }
#else
    /* No OTA */
    EddystoneTLM_Init(&EddystoneTLM_InitStruct);

    tlm_adv = FALSE;

    /* Wait 1s */
    HW_TS_Start(TimerTLM_Id, DEFAULT_BEACON_SEC);
#endif
  }
  else
  { /* Advertising of UUID or URL */
    ret = aci_gap_set_non_discoverable();

    if (ret != BLE_STATUS_SUCCESS)
    {
      while(1);
    }

#ifdef USE_OTA
    data_address += 9; /* 0x8006012 */
    if((*(uint8_t *)data_address) == 0x00)
    {
      /* Eddystone UID */
      uint8_t i, NameId[10], BeaconId[6];

      EddystoneUID_InitStruct.NamespaceID = NameId;
      EddystoneUID_InitStruct.BeaconID = BeaconId;

      data_address += 1; /* 0x8006013 */
      EddystoneUID_InitStruct.CalibratedTxPower   = *(uint8_t *)data_address;
      data_address += 1; /* 0x8006014 */
      for(i = 0; i < 10; i++)
       EddystoneUID_InitStruct.NamespaceID[i] = *(uint8_t *)(data_address + i);
      data_address += 10; /* 0x800601E */
      for(i = 0; i < 6; i++)
        EddystoneUID_InitStruct.BeaconID[i] = *(uint8_t *)(data_address + i);
      data_address += 8; /* 0x8006026 */

      EddystoneUID_Init(&EddystoneUID_InitStruct);
      tlm_adv = TRUE;  /* Next TLM advertise */
      /* 10s of UID advertise */
      HW_TS_Start(TimerTLM_Id, DEFAULT_BEACON_SEC * 10);
    }
    else if((*(uint8_t *)data_address) == 0x10)
    {
      /* Eddystone URL */
      uint8_t i;
      uint8_t Url[100];

      EddystoneURL_InitStruct.Url = Url;
      EddystoneURL_InitStruct.AdvertisingInterval = ADVERTISING_INTERVAL_IN_MS;

      data_address += 1; /* 0x8006013 */
      EddystoneURL_InitStruct.CalibratedTxPower   = *(uint8_t *)data_address;
      data_address += 1; /* 0x8006014 */
      EddystoneURL_InitStruct.CalibratedTxPower   = *(uint8_t *)data_address;
      data_address += 1; /* 0x8006015 */
      for(i = 0; i < EddystoneURL_InitStruct.UrlLength - 6; i++)
        EddystoneURL_InitStruct.Url[i] = *(uint8_t *)(data_address + i);
      EddystoneURL_InitStruct.UrlLength           = *(uint8_t *)(OTA_BEACON_DATA_ADDRESS + OFFSET_PAYLOAD_LENGTH + 5); /* 0x800600e */
      data_address += EddystoneURL_InitStruct.UrlLength - 6; /* 0x8006015 + EddystoneURL_InitStruct.UrlLength - 6 */

      EddystoneURL_Init(&EddystoneURL_InitStruct);
      tlm_adv = TRUE;  /* Next TLM advertise */
      /* 10s of URL advertise */
      HW_TS_Start(TimerTLM_Id, DEFAULT_BEACON_SEC * 10);
    }
    else
    {
      EddystoneURL_Init(&EddystoneURL_InitStruct);
      tlm_adv = TRUE;
      /* 10s of URL advertise */
      HW_TS_Start(TimerTLM_Id, DEFAULT_BEACON_SEC * 10);
    }
#else
    /* No OTA */
    EddystoneURL_Init(&EddystoneURL_InitStruct);
    tlm_adv = TRUE;
    /* 10s of URL advertise */
    HW_TS_Start(TimerTLM_Id, DEFAULT_BEACON_SEC * 10);
#endif
  }
}

/* Exported functions --------------------------------------------------------*/
void EddystoneTLM_Process(void)
{
#ifdef USE_OTA
  uint32_t data_address = OTA_BEACON_DATA_ADDRESS + OFFSET_PAYLOAD_LENGTH; /* 0x8006009 */
  uint8_t length = *(uint8_t *)(data_address);
#else
  uint8_t UrlScheme     = URL_PREFIX;
  uint8_t Url[]         = PHYSICAL_WEB_URL;
  uint8_t NamespaceID[] = { NAMESPACE_ID };
  uint8_t BeaconID[]    = { BEACON_ID };

  EddystoneURL_InitStruct.AdvertisingInterval = ADVERTISING_INTERVAL_IN_MS;
  EddystoneURL_InitStruct.CalibratedTxPower = CALIBRATED_TX_POWER_AT_0_M;
  EddystoneURL_InitStruct.UrlScheme = UrlScheme;
  EddystoneURL_InitStruct.Url = Url;
  EddystoneURL_InitStruct.UrlLength = sizeof(Url) - 1;

  EddystoneTLM_InitStruct.AdvertisingInterval = ADVERTISING_INTERVAL_IN_MS;
  EddystoneTLM_InitStruct.TLM_Version       = 0;
  EddystoneTLM_InitStruct.BatteryVoltage = 3000;
  EddystoneTLM_InitStruct.BeaconTemperature = 10000;
  EddystoneTLM_InitStruct.Uptime = 2000000;
  EddystoneTLM_InitStruct.AdvertisingCount = 3000000;

  EddystoneUID_InitStruct.AdvertisingInterval = ADVERTISING_INTERVAL_IN_MS;
  EddystoneUID_InitStruct.CalibratedTxPower = CALIBRATED_TX_POWER_AT_0_M;
  EddystoneUID_InitStruct.NamespaceID = NamespaceID;
  EddystoneUID_InitStruct.BeaconID = BeaconID;
#endif

  HW_TS_Create(CFG_TIM_PROC_ID_ISR, &(TimerTLM_Id), hw_ts_SingleShot, EddystoneTLM);

#ifdef USE_OTA
  if(((*(uint8_t *)(OTA_BEACON_DATA_ADDRESS)) !=  0xFF) &&
     (length !=  0xFF))
  {
    /* Service Data Updated via OTA */
    data_address += 9; /* 0x8006012 */
    if((*(uint8_t *)data_address) == 0x00)
    {
      /* Eddystone UID */
      uint8_t i, NameId[10], BeaconId[6];

      EddystoneUID_InitStruct.NamespaceID = NameId;
      EddystoneUID_InitStruct.BeaconID = BeaconId;

      data_address += 1; /* 0x8006013 */
      EddystoneUID_InitStruct.CalibratedTxPower   = *(uint8_t *)data_address;
      data_address += 1; /* 0x8006014 */
      for(i = 0; i < 10; i++)
       EddystoneUID_InitStruct.NamespaceID[i] = *(uint8_t *)(data_address + i);
      data_address += 10; /* 0x800601E */
      for(i = 0; i < 6; i++)
        EddystoneUID_InitStruct.BeaconID[i] = *(uint8_t *)(data_address + i);
      data_address += 8; /* 0x8006026 */

      EddystoneUID_Init(&EddystoneUID_InitStruct);
      tlm_adv = TRUE;
      /* 10s of UID advertise */
      HW_TS_Start(TimerTLM_Id, DEFAULT_BEACON_SEC * 10);
    }
    else if((*(uint8_t *)data_address) == 0x10)
    {
      /* Eddystone URL */
      uint8_t i;
      uint8_t Url[100];

      EddystoneURL_InitStruct.Url = Url;
      EddystoneURL_InitStruct.AdvertisingInterval = ADVERTISING_INTERVAL_IN_MS;

      data_address += 1; /* 0x8006013 */
      EddystoneURL_InitStruct.CalibratedTxPower   = *(uint8_t *)data_address;
      data_address += 1; /* 0x8006014 */
      EddystoneURL_InitStruct.CalibratedTxPower   = *(uint8_t *)data_address;
      data_address += 1; /* 0x8006015 */
      for(i = 0; i < EddystoneURL_InitStruct.UrlLength - 6; i++)
        EddystoneURL_InitStruct.Url[i] = *(uint8_t *)(data_address + i);
      EddystoneURL_InitStruct.UrlLength           = *(uint8_t *)(OTA_BEACON_DATA_ADDRESS + OFFSET_PAYLOAD_LENGTH + 5); /* 0x800600e */
      data_address += EddystoneURL_InitStruct.UrlLength - 6; /* 0x8006015 + EddystoneURL_InitStruct.UrlLength - 6 */

      EddystoneURL_Init(&EddystoneURL_InitStruct);
      tlm_adv = TRUE;
      /* 10s of URL advertise */
      HW_TS_Start(TimerTLM_Id, DEFAULT_BEACON_SEC * 10);
    }
  }
  else
#endif
  {
    uint8_t UrlScheme     = URL_PREFIX;
    uint8_t Url[]         = PHYSICAL_WEB_URL;
    uint8_t NamespaceID[] = { NAMESPACE_ID };
    uint8_t BeaconID[]    = { BEACON_ID };

    EddystoneURL_InitStruct.AdvertisingInterval = ADVERTISING_INTERVAL_IN_MS;
    EddystoneURL_InitStruct.CalibratedTxPower   = CALIBRATED_TX_POWER_AT_0_M;
    EddystoneURL_InitStruct.UrlScheme           = UrlScheme;
    EddystoneURL_InitStruct.Url                 = Url;
    EddystoneURL_InitStruct.UrlLength           = sizeof(Url) - 1;

    EddystoneTLM_InitStruct.AdvertisingInterval = ADVERTISING_INTERVAL_IN_MS;
    EddystoneTLM_InitStruct.TLM_Version         = 0;
    EddystoneTLM_InitStruct.BatteryVoltage      = 3000;
    EddystoneTLM_InitStruct.BeaconTemperature   = 10000;
    EddystoneTLM_InitStruct.Uptime              = 2000000;
    EddystoneTLM_InitStruct.AdvertisingCount    = 3000000;

    EddystoneUID_InitStruct.AdvertisingInterval = ADVERTISING_INTERVAL_IN_MS;
    EddystoneUID_InitStruct.CalibratedTxPower   = CALIBRATED_TX_POWER_AT_0_M;
    EddystoneUID_InitStruct.NamespaceID         = NamespaceID;
    EddystoneUID_InitStruct.BeaconID            = BeaconID;

    EddystoneURL_Init(&EddystoneURL_InitStruct);
    tlm_adv = TRUE;

    /* 10s of URL advertise */
    HW_TS_Start(TimerTLM_Id, DEFAULT_BEACON_SEC * 10);
  }
}
