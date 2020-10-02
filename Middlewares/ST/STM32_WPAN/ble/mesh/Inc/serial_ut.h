/**
******************************************************************************
* @file    serial_ut.h
* @author  BLE Mesh Team
* @brief   Header file for the upper tester file 
******************************************************************************
* @attention
*
* <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
* All rights reserved.</center></h2>
*
* This software component is licensed by ST under Ultimate Liberty license
* SLA0044, the "License"; You may not use this file except in compliance with
* the License. You may obtain a copy of the License at:
*                             www.st.com/SLA0044
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

/******************* (C) COPYRIGHT 2018 STMicroelectronics *****END OF FILE****/

