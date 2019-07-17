/**
  ******************************************************************************
  * @file    appli_generic.h
  * @author  BLE Mesh Team
  * @brief   Application interface for Generic Mesh Models  
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
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
#ifndef __GENERIC_APPLI_H
#define __GENERIC_APPLI_H

/* Includes ------------------------------------------------------------------*/
#include "types.h"
#include "generic.h"
#include "mesh_cfg.h"


/* Exported macro ------------------------------------------------------------*/
/* user configuration for Battery status */
#define BATTERY_ABSENT                 0X00
#define BATTERY_PRESENT_REMOVABLE      0X01
#define BATTERY_PRESENT_NON_REMOVABLE  0X02
#define BATTERY_UNKNOWN                0X03
#define BATTERY_CRITICAL_LOW_LEVEL     0X00
#define BATTERY_LOW_LEVEL              0X01
#define BATTERY_GOOD_LEVEL             0X02
#define BATTERY_LEVEL_UNKNOWN         0X03
#define BATTERY_NOT_CHARGEABLE         0X00
#define BATTERY_NOT_CHARGING           0X01
#define BATTERY_IS_CHARGING            0X02
#define BATTERY_CHARGING_UNKNOWN       0X03
#define BATTERY_SERVICE_RFU            0X00
#define BATTERY_REQUIRE_NO_SERVICE      0X01
#define BATTERY_REQUIRE_SERVICE        0X02
#define BATTERY_SERVICE_UNKNOWN        0X03   
/* Exported variables  ------------------------------------------------------- */
/* Application Variable-------------------------------------------------------*/
typedef struct 
{
  MOBLEUINT8 Is_BatteryPresent;
  MOBLEUINT8 Is_Chargeable;
  MOBLEUINT8 Is_Serviceable;
}Appli_BatteryUserflag_param_t;

/* Exported Functions Prototypes ---------------------------------------------*/

MOBLE_RESULT Appli_Generic_OnOff_Set(Generic_OnOffStatus_t*, MOBLEUINT8);
MOBLE_RESULT Appli_Generic_Level_Set(Generic_LevelStatus_t*, MOBLEUINT8);
MOBLE_RESULT Appli_Generic_LevelDelta_Set(Generic_LevelStatus_t*, MOBLEUINT8 );
MOBLE_RESULT Appli_Generic_LevelMove_Set(Generic_LevelStatus_t* pdeltaMoveParam, 
                                              MOBLEUINT8 OptionalValid);
MOBLE_RESULT Appli_Generic_Level_Status(MOBLEUINT8* level_status, 
                                                MOBLEUINT32 *plength);
MOBLE_RESULT Appli_Generic_GetOnOffStatus(MOBLEUINT8* pOnOff_Status);
MOBLE_RESULT Appli_Generic_GetLevelStatus(MOBLEUINT8* pLevel_Status);

#endif /* __GENERIC_APPLI_H */

/******************* (C) COPYRIGHT 2019 STMicroelectronics *****END OF FILE****/

