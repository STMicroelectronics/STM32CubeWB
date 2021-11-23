/**
******************************************************************************
* @file    vendor.h
* @author  BLE Mesh Team
* @brief   Header file for the user application file 
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
/********************* Sub Commands for APPLI_DATA_CNTRL_CMD*************************/
#define APPLI_STRING_WRITE                     0X01
#define APPLI_STRING_READ                      0X02
#define APPLI_DATA_READ                        0x03

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
/*********************Sub Commands for APPLI_LED_CONTROL_STATUS_CMD*******************/
#define APPLI_CMD_ON                    0x01U
#define APPLI_CMD_OFF                   0x02U
#define APPLI_CMD_TOGGLE                0x03U
#define APPLI_CMD_LED_BULB              0x05U
#define APPLI_CMD_LED_INTENSITY         0X06U
#define APPLI_CMD_LED_THERMOMETER       0X07U
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
#define VENDOR_DATA_BUFFER_SIZE           60 /* changed from 140 , as maximum packet size is 60 */
#define R_ASCI_CODE            0X52

#define DEFAULT_DELAY_PACKET_FROM         500U
#define DEFAULT_DELAY_PACKET_RANDOM_TIME  500U

#define MAX_PENDING_PACKETS_QUE_SIZE    2
#define DATA_BUFFER_LENGTH              16

#define BOUNCE_THRESHOLD                20U
#define LONG_PRESS_THRESHOLD            1000U

/* Exported variables  ------------------------------------------------------- */
/** \brief Callback map */

#pragma pack(push, 1)

typedef struct
{
  MOBLE_RESULT (*LEDControlCommand_Cb)(MOBLEUINT8 const *, MOBLEUINT32, MOBLEUINT8,MOBLE_ADDRESS);      /// Vendor_WriteLocalDataCb with command = APPLI_LED_CONTROL_STATUS_CMD 
  MOBLE_RESULT (*DeviceInfoCommand_cb)(MOBLEUINT8 const *, MOBLEUINT32);                                /// Vendor_WriteLocalDataCb with command = APPLI_DEVICE_INFO_CMD
  MOBLE_RESULT (*TestCommand_cb)(MOBLEUINT8 const *, MOBLEUINT32);                                      /// Vendor_WriteLocalDataCb with command = APPLI_TEST_CMD
  void (*LEDControl_cb)(void);                                                                          /// Vendor_ReadLocalDataCb with command  = APPLI_LED_CONTROL_STATUS_CMD
  void (*GetTestCount)(MOBLEUINT8*);                                                                    /// Vendor_ReadLocalDataCb with command  = APPLI_TEST_CMD
  MOBLE_RESULT (*DataControlCommand_cb)(MOBLEUINT8 const *, MOBLEUINT32);                               /// Vendor_WriteLocalDataCb with command = APPLI_DATA_CNTRL_CMD 
} Appli_Vendor_cb_t;

#pragma pack(pop)

extern const Appli_Vendor_cb_t VendorAppli_cb;

/* Exported Functions Prototypes ---------------------------------------------*/

MOBLE_RESULT Vendor_WriteLocalDataCb(MODEL_MessageHeader_t *pmsgParams,  
                                    MOBLEUINT8 command, MOBLEUINT8 const *data, 
                                    MOBLEUINT32 length, MOBLEBOOL response);
MOBLE_RESULT Vendor_ReadLocalDataCb(MODEL_MessageHeader_t *pmsgParams,  
                                  MOBLEUINT8 command, MOBLEUINT8 const *data, 
                                  MOBLEUINT32 length, MOBLEBOOL response);

MOBLE_RESULT Vendor_OnResponseDataCb(MODEL_MessageHeader_t *pmsgParams,  
                                  MOBLEUINT8 command, MOBLEUINT8 const *pRxData, 
                                  MOBLEUINT32 dataLength, MOBLEBOOL response);

void Vendor_Process(void);
void Vendor_Publish(MOBLE_ADDRESS srcAddress);
void Vendor_TestRemoteData(MOBLE_ADDRESS src,MOBLE_ADDRESS dst,MOBLEUINT8 elementIndex);
void Vendor_TestCounterInc(MOBLE_ADDRESS src ,MOBLE_ADDRESS dst ,MOBLEUINT8 elementIndex);
void Vendor_SendDataFreq(MOBLEUINT8 freq);
MOBLE_RESULT VendorModel_PID1_GetOpcodeTableCb(const MODEL_OpcodeTableParam_t **data, 
                                                 MOBLEUINT16 *length);

MOBLE_RESULT VendorModel_PID1_GetStatusRequestCb(MODEL_MessageHeader_t *pmsgParams, 
                                    MOBLEUINT16 opcode, 
                                    MOBLEUINT8 *pResponsedata, 
                                    MOBLEUINT32 *plength, 
                                    MOBLEUINT8 const *pRxData,
                                    MOBLEUINT32 dataLength,
                                    MOBLEBOOL response);


MOBLE_RESULT VendorModel_PID1_ProcessMessageCb(MODEL_MessageHeader_t *pmsgParams,  
                                                 MOBLEUINT16 opcode, 
                                                 MOBLEUINT8 const *pRxData, 
                                                 MOBLEUINT32 dataLength, 
                                                 MOBLEBOOL response
                                                   );

#endif /* __VENDOR_H */


