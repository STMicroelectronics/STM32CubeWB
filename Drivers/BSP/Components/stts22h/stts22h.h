/**
  ******************************************************************************
  * @file    stts22h.h
  * @author  MEMS Software Solutions Team
  * @brief   STTS22H header driver file
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
#ifndef STTS22H_H
#define STTS22H_H

#ifdef __cplusplus
extern "C"
{
#endif

/* Includes ------------------------------------------------------------------*/
#include "stts22h_reg.h"
#include <string.h>

/** @addtogroup BSP BSP
  * @{
  */

/** @addtogroup Component Component
  * @{
  */

/** @addtogroup STTS22H STTS22H
  * @{
  */

/** @defgroup STTS22H_Exported_Types STTS22H Exported Types
  * @{
  */

typedef int32_t (*STTS22H_Init_Func)(void);
typedef int32_t (*STTS22H_DeInit_Func)(void);
typedef int32_t (*STTS22H_GetTick_Func)(void);
typedef int32_t (*STTS22H_WriteReg_Func)(uint16_t, uint16_t, uint8_t *, uint16_t);
typedef int32_t (*STTS22H_ReadReg_Func)(uint16_t, uint16_t, uint8_t *, uint16_t);

typedef struct
{
  STTS22H_Init_Func          Init;
  STTS22H_DeInit_Func        DeInit;
  uint32_t                   BusType; /*0 means I2C */
  uint8_t                    Address;
  STTS22H_WriteReg_Func      WriteReg;
  STTS22H_ReadReg_Func       ReadReg;
  STTS22H_GetTick_Func       GetTick;
} STTS22H_IO_t;

typedef struct
{
  STTS22H_IO_t       IO;
  stmdev_ctx_t       Ctx;
  uint8_t            is_initialized;
  uint8_t            temp_is_enabled;
  float              temp_odr;
} STTS22H_Object_t;

typedef struct
{
  uint8_t Temperature;
  uint8_t Pressure;
  uint8_t Humidity;
  uint8_t LowPower;
  float   HumMaxOdr;
  float   TempMaxOdr;
  float   PressMaxOdr;
} STTS22H_Capabilities_t;

typedef struct
{
  int32_t (*Init)(STTS22H_Object_t *);
  int32_t (*DeInit)(STTS22H_Object_t *);
  int32_t (*ReadID)(STTS22H_Object_t *, uint8_t *);
  int32_t (*GetCapabilities)(STTS22H_Object_t *, STTS22H_Capabilities_t *);
} STTS22H_CommonDrv_t;

typedef struct
{
  int32_t (*Enable)(STTS22H_Object_t *);
  int32_t (*Disable)(STTS22H_Object_t *);
  int32_t (*GetOutputDataRate)(STTS22H_Object_t *, float *);
  int32_t (*SetOutputDataRate)(STTS22H_Object_t *, float);
  int32_t (*GetTemperature)(STTS22H_Object_t *, float *);
} STTS22H_TEMP_Drv_t;

/**
  * @}
  */

/** @defgroup STTS22H_Exported_Constants STTS22H Exported Constants
  * @{
  */
#define STTS22H_I2C_BUS           0U

/** STTS22H error codes  **/
#define STTS22H_OK                 0
#define STTS22H_ERROR             -1

/**
  * @}
  */

/** @addtogroup STTS22H_Exported_Functions STTS22H Exported Functions
  * @{
  */

int32_t STTS22H_RegisterBusIO(STTS22H_Object_t *pObj, STTS22H_IO_t *pIO);
int32_t STTS22H_Init(STTS22H_Object_t *pObj);
int32_t STTS22H_DeInit(STTS22H_Object_t *pObj);
int32_t STTS22H_ReadID(STTS22H_Object_t *pObj, uint8_t *Id);
int32_t STTS22H_GetCapabilities(STTS22H_Object_t *pObj, STTS22H_Capabilities_t *Capabilities);
int32_t STTS22H_Get_Init_Status(STTS22H_Object_t *pObj, uint8_t *Status);

int32_t STTS22H_TEMP_Enable(STTS22H_Object_t *pObj);
int32_t STTS22H_TEMP_Disable(STTS22H_Object_t *pObj);
int32_t STTS22H_TEMP_GetOutputDataRate(STTS22H_Object_t *pObj, float *Odr);
int32_t STTS22H_TEMP_SetOutputDataRate(STTS22H_Object_t *pObj, float Odr);
int32_t STTS22H_TEMP_GetTemperature(STTS22H_Object_t *pObj, float *Value);
int32_t STTS22H_TEMP_Get_DRDY_Status(STTS22H_Object_t *pObj, uint8_t *Status);
int32_t STTS22H_TEMP_SetLowTemperatureThreshold(STTS22H_Object_t *pObj, float Value);
int32_t STTS22H_TEMP_SetHighTemperatureThreshold(STTS22H_Object_t *pObj, float Value);
int32_t STTS22H_TEMP_GetTemperatureLimitStatus(STTS22H_Object_t *pObj, uint8_t *HighLimit, uint8_t *LowLimit);

int32_t STTS22H_Read_Reg(STTS22H_Object_t *pObj, uint8_t Reg, uint8_t *Data);
int32_t STTS22H_Write_Reg(STTS22H_Object_t *pObj, uint8_t Reg, uint8_t Data);

int32_t STTS22H_Set_One_Shot(STTS22H_Object_t *pObj);
int32_t STTS22H_Get_One_Shot_Status(STTS22H_Object_t *pObj, uint8_t *Status);

/**
  * @}
  */

/** @addtogroup STTS22H_Exported_Variables STTS22H Exported Variables
  * @{
  */

extern STTS22H_CommonDrv_t STTS22H_COMMON_Driver;
extern STTS22H_TEMP_Drv_t STTS22H_TEMP_Driver;

/**
  * @}
  */

#ifdef __cplusplus
}
#endif

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
