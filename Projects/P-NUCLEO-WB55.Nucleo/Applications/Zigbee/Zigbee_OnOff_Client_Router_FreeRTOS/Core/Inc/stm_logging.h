/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    stm_logging.h
  * @author  MCD Application Team
  * @brief   Application header file for logging
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

#ifndef STM_LOGGING_H_
#define STM_LOGGING_H_

#define LOG_LEVEL_NONE  0  /* None     */
#define LOG_LEVEL_CRIT  1U  /* Critical */
#define LOG_LEVEL_WARN  2U  /* Warning  */
#define LOG_LEVEL_INFO  3U  /* Info     */
#define LOG_LEVEL_DEBG  4U  /* Debug    */

#define APP_DBG_FULL(level, region, ...)                                                    \
  {                                                                                         \
    if (APPLI_PRINT_FILE_FUNC_LINE == 1U)                                                   \
    {                                                                                       \
        printf("\r\n[%s][%s][%d] ", DbgTraceGetFileName(__FILE__),__FUNCTION__,__LINE__);   \
    }                                                                                       \
    logApplication(level, region, __VA_ARGS__);                                             \
  }
#define APP_DBG(...)                                                                        \
  {                                                                                         \
    if (APPLI_PRINT_FILE_FUNC_LINE == 1U)                                                   \
    {                                                                                       \
        printf("\r\n[%s][%s][%d] ", DbgTraceGetFileName(__FILE__),__FUNCTION__,__LINE__);   \
    }                                                                                       \
    logApplication(LOG_LEVEL_NONE, APPLI_LOG_REGION_GENERAL, __VA_ARGS__);                  \
  }
/**
 * This enumeration represents log regions.
 *
 */
typedef enum
{
  APPLI_LOG_REGION_GENERAL                    = 1U,  /* General                 */
  APPLI_LOG_REGION_ZIGBEE_API                 = 2U,  /* Zigbee API              */
} appliLogRegion_t;

typedef uint8_t appliLogLevel_t;

void logApplication(appliLogLevel_t aLogLevel, appliLogRegion_t aLogRegion, const char *aFormat, ...);

#endif  /* STM_LOGGING_H_ */
