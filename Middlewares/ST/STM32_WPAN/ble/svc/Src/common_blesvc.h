
/**
  ******************************************************************************
  * @file    common_blesvc.h
  * @author  MCD Application Team
  * @brief   Header for ble modules
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


/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __COMMON_BLESVC_H
#define __COMMON_BLESVC_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "ble_common.h"
#include "ble.h"
#include "dbg_trace.h"



/* Exported types ------------------------------------------------------------*/
typedef enum
{
  COMSVC_Notification = ( 1 << 0 ),
  COMSVC_Indication =   ( 1 << 1 ),
} COMSVC_ClientCharConfMask_t;

/* Exported constants --------------------------------------------------------*/
/* External variables --------------------------------------------------------*/
/* Exported macros -----------------------------------------------------------*/


#ifdef __cplusplus
}
#endif

#endif /*__COMMON_BLESVC_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
