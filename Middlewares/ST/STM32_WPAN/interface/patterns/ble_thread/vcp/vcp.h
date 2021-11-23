/**
 ******************************************************************************
 * @file    vcp.h
 * @author  MCD Application Team
 * @brief   VCP Interfaces
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

