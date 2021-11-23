/**
  ******************************************************************************
  * @file    nucleo_wb15cc_conf.h
  * @author  MCD Application Team
  * @brief   NUCLEO-WB15CC board configuration file.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef NUCLEO_WB15CC_CONF_H
#define NUCLEO_WB15CC_CONF_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32wbxx_hal.h"

/** @addtogroup BSP
  * @{
  */

/** @addtogroup NUCLEO_WB15CC
  * @{
  */

/** @defgroup NUCLEO_WB15CC_CONFIG CONFIG
  * @{
  */

/** @defgroup NUCLEO_WB15CC_CONFIG_Exported_Constants Exported Constants
  * @{
  */
/* COM usage define */
#define USE_BSP_COM_FEATURE                 0U

/* COM log define */
#define USE_COM_LOG                         0U

/* IRQ priorities */
#define BSP_BUTTON_USER_IT_PRIORITY         15U

/**
  * @}
  */

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

#endif /* NUCLEO_WB15CC_CONF_H */
