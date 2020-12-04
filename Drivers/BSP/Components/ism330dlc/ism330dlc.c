/**
 ******************************************************************************
 * @file    ism330dlc.c
 * @author  MEMS Software Solutions Team
 * @brief   ISM330DLC driver file
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; COPYRIGHT(c) 2018 STMicroelectronics</center></h2>
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *   1. Redistributions of source code must retain the above copyright notice,
 *      this list of conditions and the following disclaimer.
 *   2. Redistributions in binary form must reproduce the above copyright notice,
 *      this list of conditions and the following disclaimer in the documentation
 *      and/or other materials provided with the distribution.
 *   3. Neither the name of STMicroelectronics nor the names of its contributors
 *      may be used to endorse or promote products derived from this software
 *      without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSIBILITY OF SUCH DAMAGE.
 *
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "ism330dlc.h"

/** @addtogroup BSP BSP
 * @{
 */

/** @addtogroup Component Component
 * @{
 */

/** @defgroup ISM330DLC ISM330DLC
 * @{
 */

/** @defgroup ISM330DLC_Exported_Variables ISM330DLC Exported Variables
 * @{
 */

ISM330DLC_CommonDrv_t ISM330DLC_COMMON_Driver =
{
  ISM330DLC_Init,
  ISM330DLC_DeInit,
  ISM330DLC_ReadID,
  ISM330DLC_GetCapabilities,
};

ISM330DLC_ACC_Drv_t ISM330DLC_ACC_Driver =
{
  ISM330DLC_ACC_Enable,
  ISM330DLC_ACC_Disable,
  ISM330DLC_ACC_GetSensitivity,
  ISM330DLC_ACC_GetOutputDataRate,
  ISM330DLC_ACC_SetOutputDataRate,
  ISM330DLC_ACC_GetFullScale,
  ISM330DLC_ACC_SetFullScale,
  ISM330DLC_ACC_GetAxes,
  ISM330DLC_ACC_GetAxesRaw,
};

ISM330DLC_GYRO_Drv_t ISM330DLC_GYRO_Driver =
{
  ISM330DLC_GYRO_Enable,
  ISM330DLC_GYRO_Disable,
  ISM330DLC_GYRO_GetSensitivity,
  ISM330DLC_GYRO_GetOutputDataRate,
  ISM330DLC_GYRO_SetOutputDataRate,
  ISM330DLC_GYRO_GetFullScale,
  ISM330DLC_GYRO_SetFullScale,
  ISM330DLC_GYRO_GetAxes,
  ISM330DLC_GYRO_GetAxesRaw,
};

/**
 * @}
 */

/** @defgroup ISM330DLC_Private_Function_Prototypes ISM330DLC Private Function Prototypes
 * @{
 */

static int32_t ReadRegWrap(void *Handle, uint8_t Reg, uint8_t *pData, uint16_t Length);
static int32_t WriteRegWrap(void *Handle, uint8_t Reg, uint8_t *pData, uint16_t Length);
static int32_t ISM330DLC_ACC_SetOutputDataRate_When_Enabled(ISM330DLC_Object_t *pObj, float Odr);
static int32_t ISM330DLC_ACC_SetOutputDataRate_When_Disabled(ISM330DLC_Object_t *pObj, float Odr);
static int32_t ISM330DLC_GYRO_SetOutputDataRate_When_Enabled(ISM330DLC_Object_t *pObj, float Odr);
static int32_t ISM330DLC_GYRO_SetOutputDataRate_When_Disabled(ISM330DLC_Object_t *pObj, float Odr);

/**
 * @}
 */

/** @defgroup ISM330DLC_Exported_Functions ISM330DLC Exported Functions
 * @{
 */

/**
 * @brief  Register Component Bus IO operations
 * @param  pObj the device pObj
 * @retval 0 in case of success, an error code otherwise
 */
int32_t ISM330DLC_RegisterBusIO(ISM330DLC_Object_t *pObj, ISM330DLC_IO_t *pIO)
{
  int32_t ret = ISM330DLC_OK;

  if (pObj == NULL)
  {
    ret = ISM330DLC_ERROR;
  }
  else
  {
    pObj->IO.Init      = pIO->Init;
    pObj->IO.DeInit    = pIO->DeInit;
    pObj->IO.BusType   = pIO->BusType;
    pObj->IO.Address   = pIO->Address;
    pObj->IO.WriteReg  = pIO->WriteReg;
    pObj->IO.ReadReg   = pIO->ReadReg;
    pObj->IO.GetTick   = pIO->GetTick;

    pObj->Ctx.read_reg  = ReadRegWrap;
    pObj->Ctx.write_reg = WriteRegWrap;
    pObj->Ctx.handle   = pObj;

    if (pObj->IO.Init == NULL)
    {
      ret = ISM330DLC_ERROR;
    }
    else if (pObj->IO.Init() != ISM330DLC_OK)
    {
      ret = ISM330DLC_ERROR;
    }
    else
    {
      if (pObj->IO.BusType == ISM330DLC_SPI_3WIRES_BUS) /* SPI 3-Wires */
      {
        /* Enable the SPI 3-Wires support only the first time */
        if (pObj->is_initialized == 0U)
        {
          /* Enable SPI 3-Wires on the component */
          uint8_t data = 0x0C;

          if (ISM330DLC_Write_Reg(pObj, ISM330DLC_CTRL3_C, data) != ISM330DLC_OK)
          {
            ret = ISM330DLC_ERROR;
          }
        }
      }
    }
  }

  return ret;
}

/**
 * @brief  Initialize the ISM330DLC sensor
 * @param  pObj the device pObj
 * @retval 0 in case of success, an error code otherwise
 */
int32_t ISM330DLC_Init(ISM330DLC_Object_t *pObj)
{
  /* Reset all the configuration registers in order to set correctly */
  if (ism330dlc_reset_set(&(pObj->Ctx),PROPERTY_ENABLE) != ISM330DLC_OK)
  {
    return ISM330DLC_ERROR;
  }

  /* Enable register address automatically incremented during a multiple byte
  access with a serial interface. */
  if (ism330dlc_auto_increment_set(&(pObj->Ctx), PROPERTY_ENABLE) != ISM330DLC_OK)
  {
    return ISM330DLC_ERROR;
  }

  /* Enable BDU */
  if (ism330dlc_block_data_update_set(&(pObj->Ctx), PROPERTY_ENABLE) != ISM330DLC_OK)
  {
    return ISM330DLC_ERROR;
  }

  /* FIFO mode selection */
  if (ism330dlc_fifo_mode_set(&(pObj->Ctx), ISM330DLC_BYPASS_MODE) != ISM330DLC_OK)
  {
    return ISM330DLC_ERROR;
  }

  /* Select default output data rate. */
  pObj->acc_odr = ISM330DLC_XL_ODR_104Hz;

  /* Output data rate selection - power down. */
  if (ism330dlc_xl_data_rate_set(&(pObj->Ctx), ISM330DLC_XL_ODR_OFF) != ISM330DLC_OK)
  {
    return ISM330DLC_ERROR;
  }

  /* Full scale selection. */
  if (ism330dlc_xl_full_scale_set(&(pObj->Ctx), ISM330DLC_2g) != ISM330DLC_OK)
  {
    return ISM330DLC_ERROR;
  }

  /* Select default output data rate. */
  pObj->gyro_odr = ISM330DLC_GY_ODR_104Hz;

  /* Output data rate selection - power down. */
  if (ism330dlc_gy_data_rate_set(&(pObj->Ctx), ISM330DLC_GY_ODR_OFF) != ISM330DLC_OK)
  {
    return ISM330DLC_ERROR;
  }

  /* Full scale selection. */
  if (ism330dlc_gy_full_scale_set(&(pObj->Ctx), ISM330DLC_2000dps) != ISM330DLC_OK)
  {
    return ISM330DLC_ERROR;
  }

  pObj->is_initialized = 1;

  return ISM330DLC_OK;
}

/**
 * @brief  Deinitialize the ISM330DLC sensor
 * @param  pObj the device pObj
 * @retval 0 in case of success, an error code otherwise
 */
int32_t ISM330DLC_DeInit(ISM330DLC_Object_t *pObj)
{
  /* Disable the component */
  if (ISM330DLC_ACC_Disable(pObj) != ISM330DLC_OK)
  {
    return ISM330DLC_ERROR;
  }

  if (ISM330DLC_GYRO_Disable(pObj) != ISM330DLC_OK)
  {
    return ISM330DLC_ERROR;
  }

  /* Reset output data rate. */
  pObj->acc_odr = ISM330DLC_XL_ODR_OFF;
  pObj->gyro_odr = ISM330DLC_GY_ODR_OFF;

  pObj->is_initialized = 0;

  return ISM330DLC_OK;
}

/**
 * @brief  Read component ID
 * @param  pObj the device pObj
 * @param  Id the WHO_AM_I value
 * @retval 0 in case of success, an error code otherwise
 */
int32_t ISM330DLC_ReadID(ISM330DLC_Object_t *pObj, uint8_t *Id)
{
  if (ism330dlc_device_id_get(&(pObj->Ctx), Id) != ISM330DLC_OK)
  {
    return ISM330DLC_ERROR;
  }

  return ISM330DLC_OK;
}

/**
 * @brief  Get ISM330DLC sensor capabilities
 * @param  pObj Component object pointer
 * @param  Capabilities pointer to ISM330DLC sensor capabilities
 * @retval 0 in case of success, an error code otherwise
 */
int32_t ISM330DLC_GetCapabilities(ISM330DLC_Object_t *pObj, ISM330DLC_Capabilities_t *Capabilities)
{
  /* Prevent unused argument(s) compilation warning */
  (void)(pObj);

  Capabilities->Acc          = 1;
  Capabilities->Gyro         = 1;
  Capabilities->Magneto      = 0;
  Capabilities->LowPower     = 0;
  Capabilities->GyroMaxFS    = 2000;
  Capabilities->AccMaxFS     = 16;
  Capabilities->MagMaxFS     = 0;
  Capabilities->GyroMaxOdr   = 6660.0f;
  Capabilities->AccMaxOdr    = 6660.0f;
  Capabilities->MagMaxOdr    = 0.0f;
  return ISM330DLC_OK;
}

/**
 * @brief  Enable the ISM330DLC accelerometer sensor
 * @param  pObj the device pObj
 * @retval 0 in case of success, an error code otherwise
 */
int32_t ISM330DLC_ACC_Enable(ISM330DLC_Object_t *pObj)
{
  /* Check if the component is already enabled */
  if (pObj->acc_is_enabled == 1U)
  {
    return ISM330DLC_OK;
  }

  /* Output data rate selection. */
  if (ism330dlc_xl_data_rate_set(&(pObj->Ctx), pObj->acc_odr) != ISM330DLC_OK)
  {
    return ISM330DLC_ERROR;
  }

  pObj->acc_is_enabled = 1;

  return ISM330DLC_OK;
}

/**
 * @brief  Disable the ISM330DLC accelerometer sensor
 * @param  pObj the device pObj
 * @retval 0 in case of success, an error code otherwise
 */
int32_t ISM330DLC_ACC_Disable(ISM330DLC_Object_t *pObj)
{
  /* Check if the component is already disabled */
  if (pObj->acc_is_enabled == 0U)
  {
    return ISM330DLC_OK;
  }

  /* Get current output data rate. */
  if (ism330dlc_xl_data_rate_get(&(pObj->Ctx), &pObj->acc_odr) != ISM330DLC_OK)
  {
    return ISM330DLC_ERROR;
  }

  /* Output data rate selection - power down. */
  if (ism330dlc_xl_data_rate_set(&(pObj->Ctx), ISM330DLC_XL_ODR_OFF) != ISM330DLC_OK)
  {
    return ISM330DLC_ERROR;
  }

  pObj->acc_is_enabled = 0;

  return ISM330DLC_OK;
}

/**
 * @brief  Get the ISM330DLC accelerometer sensor sensitivity
 * @param  pObj the device pObj
 * @param  Sensitivity pointer
 * @retval 0 in case of success, an error code otherwise
 */
int32_t ISM330DLC_ACC_GetSensitivity(ISM330DLC_Object_t *pObj, float *Sensitivity)
{
  int32_t ret = ISM330DLC_OK;
  ism330dlc_fs_xl_t full_scale;

  /* Read actual full scale selection from sensor. */
  if (ism330dlc_xl_full_scale_get(&(pObj->Ctx), &full_scale) != ISM330DLC_OK)
  {
    return ISM330DLC_ERROR;
  }

  /* Store the Sensitivity based on actual full scale. */
  switch (full_scale)
  {
    case ISM330DLC_2g:
      *Sensitivity = ISM330DLC_ACC_SENSITIVITY_FS_2G;
      break;

    case ISM330DLC_4g:
      *Sensitivity = ISM330DLC_ACC_SENSITIVITY_FS_4G;
      break;

    case ISM330DLC_8g:
      *Sensitivity = ISM330DLC_ACC_SENSITIVITY_FS_8G;
      break;

    case ISM330DLC_16g:
      *Sensitivity = ISM330DLC_ACC_SENSITIVITY_FS_16G;
      break;

    default:
      ret = ISM330DLC_ERROR;
      break;
  }

  return ret;
}

/**
 * @brief  Get the ISM330DLC accelerometer sensor output data rate
 * @param  pObj the device pObj
 * @param  Odr pointer where the output data rate is written
 * @retval 0 in case of success, an error code otherwise
 */
int32_t ISM330DLC_ACC_GetOutputDataRate(ISM330DLC_Object_t *pObj, float *Odr)
{
  int32_t ret = ISM330DLC_OK;
  ism330dlc_odr_xl_t odr_low_level;

  /* Get current output data rate. */
  if (ism330dlc_xl_data_rate_get(&(pObj->Ctx), &odr_low_level) != ISM330DLC_OK)
  {
    return ISM330DLC_ERROR;
  }

  switch (odr_low_level)
  {
    case ISM330DLC_XL_ODR_OFF:
      *Odr = 0.0f;
      break;

    case ISM330DLC_XL_ODR_1Hz6:
      *Odr = 1.6f;
      break;

    case ISM330DLC_XL_ODR_12Hz5:
      *Odr = 12.5f;
      break;

    case ISM330DLC_XL_ODR_26Hz:
      *Odr = 26.0f;
      break;

    case ISM330DLC_XL_ODR_52Hz:
      *Odr = 52.0f;
      break;

    case ISM330DLC_XL_ODR_104Hz:
      *Odr = 104.0f;
      break;

    case ISM330DLC_XL_ODR_208Hz:
      *Odr = 208.0f;
      break;

    case ISM330DLC_XL_ODR_416Hz:
      *Odr = 416.0f;
      break;

    case ISM330DLC_XL_ODR_833Hz:
      *Odr = 833.0f;
      break;

    case ISM330DLC_XL_ODR_1k66Hz:
      *Odr = 1660.0f;
      break;

    case ISM330DLC_XL_ODR_3k33Hz:
      *Odr = 3330.0f;
      break;

    case ISM330DLC_XL_ODR_6k66Hz:
      *Odr = 6660.0f;
      break;

    default:
      ret = ISM330DLC_ERROR;
      break;
  }

  return ret;
}

/**
 * @brief  Set the ISM330DLC accelerometer sensor output data rate
 * @param  pObj the device pObj
 * @param  Odr the output data rate value to be set
 * @retval 0 in case of success, an error code otherwise
 */
int32_t ISM330DLC_ACC_SetOutputDataRate(ISM330DLC_Object_t *pObj, float Odr)
{
  /* Check if the component is enabled */
  if (pObj->acc_is_enabled == 1U)
  {
    return ISM330DLC_ACC_SetOutputDataRate_When_Enabled(pObj, Odr);
  }
  else
  {
    return ISM330DLC_ACC_SetOutputDataRate_When_Disabled(pObj, Odr);
  }
}

/**
 * @brief  Get the ISM330DLC accelerometer sensor full scale
 * @param  pObj the device pObj
 * @param  FullScale pointer where the full scale is written
 * @retval 0 in case of success, an error code otherwise
 */
int32_t ISM330DLC_ACC_GetFullScale(ISM330DLC_Object_t *pObj, int32_t *FullScale)
{
  int32_t ret = ISM330DLC_OK;
  ism330dlc_fs_xl_t fs_low_level;

  /* Read actual full scale selection from sensor. */
  if (ism330dlc_xl_full_scale_get(&(pObj->Ctx), &fs_low_level) != ISM330DLC_OK)
  {
    return ISM330DLC_ERROR;
  }

  switch (fs_low_level)
  {
    case ISM330DLC_2g:
      *FullScale =  2;
      break;

    case ISM330DLC_4g:
      *FullScale =  4;
      break;

    case ISM330DLC_8g:
      *FullScale =  8;
      break;

    case ISM330DLC_16g:
      *FullScale = 16;
      break;

    default:
      ret = ISM330DLC_ERROR;
      break;
  }

  return ret;
}

/**
 * @brief  Set the ISM330DLC accelerometer sensor full scale
 * @param  pObj the device pObj
 * @param  FullScale the functional full scale to be set
 * @retval 0 in case of success, an error code otherwise
 */
int32_t ISM330DLC_ACC_SetFullScale(ISM330DLC_Object_t *pObj, int32_t FullScale)
{
  ism330dlc_fs_xl_t new_fs;

  /* Seems like MISRA C-2012 rule 14.3a violation but only from single file statical analysis point of view because
     the parameter passed to the function is not known at the moment of analysis */
  new_fs = (FullScale <= 2) ? ISM330DLC_2g
           : (FullScale <= 4) ? ISM330DLC_4g
           : (FullScale <= 8) ? ISM330DLC_8g
           :                    ISM330DLC_16g;

  if (ism330dlc_xl_full_scale_set(&(pObj->Ctx), new_fs) != ISM330DLC_OK)
  {
    return ISM330DLC_ERROR;
  }

  return ISM330DLC_OK;
}

/**
 * @brief  Get the ISM330DLC accelerometer sensor raw axes
 * @param  pObj the device pObj
 * @param  Value pointer where the raw values of the axes are written
 * @retval 0 in case of success, an error code otherwise
 */
int32_t ISM330DLC_ACC_GetAxesRaw(ISM330DLC_Object_t *pObj, ISM330DLC_AxesRaw_t *Value)
{
  axis3bit16_t data_raw;

  /* Read raw data values. */
  if (ism330dlc_acceleration_raw_get(&(pObj->Ctx), data_raw.u8bit) != ISM330DLC_OK)
  {
    return ISM330DLC_ERROR;
  }

  /* Format the data. */
  Value->x = data_raw.i16bit[0];
  Value->y = data_raw.i16bit[1];
  Value->z = data_raw.i16bit[2];

  return ISM330DLC_OK;
}

/**
 * @brief  Get the ISM330DLC accelerometer sensor axes
 * @param  pObj the device pObj
 * @param  Acceleration pointer where the values of the axes are written
 * @retval 0 in case of success, an error code otherwise
 */
int32_t ISM330DLC_ACC_GetAxes(ISM330DLC_Object_t *pObj, ISM330DLC_Axes_t *Acceleration)
{
  axis3bit16_t data_raw;
  float sensitivity = 0.0f;

  /* Read raw data values. */
  if (ism330dlc_acceleration_raw_get(&(pObj->Ctx), data_raw.u8bit) != ISM330DLC_OK)
  {
    return ISM330DLC_ERROR;
  }

  /* Get ISM330DLC actual sensitivity. */
  if (ISM330DLC_ACC_GetSensitivity(pObj, &sensitivity) != ISM330DLC_OK)
  {
    return ISM330DLC_ERROR;
  }

  /* Calculate the data. */
  Acceleration->x = (int32_t)((float)((float)data_raw.i16bit[0] * sensitivity));
  Acceleration->y = (int32_t)((float)((float)data_raw.i16bit[1] * sensitivity));
  Acceleration->z = (int32_t)((float)((float)data_raw.i16bit[2] * sensitivity));

  return ISM330DLC_OK;
}

/**
 * @brief  Enable the ISM330DLC gyroscope sensor
 * @param  pObj the device pObj
 * @retval 0 in case of success, an error code otherwise
 */
int32_t ISM330DLC_GYRO_Enable(ISM330DLC_Object_t *pObj)
{
  /* Check if the component is already enabled */
  if (pObj->gyro_is_enabled == 1U)
  {
    return ISM330DLC_OK;
  }

  /* Output data rate selection. */
  if (ism330dlc_gy_data_rate_set(&(pObj->Ctx), pObj->gyro_odr) != ISM330DLC_OK)
  {
    return ISM330DLC_ERROR;
  }

  pObj->gyro_is_enabled = 1;

  return ISM330DLC_OK;
}

/**
 * @brief  Disable the ISM330DLC gyroscope sensor
 * @param  pObj the device pObj
 * @retval 0 in case of success, an error code otherwise
 */
int32_t ISM330DLC_GYRO_Disable(ISM330DLC_Object_t *pObj)
{
  /* Check if the component is already disabled */
  if (pObj->gyro_is_enabled == 0U)
  {
    return ISM330DLC_OK;
  }

  /* Get current output data rate. */
  if (ism330dlc_gy_data_rate_get(&(pObj->Ctx), &pObj->gyro_odr) != ISM330DLC_OK)
  {
    return ISM330DLC_ERROR;
  }

  /* Output data rate selection - power down. */
  if (ism330dlc_gy_data_rate_set(&(pObj->Ctx), ISM330DLC_GY_ODR_OFF) != ISM330DLC_OK)
  {
    return ISM330DLC_ERROR;
  }

  pObj->gyro_is_enabled = 0;

  return ISM330DLC_OK;
}

/**
 * @brief  Get the ISM330DLC gyroscope sensor sensitivity
 * @param  pObj the device pObj
 * @param  Sensitivity pointer
 * @retval 0 in case of success, an error code otherwise
 */
int32_t ISM330DLC_GYRO_GetSensitivity(ISM330DLC_Object_t *pObj, float *Sensitivity)
{
  int32_t ret = ISM330DLC_OK;
  ism330dlc_fs_g_t full_scale;

  /* Read actual full scale selection from sensor. */
  if (ism330dlc_gy_full_scale_get(&(pObj->Ctx), &full_scale) != ISM330DLC_OK)
  {
    return ISM330DLC_ERROR;
  }

  /* Store the sensitivity based on actual full scale. */
  switch (full_scale)
  {
    case ISM330DLC_125dps:
      *Sensitivity = ISM330DLC_GYRO_SENSITIVITY_FS_125DPS;
      break;

    case ISM330DLC_250dps:
      *Sensitivity = ISM330DLC_GYRO_SENSITIVITY_FS_250DPS;
      break;

    case ISM330DLC_500dps:
      *Sensitivity = ISM330DLC_GYRO_SENSITIVITY_FS_500DPS;
      break;

    case ISM330DLC_1000dps:
      *Sensitivity = ISM330DLC_GYRO_SENSITIVITY_FS_1000DPS;
      break;

    case ISM330DLC_2000dps:
      *Sensitivity = ISM330DLC_GYRO_SENSITIVITY_FS_2000DPS;
      break;

    default:
      ret = ISM330DLC_ERROR;
      break;
  }

  return ret;
}

/**
 * @brief  Get the ISM330DLC gyroscope sensor output data rate
 * @param  pObj the device pObj
 * @param  Odr pointer where the output data rate is written
 * @retval 0 in case of success, an error code otherwise
 */
int32_t ISM330DLC_GYRO_GetOutputDataRate(ISM330DLC_Object_t *pObj, float *Odr)
{
  int32_t ret = ISM330DLC_OK;
  ism330dlc_odr_g_t odr_low_level;

  /* Get current output data rate. */
  if (ism330dlc_gy_data_rate_get(&(pObj->Ctx), &odr_low_level) != ISM330DLC_OK)
  {
    return ISM330DLC_ERROR;
  }

  switch (odr_low_level)
  {
    case ISM330DLC_GY_ODR_OFF:
      *Odr = 0.0f;
      break;

    case ISM330DLC_GY_ODR_12Hz5:
      *Odr = 12.5f;
      break;

    case ISM330DLC_GY_ODR_26Hz:
      *Odr = 26.0f;
      break;

    case ISM330DLC_GY_ODR_52Hz:
      *Odr = 52.0f;
      break;

    case ISM330DLC_GY_ODR_104Hz:
      *Odr = 104.0f;
      break;

    case ISM330DLC_GY_ODR_208Hz:
      *Odr = 208.0f;
      break;

    case ISM330DLC_GY_ODR_416Hz:
      *Odr = 416.0f;
      break;

    case ISM330DLC_GY_ODR_833Hz:
      *Odr = 833.0f;
      break;

    case ISM330DLC_GY_ODR_1k66Hz:
      *Odr =  1660.0f;
      break;

    case ISM330DLC_GY_ODR_3k33Hz:
      *Odr =  3330.0f;
      break;

    case ISM330DLC_GY_ODR_6k66Hz:
      *Odr =  6660.0f;
      break;

    default:
      ret = ISM330DLC_ERROR;
      break;
  }

  return ret;
}

/**
 * @brief  Set the ISM330DLC gyroscope sensor output data rate
 * @param  pObj the device pObj
 * @param  Odr the output data rate value to be set
 * @retval 0 in case of success, an error code otherwise
 */
int32_t ISM330DLC_GYRO_SetOutputDataRate(ISM330DLC_Object_t *pObj, float Odr)
{
  /* Check if the component is enabled */
  if (pObj->gyro_is_enabled == 1U)
  {
    return ISM330DLC_GYRO_SetOutputDataRate_When_Enabled(pObj, Odr);
  }
  else
  {
    return ISM330DLC_GYRO_SetOutputDataRate_When_Disabled(pObj, Odr);
  }
}

/**
 * @brief  Get the ISM330DLC gyroscope sensor full scale
 * @param  pObj the device pObj
 * @param  FullScale pointer where the full scale is written
 * @retval 0 in case of success, an error code otherwise
 */
int32_t ISM330DLC_GYRO_GetFullScale(ISM330DLC_Object_t *pObj, int32_t  *FullScale)
{
  int32_t ret = ISM330DLC_OK;
  ism330dlc_fs_g_t fs_low_level;

  /* Read actual full scale selection from sensor. */
  if (ism330dlc_gy_full_scale_get(&(pObj->Ctx), &fs_low_level) != ISM330DLC_OK)
  {
    return ISM330DLC_ERROR;
  }

  switch (fs_low_level)
  {
    case ISM330DLC_125dps:
      *FullScale =  125;
      break;

    case ISM330DLC_250dps:
      *FullScale =  250;
      break;

    case ISM330DLC_500dps:
      *FullScale =  500;
      break;

    case ISM330DLC_1000dps:
      *FullScale = 1000;
      break;

    case ISM330DLC_2000dps:
      *FullScale = 2000;
      break;

    default:
      ret = ISM330DLC_ERROR;
      break;
  }

  return ret;
}

/**
 * @brief  Set the ISM330DLC gyroscope sensor full scale
 * @param  pObj the device pObj
 * @param  FullScale the functional full scale to be set
 * @retval 0 in case of success, an error code otherwise
 */
int32_t ISM330DLC_GYRO_SetFullScale(ISM330DLC_Object_t *pObj, int32_t FullScale)
{
  ism330dlc_fs_g_t new_fs;

  new_fs = (FullScale <= 125)  ? ISM330DLC_125dps
           : (FullScale <= 250)  ? ISM330DLC_250dps
           : (FullScale <= 500)  ? ISM330DLC_500dps
           : (FullScale <= 1000) ? ISM330DLC_1000dps
           :                       ISM330DLC_2000dps;

  if (ism330dlc_gy_full_scale_set(&(pObj->Ctx), new_fs) != ISM330DLC_OK)
  {
    return ISM330DLC_ERROR;
  }

  return ISM330DLC_OK;
}

/**
 * @brief  Get the ISM330DLC gyroscope sensor raw axes
 * @param  pObj the device pObj
 * @param  Value pointer where the raw values of the axes are written
 * @retval 0 in case of success, an error code otherwise
 */
int32_t ISM330DLC_GYRO_GetAxesRaw(ISM330DLC_Object_t *pObj, ISM330DLC_AxesRaw_t *Value)
{
  axis3bit16_t data_raw;

  /* Read raw data values. */
  if (ism330dlc_angular_rate_raw_get(&(pObj->Ctx), data_raw.u8bit) != ISM330DLC_OK)
  {
    return ISM330DLC_ERROR;
  }

  /* Format the data. */
  Value->x = data_raw.i16bit[0];
  Value->y = data_raw.i16bit[1];
  Value->z = data_raw.i16bit[2];

  return ISM330DLC_OK;
}

/**
 * @brief  Get the ISM330DLC gyroscope sensor axes
 * @param  pObj the device pObj
 * @param  AngularRate pointer where the values of the axes are written
 * @retval 0 in case of success, an error code otherwise
 */
int32_t ISM330DLC_GYRO_GetAxes(ISM330DLC_Object_t *pObj, ISM330DLC_Axes_t *AngularRate)
{
  axis3bit16_t data_raw;
  float sensitivity;

  /* Read raw data values. */
  if (ism330dlc_angular_rate_raw_get(&(pObj->Ctx), data_raw.u8bit) != ISM330DLC_OK)
  {
    return ISM330DLC_ERROR;
  }

  /* Get ISM330DLC actual sensitivity. */
  if (ISM330DLC_GYRO_GetSensitivity(pObj, &sensitivity) != ISM330DLC_OK)
  {
    return ISM330DLC_ERROR;
  }

  /* Calculate the data. */
  AngularRate->x = (int32_t)((float)((float)data_raw.i16bit[0] * sensitivity));
  AngularRate->y = (int32_t)((float)((float)data_raw.i16bit[1] * sensitivity));
  AngularRate->z = (int32_t)((float)((float)data_raw.i16bit[2] * sensitivity));

  return ISM330DLC_OK;
}

/**
 * @brief  Get the ISM330DLC register value
 * @param  pObj the device pObj
 * @param  Reg address to be read
 * @param  Data pointer where the value is written
 * @retval 0 in case of success, an error code otherwise
 */
int32_t ISM330DLC_Read_Reg(ISM330DLC_Object_t *pObj, uint8_t Reg, uint8_t *Data)
{
  if (ism330dlc_read_reg(&(pObj->Ctx), Reg, Data, 1) != ISM330DLC_OK)
  {
    return ISM330DLC_ERROR;
  }

  return ISM330DLC_OK;
}

/**
 * @brief  Set the ISM330DLC register value
 * @param  pObj the device pObj
 * @param  Reg address to be written
 * @param  Data value to be written
 * @retval 0 in case of success, an error code otherwise
 */
int32_t ISM330DLC_Write_Reg(ISM330DLC_Object_t *pObj, uint8_t Reg, uint8_t Data)
{
  if (ism330dlc_write_reg(&(pObj->Ctx), Reg, &Data, 1) != ISM330DLC_OK)
  {
    return ISM330DLC_ERROR;
  }

  return ISM330DLC_OK;
}

/**
 * @brief  Set the interrupt latch
 * @param  pObj the device pObj
 * @param  Status value to be written
 * @retval 0 in case of success, an error code otherwise
 */
int32_t ISM330DLC_Set_Interrupt_Latch(ISM330DLC_Object_t *pObj, uint8_t Status)
{
  if (Status > 1U)
  {
    return ISM330DLC_ERROR;
  }

  if (ism330dlc_int_notification_set(&(pObj->Ctx), (ism330dlc_lir_t)Status) != ISM330DLC_OK)
  {
    return ISM330DLC_ERROR;
  }

  return ISM330DLC_OK;
}

/**
 * @brief  Set the ISM330DLC FIFO full interrupt on INT1 pin
 * @param  pObj the device pObj
 * @param  Status DRDY interrupt on INT1 pin status
 * @retval 0 in case of success, an error code otherwise
 */
int32_t ISM330DLC_Set_INT1_Drdy(ISM330DLC_Object_t *pObj, uint8_t Status)
{
  ism330dlc_reg_t reg;

  if (ism330dlc_read_reg(&(pObj->Ctx), ISM330DLC_INT1_CTRL, &reg.byte, 1) != ISM330DLC_OK)
  {
    return ISM330DLC_ERROR;
  }

  reg.int1_ctrl.int1_drdy_xl = Status;

  if (ism330dlc_write_reg(&(pObj->Ctx), ISM330DLC_INT1_CTRL, &reg.byte, 1) != ISM330DLC_OK)
  {
    return ISM330DLC_ERROR;
  }

  return ISM330DLC_OK;
}

/**
 * @brief  Set the dataready mode status
 * @param  pObj the device pObj
 * @param  Status DRDY mode status
 * @retval 0 in case of success, an error code otherwise
 */
int32_t ISM330DLC_Set_Drdy_Mode(ISM330DLC_Object_t *pObj, uint8_t Status)
{
  if (ism330dlc_data_ready_mode_set(&(pObj->Ctx),(ism330dlc_drdy_pulsed_t)Status) != ISM330DLC_OK)
  {
    return ISM330DLC_ERROR;
  }
  return ISM330DLC_OK;
}

/**
 * @brief  Enable free fall detection
 * @param  pObj the device pObj
 * @param  IntPin interrupt pin line to be used
 * @retval 0 in case of success, an error code otherwise
 */
int32_t ISM330DLC_ACC_Enable_Free_Fall_Detection(ISM330DLC_Object_t *pObj, ISM330DLC_SensorIntPin_t IntPin)
{
  int32_t ret = ISM330DLC_OK;
  ism330dlc_int1_route_t val1;
  ism330dlc_int2_route_t val2;

  /* Output Data Rate selection */
  if (ISM330DLC_ACC_SetOutputDataRate(pObj, 416.0f) != ISM330DLC_OK)
  {
    return ISM330DLC_ERROR;
  }

  /* Full scale selection */
  if (ISM330DLC_ACC_SetFullScale(pObj, 2) != ISM330DLC_OK)
  {
    return ISM330DLC_ERROR;
  }

  /* FF_DUR setting */
  if (ism330dlc_ff_dur_set(&(pObj->Ctx), 0x06) != ISM330DLC_OK)
  {
    return ISM330DLC_ERROR;
  }

  /* WAKE_DUR setting */
  if (ism330dlc_wkup_dur_set(&(pObj->Ctx), 0x00) != ISM330DLC_OK)
  {
    return ISM330DLC_ERROR;
  }

  /* TIMER_HR setting */
  if (ism330dlc_timestamp_res_set(&(pObj->Ctx), ISM330DLC_LSB_6ms4) != ISM330DLC_OK)
  {
    return ISM330DLC_ERROR;
  }

  /* SLEEP_DUR setting */
  if (ism330dlc_act_sleep_dur_set(&(pObj->Ctx), 0x00) != ISM330DLC_OK)
  {
    return ISM330DLC_ERROR;
  }

  /* FF_THS setting */
  if (ism330dlc_ff_threshold_set(&(pObj->Ctx), ISM330DLC_FF_TSH_312mg) != ISM330DLC_OK)
  {
    return ISM330DLC_ERROR;
  }

  /* Enable free fall event on either INT1 or INT2 pin */
  switch (IntPin)
  {
    case ISM330DLC_INT1_PIN:
      if (ism330dlc_pin_int1_route_get(&(pObj->Ctx), &val1) != ISM330DLC_OK)
      {
        return ISM330DLC_ERROR;
      }

      val1.int1_ff = PROPERTY_ENABLE;

      if (ism330dlc_pin_int1_route_set(&(pObj->Ctx), val1) != ISM330DLC_OK)
      {
        return ISM330DLC_ERROR;
      }
      break;

    case ISM330DLC_INT2_PIN:
      if (ism330dlc_pin_int2_route_get(&(pObj->Ctx), &val2) != ISM330DLC_OK)
      {
        return ISM330DLC_ERROR;
      }

      val2.int2_ff = PROPERTY_ENABLE;

      if (ism330dlc_pin_int2_route_set(&(pObj->Ctx), val2) != ISM330DLC_OK)
      {
        return ISM330DLC_ERROR;
      }
      break;

    default:
      ret = ISM330DLC_ERROR;
      break;
  }

  return ret;
}

/**
 * @brief  Disable free fall detection
 * @param  pObj the device pObj
 * @retval 0 in case of success, an error code otherwise
 */
int32_t ISM330DLC_ACC_Disable_Free_Fall_Detection(ISM330DLC_Object_t *pObj)
{
  ism330dlc_int1_route_t val1;
  ism330dlc_int2_route_t val2;

  /* Disable free fall event on both INT1 and INT2 pins */
  if (ism330dlc_pin_int1_route_get(&(pObj->Ctx), &val1) != ISM330DLC_OK)
  {
    return ISM330DLC_ERROR;
  }

  val1.int1_ff = PROPERTY_DISABLE;

  if (ism330dlc_pin_int1_route_set(&(pObj->Ctx), val1) != ISM330DLC_OK)
  {
    return ISM330DLC_ERROR;
  }

  if (ism330dlc_pin_int2_route_get(&(pObj->Ctx), &val2) != ISM330DLC_OK)
  {
    return ISM330DLC_ERROR;
  }

  val2.int2_ff = PROPERTY_DISABLE;

  if (ism330dlc_pin_int2_route_set(&(pObj->Ctx), val2) != ISM330DLC_OK)
  {
    return ISM330DLC_ERROR;
  }

  /* FF_DUR setting */
  if (ism330dlc_ff_dur_set(&(pObj->Ctx), 0x00) != ISM330DLC_OK)
  {
    return ISM330DLC_ERROR;
  }

  /* FF_THS setting */
  if (ism330dlc_ff_threshold_set(&(pObj->Ctx), ISM330DLC_FF_TSH_156mg) != ISM330DLC_OK)
  {
    return ISM330DLC_ERROR;
  }

  return ISM330DLC_OK;
}

/**
 * @brief  Set free fall threshold
 * @param  pObj the device pObj
 * @param  Threshold free fall detection threshold
 * @retval 0 in case of success, an error code otherwise
 */
int32_t ISM330DLC_ACC_Set_Free_Fall_Threshold(ISM330DLC_Object_t *pObj, uint8_t Threshold)
{
  if (ism330dlc_ff_threshold_set(&(pObj->Ctx), (ism330dlc_ff_ths_t)Threshold) != ISM330DLC_OK)
  {
    return ISM330DLC_ERROR;
  }

  return ISM330DLC_OK;
}

/**
 * @brief  Set free fall duration
 * @param  pObj the device pObj
 * @param  Duration free fall detection duration
 * @retval 0 in case of success, an error code otherwise
 */
int32_t ISM330DLC_ACC_Set_Free_Fall_Duration(ISM330DLC_Object_t *pObj, uint8_t Duration)
{
  if (ism330dlc_ff_dur_set(&(pObj->Ctx), Duration) != ISM330DLC_OK)
  {
    return ISM330DLC_ERROR;
  }

  return ISM330DLC_OK;
}

/**
 * @brief  Enable tilt detection
 * @param  pObj the device pObj
 * @param  IntPin interrupt pin line to be used
 * @retval 0 in case of success, an error code otherwise
 */
int32_t ISM330DLC_ACC_Enable_Tilt_Detection(ISM330DLC_Object_t *pObj, ISM330DLC_SensorIntPin_t IntPin)
{
  int32_t ret = ISM330DLC_OK;
  ism330dlc_int1_route_t val1;
  ism330dlc_int2_route_t val2;

  /* Output Data Rate selection */
  if (ISM330DLC_ACC_SetOutputDataRate(pObj, 26.0f) != ISM330DLC_OK)
  {
    return ISM330DLC_ERROR;
  }

  /* Full scale selection */
  if (ISM330DLC_ACC_SetFullScale(pObj, 2) != ISM330DLC_OK)
  {
    return ISM330DLC_ERROR;
  }

  /* Enable tilt calculation. */
  if (ism330dlc_tilt_sens_set(&(pObj->Ctx), PROPERTY_ENABLE) != ISM330DLC_OK)
  {
    return ISM330DLC_ERROR;
  }

  /* Enable tilt event on either INT1 or INT2 pin */
  switch (IntPin)
  {
    case ISM330DLC_INT1_PIN:
      if (ism330dlc_pin_int1_route_get(&(pObj->Ctx), &val1) != ISM330DLC_OK)
      {
        return ISM330DLC_ERROR;
      }

      val1.int1_tilt = PROPERTY_ENABLE;

      if (ism330dlc_pin_int1_route_set(&(pObj->Ctx), val1) != ISM330DLC_OK)
      {
        return ISM330DLC_ERROR;
      }
      break;

    case ISM330DLC_INT2_PIN:
      if (ism330dlc_pin_int2_route_get(&(pObj->Ctx), &val2) != ISM330DLC_OK)
      {
        return ISM330DLC_ERROR;
      }

      val2.int2_tilt = PROPERTY_ENABLE;

      if (ism330dlc_pin_int2_route_set(&(pObj->Ctx), val2) != ISM330DLC_OK)
      {
        return ISM330DLC_ERROR;
      }
      break;

    default:
      ret = ISM330DLC_ERROR;
      break;
  }

  return ret;
}

/**
 * @brief  Disable tilt detection
 * @param  pObj the device pObj
 * @retval 0 in case of success, an error code otherwise
 */
int32_t ISM330DLC_ACC_Disable_Tilt_Detection(ISM330DLC_Object_t *pObj)
{
  ism330dlc_int1_route_t val1;
  ism330dlc_int2_route_t val2;

  /* Disable tilt event on both INT1 and INT2 pins */
  if (ism330dlc_pin_int1_route_get(&(pObj->Ctx), &val1) != ISM330DLC_OK)
  {
    return ISM330DLC_ERROR;
  }

  val1.int1_tilt = PROPERTY_DISABLE;

  if (ism330dlc_pin_int1_route_set(&(pObj->Ctx), val1) != ISM330DLC_OK)
  {
    return ISM330DLC_ERROR;
  }

  if (ism330dlc_pin_int2_route_get(&(pObj->Ctx), &val2) != ISM330DLC_OK)
  {
    return ISM330DLC_ERROR;
  }

  val2.int2_tilt = PROPERTY_DISABLE;

  if (ism330dlc_pin_int2_route_set(&(pObj->Ctx), val2) != ISM330DLC_OK)
  {
    return ISM330DLC_ERROR;
  }

  /* Disable tilt calculation. */
  if (ism330dlc_tilt_sens_set(&(pObj->Ctx), PROPERTY_DISABLE) != ISM330DLC_OK)
  {
    return ISM330DLC_ERROR;
  }

  return ISM330DLC_OK;
}

/**
 * @brief  Enable wake up detection
 * @param  pObj the device pObj
 * @param  IntPin interrupt pin line to be used
 * @retval 0 in case of success, an error code otherwise
 */
int32_t ISM330DLC_ACC_Enable_Wake_Up_Detection(ISM330DLC_Object_t *pObj, ISM330DLC_SensorIntPin_t IntPin)
{
  int32_t ret = ISM330DLC_OK;
  ism330dlc_int1_route_t val1;
  ism330dlc_int2_route_t val2;

  /* Output Data Rate selection */
  if (ISM330DLC_ACC_SetOutputDataRate(pObj, 416.0f) != ISM330DLC_OK)
  {
    return ISM330DLC_ERROR;
  }

  /* Full scale selection */
  if (ISM330DLC_ACC_SetFullScale(pObj, 2) != ISM330DLC_OK)
  {
    return ISM330DLC_ERROR;
  }

  /* WAKE_DUR setting */
  if (ism330dlc_wkup_dur_set(&(pObj->Ctx), 0x00) != ISM330DLC_OK)
  {
    return ISM330DLC_ERROR;
  }

  /* Set wake up threshold. */
  if (ism330dlc_wkup_threshold_set(&(pObj->Ctx), 0x02) != ISM330DLC_OK)
  {
    return ISM330DLC_ERROR;
  }

  /* Enable wake up event on either INT1 or INT2 pin */
  switch (IntPin)
  {
    case ISM330DLC_INT1_PIN:
      if (ism330dlc_pin_int1_route_get(&(pObj->Ctx), &val1) != ISM330DLC_OK)
      {
        return ISM330DLC_ERROR;
      }

      val1.int1_wu = PROPERTY_ENABLE;

      if (ism330dlc_pin_int1_route_set(&(pObj->Ctx), val1) != ISM330DLC_OK)
      {
        return ISM330DLC_ERROR;
      }
      break;

    case ISM330DLC_INT2_PIN:
      if (ism330dlc_pin_int2_route_get(&(pObj->Ctx), &val2) != ISM330DLC_OK)
      {
        return ISM330DLC_ERROR;
      }

      val2.int2_wu = PROPERTY_ENABLE;

      if (ism330dlc_pin_int2_route_set(&(pObj->Ctx), val2) != ISM330DLC_OK)
      {
        return ISM330DLC_ERROR;
      }
      break;

    default:
      ret = ISM330DLC_ERROR;
      break;
  }

  return ret;
}

/**
 * @brief  Disable wake up detection
 * @param  pObj the device pObj
 * @retval 0 in case of success, an error code otherwise
 */
int32_t ISM330DLC_ACC_Disable_Wake_Up_Detection(ISM330DLC_Object_t *pObj)
{
  ism330dlc_int1_route_t val1;
  ism330dlc_int2_route_t val2;

  /* Disable wake up event on both INT1 and INT2 pins */
  if (ism330dlc_pin_int1_route_get(&(pObj->Ctx), &val1) != ISM330DLC_OK)
  {
    return ISM330DLC_ERROR;
  }

  val1.int1_wu = PROPERTY_DISABLE;

  if (ism330dlc_pin_int1_route_set(&(pObj->Ctx), val1) != ISM330DLC_OK)
  {
    return ISM330DLC_ERROR;
  }

  if (ism330dlc_pin_int2_route_get(&(pObj->Ctx), &val2) != ISM330DLC_OK)
  {
    return ISM330DLC_ERROR;
  }

  val2.int2_wu = PROPERTY_DISABLE;

  if (ism330dlc_pin_int2_route_set(&(pObj->Ctx), val2) != ISM330DLC_OK)
  {
    return ISM330DLC_ERROR;
  }

  /* Reset wake up threshold. */
  if (ism330dlc_wkup_threshold_set(&(pObj->Ctx), 0x00) != ISM330DLC_OK)
  {
    return ISM330DLC_ERROR;
  }

  /* WAKE_DUR setting */
  if (ism330dlc_wkup_dur_set(&(pObj->Ctx), 0x00) != ISM330DLC_OK)
  {
    return ISM330DLC_ERROR;
  }

  return ISM330DLC_OK;
}

/**
 * @brief  Set wake up threshold
 * @param  pObj the device pObj
 * @param  Threshold wake up detection threshold
 * @retval 0 in case of success, an error code otherwise
 */
int32_t ISM330DLC_ACC_Set_Wake_Up_Threshold(ISM330DLC_Object_t *pObj, uint8_t Threshold)
{
  /* Set wake up threshold. */
  if (ism330dlc_wkup_threshold_set(&(pObj->Ctx), Threshold) != ISM330DLC_OK)
  {
    return ISM330DLC_ERROR;
  }

  return ISM330DLC_OK;
}

/**
 * @brief  Set wake up duration
 * @param  pObj the device pObj
 * @param  Duration wake up detection duration
 * @retval 0 in case of success, an error code otherwise
 */
int32_t ISM330DLC_ACC_Set_Wake_Up_Duration(ISM330DLC_Object_t *pObj, uint8_t Duration)
{
  /* Set wake up duration. */
  if (ism330dlc_wkup_dur_set(&(pObj->Ctx), Duration) != ISM330DLC_OK)
  {
    return ISM330DLC_ERROR;
  }

  return ISM330DLC_OK;
}

/**
 * @brief  Enable single tap detection
 * @param  pObj the device pObj
 * @param  IntPin interrupt pin line to be used
 * @retval 0 in case of success, an error code otherwise
 */
int32_t ISM330DLC_ACC_Enable_Single_Tap_Detection(ISM330DLC_Object_t *pObj, ISM330DLC_SensorIntPin_t IntPin)
{
  int32_t ret = ISM330DLC_OK;
  ism330dlc_int1_route_t val1;
  ism330dlc_int2_route_t val2;

  /* Output Data Rate selection */
  if (ISM330DLC_ACC_SetOutputDataRate(pObj, 416.0f) != ISM330DLC_OK)
  {
    return ISM330DLC_ERROR;
  }

  /* Full scale selection */
  if (ISM330DLC_ACC_SetFullScale(pObj, 2) != ISM330DLC_OK)
  {
    return ISM330DLC_ERROR;
  }

  /* Enable X direction in tap recognition. */
  if (ism330dlc_tap_detection_on_x_set(&(pObj->Ctx), PROPERTY_ENABLE) != ISM330DLC_OK)
  {
    return ISM330DLC_ERROR;
  }

  /* Enable Y direction in tap recognition. */
  if (ism330dlc_tap_detection_on_y_set(&(pObj->Ctx), PROPERTY_ENABLE) != ISM330DLC_OK)
  {
    return ISM330DLC_ERROR;
  }

  /* Enable Z direction in tap recognition. */
  if (ism330dlc_tap_detection_on_z_set(&(pObj->Ctx), PROPERTY_ENABLE) != ISM330DLC_OK)
  {
    return ISM330DLC_ERROR;
  }

  /* Set tap threshold. */
  if (ism330dlc_tap_threshold_x_set(&(pObj->Ctx), 0x08) != ISM330DLC_OK)
  {
    return ISM330DLC_ERROR;
  }

  /* Set tap shock time window. */
  if (ism330dlc_tap_shock_set(&(pObj->Ctx), 0x02) != ISM330DLC_OK)
  {
    return ISM330DLC_ERROR;
  }

  /* Set tap quiet time window. */
  if (ism330dlc_tap_quiet_set(&(pObj->Ctx), 0x01) != ISM330DLC_OK)
  {
    return ISM330DLC_ERROR;
  }

  /* _NOTE_: Tap duration time window - don't care for single tap. */

  /* _NOTE_: Single/Double Tap event - don't care of this flag for single tap. */

  /* Enable single tap event on either INT1 or INT2 pin */
  switch (IntPin)
  {
    case ISM330DLC_INT1_PIN:
      if (ism330dlc_pin_int1_route_get(&(pObj->Ctx), &val1) != ISM330DLC_OK)
      {
        return ISM330DLC_ERROR;
      }

      val1.int1_single_tap = PROPERTY_ENABLE;

      if (ism330dlc_pin_int1_route_set(&(pObj->Ctx), val1) != ISM330DLC_OK)
      {
        return ISM330DLC_ERROR;
      }
      break;

    case ISM330DLC_INT2_PIN:
      if (ism330dlc_pin_int2_route_get(&(pObj->Ctx), &val2) != ISM330DLC_OK)
      {
        return ISM330DLC_ERROR;
      }

      val2.int2_single_tap = PROPERTY_ENABLE;

      if (ism330dlc_pin_int2_route_set(&(pObj->Ctx), val2) != ISM330DLC_OK)
      {
        return ISM330DLC_ERROR;
      }
      break;

    default:
      ret = ISM330DLC_ERROR;
      break;
  }

  return ret;
}

/**
 * @brief  Disable single tap detection
 * @param  pObj the device pObj
 * @retval 0 in case of success, an error code otherwise
 */
int32_t ISM330DLC_ACC_Disable_Single_Tap_Detection(ISM330DLC_Object_t *pObj)
{
  ism330dlc_int1_route_t val1;
  ism330dlc_int2_route_t val2;

  /* Disable single tap event on both INT1 and INT2 pins */
  if (ism330dlc_pin_int1_route_get(&(pObj->Ctx), &val1) != ISM330DLC_OK)
  {
    return ISM330DLC_ERROR;
  }

  val1.int1_single_tap = PROPERTY_DISABLE;

  if (ism330dlc_pin_int1_route_set(&(pObj->Ctx), val1) != ISM330DLC_OK)
  {
    return ISM330DLC_ERROR;
  }

  if (ism330dlc_pin_int2_route_get(&(pObj->Ctx), &val2) != ISM330DLC_OK)
  {
    return ISM330DLC_ERROR;
  }

  val2.int2_single_tap = PROPERTY_DISABLE;

  if (ism330dlc_pin_int2_route_set(&(pObj->Ctx), val2) != ISM330DLC_OK)
  {
    return ISM330DLC_ERROR;
  }

  /* Reset tap quiet time window. */
  if (ism330dlc_tap_quiet_set(&(pObj->Ctx), 0x00) != ISM330DLC_OK)
  {
    return ISM330DLC_ERROR;
  }

  /* Reset tap shock time window. */
  if (ism330dlc_tap_shock_set(&(pObj->Ctx), 0x00) != ISM330DLC_OK)
  {
    return ISM330DLC_ERROR;
  }

  /* Reset tap threshold. */
  if (ism330dlc_tap_threshold_x_set(&(pObj->Ctx), 0x00) != ISM330DLC_OK)
  {
    return ISM330DLC_ERROR;
  }

  /* Disable Z direction in tap recognition. */
  if (ism330dlc_tap_detection_on_z_set(&(pObj->Ctx), PROPERTY_DISABLE) != ISM330DLC_OK)
  {
    return ISM330DLC_ERROR;
  }

  /* Disable Y direction in tap recognition. */
  if (ism330dlc_tap_detection_on_y_set(&(pObj->Ctx), PROPERTY_DISABLE) != ISM330DLC_OK)
  {
    return ISM330DLC_ERROR;
  }

  /* Disable X direction in tap recognition. */
  if (ism330dlc_tap_detection_on_x_set(&(pObj->Ctx), PROPERTY_DISABLE) != ISM330DLC_OK)
  {
    return ISM330DLC_ERROR;
  }

  return ISM330DLC_OK;
}

/**
 * @brief  Enable double tap detection
 * @param  pObj the device pObj
 * @param  IntPin interrupt pin line to be used
 * @retval 0 in case of success, an error code otherwise
 */
int32_t ISM330DLC_ACC_Enable_Double_Tap_Detection(ISM330DLC_Object_t *pObj, ISM330DLC_SensorIntPin_t IntPin)
{
  int32_t ret = ISM330DLC_OK;
  ism330dlc_int1_route_t val1;
  ism330dlc_int2_route_t val2;

  /* Output Data Rate selection */
  if (ISM330DLC_ACC_SetOutputDataRate(pObj, 416.0f) != ISM330DLC_OK)
  {
    return ISM330DLC_ERROR;
  }

  /* Full scale selection */
  if (ISM330DLC_ACC_SetFullScale(pObj, 2) != ISM330DLC_OK)
  {
    return ISM330DLC_ERROR;
  }

  /* Enable X direction in tap recognition. */
  if (ism330dlc_tap_detection_on_x_set(&(pObj->Ctx), PROPERTY_ENABLE) != ISM330DLC_OK)
  {
    return ISM330DLC_ERROR;
  }

  /* Enable Y direction in tap recognition. */
  if (ism330dlc_tap_detection_on_y_set(&(pObj->Ctx), PROPERTY_ENABLE) != ISM330DLC_OK)
  {
    return ISM330DLC_ERROR;
  }

  /* Enable Z direction in tap recognition. */
  if (ism330dlc_tap_detection_on_z_set(&(pObj->Ctx), PROPERTY_ENABLE) != ISM330DLC_OK)
  {
    return ISM330DLC_ERROR;
  }

  /* Set tap threshold. */
  if (ism330dlc_tap_threshold_x_set(&(pObj->Ctx), 0x08) != ISM330DLC_OK)
  {
    return ISM330DLC_ERROR;
  }

  /* Set tap shock time window. */
  if (ism330dlc_tap_shock_set(&(pObj->Ctx), 0x03) != ISM330DLC_OK)
  {
    return ISM330DLC_ERROR;
  }

  /* Set tap quiet time window. */
  if (ism330dlc_tap_quiet_set(&(pObj->Ctx), 0x03) != ISM330DLC_OK)
  {
    return ISM330DLC_ERROR;
  }

  /* Set tap duration time window. */
  if (ism330dlc_tap_dur_set(&(pObj->Ctx), 0x08) != ISM330DLC_OK)
  {
    return ISM330DLC_ERROR;
  }

  /* Single and double tap enabled. */
  if (ism330dlc_tap_mode_set(&(pObj->Ctx), ISM330DLC_BOTH_SINGLE_DOUBLE) != ISM330DLC_OK)
  {
    return ISM330DLC_ERROR;
  }

  /* Enable double tap event on either INT1 or INT2 pin */
  switch (IntPin)
  {
    case ISM330DLC_INT1_PIN:
      if (ism330dlc_pin_int1_route_get(&(pObj->Ctx), &val1) != ISM330DLC_OK)
      {
        return ISM330DLC_ERROR;
      }

      val1.int1_double_tap = PROPERTY_ENABLE;

      if (ism330dlc_pin_int1_route_set(&(pObj->Ctx), val1) != ISM330DLC_OK)
      {
        return ISM330DLC_ERROR;
      }
      break;

    case ISM330DLC_INT2_PIN:
      if (ism330dlc_pin_int2_route_get(&(pObj->Ctx), &val2) != ISM330DLC_OK)
      {
        return ISM330DLC_ERROR;
      }

      val2.int2_double_tap = PROPERTY_ENABLE;

      if (ism330dlc_pin_int2_route_set(&(pObj->Ctx), val2) != ISM330DLC_OK)
      {
        return ISM330DLC_ERROR;
      }
      break;

    default:
      ret = ISM330DLC_ERROR;
      break;
  }

  return ret;
}

/**
 * @brief  Disable double tap detection
 * @param  pObj the device pObj
 * @retval 0 in case of success, an error code otherwise
 */
int32_t ISM330DLC_ACC_Disable_Double_Tap_Detection(ISM330DLC_Object_t *pObj)
{
  ism330dlc_int1_route_t val1;
  ism330dlc_int2_route_t val2;

  /* Disable double tap event on both INT1 and INT2 pins */
  if (ism330dlc_pin_int1_route_get(&(pObj->Ctx), &val1) != ISM330DLC_OK)
  {
    return ISM330DLC_ERROR;
  }

  val1.int1_double_tap = PROPERTY_DISABLE;

  if (ism330dlc_pin_int1_route_set(&(pObj->Ctx), val1) != ISM330DLC_OK)
  {
    return ISM330DLC_ERROR;
  }

  if (ism330dlc_pin_int2_route_get(&(pObj->Ctx), &val2) != ISM330DLC_OK)
  {
    return ISM330DLC_ERROR;
  }

  val2.int2_double_tap = PROPERTY_DISABLE;

  if (ism330dlc_pin_int2_route_set(&(pObj->Ctx), val2) != ISM330DLC_OK)
  {
    return ISM330DLC_ERROR;
  }

  /* Only single tap enabled. */
  if (ism330dlc_tap_mode_set(&(pObj->Ctx), ISM330DLC_ONLY_SINGLE) != ISM330DLC_OK)
  {
    return ISM330DLC_ERROR;
  }

  /* Reset tap duration time window. */
  if (ism330dlc_tap_dur_set(&(pObj->Ctx), 0x00) != ISM330DLC_OK)
  {
    return ISM330DLC_ERROR;
  }

  /* Reset tap quiet time window. */
  if (ism330dlc_tap_quiet_set(&(pObj->Ctx), 0x00) != ISM330DLC_OK)
  {
    return ISM330DLC_ERROR;
  }

  /* Reset tap shock time window. */
  if (ism330dlc_tap_shock_set(&(pObj->Ctx), 0x00) != ISM330DLC_OK)
  {
    return ISM330DLC_ERROR;
  }

  /* Reset tap threshold. */
  if (ism330dlc_tap_threshold_x_set(&(pObj->Ctx), 0x00) != ISM330DLC_OK)
  {
    return ISM330DLC_ERROR;
  }

  /* Disable Z direction in tap recognition. */
  if (ism330dlc_tap_detection_on_z_set(&(pObj->Ctx), PROPERTY_DISABLE) != ISM330DLC_OK)
  {
    return ISM330DLC_ERROR;
  }

  /* Disable Y direction in tap recognition. */
  if (ism330dlc_tap_detection_on_y_set(&(pObj->Ctx), PROPERTY_DISABLE) != ISM330DLC_OK)
  {
    return ISM330DLC_ERROR;
  }

  /* Disable X direction in tap recognition. */
  if (ism330dlc_tap_detection_on_x_set(&(pObj->Ctx), PROPERTY_DISABLE) != ISM330DLC_OK)
  {
    return ISM330DLC_ERROR;
  }

  return ISM330DLC_OK;
}

/**
 * @brief  Set tap threshold
 * @param  pObj the device pObj
 * @param  Threshold tap threshold
 * @retval 0 in case of success, an error code otherwise
 */
int32_t ISM330DLC_ACC_Set_Tap_Threshold(ISM330DLC_Object_t *pObj, uint8_t Threshold)
{
  /* Set tap threshold. */
  if (ism330dlc_tap_threshold_x_set(&(pObj->Ctx), Threshold) != ISM330DLC_OK)
  {
    return ISM330DLC_ERROR;
  }

  return ISM330DLC_OK;
}

/**
 * @brief  Set tap shock time
 * @param  pObj the device pObj
 * @param  Time tap shock time
 * @retval 0 in case of success, an error code otherwise
 */
int32_t ISM330DLC_ACC_Set_Tap_Shock_Time(ISM330DLC_Object_t *pObj, uint8_t Time)
{
  /* Set tap shock time window. */
  if (ism330dlc_tap_shock_set(&(pObj->Ctx), Time) != ISM330DLC_OK)
  {
    return ISM330DLC_ERROR;
  }

  return ISM330DLC_OK;
}

/**
 * @brief  Set tap quiet time
 * @param  pObj the device pObj
 * @param  Time tap quiet time
 * @retval 0 in case of success, an error code otherwise
 */
int32_t ISM330DLC_ACC_Set_Tap_Quiet_Time(ISM330DLC_Object_t *pObj, uint8_t Time)
{
  /* Set tap quiet time window. */
  if (ism330dlc_tap_quiet_set(&(pObj->Ctx), Time) != ISM330DLC_OK)
  {
    return ISM330DLC_ERROR;
  }

  return ISM330DLC_OK;
}

/**
 * @brief  Set tap duration time
 * @param  pObj the device pObj
 * @param  Time tap duration time
 * @retval 0 in case of success, an error code otherwise
 */
int32_t ISM330DLC_ACC_Set_Tap_Duration_Time(ISM330DLC_Object_t *pObj, uint8_t Time)
{
  /* Set tap duration time window. */
  if (ism330dlc_tap_dur_set(&(pObj->Ctx), Time) != ISM330DLC_OK)
  {
    return ISM330DLC_ERROR;
  }

  return ISM330DLC_OK;
}

/**
 * @brief  Enable 6D orientation detection
 * @param  pObj the device pObj
 * @param  IntPin interrupt pin line to be used
 * @retval 0 in case of success, an error code otherwise
 */
int32_t ISM330DLC_ACC_Enable_6D_Orientation(ISM330DLC_Object_t *pObj, ISM330DLC_SensorIntPin_t IntPin)
{
  int32_t ret = ISM330DLC_OK;
  ism330dlc_int1_route_t val1;
  ism330dlc_int2_route_t val2;

  /* Output Data Rate selection */
  if (ISM330DLC_ACC_SetOutputDataRate(pObj, 416.0f) != ISM330DLC_OK)
  {
    return ISM330DLC_ERROR;
  }

  /* Full scale selection */
  if (ISM330DLC_ACC_SetFullScale(pObj, 2) != ISM330DLC_OK)
  {
    return ISM330DLC_ERROR;
  }

  /* 6D orientation enabled. */
  if (ism330dlc_6d_threshold_set(&(pObj->Ctx), ISM330DLC_DEG_60) != ISM330DLC_OK)
  {
    return ISM330DLC_ERROR;
  }

  /* Enable 6D orientation event on either INT1 or INT2 pin */
  switch (IntPin)
  {
    case ISM330DLC_INT1_PIN:
      if (ism330dlc_pin_int1_route_get(&(pObj->Ctx), &val1) != ISM330DLC_OK)
      {
        return ISM330DLC_ERROR;
      }

      val1.int1_6d = PROPERTY_ENABLE;

      if (ism330dlc_pin_int1_route_set(&(pObj->Ctx), val1) != ISM330DLC_OK)
      {
        return ISM330DLC_ERROR;
      }
      break;

    case ISM330DLC_INT2_PIN:
      if (ism330dlc_pin_int2_route_get(&(pObj->Ctx), &val2) != ISM330DLC_OK)
      {
        return ISM330DLC_ERROR;
      }

      val2.int2_6d = PROPERTY_ENABLE;

      if (ism330dlc_pin_int2_route_set(&(pObj->Ctx), val2) != ISM330DLC_OK)
      {
        return ISM330DLC_ERROR;
      }
      break;

    default:
      ret = ISM330DLC_ERROR;
      break;
  }

  return ret;
}

/**
 * @brief  Disable 6D orientation detection
 * @param  pObj the device pObj
 * @retval 0 in case of success, an error code otherwise
 */
int32_t ISM330DLC_ACC_Disable_6D_Orientation(ISM330DLC_Object_t *pObj)
{
  ism330dlc_int1_route_t val1;
  ism330dlc_int2_route_t val2;

  /* Disable 6D orientation event on both INT1 and INT2 pins */
  if (ism330dlc_pin_int1_route_get(&(pObj->Ctx), &val1) != ISM330DLC_OK)
  {
    return ISM330DLC_ERROR;
  }

  val1.int1_6d = PROPERTY_DISABLE;

  if (ism330dlc_pin_int1_route_set(&(pObj->Ctx), val1) != ISM330DLC_OK)
  {
    return ISM330DLC_ERROR;
  }

  if (ism330dlc_pin_int2_route_get(&(pObj->Ctx), &val2) != ISM330DLC_OK)
  {
    return ISM330DLC_ERROR;
  }

  val2.int2_6d = PROPERTY_DISABLE;

  if (ism330dlc_pin_int2_route_set(&(pObj->Ctx), val2) != ISM330DLC_OK)
  {
    return ISM330DLC_ERROR;
  }

  /* Reset 6D orientation. */
  if (ism330dlc_6d_threshold_set(&(pObj->Ctx), ISM330DLC_DEG_80) != ISM330DLC_OK)
  {
    return ISM330DLC_ERROR;
  }

  return ISM330DLC_OK;
}

/**
 * @brief  Set 6D orientation threshold
 * @param  pObj the device pObj
 * @param  Threshold free fall detection threshold
 * @retval 0 in case of success, an error code otherwise
 */
int32_t ISM330DLC_ACC_Set_6D_Orientation_Threshold(ISM330DLC_Object_t *pObj, uint8_t Threshold)
{
  if (ism330dlc_6d_threshold_set(&(pObj->Ctx), (ism330dlc_sixd_ths_t)Threshold) != ISM330DLC_OK)
  {
    return ISM330DLC_ERROR;
  }

  return ISM330DLC_OK;
}

/**
 * @brief  Get the status of XLow orientation
 * @param  pObj the device pObj
 * @param  XLow the status of XLow orientation
 * @retval 0 in case of success, an error code otherwise
 */
int32_t ISM330DLC_ACC_Get_6D_Orientation_XL(ISM330DLC_Object_t *pObj, uint8_t *XLow)
{
  ism330dlc_d6d_src_t data;

  if (ism330dlc_read_reg(&(pObj->Ctx), ISM330DLC_D6D_SRC, (uint8_t *)&data, 1) != ISM330DLC_OK)
  {
    return ISM330DLC_ERROR;
  }

  *XLow = data.xl;

  return ISM330DLC_OK;
}

/**
 * @brief  Get the status of XHigh orientation
 * @param  pObj the device pObj
 * @param  XHigh the status of XHigh orientation
 * @retval 0 in case of success, an error code otherwise
 */
int32_t ISM330DLC_ACC_Get_6D_Orientation_XH(ISM330DLC_Object_t *pObj, uint8_t *XHigh)
{
  ism330dlc_d6d_src_t data;

  if (ism330dlc_read_reg(&(pObj->Ctx), ISM330DLC_D6D_SRC, (uint8_t *)&data, 1) != ISM330DLC_OK)
  {
    return ISM330DLC_ERROR;
  }

  *XHigh = data.xh;

  return ISM330DLC_OK;
}

/**
 * @brief  Get the status of YLow orientation
 * @param  pObj the device pObj
 * @param  YLow the status of YLow orientation
 * @retval 0 in case of success, an error code otherwise
 */
int32_t ISM330DLC_ACC_Get_6D_Orientation_YL(ISM330DLC_Object_t *pObj, uint8_t *YLow)
{
  ism330dlc_d6d_src_t data;

  if (ism330dlc_read_reg(&(pObj->Ctx), ISM330DLC_D6D_SRC, (uint8_t *)&data, 1) != ISM330DLC_OK)
  {
    return ISM330DLC_ERROR;
  }

  *YLow = data.yl;

  return ISM330DLC_OK;
}

/**
 * @brief  Get the status of YHigh orientation
 * @param  pObj the device pObj
 * @param  YHigh the status of YHigh orientation
 * @retval 0 in case of success, an error code otherwise
 */
int32_t ISM330DLC_ACC_Get_6D_Orientation_YH(ISM330DLC_Object_t *pObj, uint8_t *YHigh)
{
  ism330dlc_d6d_src_t data;

  if (ism330dlc_read_reg(&(pObj->Ctx), ISM330DLC_D6D_SRC, (uint8_t *)&data, 1) != ISM330DLC_OK)
  {
    return ISM330DLC_ERROR;
  }

  *YHigh = data.yh;

  return ISM330DLC_OK;
}

/**
 * @brief  Get the status of ZLow orientation
 * @param  pObj the device pObj
 * @param  ZLow the status of ZLow orientation
 * @retval 0 in case of success, an error code otherwise
 */
int32_t ISM330DLC_ACC_Get_6D_Orientation_ZL(ISM330DLC_Object_t *pObj, uint8_t *ZLow)
{
  ism330dlc_d6d_src_t data;

  if (ism330dlc_read_reg(&(pObj->Ctx), ISM330DLC_D6D_SRC, (uint8_t *)&data, 1) != ISM330DLC_OK)
  {
    return ISM330DLC_ERROR;
  }

  *ZLow = data.zl;

  return ISM330DLC_OK;
}

/**
 * @brief  Get the status of ZHigh orientation
 * @param  pObj the device pObj
 * @param  ZHigh the status of ZHigh orientation
 * @retval 0 in case of success, an error code otherwise
 */
int32_t ISM330DLC_ACC_Get_6D_Orientation_ZH(ISM330DLC_Object_t *pObj, uint8_t *ZHigh)
{
  ism330dlc_d6d_src_t data;

  if (ism330dlc_read_reg(&(pObj->Ctx), ISM330DLC_D6D_SRC, (uint8_t *)&data, 1) != ISM330DLC_OK)
  {
    return ISM330DLC_ERROR;
  }

  *ZHigh = data.zh;

  return ISM330DLC_OK;
}

/**
 * @brief  Get the status of all hardware events
 * @param  pObj the device pObj
 * @param  Status the status of all hardware events
 * @retval 0 in case of success, an error code otherwise
 */
int32_t ISM330DLC_ACC_Get_Event_Status(ISM330DLC_Object_t *pObj, ISM330DLC_Event_Status_t *Status)
{
  ism330dlc_wake_up_src_t wake_up_src;
  ism330dlc_tap_src_t tap_src;
  ism330dlc_d6d_src_t d6d_src;
  ism330dlc_func_src1_t func_src;
  ism330dlc_md1_cfg_t md1_cfg;
  ism330dlc_md2_cfg_t md2_cfg;
  ism330dlc_int1_ctrl_t int1_ctrl;

  (void)memset((void *)Status, 0x0, sizeof(ISM330DLC_Event_Status_t));

  if (ism330dlc_read_reg(&(pObj->Ctx), ISM330DLC_WAKE_UP_SRC, (uint8_t *)&wake_up_src, 1) != ISM330DLC_OK)
  {
    return ISM330DLC_ERROR;
  }

  if (ism330dlc_read_reg(&(pObj->Ctx), ISM330DLC_TAP_SRC, (uint8_t *)&tap_src, 1) != ISM330DLC_OK)
  {
    return ISM330DLC_ERROR;
  }

  if (ism330dlc_read_reg(&(pObj->Ctx), ISM330DLC_D6D_SRC, (uint8_t *)&d6d_src, 1) != ISM330DLC_OK)
  {
    return ISM330DLC_ERROR;
  }

  if (ism330dlc_read_reg(&(pObj->Ctx), ISM330DLC_FUNC_SRC1, (uint8_t *)&func_src, 1) != ISM330DLC_OK)
  {
    return ISM330DLC_ERROR;
  }

  if (ism330dlc_read_reg(&(pObj->Ctx), ISM330DLC_MD1_CFG, (uint8_t *)&md1_cfg, 1) != ISM330DLC_OK)
  {
    return ISM330DLC_ERROR;
  }

  if (ism330dlc_read_reg(&(pObj->Ctx), ISM330DLC_MD2_CFG, (uint8_t *)&md2_cfg, 1) != ISM330DLC_OK)
  {
    return ISM330DLC_ERROR;
  }

  if (ism330dlc_read_reg(&(pObj->Ctx), ISM330DLC_INT1_CTRL, (uint8_t *)&int1_ctrl, 1) != ISM330DLC_OK)
  {
    return ISM330DLC_ERROR;
  }

  if ((md1_cfg.int1_ff == 1U) || (md2_cfg.int2_ff == 1U))
  {
    if (wake_up_src.ff_ia == 1U)
    {
      Status->FreeFallStatus = 1;
    }
  }

  if ((md1_cfg.int1_wu == 1U) || (md2_cfg.int2_wu == 1U))
  {
    if (wake_up_src.wu_ia == 1U)
    {
      Status->WakeUpStatus = 1;
    }
  }

  if ((md1_cfg.int1_single_tap == 1U) || (md2_cfg.int2_single_tap == 1U))
  {
    if (tap_src.single_tap == 1U)
    {
      Status->TapStatus = 1;
    }
  }

  if ((md1_cfg.int1_double_tap == 1U) || (md2_cfg.int2_double_tap == 1U))
  {
    if (tap_src.double_tap == 1U)
    {
      Status->DoubleTapStatus = 1;
    }
  }

  if ((md1_cfg.int1_6d == 1U) || (md2_cfg.int2_6d == 1U))
  {
    if (d6d_src.d6d_ia == 1U)
    {
      Status->D6DOrientationStatus = 1;
    }
  }

//  if (int1_ctrl.int1_step_detector == 1U)
//  {
//    if (func_src.step_detected == 1U)
//    {
//      Status->StepStatus = 1;
//    }
//  }

  if ((md1_cfg.int1_tilt == 1U) || (md2_cfg.int2_tilt == 1U))
  {
    if (func_src.tilt_ia == 1U)
    {
      Status->TiltStatus = 1;
    }
  }

  return ISM330DLC_OK;
}

/**
 * @brief  Set self test
 * @param  pObj the device pObj
 * @param  val the value of st_xl in reg CTRL5_C
 * @retval 0 in case of success, an error code otherwise
 */
int32_t ISM330DLC_ACC_Set_SelfTest(ISM330DLC_Object_t *pObj, uint8_t val)
{
  ism330dlc_st_xl_t reg;

  reg = (val == 0U)  ? ISM330DLC_XL_ST_DISABLE
        : (val == 1U)  ? ISM330DLC_XL_ST_POSITIVE
        : (val == 2U)  ? ISM330DLC_XL_ST_NEGATIVE
        :                ISM330DLC_XL_ST_DISABLE;

  if (ism330dlc_xl_self_test_set(&(pObj->Ctx), reg) != ISM330DLC_OK)
  {
    return ISM330DLC_ERROR;
  }

  return ISM330DLC_OK;
}

/**
 * @brief  Get the ISM330DLC ACC data ready bit value
 * @param  pObj the device pObj
 * @param  Status the status of data ready bit
 * @retval 0 in case of success, an error code otherwise
 */
int32_t ISM330DLC_ACC_Get_DRDY_Status(ISM330DLC_Object_t *pObj, uint8_t *Status)
{
  if (ism330dlc_xl_flag_data_ready_get(&(pObj->Ctx), Status) != ISM330DLC_OK)
  {
    return ISM330DLC_ERROR;
  }

  return ISM330DLC_OK;
}

/**
 * @brief  Get the ISM330DLC ACC initialization status
 * @param  pObj the device pObj
 * @param  Status 1 if initialized, 0 otherwise
 * @retval 0 in case of success, an error code otherwise
 */
int32_t ISM330DLC_ACC_Get_Init_Status(ISM330DLC_Object_t *pObj, uint8_t *Status)
{
  if (pObj == NULL)
  {
    return ISM330DLC_ERROR;
  }

  *Status = pObj->is_initialized;

  return ISM330DLC_OK;
}

/**
 * @brief  Set HP filter
 * @param  pObj the device pObj
 * @param  CutOff frequency
 * @retval 0 in case of success, an error code otherwise
 */
int32_t ISM330DLC_ACC_Enable_HP_Filter(ISM330DLC_Object_t *pObj, ism330dlc_hpcf_xl_t CutOff)
{

  if (ism330dlc_xl_hp_bandwidth_set(&(pObj->Ctx), CutOff) != ISM330DLC_OK)
  {
    return ISM330DLC_ERROR;
  }
  return ISM330DLC_OK;
}

/**
 * @brief  Set self test
 * @param  pObj the device pObj
 * @param  val the value of st_xl in reg CTRL5_C
 * @retval 0 in case of success, an error code otherwise
 */
int32_t ISM330DLC_GYRO_Set_SelfTest(ISM330DLC_Object_t *pObj, uint8_t val)
{
  ism330dlc_st_g_t reg;

  reg = (val == 0U)  ? ISM330DLC_GY_ST_DISABLE
        : (val == 1U)  ? ISM330DLC_GY_ST_POSITIVE
        : (val == 3U)  ? ISM330DLC_GY_ST_NEGATIVE
        :                ISM330DLC_GY_ST_DISABLE;

  if (ism330dlc_gy_self_test_set(&(pObj->Ctx), reg) != ISM330DLC_OK)
  {
    return ISM330DLC_ERROR;
  }

  return ISM330DLC_OK;
}

/**
 * @brief  Get the ISM330DLC GYRO data ready bit value
 * @param  pObj the device pObj
 * @param  Status the status of data ready bit
 * @retval 0 in case of success, an error code otherwise
 */
int32_t ISM330DLC_GYRO_Get_DRDY_Status(ISM330DLC_Object_t *pObj, uint8_t *Status)
{
  if (ism330dlc_gy_flag_data_ready_get(&(pObj->Ctx), Status) != ISM330DLC_OK)
  {
    return ISM330DLC_ERROR;
  }

  return ISM330DLC_OK;
}

/**
 * @brief  Get the ISM330DLC GYRO initialization status
 * @param  pObj the device pObj
 * @param  Status 1 if initialized, 0 otherwise
 * @retval 0 in case of success, an error code otherwise
 */
int32_t ISM330DLC_GYRO_Get_Init_Status(ISM330DLC_Object_t *pObj, uint8_t *Status)
{
  if (pObj == NULL)
  {
    return ISM330DLC_ERROR;
  }

  *Status = pObj->is_initialized;

  return ISM330DLC_OK;
}

/**
 * @brief  Get the ISM330DLC FIFO number of samples
 * @param  pObj the device pObj
 * @param  NumSamples number of samples
 * @retval 0 in case of success, an error code otherwise
 */
int32_t ISM330DLC_FIFO_Get_Num_Samples(ISM330DLC_Object_t *pObj, uint16_t *NumSamples)
{
  if (ism330dlc_fifo_data_level_get(&(pObj->Ctx), NumSamples) != ISM330DLC_OK)
  {
    return ISM330DLC_ERROR;
  }

  return ISM330DLC_OK;
}

/**
 * @brief  Get the ISM330DLC FIFO full status
 * @param  pObj the device pObj
 * @param  Status FIFO full status
 * @retval 0 in case of success, an error code otherwise
 */
int32_t ISM330DLC_FIFO_Get_Full_Status(ISM330DLC_Object_t *pObj, uint8_t *Status)
{
  ism330dlc_reg_t reg;

  if (ism330dlc_read_reg(&(pObj->Ctx), ISM330DLC_FIFO_STATUS2, &reg.byte, 1) != ISM330DLC_OK)
  {
    return ISM330DLC_ERROR;
  }

  *Status = reg.fifo_status2.fifo_full_smart;

  return ISM330DLC_OK;
}

/**
 * @brief  Set the ISM330DLC FIFO ODR value
 * @param  pObj the device pObj
 * @param  Odr FIFO ODR value
 * @retval 0 in case of success, an error code otherwise
 */
int32_t ISM330DLC_FIFO_Set_ODR_Value(ISM330DLC_Object_t *pObj, float Odr)
{
  ism330dlc_odr_fifo_t new_odr;

  new_odr = (Odr <=   12.5f) ? ISM330DLC_FIFO_12Hz5
            : (Odr <=   26.0f) ? ISM330DLC_FIFO_26Hz
            : (Odr <=   52.0f) ? ISM330DLC_FIFO_52Hz
            : (Odr <=  104.0f) ? ISM330DLC_FIFO_104Hz
            : (Odr <=  208.0f) ? ISM330DLC_FIFO_208Hz
            : (Odr <=  416.0f) ? ISM330DLC_FIFO_416Hz
            : (Odr <=  833.0f) ? ISM330DLC_FIFO_833Hz
            : (Odr <= 1660.0f) ? ISM330DLC_FIFO_1k66Hz
            : (Odr <= 3330.0f) ? ISM330DLC_FIFO_3k33Hz
            :                    ISM330DLC_FIFO_6k66Hz;

  if (ism330dlc_fifo_data_rate_set(&(pObj->Ctx), new_odr) != ISM330DLC_OK)
  {
    return ISM330DLC_ERROR;
  }

  return ISM330DLC_OK;
}

/**
 * @brief  Set the ISM330DLC FIFO full interrupt on INT1 pin
 * @param  pObj the device pObj
 * @param  Status FIFO full interrupt on INT1 pin status
 * @retval 0 in case of success, an error code otherwise
 */
int32_t ISM330DLC_FIFO_Set_INT1_FIFO_Full(ISM330DLC_Object_t *pObj, uint8_t Status)
{
  ism330dlc_reg_t reg;

  if (ism330dlc_read_reg(&(pObj->Ctx), ISM330DLC_INT1_CTRL, &reg.byte, 1) != ISM330DLC_OK)
  {
    return ISM330DLC_ERROR;
  }

  reg.int1_ctrl.int1_full_flag = Status;

  if (ism330dlc_write_reg(&(pObj->Ctx), ISM330DLC_INT1_CTRL, &reg.byte, 1) != ISM330DLC_OK)
  {
    return ISM330DLC_ERROR;
  }

  return ISM330DLC_OK;
}

/**
 * @brief  Set the ISM330DLC FIFO full interrupt on INT2 pin
 * @param  pObj the device pObj
 * @param  Status FIFO full interrupt on INT1 pin status
 * @retval 0 in case of success, an error code otherwise
 */
int32_t ISM330DLC_FIFO_Set_INT2_FIFO_Full(ISM330DLC_Object_t *pObj, uint8_t Status)
{
  ism330dlc_reg_t reg;

  if (ism330dlc_read_reg(&(pObj->Ctx), ISM330DLC_INT2_CTRL, &reg.byte, 1) != ISM330DLC_OK)
  {
    return ISM330DLC_ERROR;
  }

  reg.int1_ctrl.int1_full_flag = Status;

  if (ism330dlc_write_reg(&(pObj->Ctx), ISM330DLC_INT2_CTRL, &reg.byte, 1) != ISM330DLC_OK)
  {
    return ISM330DLC_ERROR;
  }

  return ISM330DLC_OK;
}

/**
 * @brief  Set the ISM330DLC FIFO watermark level
 * @param  pObj the device pObj
 * @param  Watermark FIFO watermark level
 * @retval 0 in case of success, an error code otherwise
 */
int32_t ISM330DLC_FIFO_Set_Watermark_Level(ISM330DLC_Object_t *pObj, uint16_t Watermark)
{
  if (ism330dlc_fifo_watermark_set(&(pObj->Ctx), Watermark) != ISM330DLC_OK)
  {
    return ISM330DLC_ERROR;
  }

  return ISM330DLC_OK;
}

/**
 * @brief  Set the ISM330DLC FIFO stop on watermark
 * @param  pObj the device pObj
 * @param  Status FIFO stop on watermark status
 * @retval 0 in case of success, an error code otherwise
 */
int32_t ISM330DLC_FIFO_Set_Stop_On_Fth(ISM330DLC_Object_t *pObj, uint8_t Status)
{
  if (ism330dlc_fifo_stop_on_wtm_set(&(pObj->Ctx), Status) != ISM330DLC_OK)
  {
    return ISM330DLC_ERROR;
  }

  return ISM330DLC_OK;
}

/**
 * @brief  Set the ISM330DLC FIFO mode
 * @param  pObj the device pObj
 * @param  Mode FIFO mode
 * @retval 0 in case of success, an error code otherwise
 */
int32_t ISM330DLC_FIFO_Set_Mode(ISM330DLC_Object_t *pObj, uint8_t Mode)
{
  int32_t ret = ISM330DLC_OK;

  /* Verify that the passed parameter contains one of the valid values. */
  switch ((ism330dlc_fifo_mode_t)Mode)
  {
    case ISM330DLC_BYPASS_MODE:
    case ISM330DLC_FIFO_MODE:
    case ISM330DLC_STREAM_TO_FIFO_MODE:
    case ISM330DLC_BYPASS_TO_STREAM_MODE:
    case ISM330DLC_STREAM_MODE:
      break;

    default:
      ret = ISM330DLC_ERROR;
      break;
  }

  if (ret == ISM330DLC_ERROR)
  {
    return ret;
  }

  if (ism330dlc_fifo_mode_set(&(pObj->Ctx), (ism330dlc_fifo_mode_t)Mode) != ISM330DLC_OK)
  {
    return ISM330DLC_ERROR;
  }

  return ret;
}

/**
 * @brief  Get the ISM330DLC FIFO pattern
 * @param  pObj the device pObj
 * @param  Pattern FIFO pattern
 * @retval 0 in case of success, an error code otherwise
 */
int32_t ISM330DLC_FIFO_Get_Pattern(ISM330DLC_Object_t *pObj, uint16_t *Pattern)
{
  if (ism330dlc_fifo_pattern_get(&(pObj->Ctx), Pattern) != ISM330DLC_OK)
  {
    return ISM330DLC_ERROR;
  }

  return ISM330DLC_OK;
}

/**
 * @brief  Get the ISM330DLC FIFO raw data
 * @param  pObj the device pObj
 * @param  Data FIFO raw data array [2]
 * @retval 0 in case of success, an error code otherwise
 */
int32_t ISM330DLC_FIFO_Get_Data(ISM330DLC_Object_t *pObj, uint8_t *Data)
{
  if (ism330dlc_read_reg(&(pObj->Ctx), ISM330DLC_FIFO_DATA_OUT_L, Data, 2) != ISM330DLC_OK)
  {
    return ISM330DLC_ERROR;
  }

  return ISM330DLC_OK;
}

/**
 * @brief  Set the ISM330DLC FIFO full interrupt on INT1 pin
 * @param  pObj the device pObj
 * @param  Status FIFO full interrupt on INT1 pin status
 * @retval 0 in case of success, an error code otherwise
 */
int32_t ISM330DLC_FIFO_Full_Set_INT1(ISM330DLC_Object_t *pObj, uint8_t Status)
{
  ism330dlc_reg_t reg;

  if (ism330dlc_read_reg(&(pObj->Ctx), ISM330DLC_INT1_CTRL, &reg.byte, 1) != ISM330DLC_OK)
  {
    return ISM330DLC_ERROR;
  }

  reg.int1_ctrl.int1_full_flag = Status;

  if (ism330dlc_write_reg(&(pObj->Ctx), ISM330DLC_INT1_CTRL, &reg.byte, 1) != ISM330DLC_OK)
  {
    return ISM330DLC_ERROR;
  }

  return ISM330DLC_OK;
}

/**
 * @brief  Set the ISM330DLC FIFO accelero decimation
 * @param  pObj the device pObj
 * @param  Decimation FIFO accelero decimation
 * @retval 0 in case of success, an error code otherwise
 */
int32_t ISM330DLC_FIFO_ACC_Set_Decimation(ISM330DLC_Object_t *pObj, uint8_t Decimation)
{
  int32_t ret = ISM330DLC_OK;

  /* Verify that the passed parameter contains one of the valid values. */
  switch ((ism330dlc_dec_fifo_xl_t)Decimation)
  {
    case ISM330DLC_FIFO_XL_DISABLE:
    case ISM330DLC_FIFO_XL_NO_DEC:
    case ISM330DLC_FIFO_XL_DEC_2:
    case ISM330DLC_FIFO_XL_DEC_3:
    case ISM330DLC_FIFO_XL_DEC_4:
    case ISM330DLC_FIFO_XL_DEC_8:
    case ISM330DLC_FIFO_XL_DEC_16:
    case ISM330DLC_FIFO_XL_DEC_32:
      break;

    default:
      ret = ISM330DLC_ERROR;
      break;
  }

  if (ret == ISM330DLC_ERROR)
  {
    return ret;
  }

  if (ism330dlc_fifo_xl_batch_set(&(pObj->Ctx), (ism330dlc_dec_fifo_xl_t)Decimation) != ISM330DLC_OK)
  {
    return ISM330DLC_ERROR;
  }

  return ret;
}

/**
 * @brief  Get the ISM330DLC FIFO accelero single sample (16-bit data) and calculate acceleration [mg]
 * @param  pObj the device pObj
 * @param  Acceleration FIFO single accelero axis [mg]
 * @retval 0 in case of success, an error code otherwise
 */
int32_t ISM330DLC_FIFO_ACC_Get_Axis(ISM330DLC_Object_t *pObj, int32_t *Acceleration)
{
  uint8_t data[2];
  int16_t data_raw;
  float sensitivity = 0.0f;
  float acceleration_float;

  if (ISM330DLC_FIFO_Get_Data(pObj, data) != ISM330DLC_OK)
  {
    return ISM330DLC_ERROR;
  }

  data_raw = ((int16_t)data[1] << 8) | data[0];

  if (ISM330DLC_ACC_GetSensitivity(pObj, &sensitivity) != ISM330DLC_OK)
  {
    return ISM330DLC_ERROR;
  }

  acceleration_float = (float)data_raw * sensitivity;
  *Acceleration = (int32_t)acceleration_float;

  return ISM330DLC_OK;
}

/**
 * @brief  Get the ISM330DLC FIFO accelero single word (16-bit data)
 * @param  pObj the device pObj
 * @param  Acceleration FIFO single data
 * @retval 0 in case of success, an error code otherwise
 */
int32_t ISM330DLC_FIFO_Get_Data_Word(ISM330DLC_Object_t *pObj, int16_t *data_raw)
{
  uint8_t data[2];
  
  if (ISM330DLC_FIFO_Get_Data(pObj, data) != ISM330DLC_OK)
  {
    return ISM330DLC_ERROR;
  }

  *data_raw = ((((int16_t)data[1])<<8)+(int16_t)data[0]);

  return ISM330DLC_OK;
}

/**
 * @brief  Set the ISM330DLC FIFO gyro decimation
 * @param  pObj the device pObj
 * @param  Decimation FIFO gyro decimation
 * @retval 0 in case of success, an error code otherwise
 */
int32_t ISM330DLC_FIFO_GYRO_Set_Decimation(ISM330DLC_Object_t *pObj, uint8_t Decimation)
{
  int32_t ret = ISM330DLC_OK;

  /* Verify that the passed parameter contains one of the valid values. */
  switch ((ism330dlc_dec_fifo_gyro_t)Decimation)
  {
    case ISM330DLC_FIFO_GY_DISABLE:
    case ISM330DLC_FIFO_GY_NO_DEC:
    case ISM330DLC_FIFO_GY_DEC_2:
    case ISM330DLC_FIFO_GY_DEC_3:
    case ISM330DLC_FIFO_GY_DEC_4:
    case ISM330DLC_FIFO_GY_DEC_8:
    case ISM330DLC_FIFO_GY_DEC_16:
    case ISM330DLC_FIFO_GY_DEC_32:
      break;

    default:
      ret = ISM330DLC_ERROR;
      break;
  }

  if (ret == ISM330DLC_ERROR)
  {
    return ret;
  }

  if (ism330dlc_fifo_gy_batch_set(&(pObj->Ctx), (ism330dlc_dec_fifo_gyro_t)Decimation) != ISM330DLC_OK)
  {
    return ISM330DLC_ERROR;
  }

  return ret;
}

/**
 * @brief  Get the ISM330DLC FIFO gyro single sample (16-bit data) and calculate angular velocity [mDPS]
 * @param  pObj the device pObj
 * @param  AngularVelocity FIFO single gyro axis [mDPS]
 * @retval 0 in case of success, an error code otherwise
 */
int32_t ISM330DLC_FIFO_GYRO_Get_Axis(ISM330DLC_Object_t *pObj, int32_t *AngularVelocity)
{
  uint8_t data[2];
  int16_t data_raw;
  float sensitivity = 0.0f;
  float angular_velocity_float;

  if (ISM330DLC_FIFO_Get_Data(pObj, data) != ISM330DLC_OK)
  {
    return ISM330DLC_ERROR;
  }

  data_raw = ((int16_t)data[1] << 8) | data[0];

  if (ISM330DLC_GYRO_GetSensitivity(pObj, &sensitivity) != ISM330DLC_OK)
  {
    return ISM330DLC_ERROR;
  }

  angular_velocity_float = (float)data_raw * sensitivity;
  *AngularVelocity = (int32_t)angular_velocity_float;

  return ISM330DLC_OK;
}

/**
 * @}
 */

/** @defgroup ISM330DLC_Private_Functions ISM330DLC Private Functions
 * @{
 */

/**
 * @brief  Set the ISM330DLC accelerometer sensor output data rate when enabled
 * @param  pObj the device pObj
 * @param  Odr the functional output data rate to be set
 * @retval 0 in case of success, an error code otherwise
 */
static int32_t ISM330DLC_ACC_SetOutputDataRate_When_Enabled(ISM330DLC_Object_t *pObj, float Odr)
{
  ism330dlc_odr_xl_t new_odr;

  new_odr = (Odr <=   12.5f) ? ISM330DLC_XL_ODR_12Hz5
          : (Odr <=   26.0f) ? ISM330DLC_XL_ODR_26Hz
          : (Odr <=   52.0f) ? ISM330DLC_XL_ODR_52Hz
          : (Odr <=  104.0f) ? ISM330DLC_XL_ODR_104Hz
          : (Odr <=  208.0f) ? ISM330DLC_XL_ODR_208Hz
          : (Odr <=  416.0f) ? ISM330DLC_XL_ODR_416Hz
          : (Odr <=  833.0f) ? ISM330DLC_XL_ODR_833Hz
          : (Odr <= 1660.0f) ? ISM330DLC_XL_ODR_1k66Hz
          : (Odr <= 3330.0f) ? ISM330DLC_XL_ODR_3k33Hz
          :                    ISM330DLC_XL_ODR_6k66Hz;

  /* Output data rate selection. */
  if (ism330dlc_xl_data_rate_set(&(pObj->Ctx), new_odr) != ISM330DLC_OK)
  {
    return ISM330DLC_ERROR;
  }

  return ISM330DLC_OK;
}

/**
 * @brief  Set the ISM330DLC accelerometer sensor output data rate when disabled
 * @param  pObj the device pObj
 * @param  Odr the functional output data rate to be set
 * @retval 0 in case of success, an error code otherwise
 */
static int32_t ISM330DLC_ACC_SetOutputDataRate_When_Disabled(ISM330DLC_Object_t *pObj, float Odr)
{
  pObj->acc_odr = (Odr <=   12.5f) ? ISM330DLC_XL_ODR_12Hz5
                : (Odr <=   26.0f) ? ISM330DLC_XL_ODR_26Hz
                : (Odr <=   52.0f) ? ISM330DLC_XL_ODR_52Hz
                : (Odr <=  104.0f) ? ISM330DLC_XL_ODR_104Hz
                : (Odr <=  208.0f) ? ISM330DLC_XL_ODR_208Hz
                : (Odr <=  416.0f) ? ISM330DLC_XL_ODR_416Hz
                : (Odr <=  833.0f) ? ISM330DLC_XL_ODR_833Hz
                : (Odr <= 1660.0f) ? ISM330DLC_XL_ODR_1k66Hz
                : (Odr <= 3330.0f) ? ISM330DLC_XL_ODR_3k33Hz
                :                    ISM330DLC_XL_ODR_6k66Hz;

  return ISM330DLC_OK;
}

/**
 * @brief  Set the ISM330DLC gyroscope sensor output data rate when enabled
 * @param  pObj the device pObj
 * @param  Odr the functional output data rate to be set
 * @retval 0 in case of success, an error code otherwise
 */
static int32_t ISM330DLC_GYRO_SetOutputDataRate_When_Enabled(ISM330DLC_Object_t *pObj, float Odr)
{
  ism330dlc_odr_g_t new_odr;

  new_odr = (Odr <=   12.5f) ? ISM330DLC_GY_ODR_12Hz5
          : (Odr <=   26.0f) ? ISM330DLC_GY_ODR_26Hz
          : (Odr <=   52.0f) ? ISM330DLC_GY_ODR_52Hz
          : (Odr <=  104.0f) ? ISM330DLC_GY_ODR_104Hz
          : (Odr <=  208.0f) ? ISM330DLC_GY_ODR_208Hz
          : (Odr <=  416.0f) ? ISM330DLC_GY_ODR_416Hz
          : (Odr <=  833.0f) ? ISM330DLC_GY_ODR_833Hz
          : (Odr <= 1660.0f) ? ISM330DLC_GY_ODR_1k66Hz
          : (Odr <= 3330.0f) ? ISM330DLC_GY_ODR_3k33Hz
          :                    ISM330DLC_GY_ODR_6k66Hz;

  /* Output data rate selection. */
  if (ism330dlc_gy_data_rate_set(&(pObj->Ctx), new_odr) != ISM330DLC_OK)
  {
    return ISM330DLC_ERROR;
  }

  return ISM330DLC_OK;
}

/**
 * @brief  Set the ISM330DLC gyroscope sensor output data rate when disabled
 * @param  pObj the device pObj
 * @param  Odr the functional output data rate to be set
 * @retval 0 in case of success, an error code otherwise
 */
static int32_t ISM330DLC_GYRO_SetOutputDataRate_When_Disabled(ISM330DLC_Object_t *pObj, float Odr)
{
  pObj->gyro_odr = (Odr <=   12.5f) ? ISM330DLC_GY_ODR_12Hz5
                 : (Odr <=   26.0f) ? ISM330DLC_GY_ODR_26Hz
                 : (Odr <=   52.0f) ? ISM330DLC_GY_ODR_52Hz
                 : (Odr <=  104.0f) ? ISM330DLC_GY_ODR_104Hz
                 : (Odr <=  208.0f) ? ISM330DLC_GY_ODR_208Hz
                 : (Odr <=  416.0f) ? ISM330DLC_GY_ODR_416Hz
                 : (Odr <=  833.0f) ? ISM330DLC_GY_ODR_833Hz
                 : (Odr <= 1660.0f) ? ISM330DLC_GY_ODR_1k66Hz
                 : (Odr <= 3330.0f) ? ISM330DLC_GY_ODR_3k33Hz
                 :                    ISM330DLC_GY_ODR_6k66Hz;

  return ISM330DLC_OK;
}

/**
 * @brief  Set the ISM330DLC FIFO full interrupt on INT2 pin
 * @param  pObj the device pObj
 * @param  Status FIFO full interrupt on INT2 pin status
 * @retval 0 in case of success, an error code otherwise
 */
int32_t ISM330DLC_FIFO_Set_INT2_Drdy(ISM330DLC_Object_t *pObj, uint8_t Status)
{
  ism330dlc_reg_t reg;

  if (ism330dlc_read_reg(&(pObj->Ctx), ISM330DLC_INT2_CTRL, &reg.byte, 1) != ISM330DLC_OK)
  {
    return ISM330DLC_ERROR;
  }

  reg.int1_ctrl.int1_full_flag = Status;

  if (ism330dlc_write_reg(&(pObj->Ctx), ISM330DLC_INT2_CTRL, &reg.byte, 1) != ISM330DLC_OK)
  {
    return ISM330DLC_ERROR;
  }

  return ISM330DLC_OK;
}

/**
 * @brief  Set the ISM330DLC FIFO full interrupt on INT2 pin
 * @param  pObj the device pObj
 * @param  Status DRDY interrupt on INT2 pin status
 * @retval 0 in case of success, an error code otherwise
 */
int32_t ISM330DLC_Set_INT2_Drdy(ISM330DLC_Object_t *pObj, uint8_t Status)
{
  ism330dlc_reg_t reg;

  if (ism330dlc_read_reg(&(pObj->Ctx), ISM330DLC_INT2_CTRL, &reg.byte, 1) != ISM330DLC_OK)
  {
    return ISM330DLC_ERROR;
  }

  reg.int1_ctrl.int1_drdy_xl = Status;

  if (ism330dlc_write_reg(&(pObj->Ctx), ISM330DLC_INT2_CTRL, &reg.byte, 1) != ISM330DLC_OK)
  {
    return ISM330DLC_ERROR;
  }

  return ISM330DLC_OK;
}


/**
 * @brief  Wrap Read register component function to Bus IO function
 * @param  Handle the device handler
 * @param  Reg the register address
 * @param  pData the stored data pointer
 * @param  Length the length
 * @retval 0 in case of success, an error code otherwise
 */
static int32_t ReadRegWrap(void *Handle, uint8_t Reg, uint8_t *pData, uint16_t Length)
{
  ISM330DLC_Object_t *pObj = (ISM330DLC_Object_t *)Handle;

  return pObj->IO.ReadReg(pObj->IO.Address, Reg, pData, Length);
}

/**
 * @brief  Wrap Write register component function to Bus IO function
 * @param  Handle the device handler
 * @param  Reg the register address
 * @param  pData the stored data pointer
 * @param  Length the length
 * @retval 0 in case of success, an error code otherwise
 */
static int32_t WriteRegWrap(void *Handle, uint8_t Reg, uint8_t *pData, uint16_t Length)
{
  ISM330DLC_Object_t *pObj = (ISM330DLC_Object_t *)Handle;

  return pObj->IO.WriteReg(pObj->IO.Address, Reg, pData, Length);
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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
