/**
  ******************************************************************************
  * @file    tdes.h
  * @author  MCD Application Team
  * @brief   Container for the TDES functionalities
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2015 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Image license SLA0044,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        http://www.st.com/SLA0044
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "Common/tdes_common.h"

#ifdef INCLUDE_ECB
#include "ECB/tdes_ecb.h"
#endif

#ifdef INCLUDE_CBC
#include "CBC/tdes_cbc.h"
#endif

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
