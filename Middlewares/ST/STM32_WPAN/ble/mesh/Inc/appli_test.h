/**
******************************************************************************
* @file    appli_test.h
* @author  BLE Mesh Team
* @brief   Header file for the serial interface file 
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
#ifndef __APPLI_TEST_H
#define __APPLI_TEST_H

/* Includes ------------------------------------------------------------------*/
#include "types.h"

/* Exported macro ------------------------------------------------------------*/
#define     TEST_1_WAIT_PERIOD          2000
#define     TEST_2_WAIT_PERIOD          300
#define     TEST_3_WAIT_PERIOD          3000
#define     TEST_READ_PERIOD            2000
#define     CLOCK_FLAG_ENABLE           1 
#define     CLOCK_FLAG_DISABLE          0 
#define     DATA_BYTE_SEND              50
/* Exported variables  ------------------------------------------------------- */
/* Exported Functions Prototypes ---------------------------------------------*/
void SerialResponse_Process(char *rcvdStringBuff, uint16_t rcvdStringSize);
MOBLE_RESULT Test_ApplicationTest_Set01(MOBLEUINT32 testCount,MOBLE_ADDRESS src ,MOBLE_ADDRESS dst);
MOBLE_RESULT Packet_ResponseTimeStamp(MOBLEUINT32 rcvTimeStamp);
MOBLE_RESULT Test_ApplicationTest_Set02(MOBLEUINT32 testCount ,MOBLE_ADDRESS src ,MOBLE_ADDRESS dst);
MOBLE_RESULT Test_ApplicationTest_Set03(MOBLE_ADDRESS src ,MOBLE_ADDRESS dst);
MOBLEUINT8 processDelay(MOBLEUINT16 waitPeriod);                                               


#endif /* __RESPONSE_TEST_H */



