/**
 ******************************************************************************
 * @file    ism330dlc.h
 * @author  MEMS Software Solutions Team
 * @brief   ISM330DLC header driver file
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
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 ******************************************************************************
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef ISM330DLC_H
#define ISM330DLC_H

#ifdef __cplusplus
extern "C"
{
#endif

/* Includes ------------------------------------------------------------------*/
#include "ism330dlc_reg.h"
#include <string.h>

/** @addtogroup BSP BSP
 * @{
 */

/** @addtogroup Component Component
 * @{
 */

/** @addtogroup ISM330DLC ISM330DLC
 * @{
 */

/** @defgroup ISM330DLC_Exported_Types ISM330DLC Exported Types
 * @{
 */

typedef int32_t (*ISM330DLC_Init_Func)(void);
typedef int32_t (*ISM330DLC_DeInit_Func)(void);
typedef int32_t (*ISM330DLC_GetTick_Func)(void);
typedef int32_t (*ISM330DLC_WriteReg_Func)(uint16_t, uint16_t, uint8_t *, uint16_t);
typedef int32_t (*ISM330DLC_ReadReg_Func)(uint16_t, uint16_t, uint8_t *, uint16_t);

typedef enum
{
  ISM330DLC_INT1_PIN,
  ISM330DLC_INT2_PIN,
} ISM330DLC_SensorIntPin_t;

typedef struct
{
  ISM330DLC_Init_Func          Init;
  ISM330DLC_DeInit_Func        DeInit;
  uint32_t                     BusType; /*0 means I2C, 1 means SPI 4-Wires, 2 means SPI-3-Wires */
  uint8_t                      Address;
  ISM330DLC_WriteReg_Func      WriteReg;
  ISM330DLC_ReadReg_Func       ReadReg;
  ISM330DLC_GetTick_Func       GetTick;
} ISM330DLC_IO_t;

typedef struct
{
  int16_t x;
  int16_t y;
  int16_t z;
} ISM330DLC_AxesRaw_t;

typedef struct
{
  int32_t x;
  int32_t y;
  int32_t z;
} ISM330DLC_Axes_t;

typedef struct
{
  unsigned int FreeFallStatus : 1;
  unsigned int TapStatus : 1;
  unsigned int DoubleTapStatus : 1;
  unsigned int WakeUpStatus : 1;
  unsigned int StepStatus : 1;
  unsigned int TiltStatus : 1;
  unsigned int D6DOrientationStatus : 1;
  unsigned int SleepStatus : 1;
} ISM330DLC_Event_Status_t;

typedef struct
{
  ISM330DLC_IO_t        IO;
  ism330dlc_ctx_t       Ctx;
  uint8_t               is_initialized;
  uint8_t               acc_is_enabled;
  uint8_t               gyro_is_enabled;
  ism330dlc_odr_xl_t    acc_odr;
  ism330dlc_odr_g_t     gyro_odr;
} ISM330DLC_Object_t;

typedef struct
{
  uint8_t   Acc;
  uint8_t   Gyro;
  uint8_t   Magneto;
  uint8_t   LowPower;
  uint32_t  GyroMaxFS;
  uint32_t  AccMaxFS;
  uint32_t  MagMaxFS;
  float     GyroMaxOdr;
  float     AccMaxOdr;
  float     MagMaxOdr;
} ISM330DLC_Capabilities_t;

typedef struct
{
  int32_t (*Init)(ISM330DLC_Object_t *);
  int32_t (*DeInit)(ISM330DLC_Object_t *);
  int32_t (*ReadID)(ISM330DLC_Object_t *, uint8_t *);
  int32_t (*GetCapabilities)(ISM330DLC_Object_t *, ISM330DLC_Capabilities_t *);
} ISM330DLC_CommonDrv_t;

typedef struct
{
  int32_t (*Enable)(ISM330DLC_Object_t *);
  int32_t (*Disable)(ISM330DLC_Object_t *);
  int32_t (*GetSensitivity)(ISM330DLC_Object_t *, float *);
  int32_t (*GetOutputDataRate)(ISM330DLC_Object_t *, float *);
  int32_t (*SetOutputDataRate)(ISM330DLC_Object_t *, float);
  int32_t (*GetFullScale)(ISM330DLC_Object_t *, int32_t *);
  int32_t (*SetFullScale)(ISM330DLC_Object_t *, int32_t);
  int32_t (*GetAxes)(ISM330DLC_Object_t *, ISM330DLC_Axes_t *);
  int32_t (*GetAxesRaw)(ISM330DLC_Object_t *, ISM330DLC_AxesRaw_t *);
} ISM330DLC_ACC_Drv_t;

typedef struct
{
  int32_t (*Enable)(ISM330DLC_Object_t *);
  int32_t (*Disable)(ISM330DLC_Object_t *);
  int32_t (*GetSensitivity)(ISM330DLC_Object_t *, float *);
  int32_t (*GetOutputDataRate)(ISM330DLC_Object_t *, float *);
  int32_t (*SetOutputDataRate)(ISM330DLC_Object_t *, float);
  int32_t (*GetFullScale)(ISM330DLC_Object_t *, int32_t *);
  int32_t (*SetFullScale)(ISM330DLC_Object_t *, int32_t);
  int32_t (*GetAxes)(ISM330DLC_Object_t *, ISM330DLC_Axes_t *);
  int32_t (*GetAxesRaw)(ISM330DLC_Object_t *, ISM330DLC_AxesRaw_t *);
} ISM330DLC_GYRO_Drv_t;

/**
 * @}
 */

/** @defgroup ISM330DLC_Exported_Constants ISM330DLC Exported Constants
 * @{
 */

#define ISM330DLC_OK                       0
#define ISM330DLC_ERROR                   -1

#define ISM330DLC_I2C_BUS                 0U
#define ISM330DLC_SPI_4WIRES_BUS          1U
#define ISM330DLC_SPI_3WIRES_BUS          2U

#define ISM330DLC_ACC_SENSITIVITY_FS_2G   0.061f
#define ISM330DLC_ACC_SENSITIVITY_FS_4G   0.122f
#define ISM330DLC_ACC_SENSITIVITY_FS_8G   0.244f
#define ISM330DLC_ACC_SENSITIVITY_FS_16G  0.488f

#define ISM330DLC_GYRO_SENSITIVITY_FS_125DPS    4.375f
#define ISM330DLC_GYRO_SENSITIVITY_FS_250DPS    8.750f
#define ISM330DLC_GYRO_SENSITIVITY_FS_500DPS   17.500f
#define ISM330DLC_GYRO_SENSITIVITY_FS_1000DPS  35.000f
#define ISM330DLC_GYRO_SENSITIVITY_FS_2000DPS  70.000f

/**
 * @}
 */

/** @addtogroup ISM330DLC_Exported_Functions ISM330DLC Exported Functions
 * @{
 */

int32_t ISM330DLC_RegisterBusIO(ISM330DLC_Object_t *pObj, ISM330DLC_IO_t *pIO);
int32_t ISM330DLC_Init(ISM330DLC_Object_t *pObj);
int32_t ISM330DLC_DeInit(ISM330DLC_Object_t *pObj);
int32_t ISM330DLC_ReadID(ISM330DLC_Object_t *pObj, uint8_t *Id);
int32_t ISM330DLC_GetCapabilities(ISM330DLC_Object_t *pObj, ISM330DLC_Capabilities_t *Capabilities);

int32_t ISM330DLC_ACC_Enable(ISM330DLC_Object_t *pObj);
int32_t ISM330DLC_ACC_Disable(ISM330DLC_Object_t *pObj);
int32_t ISM330DLC_ACC_GetSensitivity(ISM330DLC_Object_t *pObj, float *Sensitivity);
int32_t ISM330DLC_ACC_GetOutputDataRate(ISM330DLC_Object_t *pObj, float *Odr);
int32_t ISM330DLC_ACC_SetOutputDataRate(ISM330DLC_Object_t *pObj, float Odr);
int32_t ISM330DLC_ACC_GetFullScale(ISM330DLC_Object_t *pObj, int32_t *FullScale);
int32_t ISM330DLC_ACC_SetFullScale(ISM330DLC_Object_t *pObj, int32_t FullScale);
int32_t ISM330DLC_ACC_GetAxesRaw(ISM330DLC_Object_t *pObj, ISM330DLC_AxesRaw_t *Value);
int32_t ISM330DLC_ACC_GetAxes(ISM330DLC_Object_t *pObj, ISM330DLC_Axes_t *Acceleration);

int32_t ISM330DLC_GYRO_Enable(ISM330DLC_Object_t *pObj);
int32_t ISM330DLC_GYRO_Disable(ISM330DLC_Object_t *pObj);
int32_t ISM330DLC_GYRO_GetSensitivity(ISM330DLC_Object_t *pObj, float *Sensitivity);
int32_t ISM330DLC_GYRO_GetOutputDataRate(ISM330DLC_Object_t *pObj, float *Odr);
int32_t ISM330DLC_GYRO_SetOutputDataRate(ISM330DLC_Object_t *pObj, float Odr);
int32_t ISM330DLC_GYRO_GetFullScale(ISM330DLC_Object_t *pObj, int32_t *FullScale);
int32_t ISM330DLC_GYRO_SetFullScale(ISM330DLC_Object_t *pObj, int32_t FullScale);
int32_t ISM330DLC_GYRO_GetAxesRaw(ISM330DLC_Object_t *pObj, ISM330DLC_AxesRaw_t *Value);
int32_t ISM330DLC_GYRO_GetAxes(ISM330DLC_Object_t *pObj, ISM330DLC_Axes_t *AngularRate);

int32_t ISM330DLC_Read_Reg(ISM330DLC_Object_t *pObj, uint8_t reg, uint8_t *Data);
int32_t ISM330DLC_Write_Reg(ISM330DLC_Object_t *pObj, uint8_t reg, uint8_t Data);
int32_t ISM330DLC_Set_Interrupt_Latch(ISM330DLC_Object_t *pObj, uint8_t Status);
int32_t ISM330DLC_Set_INT1_Drdy(ISM330DLC_Object_t *pObj, uint8_t Status);
int32_t ISM330DLC_Set_INT2_Drdy(ISM330DLC_Object_t *pObj, uint8_t Status);
int32_t ISM330DLC_Set_Drdy_Mode(ISM330DLC_Object_t *pObj, uint8_t Status);

int32_t ISM330DLC_ACC_Enable_Free_Fall_Detection(ISM330DLC_Object_t *pObj, ISM330DLC_SensorIntPin_t IntPin);
int32_t ISM330DLC_ACC_Disable_Free_Fall_Detection(ISM330DLC_Object_t *pObj);
int32_t ISM330DLC_ACC_Set_Free_Fall_Threshold(ISM330DLC_Object_t *pObj, uint8_t Threshold);
int32_t ISM330DLC_ACC_Set_Free_Fall_Duration(ISM330DLC_Object_t *pObj, uint8_t Duration);

int32_t ISM330DLC_ACC_Enable_Tilt_Detection(ISM330DLC_Object_t *pObj, ISM330DLC_SensorIntPin_t IntPin);
int32_t ISM330DLC_ACC_Disable_Tilt_Detection(ISM330DLC_Object_t *pObj);

int32_t ISM330DLC_ACC_Enable_Wake_Up_Detection(ISM330DLC_Object_t *pObj, ISM330DLC_SensorIntPin_t IntPin);
int32_t ISM330DLC_ACC_Disable_Wake_Up_Detection(ISM330DLC_Object_t *pObj);
int32_t ISM330DLC_ACC_Set_Wake_Up_Threshold(ISM330DLC_Object_t *pObj, uint8_t Threshold);
int32_t ISM330DLC_ACC_Set_Wake_Up_Duration(ISM330DLC_Object_t *pObj, uint8_t Duration);

int32_t ISM330DLC_ACC_Enable_Single_Tap_Detection(ISM330DLC_Object_t *pObj, ISM330DLC_SensorIntPin_t IntPin);
int32_t ISM330DLC_ACC_Disable_Single_Tap_Detection(ISM330DLC_Object_t *pObj);
int32_t ISM330DLC_ACC_Enable_Double_Tap_Detection(ISM330DLC_Object_t *pObj, ISM330DLC_SensorIntPin_t IntPin);
int32_t ISM330DLC_ACC_Disable_Double_Tap_Detection(ISM330DLC_Object_t *pObj);
int32_t ISM330DLC_ACC_Set_Tap_Threshold(ISM330DLC_Object_t *pObj, uint8_t Threshold);
int32_t ISM330DLC_ACC_Set_Tap_Shock_Time(ISM330DLC_Object_t *pObj, uint8_t Time);
int32_t ISM330DLC_ACC_Set_Tap_Quiet_Time(ISM330DLC_Object_t *pObj, uint8_t Time);
int32_t ISM330DLC_ACC_Set_Tap_Duration_Time(ISM330DLC_Object_t *pObj, uint8_t Time);

int32_t ISM330DLC_ACC_Enable_6D_Orientation(ISM330DLC_Object_t *pObj, ISM330DLC_SensorIntPin_t IntPin);
int32_t ISM330DLC_ACC_Disable_6D_Orientation(ISM330DLC_Object_t *pObj);
int32_t ISM330DLC_ACC_Set_6D_Orientation_Threshold(ISM330DLC_Object_t *pObj, uint8_t Threshold);
int32_t ISM330DLC_ACC_Get_6D_Orientation_XL(ISM330DLC_Object_t *pObj, uint8_t *XLow);
int32_t ISM330DLC_ACC_Get_6D_Orientation_XH(ISM330DLC_Object_t *pObj, uint8_t *XHigh);
int32_t ISM330DLC_ACC_Get_6D_Orientation_YL(ISM330DLC_Object_t *pObj, uint8_t *YLow);
int32_t ISM330DLC_ACC_Get_6D_Orientation_YH(ISM330DLC_Object_t *pObj, uint8_t *YHigh);
int32_t ISM330DLC_ACC_Get_6D_Orientation_ZL(ISM330DLC_Object_t *pObj, uint8_t *ZLow);
int32_t ISM330DLC_ACC_Get_6D_Orientation_ZH(ISM330DLC_Object_t *pObj, uint8_t *ZHigh);

int32_t ISM330DLC_ACC_Get_Event_Status(ISM330DLC_Object_t *pObj, ISM330DLC_Event_Status_t *Status);
int32_t ISM330DLC_ACC_Set_SelfTest(ISM330DLC_Object_t *pObj, uint8_t Status);
int32_t ISM330DLC_ACC_Get_DRDY_Status(ISM330DLC_Object_t *pObj, uint8_t *Status);
int32_t ISM330DLC_ACC_Get_Init_Status(ISM330DLC_Object_t *pObj, uint8_t *Status);
int32_t ISM330DLC_ACC_Enable_HP_Filter(ISM330DLC_Object_t *pObj, ism330dlc_hpcf_xl_t CutOff);

int32_t ISM330DLC_GYRO_Set_SelfTest(ISM330DLC_Object_t *pObj, uint8_t Status);
int32_t ISM330DLC_GYRO_Get_DRDY_Status(ISM330DLC_Object_t *pObj, uint8_t *Status);
int32_t ISM330DLC_GYRO_Get_Init_Status(ISM330DLC_Object_t *pObj, uint8_t *Status);

int32_t ISM330DLC_FIFO_Get_Num_Samples(ISM330DLC_Object_t *pObj, uint16_t *NumSamples);
int32_t ISM330DLC_FIFO_Get_Full_Status(ISM330DLC_Object_t *pObj, uint8_t *Status);
int32_t ISM330DLC_FIFO_Set_ODR_Value(ISM330DLC_Object_t *pObj, float Odr);
int32_t ISM330DLC_FIFO_Set_INT1_FIFO_Full(ISM330DLC_Object_t *pObj, uint8_t Status);
int32_t ISM330DLC_FIFO_Set_INT2_FIFO_Full(ISM330DLC_Object_t *pObj, uint8_t Status);
int32_t ISM330DLC_FIFO_Set_Watermark_Level(ISM330DLC_Object_t *pObj, uint16_t Watermark);
int32_t ISM330DLC_FIFO_Set_Stop_On_Fth(ISM330DLC_Object_t *pObj, uint8_t Status);
int32_t ISM330DLC_FIFO_Set_Mode(ISM330DLC_Object_t *pObj, uint8_t Mode);
int32_t ISM330DLC_FIFO_Get_Pattern(ISM330DLC_Object_t *pObj, uint16_t *Pattern);
int32_t ISM330DLC_FIFO_Get_Data(ISM330DLC_Object_t *pObj, uint8_t *Data);
int32_t ISM330DLC_FIFO_Full_Set_INT1(ISM330DLC_Object_t *pObj, uint8_t Status);
int32_t ISM330DLC_FIFO_Set_INT2_Drdy(ISM330DLC_Object_t *pObj, uint8_t Status);int32_t ISM330DLC_FIFO_ACC_Set_Decimation(ISM330DLC_Object_t *pObj, uint8_t Decimation);
int32_t ISM330DLC_FIFO_ACC_Set_Decimation(ISM330DLC_Object_t *pObj, uint8_t Decimation);
int32_t ISM330DLC_FIFO_Get_Data_Word(ISM330DLC_Object_t *pObj, int16_t *data_raw);
int32_t ISM330DLC_FIFO_ACC_Get_Axis(ISM330DLC_Object_t *pObj, int32_t *Acceleration);
int32_t ISM330DLC_FIFO_GYRO_Set_Decimation(ISM330DLC_Object_t *pObj, uint8_t Decimation);
int32_t ISM330DLC_FIFO_GYRO_Get_Axis(ISM330DLC_Object_t *pObj, int32_t *AngularVelocity);

/**
 * @}
 */

/** @addtogroup ISM330DLC_Exported_Variables ISM330DLC Exported Variables
 * @{
 */

extern ISM330DLC_CommonDrv_t ISM330DLC_COMMON_Driver;
extern ISM330DLC_ACC_Drv_t ISM330DLC_ACC_Driver;
extern ISM330DLC_GYRO_Drv_t ISM330DLC_GYRO_Driver;

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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
