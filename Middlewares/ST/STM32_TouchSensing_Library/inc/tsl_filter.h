/**
  ******************************************************************************
  * @file    tsl_filter.h
  * @author  MCD Application Team
  * @brief   This file contains external declarations of the tsl_filter.c file.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
