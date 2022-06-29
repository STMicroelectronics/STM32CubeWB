/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    appli_generic.h
  * @author  MCD Application Team
  * @brief   Application interface for Generic Mesh Models  
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef APPLI_GENERIC_H
#define APPLI_GENERIC_H

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

#pragma pack(push, 1)

typedef struct
{
  MOBLEUINT8 Present_OnOff;
  MOBLEUINT16 Present_OnOffValue;
  MOBLEUINT16 TargetValue;
}Appli_Generic_OnOffSet;

typedef struct
{
  MOBLEINT16 Present_Level16; 
  MOBLEINT16 Target_Level16;
  MOBLEUINT8 RemainingTime;
  MOBLEUINT8 Last_Level_TID;
  MOBLEINT16 Last_Present_Level16;  
  MOBLEINT32 Last_delta_level;
}Appli_Generic_LevelSet;

typedef struct
{
  MOBLEINT16 PowerOnState; 
}Appli_Generic_PowerOnOffSet;

typedef struct
{
  MOBLEUINT8 DefaultTransitionTime; 
}Appli_Generic_DefaultTransitionSet;

typedef struct 
{
  MOBLEUINT8 Is_BatteryPresent;
  MOBLEUINT8 Is_Chargeable;
  MOBLEUINT8 Is_Serviceable;
}Appli_BatteryUserflag_param_t;

#pragma pack(pop)

/* Exported Functions Prototypes ---------------------------------------------*/

MOBLE_RESULT Appli_Generic_OnOff_Set(Generic_OnOffStatus_t*, 
                                     MOBLEUINT8,
                                     MOBLEUINT16 dstPeer, 
                                     MOBLEUINT8 elementIndex);
MOBLE_RESULT Appli_Generic_OnOff_Status(MOBLEUINT8 const *pOnOff_status,
                                        MOBLEUINT32 plength,
                                        MOBLEUINT16 dstPeer, 
                                        MOBLEUINT8 elementIndex);
MOBLE_RESULT Appli_Generic_Level_Set(Generic_LevelStatus_t*, 
                                     MOBLEUINT8,
                                     MOBLEUINT16 dstPeer, 
                                     MOBLEUINT8 elementIndex);
MOBLE_RESULT Appli_Generic_Delta_Set(Generic_LevelStatus_t*, 
                                     MOBLEUINT8,
                                     MOBLEUINT16 dstPeer, 
                                     MOBLEUINT8 elementIndex);
MOBLE_RESULT Appli_Generic_Move_Set(Generic_LevelStatus_t* pdeltaMoveParam, 
                                    MOBLEUINT8 OptionalValid,
                                    MOBLEUINT16 dstPeer, 
                                    MOBLEUINT8 elementIndex);
MOBLE_RESULT Appli_Generic_Level_Status(MOBLEUINT8 const *plevel_status,
                                        MOBLEUINT32 plength,
                                        MOBLEUINT16 dstPeer, 
                                        MOBLEUINT8 elementIndex);
MOBLE_RESULT Appli_Generic_PowerOnOff_Set(Generic_PowerOnOffParam_t* pPowerOnOffParam, 
                                          MOBLEUINT8 OptionalValid,
                                          MOBLEUINT16 dstPeer, 
                                          MOBLEUINT8 elementIndex);  
MOBLE_RESULT Appli_Generic_PowerOnOff_Status(MOBLEUINT8 const *powerOnOff_status , 
                                             MOBLEUINT32 plength,
                                             MOBLEUINT16 dstPeer, 
                                             MOBLEUINT8 elementIndex); 
MOBLE_RESULT Appli_Generic_DefaultTransitionTime_Set(Generic_DefaultTransitionParam_t* pDefaultTimeParam, 
                                                     MOBLEUINT8 OptionalValid,
                                                     MOBLEUINT16 dstPeer, 
                                                     MOBLEUINT8 elementIndex);

MOBLE_RESULT Appli_Generic_DefaultTransitionTime_Status(MOBLEUINT8 const *pTransition_status , 
                                                        MOBLEUINT32 plength,
                                                        MOBLEUINT16 dstPeer, 
                                                        MOBLEUINT8 elementIndex); 

MOBLE_RESULT Appli_Generic_GetOnOffStatus(MOBLEUINT8* pOnOff_Status,
                                          MOBLEUINT16 dstPeer, 
                                          MOBLEUINT8 elementIndex);
MOBLE_RESULT Appli_Generic_GetOnOffValue(MOBLEUINT8* pOnOff_Value,
                                         MOBLEUINT16 dstPeer, 
                                         MOBLEUINT8 elementIndex);
MOBLE_RESULT Appli_Generic_GetLevelStatus(MOBLEUINT8* pLevel_Status,
                                          MOBLEUINT16 dstPeer, 
                                          MOBLEUINT8 elementIndex);
MOBLE_RESULT Appli_Generic_GetPowerOnOffStatus(MOBLEUINT8* pLevel_Status,
                                               MOBLEUINT16 dstPeer, 
                                               MOBLEUINT8 elementIndex);
void Appli_Generic_Restore_PowerOn_Value(MOBLEUINT8 restoreValue,
                                         MOBLEUINT16 dstPeer, 
                                         MOBLEUINT8 elementIndex);
MOBLE_RESULT Appli_Generic_GetDefaultTransitionStatus(MOBLEUINT8* pTransition_Status,
                                                      MOBLEUINT16 dstPeer, 
                                                      MOBLEUINT8 elementIndex);
 


#endif /* APPLI_GENERIC_H */


