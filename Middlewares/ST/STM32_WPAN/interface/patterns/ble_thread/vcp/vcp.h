/**
 ******************************************************************************
 * @file    vcp.h
 * @author  MCD Application Team
 * @brief   VCP Interfaces
 ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics. 
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the 
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
 */


/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __VCP_H
#define __VCP_H

#ifdef __cplusplus
extern "C"
{
#endif

/* Includes ------------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* External variables --------------------------------------------------------*/
/* Exported macros -----------------------------------------------------------*/

/* Exported functions ------------------------------------------------------- */

/**
 * @brief Initialization of the traces over USB (VCP)
 *
 * @param  p_tx_buffer : Address of the buffer where the data to be transmitted over USB are written
 * @param  p_rx_buffer : Address the buffer where the data to be received over USB shall be written
 * @retval None
 */
void VCP_Init( uint8_t *p_tx_buffer , uint8_t *p_rx_buffer );

/**
 * @brief Send the traces over USB (VCP)
 *
 * @param  p_data : Address of the traces to be sent
 * @param  size   : Size of the traces to be sent
 * @param  cb     : CallBack to be called when the data is sent over USB.
 *                  The USB driver shall be ready to transmit new data
 * @retval None
 */
void VCP_SendData ( uint8_t *p_data , uint16_t size , void (*cb)( void ) );

/**
 * @brief Report to the application the data received over USB (VCP)
 *
 * @param  Buf : Address of the data received
 * @param  Len : Address where the length of received data can be read
 *
 * @retval None
 */
extern void VCP_DataReceived( uint8_t* Buf , uint32_t *Len );

/**
 * @brief Report to the application data transmission complete over USB (VCP)
 *
 * @param  Buf : Address of the data transmitted
 * @param  Len : length of transmitted data
 * @param  epnum : endpoint number
 *
 * @retval None
 */
extern  int8_t VCP_TransmitCplt( uint8_t *Buf, uint32_t *Len, uint8_t epnum );


#ifdef __cplusplus
}
#endif

#endif /*__VCP_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
