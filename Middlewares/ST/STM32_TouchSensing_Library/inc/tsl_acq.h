/**
  ******************************************************************************
  * @file    tsl_acq.h
  * @author  MCD Application Team
  * @brief   This file contains external declarations of the tsl_acq.c file.
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
#ifndef __TSL_ACQ_H
#define __TSL_ACQ_H

/* Includes ------------------------------------------------------------------*/
#include "tsl_conf.h"

/* Defines -------------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/

// Filter functions
typedef TSL_tMeas_T(* TSL_pFuncMeasFilter_T)(TSL_tMeas_T, TSL_tMeas_T); /**< Pointer to the Measure filter function */
typedef TSL_tDelta_T(* TSL_pFuncDeltaFilter_T)(TSL_tDelta_T); /**< Pointer to the Delta filter function */

/* Exported variables --------------------------------------------------------*/

/* Exported macros -----------------------------------------------------------*/

/* Exported functions ------------------------------------------------------- */
TSL_Status_enum_T TSL_acq_BankGetResult(TSL_tIndex_T idx_bk, TSL_pFuncMeasFilter_T mfilter, TSL_pFuncDeltaFilter_T dfilter);
TSL_Status_enum_T TSL_acq_BankCalibrate(TSL_tIndex_T bank);
void TSL_acq_BankClearData(TSL_tIndex_T bank);

#endif /* __TSL_ACQ_H */
