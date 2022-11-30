/**
  ******************************************************************************
  * @file    tsl.h
  * @author  MCD Application Team
  * @brief   This file contains external declarations of the tsl.c file.
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
