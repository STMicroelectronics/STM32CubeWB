/**
******************************************************************************
* @file    ble_clock.h
* @author  BLE Mesh Team
* @brief   Header file for clock library, that gives a simple time
*          reference to the BLE Stack.
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
#ifndef __BLE_CLOCK_H__
#define __BLE_CLOCK_H__

#include <stdint.h>

typedef uint32_t tClockTime;
#define CLOCK_SECOND 1000
#define Clock_Init() 
#define Clock_Time HAL_GetTick
#define Clock_Wait HAL_Delay

#endif /* __BLE_CLOCK_H__ */
