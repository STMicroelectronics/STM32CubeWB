/**
  ******************************************************************************
  * @file    appli_generic.c
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

/* Includes ------------------------------------------------------------------*/
#include "app_conf.h"
#include "hal_common.h"
#include "types.h"
#include "ble_mesh.h"
#include "appli_mesh.h"
#include "mesh_cfg.h"
#include "generic.h"
#include "light.h"
#include "appli_generic.h"
#include "common.h"
#include "mesh_cfg_usr.h"
/** @addtogroup BLE_Mesh
 *  @{
 */

/** @addtogroup models_BLE
 *  @{
 */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
#define POWER_OFF_STATE       0X02
#define POWER_ON_STATE        0X01
#define POWER_RESTORE_STATE   0X02
   
/* Private variables ---------------------------------------------------------*/


MOBLEUINT8 OptionalValidParam = 0;
MOBLEUINT8 LEDState; 
MOBLEUINT8 StatusCode;

#pragma pack(1)
typedef struct
{
  MOBLEUINT8 Present_OnOff;
}Appli_Generic_OnOffSet;

Appli_Generic_OnOffSet AppliOnOffSet;

#pragma pack(1)
typedef struct
{
  MOBLEINT16 Present_Level16; 
}Appli_Generic_LevelSet;

Appli_Generic_LevelSet AppliLevelSet;


/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

#ifdef ENABLE_GENERIC_MODEL_SERVER_ONOFF
/**
* @brief  Appli_Generic_OnOff_Set: This function is callback for Application
           when Generic OnOff message is received
* @param  pGeneric_OnOffParam: Pointer to the parameters received for message
* @param  OptionalValid: Flag to inform about the validity of optional parameters 
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT Appli_Generic_OnOff_Set(Generic_OnOffStatus_t* pGeneric_OnOffParam, 
                                     MOBLEUINT8 OptionalValid)
{
    AppliOnOffSet.Present_OnOff = pGeneric_OnOffParam->Present_OnOff;    
    if(AppliOnOffSet.Present_OnOff > 0)
    {
      BSP_LED_On(LED_BLUE);
    }
    else
    {
      BSP_LED_Off(LED_BLUE);
    }

  return MOBLE_RESULT_SUCCESS;
}
#endif


#ifdef ENABLE_GENERIC_MODEL_SERVER_LEVEL
/**
* @brief  Appli_Generic_Level_Set: This function is callback for Application
           when Generic Level message is received
* @param  plevelParam: Pointer to the parameters message
* @param  OptionalValid: Flag to inform about the validity of optional parameters 
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT Appli_Generic_Level_Set(Generic_LevelStatus_t* plevelParam, 
                                     MOBLEUINT8 OptionalValid)
{
   
  AppliLevelSet.Present_Level16= plevelParam->Present_Level16;   
  
  /* For demo, if Level is more than 100, switch ON the LED */
  if(AppliLevelSet.Present_Level16 >= 50)
  {
    BSP_LED_On(LED_BLUE);
    LEDState = 1;
  }
  else
  {
    BSP_LED_Off(LED_BLUE);
    LEDState = 1;
  }
  
  return MOBLE_RESULT_SUCCESS;
}

/**
* @brief  Appli_Generic_LevelDelta_Set: This function is callback for Application
           when Generic Level Delta message is received
* @param  pdeltalevelParam: Pointer to the parameters message
* @param  OptionalValid: Flag to inform about the validity of optional parameters 
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT Appli_Generic_LevelDelta_Set(Generic_LevelStatus_t* pdeltalevelParam, 
                                     MOBLEUINT8 OptionalValid)
{
  
  AppliLevelSet.Present_Level16 = pdeltalevelParam->Present_Level16;
  
  /* For demo, if Level is more than 50, switch ON the LED */
  if (AppliLevelSet.Present_Level16 >= 50)
  {
    BSP_LED_On(LED_BLUE);
    LEDState = 1;
  }
  else
  {
    BSP_LED_Off(LED_BLUE);
    LEDState = 1;
  }
  return MOBLE_RESULT_SUCCESS;
}


/**
* @brief  Appli_Generic_LevelMove_Set: This function is callback for Application
           when Generic Level Move message is received
* @param  pdeltaMoveParam: Pointer to the parameters message
* @param  OptionalValid: Flag to inform about the validity of optional parameters 
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT Appli_Generic_LevelMove_Set(Generic_LevelStatus_t* pdeltaMoveParam, 
                                               MOBLEUINT8 OptionalValid)
{
  
  if(OptionalValid == 1)
  {
    AppliLevelSet.Present_Level16= pdeltaMoveParam->Present_Level16;   
      
    OptionalValidParam = 1;
  }
  return MOBLE_RESULT_SUCCESS;
}

#endif  
 

/**
* @brief  Appli_Generic_GetOnOffState: This function is callback for Application
           when Generic on off status message is to be provided
* @param  pOnOff_status: Pointer to the status message
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT Appli_Generic_GetOnOffStatus(MOBLEUINT8* pOnOff_Status)                                        
{
  
  *pOnOff_Status = AppliOnOffSet.Present_OnOff;
  *(pOnOff_Status+1) = AppliOnOffSet.Present_OnOff >> 8;
  
  return MOBLE_RESULT_SUCCESS; 
}


/**
* @brief  Appli_Generic_GetLevelStatus: This function is callback for Application
           when Generic Level status message is to be provided
* @param  pLevel_status: Pointer to the status message
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT Appli_Generic_GetLevelStatus(MOBLEUINT8* pLevel_Status) 
                                        
{ 
   *pLevel_Status = AppliLevelSet.Present_Level16;
   *(pLevel_Status+1) = AppliLevelSet.Present_Level16 >> 8;
  
  return MOBLE_RESULT_SUCCESS; 
}

/**
 * @}
 */

/**
 * @}
 */

/******************* (C) COPYRIGHT 2019 STMicroelectronics *****END OF FILE****/

