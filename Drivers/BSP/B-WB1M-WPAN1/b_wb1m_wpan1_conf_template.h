/**
  ******************************************************************************
  * @file    b_wb1m_wpan1_conf_template.h
  * @author  MCD Application Team
  * @brief   configuration file.
  *          This file should be copied to the application folder and renamed
  *          to b_wb1m_wpan1_conf.h
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef B_WB1M_WPAN1_CONF_H
#define B_WB1M_WPAN1_CONF_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Includes ------------------------------------------------------------------*/
#include "stm32wbxx_hal.h"

/* Environmental Sensors usage */
#define USE_ENV_SENSOR_STTS22H_0             1U

/* Motion Sensors usage */
#define USE_MOTION_SENSOR_ISM330DHCX_0       1U

/* COM port usage */
#if defined(USE_VCP_CONNECTION)
#define USE_BSP_COM_FEATURE                  1U
#define USE_COM_LOG                          1U
#else
#define USE_BSP_COM_FEATURE                  0U
#define USE_COM_LOG                          0U
#endif /* USE_VCP_CONNECTION */

/* IRQ priorities */
#define BSP_BUTTON_USERx_IT_PRIORITY         0x0FUL

/* I2C1 Frequency in Hz  */
#define BUS_I2C1_FREQUENCY                   100000UL /* Frequency of I2C1 = 100 KHz*/


#define B_WB1M_WPAN1_I2C_Init BSP_I2C1_Init
#define B_WB1M_WPAN1_I2C_DeInit BSP_I2C1_DeInit
#define B_WB1M_WPAN1_I2C_ReadReg BSP_I2C1_ReadReg
#define B_WB1M_WPAN1_I2C_WriteReg BSP_I2C1_WriteReg

#define B_WB1M_WPAN1_GetTick BSP_GetTick

/* Number of millisecond of audio at each DMA interrupt */
#define N_MS_PER_INTERRUPT               (20U)

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* B_WB1M_WPAN1_CONF_H */
