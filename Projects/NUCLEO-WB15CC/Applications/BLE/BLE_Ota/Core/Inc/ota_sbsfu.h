/**
  ******************************************************************************
  * @file    ota_sbsfu.h
  * @author  MCD Application Team
  * @brief   OTP manager  interface
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2018(-2021) STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* This file brings SBSFU functions and constants that are needed by ble_ota project*/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __OTA_SBSFU_H
#define __OTA_SBSFU_H


#ifdef __cplusplus
extern "C" {
#endif

/* SBSFU communication definitions
   =============================== */
#include "sfu_standalone_loader.h"

/* SBSFU mapping definitions 
   ========================= */
#if defined (__ICCARM__) || defined(__GNUC__)
#include "mapping_export.h"
#elif defined(__CC_ARM)
#include "mapping_sbsfu.h"
#include "mapping_fwimg.h"
#endif /* __ICCARM__ || __GNUC__ */

#ifdef __cplusplus
}
#endif

#endif /*__OTA_SBSFU_H */
