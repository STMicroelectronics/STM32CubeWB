/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    tm.h
  * @author  MCD Application Team
  * @brief   Transparent mode interface
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
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef TM_H
#define TM_H

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

#endif /*TM_H */
