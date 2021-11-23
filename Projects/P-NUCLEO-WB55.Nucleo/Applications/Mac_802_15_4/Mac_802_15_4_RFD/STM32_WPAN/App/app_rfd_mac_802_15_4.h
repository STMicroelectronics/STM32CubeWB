
/**
  ******************************************************************************
  * @file    app_rfd_mac_802_15_4.h
  * @author  MCD Application Team
  * @brief   Header for RFD MAC 802.15.4 demo
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
#ifndef APP_RFD_MAC_802_15_4_H
#define APP_RFD_MAC_802_15_4_H

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


/* RFD MAC_802_15_4 application generic defines */
/*------------------------------------*/
typedef enum
{
  APP_MAC_802_15_4_LIMITED,
  APP_MAC_802_15_4_FULL,
} APP_MAC_802_15_4_InitMode_t;


/* Scheduler types and defines        */
/*------------------------------------*/

  /**
   * This is the list of task id required by the application
   * Each Id shall be in the range 0..31
   */

typedef enum
{
  CFG_TASK_RFD,              	/* Init RFD Task - mainly handling RFD Setup */
  CFG_TASK_SERVICE_NODE,    	/* RFD Service Task - Handle Service request/response */
  CFG_TASK_DATA_NODE,       	/* RFD DATA Task - Handle Service request/response */
  CFG_TASK_MSG_FROM_RF_CORE, 	/* Task handling Incoming RF Core messages. */
  CFG_TASK_SYSTEM_HCI_ASYNCH_EVT,
  CFG_TASK_NBR,  /**< Shall be last in the list */
} CFG_IdleTask_Id_t;

typedef enum
{
  CFG_SCH_PRIO_0,
  CFG_SCH_PRIO_1,
  CFG_PRIO_NBR,
} CFG_SCH_Prio_Id_t;

#define TASK_RFD                         (1U << CFG_TASK_RFD)
#define TASK_NODE_SRV                    (1U << CFG_TASK_SERVICE_NODE)
#define TASK_NODE_DATA                   (1U << CFG_TASK_DATA_NODE)
#define TASK_MSG_FROM_RF_CORE            (1U << CFG_TASK_MSG_FROM_RF_CORE)


  /**
   * This is a bit mapping over 32bits listing all events id supported in the application
   */
typedef enum
{
  CFG_EVT_SYSTEM_HCI_CMD_EVT_RESP,
  CFG_EVT_ACK_FROM_RFCore_EVT,
  CFG_EVT_SYNCHRO_BYPASS_IDLE,
  CFG_EVT_DEVICE_RESET_CNF,       /* Device Reset Confirm */
  CFG_EVT_SET_CNF,                /* SET PIB Confirm */
  CFG_EVT_ASSOCIATE_CNF,          /* DEVICE STARTED Confirm */
  CFG_EVT_DATA_DATA_CNF,          /* DATA Confirm */
} CFG_IdleEvt_Id_t;

#define EVENT_ACK_FROM_RFCore_EVT           (1U << CFG_EVT_ACK_FROM_RFCore_EVT)
#define EVENT_SYNCHRO_BYPASS_IDLE           (1U << CFG_EVT_SYNCHRO_BYPASS_IDLE)

/*Events managed by Setup Task*/
#define EVENT_DEVICE_RESET_CNF          (1U << CFG_EVT_DEVICE_RESET_CNF)
#define EVENT_SET_CNF                   (1U << CFG_EVT_SET_CNF)
#define EVENT_SHORT_ADDR_SET_CNF        (1U << CFG_EVT_SHORT_ADDR_SET_CNF)
#define EVENT_ASSO_PERMIT_SET_CNF       (1U << CFG_EVT_ASSO_PERMIT_SET_CNF)
#define EVENT_ASSOCIATE_CNF             (1U << CFG_EVT_ASSOCIATE_CNF)
#define EVENT_SET_RX_ON_WHEN_IDLE_CNF   (1U << CFG_EVT_SET_RX_ON_WHEN_IDLE_CNF)

/*Events managed by Current Node Task*/
#define EVENT_SRV_ASSOC_CNF (1U << CFG_EVT_ASSOCIATE_CNF)

/*Events managed by data Task*/
#define EVENT_DATA_CNF          (1U << CFG_EVT_DATA_DATA_CNF)


typedef enum
{
  CFG_ASSO_PENDING,
  CFG_SRV_SER_REQ_NBR,
} CFG_NODE_Srv_Task_Req_t;

typedef enum
{
  CFG_DATA_PENDING,
  CFG_SRV_DATA_REQ_NBR,
} CFG_NODE_Data_Task_Req_t;

/* Low power types and defines        */
/*------------------------------------*/

/*
 * Supported requester to the MCU Low Power Manager - can be increased up to 32
 * It lists a bit mapping of all users of the Low Power Manager
 */
typedef enum
{
  CFG_LPM_APP,
  CFG_LPM_APP_BLE,
} CFG_LPM_Id_t;

/******************************************************************************
 * OTP manager
 ******************************************************************************/
#define CFG_OTP_BASE_ADDRESS    OTP_AREA_BASE

#define CFG_OTP_END_ADRESS      OTP_AREA_END_ADDR

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
} ErrAppliIdEnum_t;

/* ---------------------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
/* ---------------------------------------------------------------------------*/
void APP_RFD_MAC_802_15_4_Init( APP_MAC_802_15_4_InitMode_t InitMode, TL_CmdPacket_t* pCmdBuffer);

void APP_RFD_MAC_802_15_4_Error(uint32_t ErrId, uint32_t ErrCode);

void APP_RFD_MAC_802_15_4_SetupTask(void);

void APP_RFD_MAC_802_15_4_SendData(const char *);

void APP_RFD_MAC_802_15_4_NodeSrvTask(void);
void APP_RFD_MAC_802_15_4_NodeDataTask(void);

void APP_RFD_Init( void );


void APP_RFD_postMacInfCnfNot(const char * buf);

void APP_RFD_parseClientReqResCmd ( const char * CommandString  );



#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* __APP_RFD_MAC_802_15_4_H */
