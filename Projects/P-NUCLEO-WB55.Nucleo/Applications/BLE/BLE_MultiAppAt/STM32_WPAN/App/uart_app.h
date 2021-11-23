/* USER CODE BEGIN */
/**
  ******************************************************************************
  * File Name          : App/uart_app.h
  * Description        : Header for uart_app.c module
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
#ifndef __UART_APP_H
#define __UART_APP_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/

/* Private includes ----------------------------------------------------------*/


/* Exported functions ---------------------------------------------*/
void UART_App_Init( void );
char* UART_App_ConvertBleDataToString(const uint8_t *in, char *out, size_t len);
void UART_App_SendData(char * message, uint8_t len);

/* Exported typedef ---------------------------------------------*/

typedef struct {
  uint16_t Measurement;
  uint16_t EnergyExpended;
} HR_Notify_Context_t;

typedef struct
{
  uint8_t PairingConfirmRequested;  
  uint8_t PairingComplete;
} PairingContext_t;


#ifdef __cplusplus
}
#endif

#endif /*__UART_APP_H */

