/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    ibeacon.h
  * @author  MCD Application Team
  * @brief   Header for ibeacon_application.c module
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2019-2021 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef IBEACON_H
#define IBEACON_H

#ifdef __cplusplus
extern "C"
{
#endif

/* Includes ------------------------------------------------------------------*/
#if defined (__CC_ARM) || defined (__ARMCC_VERSION)
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
/* Exported constants --------------------------------------------------------*/
#define ADVERTISING_INTERVAL_IN_MS                                        (1000)
#define CALIBRATED_TX_POWER_AT_1_M                             ((uint8_t) (-63))
#define UUID                0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15
#define MAJOR_ID                                                            0, 1
#define MINOR_ID                                                            0, 1

#if ((ADVERTISING_INTERVAL_IN_MS <= 0) || (ADVERTISING_INTERVAL_IN_MS > 40959))
  #error "Invalid advertising interval! Please select a value between 0 and 40959 ms."
#endif

/* Exported Macros -----------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

#ifdef __cplusplus
}
#endif

#endif /* IBEACON_H */
