/**
  ******************************************************************************
  * File Name          : App/eddystone_beacon.h
  * Description        :
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
#ifndef EDDYSTONE_BEACON_H
#define EDDYSTONE_BEACON_H

#ifdef __cplusplus
extern "C"
{
#endif

/* Includes ------------------------------------------------------------------*/
#if defined (__CC_ARM)
	#define ASSERT_CONCAT_(a, b) a##b
	#define ASSERT_CONCAT(a, b) ASSERT_CONCAT_(a, b)
	/* These can't be used after statements in c89. */
	#ifdef __COUNTER__
		#define STATIC_ASSERT(e,m) \
			;enum { ASSERT_CONCAT(static_assert_, __COUNTER__) = 1/(!!(e)) }
	#else
		/* This can't be used twice on the same line so ensure if using in headers
		 * that the headers are not included twice (by wrapping in #ifndef...#endif)
		 * Note it doesn't cause an issue when used on same line of separate modules
		 * compiled with gcc -combine -fwhole-program.  */
		#define static_assert(e,m) \
			;enum { ASSERT_CONCAT(assert_line_, __LINE__) = 1/(!!(e)) }
	#endif
#endif

/* Exported types ------------------------------------------------------------*/
enum
{
  NOT_CONNECTED,
  CONNECTED
};

/* Exported constants --------------------------------------------------------*/
#define ADVERTISING_INTERVAL_IN_MS  (1000)
#define CALIBRATED_TX_POWER_AT_0_M  ((uint8_t) (-22))
#define NAMESPACE_ID                'w', 'w', 'w', '.', 's', 't', '.', 'c', 'o', 'm'
#define BEACON_ID                   0, 0, 0, 0, 0, 1
#define URL_PREFIX                  HTTP
#define PHYSICAL_WEB_URL            "www.st.com"
#define NORMAL_TIME_ADV             10000000
#define TLM_TIME_ADV                1000000

#if (0 != (CFG_BEACON_TYPE & (CFG_BEACON_TYPE - 1)))
  #error "Please select only a single beacon type!"
#endif

#if ((ADVERTISING_INTERVAL_IN_MS <= 0) || (ADVERTISING_INTERVAL_IN_MS > 40959))
  #error "Invalid advertising interval! Please select a value between 0 and 40959 ms."
#endif

#if (0 != (CFG_BEACON_TYPE & CFG_EDDYSTONE_TLM_BEACON_TYPE))
  #if ((ADVERTISING_INTERVAL_IN_MS < 1000) || ((ADVERTISING_INTERVAL_IN_MS % 1000) != 0))
    #error "EDDYSTONE_TLM only supports intervals in increments of 1 s!"
  #endif
#endif

#if (0 != (CFG_BEACON_TYPE & CFG_EDDYSTONE_URL_BEACON_TYPE))
#if defined (__CC_ARM)
  STATIC_ASSERT(sizeof(PHYSICAL_WEB_URL) < 17, "The URL must be less than 17 characters.");
#elif defined (__ICCARM__)
  static_assert(sizeof(PHYSICAL_WEB_URL) < 17, "The URL must be less than 17 characters.");
#else
#warning Please check that sizeof(PHYSICAL_WEB_URL) < 17
#endif
#endif

/* Exported Macros -----------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

#ifdef __cplusplus
}
#endif

#endif /* EDDYSTONE_BEACON_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
