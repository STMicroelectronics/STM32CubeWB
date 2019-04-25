/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    tm.h
  * @author  MCD Application Team
  * @brief   Transparent mode interface
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
/* USER CODE END Header */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __TM_H
#define __TM_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "tl.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* External variables --------------------------------------------------------*/
/* USER CODE BEGIN EV */

/* USER CODE END EV */

/* Exported macros ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions ---------------------------------------------*/
  /**
   * @brief  Transparent mode initialization
   *
   * @param  None
   * @retval None
   */
  void TM_Init( void );
  /**
   * @brief  Call back to receive system command response
   *
   * @param  None
   * @retval None
   */
  void TM_SysCmdRspCb(TL_EvtPacket_t * p_cmd_resp);

  /**
   * @brief  Call back to receive system user user
   *
   * @param  None
   * @retval None
   */
  void TM_SysUserEvtRxCb(TL_EvtPacket_t * p_evt_rx);

/* USER CODE BEGIN EFP */
  /**
   * @brief  Set the low power mode
   *
   * @param  None
   * @retval None
   */
  void TM_SetLowPowerMode( void );
/* USER CODE END EFP */

#ifdef __cplusplus
}
#endif

#endif /*__TM_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
