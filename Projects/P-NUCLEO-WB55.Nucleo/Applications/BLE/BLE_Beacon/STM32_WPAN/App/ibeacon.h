/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : App/ibeacon.h
  * Description        : Header for ibeacon_application.c module
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
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef IBEACON_H
#define IBEACON_H

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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
