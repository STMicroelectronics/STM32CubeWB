
/**
 ******************************************************************************
 * @file    p2p_routeur_app.h
 * @author  MCD Application Team
 * @brief   Header for p2p routeur application module
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


/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __P2P_ROUTEUR_APP_H
#define __P2P_ROUTEUR_APP_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
typedef struct
{
    uint8_t     Device1_Status;
    uint8_t     Device2_Status;
    uint8_t     Device3_Status;
    uint8_t     Device4_Status;
    uint8_t     Device5_Status;
    uint8_t     Device6_Status;
 }P2PR_APP_Device_Status_t;

/* Exported constants --------------------------------------------------------*/
/* External variables --------------------------------------------------------*/
/* Exported macros -----------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void P2PR_APP_Init( void );
void P2PR_APP_End_Device_Mgt_Connection_Update( P2PR_APP_Device_Status_t *p_device_status );

#ifdef __cplusplus
}
#endif

#endif /*__P2P_ROUTEUR_APP_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
