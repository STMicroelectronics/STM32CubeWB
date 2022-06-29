/**
  ******************************************************************************
  * @file    b_wb1m_wpan1_motion_sensors_ex.c
  * @author  MEMS Software Solutions Team
  * @brief   This file provides a set of functions needed to manage the
  *          motion sensors mounted on the B-WB1M-WPAN1 board (MB1868).
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

/* Includes ------------------------------------------------------------------*/
#include "b_wb1m_wpan1_motion_sensors_ex.h"

/** @addtogroup BSP BSP
  * @{
  */

/** @addtogroup B_WB1M_WPAN1 B-WB1M-WPAN1
  * @{
  */

/** @defgroup B_WB1M_WPAN1_MOTION_SENSOR_EX B-WB1M-WPAN1 MOTION SENSOR EX
  * @{
  */

/** @defgroup B_WB1M_WPAN1_MOTION_SENSOR_EX_Imported_Variables B-WB1M-WPAN1 MOTION SENSOR EX Imported Variables
  * @{
  */

extern void *MotionCompObj[MOTION_SENSOR_INSTANCES_NBR];

/**
  * @}
  */

/** @defgroup B_WB1M_WPAN1_MOTION_SENSOR_EX_Exported_Functions B-WB1M-WPAN1 MOTION SENSOR EX Exported Functions
  * @{
  */

/**
  * @brief  Get the status of data ready bit
  * @param  Instance the device instance
  * @param  Function Motion sensor function. Could be:
  *         - MOTION_GYRO and/or MOTION_ACCELERO for MOTION_SENSOR_ISM330DHCX_0 (Instance 0)
  * @param  Status the pointer to the status
  * @retval BSP status
  */
int32_t BSP_MOTION_SENSOR_Get_DRDY_Status(uint32_t Instance, uint32_t Function, uint8_t *Status)
{
  int32_t ret;

  switch (Instance)
  {
#if (USE_MOTION_SENSOR_ISM330DHCX_0 == 1)
    case MOTION_SENSOR_ISM330DHCX_0:
      if ((Function & MOTION_ACCELERO) == MOTION_ACCELERO)
      {
        if (ISM330DHCX_ACC_Get_DRDY_Status(MotionCompObj[Instance], Status) != BSP_ERROR_NONE)
        {
          ret = BSP_ERROR_COMPONENT_FAILURE;
        }
        else
        {
          ret = BSP_ERROR_NONE;
        }
      }
      else if ((Function & MOTION_GYRO) == MOTION_GYRO)
      {
        if (ISM330DHCX_GYRO_Get_DRDY_Status(MotionCompObj[Instance], Status) != BSP_ERROR_NONE)
        {
          ret = BSP_ERROR_COMPONENT_FAILURE;
        }
        else
        {
          ret = BSP_ERROR_NONE;
        }
      }
      else
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      break;
#endif /* (USE_MOTION_SENSOR_ISM330DHCX_0 == 1) */

    default:
      ret = BSP_ERROR_WRONG_PARAM;
      break;
  }

  return ret;
}

/**
  * @brief  Get the register value
  * @param  Instance the device instance
  * @param  Reg address to be read
  * @param  Data pointer where the value is written to
  * @retval BSP status
  */
int32_t BSP_MOTION_SENSOR_Read_Register(uint32_t Instance, uint8_t Reg, uint8_t *Data)
{
  int32_t ret;

  switch (Instance)
  {
#if (USE_MOTION_SENSOR_ISM330DHCX_0 == 1)
    case MOTION_SENSOR_ISM330DHCX_0:
      if (ISM330DHCX_Read_Reg(MotionCompObj[Instance], Reg, Data) != BSP_ERROR_NONE)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      else
      {
        ret = BSP_ERROR_NONE;
      }
      break;
#endif /* (USE_MOTION_SENSOR_ISM330DHCX_0 == 1) */

    default:
      ret = BSP_ERROR_WRONG_PARAM;
      break;
  }

  return ret;
}

/**
  * @brief  Set the register value
  * @param  Instance the device instance
  * @param  Reg address to be read
  * @param  Data value to be written
  * @retval BSP status
  */
int32_t BSP_MOTION_SENSOR_Write_Register(uint32_t Instance, uint8_t Reg, uint8_t Data)
{
  int32_t ret;

  switch (Instance)
  {
#if (USE_MOTION_SENSOR_ISM330DHCX_0 == 1)
    case MOTION_SENSOR_ISM330DHCX_0:
      if (ISM330DHCX_Write_Reg(MotionCompObj[Instance], Reg, Data) != BSP_ERROR_NONE)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      else
      {
        ret = BSP_ERROR_NONE;
      }
      break;
#endif /* (USE_MOTION_SENSOR_ISM330DHCX_0 == 1) */

    default:
      ret = BSP_ERROR_WRONG_PARAM;
      break;
  }

  return ret;
}

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
