/**
  ******************************************************************************
  * @file    tsl_ecs.h
  * @author  MCD Application Team
  * @brief   This file contains external declarations of the tsl_ecs.c file.
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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
