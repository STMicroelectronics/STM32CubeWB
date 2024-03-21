/**
 ******************************************************************************
 * @file    cli.c
 * @author  MCD Application Team
 * @brief   This file contains the CLI interface shared between M0 and
 *          M4.
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
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
#include "stm32wbxx_hal.h"

#include "stm32wbxx_core_interface_def.h"
#include "tl_thread_hci.h"
#include "entropy.h"
#include "logging.h"
#include "app_conf.h"

#include OPENTHREAD_PROJECT_CORE_CONFIG_FILE

#define OT_RADIO_2P4GHZ_OQPSK_CHANNEL_MIN       11
#define OT_RADIO_2P4GHZ_OQPSK_CHANNEL_MASK      0xffff << OT_RADIO_2P4GHZ_OQPSK_CHANNEL_MIN

void otPlatAlarmMilliStartAt(otInstance *aInstance, uint32_t aT0, uint32_t aDt);
uint32_t otPlatAlarmMilliGetNow(void);
void otPlatAlarmMilliStop(otInstance *aInstance);
otError otPlatUartEnable(void);

/* NOT IMPLEMENTED ON M0 */
otError otPlatRadioGetCcaEnergyDetectThreshold(otInstance *aInstance, int8_t *aThreshold)
{
  return OT_ERROR_NOT_IMPLEMENTED;
}

/* NOT IMPLEMENTED ON M0 */
otError otPlatRadioSetCcaEnergyDetectThreshold(otInstance *aInstance, int8_t aThreshold)
{
  return OT_ERROR_NOT_IMPLEMENTED;
}

uint32_t otPlatRadioGetSupportedChannelMask(otInstance *aInstance)
{
  return OT_RADIO_2P4GHZ_OQPSK_CHANNEL_MASK;
}

uint32_t otPlatRadioGetPreferredChannelMask(otInstance *aInstance)
{
  return otPlatRadioGetSupportedChannelMask(NULL);
}

/* NOT IMPLEMENTED ON M0 */
otError otPlatRadioGetFemLnaGain(otInstance *aInstance, int8_t *aGain)
{
  return OT_ERROR_NOT_IMPLEMENTED;
}

/* NOT IMPLEMENTED ON M0 */
otError otPlatRadioSetFemLnaGain(otInstance *aInstance, int8_t aGain)
{
  return OT_ERROR_NOT_IMPLEMENTED;
}

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

/* NOT IMPLEMENTED ON M0 */
otError otPlatRadioGetRegion(otInstance *aInstance, uint16_t *aRegionCode)
{
  return OT_ERROR_NOT_IMPLEMENTED;
}

/* NOT IMPLEMENTED ON M0 */
otError otPlatRadioSetRegion(otInstance *aInstance, uint16_t aRegionCode)
{
  return OT_ERROR_NOT_IMPLEMENTED;
}

const char *otPlatRadioGetVersionString(otInstance *aInstance)
{
  static char sVersion[100];
  snprintf(sVersion, sizeof(sVersion), "OPEN THREAD PLATFORM VERSION: %s ; %s", VERSION, PACKAGE_VERSION);
  return sVersion;
}

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

void otPlatAlarmMilliStartAt(otInstance *aInstance, uint32_t aT0, uint32_t aDt)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_ALARM_MILLI_START_AT;

  p_ot_req->Size=2;
  p_ot_req->Data[0] = (uint32_t) aT0;
  p_ot_req->Data[1] = (uint32_t) aDt;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
}


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

void otPlatAlarmMilliStop(otInstance *aInstance)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_ALARM_MILLI_STOP;

  p_ot_req->Size=0;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
}

/* NOT IMPLEMENTED ON M0 */
otError otPlatUartEnable(void)
{
  return OT_ERROR_NONE;
}

/* TO BE IMPLEMENTED */
void otLogWarnPlat(const char *aFormat, ...) {}

#if defined ( __ARMCC_VERSION ) /* KEIL with ARMCC or Clang */
// Prevent compilation error with KEIL - these functions are not called though
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
#endif /* (CFG_FULL_LOW_POWER != 0) */
#endif /* __ARMCC_VERSION */

#endif /* ENABLE_OPENTHREAD_CLI */