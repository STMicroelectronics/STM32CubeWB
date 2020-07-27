/**
******************************************************************************
* @file    light_lc.c
* @author  BLE Mesh Team
* @brief   Light Control model middleware file
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
/* Includes ------------------------------------------------------------------*/
#include "hal_common.h"
#include "mesh_cfg.h"
#include "generic.h"
#include "light.h"
#include "common.h"
#include "light_lc.h"
#include <string.h>
#include "compiler.h"


/** @addtogroup MODEL_LIGHT_CONTROL
*  @{
*/

/** @addtogroup Light_Control_Model_Callbacks
*  @{
*/

#ifdef ENABLE_LIGHT_MODEL_SERVER_LC
MOBLEUINT16 Get_LengthInByteProprtyValue(MOBLEUINT16 property_ID);
#endif

/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
#ifdef ENABLE_LIGHT_MODEL_SERVER_LC_SETUP 
Lc_Event_e Lc_Event = LC_NO_EVENT;

Light_LC_States LightLC_States = {LC_STANDBY};

Light_LC_TimeParam_t Light_LC_TimeParam;

Light_LC_Param_t Light_LC_Param;

Light_LC_ModelFlag_t Light_LC_ModelFlag;

Light_LC_TemporaryStatus_t Light_LC_TemporaryStatus;

//extern Light_PublishOpcodeList_t Light_PublishOpcodeList;

MOBLEUINT8 Light_LC_UpdateFlag = 0;
MOBLEUINT32 Timer_value;
extern MOBLEUINT8 TidSend;
extern MOBLE_ADDRESS Dst_Peer;
#endif

MODEL_OpcodeTableParam_t Light_LC_Opcodes_Table[] = {
/* model_id                             opcode                       reliable      min_payload_size max_payload_size response_opcode           min_response_size max_response_size */
#ifdef ENABLE_LIGHT_MODEL_SERVER_LC      
 {LIGHT_MODEL_SERVER_LC_MODEL_ID,       LIGHT_LC_MODE_GET,           MOBLE_TRUE,   0,               0,               LIGHT_LC_MODE_STATUS  ,   1,                1},
 {LIGHT_MODEL_SERVER_LC_MODEL_ID,       LIGHT_LC_MODE_SET,           MOBLE_TRUE,   1,               1,               LIGHT_LC_MODE_STATUS  ,   1,                1},
 {LIGHT_MODEL_SERVER_LC_MODEL_ID,       LIGHT_LC_MODE_SET_UNACK,     MOBLE_FALSE,  1,               1,               LIGHT_LC_MODE_STATUS  ,   1,                1},
 {LIGHT_MODEL_SERVER_LC_MODEL_ID,       LIGHT_LC_MODE_STATUS,        MOBLE_FALSE,  1,               1,               0                     ,   1,                1},
 {LIGHT_MODEL_SERVER_LC_MODEL_ID,       LIGHT_LC_OM_GET,             MOBLE_TRUE,   0,               0,               LIGHT_LC_OM_STATUS    ,   1,                1},
 {LIGHT_MODEL_SERVER_LC_MODEL_ID,       LIGHT_LC_OM_SET,             MOBLE_TRUE,   1,               1,               LIGHT_LC_OM_STATUS    ,   1,                1},
 {LIGHT_MODEL_SERVER_LC_MODEL_ID,       LIGHT_LC_OM_SET_UNACK,       MOBLE_FALSE,  1,               1,               LIGHT_LC_OM_STATUS    ,   1,                1},
 {LIGHT_MODEL_SERVER_LC_MODEL_ID,       LIGHT_LC_OM_STATUS,          MOBLE_FALSE,  1,               1,               0                     ,   1,                1},
 {LIGHT_MODEL_SERVER_LC_MODEL_ID,       LIGHT_LC_ON_OFF_GET,         MOBLE_TRUE,   0,               0,               LIGHT_LC_ON_OFF_STATUS,   1,                3},
 {LIGHT_MODEL_SERVER_LC_MODEL_ID,       LIGHT_LC_ON_OFF_SET,         MOBLE_TRUE,   2,               4,               LIGHT_LC_ON_OFF_STATUS,   1,                3},
 {LIGHT_MODEL_SERVER_LC_MODEL_ID,       LIGHT_LC_ON_OFF_SET_UNACK,   MOBLE_FALSE,  2,               4,               LIGHT_LC_ON_OFF_STATUS,   1,                3},
 {LIGHT_MODEL_SERVER_LC_MODEL_ID,       LIGHT_LC_ON_OFF_STATUS,      MOBLE_FALSE,  1,               3,               0                     ,   1,                3},
#endif                                                                                                                  
                                                                                                                        
#ifdef ENABLE_LIGHT_MODEL_CLIENT_LC      
 {LIGHT_MODEL_CLIENT_LC_MODEL_ID,       LIGHT_LC_MODE_GET,           MOBLE_TRUE,   0,               0,               LIGHT_LC_MODE_STATUS  ,   1,                1},
 {LIGHT_MODEL_CLIENT_LC_MODEL_ID,       LIGHT_LC_MODE_SET,           MOBLE_TRUE,   1,               1,               LIGHT_LC_MODE_STATUS  ,   1,                1},
 {LIGHT_MODEL_CLIENT_LC_MODEL_ID,       LIGHT_LC_MODE_SET_UNACK,     MOBLE_FALSE,  1,               1,               LIGHT_LC_MODE_STATUS  ,   1,                1},
 {LIGHT_MODEL_CLIENT_LC_MODEL_ID,       LIGHT_LC_MODE_STATUS,        MOBLE_FALSE,  1,               1,               0                     ,   1,                1},
 {LIGHT_MODEL_CLIENT_LC_MODEL_ID,       LIGHT_LC_OM_GET,             MOBLE_TRUE,   0,               0,               LIGHT_LC_OM_STATUS    ,   1,                1},
 {LIGHT_MODEL_CLIENT_LC_MODEL_ID,       LIGHT_LC_OM_SET,             MOBLE_TRUE,   1,               1,               LIGHT_LC_OM_STATUS    ,   1,                1},
 {LIGHT_MODEL_CLIENT_LC_MODEL_ID,       LIGHT_LC_OM_SET_UNACK,       MOBLE_FALSE,  1,               1,               LIGHT_LC_OM_STATUS    ,   1,                1},
 {LIGHT_MODEL_CLIENT_LC_MODEL_ID,       LIGHT_LC_OM_STATUS,          MOBLE_FALSE,  1,               1,               0                     ,   1,                1},
 {LIGHT_MODEL_CLIENT_LC_MODEL_ID,       LIGHT_LC_ON_OFF_GET,         MOBLE_TRUE,   0,               0,               LIGHT_LC_ON_OFF_STATUS,   1,                3},
 {LIGHT_MODEL_CLIENT_LC_MODEL_ID,       LIGHT_LC_ON_OFF_SET,         MOBLE_TRUE,   2,               4,               LIGHT_LC_ON_OFF_STATUS,   1,                3},
 {LIGHT_MODEL_CLIENT_LC_MODEL_ID,       LIGHT_LC_ON_OFF_SET_UNACK,   MOBLE_FALSE,  2,               4,               LIGHT_LC_ON_OFF_STATUS,   1,                3},
 {LIGHT_MODEL_CLIENT_LC_MODEL_ID,       LIGHT_LC_ON_OFF_STATUS,      MOBLE_FALSE,  1,               3,               0                     ,   1,                3},
#endif                                                                                                                  
                                                                                                                        
#ifdef ENABLE_LIGHT_MODEL_SERVER_LC_SETUP                                                                               
 {LIGHT_MODEL_SERVER_LC_SETUP_MODEL_ID, LIGHT_LC_PROPERTY_GET,       MOBLE_TRUE,   2,               2,               LIGHT_LC_PROPERTY_STATUS, 2,                10},
 {LIGHT_MODEL_SERVER_LC_SETUP_MODEL_ID, LIGHT_LC_PROPERTY_SET,       MOBLE_TRUE,   2,               10,              LIGHT_LC_PROPERTY_STATUS, 2,                10},
 {LIGHT_MODEL_SERVER_LC_SETUP_MODEL_ID, LIGHT_LC_PROPERTY_SET_UNACK, MOBLE_FALSE,  2,               10,              LIGHT_LC_PROPERTY_STATUS, 2,                10},
 {LIGHT_MODEL_SERVER_LC_SETUP_MODEL_ID, LIGHT_LC_PROPERTY_STATUS,    MOBLE_FALSE,  2,               10,              0                       , 2,                10}, 
#endif 
 {0}
};

/* This table is structure consisting structures of array, this has parameters LC 
   Property and Lc Property value.
   Table has 4 structures depending on the data type.
*/
Light_Property_Table_t Light_Property_Table = {
  
  {
    /* regulator accxuracy id which is 8 bit value */
    { LIGHT_CONTROL_REGULATOR_ACCURACY_ID          , LIGHT_CONTROL_REGULATOR_ACCURACY_VALUE},
  },
  {
    /*structure consist of Lc Property value of data type 16 bit */
    {LIGHT_CONTROL_LIGHTNESS_ON_ID                 , LIGHT_CONTROL_LIGHTNESS_ON_VALUE},
    {LIGHT_CONTROL_LIGHTNESS_PROLONG_ID            , LIGHT_CONTROL_LIGHTNESS_PROLONG_VALUE},
    {LIGHT_CONTROL_LIGHTNESS_STANDBY_ID            , LIGHT_CONTROL_LIGHTNESS_STANDBY_VALUE},
  },
  {
    /*structure consist of Lc Property value of Maximum range of data 24 bit */
    {LIGHT_CONTROL_LUX_LEVEL_ON_ID                 ,LIGHT_CONTROL_AMBIENT_LUXLEVEL_ON},     
    {LIGHT_CONTROL_LUX_LEVEL_PROLONG_ID            ,LIGHT_CONTROL_AMBIENT_LUXLEVEL_PROLONG} ,    
    {LIGHT_CONTROL_LUX_LEVEL_STANDBY_ID            ,LIGHT_CONTROL_AMBIENT_LUXLEVEL_STANDBY},
    {LIGHT_CONTROL_TIME_FADE_ID                    ,LIGHT_CONTROL_TIME_FADE_VALUE},
    {LIGHT_CONTROL_TIME_FADE_ON_ID                 ,LIGHT_CONTROL_TIME_FADE_ON_VALUE},
    {LIGHT_CONTROL_TIME_FADE_STANDBY_AUTO_ID       ,LIGHT_CONTROL_TIME_FADE_STANDBY_AUTO_VALUE},
    {LIGHT_CONTROL_TIME_FADE_STANDBY_MANUAL_ID     ,LIGHT_CONTROL_TIME_FADE_STANDBY_MANUAL_VALUE},
    {LIGHT_CONTROL_TIME_PROLONG_ID                 ,LIGHT_CONTROL_TIME_PROLONG_VALUE},
    {LIGHT_CONTROL_TIME_RUN_ON_ID                  ,LIGHT_CONTROL_TIME_RUN_ON_VALUE},      
  },
  {
    /*structure consist of Lc Property value of data type float. */
    {LIGHT_CONTROL_REGULATOR_KID_ID                ,LIGHT_CONTROL_KID},     
    {LIGHT_CONTROL_REGULATOR_KIU_ID                ,LIGHT_CONTROL_KIU} ,    
    {LIGHT_CONTROL_REGULATOR_KPD_ID                ,LIGHT_CONTROL_KPD},
    {LIGHT_CONTROL_REGULATOR_KPU_ID                ,LIGHT_CONTROL_KPU},
  },
};
 
#ifdef ENABLE_LIGHT_MODEL_SERVER_LC
/**
* @brief  Light_LC_ModeSet: This function is called for both Acknowledged and 
*         unacknowledged message
* @param  lcMode_param: Pointer to the parameters received for message
* @param  length: Length of the parameters received for message
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT Light_LC_ModeSet(MOBLEUINT8 const *lcMode_param, MOBLEUINT32 length)
{  
  
  TRACE_I(TF_LIGHT_LC_M, "Light_LC_ModeSet callback received \r\n");
  
  Light_LC_Param.LC_mode = lcMode_param[0];
  
  /* Application Callback */
  (LightLCAppli_cb.LightLC_Mode_Set_cb)(&Light_LC_Param , 0);
  
  return MOBLE_RESULT_SUCCESS;
}

/**
* @brief  Light_LC_ModeStatus
* @param  lcMode_status: Pointer to the status message, which needs to be updated
* @param  plength: Pointer to the Length of the Status message
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT Light_LC_ModeStatus(MOBLEUINT8* lcMode_status, MOBLEUINT32 *plength)
{
  MOBLEUINT8 LightLC_GetBuff[2];
  TRACE_I(TF_LIGHT_LC_M,"Light_LC_ModeStatus callback received \r\n");
  Appli_LightLC_GetStatus_cb.GetLightLC_ModeState_cb(LightLC_GetBuff);
  
  *lcMode_status = LightLC_GetBuff[0];
  *plength = 1;
  return MOBLE_RESULT_SUCCESS;
}

/**
* @brief  Light_LC_OMSet: This function is called for both Acknowledged and 
*         unacknowledged message
* @param  lcOM_param: Pointer to the parameters received for message
* @param  length: Length of the parameters received for message
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT Light_LC_OMSet(MOBLEUINT8 const *lcOM_param, MOBLEUINT32 length)
{
  
  TRACE_I(TF_LIGHT_LC_M,"Light_LC_OMSet callback received \r\n");
  
  Light_LC_Param.LC_OM = lcOM_param[0];
  
  if(Light_LC_Param.LC_OM == 1)
  {
    Lc_Event = LC_OCCUPANCY_ON;
  }
  else
  {
    Lc_Event = LC_OCCUPANCY_OFF;
  }
  
  /* Application Callback */
  (LightLCAppli_cb.LightLC_OM_Set_cb)(&Light_LC_Param , 0);
  
  return MOBLE_RESULT_SUCCESS;
}

/**
* @brief  Light_LC_OMStatus
* @param  lcOM_status: Pointer to the status message, which needs to be updated
* @param  plength: Pointer to the Length of the Status message
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT Light_LC_OMStatus(MOBLEUINT8* lcOM_status, MOBLEUINT32 *plength)
{
  MOBLEUINT8 LightLC_GetBuff[2];
  TRACE_I(TF_LIGHT_LC_M,"Light_LC_OMStatus callback received \r\n");
  
   Appli_LightLC_GetStatus_cb.GetLightLC_OMState_cb(LightLC_GetBuff);
  
  *lcOM_status = LightLC_GetBuff[0];
  *plength = 1;
  
  return MOBLE_RESULT_SUCCESS;
}

/**
* @brief  Light_LC_OnOffSet: This function is called for both Acknowledged and 
*         unacknowledged message
* @param  lcOnOff_param: Pointer to the parameters received for message
* @param  length: Length of the parameters received for message
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT Light_LC_OnOffSet(MOBLEUINT8 const *lcOnOff_param, MOBLEUINT32 length)
{
  TRACE_I(TF_LIGHT_LC_M,"Light_LC_OnOffSet callback received \r\n");
  
  Light_LC_Param.Target_Light_OnOff = lcOnOff_param[0];
  Light_LC_Param.Tid = lcOnOff_param[1];
  MOBLEUINT16 property_ID;
  MOBLEUINT32 prop_Value;

  /* Event set to descide the state to be process for LC state machine */
  if(Light_LC_Param.Target_Light_OnOff == 1)
  {
  Lc_Event = LC_LIGHT_ON;
  }
  else
  {
    Lc_Event = LC_LIGHT_OFF;
  }
  
  if((length > 2) && (lcOnOff_param[2] != 0))
  {
    Light_LC_Param.Transition_Time = lcOnOff_param[2];
    Light_LC_Param.Delay = lcOnOff_param[3];
    Light_LC_ModelFlag.Light_LC_OptionalParam = VALUE_SET;
    prop_Value = Light_LC_Param.Transition_Time;
    Light_LC_Param.Remaining_Time = Light_LC_Param.Transition_Time;  
    Light_LC_GetStepValue(lcOnOff_param[2]);
    
    if( LightLC_States.Lc_States == LC_STANDBY)
    {
      property_ID = LIGHT_CONTROL_TIME_FADE_ON_ID;
      Light_LC_SetPropertyID_value(prop_Value , property_ID);
    }
    else if(LightLC_States.Lc_States == LC_RUN)
    {
      property_ID = LIGHT_CONTROL_TIME_FADE_ID;
      Light_LC_SetPropertyID_value(prop_Value , property_ID);    
    }    
    else if(LightLC_States.Lc_States == LC_PROLONG)
    {
      property_ID = LIGHT_CONTROL_TIME_FADE_STANDBY_AUTO_ID;
      Light_LC_SetPropertyID_value(prop_Value , property_ID);    
  } 
  else
  {
      /* No Comment */
    }
    
  }
  else
  {
    Light_LC_Param.Present_Light_OnOff = Light_LC_Param.Target_Light_OnOff;
  }
  
  /* Application Callback */
  (LightLCAppli_cb.LightLC_OnOff_Set_cb)(&Light_LC_Param , 0);
  
#ifdef ENABLE_GENERIC_MODEL_SERVER_ONOFF    
   Light_LC_OnOff_Generic_OnOffBinding();
#endif
  
  return MOBLE_RESULT_SUCCESS;
}

/**
* @brief  Light_LC_OnOffStatus
* @param  lcOnOff_status: Pointer to the status message, which needs to be updated
* @param  plength: Pointer to the Length of the Status message
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT Light_LC_OnOffStatus(MOBLEUINT8* lcOnOff_status, MOBLEUINT32 *plength)
{
  TRACE_I(TF_LIGHT_LC_M,"Light_LC_OnOffStatus callback received \r\n");
  
  if((Light_LC_ModelFlag.Light_LC_OptionalParam == VALUE_SET) || (Light_LC_TimeParam.StepValue != 0)) 
  {   
    *lcOnOff_status = Light_LC_Param.Present_Light_OnOff;
    *(lcOnOff_status+1) = Light_LC_Param.Target_Light_OnOff;
    *(lcOnOff_status+2) = Light_LC_Param.Remaining_Time;
    *plength = 3;    
    Light_LC_ModelFlag.Light_LC_OptionalParam = VALUE_RESET;
  }
  else
  { /* When no optional parameter received, target value will
    be sent in status message.
    */
    *lcOnOff_status = Light_LC_Param.Present_Light_OnOff;
    *plength = 1;
  }  
   
  return MOBLE_RESULT_SUCCESS;
}

/**
* @brief  Light_LC_PropertySet: This function is called for both Acknowledged and 
*         unacknowledged message
* @param  lcProp_param: Pointer to the parameters received for message
* @param  length: Length of the parameters received for message
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT Light_LC_PropertySet(MOBLEUINT8 const *lcProp_param, MOBLEUINT32 length)
{
  TRACE_I(TF_LIGHT_LC_M,"Light_LC_PropertySet callback received \r\n");
  
  MOBLEUINT16  Light_LC_PropertyID;
  MOBLEUINT32  property_Value;
  
  Light_LC_Value_t Light_LC_Value;

  Light_LC_Value.Property_Value = 0;

  Light_LC_PropertyID = lcProp_param[1] << 8;
  Light_LC_PropertyID |= lcProp_param[0];
  
  switch (length)
  {
  case 3:
     Light_LC_Value.Property_Value = lcProp_param[2];
    break;
  case 4:
    Light_LC_Value.Property_Value = lcProp_param[3] << 8;
    Light_LC_Value.Property_Value |= lcProp_param[2];
    break;
    
  case 5:
    Light_LC_Value.Property_Value = lcProp_param[4] << 16;
    Light_LC_Value.Property_Value |= lcProp_param[3] << 8;
    Light_LC_Value.Property_Value |= lcProp_param[2];    
    break;
  
  case 6:
    Light_LC_Value.Property_Value = lcProp_param[5] << 24;
    Light_LC_Value.Property_Value |= lcProp_param[4] << 16;
    Light_LC_Value.Property_Value |= lcProp_param[3] << 8; 
    Light_LC_Value.Property_Value |= lcProp_param[2];
    
    break;
    
  default:
    break;
  }
   property_Value = Light_LC_Value.Property_Value;
   Light_LC_SetPropertyID_value(property_Value,Light_LC_PropertyID);
  
  return MOBLE_RESULT_SUCCESS;
}

/**
* @brief  Light_LC_PropertyStatus
* @param  lcData_param: Pointer to the status message, which needs to be updated
* @param  plength: Pointer to the status message length, which needs to be updated
* @param  pData: Pointer to the message received.
* @param  length: Received message length.
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT Light_LC_PropertyStatus( MOBLEUINT8* lcData_param, MOBLEUINT32* plength ,
                                        MOBLEUINT8 const *pData, MOBLEUINT32 length)
{
  MOBLEUINT16 prop_ID = 0x00;
  MOBLEUINT32 Property_Value;
  MOBLEUINT16 prop_value_length;
  
  TRACE_M(TF_LIGHT_LC_M,"Light_LC_PropertyStatus callback received \r\n");
  
  if(length > 0)  
  {
    prop_ID = pData[1] << 8;
    prop_ID |= pData[0]; 
  }
 
  Property_Value = Light_LC_GetPropertyID_value(prop_ID,&prop_value_length);
  
   *lcData_param = prop_ID;
  *(lcData_param+1) = prop_ID >> 8;
  
  switch (prop_value_length)
  {
  case ONE_BYTE_VALUE:
     *(lcData_param+2) = Property_Value;
     *plength = 3;
    break;
  case TWO_BYTE_VALUE:
     *(lcData_param+2) = Property_Value ;
     *(lcData_param+3) = Property_Value >> 8;
     *plength = 4;
    break;
    
  case THREE_BYTE_VALUE:
    *(lcData_param+2) = Property_Value ;
    *(lcData_param+3) = Property_Value >> 8;
    *(lcData_param+4) = Property_Value >> 16; 
    *plength = 5;
    break;
  
  case FOUR_BYTE_VALUE:
    *(lcData_param+2) = Property_Value;
    *(lcData_param+3) = Property_Value >> 8;
    *(lcData_param+4) = Property_Value >> 16;
    *(lcData_param+5) = Property_Value >> 24 ;
    *plength = 6;
    
    break;
    
  default:
        break;
  }
     
  return MOBLE_RESULT_SUCCESS;
}


/**
* @brief   Light_LC_GetPropertyID_value: This function is call-back 
*          from the library to send Model Opcode Table info to library
* @param  Prop_Value:value belongs to property id
* @param  prop_ID: Property id of the parameter.
* @retval MOBLEUINT32
*/ 
MOBLE_RESULT Light_LC_SetPropertyID_value(MOBLEUINT32 Prop_Value,
                                            MOBLEUINT16 prop_ID)                                                                                                      
{
  MOBLEUINT8 length = sizeof(Light_Property_Table.LC_PropertyTable8b)/sizeof(Light_Property_Table.LC_PropertyTable8b[0]);
  MOBLEUINT8 valid_flag = 0;
  
  for(int i=0;i < length;i++)
  {
    if(prop_ID == Light_Property_Table.LC_PropertyTable8b[i].Property_ID)
    {
       Light_Property_Table.LC_PropertyTable8b[i].Property_Value_8b = Prop_Value;
       valid_flag = 1;
       return MOBLE_RESULT_SUCCESS;
       
    }
  }
  
  length = sizeof(Light_Property_Table.LC_PropertyTable16b)/sizeof(Light_Property_Table.LC_PropertyTable16b[0]);
  
  for(int i=0;i < length;i++)
  {
    if(prop_ID == Light_Property_Table.LC_PropertyTable16b[i].Property_ID)
    {
       Light_Property_Table.LC_PropertyTable16b[i].Property_Value_16= Prop_Value;
       valid_flag = 1;
       return MOBLE_RESULT_SUCCESS;
       
    }
  }
  
   length = sizeof(Light_Property_Table.LC_PropertyTable24b)/sizeof(Light_Property_Table.LC_PropertyTable24b[0]);
  
  for(int i=0;i < length;i++)
  {
    if(prop_ID == Light_Property_Table.LC_PropertyTable24b[i].Property_ID)
    {
       Light_Property_Table.LC_PropertyTable24b[i].Property_Value_24b= Prop_Value;
       valid_flag = 1;
       return MOBLE_RESULT_SUCCESS;
       
    }
  }
      
     length = sizeof(Light_Property_Table.LC_PropertyTableFloat)/sizeof(Light_Property_Table.LC_PropertyTableFloat[0]);
  
  for(int i=0;i < length;i++)
  {
    if(prop_ID == Light_Property_Table.LC_PropertyTableFloat[i].Property_ID)
    {
       Light_Property_Table.LC_PropertyTableFloat[i].Property_Value_float = Prop_Value;
       valid_flag = 1;
       return MOBLE_RESULT_SUCCESS;
       
    }
  }
  
  if(valid_flag == 0)
  {
    TRACE_I(TF_LIGHT_LC_M,"Wrong Property ID \r\n");
  }
    return MOBLE_RESULT_SUCCESS;
}

/**
* @brief   Light_LC_GetPropertyID_value: This function is call-back 
*          from the library to send Model Opcode Table info to library
* @param  property_ID: Property id of the parameter.
* @retval MOBLEUINT32
*/ 
MOBLEUINT32 Light_LC_GetPropertyID_value(MOBLEUINT16 property_ID,MOBLEUINT16 *value_length)                                             
{
   MOBLEUINT8 length;
  
    length = sizeof(Light_Property_Table.LC_PropertyTable8b)/sizeof(Light_Property_Table.LC_PropertyTable8b[0]);
  
    for(int i=0;i < length;i++)
  {
    if(property_ID == Light_Property_Table.LC_PropertyTable8b[i].Property_ID)
    {
        *value_length = ONE_BYTE_VALUE;
      return Light_Property_Table.LC_PropertyTable8b[i].Property_Value_8b;
    }
  }
  
  length = sizeof(Light_Property_Table.LC_PropertyTable16b)/sizeof(Light_Property_Table.LC_PropertyTable16b[0]);
    for(int i=0;i < length;i++)
  {
    if(property_ID == Light_Property_Table.LC_PropertyTable16b[i].Property_ID)
    {
        *value_length = TWO_BYTE_VALUE;
      return Light_Property_Table.LC_PropertyTable16b[i].Property_Value_16;
    }
  }
  
  length = sizeof(Light_Property_Table.LC_PropertyTable24b)/sizeof(Light_Property_Table.LC_PropertyTable24b[0]);
    for(int i=0;i < length;i++)
  {
    if(property_ID == Light_Property_Table.LC_PropertyTable24b[i].Property_ID)
    {
        *value_length = THREE_BYTE_VALUE;
      return Light_Property_Table.LC_PropertyTable24b[i].Property_Value_24b;
    }
  }
  
  length = sizeof(Light_Property_Table.LC_PropertyTableFloat)/sizeof(Light_Property_Table.LC_PropertyTableFloat[0]);
    for(int i=0;i < length;i++)
  {
    if(property_ID == Light_Property_Table.LC_PropertyTableFloat[i].Property_ID)
    {
        *value_length = FOUR_BYTE_VALUE;
      return (MOBLEUINT32)Light_Property_Table.LC_PropertyTableFloat[i].Property_Value_float;
    }
  }
   
  TRACE_I(TF_LIGHT_LC_M,"Wrong Property ID \r\n");
  return 0xFFFF;
    
}


/**
* @brief  Get_LengthInByteProprtyValue:Function return the length of the byte length 
*         of the Property value.
* @param MOBLEUINT16.
* retval MOBLEUINT16.
*/
MOBLEUINT16 Get_LengthInByteProprtyValue(MOBLEUINT16 property_ID)
{
  MOBLEUINT16 prop_value_length = 0;
  
  Light_LC_GetPropertyID_value(property_ID,&prop_value_length);
  return prop_value_length;
  
}
#endif

/**
* @brief   Light_LC_ModelServer_GetOpcodeTableCb: This function is call-back 
*          from the library to send Model Opcode Table info to library
* @param  MODEL_OpcodeTableParam_t:  Pointer to the Generic Model opcode array 
* @param  length: Pointer to the Length of Generic Model opcode array
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT Light_LC_ModelServer_GetOpcodeTableCb(const MODEL_OpcodeTableParam_t **data, 
                                                 MOBLEUINT16 *length)
{
  *data = Light_LC_Opcodes_Table;
  *length = sizeof(Light_LC_Opcodes_Table)/sizeof(Light_LC_Opcodes_Table[0]);
  
  return MOBLE_RESULT_SUCCESS;
}

#ifdef ENABLE_LIGHT_MODEL_SERVER_LC_SETUP 
/**
* @brief  Light_LC_ModelServer_GetStatusRequestCb : This function is call-back 
*         from the library to send response to the message from peer
* @param  peer_addr: Address of the peer
* @param  dst_peer: destination send by peer for this node. It can be a
*                                                     unicast or group address 
* @param  opcode: Received opcode of the Status message callback
* @param  pResponsedata: Pointer to the buffer to be updated with status
* @param  plength: Pointer to the Length of the data, to be updated by application
* @param  pRxData: Pointer to the data received in packet.
* @param  dataLength: length of the data in packet.
* @param  response: Value to indicate wheather message is acknowledged meassage or not.
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT Light_LC_ModelServer_GetStatusRequestCb(MOBLE_ADDRESS peer_addr, 
                                    MOBLE_ADDRESS dst_peer, 
                                    MOBLEUINT16 opcode, 
                                    MOBLEUINT8 *pResponsedata, 
                                    MOBLEUINT32 *plength, 
                                    MOBLEUINT8 const *pRxData,
                                    MOBLEUINT32 dataLength,
                                    MOBLEBOOL response)

{
  switch(opcode)
  {
#ifdef ENABLE_LIGHT_MODEL_SERVER_LC 
    
    case LIGHT_LC_MODE_STATUS:
      {   
        Light_LC_ModeStatus(pResponsedata,plength);
        break;
      }   
    case LIGHT_LC_OM_STATUS:
      {
        Light_LC_OMStatus(pResponsedata,plength);
        break;
      }
    case LIGHT_LC_ON_OFF_STATUS:
      {
        Light_LC_OnOffStatus(pResponsedata,plength);
        break;
      }
#endif

#ifdef ENABLE_LIGHT_MODEL_SERVER_LC_SETUP
      
    case LIGHT_LC_PROPERTY_STATUS:
      {
        Light_LC_PropertyStatus(pResponsedata ,plength,pRxData,dataLength);
        break;
      }
#endif      
    default:
      {
        break;
      }
    }
  return MOBLE_RESULT_SUCCESS;    
}


/**
* @brief  Light_LC_ModelServer_ProcessMessageCb: This is a callback function from
*         the library whenever a Generic Model message is received
* @param  peer_addr: Address of the peer
* @param  dst_peer: destination send by peer for this node. It can be a
*                                                     unicast or group address 
* @param  opcode: Received opcode of the Status message callback
* @param  pData: Pointer to the buffer to be updated with status
* @param  length: Length of the parameters received 
* @param  response: if TRUE, the message is an acknowledged message
* @param  pRxData: Pointer to the data received in packet.
* @param  dataLength: length of the data in packet.
* @param  response: Value to indicate wheather message is acknowledged meassage or not.
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT Light_LC_ModelServer_ProcessMessageCb(MOBLE_ADDRESS peer_addr, 
                                                 MOBLE_ADDRESS dst_peer, 
                                                 MOBLEUINT16 opcode, 
                                                 MOBLEUINT8 const *pRxData, 
                                                 MOBLEUINT32 dataLength, 
                                                 MOBLEBOOL response
                                                   )
{
  
  MOBLE_RESULT result = MOBLE_RESULT_SUCCESS;
  MOBLEUINT16 property_ID;
  MOBLEUINT16 prop_length;
  MOBLE_ADDRESS publishAddress;
  MOBLEUINT8 elementNumber;
  MOBLEUINT8 modelStateChangeFlag = MOBLE_FALSE;  
  MOBLEUINT16 model_ID = 0;
  MOBLE_ADDRESS my_Address;  
 
  Dst_Peer = dst_peer;
  my_Address = BLEMesh_GetAddress();

  TRACE_M(TF_LIGHT_LC_M,"dst_peer = %.2X , peer_add = %.2X, opcode= %.2X,response= %.2X \r\n ",dst_peer, peer_addr, opcode,response);

  switch(opcode)
  {
#ifdef ENABLE_LIGHT_MODEL_SERVER_LC 
    
    case LIGHT_LC_MODE_SET:
    case LIGHT_LC_MODE_SET_UNACK:
      { 
        Light_LC_ModeSet(pRxData,dataLength);        
        modelStateChangeFlag = MOBLE_TRUE;
        model_ID =(MOBLEUINT16)LIGHT_MODEL_SERVER_LC_MODEL_ID;
        break;
      } 
    case LIGHT_LC_OM_SET:
    case LIGHT_LC_OM_SET_UNACK:
      {
        Light_LC_OMSet(pRxData,dataLength);       
        modelStateChangeFlag = MOBLE_TRUE;
        model_ID =(MOBLEUINT16)LIGHT_MODEL_SERVER_LC_MODEL_ID;
        break;
      } 
    case LIGHT_LC_ON_OFF_SET:
    case LIGHT_LC_ON_OFF_SET_UNACK:
      {
        if(!MOBLE_FAILED(result = Chk_TidValidity(peer_addr,dst_peer,pRxData[1])))
        {
          Light_LC_OnOffSet(pRxData,dataLength);
          modelStateChangeFlag = MOBLE_TRUE;
          model_ID =(MOBLEUINT16)LIGHT_MODEL_SERVER_LC_MODEL_ID;
        }      
        break;
      }
#endif

#ifdef ENABLE_LIGHT_MODEL_SERVER_LC_SETUP 
      
    case LIGHT_LC_PROPERTY_SET:
    case LIGHT_LC_PROPERTY_SET_UNACK:
      {
        property_ID = (pRxData[0] | pRxData[1] << 8);
        prop_length = dataLength -2;        
        if((property_ID != 0x00) && (prop_length == Get_LengthInByteProprtyValue(property_ID)))
        {
        Light_LC_PropertySet(pRxData,dataLength);        
          modelStateChangeFlag = MOBLE_TRUE;
          model_ID =(MOBLEUINT16)LIGHT_MODEL_SERVER_LC_SETUP_MODEL_ID;
          result = MOBLE_RESULT_SUCCESS;
        }
        else
        {
          result = MOBLE_RESULT_FAIL;         
        }
        break;
      }
  case LIGHT_LC_PROPERTY_GET:
    {
      property_ID = (pRxData[0] | pRxData[1] << 8);
      if(property_ID == 0x00)
      {
        result = MOBLE_RESULT_FAIL;
      }
        break;
      } 
#endif      
    default:
      {
        break;
      }          
  } /* Switch ends */
  
  if((result == MOBLE_RESULT_SUCCESS) && (response == MOBLE_TRUE) && (ADDRESS_IS_UNICAST(dst_peer)))
  {
    Model_SendResponse(peer_addr,my_Address,opcode,pRxData,dataLength);
  }
  /*
     Publish the status to publish address if the publication is set by client 
     publication is independent of the response of the message.if thek condition 
     for publication is full filled as per specification then the status will be 
     published.
  */  
    elementNumber = BLE_GetElementNumber();
    publishAddress = BLEMesh_GetPublishAddress(elementNumber,model_ID);
  
  if((result == MOBLE_RESULT_SUCCESS) && (publishAddress != 0x0000) && (modelStateChangeFlag == MOBLE_TRUE)
     && (ADDRESS_IS_UNICAST(dst_peer)))
  {
    Model_SendResponse(publishAddress,my_Address,opcode,pRxData,dataLength);
    
    modelStateChangeFlag = MOBLE_FALSE;  
    TRACE_I(TF_LIGHT_LC_M,"Publishing state to the address  %.2X \r\n",publishAddress);
  }
  
  return MOBLE_RESULT_SUCCESS;
}
 
/**
* @brief  Light_LC_Fsm: This function is state machine for the light lc
  controller,which works on the light on off event and sensor occupany event. it 
  will publsih the light linear set command for the nodes, values will be decided
  according to the controller state.
* @param  void:
* @retval void:    
**/
void Light_LC_Fsm(void)
{
  MOBLEUINT32 resetTime = Clock_Time();
  MOBLEUINT32 delta = resetTime - Timer_value;
  MOBLEUINT16 state_Value = 0;
  MOBLEUINT16 LightnessValue;
  MOBLEUINT16 luxLightnessvalue = 0;
  MOBLEUINT16 prop_value_length;
  
  static MOBLEUINT32 wait_time;
    
  switch(LightLC_States.Lc_States)
  {
    case LC_STANDBY:
      {
        if(Lc_Event == LC_LIGHT_ON)
        {
          /* when controller get the event Light on , changes state from standby to
             fade on state. controller publish the light on value to the node,  
             transition time for the light node.
          */
          LightLC_States.Lc_States = LC_FADE_ON; 
          
          /* getting the Lightness on value from the table.*/
        LightnessValue = Light_LC_GetPropertyID_value(LIGHT_CONTROL_LIGHTNESS_ON_ID,&prop_value_length);
          /* getting the Lightness value after complete calculation from the ambient 
             Lux Level and Lux Level output  from state machine.*/
          luxLightnessvalue = Light_LC_LuxLevelOutputValue(LIGHT_CONTROL_LUX_LEVEL_ON_ID);
          /* Select the maximum value of lightness on value between Lightness on from
             table and Lux Level Output.
          */
          state_Value = Light_LC_MaxLightnessValue(LightnessValue,luxLightnessvalue);
        Light_LC_TemporaryStatus.TargetParam_1 = state_Value;
          wait_time = Get_TimeToWait(LIGHT_CONTROL_TIME_FADE_ON_ID);
          Timer_value = resetTime;
        Light_LC_ModelFlag.Light_LC_Transition_Flag = LIGHT_LC_TRANSITION_START;
          TRACE_I(TF_LIGHT_LC_M,"STANDBY STATE --> FADE ON STATE , LIGHT ON EVENT \r\n\n");
        }
        else if(Lc_Event == LC_OCCUPANCY_ON)
        {            
          /* when controller get the event Occupancy on , changes state from standby to
             fade on state. controller publish the light on value to the node,  
             transition time for the light node.
          */
          LightLC_States.Lc_States = LC_FADE_ON;  
          
          /* getting the Lightness on value from the table.*/
        LightnessValue = Light_LC_GetPropertyID_value(LIGHT_CONTROL_LIGHTNESS_ON_ID,&prop_value_length);
          /* getting the Lightness value after complete calculation from the ambient 
             Lux Level and Lux Level output  from state machine.*/
          luxLightnessvalue = Light_LC_LuxLevelOutputValue(LIGHT_CONTROL_LUX_LEVEL_ON_ID);
          /* Select the maximum value of lightness on value between Lightness on from
             table and Lux Level Output.
          */
          state_Value = Light_LC_MaxLightnessValue(LightnessValue,luxLightnessvalue);
        Light_LC_TemporaryStatus.TargetParam_1 = state_Value;   
          wait_time = Get_TimeToWait(LIGHT_CONTROL_TIME_FADE_ON_ID);
          Timer_value = resetTime;
          TRACE_I(TF_LIGHT_LC_M,"STANDBY STATE --> FADE ON STATE , OCCUPANCY ON EVENT \r\n\n");
        }
        else
        {
          /* No Comment */
        }
        break;
      }
    case LC_FADE_ON:
      {
      Light_LC_Param.Present_Light_OnOff = Light_LC_Param.Target_Light_OnOff;
        if(Lc_Event == LC_LIGHT_OFF)
        {
          /* set the light 0x00 with transition time already by the light LC on off set */  
          LightLC_States.Lc_States = LC_FADE_STANDBY_MANUAL; 
          
          /* getting the Lightness on value from the table.*/
        LightnessValue = Light_LC_GetPropertyID_value(LIGHT_CONTROL_LIGHTNESS_STANDBY_ID,&prop_value_length);
          /* getting the Lightness value after complete calculation from the ambient 
             Lux Level and Lux Level output  from state machine.*/
          luxLightnessvalue = Light_LC_LuxLevelOutputValue(LIGHT_CONTROL_LUX_LEVEL_STANDBY_ID);
          /* Select the maximum value of lightness on value between Lightness on from
             table and Lux Level Output.
          */
          state_Value = Light_LC_MaxLightnessValue(LightnessValue,luxLightnessvalue);
        Light_LC_TemporaryStatus.TargetParam_1 = state_Value;
          
          /* Getting the wait time for the timer on the state */
          wait_time = Get_TimeToWait(LIGHT_CONTROL_TIME_FADE_STANDBY_MANUAL_ID);
        Light_LC_ModelFlag.Light_LC_Transition_Flag = LIGHT_LC_TRANSITION_START;
          TRACE_I(TF_LIGHT_LC_M,"FADE ON STATE --> RUN STATE, LIGHT OFF EVENT \r\n\n");
        }
        else 
        {           
          /* Controller wait here till the transition of light node is not completed
             so the wait time will be equal to the transition time sent by the controller
             then controller will change state to run state.
          */
          if(delta > wait_time)
          {
            LightLC_States.Lc_States = LC_RUN;
          wait_time = Light_LC_GetPropertyID_value(LIGHT_CONTROL_TIME_RUN_ON_ID,&prop_value_length);
            Timer_value = resetTime;    
            Lc_Event = LC_TIMER_OFF;
          TRACE_I(TF_LIGHT_LC_M,"FADE ON STATE --> RUN STATE , wait time = %ld \r\n",wait_time);
          }
        }     
        break;
      }
    case LC_RUN:
      {
        if(Lc_Event == LC_LIGHT_OFF)
        {
          
          LightLC_States.Lc_States = LC_FADE_STANDBY_MANUAL; 
          
          /* getting the Lightness on value from the table*/
        LightnessValue = Light_LC_GetPropertyID_value(LIGHT_CONTROL_LIGHTNESS_STANDBY_ID,&prop_value_length);
          /* getting the Lightness value after complete calculation from the ambient 
             Lux Level and Lux Level output  from state machine.*/
          luxLightnessvalue = Light_LC_LuxLevelOutputValue(LIGHT_CONTROL_LUX_LEVEL_STANDBY_ID);
          /* Select the maximum value of lightness on value between Lightness on from
             table and Lux Level Output.
          */
          state_Value = Light_LC_MaxLightnessValue(LightnessValue,luxLightnessvalue);
        Light_LC_TemporaryStatus.TargetParam_1 = state_Value; 
                 
          wait_time = Get_TimeToWait(LIGHT_CONTROL_TIME_FADE_STANDBY_MANUAL_ID);
        Light_LC_ModelFlag.Light_LC_Transition_Flag = LIGHT_LC_TRANSITION_START;
        TRACE_I(TF_LIGHT_LC_M,"RUN STATE --> STANDBY MANUAL STATE , LIGHT OFF EVENT \r\n");
        }
        else if(Lc_Event == LC_OCCUPANCY_ON)
        {
          /* 
             when controller is in run state already and it gets the Occupancy on event,
             then the next state is run state.
          */ 
          LightLC_States.Lc_States = LC_RUN; 
          Timer_value = resetTime;
          Lc_Event = LC_TIMER_OFF;  
          TRACE_I(TF_LIGHT_LC_M,"RUN STATE --> RUN STATE , OCCUPANCY EVENT \r\n");
        }
        else if(Lc_Event == LC_LIGHT_ON)
        {
          /* 
             when controller is in run state already and it gets the light on event,
             then the next state is run state.
          */ 
          LightLC_States.Lc_States = LC_RUN;        
          Timer_value = resetTime;
          Lc_Event = LC_TIMER_OFF;
          TRACE_I(TF_LIGHT_LC_M,"RUN STATE --> RUN STATE , LIGHT ON EVENT \r\n\n");
        }      
        else if(Lc_Event == LC_TIMER_OFF)
        {
          /*
            when controller is not getting any event and it has completed its wait 
            time of run state then it will change the state to the fade state by 
            send the new light node value and transtion time .
          */ 
          if(delta > wait_time) 
          {          
            LightLC_States.Lc_States = LC_FADE;
            
          LightnessValue = Light_LC_GetPropertyID_value(LIGHT_CONTROL_LIGHTNESS_PROLONG_ID,&prop_value_length);
            /* getting the Lightness value after complete calculation from the ambient 
             Lux Level and Lux Level output  from state machine.*/
            luxLightnessvalue = Light_LC_LuxLevelOutputValue(LIGHT_CONTROL_LUX_LEVEL_PROLONG_ID);
            /* Select the maximum value of lightness on value between Lightness on from
             table and Lux Level Output.
            */
            state_Value = Light_LC_MaxLightnessValue(LightnessValue,luxLightnessvalue);
          Light_LC_TemporaryStatus.TargetParam_1 = state_Value;  
                     
            wait_time = Get_TimeToWait(LIGHT_CONTROL_TIME_FADE_ID);
          Light_LC_ModelFlag.Light_LC_Transition_Flag = LIGHT_LC_TRANSITION_START;
            Timer_value = resetTime;
            Lc_Event = LC_TIMER_OFF;
          TRACE_I(TF_LIGHT_LC_M,"RUN STATE --> FADE STATE , wait time = %ld \r\n",wait_time);
          }
        }
        else
        {
          /* No Comment */
        }
        break;
      }
    case LC_FADE:
      {
        if(Lc_Event == LC_LIGHT_OFF)
        {
          /* set the light 0x00 with transition time already by the light LC on off set */        
          LightLC_States.Lc_States = LC_FADE_STANDBY_MANUAL;
          
        LightnessValue = Light_LC_GetPropertyID_value(LIGHT_CONTROL_LIGHTNESS_STANDBY_ID,&prop_value_length);
          /* getting the Lightness value after complete calculation from the ambient 
           Lux Level and Lux Level output  from state machine.*/
          luxLightnessvalue = Light_LC_LuxLevelOutputValue(LIGHT_CONTROL_LUX_LEVEL_STANDBY_ID);
          /* Select the maximum value of lightness on value between Lightness on from
           table and Lux Level Output.
          */
          state_Value = Light_LC_MaxLightnessValue(LightnessValue,luxLightnessvalue);
        Light_LC_TemporaryStatus.TargetParam_1 = state_Value; 
                 
          wait_time = Get_TimeToWait(LIGHT_CONTROL_TIME_FADE_STANDBY_MANUAL_ID);
        Light_LC_ModelFlag.Light_LC_Transition_Flag = LIGHT_LC_TRANSITION_START;
        TRACE_I(TF_LIGHT_LC_M,"FADE STATE --> STANDBY MANUAL , LIGHT OFF EVENT \r\n");
        }
        else if(Lc_Event == LC_OCCUPANCY_ON)
        {
          /* 
             when controller is in fade state and it gets the Occupancy on event,
             then the next state is fade on state.
          */ 
          LightLC_States.Lc_States = LC_FADE_ON;
          
        LightnessValue = Light_LC_GetPropertyID_value(LIGHT_CONTROL_LIGHTNESS_ON_ID,&prop_value_length);
          /* getting the Lightness value after complete calculation from the ambient 
           Lux Level and Lux Level output  from state machine.*/
          luxLightnessvalue = Light_LC_LuxLevelOutputValue(LIGHT_CONTROL_LUX_LEVEL_ON_ID);
          /* Select the maximum value of lightness on value between Lightness on from
           table and Lux Level Output.
          */
          state_Value = Light_LC_MaxLightnessValue(LightnessValue,luxLightnessvalue);
        Light_LC_TemporaryStatus.TargetParam_1 = state_Value;
          
          wait_time = Get_TimeToWait(LIGHT_CONTROL_TIME_FADE_ON_ID);
          Lc_Event = LC_TIMER_OFF;
          Timer_value = resetTime;
        Light_LC_ModelFlag.Light_LC_Transition_Flag = LIGHT_LC_TRANSITION_START;
        TRACE_I(TF_LIGHT_LC_M,"FADE STATE --> FADE ON STATE , OCCUPANCY ON EVENT \r\n");
        }
        else if(Lc_Event == LC_LIGHT_ON)
        {
          /* 
             when controller is in fade state and it gets the Light on event,
             then the next state is fade on state.
          */ 
          LightLC_States.Lc_States = LC_FADE_ON;
          
        LightnessValue = Light_LC_GetPropertyID_value(LIGHT_CONTROL_LIGHTNESS_ON_ID,&prop_value_length);
          /* getting the Lightness value after complete calculation from the ambient 
           Lux Level and Lux Level output  from state machine.*/
          luxLightnessvalue = Light_LC_LuxLevelOutputValue(LIGHT_CONTROL_LUX_LEVEL_ON_ID);
          /* Select the maximum value of lightness on value between Lightness on from
           table and Lux Level Output.
          */
          state_Value = Light_LC_MaxLightnessValue(LightnessValue,luxLightnessvalue);
        Light_LC_TemporaryStatus.TargetParam_1 = state_Value;
          
          wait_time = Get_TimeToWait(LIGHT_CONTROL_TIME_FADE_ON_ID);
          Timer_value = resetTime;
        Light_LC_ModelFlag.Light_LC_Transition_Flag = LIGHT_LC_TRANSITION_START;
          Lc_Event = LC_TIMER_OFF;
        TRACE_I(TF_LIGHT_LC_M,"FADE STATE --> FADE ON STATE , LIGHT ON EVENT \r\n");
        }  
        else if(Lc_Event == LC_TIMER_OFF)
        {
          /* Controller wait here till the transition of light node is not completed
             so the wait time will be equal to the transition time sent by the controller
             then controller will change state to prolong state.
          */
          if(delta > wait_time) 
          {          
            LightLC_States.Lc_States = LC_PROLONG;                  
          wait_time = Light_LC_GetPropertyID_value(LIGHT_CONTROL_TIME_PROLONG_ID,&prop_value_length);                
            Timer_value = resetTime;
            Lc_Event = LC_TIMER_OFF;
          TRACE_I(TF_LIGHT_LC_M,"FADE STATE --> PROLONG STATE , wait time = %ld \r\n",wait_time);
          }
        }
         else
        {
          /* no comments */
        }     
        break;
      }
    case LC_PROLONG:
      {
        if(Lc_Event == LC_LIGHT_OFF)
        {
          /* set the light 0x00 with transition time already by the light LC on off set */       
          LightLC_States.Lc_States = LC_FADE_STANDBY_MANUAL;
          
        LightnessValue = Light_LC_GetPropertyID_value(LIGHT_CONTROL_LIGHTNESS_STANDBY_ID,&prop_value_length);
          /* getting the Lightness value after complete calculation from the ambient 
           Lux Level and Lux Level output  from state machine.*/
          luxLightnessvalue = Light_LC_LuxLevelOutputValue(LIGHT_CONTROL_LUX_LEVEL_STANDBY_ID);
          /* Select the maximum value of lightness on value between Lightness on from
           table and Lux Level Output.
          */
          state_Value = Light_LC_MaxLightnessValue(LightnessValue,luxLightnessvalue);
        Light_LC_TemporaryStatus.TargetParam_1 = state_Value;
          
          wait_time = Get_TimeToWait(LIGHT_CONTROL_TIME_FADE_STANDBY_MANUAL_ID);
        Light_LC_ModelFlag.Light_LC_Transition_Flag = LIGHT_LC_TRANSITION_START;
        TRACE_I(TF_LIGHT_LC_M,"PROLONG STATE --> STANDBY MANUAL , LIGHT OFF EVENT \r\n");
        }
        else if(Lc_Event == LC_OCCUPANCY_ON)
        {
          /* 
             when controller is in prolong state and it gets the Occupancy on event,
             then the next state is fade on state.
          */
            LightLC_States.Lc_States = LC_FADE_ON; 

          LightnessValue = Light_LC_GetPropertyID_value(LIGHT_CONTROL_LIGHTNESS_ON_ID,&prop_value_length);
            /* getting the Lightness value after complete calculation from the ambient 
            Lux Level and Lux Level output  from state machine.*/
            luxLightnessvalue = Light_LC_LuxLevelOutputValue(LIGHT_CONTROL_LUX_LEVEL_ON_ID);
            /* Select the maximum value of lightness on value between Lightness on from
              table and Lux Level Output.
           */
            state_Value = Light_LC_MaxLightnessValue(LightnessValue,luxLightnessvalue);
          Light_LC_TemporaryStatus.TargetParam_1 = state_Value; 
                     
            wait_time = Get_TimeToWait(LIGHT_CONTROL_TIME_FADE_ON_ID);
            Timer_value = resetTime;
          Light_LC_ModelFlag.Light_LC_Transition_Flag = LIGHT_LC_TRANSITION_START;
          TRACE_I(TF_LIGHT_LC_M,"STANDBY AUTO --> FADE ON STATE , OCCUPANCY ON EVENT \r\n");
        }
        else if(Lc_Event == LC_LIGHT_ON)
        {
          /* 
             when controller is in prolong state and it gets the Light on event,
             then the next state is fade on state.
          */
          LightLC_States.Lc_States = LC_FADE_ON; 
          
        LightnessValue = Light_LC_GetPropertyID_value(LIGHT_CONTROL_LIGHTNESS_ON_ID,&prop_value_length);
          /* getting the Lightness value after complete calculation from the ambient 
          Lux Level and Lux Level output  from state machine.*/
          luxLightnessvalue = Light_LC_LuxLevelOutputValue(LIGHT_CONTROL_LUX_LEVEL_ON_ID);
          /* Select the maximum value of lightness on value between Lightness on from
            table and Lux Level Output.
         */
          state_Value = Light_LC_MaxLightnessValue(LightnessValue,luxLightnessvalue);
        Light_LC_TemporaryStatus.TargetParam_1 = state_Value;
                  
          wait_time = Get_TimeToWait(LIGHT_CONTROL_TIME_FADE_ON_ID);
          Timer_value = resetTime; 
        Light_LC_ModelFlag.Light_LC_Transition_Flag = LIGHT_LC_TRANSITION_START;
        TRACE_I(TF_LIGHT_LC_M,"PROLONG STATE --> FADE ON STATE , LIGHT ON EVENT \r\n");
        }
        else if(Lc_Event == LC_TIMER_OFF)
        {
         /*
            when controller is not getting any event and it has completed its wait 
            time of prolong state then it will change the state to the fade standby 
            and send the new light node value and transtion time .
          */ 
         if(delta > wait_time)
          {
             LightLC_States.Lc_States = LC_FADE_STANDBY_AUTO;
             
           LightnessValue = Light_LC_GetPropertyID_value(LIGHT_CONTROL_LIGHTNESS_STANDBY_ID,&prop_value_length);
             /* getting the Lightness value after complete calculation from the ambient 
             Lux Level and Lux Level output  from state machine.*/
             luxLightnessvalue = Light_LC_LuxLevelOutputValue(LIGHT_CONTROL_LUX_LEVEL_STANDBY_ID);
             /* Select the maximum value of lightness on value between Lightness on from
                table and Lux Level Output.
             */
             state_Value = Light_LC_MaxLightnessValue(LightnessValue,luxLightnessvalue);
           Light_LC_TemporaryStatus.TargetParam_1 = state_Value;
                   
             wait_time = Get_TimeToWait(LIGHT_CONTROL_TIME_FADE_STANDBY_AUTO_ID);
           Light_LC_ModelFlag.Light_LC_Transition_Flag = LIGHT_LC_TRANSITION_START;
             Timer_value = resetTime;    
           TRACE_I(TF_LIGHT_LC_M,"PROLONG STATE --> STANDBY AUTO STATE , wait time = %ld \r\n",wait_time);
          }            
        }
         else
        {
          /* no comments */
        }
        break;
      }
    case LC_FADE_STANDBY_AUTO:
      {
        if(Lc_Event == LC_LIGHT_OFF)
        {
          /* set the light 0x00 with transition time already by the light LC on off set */       
          LightLC_States.Lc_States = LC_FADE_STANDBY_MANUAL;
          
        LightnessValue = Light_LC_GetPropertyID_value(LIGHT_CONTROL_LIGHTNESS_STANDBY_ID,&prop_value_length);
          /* getting the Lightness value after complete calculation from the ambient 
          Lux Level and Lux Level output  from state machine.*/
          luxLightnessvalue = Light_LC_LuxLevelOutputValue(LIGHT_CONTROL_LUX_LEVEL_STANDBY_ID);
          /* Select the maximum value of lightness on value between Lightness on from
             table and Lux Level Output.
          */
          state_Value = Light_LC_MaxLightnessValue(LightnessValue,luxLightnessvalue);
        Light_LC_TemporaryStatus.TargetParam_1 = state_Value;
         
          wait_time = Get_TimeToWait(LIGHT_CONTROL_TIME_FADE_STANDBY_MANUAL_ID);
        Light_LC_ModelFlag.Light_LC_Transition_Flag = LIGHT_LC_TRANSITION_START;
        TRACE_I(TF_LIGHT_LC_M,"STANDBY AUTO STATE --> STANDBY MANUAL , LIGHT OFF EVENT \r\n");
        }
        else if(Lc_Event == LC_OCCUPANCY_ON)
        {
          /* 
             when controller is in standby auto state and it gets the occupancy on event,
             then the next state is fade on state.
          */
            LightLC_States.Lc_States = LC_FADE_ON;
            
          LightnessValue = Light_LC_GetPropertyID_value(LIGHT_CONTROL_LIGHTNESS_ON_ID,&prop_value_length);
            /* getting the Lightness value after complete calculation from the ambient 
            Lux Level and Lux Level output  from state machine.*/
            luxLightnessvalue = Light_LC_LuxLevelOutputValue(LIGHT_CONTROL_LUX_LEVEL_ON_ID);
            /* Select the maximum value of lightness on value between Lightness on from
                table and Lux Level Output.
            */
            state_Value = Light_LC_MaxLightnessValue(LightnessValue,luxLightnessvalue);
          Light_LC_TemporaryStatus.TargetParam_1 = state_Value;
            
            wait_time = Get_TimeToWait(LIGHT_CONTROL_TIME_FADE_ON_ID);
            Timer_value = resetTime; 
          Light_LC_ModelFlag.Light_LC_Transition_Flag = LIGHT_LC_TRANSITION_START;
          TRACE_I(TF_LIGHT_LC_M,"STANDBY AUTO --> FADE ON STATE , OCCUPANCY ON EVENT \r\n");
        }
        else if(Lc_Event == LC_LIGHT_ON)
        {
          /* 
             when controller is in standby auto state and it gets the Light on event,
             then the next state is fade on state.
          */
          LightLC_States.Lc_States = LC_FADE_ON;  
          
        LightnessValue = Light_LC_GetPropertyID_value(LIGHT_CONTROL_LIGHTNESS_ON_ID,&prop_value_length);
          /* getting the Lightness value after complete calculation from the ambient 
          Lux Level and Lux Level output  from state machine.*/
          luxLightnessvalue = Light_LC_LuxLevelOutputValue(LIGHT_CONTROL_LUX_LEVEL_ON_ID);
          /* Select the maximum value of lightness on value between Lightness on from
              table and Lux Level Output.
          */
          state_Value = Light_LC_MaxLightnessValue(LightnessValue,luxLightnessvalue);
        Light_LC_TemporaryStatus.TargetParam_1 = state_Value;
          
          wait_time = Get_TimeToWait(LIGHT_CONTROL_TIME_FADE_ON_ID);
          Timer_value = resetTime; 
        Light_LC_ModelFlag.Light_LC_Transition_Flag = LIGHT_LC_TRANSITION_START;
        TRACE_I(TF_LIGHT_LC_M,"STANDBY AUTO --> FADE ON STATE , LIGHT ON EVENT \r\n");
        }
        else if(Lc_Event == LC_TIMER_OFF)
        {
          /* Controller wait here till the transition of light node is not completed
             so the wait time will be equal to the transition time sent by the controller
             then controller will change state to standby state.
          */
          if(delta > wait_time)
          {
             LightLC_States.Lc_States = LC_STANDBY;
             Lc_Event = LC_NO_EVENT;
             Timer_value = resetTime;
           TRACE_I(TF_LIGHT_LC_M,"STANDBY AUTO --> STANDBY STATE , wait time = %ld \r\n",wait_time);
          }       
        }
         else
        {
          /* no comments */
        }
        break;
      }
    case LC_FADE_STANDBY_MANUAL:
      {
        /* 
           when controller is in run state already and it gets the Light on event,
           then the next state is fade on state.
        */
        if(Lc_Event == LC_LIGHT_ON)
        {                
          LightLC_States.Lc_States = LC_FADE_ON; 
        
        LightnessValue = Light_LC_GetPropertyID_value(LIGHT_CONTROL_LIGHTNESS_ON_ID,&prop_value_length);
          /* getting the Lightness value after complete calculation from the ambient 
          Lux Level and Lux Level output  from state machine.*/
          luxLightnessvalue = Light_LC_LuxLevelOutputValue(LIGHT_CONTROL_LUX_LEVEL_ON_ID);
          /* Select the maximum value of lightness on value between Lightness on from
              table and Lux Level Output.
          */
          state_Value = Light_LC_MaxLightnessValue(LightnessValue,luxLightnessvalue);
        Light_LC_TemporaryStatus.TargetParam_1 = state_Value;
             
          wait_time = Get_TimeToWait(LIGHT_CONTROL_TIME_FADE_ON_ID);
          Timer_value = resetTime; 
        Light_LC_ModelFlag.Light_LC_Transition_Flag = LIGHT_LC_TRANSITION_START;
        TRACE_I(TF_LIGHT_LC_M,"STANDBY MANUAL STATE --> FADE ON STATE , LIGHT ON EVENT \r\n");
        }
        else
        { 
          /* Controller wait here till the transition of light node is not completed
             so the wait time will be equal to the transition time sent by the controller
             then controller will change state to standby state.
          */
          if(delta > wait_time)
          { 
            LightLC_States.Lc_States = LC_STANDBY;  
            Lc_Event = LC_NO_EVENT;
            Timer_value = resetTime;
          TRACE_I(TF_LIGHT_LC_M,"STANDBY MANUAL STATE --> STANDBY STATE , wait time = %ld \r\n",wait_time);
          }
        }
        break;
      }
    default:
      {
        break;
      }
  }
}

/**
* @brief Light_LC_TransitionBehaviourSingle_Param funtion is used for the Light LC Lightness model
*        when transition time is  received in message.This function is used for 
*        single paramter transition.
* @param GetValue: Pointer of the array
* @retval MOBLE_RESULT
*/
MOBLE_RESULT Light_LC_TransitionBehaviourSingle_Param(MOBLEUINT8 *GetValue)
  { 
  
  static MOBLEUINT8 Clockflag = 0;
  static MOBLEUINT32 Check_time;
  MOBLEUINT16 targetRange;
  MOBLEUINT16 targetSlot;
  
  if(Clockflag == 0)
  {
    Check_time = Clock_Time();
    Clockflag = 1;
  }
  /* Values from application are copied into temporary vaiables for processing */    
  Light_LC_TemporaryStatus.PresentParam_1 = GetValue[1] << 8;
  Light_LC_TemporaryStatus.PresentParam_1 |= GetValue[0];   
  /*if condition to wait untill the time is equal to the given resolution time */  
  if(((Clock_Time()- Check_time) >= Light_LC_TimeParam.Res_Value))
  { 
    if(Light_LC_TimeParam.StepValue == 0)
      {
      Light_LC_TimeParam.StepValue = 1;
    }
        
    if(Light_LC_TemporaryStatus.TargetParam_1 > Light_LC_TemporaryStatus.PresentParam_1)
    {
      /* calulating the target range to be acheived which is traget value
      which is traget value minus present value if target value is greater 
      than present value.   
      */
      targetRange = Light_LC_TemporaryStatus.TargetParam_1 - Light_LC_TemporaryStatus.PresentParam_1;  
      /*target slot = time to cover in single step */
      targetSlot = targetRange/Light_LC_TimeParam.StepValue;
      /* target slot added to present value to achieve target value */
      Light_LC_TemporaryStatus.PresentParam_1 += targetSlot;             
      }
    else
     {
      /* if present value is greater than target value, this condition executes */
      targetRange = Light_LC_TemporaryStatus.PresentParam_1 - Light_LC_TemporaryStatus.TargetParam_1;;
      targetSlot = targetRange/Light_LC_TimeParam.StepValue;          
      Light_LC_TemporaryStatus.PresentParam_1 -= targetSlot;
     }
      
    Light_LC_TimeParam.StepValue--;         
    /* updating the remaining time after each step covered*/
    Light_LC_Param.Remaining_Time =  Light_LC_TimeParam.StepValue  | (Light_LC_TimeParam.ResBitValue << 6) ;   
    Light_LC_ModelFlag.LightLCUpdateFlag = VALUE_UPDATE_SET;
    Check_time = 0;
    Clockflag = 0;
    
    /* when transition is completed, disable the transition by disabling 
    transition flag
    */
    if(Light_LC_TimeParam.StepValue == 0)
   {
      Light_LC_ModelFlag.Light_LC_Transition_Flag = LIGHT_LC_TRANSITION_STOP;
      Light_LC_ModelFlag.Transition_Cmplt = 1;    
      Light_LC_Param.Present_Light_OnOff = Light_LC_Param.Target_Light_OnOff;
   }
    
 TRACE_M(TF_LIGHT_M,"Inside light lightness transmition time at %ld, Current state 0x%.2x ,target state 0x%.2x , Remaining Time 0x%.2x\n\r",
            Clock_Time(),Light_LC_TemporaryStatus.PresentParam_1,Light_LC_TemporaryStatus.TargetParam_1,\
                                                                Light_LC_Param.Remaining_Time);
    
    
   }
  
  return MOBLE_RESULT_SUCCESS;         
} 
/** 
* @brief Light_LC_GetStepValue: This function calculates values for transition time
* @param stepParam: Transition time set value of particular model message.
* retval MOBLEUINT32
*/
MOBLEUINT32 Light_LC_GetStepValue(MOBLEUINT8 stepParam)
{
  /*
  Two MSB bit of transition time is dedicated to resolution.
  00 = resolution is 100 ms.
  01 = resolution is 1000 ms. 
  10 = resolution is 10000 ms.
  11 = resolution is 600000 ms. 
  Last bits from 0 to 5th index is step number.
  */
  Light_LC_TimeParam.ResBitValue = stepParam >> 6 ;
  Light_LC_TimeParam.Res_Value = Get_StepResolutionValue(Light_LC_TimeParam.ResBitValue);
  Light_LC_TimeParam.StepValue = stepParam & 0x3F ;
  
  if(Light_LC_TimeParam.Res_Value >= 100)
  {
    Light_LC_TimeParam.Res_Value = Light_LC_TimeParam.Res_Value/TRANSITION_SCALER;
    Light_LC_TimeParam.StepValue = (Light_LC_TimeParam.StepValue * TRANSITION_SCALER);
  }
  Light_LC_TimeParam.TotalTime = Light_LC_TimeParam.Res_Value * Light_LC_TimeParam.StepValue;
  
  return Light_LC_TimeParam.TotalTime;  
}

/** 
* @brief Get_TimeToWait: This function calculates the time to wait foe any condition
* @param Proprety_ID: property id of the parameter
* retval MOBLEUINT32
*/
MOBLEUINT32 Get_TimeToWait(MOBLEUINT16 Proprety_ID)
{
  MOBLEUINT32 total_time;
  MOBLEUINT16 length;
  if(Light_LC_Param.Transition_Time > 0x00)
  {
    total_time = (MOBLEUINT32)Light_LC_GetStepValue((MOBLEUINT8)Light_LC_Param.Transition_Time);
  }
  else
  {
    total_time = Light_LC_GetPropertyID_value(Proprety_ID,&length);
    Light_LC_TimeParam.Res_Value = TRANSITION_RES_VALUE;
    Light_LC_TimeParam.StepValue = total_time/Light_LC_TimeParam.Res_Value;
  
  }
  
  TRACE_I(TF_LIGHT_LC_M,"TOTAL WAIT TIME %lu \r\n",total_time);
    
  return total_time;
}

/**
* @brief  GenericOnOff_Light_LC_Binding: Data binding b/w Generic On Off and 
*         light LC on off. this function will set the light LC on off
*         value at the time of  generic on off set. 
* @param onOff_param: Pointer to the data which needs to be checked.
* return void.
*/
void Light_LC_OnOff_Generic_OnOffBinding(void)
{
  Light_LC_GenericOnOffBinding(&Light_LC_Param);
  
  Light_Publish_Add(GENERIC_MODEL_SERVER_ONOFF_MODEL_ID, 
                    GENERIC_ON_OFF_SET_UNACK);
//  Light_PublishOpcodeList.PublishStateOpcode[Light_PublishOpcodeList.BindedStateCount]= GENERIC_ON_OFF_SET_UNACK;
//  Light_PublishOpcodeList.Model_ID[Light_PublishOpcodeList.BindedStateCount] = GENERIC_MODEL_SERVER_ONOFF_MODEL_ID;
//  Light_PublishOpcodeList.BindedStateCount++;
}
/**
* @brief  GenericOnOff_Light_LC_Binding: Data binding b/w Generic On Off and 
*         light LC on off. this function will set the light LC on off
*         value at the time of  generic on off set. 
* @param onOff_param: Pointer to the data which needs to be checked.
* return void.
*/
void GenericOnOff_Light_LC_Binding(Generic_OnOffStatus_t* onOff_param)
{
  if(onOff_param->Present_OnOff_State == 0x00)
  {
    Light_LC_Param.Present_Light_OnOff = 0x00;
  }
  else
  {
    Light_LC_Param.Present_Light_OnOff = 0x01;
  }
  Light_Publish_Add(LIGHT_MODEL_SERVER_LC_MODEL_ID, 
                    LIGHT_LC_ON_OFF_SET_UNACK);
//  Light_PublishOpcodeList.PublishStateOpcode[Light_PublishOpcodeList.BindedStateCount]= LIGHT_LC_ON_OFF_SET_UNACK;
//  Light_PublishOpcodeList.Model_ID[Light_PublishOpcodeList.BindedStateCount] = LIGHT_MODEL_SERVER_LC_MODEL_ID;
//  Light_PublishOpcodeList.BindedStateCount++;
}
/**
* @brief  Light_control_Process: Function to execute the transition state machine for
*         particular Light LC model and state machine Light LC application
* @param  void
* @retval void
*/ 
void Light_control_Process(void)
{
  MOBLEUINT8 Light_GetBuff[8];
  MOBLEUINT8 const pRxData[12] = {0};
  MOBLEUINT32 dataLength = 0;
  MOBLE_ADDRESS publishAddress;
  MOBLEUINT8 elementNumber;
  MOBLE_ADDRESS my_Address;
  if(Light_LC_Param.LC_mode == LC_MODE_ENABLE)
  {   
    Light_LC_Fsm();   
    
    if(Light_LC_ModelFlag.Light_LC_Transition_Flag == LIGHT_LC_TRANSITION_START)
    {  
      (Appli_Light_GetStatus_cb.GetLightLightness_cb)(Light_GetBuff);     
      Light_LC_TransitionBehaviourSingle_Param(Light_GetBuff);
      if(Light_LC_ModelFlag.LightLCUpdateFlag == VALUE_UPDATE_SET)
      {
        Light_LC_LightnessStateUpdate_Process(&Light_LC_TemporaryStatus);
        Light_LC_ModelFlag.LightLCUpdateFlag = VALUE_UPDATE_RESET;
      }
    }  
    
    if(Light_LC_ModelFlag.Transition_Cmplt == MOBLE_TRUE)
    {    
      elementNumber = BLE_GetElementNumber();
      my_Address = BLEMesh_GetAddress();
      publishAddress = BLEMesh_GetPublishAddress(elementNumber,LIGHT_MODEL_SERVER_LC_MODEL_ID);
      if(publishAddress != 0x0000)
      {
        Model_SendResponse(publishAddress,my_Address,LIGHT_LC_ON_OFF_SET_UNACK,pRxData,dataLength);
      }
      TRACE_I(TF_LIGHT_LC_M,"PUBLISHING OF LIGHT LC AT END OF TRANSITION \r\n");
      Light_LC_ModelFlag.Transition_Cmplt = MOBLE_FALSE;
    }
  }   
  else
    {
    /* Light LC on off set to off */
    Light_LC_Param.Present_Light_OnOff = 0x0;
    LightLC_States.Lc_States = LC_STANDBY;
  }  
}
      
/**
* @brief Light_LC_LuxLevelOutputValue: This function will return the lightness value 
         from the lux sensors.
* @param property_ID:property id of the parameter
* @retval MOBLEUINT16:    
**/
MOBLEUINT16 Light_LC_LuxLevelOutputValue(MOBLEUINT16 property_ID)
{
  MOBLEUINT16 availableLuxValue;
  MOBLEUINT16 ambientLuxValue;
  MOBLEUINT16 lightnessOutput;
  MOBLEUINT16 length;
  
  availableLuxValue = Light_LC_GetPropertyID_value(property_ID,&length);
  ambientLuxValue = Appli_LightLC_GetStatus_cb.GetLightLC_AmbientLuxLevel_cb();
  
  lightnessOutput = Appli_LightLC_GetStatus_cb.GetLightLC_PIregulatorOut_cb(availableLuxValue,ambientLuxValue);
   
  return lightnessOutput; 
    }


/**
* @brief Light_LC_MaxLightnessValue: This function will return the  maximum value 
         after comparision.
* @param Param1:paramter to the function
* @param Param2:paramter to the function
* @retval MOBLEUINT16:    
**/
MOBLEUINT16 Light_LC_MaxLightnessValue(MOBLEUINT16 Param1,MOBLEUINT16 Param2)
{
  if(Param1 > Param2)
  {
    return Param1;
  }
  else if(Param2 > Param1)
  {
    return Param2;
  }
  else
  {
    return Param1;
  }    
}

#endif
/**
* @}
*/

/**
* @}
*/

/******************* (C) COPYRIGHT 2017 STMicroelectronics *****END OF FILE****/

