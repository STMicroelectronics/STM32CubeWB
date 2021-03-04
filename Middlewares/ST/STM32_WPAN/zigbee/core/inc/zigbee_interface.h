/**
 ******************************************************************************
 * @file    Zigbee_interface.h
 * @author
 * @brief   Header for zigbee interface API (M4 side)
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
 * All rights reserved.</center></h2>
 *
 * This software component is licensed by ST under Ultimate Liberty license
 * SLA0044, the "License"; You may not use this file except in compliance with
 * the License. You may obtain a copy of the License at:
 *                             www.st.com/SLA0044
 *
 ******************************************************************************
 */
#ifndef _ZIGBEE_INTERFACE
#define _ZIGBEE_INTERFACE

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <stdbool.h>
#include "zigbee_types.h"
#include "stm32wbxx_core_interface_def.h"
#include "zigbee.h"

/*---------------------------------------------------------------
 * Well-Known Zigbee Security Keys
 *---------------------------------------------------------------
 */
/* Null (all zeroes)
 * 00:00:00:00:00:00:00:00:00:00:00:00:00:00:00:00 */
extern const uint8_t sec_key_null[ZB_SEC_KEYSIZE];

/* "ZigBeeAlliance09"
 * 5a:69:67:42:65:65:41:6c:6c:69:61:6e:63:65:30:39 */
extern const uint8_t sec_key_ha[ZB_SEC_KEYSIZE];

/* Uncertified Device's Distributed Link Key
 * d0:d1:d2:d3:d4:d5:d6:d7:d8:d9:da:db:dc:dd:de:df */
extern const uint8_t sec_key_distrib_uncert[ZB_SEC_KEYSIZE];

/* TOUCHLINK_KEY_INDEX_CERTIFICATION key
 * c0:c1:c2:c3:c4:c5:c6:c7 0xc8:c9:ca:cb:cc:cd:ce:cf */
extern const uint8_t sec_key_touchlink_cert[ZB_SEC_KEYSIZE];

/* Exported defines ----------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* External variables --------------------------------------------------------*/
/* Exported macros -----------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* _ZIGBEE_INTERFACE */
