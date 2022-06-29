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


/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef TM_H
#define TM_H

#ifdef __cplusplus
extern "C" {
#endif

  /* Includes ------------------------------------------------------------------*/
#include "tl.h"

  /* Exported types ------------------------------------------------------------*/
  /* Exported constants --------------------------------------------------------*/
  /* External variables --------------------------------------------------------*/
  /* Exported macros -----------------------------------------------------------*/
  /* Exported functions ------------------------------------------------------- */
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
  void TM_SysCmdRspCb (TL_EvtPacket_t * p_cmd_resp);

  /**
   * @brief  Call back to receive system user user
   *
   * @param  None
   * @retval None
   */
  void TM_SysUserEvtRxCb (TL_EvtPacket_t * p_evt_rx);

#ifdef __cplusplus
}
#endif

#endif /* TM_H */
