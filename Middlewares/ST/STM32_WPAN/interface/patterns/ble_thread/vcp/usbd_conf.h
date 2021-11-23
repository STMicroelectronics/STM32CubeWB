/**
 ******************************************************************************
 * @file    USB_Device/CDC_Standalone/Inc/usbd_conf.h
 * @author  MCD Application Team
 * @brief   General low level driver configuration
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2018-2021 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */


/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USBD_CONF_H
#define __USBD_CONF_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "app_common.h"
#undef MIN
#undef MAX

/** @addtogroup STM32_USB_DEVICE_LIBRARY
  * @{
  */

/** @defgroup USBD_CONF
  * @brief USB device low level driver configuration file
  * @{
  */

/** @defgroup USBD_CONF_Exported_Defines
  * @{
  */


/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Common Config */
#define USBD_MAX_NUM_INTERFACES                1U
#define USBD_MAX_NUM_CONFIGURATION             1U
#define USBD_MAX_STR_DESC_SIZ                  0x100U

#define USBD_SELF_POWERED                      1U
#define USBD_DEBUG_LEVEL                       0U

/* ECM, RNDIS, DFU Class Config */
#define USBD_SUPPORT_USER_STRING_DESC          0U


/* Exported macro ------------------------------------------------------------*/
/* Memory management macros */   

/* For footprint reasons and since only one allocation is handled in the CDC class 
   driver, the malloc/free is changed into a static allocation method */

void *USBD_static_malloc(uint32_t size);
void USBD_static_free(void *p);

#define MAX_STATIC_ALLOC_SIZE     136 /*CDC Class Driver Structure size*/

/** @defgroup USBD_Exported_Macros
  * @{
  */

/* Memory management macros */
#define USBD_malloc               malloc
#define USBD_free                 free
#define USBD_memset               memset
#define USBD_memcpy               memcpy
#define USBD_Delay                HAL_Delay

/* DEBUG macros */
#if (USBD_DEBUG_LEVEL > 0U)
#define  USBD_UsrLog(...)   do { \
                            printf(__VA_ARGS__); \
                            printf("\n"); \
} while (0)
#else
#define USBD_UsrLog(...) do {} while (0)
#endif

#if (USBD_DEBUG_LEVEL > 1U)

#define  USBD_ErrLog(...) do { \
                            printf("ERROR: ") ; \
                            printf(__VA_ARGS__); \
                            printf("\n"); \
} while (0)
#else
#define USBD_ErrLog(...) do {} while (0)
#endif

#if (USBD_DEBUG_LEVEL > 2U)
#define  USBD_DbgLog(...)   do { \
                            printf("DEBUG : ") ; \
                            printf(__VA_ARGS__); \
                            printf("\n"); \
} while (0)
#else
#define USBD_DbgLog(...) do {} while (0)
#endif

/**
  * @}
  */



/**
  * @}
  */


/** @defgroup USBD_CONF_Exported_Types
  * @{
  */
/**
  * @}
  */


/** @defgroup USBD_CONF_Exported_Macros
  * @{
  */
/**
  * @}
  */

/** @defgroup USBD_CONF_Exported_Variables
  * @{
  */
/**
  * @}
  */

/** @defgroup USBD_CONF_Exported_FunctionsPrototype
  * @{
  */
/**
  * @}
  */

#ifdef __cplusplus
}
#endif
#endif /* __USBD_CONF_H */


/**
  * @}
  */

/**
  * @}
  */
