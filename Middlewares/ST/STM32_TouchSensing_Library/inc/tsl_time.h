/**
  ******************************************************************************
  * @file    tsl_time.h
  * @author  MCD Application Team
  * @brief   This file contains external declarations of the tsl_time.c file.
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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
