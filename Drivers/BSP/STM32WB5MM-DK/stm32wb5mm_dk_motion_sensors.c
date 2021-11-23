/**
  ******************************************************************************
  * @file    stm32wb5mm_dk_motion_sensors.c
  * @author  MCD Application Team
  * @brief   This file provides a set of functions needed to manage the 
  *          motion sensors mounted on the STM32WB5MM-DK board (MB1292).
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

/* Includes ------------------------------------------------------------------*/
#include "stm32wb5mm_dk_motion_sensors.h"
#include "stm32wb5mm_dk_bus.h"

/** @addtogroup BSP BSP
 * @{
 */

/** @addtogroup STM32WB5MM_DK STM32WB5MM-DK
 * @{
 */

/** @defgroup STM32WB5MM_DK_MOTION_SENSORS STM32WB5MM-DK MOTION SENSORS
 * @{
 */

/** @defgroup STM32WB5MM_DK_MOTION_SENSOR_Exported_Variables STM32WB5MM-DK MOTION SENSORS Exported Variables
 * @{
 */
extern void
*MotionCompObj[MOTION_SENSOR_INSTANCES_NBR]; /* This "redundant" line is here to fulfil MISRA C-2012 rule 8.4 */
void *MotionCompObj[MOTION_SENSOR_INSTANCES_NBR];

/**
 * @}
 */

/** @defgroup STM32WB5MM_DK_MOTION_SENSOR_Private_Variables STM32WB5MM-DK MOTION SENSORS Private Variables
 * @{
 */

/* We define a jump table in order to get the correct index from the desired function. */
/* This table should have a size equal to the maximum value of a function plus 1.      */
static uint32_t FunctionIndex[5] = {0, 0, 1, 1, 2};
static MOTION_SENSOR_FuncDrv_t *MotionFuncDrv[MOTION_SENSOR_INSTANCES_NBR][MOTION_SENSOR_FUNCTIONS_NBR];
static MOTION_SENSOR_CommonDrv_t *MotionDrv[MOTION_SENSOR_INSTANCES_NBR];
static MOTION_SENSOR_Ctx_t MotionCtx[MOTION_SENSOR_INSTANCES_NBR];

/**
 * @}
 */

/** @defgroup STM32WB5MM_DK_MOTION_SENSOR_Private_Function_Prototypes STM32WB5MM-DK MOTION SENSORS Private Function Prototypes
 * @{
 */

#if (USE_MOTION_SENSOR_ISM330DHCX_0 == 1)
static int32_t ISM330DHCX_0_Probe(uint32_t Functions);
#endif

/**
 * @}
 */

/** @defgroup STM32WB5MM_DK_MOTION_SENSOR_Exported_Functions STM32WB5MM-DK MOTION SENSORS Exported Functions
 * @{
 */

/**
 * @brief  Initializes the motion sensors
 * @param  Instance Motion sensor instance
 * @param  Functions Motion sensor functions. Could be :
 *         - MOTION_GYRO and/or MOTION_ACCELERO for MOTION_SENSOR_ISM330DHCX_0
 *         - MOTION_MAGNETO for MOTION_SENSOR_IIS2MDC_0
 * @retval BSP status
 */
int32_t BSP_MOTION_SENSOR_Init(uint32_t Instance, uint32_t Functions)
{
  int32_t ret = BSP_ERROR_NONE;
  uint32_t function = MOTION_GYRO;
  uint32_t i;
  uint32_t component_functions = 0;
  MOTION_SENSOR_Capabilities_t cap;

  switch (Instance)
  {
#if (USE_MOTION_SENSOR_ISM330DHCX_0 == 1)
    case MOTION_SENSOR_ISM330DHCX_0:
      if (ISM330DHCX_0_Probe(Functions) != BSP_ERROR_NONE)
      {
        return BSP_ERROR_NO_INIT;
      }
      if (MotionDrv[Instance]->GetCapabilities(MotionCompObj[Instance], (void *)&cap) != BSP_ERROR_NONE)
      {
        return BSP_ERROR_UNKNOWN_COMPONENT;
      }
      if (cap.Acc == 1U)
      {
        component_functions |= MOTION_ACCELERO;
      }
      if (cap.Gyro == 1U)
      {
        component_functions |= MOTION_GYRO;
      }
      if (cap.Magneto == 1U)
      {
        component_functions |= MOTION_MAGNETO;
      }
      break;
#endif

    default:
      ret = BSP_ERROR_WRONG_PARAM;
      break;
  }

  if (ret != BSP_ERROR_NONE)
  {
    return ret;
  }

  for (i = 0; i < MOTION_SENSOR_FUNCTIONS_NBR; i++)
  {
    if (((Functions & function) == function) && ((component_functions & function) == function))
    {
      if (MotionFuncDrv[Instance][FunctionIndex[function]]->Enable(MotionCompObj[Instance]) != BSP_ERROR_NONE)
      {
        return BSP_ERROR_COMPONENT_FAILURE;
      }
    }
    function = function << 1;
  }

  return ret;
}

/**
 * @brief  Deinitialize Motion sensor
 * @param  Instance Motion sensor instance
 * @retval BSP status
 */
int32_t BSP_MOTION_SENSOR_DeInit(uint32_t Instance)
{
  int32_t ret;

  if (Instance >= MOTION_SENSOR_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else if (MotionDrv[Instance]->DeInit(MotionCompObj[Instance]) != BSP_ERROR_NONE)
  {
    ret = BSP_ERROR_COMPONENT_FAILURE;
  }
  else
  {
    ret = BSP_ERROR_NONE;
  }

  return ret;
}

/**
 * @brief  Get motion sensor instance capabilities
 * @param  Instance Motion sensor instance
 * @param  Capabilities pointer to motion sensor capabilities
 * @retval BSP status
 */
int32_t BSP_MOTION_SENSOR_GetCapabilities(uint32_t Instance, MOTION_SENSOR_Capabilities_t *Capabilities)
{
  int32_t ret;

  if (Instance >= MOTION_SENSOR_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else if (MotionDrv[Instance]->GetCapabilities(MotionCompObj[Instance], Capabilities) != BSP_ERROR_NONE)
  {
    ret = BSP_ERROR_UNKNOWN_COMPONENT;
  }
  else
  {
    ret = BSP_ERROR_NONE;
  }

  return ret;
}

/**
 * @brief  Get WHOAMI value
 * @param  Instance Motion sensor instance
 * @param  Id WHOAMI value
 * @retval BSP status
 */
int32_t BSP_MOTION_SENSOR_ReadID(uint32_t Instance, uint8_t *Id)
{
  int32_t ret;

  if (Instance >= MOTION_SENSOR_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else if (MotionDrv[Instance]->ReadID(MotionCompObj[Instance], Id) != BSP_ERROR_NONE)
  {
    ret = BSP_ERROR_UNKNOWN_COMPONENT;
  }
  else
  {
    ret = BSP_ERROR_NONE;
  }

  return ret;
}

/**
 * @brief  Enable Motion sensor
 * @param  Instance Motion sensor instance
 * @param  Function Motion sensor function. Could be :
 *         - MOTION_MAGNETO for instance 0
 *         - MOTION_GYRO and/or MOTION_ACCELERO for instance 1
 * @retval BSP status
 */
int32_t BSP_MOTION_SENSOR_Enable(uint32_t Instance, uint32_t Function)
{
  int32_t ret;

  if (Instance >= MOTION_SENSOR_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    if ((MotionCtx[Instance].Functions & Function) == Function)
    {
      if (MotionFuncDrv[Instance][FunctionIndex[Function]]->Enable(MotionCompObj[Instance]) != BSP_ERROR_NONE)
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
      ret = BSP_ERROR_WRONG_PARAM;
    }
  }

  return ret;
}

/**
 * @brief  Disable Motion sensor
 * @param  Instance Motion sensor instance
 * @param  Function Motion sensor function. Could be :
 *         - MOTION_MAGNETO for instance 0
 *         - MOTION_GYRO and/or MOTION_ACCELERO for instance 1
 * @retval BSP status
 */
int32_t BSP_MOTION_SENSOR_Disable(uint32_t Instance, uint32_t Function)
{
  int32_t ret;

  if (Instance >= MOTION_SENSOR_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    if ((MotionCtx[Instance].Functions & Function) == Function)
    {
      if (MotionFuncDrv[Instance][FunctionIndex[Function]]->Disable(MotionCompObj[Instance]) != BSP_ERROR_NONE)
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
      ret = BSP_ERROR_WRONG_PARAM;
    }
  }

  return ret;
}

/**
 * @brief  Get motion sensor axes data
 * @param  Instance Motion sensor instance
 * @param  Function Motion sensor function. Could be :
 *         - MOTION_MAGNETO for instance 0
 *         - MOTION_GYRO and/or MOTION_ACCELERO for instance 1
 * @param  Axes pointer to axes data structure
 * @retval BSP status
 */
int32_t BSP_MOTION_SENSOR_GetAxes(uint32_t Instance, uint32_t Function, MOTION_SENSOR_Axes_t *Axes)
{
  int32_t ret;

  if (Instance >= MOTION_SENSOR_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    if ((MotionCtx[Instance].Functions & Function) == Function)
    {
      if (MotionFuncDrv[Instance][FunctionIndex[Function]]->GetAxes(MotionCompObj[Instance], Axes) != BSP_ERROR_NONE)
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
      ret = BSP_ERROR_WRONG_PARAM;
    }
  }

  return ret;
}

/**
 * @brief  Get motion sensor axes raw data
 * @param  Instance Motion sensor instance
 * @param  Function Motion sensor function. Could be :
 *         - MOTION_MAGNETO for instance 0
 *         - MOTION_GYRO and/or MOTION_ACCELERO for instance 1
 * @param  Axes pointer to axes raw data structure
 * @retval BSP status
 */
int32_t BSP_MOTION_SENSOR_GetAxesRaw(uint32_t Instance, uint32_t Function, MOTION_SENSOR_AxesRaw_t *Axes)
{
  int32_t ret;

  if (Instance >= MOTION_SENSOR_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    if ((MotionCtx[Instance].Functions & Function) == Function)
    {
      if (MotionFuncDrv[Instance][FunctionIndex[Function]]->GetAxesRaw(MotionCompObj[Instance], Axes) != BSP_ERROR_NONE)
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
      ret = BSP_ERROR_WRONG_PARAM;
    }
  }

  return ret;
}

/**
 * @brief  Get motion sensor sensitivity
 * @param  Instance Motion sensor instance
 * @param  Function Motion sensor function. Could be :
 *         - MOTION_MAGNETO for instance 0
 *         - MOTION_GYRO and/or MOTION_ACCELERO for instance 1
 * @param  Sensitivity pointer to sensitivity read value
 * @retval BSP status
 */
int32_t BSP_MOTION_SENSOR_GetSensitivity(uint32_t Instance, uint32_t Function, float *Sensitivity)
{
  int32_t ret;

  if (Instance >= MOTION_SENSOR_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    if ((MotionCtx[Instance].Functions & Function) == Function)
    {
      if (MotionFuncDrv[Instance][FunctionIndex[Function]]->GetSensitivity(MotionCompObj[Instance],
          Sensitivity) != BSP_ERROR_NONE)
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
      ret = BSP_ERROR_WRONG_PARAM;
    }
  }

  return ret;
}

/**
 * @brief  Get motion sensor Output Data Rate
 * @param  Instance Motion sensor instance
 * @param  Function Motion sensor function. Could be :
 *         - MOTION_MAGNETO for instance 0
 *         - MOTION_GYRO and/or MOTION_ACCELERO for instance 1
 * @param  Odr pointer to Output Data Rate read value
 * @retval BSP status
 */
int32_t BSP_MOTION_SENSOR_GetOutputDataRate(uint32_t Instance, uint32_t Function, float *Odr)
{
  int32_t ret;

  if (Instance >= MOTION_SENSOR_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    if ((MotionCtx[Instance].Functions & Function) == Function)
    {
      if (MotionFuncDrv[Instance][FunctionIndex[Function]]->GetOutputDataRate(MotionCompObj[Instance], Odr) != BSP_ERROR_NONE)
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
      ret = BSP_ERROR_WRONG_PARAM;
    }
  }

  return ret;
}

/**
 * @brief  Get motion sensor Full Scale
 * @param  Instance Motion sensor instance
 * @param  Function Motion sensor function. Could be :
 *         - MOTION_MAGNETO for instance 0
 *         - MOTION_GYRO and/or MOTION_ACCELERO for instance 1
 * @param  Fullscale pointer to Fullscale read value
 * @retval BSP status
 */
int32_t BSP_MOTION_SENSOR_GetFullScale(uint32_t Instance, uint32_t Function, int32_t *Fullscale)
{
  int32_t ret;

  if (Instance >= MOTION_SENSOR_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    if ((MotionCtx[Instance].Functions & Function) == Function)
    {
      if (MotionFuncDrv[Instance][FunctionIndex[Function]]->GetFullScale(MotionCompObj[Instance],
          Fullscale) != BSP_ERROR_NONE)
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
      ret = BSP_ERROR_WRONG_PARAM;
    }
  }

  return ret;
}

/**
 * @brief  Set motion sensor Output Data Rate
 * @param  Instance Motion sensor instance
 * @param  Function Motion sensor function. Could be :
 *         - MOTION_MAGNETO for instance 0
 *         - MOTION_GYRO and/or MOTION_ACCELERO for instance 1
 * @param  Odr Output Data Rate value to be set
 * @retval BSP status
 */
int32_t BSP_MOTION_SENSOR_SetOutputDataRate(uint32_t Instance, uint32_t Function, float Odr)
{
  int32_t ret;

  if (Instance >= MOTION_SENSOR_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    if ((MotionCtx[Instance].Functions & Function) == Function)
    {
      if (MotionFuncDrv[Instance][FunctionIndex[Function]]->SetOutputDataRate(MotionCompObj[Instance], Odr) != BSP_ERROR_NONE)
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
      ret = BSP_ERROR_WRONG_PARAM;
    }
  }

  return ret;
}

/**
 * @brief  Set motion sensor Full Scale
 * @param  Instance Motion sensor instance
 * @param  Function Motion sensor function. Could be :
 *         - MOTION_MAGNETO for instance 0
 *         - MOTION_GYRO and/or MOTION_ACCELERO for instance 1
 * @param  Fullscale Fullscale value to be set
 * @retval BSP status
 */
int32_t BSP_MOTION_SENSOR_SetFullScale(uint32_t Instance, uint32_t Function, int32_t Fullscale)
{
  int32_t ret;

  if (Instance >= MOTION_SENSOR_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    if ((MotionCtx[Instance].Functions & Function) == Function)
    {
      if (MotionFuncDrv[Instance][FunctionIndex[Function]]->SetFullScale(MotionCompObj[Instance],
          Fullscale) != BSP_ERROR_NONE)
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
      ret = BSP_ERROR_WRONG_PARAM;
    }
  }

  return ret;
}

/**
 * @}
 */

/** @defgroup STM32WB5MM_DK_MOTION_SENSOR_Private_Functions STM32WB5MM-DK MOTION SENSORS Private Functions
 * @{
 */

#if (USE_MOTION_SENSOR_ISM330DHCX_0  == 1)
/**
 * @brief  Register Bus IOs for instance 0 if component ID is OK
 * @retval BSP status
 */
static int32_t ISM330DHCX_0_Probe(uint32_t Functions)
{
  ISM330DHCX_IO_t            io_ctx;
  uint8_t                   id;
  static ISM330DHCX_Object_t ism330dhcx_obj_0;
  ISM330DHCX_Capabilities_t  cap;
  int32_t ret = BSP_ERROR_NONE;

  /* Configure the accelero driver */
  io_ctx.BusType     = ISM330DHCX_I2C_BUS; /* I2C */
  io_ctx.Address     = ISM330DHCX_I2C_ADD_H;
  io_ctx.Init        = STM32WB5MM_DK_I2C_Init;
  io_ctx.DeInit      = STM32WB5MM_DK_I2C_DeInit;
  io_ctx.ReadReg     = STM32WB5MM_DK_I2C_ReadReg;
  io_ctx.WriteReg    = STM32WB5MM_DK_I2C_WriteReg;
  io_ctx.GetTick     = STM32WB5MM_DK_GetTick;

  if (ISM330DHCX_RegisterBusIO(&ism330dhcx_obj_0, &io_ctx) != ISM330DHCX_OK)
  {
    ret = BSP_ERROR_UNKNOWN_COMPONENT;
  }
  else if (ISM330DHCX_ReadID(&ism330dhcx_obj_0, &id) != ISM330DHCX_OK)
  {
    ret = BSP_ERROR_UNKNOWN_COMPONENT;
  }
  else if (id != ISM330DHCX_ID)
  {
    ret = BSP_ERROR_UNKNOWN_COMPONENT;
  }
  else
  {
    (void)ISM330DHCX_GetCapabilities(&ism330dhcx_obj_0, &cap);
    MotionCtx[MOTION_SENSOR_ISM330DHCX_0].Functions = ((uint32_t)cap.Gyro) | ((uint32_t)cap.Acc << 1) | ((uint32_t)cap.Magneto << 2);

    MotionCompObj[MOTION_SENSOR_ISM330DHCX_0] = &ism330dhcx_obj_0;
    /* The second cast (void *) is added to bypass Misra R11.3 rule */
    MotionDrv[MOTION_SENSOR_ISM330DHCX_0] = (MOTION_SENSOR_CommonDrv_t *)(void *)&ISM330DHCX_COMMON_Driver;

    if ((ret == BSP_ERROR_NONE) && ((Functions & MOTION_GYRO) == MOTION_GYRO) && (cap.Gyro == 1U))
    {
      /* The second cast (void *) is added to bypass Misra R11.3 rule */
      MotionFuncDrv[MOTION_SENSOR_ISM330DHCX_0][FunctionIndex[MOTION_GYRO]] = (MOTION_SENSOR_FuncDrv_t *)(void *)&ISM330DHCX_GYRO_Driver;

      if (MotionDrv[MOTION_SENSOR_ISM330DHCX_0]->Init(MotionCompObj[MOTION_SENSOR_ISM330DHCX_0]) != ISM330DHCX_OK)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      else
      {
        ret = BSP_ERROR_NONE;
      }
    }
    if ((ret == BSP_ERROR_NONE) && ((Functions & MOTION_ACCELERO) == MOTION_ACCELERO) && (cap.Acc == 1U))
    {
      /* The second cast (void *) is added to bypass Misra R11.3 rule */
      MotionFuncDrv[MOTION_SENSOR_ISM330DHCX_0][FunctionIndex[MOTION_ACCELERO]] = (MOTION_SENSOR_FuncDrv_t *)(
            void *)&ISM330DHCX_ACC_Driver;

      if (MotionDrv[MOTION_SENSOR_ISM330DHCX_0]->Init(MotionCompObj[MOTION_SENSOR_ISM330DHCX_0]) != ISM330DHCX_OK)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      else
      {
        ret = BSP_ERROR_NONE;
      }
    }
    if ((ret == BSP_ERROR_NONE) && ((Functions & MOTION_MAGNETO) == MOTION_MAGNETO))
    {
      /* Return an error if the application try to initialize a function not supported by the component */
      ret = BSP_ERROR_COMPONENT_FAILURE;
    }
  }
  return ret;
}
#endif

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
