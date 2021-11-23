
/**
  ******************************************************************************
 * @file    dt_server_app.h
 * @author  MCD Application Team
 * @brief   Header for dt_server_app.c module
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
#ifndef __DTS_SERVER_APP_H
#define __DTS_SERVER_APP_H

#ifdef __cplusplus
extern "C" {
#endif

#define BUTTON_PRESSED GPIO_PIN_RESET

/* Exported typedefs ---------------------------------------------*/
typedef GPIO_PinState BUTTON_STATE;
typedef void (* IO_RECEIVE_DATA_USER_CALLBACK_TYPE) (uint8_t * rx_data, uint16_t data_size);

  /* Includes ------------------------------------------------------------------*/
  /* Exported types ------------------------------------------------------------*/
  /* Exported constants --------------------------------------------------------*/
  /* External variables --------------------------------------------------------*/
  /* Exported macros -----------------------------------------------------------*/
  /* Exported functions ------------------------------------------------------- */
  void DTS_App_Init(void);
  void DTS_App_KeyButtonAction(void);
  void DTS_App_KeyButton2Action( void );
  void DTS_App_KeyButton3Action( void );
  void DTS_App_TxPoolAvailableNotification(void);


#ifdef __cplusplus
}
#endif

#endif /*__DTS_SERVER_APP_H */
