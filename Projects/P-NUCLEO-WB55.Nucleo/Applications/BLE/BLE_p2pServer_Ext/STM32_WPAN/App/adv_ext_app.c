/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : App/adv_ext_app.c
  * Description        : Advertising Extension Application
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
#include "adv_ext_app.h"
#include "main.h"
#include "app_common.h"
#include "ble.h"
#include "dbg_trace.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
typedef struct
{
  uint8_t enable;
  Adv_Set_t adv_set;  
  uint8_t* data;
  uint16_t data_len;
  uint8_t sid;
  uint16_t property;
  uint16_t interval_min;
  uint16_t interval_max;
  int8_t tx_power;
  uint8_t adv_channels;
  uint8_t address_type;
  uint8_t peer_address_type;
  uint8_t* p_peer_address;
  char username[30];
} Adv_Set_Param_t;

/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private defines ------------------------------------------------------------*/
#define ADV_SET_COUNT                  (8)
#define DATA_SLICE_SIZE                (251)
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macros -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
#if (CFG_BLE_NUM_LINK < CFG_BLE_MAX_ADV_SET_NBR)
#error Parameter CFG_BLE_NUM_LINK must be above CFG_BLE_MAX_ADV_SET_NBR to be able to use all adv sets.
#endif
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
Adv_Set_Param_t adv_set_param[ADV_SET_COUNT];
static uint8_t a_Adv_Data[1650];
static uint8_t a_p2p_legacy_buff[32];
static uint8_t a_p2p_extended_buff[32];
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
static uint8_t ADV_EXT_Build_data(Adv_Set_Param_t *adv_param);
static uint8_t ADV_EXT_Set_data(Adv_Set_Param_t *adv_param, uint8_t data_slice, uint16_t adv_length_max);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* External variables --------------------------------------------------------*/
extern RNG_HandleTypeDef hrng;
extern uint8_t a_ManufData[14];
/* USER CODE BEGIN EV */

/* USER CODE END EV */

/* Functions Definition ------------------------------------------------------*/
uint8_t ADV_EXT_Config(void)
{
  uint16_t data_cpt;
  uint8_t i;

/* Extended SCANNABLE */
  i = 0;
  adv_set_param[i].enable = 0;
  adv_set_param[i].data = NULL;
  adv_set_param[i].data_len = 200;
  strcpy(adv_set_param[i].username, "extended_scannable");
  adv_set_param[i].sid = i;
  adv_set_param[i].interval_min = 1000;
  adv_set_param[i].interval_max = 1000;
  adv_set_param[i].tx_power = CFG_BLE_MAX_TX_POWER;
  adv_set_param[i].adv_channels = ADV_CH_37 | ADV_CH_38 | ADV_CH_39;
  adv_set_param[i].property = HCI_ADV_EVENT_PROP_SCANNABLE;
  adv_set_param[i].address_type = GAP_STATIC_RANDOM_ADDR;
  adv_set_param[i].peer_address_type = GAP_PUBLIC_ADDR;
  adv_set_param[i].p_peer_address = NULL;
  adv_set_param[i].adv_set.Advertising_Handle = i;
  adv_set_param[i].adv_set.Duration = 0;
  adv_set_param[i].adv_set.Max_Extended_Advertising_Events = 0;  

/* Extended NO SCANNABLE-NO CONNECTABLE */
  i = 1;
  adv_set_param[i].enable = 1;
  adv_set_param[i].data = NULL;
  adv_set_param[i].data_len = 1650;
  strcpy(adv_set_param[i].username, "ext_noscan_noconn");
  adv_set_param[i].sid = i;
  adv_set_param[i].interval_min = 300;
  adv_set_param[i].interval_max = 400;
  adv_set_param[i].tx_power = CFG_BLE_MAX_TX_POWER;
  adv_set_param[i].adv_channels = ADV_CH_37 | ADV_CH_38 | ADV_CH_39;
  adv_set_param[i].property = 0x00;
  adv_set_param[i].address_type = GAP_PUBLIC_ADDR;
  adv_set_param[i].peer_address_type = GAP_PUBLIC_ADDR;
  adv_set_param[i].p_peer_address = NULL;
  adv_set_param[i].adv_set.Advertising_Handle = i;
  adv_set_param[i].adv_set.Duration = 0;
  adv_set_param[i].adv_set.Max_Extended_Advertising_Events = 0;

/* Extended CONNECTABLE with p2pClient_Ext device */
  data_cpt = 0;
  memcpy(&a_p2p_extended_buff[0], &a_ManufData[0], sizeof(a_ManufData));
  data_cpt += sizeof(a_ManufData);
  a_p2p_extended_buff[data_cpt++] = strlen("EXT_P2P") + 1;
  a_p2p_extended_buff[data_cpt++] = AD_TYPE_COMPLETE_LOCAL_NAME;
  memcpy(&a_p2p_extended_buff[data_cpt], "EXT_P2P", strlen("EXT_P2P"));
  data_cpt += strlen("EXT_P2P");
  
  i = 2;
  adv_set_param[i].enable = 1;
  adv_set_param[i].data = &a_p2p_extended_buff[0];
  adv_set_param[i].data_len = data_cpt;
  strcpy(adv_set_param[i].username, "EXT_P2P");
  adv_set_param[i].sid = i;
  adv_set_param[i].interval_min = 100;
  adv_set_param[i].interval_max = 200;
  adv_set_param[i].tx_power = CFG_BLE_MAX_TX_POWER;
  adv_set_param[i].adv_channels = ADV_CH_37 | ADV_CH_38 | ADV_CH_39;
  adv_set_param[i].property = HCI_ADV_EVENT_PROP_CONNECTABLE;
  adv_set_param[i].address_type = GAP_STATIC_RANDOM_ADDR;
  adv_set_param[i].peer_address_type = GAP_PUBLIC_ADDR;
  adv_set_param[i].p_peer_address = NULL;
  adv_set_param[i].adv_set.Advertising_Handle = i;
  adv_set_param[i].adv_set.Duration = 0;
  adv_set_param[i].adv_set.Max_Extended_Advertising_Events = 0;

/* Legacy */
  data_cpt = 0;
  a_p2p_legacy_buff[data_cpt++] = 0x02;
  a_p2p_legacy_buff[data_cpt++] = AD_TYPE_FLAGS;
  a_p2p_legacy_buff[data_cpt++] = FLAG_BIT_LE_GENERAL_DISCOVERABLE_MODE | FLAG_BIT_BR_EDR_NOT_SUPPORTED;
  memcpy(&a_p2p_legacy_buff[data_cpt], &a_ManufData[0], sizeof(a_ManufData));
  data_cpt += sizeof(a_ManufData);
  a_p2p_legacy_buff[data_cpt++] = strlen("LEGACY") + 1;
  a_p2p_legacy_buff[data_cpt++] = AD_TYPE_COMPLETE_LOCAL_NAME;
  memcpy(&a_p2p_legacy_buff[data_cpt], "LEGACY", strlen("LEGACY"));
  data_cpt += strlen("LEGACY");
  
  i = 3;
  adv_set_param[i].enable = 1;
  adv_set_param[i].data = &a_p2p_legacy_buff[0];
  adv_set_param[i].data_len = data_cpt;
  strcpy(adv_set_param[i].username, "LEGACY");
  adv_set_param[i].sid = i;
  adv_set_param[i].interval_min = 100;
  adv_set_param[i].interval_max = 200;
  adv_set_param[i].tx_power = CFG_BLE_MAX_TX_POWER;
  adv_set_param[i].adv_channels = ADV_CH_37 | ADV_CH_38 | ADV_CH_39;
  adv_set_param[i].property = HCI_ADV_EVENT_PROP_CONNECTABLE | HCI_ADV_EVENT_PROP_LEGACY | HCI_ADV_EVENT_PROP_SCANNABLE;
  adv_set_param[i].address_type = GAP_PUBLIC_ADDR;
  adv_set_param[i].peer_address_type = GAP_PUBLIC_ADDR;
  adv_set_param[i].p_peer_address = NULL;
  adv_set_param[i].adv_set.Advertising_Handle = i;
  adv_set_param[i].adv_set.Duration = 0;
  adv_set_param[i].adv_set.Max_Extended_Advertising_Events = 0;

  return 0;
}

uint8_t ADV_EXT_Start(void)
{
  tBleStatus status = BLE_STATUS_INVALID_PARAMS;
  uint16_t adv_length_max;  
  uint8_t adv_set_count_supported, loop, adv_set_count;
  uint32_t a_bd_addr[2];
  
  APP_DBG_MSG("--\nBegin to configure and start advertising sets defined in the application : \n" );
  
  status = hci_le_read_number_of_supported_advertising_sets(&adv_set_count_supported);
  if (status == BLE_STATUS_SUCCESS)
  {
    APP_DBG_MSG("Number of advertising sets supported : %d\n" ,adv_set_count_supported);
  }
  else
  {
    Error_Handler();
  }
  
  status = hci_le_read_maximum_advertising_data_length(&adv_length_max);
  if (status == BLE_STATUS_SUCCESS)
  {
    APP_DBG_MSG("Advertising data length max          : %d\n" ,adv_length_max);
  }
  else
  {
    Error_Handler();
  }
  
  adv_set_count = 0;
  for (loop = 0 ; loop < ADV_SET_COUNT ; loop++)
  {
    if (adv_set_param[loop].enable != 0)
    {
      APP_DBG_MSG("--\n");
      adv_set_count++;
      if(adv_set_count > adv_set_count_supported)
      {
        APP_DBG_MSG("Advertising set count limit has been reached ! please update parameters app_conf.h file\n");
      }
      APP_DBG_MSG("  Adv name     : %s\n", (char*)adv_set_param[loop].username );
      APP_DBG_MSG("  SID          : %d\n", adv_set_param[loop].sid);
      APP_DBG_MSG("  Data length  : %d\n", adv_set_param[loop].data_len );
      
      status = aci_gap_adv_set_configuration(0,
                                              adv_set_param[loop].adv_set.Advertising_Handle,
                                              adv_set_param[loop].property,
                                              adv_set_param[loop].interval_min,
                                              adv_set_param[loop].interval_max,
                                              adv_set_param[loop].adv_channels,
                                              adv_set_param[loop].address_type,
                                              adv_set_param[loop].peer_address_type,
                                              adv_set_param[loop].p_peer_address,
                                              NO_WHITE_LIST_USE,
                                              (uint8_t)adv_set_param[loop].tx_power,
                                              0x00,
                                              0x01,
                                              adv_set_param[loop].sid,
                                              0x01);
      if (status == BLE_STATUS_SUCCESS)
      {
        APP_DBG_MSG("  Success: aci_gap_adv_set_configuration\n");
      }
      else
      {
        APP_DBG_MSG("  Fail   : aci_gap_adv_set_configuration, result: 0x%02X\n", status);
      }
      
      if (adv_set_param[loop].address_type == GAP_STATIC_RANDOM_ADDR)
      {
        /* Get RNG semaphore */
        while(LL_HSEM_1StepLock(HSEM, CFG_HW_RNG_SEMID));

        /* Enable RNG */
        __HAL_RNG_ENABLE(&hrng);
        
        if (HAL_RNG_GenerateRandomNumber(&hrng, &a_bd_addr[1]) != HAL_OK)
        {
          /* Random number generation error */
          Error_Handler();
        }
        if (HAL_RNG_GenerateRandomNumber(&hrng, &a_bd_addr[0]) != HAL_OK)
        {
          /* Random number generation error */
          Error_Handler();
        }
        a_bd_addr[1] |= 0xC000; /* The two upper bits shall be set to 1 */

        /* Disable RNG */
        __HAL_RNG_DISABLE(&hrng);

        /* Release RNG semaphore */
        LL_HSEM_ReleaseLock(HSEM, CFG_HW_RNG_SEMID, 0);
        
        APP_DBG_MSG("  Random Bluetooth address: %02X:%02X:%02X:%02X:%02X:%02X\n", 
                     (uint8_t)(a_bd_addr[1] >> 8),
                     (uint8_t)(a_bd_addr[1]),
                     (uint8_t)(a_bd_addr[0] >> 24),
                     (uint8_t)(a_bd_addr[0] >> 16),
                     (uint8_t)(a_bd_addr[0] >> 8),
                     (uint8_t)(a_bd_addr[0]));

        
        status = hci_le_set_advertising_set_random_address(adv_set_param[loop].adv_set.Advertising_Handle,
                                                           (uint8_t*)&a_bd_addr[0]);
        if (status == BLE_STATUS_SUCCESS)
        {
          APP_DBG_MSG("  Success: hci_le_set_advertising_set_random_address\n");
        }
        else
        {
          APP_DBG_MSG("  Fail   : hci_le_set_advertising_set_random_address, result: 0x%02X\n", status);
        }
      }
      else
      {
        APP_DBG_MSG("  Public Bluetooth address used\n");
      }
      
      if (adv_set_param[loop].data == NULL)
      {
        adv_set_param[loop].data = &a_Adv_Data[0];
            
        status = ADV_EXT_Build_data(&adv_set_param[loop]);
        if (status == BLE_STATUS_SUCCESS)
        {
          APP_DBG_MSG("  Success: ADV_EXT_Build_data\n");
        }
        else
        {
          APP_DBG_MSG("  Fail   : ADV_EXT_Build_data, result: 0x%02X\n", status);
        }
      }
    
      status = ADV_EXT_Set_data(&adv_set_param[loop], DATA_SLICE_SIZE, adv_length_max);
      if (status == BLE_STATUS_SUCCESS)
      {
        APP_DBG_MSG("  Success: aci_gap_adv_set_adv_data/aci_gap_adv_set_scan_resp_data\n");
      }
      else
      {
        APP_DBG_MSG("  Fail   : aci_gap_adv_set_adv_data/aci_gap_adv_set_scan_resp_data, result: 0x%02X\n", status);
      }
      
      status = aci_gap_adv_set_enable(1, 1, &adv_set_param[loop].adv_set);
      if (status == BLE_STATUS_SUCCESS)
      {
        APP_DBG_MSG("  Success: aci_gap_adv_set_enable\n");
      }
      else
      {
        APP_DBG_MSG("  Fail   : aci_gap_adv_set_enable, result: 0x%02X\n", status);
      }
    }}
    
  return status;
}

/* USER CODE BEGIN FD */

/* USER CODE END FD */

/*************************************************************
 *
 * LOCAL FUNCTIONS
 *
 *************************************************************/

static uint8_t ADV_EXT_Set_data(Adv_Set_Param_t *adv_param, uint8_t data_slice, uint16_t adv_length_max)
{
  uint16_t i, data_len;
  uint8_t status, adv_operation;
  
  if (adv_param->data_len > adv_length_max)
  {
    status = 1;
  }
  else
  {
    status = 0;    
    for (i = 0 ; i < adv_param->data_len ; i += data_slice)
    {
      if( adv_param->data_len <= data_slice )
      {
        adv_operation = HCI_SET_ADV_DATA_OPERATION_COMPLETE;
        data_len = adv_param->data_len;
      }
      else
      {
        if (i == 0)
        {
          adv_operation = HCI_SET_ADV_DATA_OPERATION_FIRST;
          data_len = data_slice;
        }
        else if ((i + data_slice) > adv_param->data_len)
        {
          adv_operation = HCI_SET_ADV_DATA_OPERATION_LAST;
          data_len = adv_param->data_len - i;
        }
        else
        {
          adv_operation = HCI_SET_ADV_DATA_OPERATION_INTERMEDIATE;
          data_len = data_slice;
        }
      }
      
      if (adv_param->property == HCI_ADV_EVENT_PROP_SCANNABLE )
      {
         status = aci_gap_adv_set_scan_resp_data(adv_param->adv_set.Advertising_Handle, 
                                                  adv_operation, 
                                                  0, 
                                                  data_len,
                                                  &adv_param->data[i]);
      }
      else
      {
         status = aci_gap_adv_set_adv_data(adv_param->adv_set.Advertising_Handle, 
                                           adv_operation, 
                                           0, 
                                           data_len,
                                           &adv_param->data[i]);
      }
      if(status != 0)
      {
        break;
      }
    }
  }

  return status;
}

static uint8_t ADV_EXT_Build_data(Adv_Set_Param_t *adv_param)
{
  uint16_t data_cpt, cpt;
  int16_t manuf_cpt;
  uint8_t i, status = 0;
  
  data_cpt = 0;
  memset(&adv_param->data[0], 0, adv_param->data_len);
  
  adv_param->data[data_cpt++] = 0x02;
  adv_param->data[data_cpt++] = AD_TYPE_FLAGS;
  adv_param->data[data_cpt++] = FLAG_BIT_LE_GENERAL_DISCOVERABLE_MODE | FLAG_BIT_BR_EDR_NOT_SUPPORTED;

  adv_param->data[data_cpt++] = strlen(adv_param->username) + 1;
  adv_param->data[data_cpt++] = AD_TYPE_COMPLETE_LOCAL_NAME;
  memcpy(&adv_param->data[data_cpt], &adv_param->username[0], strlen(adv_param->username));
  data_cpt += strlen(adv_param->username);
  
  manuf_cpt = (adv_param->data_len - data_cpt);
  if(manuf_cpt <= 2)
  {
    status = 1;
  }
  else
  {
    while (manuf_cpt > 0)
    {
      if (manuf_cpt >= 0xFE)
      {
        cpt = 0xFE - 2;
      }
      else
      {
        cpt = manuf_cpt - 2;
      }
      adv_param->data[data_cpt++] = cpt + 1;
      adv_param->data[data_cpt++] = AD_TYPE_MANUFACTURER_SPECIFIC_DATA;
      adv_param->data[data_cpt++] = 0x30; /* STMicroelectronis company ID */
      cpt--;
      adv_param->data[data_cpt++] = 0x00;    
      cpt--;
      for (i = 0 ; i < cpt; i++)
      {
        adv_param->data[data_cpt + i] = i;
      }
      data_cpt += cpt;
      
      manuf_cpt = (adv_param->data_len - data_cpt);
    }
  }
  
  return status;
}
/* USER CODE BEGIN FD_LOCAL_FUNCTIONS*/

/* USER CODE END FD_LOCAL_FUNCTIONS*/
