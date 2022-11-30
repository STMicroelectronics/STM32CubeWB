/**
  ******************************************************************************
  * @file    tsl_ecs.h
  * @author  MCD Application Team
  * @brief   This file contains external declarations of the tsl_ecs.c file.
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
#ifndef __TSL_ECS_H
#define __TSL_ECS_H

/* Includes ------------------------------------------------------------------*/
#include "tsl_object.h"

/* Exported types ------------------------------------------------------------*/

/* Exported functions --------------------------------------------------------*/

TSL_tKCoeff_T TSL_ecs_CalcK(TSL_ObjectGroup_T *objgrp, TSL_tKCoeff_T k_slow, TSL_tKCoeff_T k_fast);
void TSL_ecs_ProcessK(TSL_ObjectGroup_T *objgrp, TSL_tKCoeff_T Kcoeff);
TSL_Status_enum_T TSL_ecs_Process(TSL_ObjectGroup_T *objgrp);

#endif /* __TSL_ECS_H */
