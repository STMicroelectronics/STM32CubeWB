/**
******************************************************************************
* @file    appli_test.h
* @author  BLE Mesh Team
* @version V1.12.000
* @date    06-12-2019
* @brief   Header file for the serial interface file 
******************************************************************************
* @attention
*
* <h2><center>&copy; COPYRIGHT(c) 2018 STMicroelectronics</center></h2>
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
#ifndef __APPLI_TEST_H
#define __APPLI_TEST_H

/* Includes ------------------------------------------------------------------*/
#include "types.h"

/* Exported macro ------------------------------------------------------------*/
#define     TEST_1_WAIT_PERIOD          2000
#define     TEST_2_WAIT_PERIOD          100
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

/******************* (C) COPYRIGHT 2018 STMicroelectronics *****END OF FILE****/

