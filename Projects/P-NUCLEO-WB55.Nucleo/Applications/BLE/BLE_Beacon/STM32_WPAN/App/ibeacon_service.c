/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    ibeacon_service.c
  * @author  MCD Application Team
  * @brief   
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
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "app_common.h"
#include "ble.h"
#include "ibeacon.h"
#include "ibeacon_service.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef ------------------------------------------------------------*/

typedef struct
{
    uint16_t AdvertisingInterval;/*!< Specifies the desired advertising interval. */
    uint8_t * UuID;              /*!< Specifies the 16-byte UUID to which the beacon belongs. */
    uint8_t * MajorID;           /*!< Specifies the 2-byte beacon identifying information. */
    uint8_t * MinorID;           /*!< Specifies the 2-byte beacon identifying information. */
    uint8_t CalibratedTxPower;   /*!< Specifies the power at 1m. */
} IBeacon_InitTypeDef;
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
#define ADVERTISING_INTERVAL_INCREMENT (16)
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Functions Definition ------------------------------------------------------*/

static tBleStatus IBeacon_Init(IBeacon_InitTypeDef *IBeacon_Init)
{
/* USER CODE BEGIN IBeacon_Init_1 */

/* USER CODE END IBeacon_Init_1 */
  tBleStatus ret = BLE_STATUS_SUCCESS;
  uint16_t AdvertisingInterval = (IBeacon_Init->AdvertisingInterval * ADVERTISING_INTERVAL_INCREMENT / 10);

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

  uint8_t service_data[] =
  {
    26,                                                                      /*< Length. */
    AD_TYPE_MANUFACTURER_SPECIFIC_DATA,                                      /*< Manufacturer Specific Data data type value. */
    0x4C, 0x00, 0x02, 0x15,                                                  /*< 32-bit Manufacturer Data. */
    IBeacon_Init->UuID[0],                                                   /*< 16-byte Proximity UUID. */
    IBeacon_Init->UuID[1],
    IBeacon_Init->UuID[2],
    IBeacon_Init->UuID[3],
    IBeacon_Init->UuID[4],
    IBeacon_Init->UuID[5],
    IBeacon_Init->UuID[6],
    IBeacon_Init->UuID[7],
    IBeacon_Init->UuID[8],
    IBeacon_Init->UuID[9],
    IBeacon_Init->UuID[10],
    IBeacon_Init->UuID[11],
    IBeacon_Init->UuID[12],
    IBeacon_Init->UuID[13],
    IBeacon_Init->UuID[14],
    IBeacon_Init->UuID[15],
    IBeacon_Init->MajorID[0],                                                /*< 2-byte Major. */
    IBeacon_Init->MajorID[1],
    IBeacon_Init->MinorID[0],                                                /*< 2-byte Minor. */
    IBeacon_Init->MinorID[1],
    IBeacon_Init->CalibratedTxPower,                                         /*< Ranging data. */
  };

  uint8_t flags[] =
  {
    2,                                                                      /*< Length. */
    AD_TYPE_FLAGS,                                                          /*< Flags data type value. */
    (FLAG_BIT_LE_GENERAL_DISCOVERABLE_MODE | FLAG_BIT_BR_EDR_NOT_SUPPORTED) /*< BLE general discoverable, without BR/EDR support. */
  };

  /* Update the service data. */
  ret = aci_gap_update_adv_data(sizeof(service_data), service_data);

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
/* USER CODE BEGIN IBeacon_Init_2 */

/* USER CODE END IBeacon_Init_2 */
  return ret;
}

void IBeacon_Process(void)
{
/* USER CODE BEGIN IBeacon_Process_1 */

/* USER CODE END IBeacon_Process_1 */
  /* Default ibeacon */
  uint8_t UuID[]    = { UUID };
  uint8_t MajorID[] = { MAJOR_ID };
  uint8_t MinorID[] = { MINOR_ID };

  IBeacon_InitTypeDef IBeacon_InitStruct =
  {
    .AdvertisingInterval = ADVERTISING_INTERVAL_IN_MS,
    .UuID                = UuID,
    .MajorID             = MajorID,
    .MinorID             = MinorID,
    .CalibratedTxPower   = CALIBRATED_TX_POWER_AT_1_M
  };

#ifdef USE_OTA
  if(((*(uint8_t *)(OTA_BEACON_DATA_ADDRESS + OFFSET_PAYLOAD_LENGTH)) ==  27) &&
     ((*(uint8_t *)(OTA_BEACON_DATA_ADDRESS + OFFSET_PAYLOAD_DATA)) ==  26))
  {
    uint8_t i;
    uint32_t data_address = OTA_BEACON_DATA_ADDRESS + OFFSET_PAYLOAD_DATA + 6;

    for(i = 0; i < 16; i++)
      IBeacon_InitStruct.UuID[i] = *(uint8_t *)(data_address + i);
    data_address += 16;
    for(i = 0; i < 2; i++)
      IBeacon_InitStruct.MinorID[i] = *(uint8_t *)(data_address + i);
    data_address += 2;
    for(i = 0; i < 2; i++)
      IBeacon_InitStruct.MajorID[i] = *(uint8_t *)(data_address + i);
    data_address += 2;
    IBeacon_InitStruct.CalibratedTxPower = *(uint8_t *)(data_address);
  }
#endif

  IBeacon_Init(&IBeacon_InitStruct);
/* USER CODE BEGIN IBeacon_Process_2 */

/* USER CODE END IBeacon_Process_2 */
}
/* USER CODE BEGIN FD */

/* USER CODE END FD */
