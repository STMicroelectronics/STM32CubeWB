/**
******************************************************************************
* @file    serial_if.h
* @author  BLE Mesh Team
* @brief   Header file for the serial interface file 
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
#ifndef __SERIAL_IF_H
#define __SERIAL_IF_H

/* Includes ------------------------------------------------------------------*/
#include "types.h"

/* Exported macro ------------------------------------------------------------*/
/* Exported variables  ------------------------------------------------------- */
/* Exported Functions Prototypes ---------------------------------------------*/
void Serial_Init(void);
void Serial_PrintStringCb(const char *message);
void Serial_InterfaceProcess(void);
MOBLEUINT8 Serial_CharToHexConvert(char addr);


#endif /* __SERIAL_IF_H */

/******************* (C) COPYRIGHT 2018 STMicroelectronics *****END OF FILE****/

