/**
 ******************************************************************************
 * @file    svc_ctl.c
 * @author  MCD Application Team
 * @brief   BLE Controller
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


/* Includes ------------------------------------------------------------------*/
#include "common_blesvc.h"

/* Private typedef -----------------------------------------------------------*/
typedef struct
{
#if (BLE_CFG_SVC_MAX_NBR_CB > 0)
SVC_CTL_p_EvtHandler_t SVCCTL__SvcHandlerTab[BLE_CFG_SVC_MAX_NBR_CB];
#endif
uint8_t NbreOfRegisteredHandler;
} SVCCTL_EvtHandler_t;

typedef struct
{
#if (BLE_CFG_CLT_MAX_NBR_CB > 0)
SVC_CTL_p_EvtHandler_t SVCCTL_CltHandlerTable[BLE_CFG_CLT_MAX_NBR_CB];
#endif
uint8_t NbreOfRegisteredHandler;
} SVCCTL_CltHandler_t;

/* Private defines -----------------------------------------------------------*/
#define SVCCTL_EGID_EVT_MASK   0xFF00
#define SVCCTL_GATT_EVT_TYPE   0x0C00
#define SVCCTL_GAP_DEVICE_NAME_LENGTH 7

/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/**
 * START of Section BLE_DRIVER_CONTEXT
 */

PLACE_IN_SECTION("BLE_DRIVER_CONTEXT") SVCCTL_EvtHandler_t SVCCTL_EvtHandler;
PLACE_IN_SECTION("BLE_DRIVER_CONTEXT") SVCCTL_CltHandler_t SVCCTL_CltHandler;

/**
 * END of Section BLE_DRIVER_CONTEXT
 */

/* Private functions ----------------------------------------------------------*/
/* Weak functions ----------------------------------------------------------*/
void BVOPUS_STM_Init(void);

__weak void BLS_Init( void )
{
  return;
}
__weak void CRS_STM_Init( void )
{
  return;
}
__weak void DIS_Init( void )
{
  return;
}
__weak void EDS_STM_Init( void )
{
  return;
}
__weak void HIDS_Init( void )
{
  return;
}
__weak void HRS_Init( void )
{
  return;
}
__weak void HTS_Init( void )
{
  return;
}
__weak void IAS_Init( void )
{
  return;
}
__weak void LLS_Init( void )
{
  return;
}
__weak void TPS_Init( void )
{
  return;
}
__weak void MOTENV_STM_Init( void )
{
  return;
}
__weak void P2PS_STM_Init( void )
{
  return;
}
__weak void OTAS_STM_Init( void )
{
  return;
}
__weak void MESH_Init( void )
{
  return;
}
__weak void BVOPUS_STM_Init( void )
{
  return;
}
__weak void SVCCTL_InitCustomSvc( void )
{
  return;
}

/* Functions Definition ------------------------------------------------------*/

void SVCCTL_Init( void )
{
 
  /**
   * Initialize the number of registered Handler
   */
  SVCCTL_EvtHandler.NbreOfRegisteredHandler = 0;
  SVCCTL_CltHandler.NbreOfRegisteredHandler = 0;

  /**
   * Add and Initialize requested services
   */
  SVCCTL_SvcInit();

  return;
}

__weak void SVCCTL_SvcInit(void)
{
  BLS_Init();

  CRS_STM_Init();

  DIS_Init();

  EDS_STM_Init();

  HIDS_Init();

  HRS_Init();

  HTS_Init();

  IAS_Init();

  LLS_Init();

  TPS_Init();

  MOTENV_STM_Init();

  P2PS_STM_Init();

  OTAS_STM_Init();
  
  BVOPUS_STM_Init();

  MESH_Init();

  SVCCTL_InitCustomSvc();
  
  return;
}

/**
 * @brief  BLE Controller initialization
 * @param  None
 * @retval None
 */
void SVCCTL_RegisterSvcHandler( SVC_CTL_p_EvtHandler_t pfBLE_SVC_Service_Event_Handler )
{
#if (BLE_CFG_SVC_MAX_NBR_CB > 0)
  SVCCTL_EvtHandler.SVCCTL__SvcHandlerTab[SVCCTL_EvtHandler.NbreOfRegisteredHandler] = pfBLE_SVC_Service_Event_Handler;
#endif
  SVCCTL_EvtHandler.NbreOfRegisteredHandler++;

  return;
}

/**
 * @brief  BLE Controller initialization
 * @param  None
 * @retval None
 */
void SVCCTL_RegisterCltHandler( SVC_CTL_p_EvtHandler_t pfBLE_SVC_Client_Event_Handler )
{
#if (BLE_CFG_CLT_MAX_NBR_CB > 0)
  SVCCTL_CltHandler.SVCCTL_CltHandlerTable[SVCCTL_CltHandler.NbreOfRegisteredHandler] = pfBLE_SVC_Client_Event_Handler;
#endif
  SVCCTL_CltHandler.NbreOfRegisteredHandler++;

  return;
}

SVCCTL_UserEvtFlowStatus_t SVCCTL_UserEvtRx( void *pckt )
{
  hci_event_pckt *event_pckt;
  evt_blue_aci *blue_evt;
  SVCCTL_EvtAckStatus_t event_notification_status;
  SVCCTL_UserEvtFlowStatus_t return_status;
  uint8_t index;

  event_pckt = (hci_event_pckt*) ((hci_uart_pckt *) pckt)->data;
  event_notification_status = SVCCTL_EvtNotAck;

  switch (event_pckt->evt)
  {
    case EVT_VENDOR:
    {
      blue_evt = (evt_blue_aci*) event_pckt->data;

      switch ((blue_evt->ecode) & SVCCTL_EGID_EVT_MASK)
      {
        case SVCCTL_GATT_EVT_TYPE:
#if (BLE_CFG_SVC_MAX_NBR_CB > 0)
          /* For Service event handler */
          for (index = 0; index < SVCCTL_EvtHandler.NbreOfRegisteredHandler; index++)
          {
            event_notification_status = SVCCTL_EvtHandler.SVCCTL__SvcHandlerTab[index](pckt);
            /**
             * When a GATT event has been acknowledged by a Service, there is no need to call the other registered handlers
             * a GATT event is relevant for only one Service
             */
            if (event_notification_status != SVCCTL_EvtNotAck)
            {
              /**
               *  The event has been managed. The Event processing should be stopped
               */
              break;
            }
          }
#endif
#if (BLE_CFG_CLT_MAX_NBR_CB > 0)
          /* For Client event handler */
          event_notification_status = SVCCTL_EvtNotAck;
          for(index = 0; index <SVCCTL_CltHandler.NbreOfRegisteredHandler; index++)
          {
            event_notification_status = SVCCTL_CltHandler.SVCCTL_CltHandlerTable[index](pckt);
            /**
             * When a GATT event has been acknowledged by a Client, there is no need to call the other registered handlers
             * a GATT event is relevant for only one Client
             */
            if (event_notification_status != SVCCTL_EvtNotAck)
            {
              /**
               *  The event has been managed. The Event processing should be stopped
               */
              break;
            }
          }
#endif
          break;

        default:
          break;
      }
    }
      break; /* HCI_EVT_VENDOR_SPECIFIC */

    default:
      break;
  }

  /**
   * When no registered handlers (either Service or Client) has acknowledged the GATT event, it is reported to the application
   * a GAP event is always reported to the applicaiton.
   */
  switch (event_notification_status)
  {
    case SVCCTL_EvtNotAck:
      /**
       *  The event has NOT been managed.
       *  It shall be passed to the application for processing
       */
      return_status = SVCCTL_App_Notification(pckt);
      break;

    case SVCCTL_EvtAckFlowEnable:
      return_status = SVCCTL_UserEvtFlowEnable;
      break;

    case SVCCTL_EvtAckFlowDisable:
      return_status = SVCCTL_UserEvtFlowDisable;
      break;

    default:
      return_status = SVCCTL_UserEvtFlowEnable;
      break;
  }

  return (return_status);
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
