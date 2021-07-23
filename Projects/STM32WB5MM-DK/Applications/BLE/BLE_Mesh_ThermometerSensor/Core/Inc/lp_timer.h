/**
******************************************************************************
* @file    lp_timer.h
* @brief   Header for lp_timer.c module
******************************************************************************
* @attention
*
* <h2><center>&copy; Copyright (c) 2019 STMicroelectronics. 
* All rights reserved.</center></h2>
*
* This software component is licensed by ST under BSD 3-Clause license,
* the "License"; You may not use this file except in compliance with the 
* License. You may obtain a copy of the License at:
*                        opensource.org/licenses/BSD-3-Clause
*
******************************************************************************
*/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __LP_TIMER_H
#define __LP_TIMER_H

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

#endif /*__LP_TIMER_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
