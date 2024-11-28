/**
  ******************************************************************************
  * @file    stm32wbxx_nucleo_conf.h
  * @author  MCD Application Team
  * @brief   Discovery Module configuration file.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */


/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM32WBXX_NUCLEO_CONF_H
#define __STM32WBXX_NUCLEO_CONF_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32wbxx_hal.h"

/** @addtogroup BSP
  * @{
  */

/** @addtogroup STM32WBXX_NUCLEO
  * @{
  */

/** @defgroup STM32WBXX_NUCLEO_CONFIG CONFIG
  * @{
  */

/** @defgroup STM32WBXX_NUCLEO_CONFIG_Exported_Constants Exported Constants
  * @{
  */

/* Uncomment the following line to enable VCP */
/* #define USE_VCP_CONNECTION */
#define USE_NUCLEO_64      1U
/* COM port usage */
#if defined(USE_VCP_CONNECTION)
#define USE_BSP_COM_FEATURE                  1U
#define USE_COM_LOG                          1U
#else
#define USE_BSP_COM_FEATURE                  0U
#define USE_COM_LOG                          0U
#endif /* USE_VCP_CONNECTION */

/* Button interrupt priorities */
#define BSP_B1_IT_PRIORITY 0x0FUL  /* Default is lowest priority level */
#define BSP_B2_IT_PRIORITY 0x0FUL  /* Default is lowest priority level */
#define BSP_B3_IT_PRIORITY 0x0FUL  /* Default is lowest priority level */


/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif /* STM32WBXX_NUCLEO_CONF_H */
