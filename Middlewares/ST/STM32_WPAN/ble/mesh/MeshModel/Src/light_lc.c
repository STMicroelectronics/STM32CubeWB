/**
******************************************************************************
* @file    light_lc.c
* @author  BLE Mesh Team
* @version V1.12.000
* @date    06-12-2019
* @brief   Light Control model middleware file
******************************************************************************
* @attention
*
* <h2><center>&copy; COPYRIGHT(c) 2017 STMicroelectronics</center></h2>
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

/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
#ifdef ENABLE_LIGHT_MODEL_SERVER_LC_SETUP 
Lc_Event_e Lc_Event = LC_NO_EVENT;

Light_LC_States LightLC_States = {LC_STANDBY};

Light_LC_TimeParam_t Light_LC_TimeParam;

Light_LC_Param_t Light_LC_Param;

MOBLEUINT8 Light_LC_UpdateFlag = 0;
MOBLEUINT32 Timer_value;
extern MOBLEUINT8 TidSend;
#endif

MODEL_OpcodeTableParam_t Light_LC_Opcodes_Table[] = {
#ifdef ENABLE_LIGHT_MODEL_SERVER_LC  
 {LIGHT_MODEL_SERVER_LC_MODEL_ID    ,LIGHT_LC_MODE_GET,                       MOBLE_TRUE,   0, 0,         LIGHT_LC_MODE_STATUS     , 1, 1},
 {LIGHT_MODEL_SERVER_LC_MODEL_ID    ,LIGHT_LC_MODE_SET,                       MOBLE_TRUE,   1, 1,         LIGHT_LC_MODE_STATUS     , 1, 1},
 {LIGHT_MODEL_SERVER_LC_MODEL_ID    ,LIGHT_LC_MODE_SET_UNACK,                 MOBLE_FALSE,  1, 1,         0     , 1, 1},
 {LIGHT_MODEL_SERVER_LC_MODEL_ID    ,LIGHT_LC_MODE_STATUS,                    MOBLE_FALSE,  1, 1,         0     , 1, 1},
 {LIGHT_MODEL_SERVER_LC_MODEL_ID    ,LIGHT_LC_OM_GET,                         MOBLE_TRUE,   0, 0,         LIGHT_LC_OM_STATUS       , 1, 1},
 {LIGHT_MODEL_SERVER_LC_MODEL_ID    ,LIGHT_LC_OM_SET,                         MOBLE_TRUE,   1, 1,         LIGHT_LC_OM_STATUS       , 1, 1},
 {LIGHT_MODEL_SERVER_LC_MODEL_ID    ,LIGHT_LC_OM_SET_UNACK,                   MOBLE_FALSE,  1, 1,         0       , 1, 1},
 {LIGHT_MODEL_SERVER_LC_MODEL_ID    ,LIGHT_LC_OM_STATUS,                      MOBLE_FALSE,  1, 1,         0       , 1, 1},
 {LIGHT_MODEL_SERVER_LC_MODEL_ID    ,LIGHT_LC_ON_OFF_GET,                     MOBLE_TRUE,   0, 0,         LIGHT_LC_ON_OFF_STATUS   , 1, 3},
 {LIGHT_MODEL_SERVER_LC_MODEL_ID    ,LIGHT_LC_ON_OFF_SET,                     MOBLE_TRUE,   2, 4,         LIGHT_LC_ON_OFF_STATUS   , 1, 3},
 {LIGHT_MODEL_SERVER_LC_MODEL_ID    ,LIGHT_LC_ON_OFF_SET_UNACK,               MOBLE_FALSE,  2, 4,         0   , 1, 3},
 {LIGHT_MODEL_SERVER_LC_MODEL_ID    ,LIGHT_LC_ON_OFF_STATUS,                  MOBLE_FALSE,  1, 3,         0   , 1, 3},
#endif

#ifdef ENABLE_LIGHT_MODEL_SERVER_LC_SETUP  
 {LIGHT_MODEL_SERVER_LC_SETUP_MODEL_ID   ,LIGHT_LC_PROPERTY_GET,                   MOBLE_TRUE,   2, 2,          LIGHT_LC_PROPERTY_STATUS , 2, 10},
 {LIGHT_MODEL_SERVER_LC_SETUP_MODEL_ID   ,LIGHT_LC_PROPERTY_SET,                   MOBLE_TRUE,   2, 10,         LIGHT_LC_PROPERTY_STATUS , 2, 10},
 {LIGHT_MODEL_SERVER_LC_SETUP_MODEL_ID   ,LIGHT_LC_PROPERTY_SET_UNACK,             MOBLE_FALSE,  2, 10,         0 , 2, 10},
 {LIGHT_MODEL_SERVER_LC_SETUP_MODEL_ID   ,LIGHT_LC_PROPERTY_STATUS,                MOBLE_FALSE,  2, 10,         0 , 2, 10}, 
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
unacknowledged message
* @param  lcMode_param: Pointer to the parameters received for message
* @param  length: Length of the parameters received for message
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT Light_LC_ModeSet(MOBLEUINT8 const *lcMode_param, MOBLEUINT32 length)
{  
  
  TRACE_I(TF_LIGHT_LC, "Light_LC_ModeSet callback received \r\n");
  
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
  TRACE_I(TF_LIGHT_LC,"Light_LC_ModeStatus callback received \r\n");
  Appli_LightLC_GetStatus_cb.GetLightLC_ModeState_cb(LightLC_GetBuff);
  
  *lcMode_status = LightLC_GetBuff[0];
  *plength = 1;
  return MOBLE_RESULT_SUCCESS;
}

/**
* @brief  Light_LC_OMSet: This function is called for both Acknowledged and 
unacknowledged message
* @param  lcOM_param: Pointer to the parameters received for message
* @param  length: Length of the parameters received for message
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT Light_LC_OMSet(MOBLEUINT8 const *lcOM_param, MOBLEUINT32 length)
{
  
  TRACE_I(TF_LIGHT_LC,"Light_LC_OMSet callback received \r\n");
  
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
  TRACE_I(TF_LIGHT_LC,"Light_LC_OMStatus callback received \r\n");
  
   Appli_LightLC_GetStatus_cb.GetLightLC_OMState_cb(LightLC_GetBuff);
  
  *lcOM_status = LightLC_GetBuff[0];
  *plength = 1;
  
  return MOBLE_RESULT_SUCCESS;
}

/**
* @brief  Light_LC_OnOffSet: This function is called for both Acknowledged and 
unacknowledged message
* @param  lcOnOff_param: Pointer to the parameters received for message
* @param  length: Length of the parameters received for message
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT Light_LC_OnOffSet(MOBLEUINT8 const *lcOnOff_param, MOBLEUINT32 length)
{
  TRACE_I(TF_LIGHT_LC,"Light_LC_OnOffSet callback received \r\n");
  
  Light_LC_Param.Light_OnOff = lcOnOff_param[0];
  Light_LC_Param.Tid = lcOnOff_param[1];
  MOBLEUINT16 property_ID;
  MOBLEUINT32 prop_Value;

  /* Event set to descide the state to be process for LC state machine */
  if(Light_LC_Param.Light_OnOff == 1)
  {
  Lc_Event = LC_LIGHT_ON;
  }
  else
  {
    Lc_Event = LC_LIGHT_OFF;
  }
  
  if(length > 2)
  {
    Light_LC_Param.Transition_Time = lcOnOff_param[2];
    Light_LC_Param.Delay = lcOnOff_param[3];
    
    prop_Value = Light_LC_Param.Transition_Time;
    
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
  
  /* Application Callback */
  (LightLCAppli_cb.LightLC_OnOff_Set_cb)(&Light_LC_Param , 0);
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
  MOBLEUINT8 LightLC_GetBuff[3];
  TRACE_I(TF_LIGHT_LC,"Light_LC_OnOffStatus callback received \r\n");
  
    Appli_LightLC_GetStatus_cb.GetLightLC_OnOffState_cb(LightLC_GetBuff);
    
    *lcOnOff_status = LightLC_GetBuff[0] ;
    *plength = 1;
   
  return MOBLE_RESULT_SUCCESS;
}

/**
* @brief  Light_LC_PropertySet: This function is called for both Acknowledged and 
unacknowledged message
* @param  lcProp_param: Pointer to the parameters received for message
* @param  length: Length of the parameters received for message
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT Light_LC_PropertySet(MOBLEUINT8 const *lcProp_param, MOBLEUINT32 length)
{
  TRACE_I(TF_LIGHT_LC,"Light_LC_PropertySet callback received \r\n");
  
  MOBLEUINT16  Light_LC_PropertyID;
  MOBLEUINT32  property_Value;
  
  Light_LC_Value_t Light_LC_Value;

  Light_LC_PropertyID = lcProp_param[1] << 8;
  Light_LC_PropertyID |= lcProp_param[0];
  
  switch (length)
  {
  case 3:
     Light_LC_Value.Property_Value = lcProp_param[2];
    break;
  case 4:
    Light_LC_Value.Property_Value = lcProp_param[2] << 8;
    Light_LC_Value.Property_Value |= lcProp_param[3];
    break;
    
  case 5:
    Light_LC_Value.Property_Value = lcProp_param[2] << 16;
    Light_LC_Value.Property_Value |= lcProp_param[3] << 8;
    Light_LC_Value.Property_Value |= lcProp_param[4];    
    break;
  
  case 6:
    Light_LC_Value.Property_Value = lcProp_param[2] << 24;
    Light_LC_Value.Property_Value |= lcProp_param[3] << 16;
    Light_LC_Value.Property_Value |= lcProp_param[4] << 8; 
    Light_LC_Value.Property_Value |= lcProp_param[5];
    
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
  
  TRACE_M(TF_LIGHT_LC,"Light_LC_PropertyStatus callback received \r\n");
  
  if(length > 0)  
  {
    prop_ID = pData[1] << 8;
    prop_ID |= pData[0]; 
  }
 
  Property_Value = Light_LC_GetPropertyID_value(prop_ID);
  
   *lcData_param = prop_ID;
  *(lcData_param+1) = prop_ID >> 8;
  
  switch (length)
  {
  case 3:
     *(lcData_param+2) = Property_Value;
     *plength = length;
    break;
  case 4:
     *(lcData_param+2) = Property_Value >> 8;
     *(lcData_param+3) = Property_Value ;
     *plength = length;
    break;
    
  case 5:
    *(lcData_param+2) = (Property_Value >> 16);
    *(lcData_param+3) = Property_Value >> 8;
    *(lcData_param+4) = Property_Value ; 
    *plength = length;
    break;
  
  case 6:
    *(lcData_param+2) = (Property_Value >> 24);
    *(lcData_param+3) = (Property_Value >> 16);
    *(lcData_param+4) = Property_Value >> 8;
    *(lcData_param+5) = Property_Value ;
    *plength = length;
    
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
  
  for(int i=0;i <= length;i++)
  {
    if(prop_ID == Light_Property_Table.LC_PropertyTable8b[i].Property_ID)
    {
       Light_Property_Table.LC_PropertyTable8b[i].Property_Value_8b = Prop_Value;
       valid_flag = 1;
       return MOBLE_RESULT_SUCCESS;
       
    }
  }
  
  length = sizeof(Light_Property_Table.LC_PropertyTable16b)/sizeof(Light_Property_Table.LC_PropertyTable16b[0]);
  
  for(int i=0;i <= length;i++)
  {
    if(prop_ID == Light_Property_Table.LC_PropertyTable16b[i].Property_ID)
    {
       Light_Property_Table.LC_PropertyTable16b[i].Property_Value_16= Prop_Value;
       valid_flag = 1;
       return MOBLE_RESULT_SUCCESS;
       
    }
  }
  
   length = sizeof(Light_Property_Table.LC_PropertyTable24b)/sizeof(Light_Property_Table.LC_PropertyTable24b[0]);
  
  for(int i=0;i <= length;i++)
  {
    if(prop_ID == Light_Property_Table.LC_PropertyTable24b[i].Property_ID)
    {
       Light_Property_Table.LC_PropertyTable24b[i].Property_Value_24b= Prop_Value;
       valid_flag = 1;
       return MOBLE_RESULT_SUCCESS;
       
    }
  }
      
     length = sizeof(Light_Property_Table.LC_PropertyTableFloat)/sizeof(Light_Property_Table.LC_PropertyTableFloat[0]);
  
  for(int i=0;i <= length;i++)
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
    TRACE_I(TF_LIGHT_LC,"Wrong Property ID \r\n");
  }
    return MOBLE_RESULT_SUCCESS;
}

/**
* @brief   Light_LC_GetPropertyID_value: This function is call-back 
*          from the library to send Model Opcode Table info to library
* @param  property_ID: Property id of the parameter.
* @retval MOBLEUINT32
*/ 
MOBLEUINT32 Light_LC_GetPropertyID_value(MOBLEUINT16 property_ID)                                             
{
  MOBLEUINT8 length = sizeof(Light_Property_Table.LC_PropertyTable8b)/sizeof(Light_Property_Table.LC_PropertyTable8b[0]);
  
  for(int i=0;i <= length;i++)
  {
    if(property_ID == Light_Property_Table.LC_PropertyTable8b[i].Property_ID)
    {
      return Light_Property_Table.LC_PropertyTable8b[i].Property_Value_8b;
    }
  }
  
  length = sizeof(Light_Property_Table.LC_PropertyTable16b)/sizeof(Light_Property_Table.LC_PropertyTable16b[0]);
  for(int i=0;i <= length;i++)
  {
    if(property_ID == Light_Property_Table.LC_PropertyTable16b[i].Property_ID)
    {
      return Light_Property_Table.LC_PropertyTable16b[i].Property_Value_16;
    }
  }
  
  length = sizeof(Light_Property_Table.LC_PropertyTable24b)/sizeof(Light_Property_Table.LC_PropertyTable24b[0]);
  for(int i=0;i <= length;i++)
  {
    if(property_ID == Light_Property_Table.LC_PropertyTable24b[i].Property_ID)
    {
      return Light_Property_Table.LC_PropertyTable24b[i].Property_Value_24b;
    }
  }
  
  length = sizeof(Light_Property_Table.LC_PropertyTableFloat)/sizeof(Light_Property_Table.LC_PropertyTableFloat[0]);
  for(int i=0;i <= length;i++)
  {
    if(property_ID == Light_Property_Table.LC_PropertyTableFloat[i].Property_ID)
    {
      return (MOBLEUINT32)Light_Property_Table.LC_PropertyTableFloat[i].Property_Value_float;
    }
  }
   
  TRACE_I(TF_LIGHT_LC,"Wrong Property ID \r\n");
  return 0xFFFF;
    
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


/**
* @brief  Light_LC_ModelServer_GetStatusRequestCb : This function is call-back 
from the library to send response to the message from peer
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
the library whenever a Generic Model message is received
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

  TRACE_M(TF_LIGHT_LC,"dst_peer = %.2X , peer_add = %.2X, opcode= %.2X,response= %.2X \r\n ",dst_peer, peer_addr, opcode,response);
  switch(opcode)
  {
#ifdef ENABLE_LIGHT_MODEL_SERVER_LC 
    
    case LIGHT_LC_MODE_SET:
    case LIGHT_LC_MODE_SET_UNACK:
      { 
        Light_LC_ModeSet(pRxData,dataLength);        
        break;
      }
    case LIGHT_LC_MODE_STATUS:
      {
        Light_LC_Client_Mode_Status(pRxData, dataLength);          
        break;
      } 
    case LIGHT_LC_OM_SET:
    case LIGHT_LC_OM_SET_UNACK:
      {
        Light_LC_OMSet(pRxData,dataLength);       
        break;
      }     
    case LIGHT_LC_OM_STATUS:
      {
        Light_LC_Client_OM_Status(pRxData, dataLength);          
        break;
      } 
    case LIGHT_LC_ON_OFF_SET:
    case LIGHT_LC_ON_OFF_SET_UNACK:
      {
        if(!MOBLE_FAILED(result = Chk_TidValidity(peer_addr,dst_peer,pRxData[6])))
        {
          Light_LC_OnOffSet(pRxData,dataLength);
        }      
        break;
      }
    case LIGHT_LC_ON_OFF_STATUS:
      {
        Light_LC_Client_ON_OFF_Status(pRxData, dataLength);          
        break;
      } 
#endif

#ifdef ENABLE_LIGHT_MODEL_SERVER_LC_SETUP 
      
    case LIGHT_LC_PROPERTY_SET:
    case LIGHT_LC_PROPERTY_SET_UNACK:
      {
        Light_LC_PropertySet(pRxData,dataLength);        
        break;
      }
    case LIGHT_LC_PROPERTY_STATUS:
      {
        Light_LC_Client_Property_Status(pRxData, dataLength);          
        break;
      } 
#endif      
    default:
      {
        break;
      }          
  } /* Switch ends */
  
  if((result == MOBLE_RESULT_SUCCESS) && (response == MOBLE_TRUE))
  {
    Model_SendResponse(peer_addr,dst_peer,opcode,pRxData,dataLength);
  }
  
  return MOBLE_RESULT_SUCCESS;
}
 
#ifdef ENABLE_LIGHT_MODEL_SERVER_LC 
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
  MOBLEUINT8 data_Buff[6];
  MOBLE_RESULT result = MOBLE_RESULT_SUCCESS;
  MOBLE_ADDRESS srcAdd;
  MOBLEUINT16 state_Value = 0;
  MOBLEUINT16 LightnessValue;
  MOBLEUINT16 luxLightnessvalue = 0;
  MOBLEUINT16 opcode;
  MOBLEUINT8 transitionTime;
  MOBLEUINT32 length = 5;
  
  static MOBLEUINT8 Publish_flag = 0;
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
          
          opcode = LIGHT_LIGHTNESS_LINEAR_SET_UNACK;
          /* getting the Lightness on value from the table.*/
          LightnessValue = Light_LC_GetPropertyID_value(LIGHT_CONTROL_LIGHTNESS_ON_ID);
          /* getting the Lightness value after complete calculation from the ambient 
             Lux Level and Lux Level output  from state machine.*/
          luxLightnessvalue = Light_LC_LuxLevelOutputValue(LIGHT_CONTROL_LUX_LEVEL_ON_ID);
          /* Select the maximum value of lightness on value between Lightness on from
             table and Lux Level Output.
          */
          state_Value = Light_LC_MaxLightnessValue(LightnessValue,luxLightnessvalue);
          transitionTime = Light_LC_GetPropertyID_value(LIGHT_CONTROL_TIME_FADE_ON_ID);
          wait_time = Get_TimeToWait(LIGHT_CONTROL_TIME_FADE_ON_ID);
          Timer_value = resetTime;
          Publish_flag = 1;
          TRACE_I(TF_LIGHT_LC,"STANDBY STATE --> FADE ON STATE , LIGHT ON EVENT \r\n\n");
        }
        else if(Lc_Event == LC_OCCUPANCY_ON)
        {            
          /* when controller get the event Occupancy on , changes state from standby to
             fade on state. controller publish the light on value to the node,  
             transition time for the light node.
          */
          LightLC_States.Lc_States = LC_FADE_ON;  
          
          opcode = LIGHT_LIGHTNESS_LINEAR_SET_UNACK;
          /* getting the Lightness on value from the table.*/
          LightnessValue = Light_LC_GetPropertyID_value(LIGHT_CONTROL_LIGHTNESS_ON_ID);
          /* getting the Lightness value after complete calculation from the ambient 
             Lux Level and Lux Level output  from state machine.*/
          luxLightnessvalue = Light_LC_LuxLevelOutputValue(LIGHT_CONTROL_LUX_LEVEL_ON_ID);
          /* Select the maximum value of lightness on value between Lightness on from
             table and Lux Level Output.
          */
          state_Value = Light_LC_MaxLightnessValue(LightnessValue,luxLightnessvalue);
            
          transitionTime = Light_LC_GetPropertyID_value(LIGHT_CONTROL_TIME_FADE_ON_ID);       
          wait_time = Get_TimeToWait(LIGHT_CONTROL_TIME_FADE_ON_ID);
          Timer_value = resetTime;
            Publish_flag = 1;
          TRACE_I(TF_LIGHT_LC,"STANDBY STATE --> FADE ON STATE , OCCUPANCY ON EVENT \r\n\n");
        }
        else
        {
          /* No Comment */
        }
        break;
      }
    case LC_FADE_ON:
      {
        if(Lc_Event == LC_LIGHT_OFF)
        {
          /* set the light 0x00 with transition time already by the light LC on off set */  
          LightLC_States.Lc_States = LC_FADE_STANDBY_MANUAL; 
          
          opcode = LIGHT_LIGHTNESS_LINEAR_SET_UNACK;
          
          /* getting the Lightness on value from the table.*/
          LightnessValue = Light_LC_GetPropertyID_value(LIGHT_CONTROL_LIGHTNESS_STANDBY_ID);
          /* getting the Lightness value after complete calculation from the ambient 
             Lux Level and Lux Level output  from state machine.*/
          luxLightnessvalue = Light_LC_LuxLevelOutputValue(LIGHT_CONTROL_LUX_LEVEL_STANDBY_ID);
          /* Select the maximum value of lightness on value between Lightness on from
             table and Lux Level Output.
          */
          state_Value = Light_LC_MaxLightnessValue(LightnessValue,luxLightnessvalue);
          
          transitionTime = Light_LC_GetPropertyID_value(LIGHT_CONTROL_TIME_FADE_STANDBY_MANUAL_ID);
          /* Getting the wait time for the timer on the state */
          wait_time = Get_TimeToWait(LIGHT_CONTROL_TIME_FADE_STANDBY_MANUAL_ID);
          Publish_flag = 1;
          TRACE_I(TF_LIGHT_LC,"FADE ON STATE --> RUN STATE, LIGHT OFF EVENT \r\n\n");
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
            wait_time = Light_LC_GetPropertyID_value(LIGHT_CONTROL_TIME_RUN_ON_ID);
            Timer_value = resetTime;    
            Lc_Event = LC_TIMER_OFF;
            TRACE_I(TF_LIGHT_LC,"FADE ON STATE --> RUN STATE , wait time = %ld \r\n\n",wait_time);
          }
        }     
        break;
      }
    case LC_RUN:
      {
        if(Lc_Event == LC_LIGHT_OFF)
        {
          
          LightLC_States.Lc_States = LC_FADE_STANDBY_MANUAL; 
          
          opcode = LIGHT_LIGHTNESS_LINEAR_SET_UNACK;
          /* getting the Lightness on value from the table*/
          LightnessValue = Light_LC_GetPropertyID_value(LIGHT_CONTROL_LIGHTNESS_STANDBY_ID);
          /* getting the Lightness value after complete calculation from the ambient 
             Lux Level and Lux Level output  from state machine.*/
          luxLightnessvalue = Light_LC_LuxLevelOutputValue(LIGHT_CONTROL_LUX_LEVEL_STANDBY_ID);
          /* Select the maximum value of lightness on value between Lightness on from
             table and Lux Level Output.
          */
          state_Value = Light_LC_MaxLightnessValue(LightnessValue,luxLightnessvalue);
                 
          transitionTime = Light_LC_GetPropertyID_value(LIGHT_CONTROL_TIME_FADE_STANDBY_MANUAL_ID);
          wait_time = Get_TimeToWait(LIGHT_CONTROL_TIME_FADE_STANDBY_MANUAL_ID);
          Publish_flag = 1;
          TRACE_I(TF_LIGHT_LC,"RUN STATE --> STANDBY MANUAL STATE , LIGHT OFF EVENT \r\n\n");
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
          TRACE_I(TF_LIGHT_LC,"RUN STATE --> RUN STATE , OCCUPANCY EVENT \r\n");
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
          TRACE_I(TF_LIGHT_LC,"RUN STATE --> RUN STATE , LIGHT ON EVENT \r\n\n");
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
            
            opcode = LIGHT_LIGHTNESS_LINEAR_SET_UNACK;
            LightnessValue = Light_LC_GetPropertyID_value(LIGHT_CONTROL_LIGHTNESS_PROLONG_ID);
            /* getting the Lightness value after complete calculation from the ambient 
             Lux Level and Lux Level output  from state machine.*/
            luxLightnessvalue = Light_LC_LuxLevelOutputValue(LIGHT_CONTROL_LUX_LEVEL_PROLONG_ID);
            /* Select the maximum value of lightness on value between Lightness on from
             table and Lux Level Output.
            */
            state_Value = Light_LC_MaxLightnessValue(LightnessValue,luxLightnessvalue);
                     
            transitionTime = Light_LC_GetPropertyID_value(LIGHT_CONTROL_TIME_FADE_ID);
            wait_time = Get_TimeToWait(LIGHT_CONTROL_TIME_FADE_ID);
            Publish_flag = 1;
            Timer_value = resetTime;
            Lc_Event = LC_TIMER_OFF;
            TRACE_I(TF_LIGHT_LC,"RUN STATE --> FADE STATE , wait time = %ld \r\n\n",wait_time);
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
          
          opcode = LIGHT_LIGHTNESS_LINEAR_SET_UNACK;
          LightnessValue = Light_LC_GetPropertyID_value(LIGHT_CONTROL_LIGHTNESS_STANDBY_ID);
          /* getting the Lightness value after complete calculation from the ambient 
           Lux Level and Lux Level output  from state machine.*/
          luxLightnessvalue = Light_LC_LuxLevelOutputValue(LIGHT_CONTROL_LUX_LEVEL_STANDBY_ID);
          /* Select the maximum value of lightness on value between Lightness on from
           table and Lux Level Output.
          */
          state_Value = Light_LC_MaxLightnessValue(LightnessValue,luxLightnessvalue);
                 
          transitionTime = Light_LC_GetPropertyID_value(LIGHT_CONTROL_TIME_FADE_STANDBY_MANUAL_ID);
          wait_time = Get_TimeToWait(LIGHT_CONTROL_TIME_FADE_STANDBY_MANUAL_ID);
          Publish_flag = 1;
          TRACE_I(TF_LIGHT_LC,"FADE STATE --> STANDBY MANUAL , LIGHT OFF EVENT \r\n\n");
        }
        else if(Lc_Event == LC_OCCUPANCY_ON)
        {
          /* 
             when controller is in fade state and it gets the Occupancy on event,
             then the next state is fade on state.
          */ 
          LightLC_States.Lc_States = LC_FADE_ON;
          opcode = LIGHT_LIGHTNESS_LINEAR_SET_UNACK;
          
          LightnessValue = Light_LC_GetPropertyID_value(LIGHT_CONTROL_LIGHTNESS_ON_ID);
          /* getting the Lightness value after complete calculation from the ambient 
           Lux Level and Lux Level output  from state machine.*/
          luxLightnessvalue = Light_LC_LuxLevelOutputValue(LIGHT_CONTROL_LUX_LEVEL_ON_ID);
          /* Select the maximum value of lightness on value between Lightness on from
           table and Lux Level Output.
          */
          state_Value = Light_LC_MaxLightnessValue(LightnessValue,luxLightnessvalue);
          
          transitionTime = Light_LC_GetPropertyID_value(LIGHT_CONTROL_TIME_FADE_ON_ID);
          wait_time = Get_TimeToWait(LIGHT_CONTROL_TIME_FADE_ON_ID);
          Lc_Event = LC_TIMER_OFF;
          Timer_value = resetTime;
          Publish_flag = 1;
          TRACE_I(TF_LIGHT_LC,"FADE STATE --> FADE ON STATE , OCCUPANCY ON EVENT \r\n\n");
        }
        else if(Lc_Event == LC_LIGHT_ON)
        {
          /* 
             when controller is in fade state and it gets the Light on event,
             then the next state is fade on state.
          */ 
          LightLC_States.Lc_States = LC_FADE_ON;
          
          opcode = LIGHT_LIGHTNESS_LINEAR_SET_UNACK;
          LightnessValue = Light_LC_GetPropertyID_value(LIGHT_CONTROL_LIGHTNESS_ON_ID);
          /* getting the Lightness value after complete calculation from the ambient 
           Lux Level and Lux Level output  from state machine.*/
          luxLightnessvalue = Light_LC_LuxLevelOutputValue(LIGHT_CONTROL_LUX_LEVEL_ON_ID);
          /* Select the maximum value of lightness on value between Lightness on from
           table and Lux Level Output.
          */
          state_Value = Light_LC_MaxLightnessValue(LightnessValue,luxLightnessvalue);
          
          transitionTime = Light_LC_GetPropertyID_value(LIGHT_CONTROL_TIME_FADE_ON_ID);
          wait_time = Get_TimeToWait(LIGHT_CONTROL_TIME_FADE_ON_ID);
          Timer_value = resetTime;
          Publish_flag = 1;
          Lc_Event = LC_TIMER_OFF;
          TRACE_I(TF_LIGHT_LC,"FADE STATE --> FADE ON STATE , LIGHT ON EVENT \r\n\n");
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
            wait_time = Light_LC_GetPropertyID_value(LIGHT_CONTROL_TIME_PROLONG_ID);                 
            Timer_value = resetTime;
            Lc_Event = LC_TIMER_OFF;
            TRACE_I(TF_LIGHT_LC,"FADE STATE --> PROLONG STATE , wait time = %ld \r\n\n",wait_time);
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
          
          opcode = LIGHT_LIGHTNESS_LINEAR_SET_UNACK;
          LightnessValue = Light_LC_GetPropertyID_value(LIGHT_CONTROL_LIGHTNESS_STANDBY_ID);
          /* getting the Lightness value after complete calculation from the ambient 
           Lux Level and Lux Level output  from state machine.*/
          luxLightnessvalue = Light_LC_LuxLevelOutputValue(LIGHT_CONTROL_LUX_LEVEL_STANDBY_ID);
          /* Select the maximum value of lightness on value between Lightness on from
           table and Lux Level Output.
          */
          state_Value = Light_LC_MaxLightnessValue(LightnessValue,luxLightnessvalue);
          
          transitionTime = Light_LC_GetPropertyID_value(LIGHT_CONTROL_TIME_FADE_STANDBY_MANUAL_ID);
          wait_time = Get_TimeToWait(LIGHT_CONTROL_TIME_FADE_STANDBY_MANUAL_ID);
          Publish_flag = 1;
          TRACE_I(TF_LIGHT_LC,"PROLONG STATE --> STANDBY MANUAL , LIGHT OFF EVENT \r\n\n");
        }
        else if(Lc_Event == LC_OCCUPANCY_ON)
        {
          /* 
             when controller is in prolong state and it gets the Occupancy on event,
             then the next state is fade on state.
          */
            LightLC_States.Lc_States = LC_FADE_ON; 

            opcode = LIGHT_LIGHTNESS_LINEAR_SET_UNACK;
            LightnessValue = Light_LC_GetPropertyID_value(LIGHT_CONTROL_LIGHTNESS_ON_ID);
            /* getting the Lightness value after complete calculation from the ambient 
            Lux Level and Lux Level output  from state machine.*/
            luxLightnessvalue = Light_LC_LuxLevelOutputValue(LIGHT_CONTROL_LUX_LEVEL_ON_ID);
            /* Select the maximum value of lightness on value between Lightness on from
              table and Lux Level Output.
           */
            state_Value = Light_LC_MaxLightnessValue(LightnessValue,luxLightnessvalue);
                     
            transitionTime = Light_LC_GetPropertyID_value(LIGHT_CONTROL_TIME_FADE_ON_ID);
            wait_time = Get_TimeToWait(LIGHT_CONTROL_TIME_FADE_ON_ID);
            Timer_value = resetTime;
            Publish_flag = 1;
            TRACE_I(TF_LIGHT_LC,"STANDBY AUTO --> FADE ON STATE , OCCUPANCY ON EVENT \r\n\n");
        }
        else if(Lc_Event == LC_LIGHT_ON)
        {
          /* 
             when controller is in prolong state and it gets the Light on event,
             then the next state is fade on state.
          */
          LightLC_States.Lc_States = LC_FADE_ON; 
          
          opcode = LIGHT_LIGHTNESS_LINEAR_SET_UNACK;
          LightnessValue = Light_LC_GetPropertyID_value(LIGHT_CONTROL_LIGHTNESS_ON_ID);
          /* getting the Lightness value after complete calculation from the ambient 
          Lux Level and Lux Level output  from state machine.*/
          luxLightnessvalue = Light_LC_LuxLevelOutputValue(LIGHT_CONTROL_LUX_LEVEL_ON_ID);
          /* Select the maximum value of lightness on value between Lightness on from
            table and Lux Level Output.
         */
          state_Value = Light_LC_MaxLightnessValue(LightnessValue,luxLightnessvalue);
                  
          transitionTime = Light_LC_GetPropertyID_value(LIGHT_CONTROL_TIME_FADE_ON_ID);
          wait_time = Get_TimeToWait(LIGHT_CONTROL_TIME_FADE_ON_ID);
          Timer_value = resetTime; 
          Publish_flag = 1;
          TRACE_I(TF_LIGHT_LC,"PROLONG STATE --> FADE ON STATE , LIGHT ON EVENT \r\n\n");
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
             
             opcode = LIGHT_LIGHTNESS_LINEAR_SET_UNACK;
             LightnessValue = Light_LC_GetPropertyID_value(LIGHT_CONTROL_LIGHTNESS_STANDBY_ID);
             /* getting the Lightness value after complete calculation from the ambient 
             Lux Level and Lux Level output  from state machine.*/
             luxLightnessvalue = Light_LC_LuxLevelOutputValue(LIGHT_CONTROL_LUX_LEVEL_STANDBY_ID);
             /* Select the maximum value of lightness on value between Lightness on from
                table and Lux Level Output.
             */
             state_Value = Light_LC_MaxLightnessValue(LightnessValue,luxLightnessvalue);
                   
             transitionTime = Light_LC_GetPropertyID_value(LIGHT_CONTROL_TIME_FADE_STANDBY_AUTO_ID);
             wait_time = Get_TimeToWait(LIGHT_CONTROL_TIME_FADE_STANDBY_AUTO_ID);
             Publish_flag = 1;
             Timer_value = resetTime;    
             TRACE_I(TF_LIGHT_LC,"PROLONG STATE --> STANDBY AUTO STATE , wait time = %ld \r\n\n",wait_time);
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
          
          opcode = LIGHT_LIGHTNESS_LINEAR_SET_UNACK;
          LightnessValue = Light_LC_GetPropertyID_value(LIGHT_CONTROL_LIGHTNESS_STANDBY_ID);
          /* getting the Lightness value after complete calculation from the ambient 
          Lux Level and Lux Level output  from state machine.*/
          luxLightnessvalue = Light_LC_LuxLevelOutputValue(LIGHT_CONTROL_LUX_LEVEL_STANDBY_ID);
          /* Select the maximum value of lightness on value between Lightness on from
             table and Lux Level Output.
          */
          state_Value = Light_LC_MaxLightnessValue(LightnessValue,luxLightnessvalue);
         
          transitionTime = Light_LC_GetPropertyID_value(LIGHT_CONTROL_TIME_FADE_STANDBY_MANUAL_ID);
          wait_time = Get_TimeToWait(LIGHT_CONTROL_TIME_FADE_STANDBY_MANUAL_ID);
          Publish_flag = 1;
          TRACE_I(TF_LIGHT_LC,"STANDBY AUTO STATE --> STANDBY MANUAL , LIGHT OFF EVENT \r\n\n");
        }
        else if(Lc_Event == LC_OCCUPANCY_ON)
        {
          /* 
             when controller is in standby auto state and it gets the occupancy on event,
             then the next state is fade on state.
          */
            LightLC_States.Lc_States = LC_FADE_ON;
            
            opcode = LIGHT_LIGHTNESS_LINEAR_SET_UNACK;
            LightnessValue = Light_LC_GetPropertyID_value(LIGHT_CONTROL_LIGHTNESS_ON_ID);
            /* getting the Lightness value after complete calculation from the ambient 
            Lux Level and Lux Level output  from state machine.*/
            luxLightnessvalue = Light_LC_LuxLevelOutputValue(LIGHT_CONTROL_LUX_LEVEL_ON_ID);
            /* Select the maximum value of lightness on value between Lightness on from
                table and Lux Level Output.
            */
            state_Value = Light_LC_MaxLightnessValue(LightnessValue,luxLightnessvalue);
            
            transitionTime = Light_LC_GetPropertyID_value(LIGHT_CONTROL_TIME_FADE_ON_ID);
            wait_time = Get_TimeToWait(LIGHT_CONTROL_TIME_FADE_ON_ID);
            Timer_value = resetTime; 
            Publish_flag = 1;
            TRACE_I(TF_LIGHT_LC,"STANDBY AUTO --> FADE ON STATE , OCCUPANCY ON EVENT \r\n\n");
        }
        else if(Lc_Event == LC_LIGHT_ON)
        {
          /* 
             when controller is in standby auto state and it gets the Light on event,
             then the next state is fade on state.
          */
          LightLC_States.Lc_States = LC_FADE_ON;  
          
          opcode = LIGHT_LIGHTNESS_LINEAR_SET_UNACK;
          LightnessValue = Light_LC_GetPropertyID_value(LIGHT_CONTROL_LIGHTNESS_ON_ID);
          /* getting the Lightness value after complete calculation from the ambient 
          Lux Level and Lux Level output  from state machine.*/
          luxLightnessvalue = Light_LC_LuxLevelOutputValue(LIGHT_CONTROL_LUX_LEVEL_ON_ID);
          /* Select the maximum value of lightness on value between Lightness on from
              table and Lux Level Output.
          */
          state_Value = Light_LC_MaxLightnessValue(LightnessValue,luxLightnessvalue);
          
          transitionTime = Light_LC_GetPropertyID_value(LIGHT_CONTROL_TIME_FADE_ON_ID);
          wait_time = Get_TimeToWait(LIGHT_CONTROL_TIME_FADE_ON_ID);
          Timer_value = resetTime; 
          Publish_flag = 1;
          TRACE_I(TF_LIGHT_LC,"STANDBY AUTO --> FADE ON STATE , LIGHT ON EVENT \r\n\n");
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
             TRACE_I(TF_LIGHT_LC,"STANDBY AUTO --> STANDBY STATE , wait time = %ld \r\n\n",wait_time);
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
          opcode = LIGHT_LIGHTNESS_LINEAR_SET_UNACK;
          LightnessValue = Light_LC_GetPropertyID_value(LIGHT_CONTROL_LIGHTNESS_ON_ID);
          /* getting the Lightness value after complete calculation from the ambient 
          Lux Level and Lux Level output  from state machine.*/
          luxLightnessvalue = Light_LC_LuxLevelOutputValue(LIGHT_CONTROL_LUX_LEVEL_ON_ID);
          /* Select the maximum value of lightness on value between Lightness on from
              table and Lux Level Output.
          */
          state_Value = Light_LC_MaxLightnessValue(LightnessValue,luxLightnessvalue);
             
          transitionTime = Light_LC_GetPropertyID_value(LIGHT_CONTROL_TIME_FADE_ON_ID);
          wait_time = Get_TimeToWait(LIGHT_CONTROL_TIME_FADE_ON_ID);
          Timer_value = resetTime; 
          Publish_flag = 1;
          TRACE_I(TF_LIGHT_LC,"STANDBY MANUAL STATE --> FADE ON STATE , LIGHT ON EVENT \r\n\n");
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
            TRACE_I(TF_LIGHT_LC,"STANDBY MANUAL STATE --> STANDBY STATE , wait time = %ld \r\n\n",wait_time);
          }
        }
        break;
      }
    default:
      {
        break;
      }
  }
  if(Publish_flag == 1) 
  { 
      if(Light_LC_Param.Transition_Time == 0x00)
      {
        
      }
    
      data_Buff[0] = state_Value;
      data_Buff[1] = state_Value >> 8;
      data_Buff[2] = TidSend;
      data_Buff[3] = transitionTime;
      TidSend++;
      if(TidSend >= MAX_TID_VALUE)
     {
       TidSend = 0;
     }
     srcAdd = BLEMesh_GetAddress();
      
     result = BLEMesh_SetRemotePublication(LIGHT_MODEL_SERVER_LC_MODEL_ID, srcAdd ,
                                opcode , 
                                data_Buff, length,
                                MOBLE_FALSE, MOBLE_FALSE);
    
   if(result)
   {
     TRACE_I(TF_LIGHT_LC,"Publication Error \r\n");
   }
      Publish_flag = 0;
   }
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
  MOBLEUINT32 getValue32;
  MOBLEUINT8 getValue8;
  MOBLEUINT32 total_time;
  getValue32 = Light_LC_GetPropertyID_value(Proprety_ID);
  
  getValue8 = (MOBLEUINT8)(getValue32 & 0x000000FF);
  total_time = Light_LC_GetStepValue((MOBLEUINT8)getValue8);
  
  TRACE_I(TF_LIGHT_LC,"TOTAL WAIT TIME %ld \r\n",total_time);
    
  return total_time;
}

/**
* @brief  Light_control_Process: Function to execute the transition state machine for
*         particular Light LC model and state machine Light LC application
* @param  void
* @retval void
*/ 
void Light_control_Process(void)
{
  
  if(Light_LC_Param.LC_mode == LC_MODE_ENABLE)
  {   
    Light_LC_Fsm();   
  }   
  else
    {
    /* Light LC on off set to off */
    Light_LC_Param.Light_OnOff = 0x0;
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
  
  availableLuxValue = Light_LC_GetPropertyID_value(property_ID);
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

/*
* @brief Light_LC_Client_Mode_Status: Function called when status of the model 
  received on the client.
* @param pLightLc_status: pointer to the parameters received for message
* @param pLength: Length of the parameters received for message
* return MOBLE_RESULT_SUCCESS.
*/
MOBLE_RESULT Light_LC_Client_Mode_Status(MOBLEUINT8 const *pLightLc_status, MOBLEUINT32 pLength)
{
  MOBLEUINT32 i;
  
  TRACE_M(TF_LIGHT,"Light_LC_Client_Mode_Status callback received \r\n");
  for(i = 0; i < pLength; i++)
    TRACE_M(TF_SERIAL_CTRL,"Light_LC_Client_Mode_Status: %d\r\n",
            pLightLc_status[i]);
  return MOBLE_RESULT_SUCCESS;
}

/*
* @brief Light_LC_Client_OM_Status: Function called when status of the model 
  received on the client.
* @param pLightLc_status: pointer to the parameters received for message
* @param pLength: Length of the parameters received for message
* return MOBLE_RESULT_SUCCESS.
*/
MOBLE_RESULT Light_LC_Client_OM_Status(MOBLEUINT8 const *pLightLc_status, MOBLEUINT32 pLength)
{
  MOBLEUINT32 i;
  
  TRACE_M(TF_LIGHT,"Light_LC_Client_OM_Status callback received \r\n");
  for(i = 0; i < pLength; i++)
    TRACE_M(TF_SERIAL_CTRL,"Light_LC_Client_OM_Status: %d\r\n",
            pLightLc_status[i]);
  return MOBLE_RESULT_SUCCESS;
}

/*
* @brief Light_LC_Client_ON_OFF_Status: Function called when status of the model 
  received on the client.
* @param pLightLc_status: pointer to the parameters received for message
* @param pLength: Length of the parameters received for message
* return MOBLE_RESULT_SUCCESS.
*/
MOBLE_RESULT Light_LC_Client_ON_OFF_Status(MOBLEUINT8 const *pLightLc_status, MOBLEUINT32 pLength)
{
  MOBLEUINT32 i;
  
  TRACE_M(TF_LIGHT,"Light_LC_Client_ON_OFF_Status callback received \r\n");
  for(i = 0; i < pLength; i++)
    TRACE_M(TF_SERIAL_CTRL,"Light_LC_Client_ON_OFF_Status: %d\r\n",
            pLightLc_status[i]);
  return MOBLE_RESULT_SUCCESS;
}

/*
* @brief Light_LC_Client_Property_Status: Function called when status of the model 
  received on the client.
* @param pLightLc_status: pointer to the parameters received for message
* @param pLength: Length of the parameters received for message
* return MOBLE_RESULT_SUCCESS.
*/
MOBLE_RESULT Light_LC_Client_Property_Status(MOBLEUINT8 const *pLightLc_status, MOBLEUINT32 pLength)
{
  MOBLEUINT32 i;
  
  TRACE_M(TF_LIGHT,"Light_LC_Client_Property_Status callback received \r\n");
  for(i = 0; i < pLength; i++)
    TRACE_M(TF_SERIAL_CTRL,"Light_LC_Client_Property_Status: %d\r\n",
            pLightLc_status[i]);
  return MOBLE_RESULT_SUCCESS;
}

/**
* @}
*/

/**
* @}
*/

/******************* (C) COPYRIGHT 2017 STMicroelectronics *****END OF FILE****/

