/**
  ******************************************************************************
  * @file    s25fl128s_conf_template.h
  * @author  MCD Application Team
  * @brief   This file contains the configurations of the S25FL128S QSPI memory.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
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
#ifndef S25FL128S_CONF_H
#define S25FL128S_CONF_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32xxxx_hal.h"

/** @addtogroup BSP
  * @{
  */

#define CONF_S25FL128S_READ_ENHANCE           0          /* MMP performance enhance read enable/disable */
#define CONF_QSPI_DUMMY_CLOCK                 8U

/* Dummy cycles for STR read mode */
#define S25FL128S_DUMMY_CYCLES_READ_QUAD      8U
#define S25FL128S_DUMMY_CYCLES_READ           8U

#ifdef __cplusplus
}
#endif

#endif /* S25FL128S_CONF_H */

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
