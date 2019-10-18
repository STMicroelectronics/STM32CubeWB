/**
  ******************************************************************************
  * @file    802_15_4_mac_core.h
  * @author  MCD Application Team
  * @brief   This file contains all the defines and structures used for the
  *          communication between the two core M0 and M4.
  *          This file is shared between the code running on M4 and the code
  *          running on M0.
  *
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
