/**
  ******************************************************************************
  * @file    tsl_check_config_tsc.h
  * @author  MCD Application Team
  * @brief   This file contains the check of all parameters defined in the
  *          TSC configuration file.
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
#ifndef __TSL_CHECK_CONFIG_TSC_H
#define __TSL_CHECK_CONFIG_TSC_H

// Check parameters common to all acquisitions
#include "tsl_check_config.h"

//------------------------------------------------------------------------------

#if ((TSLPRM_TOTAL_CHANNELS < 1) || (TSLPRM_TOTAL_CHANNELS > 24))
#error "TSLPRM_TOTAL_CHANNELS is out of range (1 .. 24)."
#endif

#if ((TSLPRM_TOTAL_BANKS < 1) || (TSLPRM_TOTAL_BANKS > 8))
#error "TSLPRM_TOTAL_BANKS is out of range (1 .. 8)."
#endif

#if ((TSLPRM_TOTAL_TOUCHKEYS < 0) || (TSLPRM_TOTAL_TOUCHKEYS > 24))
#error "TSLPRM_TOTAL_TOUCHKEYS is out of range (0 .. 24)."
#endif

#if ((TSLPRM_TOTAL_TOUCHKEYS_B < 0) || (TSLPRM_TOTAL_TOUCHKEYS_B > 24))
#error "TSLPRM_TOTAL_TOUCHKEYS_B is out of range (0 .. 24)."
#endif

#if ((TSLPRM_TOTAL_LINROTS < 0) || (TSLPRM_TOTAL_LINROTS > 24))
#error "TSLPRM_TOTAL_LINROTS is out of range (0 .. 24)."
#endif

#if ((TSLPRM_TOTAL_LINROTS_B < 0) || (TSLPRM_TOTAL_LINROTS_B > 24))
#error "TSLPRM_TOTAL_LINROTS_B is out of range (0 .. 24)."
#endif

#if ((TSLPRM_TOTAL_OBJECTS < 1) || (TSLPRM_TOTAL_OBJECTS > 24))
#error "TSLPRM_TOTAL_OBJECTS is out of range (1 .. 24)."
#endif

#if ((TSLPRM_TOTAL_TKEYS + TSLPRM_TOTAL_LNRTS) > 24)
#error "The Sum of TouchKeys and Linear/Rotary sensors exceeds 24."
#endif

#endif /* __TSL_CHECK_CONFIG_TSC_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
