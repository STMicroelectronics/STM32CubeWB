/**
  ******************************************************************************
  * @file    chacha20_core.h
  * @author  MCD Application Team
  * @brief   Implements the core chacha functionality
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2015 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Image license SLA0044,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        http://www.st.com/SLA0044
  *
  ******************************************************************************
  */

/*
*  The implementation of chacha20 is just an adapatation (for consistency with
*  the library) of code coming from: chacha-ref.c version 20080118 D. J. Bernstein
*  Which was released in Public domain.
*/
#ifndef __CHACHA20_CORE_H__
#define __CHACHA20_CORE_H__

/* Includes ------------------------------------------------------------------*/

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/

/** @addtogroup SymKeylowlevel
* @{
*/

/** @defgroup CHACHAlowlevel ChaCha
* @brief Internal ChaCha Functions and Macros
* @{
*/

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

void salsa20_wordtobyte(uint8_t output[64],\
                        const uint32_t input[16]);

/**
* @} SymKeylowlevel
*/

/**
* @} CHACHAlowlevel
*/
#endif /* __CHACHA20_CORE_H__ */


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
