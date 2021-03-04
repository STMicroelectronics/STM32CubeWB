/**
  ******************************************************************************
  * @file    nucleo_wb15cc_conf.h
  * @author  MCD Application Team
  * @brief   NUCLEO-WB15CC board configuration file.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
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
#define BSP_BUTTON_SWx_IT_PRIORITY         15U

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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
