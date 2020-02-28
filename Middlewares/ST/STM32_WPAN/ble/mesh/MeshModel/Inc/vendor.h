/**
******************************************************************************
* @file    vendor.h
* @author  BLE Mesh Team
* @version V1.12.000
* @date    06-12-2019
* @brief   Header file for the user application file 
******************************************************************************
* @attention
*
* <h2><center>&copy; COPYRIGHT(c) 2017 STMicroelectronics</center></h2>
*
* Redistribution and use in source and binary forms, with or without modification,
* are permitted provided that the following conditions are met:
*   1. Redistributions of source code must retain the above copyright notice,
*      this list of conditions and the following disclaimer.
*   2. Redistributions in binary form must reproduce the above copyright notice,
*      this list of conditions and the following disclaimer in the documentation
*      and/or other materials provided with the distribution.
*   3. Neither the name of STMicroelectronics nor the names of its contributors
*      may be used to endorse or promote products derived from this software
*      without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
* AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
* IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
* DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
* FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
* DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
* SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
* CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
* OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
* OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*
* Initial BLE-Mesh is built over Motorola’s Mesh over Bluetooth Low Energy 
* (MoBLE) technology. The present solution is developed and maintained for both 
* Mesh library and Applications solely by STMicroelectronics.
*
******************************************************************************
*/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __VENDOR_H
#define __VENDOR_H

/* Includes ------------------------------------------------------------------*/
#include "types.h"
#include "ble_mesh.h"

/* Exported macro ------------------------------------------------------------*/

/* Refer to 3.7.2 Model identifier */
#define VENDOR_STMICRO_CID     0x0030      /* 16-bit Company Identifier */
#define VENDOR_STMICRO_MODEL_ID1    0x0001 /* 16-bit vendor-assigned model identifier */
#define VENDORMODEL_STMICRO_ID1  (MOBLEUINT32)(((MOBLEUINT32)VENDOR_STMICRO_MODEL_ID1 << 16)|(VENDOR_STMICRO_CID)) 

/******************* Read, write and response mask for Vendor opcode **********/
#define VENDOR_CMD_RESPONSE                          0x20
#define VENDOR_CMD_READ_nWRITE                       0x10
/******************************************************************************/

/*******************Commands Received from Android/IoS*************************/
#define APPLI_TEST_CMD                               0x1U
#define APPLI_DEVICE_INFO_CMD                        0x2U
#define APPLI_LED_CONTROL_STATUS_CMD                 0x3U
#define APPLI_ELEMENT_TYPE_CMD                       0x4U
#define APPLI_SENSOR_CNTRL_CMD                       0X5U
#define APPLI_DATA_CNTRL_CMD                         0xEU
/******************************************************************************/

/****************Data Received from Android/iOS. B0 = SubCommand***************/
/********************* Sub Commands for APPLI_DATA_WRITE*************************/
#define APPLI_STRING_WRITE                     0X01
#define APPLI_STRING_READ                      0X02

/****************Data Received from Android/iOS. B0 = SubCommand***************/
/********************* Sub Commands for APPLI_TEST_CMD*************************/
#define APPLI_RESET_TEST_PARAMETERS        0x01U
#define APPLI_TEST_ECHO                    0x02U
#define APPLI_TEST_RANDOMIZATION_RANGE     0x03U
#define APPLI_TEST_COUNTER                 0x04U
#define APPLI_TEST_INC_COUNTER             0x05U
#define APPLI_MODEL_PUBLISH_SELECT         0X06U
#define APPLI_OTA_ENABLE                   0x07U
#define APPLI_OTA_ENTER                    0x08U
/******************************************************************************/

/****************Data Received from Android/IoS. B0 = SubCommand***************/
/*******************Sub Commands for APPLI_DEVICE_INFO_CMD*********************/
#define IC_TYPE                   0x01U
#define LIB_VER                   0x02U
#define LIB_SUB_VER               0x03U
#define APPLICATION_VER           0X04U
#define APPLI_TX_POWER_RSSIF      0x05U
/******************************************************************************/
     
/************Sub Commands for IC_TYPE. B1 = IC_TYPE SubCommand*****************/        
#define BLUENRG1_BRD              0X01U
#define BLUENRG2_BRD              0X02U
#define BLUENRG_MS_BRD            0X03U
#define STM32WB55XX_BRD           0X04U
/******************************************************************************/

/****************Data Received from Android/IoS. B0 = SubCommand***************/
/*********************Sub Commands for APPLI_LED_CONTROL_CMD*******************/
#define APPLI_CMD_ON                    0x01U
#define APPLI_CMD_OFF                   0x02U
#define APPLI_CMD_TOGGLE                0x03U
#define APPLI_CMD_LED_BULB              0x05U
#define APPLI_CMD_LED_INTENSITY         0X06U
/******************************************************************************/

/*****************Sub Commands for APPLI_ELEMENT_TYPE_CMD**********************/
#define ELEMENT_TYPE_LED                      0x01U
#define ELEMENT_TYPE_SWITCH                   0x02U
/******************************************************************************/

/****************Data Received from Android/IoS. B0 = SubCommand***************/
/******************Sub Commands for APPLI_SENSOR_CNTRL_CMD *********************/
#define TEMP_SENSOR         0X1U
#define PRESS_SENSOR        0X2U
#define ACCEL_SENSOR        0X3U
/******************************************************************************/
#define VENDOR_DATA_BYTE       50
#define R_ASCI_CODE            0X52

#define DEFAULT_DELAY_PACKET_FROM         500U
#define DEFAULT_DELAY_PACKET_RANDOM_TIME  500U

#define MAX_PENDING_PACKETS_QUE_SIZE    2
#define DATA_BUFFER_LENGTH              8

#define BOUNCE_THRESHOLD                20U
#define LONG_PRESS_THRESHOLD            1000U

#define ADDRESS_IS_UNICAST(a)   (((MOBLEINT16)(a)>0)&&((MOBLEUINT16)(a)<0x8000))
#define ADDRESS_IS_GROUP(a)     (((MOBLEINT16)(a) & 0xC000) == 0xC000)  
#define ADDRESS_IS_ALL_NODES(a) ((MOBLEUINT16)(a) == 0xFFFF)

#define FIRST_ELEMENT 1
#define SECOND_ELEMENT 2
#define THIRD_ELEMENT 3

/* Exported variables  ------------------------------------------------------- */
/** \brief Callback map */
#pragma pack(1)
typedef struct
{
  MOBLE_RESULT (*LEDControlCommand_Cb)(MOBLEUINT8 const *, MOBLEUINT32, MOBLEUINT8,MOBLE_ADDRESS);
  MOBLE_RESULT (*DeviceInfoCommand_cb)(MOBLEUINT8 const *, MOBLEUINT32);
  MOBLE_RESULT (*TestCommand_cb)(MOBLEUINT8 const *, MOBLEUINT32);
  void (*LEDControl_cb)(void);
  void (*GetTestCount)(MOBLEUINT8*);
  MOBLE_RESULT (*DataControlCommand_cb)(MOBLEUINT8 const *, MOBLEUINT32);
} Appli_Vendor_cb_t;
#pragma pack(4)

extern const Appli_Vendor_cb_t VendorAppli_cb;

/* Exported Functions Prototypes ---------------------------------------------*/

MOBLE_RESULT Vendor_WriteLocalDataCb(MOBLE_ADDRESS peer_addr, MOBLE_ADDRESS dst_peer, 
                                    MOBLEUINT8 command, MOBLEUINT8 const *data, 
                                    MOBLEUINT32 length, MOBLEBOOL response);
MOBLE_RESULT Vendor_ReadLocalDataCb(MOBLE_ADDRESS peer_addr, MOBLE_ADDRESS dst_peer, 
                                  MOBLEUINT8 command, MOBLEUINT8 const *data, 
                                  MOBLEUINT32 length, MOBLEBOOL response);

MOBLE_RESULT Vendor_OnResponseDataCb(MOBLE_ADDRESS peer_addr, MOBLE_ADDRESS dst_peer, 
                                  MOBLEUINT8 command, MOBLEUINT8 const *pRxData, 
                                  MOBLEUINT32 dataLength, MOBLEBOOL response);

void Vendor_Process(void);
void Vendor_Publish(MOBLE_ADDRESS srcAddress);
void Vendor_TestRemoteData(MOBLE_ADDRESS src,MOBLE_ADDRESS dst,MOBLEUINT8 elementIndex);
void Vendor_TestCounterInc(MOBLE_ADDRESS src ,MOBLE_ADDRESS dst ,MOBLEUINT8 elementIndex);

MOBLE_RESULT VendorModel_PID1_GetOpcodeTableCb(const MODEL_OpcodeTableParam_t **data, 
                                                 MOBLEUINT16 *length);

MOBLE_RESULT VendorModel_PID1_GetStatusRequestCb(MOBLE_ADDRESS peer_addr, 
                                    MOBLE_ADDRESS dst_peer, 
                                    MOBLEUINT16 opcode, 
                                    MOBLEUINT8 *pResponsedata, 
                                    MOBLEUINT32 *plength, 
                                    MOBLEUINT8 const *pRxData,
                                    MOBLEUINT32 dataLength,
                                    MOBLEBOOL response);


MOBLE_RESULT VendorModel_PID1_ProcessMessageCb(MOBLE_ADDRESS peer_addr, 
                                                 MOBLE_ADDRESS dst_peer, 
                                                 MOBLEUINT16 opcode, 
                                                 MOBLEUINT8 const *pRxData, 
                                                 MOBLEUINT32 dataLength, 
                                                 MOBLEBOOL response
                                                   );

#endif /* __VENDOR_H */

/******************* (C) COPYRIGHT 2017 STMicroelectronics *****END OF FILE****/
