/**
  ******************************************************************************
  * @file    app_entry.h
  * @author  MCD Application Team
  * @brief   This file contains the headers of functions used for the 
  *          overall system infrastructure (scheduler, synchro with the M0, etc..)
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

 
 
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef APP_ENTRY_H
#define APP_ENTRY_H



#ifdef __cplusplus
extern "C" {
#endif




void APPE_Init( void );
void ProcessTrace(void);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* APP_ENTRY_H */
