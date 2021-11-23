/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    app_ble_cl.h
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
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef APP_BLE_CL_H
#define APP_BLE_CL_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "hci_tl.h"
#include "app_ble_common.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/

/* USER CODE BEGIN ET */

/* USER CODE END ET */  

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* External variables --------------------------------------------------------*/
/* USER CODE BEGIN EV */

/* USER CODE END EV */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions ---------------------------------------------*/
  void APP_BLE_CL_Init( void ); 

  APP_BLE_ConnStatus_t APP_BLE_CL_Get_Client_Connection_Status( uint16_t Connection_Handle );
  
  SVCCTL_UserEvtFlowStatus_t SVCCTL_App_CL_Notification( void *pckt );

/* USER CODE BEGIN EF */
  void APP_BLE_CL_Key_Button1_Action(void);
  void APP_BLE_CL_Key_Button2_Action(void);
  void APP_BLE_CL_Key_Button3_Action(void);
/* USER CODE END EF */

#ifdef __cplusplus
}
#endif

#endif /*APP_BLE_CL_H */
