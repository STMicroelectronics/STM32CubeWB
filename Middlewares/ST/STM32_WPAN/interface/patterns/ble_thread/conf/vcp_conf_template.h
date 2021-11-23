/**
 ******************************************************************************
 * @file    vcp_conf.h
 * @author  MCD Application Team
 * @brief   Configuration of the vcp interface
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2018-2021 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */


/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __VCP_CONF_H
#define __VCP_CONF_H

#ifdef __cplusplus
extern "C"
{
#endif

/* Includes ------------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
#define VCP_BAUD_RATE                             (115200)
#define VCP_TX_PATH_INTERFACE_READY_SETUP_TIME    (20*1000*1000/CFG_TS_TICK_VAL)  /** 20s   */
#define VCP_TASK_ID                               (CFG_TASK_VCP_SEND_DATA_ID)
#define VCP_TASK_PRIO                             (CFG_SCH_PRIO_1)

#ifdef  VCP_TX_PATH_INTERFACE_READY_SETUP_TIME
#define VCP_TIMER_PROC_ID                         (CFG_TIM_PROC_ID_ISR)
#endif


/* External variables --------------------------------------------------------*/
/* Exported macros -----------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */


#ifdef __cplusplus
}
#endif

#endif /*__VCP_CONF_H */

