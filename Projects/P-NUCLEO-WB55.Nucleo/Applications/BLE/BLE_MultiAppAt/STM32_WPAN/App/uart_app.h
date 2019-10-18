/* USER CODE BEGIN */
/**
 ******************************************************************************
 * File Name          : App/uart_app.h
 * Description        : Header for uart_app.c module
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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
