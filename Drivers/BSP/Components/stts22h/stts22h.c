/**
  ******************************************************************************
  * @file    stts22h.c
  * @author  MEMS Software Solutions Team
  * @brief   STTS22H driver file
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

/* Includes ------------------------------------------------------------------*/
#include "stts22h.h"

/** @addtogroup BSP BSP
  * @{
  */

/** @addtogroup Component Component
  * @{
  */

/** @defgroup STTS22H STTS22H
  * @{
  */

/** @defgroup STTS22H_Exported_Variables STTS22H Exported Variables
  * @{
  */

STTS22H_CommonDrv_t STTS22H_COMMON_Driver =
{
  STTS22H_Init,
  STTS22H_DeInit,
  STTS22H_ReadID,
  STTS22H_GetCapabilities,
};

STTS22H_TEMP_Drv_t STTS22H_TEMP_Driver =
{
  STTS22H_TEMP_Enable,
  STTS22H_TEMP_Disable,
  STTS22H_TEMP_GetOutputDataRate,
  STTS22H_TEMP_SetOutputDataRate,
  STTS22H_TEMP_GetTemperature,
};

/**
  * @}
  */

/** @defgroup STTS22H_Private_Function_Prototypes STTS22H Private Function Prototypes
  * @{
  */

static int32_t ReadRegWrap(void *Handle, uint8_t Reg, uint8_t *pData, uint16_t Length);
static int32_t WriteRegWrap(void *Handle, uint8_t Reg, uint8_t *pData, uint16_t Length);

/**
  * @}
  */

/** @defgroup STTS22H_Exported_Functions STTS22H Exported Functions
  * @{
  */

/**
  * @brief  Register Component Bus IO operations
  * @param  pObj the device pObj
  * @retval 0 in case of success, an error code otherwise
  */
int32_t STTS22H_RegisterBusIO(STTS22H_Object_t *pObj, STTS22H_IO_t *pIO)
{
  int32_t ret;

  if (pObj == NULL)
  {
    ret = STTS22H_ERROR;
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

    if (pObj->IO.Init != NULL)
    {
      ret = pObj->IO.Init();
    }
    else
    {
      ret = STTS22H_ERROR;
    }
  }

  return ret;
}

/**
  * @brief  Initialize the STTS22H sensor
  * @param  pObj the device pObj
  * @retval 0 in case of success, an error code otherwise
  */
int32_t STTS22H_Init(STTS22H_Object_t *pObj)
{
  if (pObj->is_initialized == 0U)
  {
    /* Set default ODR */
    pObj->temp_odr = 1.0f;

    /* Enable BDU */
    if (stts22h_block_data_update_set(&(pObj->Ctx), PROPERTY_ENABLE) != STTS22H_OK)
    {
      return STTS22H_ERROR;
    }

    /* Enable Automatic Address Increment */
    if (stts22h_auto_increment_set(&(pObj->Ctx), PROPERTY_ENABLE) != STTS22H_OK)
    {
      return STTS22H_ERROR;
    }

    /* Put the component in standby mode. */
    if (stts22h_temp_data_rate_set(&(pObj->Ctx), STTS22H_POWER_DOWN) != STTS22H_OK)
    {
      return STTS22H_ERROR;
    }
  }

  pObj->is_initialized = 1;

  return STTS22H_OK;
}

/**
  * @brief  Deinitialize the STTS22H sensor
  * @param  pObj the device pObj
  * @retval 0 in case of success, an error code otherwise
  */
int32_t STTS22H_DeInit(STTS22H_Object_t *pObj)
{
  if (pObj->is_initialized == 1U)
  {
    /* Put the component in standby mode */
    if (STTS22H_TEMP_Disable(pObj) != STTS22H_OK)
    {
      return STTS22H_ERROR;
    }
  }

  pObj->is_initialized = 0;

  return STTS22H_OK;
}

/**
  * @brief  Get WHO_AM_I value
  * @param  pObj the device pObj
  * @param  Id the WHO_AM_I value
  * @retval 0 in case of success, an error code otherwise
  */
int32_t STTS22H_ReadID(STTS22H_Object_t *pObj, uint8_t *Id)
{
  uint8_t buf;

  if (stts22h_dev_id_get(&(pObj->Ctx), &buf) != STTS22H_OK)
  {
    return STTS22H_ERROR;
  }

  *Id = buf;

  return STTS22H_OK;
}

/**
  * @brief  Get STTS22H sensor capabilities
  * @param  pObj Component object pointer
  * @param  Capabilities pointer to STTS22H sensor capabilities
  * @retval 0 in case of success, an error code otherwise
  */
int32_t STTS22H_GetCapabilities(STTS22H_Object_t *pObj, STTS22H_Capabilities_t *Capabilities)
{
  /* Prevent unused argument(s) compilation warning */
  (void)(pObj);

  Capabilities->Humidity    = 0;
  Capabilities->Pressure    = 0;
  Capabilities->Temperature = 1;
  Capabilities->LowPower    = 0;
  Capabilities->HumMaxOdr   = 0.0f;
  Capabilities->TempMaxOdr  = 200.0f;
  Capabilities->PressMaxOdr = 0.0f;
  return STTS22H_OK;
}

/**
  * @brief  Get the STTS22H initialization status
  * @param  pObj the device pObj
  * @param  Status 1 if initialized, 0 otherwise
  * @retval 0 in case of success, an error code otherwise
  */
int32_t STTS22H_Get_Init_Status(STTS22H_Object_t *pObj, uint8_t *Status)
{
  if (pObj == NULL)
  {
    return STTS22H_ERROR;
  }

  *Status = pObj->is_initialized;

  return STTS22H_OK;
}

/**
  * @brief  Enable the STTS22H temperature sensor
  * @param  pObj the device pObj
  * @retval 0 in case of success, an error code otherwise
  */
int32_t STTS22H_TEMP_Enable(STTS22H_Object_t *pObj)
{
  /* Check if the component is already enabled */
  if (pObj->temp_is_enabled == 1U)
  {
    return STTS22H_OK;
  }

  /* Power on the component and set the odr. */
  if (STTS22H_TEMP_SetOutputDataRate(pObj, pObj->temp_odr) != STTS22H_OK)
  {
    return STTS22H_ERROR;
  }

  pObj->temp_is_enabled = 1;

  return STTS22H_OK;
}

/**
  * @brief  Disable the STTS22H temperature sensor
  * @param  pObj the device pObj
  * @retval 0 in case of success, an error code otherwise
  */
int32_t STTS22H_TEMP_Disable(STTS22H_Object_t *pObj)
{
  /* Check if the component is already disabled */
  if (pObj->temp_is_enabled == 0U)
  {
    return STTS22H_OK;
  }

  /* Save the current odr. */
  if (STTS22H_TEMP_GetOutputDataRate(pObj, &pObj->temp_odr) != STTS22H_OK)
  {
    return STTS22H_ERROR;
  }

  /* Put the component in standby mode. */
  if (stts22h_temp_data_rate_set(&(pObj->Ctx), STTS22H_POWER_DOWN) != STTS22H_OK)
  {
    return STTS22H_ERROR;
  }

  pObj->temp_is_enabled = 0;

  return STTS22H_OK;
}

/**
  * @brief  Get the STTS22H temperature sensor output data rate
  * @param  pObj the device pObj
  * @param  Odr pointer where the output data rate is written
  * @retval 0 in case of success, an error code otherwise
  */
int32_t STTS22H_TEMP_GetOutputDataRate(STTS22H_Object_t *pObj, float *Odr)
{
  int32_t ret = STTS22H_OK;
  stts22h_odr_temp_t odr_low_level;

  if (stts22h_temp_data_rate_get(&(pObj->Ctx), &odr_low_level) != STTS22H_OK)
  {
    return STTS22H_ERROR;
  }

  switch (odr_low_level)
  {
    case STTS22H_POWER_DOWN:
    case STTS22H_ONE_SHOT:
      *Odr = 0.0f;
      break;

    case STTS22H_1Hz:
      *Odr = 1.0f;
      break;

    case STTS22H_25Hz:
      *Odr = 25.0f;
      break;

    case STTS22H_50Hz:
      *Odr = 50.0f;
      break;

    case STTS22H_100Hz:
      *Odr = 100.0f;
      break;

    case STTS22H_200Hz:
      *Odr = 200.0f;
      break;

    default:
      ret = STTS22H_ERROR;
      break;
  }

  return ret;
}

/**
  * @brief  Set the STTS22H temperature sensor output data rate
  * @param  pObj the device pObj
  * @param  Odr the output data rate value to be set
  * @retval 0 in case of success, an error code otherwise
  */
int32_t STTS22H_TEMP_SetOutputDataRate(STTS22H_Object_t *pObj, float Odr)
{
  stts22h_odr_temp_t new_odr;

  new_odr = (Odr <= 1.0f) ? STTS22H_1Hz
            : (Odr <= 25.0f) ? STTS22H_25Hz
            : (Odr <= 50.0f) ? STTS22H_50Hz
            : (Odr <= 100.0f) ? STTS22H_100Hz
            :                    STTS22H_200Hz;

  if (stts22h_temp_data_rate_set(&(pObj->Ctx), new_odr) != STTS22H_OK)
  {
    return STTS22H_ERROR;
  }

  return STTS22H_OK;
}

/**
  * @brief  Get the STTS22H temperature value
  * @param  pObj the device pObj
  * @param  Value pointer where the temperature value is written
  * @retval 0 in case of success, an error code otherwise
  */
int32_t STTS22H_TEMP_GetTemperature(STTS22H_Object_t *pObj, float *Value)
{
  int16_t raw_value;

  /* Get the temperature */
  if (stts22h_temperature_raw_get(&(pObj->Ctx), &raw_value) != STTS22H_OK)
  {
    return STTS22H_ERROR;
  }

  *Value = stts22h_from_lsb_to_celsius(raw_value);

  return STTS22H_OK;
}

/**
  * @brief  Get the STTS22H temperature data ready bit value
  * @param  pObj the device pObj
  * @param  Status the status of data ready bit
  * @retval 0 in case of success, an error code otherwise
  */
int32_t STTS22H_TEMP_Get_DRDY_Status(STTS22H_Object_t *pObj, uint8_t *Status)
{
  uint8_t val;

  if (stts22h_temp_flag_data_ready_get(&(pObj->Ctx), &val) != STTS22H_OK)
  {
    return STTS22H_ERROR;
  }

  if (val)
  {
    *Status = 1;
  }
  else
  {
    *Status = 0;
  }

  return STTS22H_OK;
}

/**
  * @brief  Set the STTS22H high temperature threshold value
  * @param  pObj the device pObj
  * @param  Value the high temperature threshold to be set
  * @retval 0 in case of success, an error code otherwise
  */
int32_t STTS22H_TEMP_SetHighTemperatureThreshold(STTS22H_Object_t *pObj, float Value)
{
  uint8_t raw_value;

  raw_value = (uint8_t)((Value / 0.64f) + 63.0f);

  /* Set the temperature threshold */
  if (stts22h_temp_trshld_high_set(&(pObj->Ctx), raw_value) != STTS22H_OK)
  {
    return STTS22H_ERROR;
  }

  return STTS22H_OK;
}

/**
  * @brief  Set the STTS22H low temperature threshold value
  * @param  pObj the device pObj
  * @param  Value the high temperature threshold to be set
  * @retval 0 in case of success, an error code otherwise
  */
int32_t STTS22H_TEMP_SetLowTemperatureThreshold(STTS22H_Object_t *pObj, float Value)
{
  uint8_t raw_value;

  raw_value = (uint8_t)((Value / 0.64f) + 63.0f);

  /* Set the temperature threshold */
  if (stts22h_temp_trshld_low_set(&(pObj->Ctx), raw_value) != STTS22H_OK)
  {
    return STTS22H_ERROR;
  }

  return STTS22H_OK;
}

/**
  * @brief  Get the STTS22H temperature limits status
  * @param  pObj the device pObj
  * @param  HighLimit indicates that high temperature limit has been exceeded
  * @param  LowhLimit indicates that low temperature limit has been exceeded
  * @retval 0 in case of success, an error code otherwise
  */
int32_t STTS22H_TEMP_GetTemperatureLimitStatus(STTS22H_Object_t *pObj, uint8_t *HighLimit, uint8_t *LowLimit)
{
  stts22h_temp_trlhd_src_t status;

  /* Read status register */
  if (stts22h_temp_trshld_src_get(&(pObj->Ctx), &status) != STTS22H_OK)
  {
    return STTS22H_ERROR;
  }

  if (HighLimit != NULL)
  {
    *HighLimit = status.over_thh;
  }

  if (LowLimit != NULL)
  {
    *LowLimit = status.under_thl;
  }

  return STTS22H_OK;
}

/**
  * @brief  Get the STTS22H register value
  * @param  pObj the device pObj
  * @param  Reg address to be read
  * @param  Data pointer where the value is written
  * @retval 0 in case of success, an error code otherwise
  */
int32_t STTS22H_Read_Reg(STTS22H_Object_t *pObj, uint8_t Reg, uint8_t *Data)
{
  if (stts22h_read_reg(&(pObj->Ctx), Reg, Data, 1) != STTS22H_OK)
  {
    return STTS22H_ERROR;
  }

  return STTS22H_OK;
}

/**
  * @brief  Set the STTS22H register value
  * @param  pObj the device pObj
  * @param  Reg address to be written
  * @param  Data value to be written
  * @retval 0 in case of success, an error code otherwise
  */
int32_t STTS22H_Write_Reg(STTS22H_Object_t *pObj, uint8_t Reg, uint8_t Data)
{
  if (stts22h_write_reg(&(pObj->Ctx), Reg, &Data, 1) != STTS22H_OK)
  {
    return STTS22H_ERROR;
  }

  return STTS22H_OK;
}

/**
  * @brief  Set the STTS22H One Shot Mode
  * @param  pObj the device pObj
  * @retval 0 in case of success, an error code otherwise
  */
int32_t STTS22H_Set_One_Shot(STTS22H_Object_t *pObj)
{
  /* Start One Shot Measurement */
  if (stts22h_temp_data_rate_set(&(pObj->Ctx), STTS22H_ONE_SHOT) != STTS22H_OK)
  {
    return STTS22H_ERROR;
  }

  return STTS22H_OK;
}

/**
  * @brief  Get the STTS22H One Shot Status
  * @param  pObj the device pObj
  * @param  Status pointer to the one shot status (1 means measurements available, 0 means measurements not available yet)
  * @retval 0 in case of success, an error code otherwise
  */
int32_t STTS22H_Get_One_Shot_Status(STTS22H_Object_t *pObj, uint8_t *Status)
{
  stts22h_dev_status_t status;

  /* Get Busy flag */
  if (stts22h_dev_status_get(&(pObj->Ctx), &status) != STTS22H_OK)
  {
    return STTS22H_ERROR;
  }

  if (status.busy)
  {
    *Status = 0;
  }
  else
  {
    *Status = 1;
  }

  return STTS22H_OK;
}

/**
  * @}
  */

/** @defgroup STTS22H_Private_Functions STTS22H Private Functions
  * @{
  */

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
  uint16_t i;
  int32_t ret = STTS22H_OK;
  STTS22H_Object_t *pObj = (STTS22H_Object_t *)Handle;

  if (pObj->IO.BusType == (uint32_t)STTS22H_I2C_BUS) /* I2C */
  {
    for (i = 0; i < Length; i++)
    {
      ret = pObj->IO.ReadReg(pObj->IO.Address, (Reg + i), &pData[i], 1);
      if (ret != STTS22H_OK)
      {
        return STTS22H_ERROR;
      }
    }
  }

  return ret;
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
  uint16_t i;
  int32_t ret = STTS22H_OK;
  STTS22H_Object_t *pObj = (STTS22H_Object_t *)Handle;

  if (pObj->IO.BusType == (uint32_t)STTS22H_I2C_BUS) /* I2C */
  {
    for (i = 0; i < Length; i++)
    {
      ret = pObj->IO.WriteReg(pObj->IO.Address, (Reg + i), &pData[i], 1);
      if (ret != STTS22H_OK)
      {
        return STTS22H_ERROR;
      }
    }
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
