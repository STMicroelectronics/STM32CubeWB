/**
  ******************************************************************************
  * @file    tsl.h
  * @author  MCD Application Team
  * @brief   This file contains external declarations of the tsl.c file.
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
#ifndef __TSL_H
#define __TSL_H

/* Includes ------------------------------------------------------------------*/
#include "tsl_acq.h"
#include "tsl_time.h"
#include "tsl_touchkey.h"
#include "tsl_linrot.h"
#include "tsl_object.h"
#include "tsl_dxs.h"
#include "tsl_ecs.h"
#include "tsl_filter.h"
#include "tsl_globals.h"

/* Exported types ------------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported macros -----------------------------------------------------------*/

/* Exported functions ------------------------------------------------------- */
TSL_Status_enum_T TSL_Init(CONST TSL_Bank_T *bank);

#endif /* __TSL_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
