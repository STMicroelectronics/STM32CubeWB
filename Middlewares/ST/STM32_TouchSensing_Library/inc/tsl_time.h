/**
  ******************************************************************************
  * @file    tsl_time.h
  * @author  MCD Application Team
  * @brief   This file contains external declarations of the tsl_time.c file.
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
#ifndef __TSL_TIME_H
#define __TSL_TIME_H

/* Includes ------------------------------------------------------------------*/
#include "tsl_conf.h"

/* Exported functions ------------------------------------------------------- */

void TSL_tim_ProcessIT(void);
TSL_Status_enum_T TSL_tim_CheckDelay_ms(TSL_tTick_ms_T delay_ms, __IO TSL_tTick_ms_T *last_tick);
TSL_Status_enum_T TSL_tim_CheckDelay_sec(TSL_tTick_sec_T delay_sec, __IO TSL_tTick_sec_T *last_tick);
void TSL_CallBack_TimerTick(void);

#endif /* __TSL_TIME_H */
