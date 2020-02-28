/******************** (C) COPYRIGHT 2012 STMicroelectronics ********************
* File Name          : ble_clock.h
* Author             : AMS - HEA&RF BU
* @version V1.12.000
* @date    06-12-2019
* Description        : Header file for clock library, that gives a simple time
*                       reference to the BLE Stack.
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

#ifndef __BLE_CLOCK_H__
#define __BLE_CLOCK_H__

//#include "bluenrg_itf.h"
#include <stdint.h>

typedef uint32_t tClockTime;
#define CLOCK_SECOND 1000
#define Clock_Init() 
#define Clock_Time HAL_GetTick
#define Clock_Wait HAL_Delay

#endif /* __BLE_CLOCK_H__ */
