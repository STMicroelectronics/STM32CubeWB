
/**
  ******************************************************************************
  * @file    app_ble.h
  * @author  MCD Application Team
  * @brief   Header for ble application
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
#ifndef __APP_BLE_H
#define __APP_BLE_H

#ifdef __cplusplus
extern "C" {
#endif

  /* Includes ------------------------------------------------------------------*/
#include "hci_tl.h"
  
  /* Exported types ------------------------------------------------------------*/


  /* Exported constants --------------------------------------------------------*/
  /* External variables --------------------------------------------------------*/
  /* Exported macros -----------------------------------------------------------*/
  /* Exported functions ------------------------------------------------------- */
  void APP_BLE_Init( void );
      
  void APP_BLE_Key_Button1_Action(void);
  void APP_BLE_Key_Button2_Action(void);
  void APP_BLE_Key_Button3_Action(void);

#ifdef __cplusplus
}
#endif

#endif /*__APP_BLE_H */
