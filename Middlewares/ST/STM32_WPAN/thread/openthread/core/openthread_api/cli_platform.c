/**
 ******************************************************************************
 * @file    cli_platform.c
 * @author  MCD Application Team
 * @brief   This file contains the functions to enable the OpenThread Command
 *          Line Interface on M4 processor.
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2024 STMicroelectronics.
 * All rights reserved.</center></h2>
 *
 * This software component is licensed by ST under Ultimate Liberty license
 * SLA0044, the "License"; You may not use this file except in compliance with
 * the License. You may obtain a copy of the License at:
 *                             www.st.com/SLA0044
 *
 ******************************************************************************
 */

#if defined(ENABLE_OPENTHREAD_CLI) || defined(__ARMCC_VERSION)

/* Includes ------------------------------------------------------------------*/
#include "alarm-milli.h"
#include "app_conf.h"
#include "entropy.h"
#include "logging.h"
#include "stm32wbxx_core_interface_def.h"
#include "stm32wbxx_hal.h"
#include "tl_thread_hci.h"
#include "uart.h"

#include OPENTHREAD_PROJECT_CORE_CONFIG_FILE

/* Private defines -----------------------------------------------------------*/
#define OT_RADIO_2P4GHZ_OQPSK_CHANNEL_MASK      0x07FFF800      // Represent channels 11 to 26

/* Private variables ---------------------------------------------------------*/
static uint8_t alarmMilliTimerID;       // Alarm ID used for HW_timeserver
static otInstance *g_aInstance;         // OpenThread instance pointer

/* Functions Definition ------------------------------------------------------*/
/**
 * @brief  This function is not implemented on M0 and is only necessary to build.
 * @retval OT_ERROR_NOT_IMPLEMENTED
 */
__weak otError otPlatRadioGetCcaEnergyDetectThreshold(otInstance *aInstance, int8_t *aThreshold)
{
  return OT_ERROR_NOT_IMPLEMENTED;
}

/**
 * @brief  This function is not implemented on M0 and is only necessary to build.
 * @retval OT_ERROR_NOT_IMPLEMENTED
 */
__weak otError otPlatRadioSetCcaEnergyDetectThreshold(otInstance *aInstance, int8_t aThreshold)
{
  return OT_ERROR_NOT_IMPLEMENTED;
}

/**
 * @brief  Get the radio supported channel mask that the device is allowed to be on.
 * @param  aInstance   The OpenThread instance structure.
 * @retval The radio supported channel mask.
 */
uint32_t otPlatRadioGetSupportedChannelMask(otInstance *aInstance)
{
  return OT_RADIO_2P4GHZ_OQPSK_CHANNEL_MASK;
}

/**
 * @brief  Gets the radio preferred channel mask that the device prefers to form on.
 * @param  aInstance   The OpenThread instance structure.
 * @retval The radio preferred channel mask.
 */
uint32_t otPlatRadioGetPreferredChannelMask(otInstance *aInstance)
{
  return otPlatRadioGetSupportedChannelMask(NULL);
}

/**
 * @brief  This function is not implemented on M0 and is only necessary to build.
 * @retval OT_ERROR_NOT_IMPLEMENTED
 */
otError otPlatRadioGetFemLnaGain(otInstance *aInstance, int8_t *aGain)
{
  return OT_ERROR_NOT_IMPLEMENTED;
}

/**
 * @brief  This function is not implemented on M0 and is only necessary to build.
 * @retval OT_ERROR_NOT_IMPLEMENTED
 */
otError otPlatRadioSetFemLnaGain(otInstance *aInstance, int8_t aGain)
{
  return OT_ERROR_NOT_IMPLEMENTED;
}

/**
 * @brief  Get the status of promiscuous mode.
 * @param  aInstance  The OpenThread instance structure.
 * @retval TRUE if promiscuous mode is enabled, FALSE otherwise
 */
bool otPlatRadioGetPromiscuous(otInstance *aInstance)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_GET_PROMISCUOUS;

  p_ot_req->Size=0;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (bool) p_ot_req->Data[0];
}

/**
 * @brief  This function is not implemented on M0 and is only necessary to build.
 * @retval OT_ERROR_NOT_IMPLEMENTED
 */
otError otPlatRadioGetRegion(otInstance *aInstance, uint16_t *aRegionCode)
{
  return OT_ERROR_NOT_IMPLEMENTED;
}

/**
 * @brief  This function is not implemented on M0 and is only necessary to build.
 * @retval OT_ERROR_NOT_IMPLEMENTED
 */
otError otPlatRadioSetRegion(otInstance *aInstance, uint16_t aRegionCode)
{
  return OT_ERROR_NOT_IMPLEMENTED;
}

/**
 * @brief  Get the radio version string.
 * @param  aInstance   The OpenThread instance structure.
 * @retval A pointer to the OpenThread radio version.
 */
const char *otPlatRadioGetVersionString(otInstance *aInstance)
{
  static char sVersion[100];
  snprintf(sVersion, sizeof(sVersion), "OPEN THREAD PLATFORM VERSION: %s ; %s", VERSION, PACKAGE_VERSION);
  return sVersion;
}

/**
 * @brief  Fill buffer with entropy.
 * @param  aOutput         A pointer to where the true random values are placed.  Must not be NULL.
 * @param  aOutputLength   Size of @p aBuffer.
 * @retval OT_ERROR_NONE, OT_ERROR_FAILED or OT_ERROR_INVALID_ARGS
 */
otError otPlatEntropyGet(uint8_t *aOutput, uint16_t aOutputLength)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_ENTROPY_GET;

  p_ot_req->Size=2;
  p_ot_req->Data[0] = (uint32_t) aOutput;
  p_ot_req->Data[1] = (uint32_t) aOutputLength;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (otError) p_ot_req->Data[0];
}

/**
 * @brief  Callback function when timer set in otPlatAlarmMilliStartAt() is fired
 * @param  None
 * @retval None
 */
static void callbackOtPlatAlarmMilliFired(void)
{
  otPlatAlarmMilliFired(g_aInstance);
}

/**
 * @brief  Set the alarm to fire at @p aDt milliseconds after @p aT0.
 * @param  aInstance  The OpenThread instance structure.
 * @param  aT0        The reference time.
 * @param  aDt        The time delay in milliseconds from @p aT0.
 * @retval None
 */
void otPlatAlarmMilliStartAt(otInstance *aInstance, uint32_t aT0, uint32_t aDt)
{
  // Keep instance pointer to set it back in callback function
  g_aInstance = aInstance;

  HW_TS_Create(CFG_TIM_PROC_ID_ISR, &alarmMilliTimerID, hw_ts_SingleShot, callbackOtPlatAlarmMilliFired);
  HW_TS_Start(alarmMilliTimerID, aDt);
}

/**
 * @brief  Get the current time.
 * @param  None
 * @retval The current time in milliseconds.
 */
uint32_t otPlatAlarmMilliGetNow(void)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_ALARM_MILLI_GET_NOW;

  p_ot_req->Size=0;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (uint32_t) p_ot_req->Data[0];
}

/**
 * @brief  Stop the alarm.
 * @param  aInstance  The OpenThread instance structure.
 * @retval None
 */
void otPlatAlarmMilliStop(otInstance *aInstance)
{
  HW_TS_Stop(alarmMilliTimerID);
  HW_TS_Delete(alarmMilliTimerID);
}

/**
 * @brief  This function is not implemented on M0 and is only necessary to build.
 * @retval OT_ERROR_NONE
 */
__weak otError otPlatUartEnable(void)
{
  return OT_ERROR_NONE;
}

/**
 * @brief  This function is not implemented on M0 and is only necessary to build.
 * @retval None
 */
__weak void otLogWarnPlat(const char *aFormat, ...) {}

#if defined ( __ARMCC_VERSION ) /* KEIL with ARMCC or Clang */
/* 
  These functions are defined empty or return 0 to prevent compilation errors 
  with KEIL, even if none of them are used. They are discarded at link obviously.
*/
void otLogDebgPlat(const char *aFormat, ...) {}
void otPlatReset(otInstance *aInstance) {}
void otPlatLog(otLogLevel aLogLevel, otLogRegion aLogRegion, const char *aFormat, ...) {}
void otTaskletsSignalPending(otInstance *aInstance) {}
uint32_t otPlatAlarmMicroGetNow(void) { return 0; }
void otPlatAlarmMicroStartAt(otInstance *aInstance, uint32_t aT0, uint32_t aDt) {}
void otPlatAlarmMicroStop(otInstance *aInstance) {}
void otPlatRadioSetPromiscuous(otInstance *aInstance, bool aEnable) {}
otRadioCaps otPlatRadioGetCaps(otInstance *aInstance) { return 0; }
otRadioFrame *otPlatRadioGetTransmitBuffer(otInstance *aInstance) { return 0; }
void otPlatRadioSetPanId(otInstance *aInstance, otPanId aPanId) {}
otError otPlatRadioEnable(otInstance *aInstance) { return 0; }
otError otPlatRadioSleep(otInstance *aInstance) { return 0; }
otError otPlatRadioDisable(otInstance *aInstance) { return 0; }
otError otPlatRadioReceive(otInstance *aInstance, uint8_t aChannel) { return 0; }
int8_t otPlatRadioGetRssi(otInstance *aInstance) { return 0; }
int8_t otPlatRadioGetReceiveSensitivity(otInstance *aInstance) { return 0; }
otError otPlatRadioEnergyScan(otInstance *aInstance, uint8_t aScanChannel, uint16_t aScanDuration) { return 0; }
otError otPlatRadioEnableCsl(otInstance *aInstance, uint32_t aCslPeriod, otShortAddress aShortAddr, const otExtAddress *aExtAddr) { return 0; }
void otPlatRadioUpdateCslSampleTime(otInstance *aInstance, uint32_t aCslSampleTime) {}
void otPlatRadioGetIeeeEui64(otInstance *aInstance, uint8_t *aIeeeEui64) {}
void otPlatRadioSetExtendedAddress(otInstance *aInstance, const otExtAddress *aExtAddress) {}
void otPlatRadioSetShortAddress(otInstance *aInstance, otShortAddress aShortAddress) {}
otError otPlatRadioTransmit(otInstance *aInstance, otRadioFrame *aFrame) { return 0; }
otError otPlatRadioConfigureEnhAckProbing(otInstance *aInstance, otLinkMetrics aLinkMetrics, otShortAddress aShortAddress, const otExtAddress *aExtAddress) { return 0; }
void otPlatRadioEnableSrcMatch(otInstance *aInstance, bool aEnable) {}
void otPlatFlashInit(otInstance *aInstance) {}
uint32_t otPlatFlashGetSwapSize(otInstance *aInstance) { return 0; }
void otPlatFlashRead(otInstance *aInstance, uint8_t aSwapIndex, uint32_t aOffset, void *aData, uint32_t aSize) {}
void otPlatFlashErase(otInstance *aInstance, uint8_t aSwapIndex) {}
void otPlatFlashWrite(otInstance *aInstance, uint8_t aSwapIndex, uint32_t aOffset, const void *aData, uint32_t aSize) {}
#if (CFG_FULL_LOW_POWER != 0) || !defined(ENABLE_OPENTHREAD_CLI)
otError otPlatUartFlush(void)	 { return 0; }
otError otPlatUartSend(const uint8_t *aBuf, uint16_t aBufLength) { return 0; }
#endif /* (CFG_FULL_LOW_POWER != 0) || !defined(ENABLE_OPENTHREAD_CLI) */
#endif /* __ARMCC_VERSION */

#endif /* ENABLE_OPENTHREAD_CLI || __ARMCC_VERSION */