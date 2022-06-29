/* USER CODE BEGIN Header */
/**
  ******************************************************************************
* @file    lp_timer.h
  * @author  MCD Application Team
* @brief   Header for lp_timer.c module
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef LP_TIMER_H
#define LP_TIMER_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/

  /**
   * @brief Initialize the low power timer
   *
   * @param  None
   * @retval None
   */
  void LpTimerInit(void);

  /**
   * @brief  Request to start a low power timer ( running is stop mode )
   *
   * @param  time_to_sleep : in ms
   * @retval None
   */
  void LpTimerStart(uint32_t time_to_sleep);

  /**
   * @brief  Read how long the timer has run
   *
   * @param  None
   * @retval The time elapsed in ms
   */
  uint32_t LpGetElapsedTime(void);


#ifdef __cplusplus
}
#endif

#endif /* LP_TIMER_H */
