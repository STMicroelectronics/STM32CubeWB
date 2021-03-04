/**
  ******************************************************************************
  * @file    ota_sbsfu.h
  * @author  MCD Application Team
  * @brief   OTP manager  interface
  ******************************************************************************
  * @attention
 *
 * <h2><center>&copy; Copyright (c) 2018 STMicroelectronics.
 * All rights reserved.</center></h2>
 *
 * This software component is licensed by ST under Ultimate Liberty license
 * SLA0044, the "License"; You may not use this file except in compliance with
 * the License. You may obtain a copy of the License at:
 *                             www.st.com/SLA0044
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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
