/**
 ******************************************************************************
 * @file    stm_logging.h
 * @author  MCD Application Team
 * @brief   Application header file for logging
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


#ifndef STM_LOGGING_H_
#define STM_LOGGING_H_

#define LOG_LEVEL_NONE  0U  /* None     */
#define LOG_LEVEL_CRIT  1U  /* Critical */
#define LOG_LEVEL_WARN  2U  /* Warning  */
#define LOG_LEVEL_INFO  3U  /* Info     */
#define LOG_LEVEL_DEBG  4U  /* Debug    */


#define APP_DBG_FULL(level, region, ...)       \
    if (APPLI_PRINT_FILE_FUNC_LINE == 1U)                  \
    {                                           \
        printf("\r\n[%s][%s][%d] ", DbgTraceGetFileName(__FILE__),__FUNCTION__,__LINE__);   \
    }                                           \
    logApplication(level, region, __VA_ARGS__);

#define APP_DBG(...)       \
    if (APPLI_PRINT_FILE_FUNC_LINE == 1U)                  \
    {                                           \
        printf("\r\n[%s][%s][%d] ", DbgTraceGetFileName(__FILE__),__FUNCTION__,__LINE__);   \
    }                                           \
    logApplication(LOG_LEVEL_NONE, APPLI_LOG_REGION_GENERAL, __VA_ARGS__);

/**
 * This enumeration represents log regions.
 *
 */
typedef enum 
{
  APPLI_LOG_REGION_GENERAL                    = 1U,  /* General                 */
  APPLI_LOG_REGION_OPENTHREAD_API             = 2U,  /* OpenThread API          */
  APPLI_LOG_REGION_OT_API_LINK                = 3U,  /* OpenThread Link API     */
  APPLI_LOG_REGION_OT_API_INSTANCE            = 4U,  /* OpenThread Instance API */
  APPLI_LOG_REGION_OT_API_MESSAGE             = 5U   /* OpenThread Message API  */
} appliLogRegion_t;

typedef uint8_t appliLogLevel_t;

void logApplication(appliLogLevel_t aLogLevel, appliLogRegion_t aLogRegion, const char *aFormat, ...);

#endif  /* STM_LOGGING_H_ */

