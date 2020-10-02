/* USER CODE BEGIN Header */
/**
 ******************************************************************************
  * File Name          : app_lld_tests.h
  * Description        : Header for RF LLDs tests application.
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
#ifndef APP_LLD_TESTS_H
#define APP_LLD_TESTS_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/

/* Application errors                 */
/*------------------------------------*/

/*
 *  List of all errors tracked by the Thread application
 *  running on M4. Some of these errors may be fatal
 *  or just warnings
 */
typedef enum
{
  //ERR_LLDTESTS_REC_MULTI_MSG_FROM_M0,
  ERR_LLDTESTS_SET_STATE_CB,
  ERR_LLDTESTS_ERASE_PERSISTENT_INFO,
  ERR_LLDTESTS_CHECK_WIRELESS
  } ErrAppliIdEnum_t;
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* External variables --------------------------------------------------------*/
/* USER CODE BEGIN EV */

/* USER CODE END EV */

/* Exported macros ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions ------------------------------------------------------- */
void APP_LLDTESTS_Init( void );
void APP_LLDTESTS_Error(uint32_t ErrId, uint32_t ErrCode);
void APP_LLDTESTS_ProcessMsgM0ToM4(void);
void APP_LLDTESTS_Init_UART_CLI(void);
void APP_LLDTESTS_DeInit_UART_CLI(void);
void APP_LLDTESTS_Init_TL(void);
/**
 * @brief Active polling for a given delay
 * @param microsec the delay in us unit
 **/
void us_delay_16m(uint32_t microsec);
void us_delay_32m(uint32_t microsec);
#ifdef USE_SYS_CLOCK_DIV_2
#define us_delay us_delay_16m
#else
#define us_delay us_delay_32m
#endif
/* USER CODE BEGIN EF */

/* USER CODE END EF */

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* APP_LLD_TESTS_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
