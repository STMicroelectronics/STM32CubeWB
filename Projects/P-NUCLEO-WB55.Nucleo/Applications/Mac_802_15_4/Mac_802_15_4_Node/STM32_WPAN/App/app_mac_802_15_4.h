
/**
  ******************************************************************************
  * @file    app_ffd_mac_802_15_4.h
  * @author  MCD Application Team
  * @brief   Header for FFD MAC 802.15.4 Core Mac Device implementation
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


/* Scheduler types and defines        */
/*------------------------------------*/

  /**
   * This is the list of task id required by the application
   * Each Id shall be in the range 0..31
   */

typedef enum
{
  CFG_TASK_SETUP,              /* Init FFD Task - mainly handling FFD Setup */
  CFG_TASK_SERVICE_COORD,    /* FFD Service Task - Handle Service request/response */
  CFG_TASK_DATA_COORD,       /* FFD DATA Task - Handle Service request/response */
  CFG_TASK_MSG_FROM_RF_CORE, 
  CFG_TASK_SYSTEM_HCI_ASYNCH_EVT,
  CFG_TASK_DATA_POOL,
  CFG_TASK_BUTTON_1,
  CFG_TASK_BUTTON_2,
  CFG_TASK_BUTTON_3,
  CFG_TASK_NBR,  /**< Shall be last in the list */
} CFG_IdleTask_Id_t;

typedef enum
{
  CFG_SCH_PRIO_0,
  CFG_SCH_PRIO_1,
  CFG_SCH_PRIO_NBR,
} CFG_SCH_Prio_Id_t;

#define TASK_SETUP                         (1U << CFG_TASK_SETUP)
#define TASK_BUTTON_1                    (1U << CFG_TASK_BUTTON_1)
#define TASK_BUTTON_2                    (1U << CFG_TASK_BUTTON_2)
#define TASK_BUTTON_3                    (1U << CFG_TASK_BUTTON_3)
#define TASK_DATA_POOL                   (1U << CFG_TASK_DATA_POOL)
#define TASK_MSG_FROM_RF_CORE            (1U << CFG_TASK_MSG_FROM_RF_CORE)


  /**
   * This is a bit mapping over 32bits listing all events id supported in the application
   */
typedef enum
{
  CFG_EVT_SYSTEM_HCI_CMD_EVT_RESP,
  CFG_EVT_ACK_FROM_RFCore_EVT,
  CFG_EVT_SYNCHRO_BYPASS_IDLE,
  CFG_EVT_RESET_CNF,       /* Device Reset Confirm */
  CFG_EVT_SET_CNF,                /* SET PIB Confirm */
  CFG_EVT_GET_CNF,                /* GET PIB CNF */
  CFG_EVT_START_CNF,     /* DEVICE STARTED Confirm */
  CFG_EVT_RX_ON_WHEN_IDLE_CNF,    /* RX ON WHEN IDLE CNF */
  CFG_EVT_ASSOCIATE_CNF,          /* ASSOCIATE CONF */
  CFG_EVT_DATA_CNF,               /* DATA CNF */
  CFG_EVT_SCAN_CNF,               /* SCAN CNF */
  CFG_EVT_POLL_CNF,               /* POLL CNF */
  CFG_EVT_DISASSOCIATE_CNF,       /* DISASSOCIATE CNF */
  CFG_EVT_PURGE_CNF,              /* PURGE CNF */
} CFG_IdleEvt_Id_t;

#define EVENT_ACK_FROM_RFCore_EVT           (1U << CFG_EVT_ACK_FROM_RFCore_EVT)
#define EVENT_SYNCHRO_BYPASS_IDLE           (1U << CFG_EVT_SYNCHRO_BYPASS_IDLE)

/*Events managed by Setup Task*/
#define EVENT_RESET_CNF                 (1U << CFG_EVT_RESET_CNF)
#define EVENT_SET_CNF                   (1U << CFG_EVT_SET_CNF)
#define EVENT_GET_CNF                   (1U << CFG_EVT_GET_CNF)
#define EVENT_ASSOCIATE_CNF             (1U << CFG_EVT_ASSOCIATE_CNF)
#define EVENT_START_CNF                 (1U << CFG_EVT_START_CNF)
#define EVENT_RX_ON_WHEN_IDLE_CNF       (1U << CFG_EVT_RX_ON_WHEN_IDLE_CNF)
#define EVENT_SCAN_CNF                  (1U << CFG_EVT_SCAN_CNF)
#define EVENT_POLL_CNF                  (1U << CFG_EVT_POLL_CNF)
#define EVENT_DISASSOCIATE_CNF          (1U << CFG_EVT_DISASSOCIATE_CNF)
#define EVENT_DATA_CNF                  (1U << CFG_EVT_DATA_CNF)
#define EVENT_PURGE_CNF                 (1U << CFG_EVT_PURGE_CNF)

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
void APP_MAC_802_15_4_Init( APP_MAC_802_15_4_InitMode_t InitMode, TL_CmdPacket_t* pCmdBuffer);
void APP_MAC_802_15_4_Error(uint32_t ErrId, uint32_t ErrCode);
void APP_MAC_802_15_4_SetupTask(void);
void APPE_Button1Action(void);
void APPE_Button2Action(void);
void APPE_Button3Action(void);
void APP_Init(void);
void APP_MAC_802_15_4_poll(void);
void APP_MAC_802_15_4_Association(void);
void APP_MAC_802_15_4_Disassociation(void);
void APP_MAC_802_15_4_SendData_broacast(const char * data);

#define MAC_RETRY_SCAN           0x03
#define BASE_SCAN_DURATION       0x05
#define BEACON_PAYLOAD           "BZH"
#define BEACON_PAYLOAD_SIZE      0x03  

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* __APP_FFD_MAC_802_15_4_H */
