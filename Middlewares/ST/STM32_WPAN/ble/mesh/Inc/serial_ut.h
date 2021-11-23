/**
******************************************************************************
* @file    serial_ut.h
* @author  BLE Mesh Team
* @brief   Header file for the upper tester file 
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
#ifndef __UPPER_TESTER_H
#define __UPPER_TESTER_H

/* Includes ------------------------------------------------------------------*/
#include "types.h"

/* Exported macro ------------------------------------------------------------*/
/* Exported variables  ------------------------------------------------------- */


/* Exported Functions Prototypes ---------------------------------------------*/
void SerialUt_Process(char *rcvdStringBuff, uint16_t rcvdStringSize);

#endif /* __UPPER_TESTER_H */



