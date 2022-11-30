/**
  ******************************************************************************
  * @file    tsl_globals.h
  * @author  MCD Application Team
  * @brief   This file contains external declarations of the tsl_globals.c file.
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
#ifndef __TSL_GLOBALS_H
#define __TSL_GLOBALS_H

/* Includes ------------------------------------------------------------------*/
#include "tsl_acq.h"
#include "tsl_object.h"

/* Exported types ------------------------------------------------------------*/

/** Store all global variables shared between the STMTouch Driver and the Application.
  */
typedef struct
{
  TSL_tTick_ms_T       Tick_ms;     /**< Incremented each 0.5ms by timing interrupt routine */
  TSL_tTick_sec_T      Tick_sec;    /**< Incremented each second by timing interrupt routine */
  CONST TSL_Bank_T     *Bank_Array; /**< Pointer to the array containing all Banks */
  TSL_tIndex_T         This_Bank;   /**< Pointer to the current Bank */
  CONST TSL_Object_T   *This_Obj;   /**< Pointer to the current Object */
  uint32_t             DelayDischarge; /**< Delay used to discharge all capacitors */
#if TSLPRM_TOTAL_TKEYS > 0
  CONST TSL_TouchKey_T *This_TKey; /**< Pointer to the current TKey */
#endif
#if TSLPRM_TOTAL_LNRTS > 0
  CONST TSL_LinRot_T   *This_LinRot; /**< Pointer to the current Linear or Rotary sensor */
#endif
}
TSL_Globals_T;

/** Store all global parametersshared between the STMTouch Driver and the Application .
  @warning Only one variable of this structure type must be created and be placed
  in RAM only.
  */
typedef struct
{
  TSL_tMeas_T       AcqMin;         /**< Acquisition minimum limit */
  TSL_tMeas_T       AcqMax;         /**< Acquisition maximum limit */
  TSL_tNb_T         NbCalibSamples; /**< Number of Calibration samples */
  TSL_tTick_sec_T   DTO;            /**< Detection Time Out */
#if TSLPRM_TOTAL_TKEYS > 0
  CONST TSL_State_T           *p_TKeySM; /**< Default state machine for TouchKey sensors */
  CONST TSL_TouchKeyMethods_T *p_TKeyMT; /**< Default methods for TouchKey sensors */
#endif
#if TSLPRM_TOTAL_LNRTS > 0
  CONST TSL_State_T         *p_LinRotSM; /**< Default state machine for Linear/Rotary sensors */
  CONST TSL_LinRotMethods_T *p_LinRotMT; /**< Default methods for Linear/Rotary sensors */
#endif
}
TSL_Params_T;

/* Exported variables --------------------------------------------------------*/
extern TSL_Globals_T TSL_Globals;
extern TSL_Params_T TSL_Params;

#endif /* __TSL_GLOBALS_H */
