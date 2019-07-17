/**
 ******************************************************************************
 * @file    otas_app.c
 * @author  MCD Application Team
 * @brief   OTA Service Application
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



/* Includes ------------------------------------------------------------------*/
#include "app_common.h"

#include "ble.h"
#include "shci.h"



/* Private typedef -----------------------------------------------------------*/

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
          memcpy( (uint8_t*)&OTAS_APP_Context.base_address, ((OTA_STM_Base_Addr_Event_Format_t*)(p_notification->pPayload))->Base_Addr, p_notification->ValueLength -1 );
          OTAS_APP_Context.write_value_index = 0;
          break;

        case OTAS_STM_APPLICATION_UPLOAD:
          OTAS_APP_Context.file_type = Fw_App;
          OTAS_APP_Context.base_address = FLASH_BASE;
          memcpy( (uint8_t*)&OTAS_APP_Context.base_address, ((OTA_STM_Base_Addr_Event_Format_t*)(p_notification->pPayload))->Base_Addr, p_notification->ValueLength -1 );
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

      while( LL_HSEM_1StepLock( HSEM, CFG_HW_FLASH_SEMID ) );
      HAL_FLASH_Unlock();
      while( size_left >= (8 - OTAS_APP_Context.write_value_index) )
      {
        memcpy( (uint8_t*)&OTAS_APP_Context.write_value + OTAS_APP_Context.write_value_index,
                ((OTA_STM_Raw_Data_Event_Format_t*)(p_notification->pPayload))->Raw_Data + count,
                8 - OTAS_APP_Context.write_value_index );
        while(LL_FLASH_IsActiveFlag_OperationSuspended());
        HAL_FLASH_Program( FLASH_TYPEPROGRAM_DOUBLEWORD,
                           OTAS_APP_Context.base_address,
                           OTAS_APP_Context.write_value);
        if(*(uint64_t*)(OTAS_APP_Context.base_address)==OTAS_APP_Context.write_value)
        {
          OTAS_APP_Context.base_address += 8;
          size_left -= (8 - OTAS_APP_Context.write_value_index);
          count += (8 - OTAS_APP_Context.write_value_index);
          OTAS_APP_Context.write_value_index = 0;
        }
      }
      HAL_FLASH_Lock();
      LL_HSEM_ReleaseLock( HSEM, CFG_HW_FLASH_SEMID, 0 );

      /**
       * The Flash shall be written by 32bits data. In case the packet received is not a multiple of 4 bytes,
       * it shall be recorded how much bytes is left to pe written in flash
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
       * 4 bytes to be written
       * As it is mandatory to write 4 bytes, some random bytes will be written at the same time than the left
       * unwritten bytes to complete up to 4 bytes.
       */
      if(OTAS_APP_Context.write_value_index != 0)
      {
        while( LL_HSEM_1StepLock( HSEM, CFG_HW_FLASH_SEMID ) );
        HAL_FLASH_Unlock();
        while(*(uint64_t*)(OTAS_APP_Context.base_address) != OTAS_APP_Context.write_value)
        {
          while(LL_FLASH_IsActiveFlag_OperationSuspended());
          HAL_FLASH_Program( FLASH_TYPEPROGRAM_DOUBLEWORD,
                             OTAS_APP_Context.base_address,
                             OTAS_APP_Context.write_value);
        }
        HAL_FLASH_Lock();
        LL_HSEM_ReleaseLock( HSEM, CFG_HW_FLASH_SEMID, 0 );
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
          NVIC_SystemReset(); /* it waits until reset */
          break;

        case Wireless_Fw:
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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
