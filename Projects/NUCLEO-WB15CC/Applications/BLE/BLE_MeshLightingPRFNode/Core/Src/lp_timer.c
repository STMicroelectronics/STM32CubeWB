/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    lp_timer.c
  * @author  MCD Application Team
  * @brief   Low power timer to be used within Mesh Application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2021 STMicroelectronics.
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

#include "lp_timer.h"

/* Exported variables --------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
static void LpTimerCb(void);

/* Private typedef -----------------------------------------------------------*/
typedef struct
{
  uint32_t LpTimeLeftOnEntry;
  uint8_t LpTimer_Id;
} LpTimerContext_t;

/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static LpTimerContext_t LpTimerContext;

/* Functions Definition ------------------------------------------------------*/

/**
 * @brief Initialize the low power timer
 *
 * @param  None
 * @retval None
 */
void LpTimerInit(void)
{
  (void) HW_TS_Create(CFG_TIM_PROC_ID_ISR, &(LpTimerContext.LpTimer_Id), hw_ts_SingleShot, LpTimerCb);

  return;
}

/**
 * @brief  Request to start a low power timer ( running is stop mode )
 *
 * @param  time_to_sleep : in ms
 * @retval None
 */
void LpTimerStart(uint32_t time_to_sleep)
{
  /* Converts the number of ms into hw timer tick */
  if(time_to_sleep > 0x400000)
  {
    time_to_sleep = time_to_sleep / (CFG_TS_TICK_VAL);
    time_to_sleep *= 1000;
  }
  else
  {
    time_to_sleep *= 1000;
    time_to_sleep = time_to_sleep / (CFG_TS_TICK_VAL);
  }

  HW_TS_Start(LpTimerContext.LpTimer_Id, time_to_sleep);

  /**
   * There might be other timers already running in the timer server that may elapse
   * before this one.
   * Store how long before the next event so that on wakeup, it will be possible to calculate
   * how long the tick has been suppressed
   */
  LpTimerContext.LpTimeLeftOnEntry = HW_TS_RTC_ReadLeftTicksToCount();

  return;
}

/**
 * @brief  Read how long the timer has run
 *
 * @param  None
 * @retval The time elapsed in ms
 */
uint32_t LpGetElapsedTime(void)
{
  uint32_t return_value;

  return_value = (CFG_TS_TICK_VAL) * (uint32_t)(LpTimerContext.LpTimeLeftOnEntry - HW_TS_RTC_ReadLeftTicksToCount());
  return_value = return_value / 1000;

  /**
   * The system may have been out from another reason than the timer
   * Stop the timer after the elapsed time is calculated other wise, HW_TS_RTC_ReadLeftTicksToCount()
   * may return 0xFFFF ( TIMER LIST EMPTY )
   * It does not hurt stopping a timer that exists but is not running.
   */
  HW_TS_Stop(LpTimerContext.LpTimer_Id);

  return return_value;
}


/*************************************************************
 *
 * LOCAL FUNCTIONS
 *
 *************************************************************/
/**
 * @brief Low power timer callback
 *
 * @param  None
 * @retval None
 */
static void LpTimerCb( void )
{
  /**
   * Nothing to be done
   */

  return;
}

