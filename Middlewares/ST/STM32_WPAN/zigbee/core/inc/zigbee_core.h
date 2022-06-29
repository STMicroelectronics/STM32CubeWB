/**
  ******************************************************************************
  * @file    zigbee_core.h
  * @author  MCD Application Team
  * @brief   This file contains the mandatory include files requested by the
  *          middleware for Zigbee and the error code level.
  *
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

#ifndef _ZIGBEE_CORE_H_
#define _ZIGBEE_CORE_H_

#include <stdint.h>

#include "stm32wbxx_hal.h"
#include "stm32wbxx_hal_def.h"
#include "stm32wbxx_hal_cortex.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Gravity error level */
typedef enum {
    ERR_INTERFACE_FATAL = 1U,
    ERR_INTERFACE_WARNING = 2U
} Error_Interface_Level_Enum_t;

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* _ZIGBEE_CORE_H_ */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
