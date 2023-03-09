/**
  ******************************************************************************
  * @file    b_wb1m_wpan1_eeprom.h
  * @author  MCD Application Team
  * @brief   This file contains all the functions prototypes for 
  *          the b_wb1m_wpan1_eeprom.c firmware driver.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef B_WB1M_WPAN1_EEPROM_H
#define B_WB1M_WPAN1_EEPROM_H

#ifdef __cplusplus
 extern "C" {
#endif /* __cplusplus */

/* Includes ------------------------------------------------------------------*/
#include "b_wb1m_wpan1.h"

/** @addtogroup BSP
  * @{
  */

/** @addtogroup B_WB1M_WPAN1
  * @{
  */

/** @addtogroup B_WB1M_WPAN1_EEPROM B-WB1M-WPAN1 EEPROM
  * @brief This file includes the I2C EEPROM driver of B-WB1M-WPAN1 board.
  * @{
  */

/** @defgroup B_WB1M_WPAN1_EEPROM_Exported_Types B-WB1M-WPAN1 EEPROM Exported Types
  * @{
  */ 
/**
  * @}
  */

/** @defgroup B_WB1M_WPAN1_EEPROM_Exported_Constants EEPROM Exported Constants
  * @{
  */
/* EEPROM hardware address and page size */ 
#define EEPROM_PAGESIZE             ((uint8_t)64)
#define EEPROM_MAX_SIZE             ((uint16_t)0x8000) /* 256Kbit */

/* Maximum number of trials for BSP_EEPROM_WaitEepromStandbyState() function */
#define EEPROM_MAX_TRIALS           ((uint32_t)3000)

#define EEPROM_TIMEOUT              ((uint32_t)2)
/**
  * @}
  */ 
  
/** @defgroup B_WB1M_WPAN1_EEPROM_Exported_Macros EEPROM Exported Macros
  * @{
  */
/**
  * @}
  */

/** @defgroup B_WB1M_WPAN1_EEPROM_Exported_Functions EEPROM Exported Functions
  * @{
  */
int32_t BSP_EEPROM_Init(void);
int32_t BSP_EEPROM_DeInit(void);
int32_t BSP_EEPROM_ReadBuffer(uint8_t* pBuffer, uint16_t ReadAddr, uint16_t* NumByteToRead);
int32_t BSP_EEPROM_WritePage(uint8_t* pBuffer, uint16_t WriteAddr, uint8_t* NumByteToWrite);
int32_t BSP_EEPROM_WriteBuffer(uint8_t* pBuffer, uint16_t WriteAddr, uint16_t NumByteToWrite);
int32_t BSP_EEPROM_WaitEepromStandbyState(void);

/* USER Callbacks: This function is declared as __weak in EEPROM driver and 
   should be implemented into user application.  
   BSP_EEPROM_TIMEOUT_UserCallback() function is called whenever a timeout condition 
   occurs during communication (waiting on an event that doesn't occur, bus
   errors, busy devices ...). */
void     BSP_EEPROM_TIMEOUT_UserCallback(void);

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */ 

#ifdef __cplusplus
}
#endif

#endif /* B_WB1M_WPAN1_EEPROM_H */
