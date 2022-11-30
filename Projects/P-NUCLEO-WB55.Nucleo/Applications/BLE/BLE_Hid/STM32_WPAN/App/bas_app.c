/**
  ******************************************************************************
 * @file    bas_app.c
 * @author  MCD Application Team
 * @brief   Battery Service Application
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


/* Includes ------------------------------------------------------------------*/
#include "app_common.h"

#include "dbg_trace.h"
#include "bas_app.h"
#include "ble.h"
#include "stm32_seq.h"

#include "app_ble.h"


/* Private typedef -----------------------------------------------------------*/
typedef struct
{
  uint16_t  Level;
  uint8_t   TimerLevel_Id;
} BSAAPP_Context_t;


/* Private defines -----------------------------------------------------------*/
#define BASAPP_DEFAULT_BAT_LEVEL       100  /**100% */
#define BASAPP_DEFAULT_BAT_LEVEL_CHG   (5000000/CFG_TS_TICK_VAL)  /**< 5s */


/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/**
 * START of Section BLE_APP_CONTEXT
 */

BSAAPP_Context_t BASAPP_Context[BLE_CFG_BAS_NUMBER];

/**
 * END of Section BLE_APP_CONTEXT
 */

/* Global variables ----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
static void BASAPP_UpdateLevel( void );


/* Functions Definition ------------------------------------------------------*/
/* Private functions ----------------------------------------------------------*/
/* Public functions ----------------------------------------------------------*/

void BAS_Notification(BAS_Notification_evt_t *pNotification)
{
  switch(pNotification->BAS_Evt_Opcode)
  {
    case BAS_LEVEL_NOT_ENABLED_EVT:
      {
        HW_TS_Stop(BASAPP_Context[pNotification->ServiceInstance].TimerLevel_Id);
        HW_TS_Start(BASAPP_Context[pNotification->ServiceInstance].TimerLevel_Id, BASAPP_DEFAULT_BAT_LEVEL_CHG);
      }
      break;

    case BAS_LEVEL_NOT_DISABLED_EVT:
      {
        HW_TS_Stop(BASAPP_Context[pNotification->ServiceInstance].TimerLevel_Id);
      }
      break;

    case BAS_LEVEL_READ_EVT:
      {
        if(BASAPP_Context[pNotification->ServiceInstance].Level > 0)
          BASAPP_Context[pNotification->ServiceInstance].Level -= 1;
        else
          BASAPP_Context[pNotification->ServiceInstance].Level = BASAPP_DEFAULT_BAT_LEVEL;
        BAS_Update_Char(BATTERY_LEVEL_CHAR_UUID,
                        pNotification->ServiceInstance,
                        (uint8_t *)&BASAPP_Context[pNotification->ServiceInstance].Level);
      }
      break;

    default:
      break;
  }

  return;
}

void BASAPP_Init(uint8_t index)
{
  UTIL_SEQ_RegTask( 1<< CFG_TASK_BAS_LEVEL_REQ_ID, UTIL_SEQ_RFU, BASAPP_Level );

  
  /**
   * Initialize Level
   */
  BASAPP_Context[index].Level = BASAPP_DEFAULT_BAT_LEVEL;
  BAS_Update_Char(BATTERY_LEVEL_CHAR_UUID, index, (uint8_t *)&BASAPP_Context[index].Level);

  /**
   * Create timer for Battery Level
   */
  HW_TS_Create(CFG_TIM_PROC_ID_ISR, &(BASAPP_Context[index].TimerLevel_Id), hw_ts_Repeated, BASAPP_UpdateLevel);

  return;
}


void BASAPP_Level(void)
{
  uint8_t index;
  
  for(index = 0; index < BLE_CFG_BAS_NUMBER; index++)
  {
    if(BASAPP_Context[index].Level > 0)
      BASAPP_Context[index].Level -= 1;
    else
      BASAPP_Context[index].Level = BASAPP_DEFAULT_BAT_LEVEL;

    BAS_Update_Char(BATTERY_LEVEL_CHAR_UUID, index, (uint8_t *)&BASAPP_Context[index].Level);
  }

  return;
}

static void BASAPP_UpdateLevel( void )
{
  /**
   * The code shall be executed in the background as aci command may be sent
   * The background is the only place where the application can make sure a new aci command
   * is not sent if there is a pending one
   */
  UTIL_SEQ_SetTask( 1<<CFG_TASK_BAS_LEVEL_REQ_ID, CFG_SCH_PRIO_0);

  return;
}


