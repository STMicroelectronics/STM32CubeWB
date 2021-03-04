
/**
  ******************************************************************************
  * @file    app_ffd_mac_802_15_4.h
  * @author  MCD Application Team
  * @brief   Header for FFD MAC 802.15.4 Core Mac Device implementation
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
#ifndef APP_FFD_MAC_802_15_4_H
#define APP_FFD_MAC_802_15_4_H

#ifdef __cplusplus
extern "C" {
#endif

/* ---------------------------------------------------------------------------*/
/* Includes ------------------------------------------------------------------*/
/* ---------------------------------------------------------------------------*/

#include "tl.h"
#include "tl_mac_802_15_4.h"


/* ---------------------------------------------------------------------------*/
/* Exported types and defines ------------------------------------------------*/
/* ---------------------------------------------------------------------------*/


/* FFD MAC_802_15_4 application generic defines */
/*------------------------------------*/
typedef enum
{
  APP_MAC_802_15_4_LIMITED,
  APP_MAC_802_15_4_FULL,
} APP_MAC_802_15_4_InitMode_t;


typedef enum
{
  CFG_ASSO_PENDING,
  CFG_SRV_SER_REQ_NBR,
} CFG_COORD_Srv_Task_Req_t;

typedef enum
{
  CFG_DATA_PENDING,
  CFG_SRV_DATA_REQ_NBR,
} CFG_COORD_Data_Task_Req_t;

/* Application errors                 */
/*------------------------------------*/

/*
 *  List of all errors tracked by the Thread application
 *  running on M4. Some of these errors may be fatal
 *  or just warnings
 */
typedef enum
{
  ERR_APPLI_REC_MULTI_MSG_FROM_RFCore,
  ERR_INTERFACE_IPCC_SEND_ACK_TO_RFCore,
  ERR_WRONG_BINARY,
  ERR_WRONG_FIRMWARE_CHECK,
} ErrAppliIdEnum_t;

/* ---------------------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
/* ---------------------------------------------------------------------------*/
void APP_FFD_MAC_802_15_4_Init( APP_MAC_802_15_4_InitMode_t InitMode, TL_CmdPacket_t* pCmdBuffer);
void APP_FFD_MAC_802_15_4_Stop(void);
void APP_FFD_MAC_802_15_4_Error(uint32_t ErrId);
void APP_FFD_MAC_802_15_4_SetupTask(void);
void APP_FFD_MAC_802_15_4_CoordSrvTask(void);
void APP_FFD_MAC_802_15_4_CoordDataTask(void);
void APP_FFD_Init( void );
void APP_FFD_postMacInfCnfNot(const char * buf);
void APP_FFD_parseClientReqResCmd ( const char * CommandString  );



#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* __APP_FFD_MAC_802_15_4_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
