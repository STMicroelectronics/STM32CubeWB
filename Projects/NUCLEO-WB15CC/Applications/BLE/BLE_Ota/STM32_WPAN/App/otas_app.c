/**
  ******************************************************************************
 * @file    otas_app.c
 * @author  MCD Application Team
 * @brief   OTA Service Application
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



/* Includes ------------------------------------------------------------------*/
#include "app_common.h"

#include "ble.h"
#include "shci.h"

#ifdef OTA_SBSFU
#include "ota_sbsfu.h"
#endif /* OTA_SBSFU */

#include "flash_driver.h"

/* Private typedef -----------------------------------------------------------*/
typedef enum
{
  Wireless_Fw,
  Fw_App,
} OTAS_APP_FileType_t;

typedef struct
{
  uint32_t base_address;
  uint64_t write_value;
  uint8_t  write_value_index;
  uint8_t  file_type;
} OTAS_APP_Context_t;

/* Private macros ------------------------------------------------------------*/
/* Number of bytes to be written in flash on each programming sequence */
#define DOUBLEWORD_SIZE_FOR_FLASH_PROGRAMMING       (8)

/* Private variables ---------------------------------------------------------*/
OTAS_APP_Context_t OTAS_APP_Context;

/* Global variables ----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Functions Definition ------------------------------------------------------*/
/* Private functions ----------------------------------------------------------*/
/* Public functions ----------------------------------------------------------*/

void OTAS_STM_Notification( OTA_STM_Notification_t *p_notification )
{
  uint32_t count;
  uint32_t size_left;
  OTAS_STM_Indication_Msg_t msg_conf;

  switch(p_notification->ChardId)
  {
    case OTAS_STM_BASE_ADDR_ID:
    {
      switch( ((OTA_STM_Base_Addr_Event_Format_t*)(p_notification->pPayload))->Command )
      {
        case OTAS_STM_STOP_ALL_UPLOAD:
          break;

        case OTAS_STM_WIRELESS_FW_UPLOAD:
          OTAS_APP_Context.file_type = Wireless_Fw;
          OTAS_APP_Context.base_address = FLASH_BASE;
          ((uint8_t*)&OTAS_APP_Context.base_address)[0] = (((uint8_t*)((OTA_STM_Base_Addr_Event_Format_t*)(p_notification->pPayload))->Base_Addr))[2];
          ((uint8_t*)&OTAS_APP_Context.base_address)[1] = (((uint8_t*)((OTA_STM_Base_Addr_Event_Format_t*)(p_notification->pPayload))->Base_Addr))[1];
          ((uint8_t*)&OTAS_APP_Context.base_address)[2] = (((uint8_t*)((OTA_STM_Base_Addr_Event_Format_t*)(p_notification->pPayload))->Base_Addr))[0];
          OTAS_APP_Context.write_value_index = 0;
          break;

        case OTAS_STM_APPLICATION_UPLOAD:
          OTAS_APP_Context.file_type = Fw_App;
          OTAS_APP_Context.base_address = FLASH_BASE;
          ((uint8_t*)&OTAS_APP_Context.base_address)[0] = (((uint8_t*)((OTA_STM_Base_Addr_Event_Format_t*)(p_notification->pPayload))->Base_Addr))[2];
          ((uint8_t*)&OTAS_APP_Context.base_address)[1] = (((uint8_t*)((OTA_STM_Base_Addr_Event_Format_t*)(p_notification->pPayload))->Base_Addr))[1];
          ((uint8_t*)&OTAS_APP_Context.base_address)[2] = (((uint8_t*)((OTA_STM_Base_Addr_Event_Format_t*)(p_notification->pPayload))->Base_Addr))[0];
          OTAS_APP_Context.write_value_index = 0;
          break;

        case OTAS_STM_UPLOAD_FINISHED:
          msg_conf = OTAS_STM_REBOOT_CONFIRMED;
          (void) OTAS_STM_UpdateChar(OTAS_STM_CONF_ID, (uint8_t*)&msg_conf);
          break;

        case OTAS_STM_CANCEL_UPLOAD:
          break;

        default:
          break;
      }
    }
    break;

    case OTAS_STM_RAW_DATA_ID:
      /**
       * Write in Flash the data received in the BLE packet
       */
      count = 0;
      size_left = p_notification->ValueLength;

      /**
       * The flash is written by bunch of DOUBLEWORD_SIZE_FOR_FLASH_PROGRAMMING bytes
       * Data are written in flash as long as there are at least DOUBLEWORD_SIZE_FOR_FLASH_PROGRAMMING bytes
       */
      while( size_left >= (DOUBLEWORD_SIZE_FOR_FLASH_PROGRAMMING - OTAS_APP_Context.write_value_index) )
      {
        uint32_t NbrOfDataToBeWritten = 1;
        
        memcpy( (uint8_t*)&OTAS_APP_Context.write_value + OTAS_APP_Context.write_value_index,
                ((OTA_STM_Raw_Data_Event_Format_t*)(p_notification->pPayload))->Raw_Data + count,
                DOUBLEWORD_SIZE_FOR_FLASH_PROGRAMMING - OTAS_APP_Context.write_value_index );
        while(NbrOfDataToBeWritten > 0)
          NbrOfDataToBeWritten = FD_WriteData(OTAS_APP_Context.base_address,
                                              &(OTAS_APP_Context.write_value),
                                              1);
        if(*(uint64_t*)(OTAS_APP_Context.base_address)==OTAS_APP_Context.write_value)
        {
          OTAS_APP_Context.base_address += DOUBLEWORD_SIZE_FOR_FLASH_PROGRAMMING;
          size_left -= (DOUBLEWORD_SIZE_FOR_FLASH_PROGRAMMING - OTAS_APP_Context.write_value_index);
          count += (DOUBLEWORD_SIZE_FOR_FLASH_PROGRAMMING - OTAS_APP_Context.write_value_index);
          OTAS_APP_Context.write_value_index = 0;
        }
      }

      /**
       * The Flash shall be written by DOUBLEWORD_SIZE_FOR_FLASH_PROGRAMMING number of bytes.
       * In case the packet received is not a multiple of DOUBLEWORD_SIZE_FOR_FLASH_PROGRAMMING bytes,
       * It shall be recorded how much bytes is left to be written in flash
       */
      if(size_left != 0)
      {
        memcpy( (uint8_t*)&OTAS_APP_Context.write_value + OTAS_APP_Context.write_value_index,
                ((OTA_STM_Raw_Data_Event_Format_t*)(p_notification->pPayload))->Raw_Data + count,
                size_left );
        OTAS_APP_Context.write_value_index += size_left;
      }
      break;

    case OTAS_STM_CONF_EVENT_ID:
    {
      /**
       * The Remote notifies it has send all the data to be written in Flash
       */

      /**
       * Write now in Flash the remaining data that has not been written before because they were less than
       * DOUBLEWORD_SIZE_FOR_FLASH_PROGRAMMING bytes to be written
       */
      if(OTAS_APP_Context.write_value_index != 0)
      {
        /**
         * As it is mandatory to write DOUBLEWORD_SIZE_FOR_FLASH_PROGRAMMING bytes, the data to be written shall be padded
         * up to DOUBLEWORD_SIZE_FOR_FLASH_PROGRAMMING bytes with 0xFF.
         * This is mandatory for correct operation with the Firmware Upgrade Service (FUS) as otherwise, it could be the magic
         * key word in wrongly duplicated
         */
        memset( (uint8_t*)&OTAS_APP_Context.write_value + OTAS_APP_Context.write_value_index,
                0xFF,
                DOUBLEWORD_SIZE_FOR_FLASH_PROGRAMMING - OTAS_APP_Context.write_value_index  );

        while(*(uint64_t*)(OTAS_APP_Context.base_address) != OTAS_APP_Context.write_value)
        {
          uint32_t NbrOfDataToBeWritten = 1;

          while(NbrOfDataToBeWritten > 0)
            NbrOfDataToBeWritten = FD_WriteData(OTAS_APP_Context.base_address,
                                                &(OTAS_APP_Context.write_value),
                                                1);
        }
      }

      /**
       * Decide now what to do after all the data has been written in Flash
       */
      switch(OTAS_APP_Context.file_type)
      {
        case Fw_App:
          /**
           * Reboot on FW Application
           */
          CFG_OTA_REBOOT_VAL_MSG = CFG_REBOOT_ON_FW_APP;
 
#ifdef OTA_SBSFU
          /* Communication with SBSFU : FW application installation by SBSFU requested */
          STANDALONE_LOADER_STATE = STANDALONE_LOADER_INSTALL_REQ;
#endif /* OTA_SBSFU */

          NVIC_SystemReset(); /* it waits until reset */
          break;

        case Wireless_Fw:
#ifdef OTA_SBSFU
          /* Communication with SBSFU : by-pass mode requested for Wireless stack installation by FUS */
          STANDALONE_LOADER_STATE = STANDALONE_LOADER_BYPASS_REQ;
          
          NVIC_SystemReset(); /* it waits until reset */
#else
          /**
           * Wireless firmware update is requested
           * Request CPU2 to reboot on FUS by sending two FUS command
           */
          SHCI_C2_FUS_GetState( NULL );
          SHCI_C2_FUS_GetState( NULL );
          while(1)
          {
            HAL_PWR_EnterSLEEPMode(PWR_MAINREGULATOR_ON, PWR_SLEEPENTRY_WFI);
          }
#endif /* OTA_SBSFU */
          break;

        default:
          break;
      }
    }
    break;

    default:
      break;
  }

  return;
}
