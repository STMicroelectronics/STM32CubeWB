/**
  ******************************************************************************
  * @file    tsl_filter.h
  * @author  MCD Application Team
  * @brief   This file contains external declarations of the tsl_filter.c file.
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
#ifndef __TSL_FILTER_H
#define __TSL_FILTER_H

/* Includes ------------------------------------------------------------------*/
#include "tsl_acq.h"

/* Exported types ------------------------------------------------------------*/
TSL_tMeas_T TSL_filt_MeasFilter(TSL_tMeas_T measn1, TSL_tMeas_T measn);
TSL_tDelta_T TSL_filt_DeltaFilter(TSL_tDelta_T delta);

#endif /* __TSL_FILTER_H */
