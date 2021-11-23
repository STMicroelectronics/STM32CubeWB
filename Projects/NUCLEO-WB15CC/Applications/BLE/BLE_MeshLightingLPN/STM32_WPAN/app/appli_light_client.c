/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    appli_light_client.c
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

/* Includes ------------------------------------------------------------------*/
#include "hal_common.h"
#include "types.h"
#include "appli_generic.h"
#include "appli_light.h"
#include "appli_light_lc.h"
#include "common.h"
#include "mesh_cfg_usr.h"
#include "appli_nvm.h"
#include "appli_mesh.h"
#include "generic_client.h"
#include "appli_light_client.h"
#include "light_lc.h"
#include "light_client.h"


/** @addtogroup ST_BLE_Mesh
*  @{
*/

/** @addtogroup Application_Mesh_Models
*  @{
*/

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

extern MOBLEUINT8 Tid_Client;
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
* @brief  Appli_LightClient_API: This function is a Wrapper to call Light Client API depending on opcode received in input
* @param  elementIndex: Index of the element
* @param  msg_opcode: opcode of the desired API
* @param  msg_params: buffer containing the desired API parameters
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT Appli_LightClient_API(MOBLEUINT8 elementIndex, MOBLEUINT16 msg_opcode, MOBLEUINT8 *msg_params) 
{ 
  MOBLE_RESULT result = MOBLE_RESULT_FAIL;
    
  switch(msg_opcode){
/******************************************************************************/
#ifdef ENABLE_LIGHT_MODEL_CLIENT_LIGHTNESS  
/******************************************************************************/
  case LIGHT_LIGHTNESS_GET:
    {
      result = LightClient_Lightness_Get(elementIndex);
      break;
    }
  case LIGHT_LIGHTNESS_SET:
    {
      result = LightClient_Lightness_Set_Ack(elementIndex,
                                             msg_params, 
                                             (sizeof(Light_LightnessParam_t)-4));       //Remove the size of Linear Lightness params, Transition Time and Delay
      break;
    }
  case LIGHT_LIGHTNESS_SET_UNACK:
    {  
      result = LightClient_Lightness_Set_Unack(elementIndex,
                                               msg_params, 
                                               (sizeof(Light_LightnessParam_t)-4));     //Remove the size of Linear Lightness params, Transition Time and Delay
      break;
    }
    
  case LIGHT_LIGHTNESS_LINEAR_GET:
    {
      result = LightClient_Lightness_Linear_Get(elementIndex);
      break;
    }
  case LIGHT_LIGHTNESS_LINEAR_SET:
    {
      result = LightClient_Lightness_Linear_Set_Ack(elementIndex,
                                                    msg_params, 
                                                    (sizeof(Light_LightnessParam_t)-4));        //Remove the size of  Lightness params, Transition Time and Delay
      break;
    }
  case LIGHT_LIGHTNESS_LINEAR_SET_UNACK:
    {     
      result = LightClient_Lightness_Linear_Set_Unack(elementIndex,
                                                      msg_params, 
                                                      (sizeof(Light_LightnessParam_t)-4));      //Remove the size of Lightness params, Transition Time and Delay
      break;
    }
    
  case LIGHT_LIGHTNESS_DEFAULT_GET:
    {
      result = LightClient_Lightness_Default_Get(elementIndex);
      break;
    }
  case LIGHT_LIGHTNESS_DEFAULT_SET:
    {
      result = LightClient_Lightness_Default_Set_Ack(elementIndex,
                                                     msg_params, 
                                                     sizeof(Light_LightnessDefaultParam_t));
      break;
    }
  case LIGHT_LIGHTNESS_DEFAULT_SET_UNACK:
    {     
      result = LightClient_Lightness_Default_Set_Unack(elementIndex,
                                                       msg_params, 
                                                       sizeof(Light_LightnessDefaultParam_t));
      break;
    }
    
    
  case LIGHT_LIGHTNESS_RANGE_GET:
    {
      result = LightClient_Lightness_Range_Get(elementIndex);
      break;
    }
  case LIGHT_LIGHTNESS_RANGE_SET:
    {
      result = LightClient_Lightness_Range_Set_Ack(elementIndex,
                                                     msg_params, 
                                                     sizeof(Light_LightnessRangeParam_t)-1);    //Remove the size of StatusCode
      break;
    }
  case LIGHT_LIGHTNESS_RANGE_SET_UNACK:
    {     
      result = LightClient_Lightness_Range_Set_Unack(elementIndex,
                                                       msg_params, 
                                                       sizeof(Light_LightnessRangeParam_t)-1);  //Remove the size of StatusCode
      break;
    }


/******************************************************************************/
#endif /* #ifdef ENABLE_LIGHT_MODEL_CLIENT_LIGHTNESS */
/******************************************************************************/
    
/******************************************************************************/
#ifdef ENABLE_LIGHT_MODEL_CLIENT_CTL  
/******************************************************************************/
  case LIGHT_CTL_GET:
    {
      result = LightClient_Ctl_Get(elementIndex);
      break;
    }
  case LIGHT_CTL_SET:
    {
      result = LightClient_Ctl_Set_Ack(elementIndex,
                                       msg_params, 
                                       (sizeof(Light_CtlParam_t)-4) ); //Remove the size of Last_Lightness and Last_Temperature
      break;
    }
  case LIGHT_CTL_SET_UNACK:
    {  
      result = LightClient_Ctl_Set_Unack(elementIndex,
                                         msg_params, 
                                         (sizeof(Light_CtlParam_t)-4) ); //Remove the size of Last_Lightness and Last_Temperature
      break;
    }
    
  case LIGHT_CTL_TEMPERATURE_GET:
    {
      result = LightClient_Ctl_Temperature_Get(elementIndex);
      break;
    }
  case LIGHT_CTL_TEMPERATURE_SET:
    {
      result = LightClient_Ctl_Temperature_Set_Ack(elementIndex,
                                                   msg_params, 
                                                   sizeof(Light_CtlTemperatureParam_t)-2);      //Remove the size of Transition Time and Delay
      break;
    }
  case LIGHT_CTL_TEMPERATURE_SET_UNACK:
    {  
      result = LightClient_Ctl_Temperature_Set_Unack(elementIndex,
                                                     msg_params, 
                                                     sizeof(Light_CtlTemperatureParam_t)-2);    //Remove the size of Transition Time and Delay
      break;
    }
                                                     
    case LIGHT_CTL_TEMPERATURE_RANGE_GET:
      {
        result = LightClient_Ctl_Temperature_Range_Get(elementIndex);
        break;
      }
    case LIGHT_CTL_TEMPERATURE_RANGE_SET:
      {
        result = LightClient_Ctl_Temperature_Range_Set_Ack(elementIndex,
                                                           msg_params, 
                                                           sizeof(Light_CtlTemperatureRangeParam_t)-1);  //Remove the size of StatusCode
        break;
      }
  case LIGHT_CTL_TEMPERATURE_RANGE_SET_UNACK:
    {  
      result = LightClient_Ctl_Temperature_Range_Set_Unack(elementIndex,
                                                           msg_params, 
                                                           sizeof(Light_CtlTemperatureRangeParam_t)-1);  //Remove the size of StatusCode
      break;
    }
      
    case LIGHT_CTL_DEFAULT_GET:
      {
        result = LightClient_Ctl_Default_Get(elementIndex);
        break;
      }
    case LIGHT_CTL_DEFAULT_SET:
      {
        result = LightClient_Ctl_Default_Set_Ack(elementIndex,
                                                 msg_params, 
                                                 sizeof(Light_CtlDefaultParam_t));
        break;
      }
  case LIGHT_CTL_DEFAULT_SET_UNACK:
    {  
      result = LightClient_Ctl_Default_Set_Unack(elementIndex,
                                                 msg_params, 
                                                 sizeof(Light_CtlDefaultParam_t));
      break;
    }
/******************************************************************************/
#endif /* #ifdef ENABLE_LIGHT_MODEL_CLIENT_CTL */
/******************************************************************************/

/******************************************************************************/
#ifdef ENABLE_LIGHT_MODEL_CLIENT_HSL  
/******************************************************************************/
  case LIGHT_HSL_GET:
    {
      result = LightClient_Hsl_Get(elementIndex);
      break;
    }
  case LIGHT_HSL_SET:
    {
      result = LightClient_Hsl_Set_Ack(elementIndex,
                                       msg_params, 
                                       sizeof(Light_HslParam_t) ); 
      break;
    }
  case LIGHT_HSL_SET_UNACK:
    {  
      result = LightClient_Hsl_Set_Unack(elementIndex,
                                         msg_params, 
                                         sizeof(Light_HslParam_t) );
      break;
    }
    
  case LIGHT_HSL_DEFAULT_GET:
    {
      result = LightClient_Hsl_Default_Get(elementIndex);
      break;
    }
  case LIGHT_HSL_DEFAULT_SET:
    {
      result = LightClient_Hsl_Default_Set_Ack(elementIndex,
                                       msg_params, 
                                       sizeof(Light_HslDefaultParam_t) ); 
      break;
    }
  case LIGHT_HSL_DEFAULT_SET_UNACK:
    {  
      result = LightClient_Hsl_Default_Set_Unack(elementIndex,
                                         msg_params, 
                                         sizeof(Light_HslDefaultParam_t) );
      break;
    }
    
  case LIGHT_HSL_RANGE_GET:
    {
      result = LightClient_Hsl_Range_Get(elementIndex);
      break;
    }
  case LIGHT_HSL_RANGE_SET:
    {
      result = LightClient_Hsl_Range_Set_Ack(elementIndex,
                                               msg_params, 
                                               sizeof(Light_HslRangeParam_t)-1);  //Remove the size of StatusCode
      break;
    }
  case LIGHT_HSL_RANGE_SET_UNACK:
    {  
      result = LightClient_Hsl_Range_Set_Unack(elementIndex,
                                                 msg_params, 
                                                 sizeof(Light_HslRangeParam_t)-1);  //Remove the size of StatusCode
      break;
    }
    
  case LIGHT_HSL_HUE_GET:
    {
      result = LightClient_Hsl_Hue_Get(elementIndex);
      break;
    }
  case LIGHT_HSL_HUE_SET:
    {
      result = LightClient_Hsl_Hue_Set_Ack(elementIndex,
                                             msg_params, 
                                             sizeof(Light_HslHueParam_t)-2);    //Remove the size of Transition Time and Delay
      break;
    }
  case LIGHT_HSL_HUE_SET_UNACK:
    {  
      result = LightClient_Hsl_Hue_Set_Unack(elementIndex,
                                               msg_params, 
                                               sizeof(Light_HslHueParam_t)-2);  //Remove the size of Transition Time and Delay
      break;
    }
    
    case LIGHT_HSL_SATURATION_GET:
      {
        result = LightClient_Hsl_Saturation_Get(elementIndex);
        break;
      }
    case LIGHT_HSL_SATURATION_SET:
      {
        result = LightClient_Hsl_Saturation_Set_Ack(elementIndex,
                                                    msg_params, 
                                                    sizeof(Light_HslSaturationParam_t)-2);  //Remove the size of Transition Time and Delay
        break;
      }
    case LIGHT_HSL_SATURATION_SET_UNACK:
      {  
        result = LightClient_Hsl_Saturation_Set_Unack(elementIndex,
                                                      msg_params, 
                                                      sizeof(Light_HslSaturationParam_t)-2);  //Remove the size of Transition Time and Delay
        break;
    }
/******************************************************************************/
#endif /* #ifdef ENABLE_LIGHT_MODEL_CLIENT_HSL */
/******************************************************************************/
/******************************************************************************/
#ifdef ENABLE_LIGHT_MODEL_CLIENT_LC  
/******************************************************************************/   
  case LIGHT_LC_MODE_GET:
    {
      result = LightClient_LC_Mode_Get(elementIndex);
      break;
    }
  case LIGHT_LC_MODE_SET:
    {
      result = LightClient_LC_Mode_Set_Ack(elementIndex,
                                           msg_params, 
                                           sizeof(Light_LC_ModeParam_t));
      break;
    }
  case LIGHT_LC_MODE_SET_UNACK:
    {  
      result = LightClient_LC_Mode_Set_Unack(elementIndex,
                                             msg_params, 
                                             sizeof(Light_LC_ModeParam_t));
      break;
    }
  case LIGHT_LC_OM_GET:
    {
      result = LightClient_LC_OM_Get(elementIndex);
      break;
    }
  case LIGHT_LC_OM_SET:
    {
      result = LightClient_LC_OM_Set_Ack(elementIndex,
                                           msg_params, 
                                           sizeof(Light_LC_OccupancyModeParam_t));
      break;
    }
  case LIGHT_LC_OM_SET_UNACK:
    {  
      result = LightClient_LC_OM_Set_Unack(elementIndex,
                                             msg_params, 
                                             sizeof(Light_LC_OccupancyModeParam_t));
      break;
    }
  case LIGHT_LC_ON_OFF_GET:
    {
      result = LightClient_LC_OnOff_Get(elementIndex);
      break;
    }
  case LIGHT_LC_ON_OFF_SET:
    {
      result = LightClient_LC_OnOff_Set_Ack(elementIndex,
                                           msg_params, 
                                           sizeof(Light_LC_OnOffParam_t));
      break;
    }
  case LIGHT_LC_ON_OFF_SET_UNACK:
    {  
      result = LightClient_LC_OnOff_Set_Unack(elementIndex,
                                             msg_params, 
                                             sizeof(Light_LC_OnOffParam_t));
      break;
    }
    
    case LIGHT_LC_PROPERTY_GET:
      {
        result = LightClient_LC_Property_Get(elementIndex, msg_params);
        break;
      }
    case LIGHT_LC_PROPERTY_SET:
      {
        
        result = LightClient_LC_Property_Set_Ack(elementIndex,
                                              msg_params, 
                                              5);
        break;
      }
  case LIGHT_LC_PROPERTY_SET_UNACK:
    {  
      
      result = LightClient_LC_Property_Set_Unack(elementIndex,
                                              msg_params, 
                                              5);
      break;
    }
/******************************************************************************/
#endif /* #ifdef ENABLE_LIGHT_MODEL_CLIENT_LC */
/******************************************************************************/
  default:
    {
      TRACE_M(TF_LIGHT, "OpCode value invalid %d \r\n", msg_opcode);
      result = MOBLE_RESULT_FAIL;
    }
  }
  return result;
}

/**
* @brief  Appli_Light_Lightness_Status: This function is callback for Application
*           when Light Lightness status message is received
* @param  pLightness_status: Pointer to the parameters received for message
* @param  pLength: length of data 
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT Appli_Light_Lightness_Status(MOBLEUINT8 const *pLightness_status, MOBLEUINT32 pLength, MOBLEUINT16 dstPeer, 
                                              MOBLEUINT8 elementIndex)
{
#if ( CFG_DEBUG_TRACE != 0 )
  MOBLEUINT32 i;
  MOBLEUINT16 value = 0;
#endif
  
  TRACE_M(TF_LIGHT,"Light_Lightness_Status callback received for element %d \r\n", elementIndex);
    TRACE_M(TF_SERIAL_CTRL,"#824E!\r\n");
  
#if ( CFG_DEBUG_TRACE != 0 )
  for(i = 0; i < pLength; i++)
  {
    if((i == 0) || (i == 2))
    {
      value = pLightness_status[i];
    }
    else if(i == 1)
    {
      TRACE_M(TF_LIGHT,"Present Lightness: 0x%x (%d)\r\n",
              value | pLightness_status[i] << 8,
              value | pLightness_status[i] << 8);
    }
    else if(i == 3)
    {
      TRACE_M(TF_LIGHT,"Target Lightness: 0x%x (%d)\r\n",
              value | pLightness_status[i] << 8,
              value | pLightness_status[i] << 8);
    }
    else if(i == 4)
    {
      TRACE_M(TF_LIGHT,"Remaining Time: 0x%x (%d)\r\n",
              pLightness_status[i],
              pLightness_status[i]);
    }
  }
#endif
  
  return MOBLE_RESULT_SUCCESS;
}

/**
* @brief  Appli_Light_Lightness_Linear_Status: This function is callback for Application
*         when Light Lightness Linear status message is received
* @param  pLightnessLinear_status: Pointer to the parameters received for message
* @param  dstPeer: destination send by peer for this node. It can be a
*                     unicast or group address 
* @param  elementIndex: index of the element received from peer for this node which
*                     is elementNumber-1
* @param  pLength: length of data
* @retval MOBLE_RESULT
*/
MOBLE_RESULT Appli_Light_Lightness_Linear_Status(MOBLEUINT8 const *pLightnessLinear_status, MOBLEUINT32 pLength, MOBLEUINT16 dstPeer, 
                                              MOBLEUINT8 elementIndex)
{
#if ( CFG_DEBUG_TRACE != 0 )
  MOBLEUINT32 i;
  MOBLEUINT16 value = 0;
#endif
  
  TRACE_M(TF_LIGHT,"Light_Lightness_Linear_Status callback received for element %d \r\n", elementIndex);
  TRACE_M(TF_SERIAL_CTRL,"#8252!\r\n");
  
#if ( CFG_DEBUG_TRACE != 0 )
  for(i = 0; i < pLength; i++)
  {
    if((i == 0) || (i == 2))
    {
      value = pLightnessLinear_status[i];
    }
    else if(i == 1)
    {
      TRACE_M(TF_LIGHT,"Present Lightness: 0x%x (%d)\r\n",
              value | pLightnessLinear_status[i] << 8,
              value | pLightnessLinear_status[i] << 8);
    }
    else if(i == 3)
    {
      TRACE_M(TF_LIGHT,"Target Lightness: 0x%x (%d)\r\n",
              value | pLightnessLinear_status[i] << 8,
              value | pLightnessLinear_status[i] << 8);
    }
    else if(i == 4)
    {
      TRACE_M(TF_LIGHT,"Remaining Time: 0x%x (%d)\r\n",
              pLightnessLinear_status[i],
              pLightnessLinear_status[i]);
    }
  }
#endif
  
  return MOBLE_RESULT_SUCCESS;
}

/**
* @brief  Appli_Light_Lightness_Last_Status: This function is callback for Application
*         when Light Lightness Last status message is received
* @param  pLightnessDefault_status: Pointer to the parameters received for message
* @param  pLength: length of data
* @param  dstPeer: destination send by peer for this node. It can be a
*                     unicast or group address 
* @param  elementIndex: index of the element received from peer for this node which
*                     is elementNumber-1
* @retval MOBLE_RESULT
*/
MOBLE_RESULT Appli_Light_Lightness_Last_Status(MOBLEUINT8 const *pLightnessLast_status, 
                                               MOBLEUINT32 pLength, 
                                               MOBLEUINT16 dstPeer, 
                                               MOBLEUINT8 elementIndex)
{
#if ( CFG_DEBUG_TRACE != 0 )
  MOBLEUINT32 i;
  MOBLEUINT16 value;
#endif
 
  TRACE_M(TF_LIGHT,"Light_Lightness_Last_Status callback received for element %d \r\n", elementIndex);
  TRACE_M(TF_SERIAL_CTRL,"#8256!\r\n");
  
#if ( CFG_DEBUG_TRACE != 0 )
  for(i = 0; i < pLength; i++)
  {
    if(i == 0)
    {
      value = pLightnessLast_status[i];
    }
    else if(i == 1)
    {
      TRACE_M(TF_LIGHT,"Lightness Last: 0x%x (%d)\r\n",
              value | pLightnessLast_status[i] << 8,
              value | pLightnessLast_status[i] << 8);
    }
  }
#endif
  
  return MOBLE_RESULT_SUCCESS;
}

/**
* @brief  Appli_Light_Lightness_Default_Status: This function is callback for Application
*         when Light Lightness Default status message is received
* @param  pLightnessDefault_status: Pointer to the parameters received for message
* @param  pLength: length of data
* @param  dstPeer: destination send by peer for this node. It can be a
*                     unicast or group address 
* @param  elementIndex: index of the element received from peer for this node which
*                     is elementNumber-1
* @retval MOBLE_RESULT
*/
MOBLE_RESULT Appli_Light_Lightness_Default_Status(MOBLEUINT8 const *pLightnessDefault_status, MOBLEUINT32 pLength
                                                  , MOBLEUINT16 dstPeer, 
                                              MOBLEUINT8 elementIndex)
{
#if ( CFG_DEBUG_TRACE != 0 )
  MOBLEUINT32 i;
  MOBLEUINT16 value;
#endif
  
  TRACE_M(TF_LIGHT,"Light_Lightness_Default_Status callback received for element %d \r\n", elementIndex);
  TRACE_M(TF_SERIAL_CTRL,"#8256!\r\n");
  
#if ( CFG_DEBUG_TRACE != 0 )
  for(i = 0; i < pLength; i++)
  {
    if(i == 0)
    {
      value = pLightnessDefault_status[i];
    }
    else if(i == 1)
    {
      TRACE_M(TF_LIGHT,"Lightness Default: 0x%x (%d)\r\n",
              value | pLightnessDefault_status[i] << 8,
              value | pLightnessDefault_status[i] << 8);
    }
  }
#endif
  
  return MOBLE_RESULT_SUCCESS;
}

/**
* @brief  Appli_Light_Lightness_Range_Status: This function is callback for Application
*         when Light Lightness range ststus message is received
* @param  pLightnessRange_status: Pointer to the parameters received for message
* @param  pLength: length of data 
* @param  dstPeer: destination send by peer for this node. It can be a
*                     unicast or group address 
* @param  elementIndex: index of the element received from peer for this node which
*                     is elementNumber-1
* @retval MOBLE_RESULT
*/
MOBLE_RESULT Appli_Light_Lightness_Range_Status(MOBLEUINT8 const *pLightnessRange_status, MOBLEUINT32 pLength
                                                , MOBLEUINT16 dstPeer, 
                                              MOBLEUINT8 elementIndex)
{
#if ( CFG_DEBUG_TRACE != 0 )
  MOBLEUINT32 i;
  MOBLEUINT16 value;
#endif
  
  TRACE_M(TF_LIGHT,"Light_Lightness_Range_Status callback received for element %d \r\n", elementIndex);
  TRACE_M(TF_SERIAL_CTRL,"#8258!\r\n");
  
#if ( CFG_DEBUG_TRACE != 0 )
  for(i = 0; i < pLength; i++)
  {
    if(i == 0)
    {
      TRACE_M(TF_LIGHT,"Status Code: 0x%x (%d)\r\n",
              pLightnessRange_status[i],
              pLightnessRange_status[i]);
    }
    else if((i == 1) || (i == 3))
    {
      value = pLightnessRange_status[i];
    }
    else if(i == 2)
    {
      TRACE_M(TF_LIGHT,"Range Min: 0x%x (%d)\r\n",
              value | pLightnessRange_status[i] << 8,
              value | pLightnessRange_status[i] << 8);
    }
    else if(i == 4)
    {
      TRACE_M(TF_LIGHT,"Range Max: 0x%x (%d)\r\n",
              value | pLightnessRange_status[i] << 8,
              value | pLightnessRange_status[i] << 8);
    }
  }
#endif
  
  return MOBLE_RESULT_SUCCESS;
}

/**
* @brief  Appli_Light_Ctl_Status: This function is callback for Application
*         when Light CTL status message is received
* @param  pLightCtl_status: Pointer to the parameters received for message
* @param  pLength: length of data
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT Appli_Light_Ctl_Status(MOBLEUINT8 const *pLightCtl_status, MOBLEUINT32 pLength, MOBLEUINT16 dstPeer, 
                                              MOBLEUINT8 elementIndex)
{
#if ( CFG_DEBUG_TRACE != 0 )
  MOBLEUINT32 i;
  MOBLEUINT16 value = 0;
#endif

  TRACE_M(TF_LIGHT,"Light_Ctl_Status callback received for element %d \r\n", elementIndex);
  TRACE_M(TF_SERIAL_CTRL,"#8260!\r\n");
  
#if ( CFG_DEBUG_TRACE != 0 )
  for(i = 0; i < pLength; i++)
  {
    if((i == 0) || (i == 2) || (i == 4) || (i == 6))
    {
      value = pLightCtl_status[i];
    }
    else if(i == 1)
    {
      TRACE_M(TF_LIGHT,"Present CTL Lightness: 0x%x (%d)\r\n",
              value | pLightCtl_status[i] << 8,
              value | pLightCtl_status[i] << 8);
    }
    else if(i == 3)
    {
      TRACE_M(TF_LIGHT,"Present CTL Temperature: 0x%x (%d)\r\n",
              value | pLightCtl_status[i] << 8,
              value | pLightCtl_status[i] << 8);
    }
    else if(i == 5)
    {
      TRACE_M(TF_LIGHT,"Target CTL Lightness: 0x%x (%d)\r\n",
              value | pLightCtl_status[i] << 8,
              value | pLightCtl_status[i] << 8);
    }
    else if(i == 7)
    {
      TRACE_M(TF_LIGHT,"Target CTL Temperature: 0x%x (%d)\r\n",
              value | pLightCtl_status[i] << 8,
              value | pLightCtl_status[i] << 8);
    }
    else if(i == 8)
    {
      TRACE_M(TF_LIGHT,"Remaining Time:0x%x (%d)\r\n",
              pLightCtl_status[i],
              pLightCtl_status[i]);
    }
  }
#endif
  
  return MOBLE_RESULT_SUCCESS;
}

/**                                                  
* @brief  Appli_Light_CtlTemperature_Status: This function is callback for Application
*         when Light CTL temperature status message is received
* @param  pLightCtlTemp_status: Pointer to the parameters received for message
* @param  pLength: length of data
* @param  dstPeer: destination send by peer for this node. It can be a
*                     unicast or group address 
* @param  elementIndex: index of the element received from peer for this node which
*                     is elementNumber-1
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT Appli_Light_CtlTemperature_Status(MOBLEUINT8 const *pLightCtlTemp_status, MOBLEUINT32 pLength
                                               , MOBLEUINT16 dstPeer, 
                                              MOBLEUINT8 elementIndex)
{
#if ( CFG_DEBUG_TRACE != 0 )
  MOBLEUINT32 i;
  MOBLEUINT16 value = 0;
#endif
  
  TRACE_M(TF_LIGHT,"Light_CtlTemperature_Status callback received for element %d \r\n", elementIndex);
  TRACE_M(TF_SERIAL_CTRL,"#8266!\r\n");
  
#if ( CFG_DEBUG_TRACE != 0 )
  for(i = 0; i < pLength; i++)
  {
    if((i == 0) || (i == 2) || (i == 4) || (i == 6))
    {
      value = pLightCtlTemp_status[i];
    }
    else if(i == 1)
    {
      TRACE_M(TF_LIGHT,"Present CTL Temperature: 0x%x (%d)\r\n",
              value | pLightCtlTemp_status[i] << 8,
              value | pLightCtlTemp_status[i] << 8);
    }
    else if(i == 3)
    {
      TRACE_M(TF_LIGHT,"Present CTL Delta UV: 0x%x (%d)\r\n",
              value | pLightCtlTemp_status[i] << 8,
              value | pLightCtlTemp_status[i] << 8);
    }
    else if(i == 5)
    {
      TRACE_M(TF_LIGHT,"Target CTL Temperature: 0x%x (%d)\r\n",
              value | pLightCtlTemp_status[i] << 8,
              value | pLightCtlTemp_status[i] << 8);
    }
    else if(i == 7)
    {
      TRACE_M(TF_LIGHT,"Target CTL Delta UV: 0x%x (%d)\r\n",
              value | pLightCtlTemp_status[i] << 8,
              value | pLightCtlTemp_status[i] << 8);
    }
    else if(i == 8)
    {
      TRACE_M(TF_LIGHT,"Remaining Time: 0x%x (%d)\r\n",
              pLightCtlTemp_status[i],
              pLightCtlTemp_status[i]);
    }
  }
#endif
  
  return MOBLE_RESULT_SUCCESS;
}


/**                                                  
* @brief  Appli_Light_CtlTemperature_Range_Set: This function is callback for Application
*         when Light CTL temperature range status message is received
* @param  pCtlTempRange_status: Pointer to the parameters received for message
* @param  pLength: length of data
* @param  dstPeer: destination send by peer for this node. It can be a
*                     unicast or group address 
* @param  elementIndex: index of the element received from peer for this node which
*                     is elementNumber-1
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT Appli_Light_CtlTemperature_Range_Status(MOBLEUINT8 const *pCtlTempRange_status, 
                                                     MOBLEUINT32 pLength, 
                                                     MOBLEUINT16 dstPeer, 
                                                     MOBLEUINT8 elementIndex)
{
#if ( CFG_DEBUG_TRACE != 0 )
  MOBLEUINT32 i;
  MOBLEUINT16 value;
#endif
  
  TRACE_M(TF_LIGHT,"Light_CtlTemperature_Range_Status callback received for element %d \r\n", elementIndex);
  TRACE_M(TF_SERIAL_CTRL,"#8263!\r\n");
    
#if ( CFG_DEBUG_TRACE != 0 )
  for(i = 0; i < pLength; i++)
  {
    if(i == 0)
    {
      TRACE_M(TF_LIGHT,"Status Code: 0x%x (%d)\r\n",
              pCtlTempRange_status[i],
              pCtlTempRange_status[i]);
    }
    else if((i == 1) || (i == 3))
    {
      value = pCtlTempRange_status[i];
    }
    else if(i == 2)
    {
      TRACE_M(TF_LIGHT,"Range Min: 0x%x (%d)\r\n",
              value | pCtlTempRange_status[i] << 8,
              value | pCtlTempRange_status[i] << 8 );
    }
    else if(i == 4)
    {
      TRACE_M(TF_LIGHT,"Range Max: 0x%x (%d)\r\n",
              value | pCtlTempRange_status[i] << 8,
              value | pCtlTempRange_status[i] << 8 );
    }
  }
#endif

  return MOBLE_RESULT_SUCCESS;
}

/**                                                  
* @brief  Appli_Light_CtlDefault_Status: This function is callback for Application
*         when Light CTL Default status message is received
* @param  pCtlDefault_status: Pointer to the parameters received for message
* @param  pLength: length of data
* @param  dstPeer: destination send by peer for this node. It can be a
*                     unicast or group address 
* @param  elementIndex: index of the element received from peer for this node which
*                     is elementNumber-1
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT Appli_Light_CtlDefault_Status(MOBLEUINT8 const *pCtlDefault_status, MOBLEUINT32 pLength, MOBLEUINT16 dstPeer, 
                                              MOBLEUINT8 elementIndex)
{
#if ( CFG_DEBUG_TRACE != 0 )
  MOBLEUINT32 i;
  MOBLEUINT16 value = 0;
#endif
  
  TRACE_M(TF_LIGHT,"Light_Ctl_DefaultStatus callback received for element %d \r\n", elementIndex);
  TRACE_M(TF_SERIAL_CTRL,"#8268!\r\n");
  
#if ( CFG_DEBUG_TRACE != 0 )
  for(i = 0; i < pLength; i++)
  {
    if((i == 0) || (i == 2) || (i == 4))
    {
      value = pCtlDefault_status[i];
    }
    else if(i == 1)
    {
      TRACE_M(TF_LIGHT,"Lightness: 0x%x (%d)\n\r",
              value | pCtlDefault_status[i] << 8,
              value | pCtlDefault_status[i] << 8);
    }
    else if(i == 3)
    {
      TRACE_M(TF_LIGHT,"Temperature: 0x%x (%d)\n\r",
              value | pCtlDefault_status[i] << 8,
              value | pCtlDefault_status[i] << 8);
    }
    else if(i == 5)
    {
      TRACE_M(TF_LIGHT,"Delta UV: 0x%x (%d)\n\r",
              value | pCtlDefault_status[i] << 8,
              value | pCtlDefault_status[i] << 8);
    }
  }
#endif
  
  return MOBLE_RESULT_SUCCESS;
} 

/**                                                  
* @brief  Appli_Light_Hsl_Status: This function is callback for Application
*         when Light HSL status message is received
* @param  pHsl_status: Pointer to the parameters received for message
* @param  pLength: length of data
* @param  dstPeer: destination send by peer for this node. It can be a
*                     unicast or group address 
* @param  elementIndex: index of the element received from peer for this node which
*                     is elementNumber-1
* @retval MOBLE_RESULT
*/
MOBLE_RESULT Appli_Light_Hsl_Status(MOBLEUINT8 const *pHsl_status, MOBLEUINT32 pLength, MOBLEUINT16 dstPeer, 
                                              MOBLEUINT8 elementIndex)
{
#if ( CFG_DEBUG_TRACE != 0 )
  MOBLEUINT32 i;
  MOBLEUINT16 value = 0;
#endif

  TRACE_M(TF_LIGHT,"Light_Hsl_Status callback received for element %d \r\n", elementIndex);
  TRACE_M(TF_SERIAL_CTRL,"#8278!\r\n");
  
#if ( CFG_DEBUG_TRACE != 0 )
  for(i = 0; i < pLength; i++)
  {
    if((i == 0) || (i == 2) || (i == 4))
    {
      value = pHsl_status[i];
    }
    else if(i == 1)
    {
      TRACE_M(TF_LIGHT,"HSL Lightness: 0x%x (%d)\n\r",
              value | pHsl_status[i] << 8,
              value | pHsl_status[i] << 8);
    }
    else if(i == 3)
    {
      TRACE_M(TF_LIGHT,"HSL Hue: 0x%x (%d)\n\r",
              value | pHsl_status[i] << 8,
              value | pHsl_status[i] << 8);
    }
    else if(i == 5)
    {
      TRACE_M(TF_LIGHT,"HSL Saturation: 0x%x (%d)\n\r",
              value | pHsl_status[i] << 8,
              value | pHsl_status[i] << 8);
    }
    else if(i == 6)
    {
      TRACE_M(TF_LIGHT,"Remaining Time: 0x%x (%d)\n\r",
              pHsl_status[i],
              pHsl_status[i]);
    }
  }
#endif
  
  return MOBLE_RESULT_SUCCESS;
}

/**                                                  
* @brief  Appli_Light_HslHue_Status: This function is callback for Application
*         when Light HSL HUE status message is received
* @param  pHslHue_status: Pointer to the parameters received for message
* @param  pLength: length of data
* @param  dstPeer: destination send by peer for this node. It can be a
*                     unicast or group address 
* @param  elementIndex: index of the element received from peer for this node which
*                     is elementNumber-1
* @retval MOBLE_RESULT
*/
MOBLE_RESULT Appli_Light_HslHue_Status(MOBLEUINT8 const *pHslHue_status, MOBLEUINT32 pLength, MOBLEUINT16 dstPeer, 
                                              MOBLEUINT8 elementIndex)
{
#if ( CFG_DEBUG_TRACE != 0 )
  MOBLEUINT32 i;
  MOBLEUINT16 value = 0;
#endif
  
  TRACE_M(TF_LIGHT,"Light_HslHue_Status callback received for element %d \r\n", elementIndex);
  TRACE_M(TF_SERIAL_CTRL,"#8271!\r\n");
    
#if ( CFG_DEBUG_TRACE != 0 )
  for(i = 0; i < pLength; i++)
  {
    if((i == 0) || (i == 2))
    {
      value = pHslHue_status[i];
    }
    else if(i == 1)
    {
      TRACE_M(TF_LIGHT,"Present Hue: 0x%x (%d)\n\r",
              value | pHslHue_status[i] << 8,
              value | pHslHue_status[i] << 8);
    }
    else if(i == 3)
    {
      TRACE_M(TF_LIGHT,"Target Hue: 0x%x (%d)\n\r",
              value | pHslHue_status[i] << 8,
              value | pHslHue_status[i] << 8);
    }
    else if(i == 4)
    {
      TRACE_M(TF_LIGHT,"Remaining Time: 0x%x (%d)\n\r",
              pHslHue_status[i],
              pHslHue_status[i]);
    }
  }
#endif
  
  return MOBLE_RESULT_SUCCESS;
} 

/**                                                  
* @brief  Appli_Light_HslSaturation_Status: This function is callback for Application
*         when Light HSL Saturation status message is received
* @param  pHslSaturation_status: Pointer to the parameters received for message
* @param  pLength: length of data
* @param  dstPeer: destination send by peer for this node. It can be a
*                     unicast or group address 
* @param  elementIndex: index of the element received from peer for this node which
*                     is elementNumber-1
* @retval MOBLE_RESULT
*/
MOBLE_RESULT Appli_Light_HslSaturation_Status(MOBLEUINT8 const *pHslSaturation_status, MOBLEUINT32 pLength, MOBLEUINT16 dstPeer, 
                                              MOBLEUINT8 elementIndex)
{
#if ( CFG_DEBUG_TRACE != 0 )
  MOBLEUINT32 i;
  MOBLEUINT16 value = 0;
#endif
  
  TRACE_M(TF_LIGHT,"Light_HslSaturation_Status callback received for element %d \r\n", elementIndex);
  TRACE_M(TF_SERIAL_CTRL,"#8275!\r\n");
  
#if ( CFG_DEBUG_TRACE != 0 )
  for(i = 0; i < pLength; i++)
  {
    if((i == 0) || (i == 2))
    {
      value = pHslSaturation_status[i];
    }
    else if(i == 1)
    {
      TRACE_M(TF_LIGHT,"Present Saturation: 0x%x (%d)\n\r",
              value | pHslSaturation_status[i] << 8,
              value | pHslSaturation_status[i] << 8);
    }
    else if(i == 3)
    {
      TRACE_M(TF_LIGHT,"Target Saturation: 0x%x (%d)\n\r",
              value | pHslSaturation_status[i] << 8,
              value | pHslSaturation_status[i] << 8);
    }
    else if(i == 4)
    {
      TRACE_M(TF_LIGHT,"Remaining Time: 0x%x (%d)\n\r",
              pHslSaturation_status[i],
              pHslSaturation_status[i]);
    }
  }
#endif
  
  return MOBLE_RESULT_SUCCESS;
}

/**                                                  
* @brief  Appli_Light_HslDefault_Status: This function is callback for Application
*          when Light HSL Default status  message is received
* @param  pHslDefault_status: Pointer to the parameters received for message
* @param  pLength: length of data
* @retval MOBLE_RESULT
*/
MOBLE_RESULT Appli_Light_HslDefault_Status(MOBLEUINT8 const *pHslDefault_status, MOBLEUINT32 pLength, MOBLEUINT16 dstPeer, 
                                              MOBLEUINT8 elementIndex)
{
#if ( CFG_DEBUG_TRACE != 0 )
  MOBLEUINT32 i;
  MOBLEUINT16 value = 0;
#endif
  
  TRACE_M(TF_LIGHT,"Light_HslDefault_Status callback received for element %d \r\n", elementIndex);  
  TRACE_M(TF_SERIAL_CTRL,"#827C!\r\n");

#if ( CFG_DEBUG_TRACE != 0 )
  for(i = 0; i < pLength; i++)
  {
    if((i == 0) || (i == 2) || (i == 4))
    {
      value = pHslDefault_status[i];
    }
    else if(i == 1)
    {
      TRACE_M(TF_LIGHT,"Lightness: 0x%x (%d)\n\r",
              value | pHslDefault_status[i] << 8,
              value | pHslDefault_status[i] << 8);
    }
    else if(i == 3)
    {
      TRACE_M(TF_LIGHT,"Hue: 0x%x (%d)\n\r",
              value | pHslDefault_status[i] << 8,
              value | pHslDefault_status[i] << 8);
    }
    else if(i == 5)
    {
      TRACE_M(TF_LIGHT,"Saturation: 0x%x (%d)\n\r",
              value | pHslDefault_status[i] << 8,
              value | pHslDefault_status[i] << 8);
    }
  }
#endif
  
  return MOBLE_RESULT_SUCCESS;
}

/**                                                  
* @brief  Appli_Light_HslRange_Status: This function is callback for Application
*         when Light HSL range status message is received
* @param  pHslRange_status: Pointer to the parameters received for message
* @param  pLength: length of data
* @param  dstPeer: destination send by peer for this node. It can be a
*                     unicast or group address 
* @param  elementIndex: index of the element received from peer for this node which
*                     is elementNumber-1
* @retval MOBLE_RESULT
*/
MOBLE_RESULT Appli_Light_HslRange_Status(MOBLEUINT8 const *pHslRange_status, MOBLEUINT32 pLength, MOBLEUINT16 dstPeer, 
                                              MOBLEUINT8 elementIndex)
{
#if ( CFG_DEBUG_TRACE != 0 )
  MOBLEUINT32 i;
  MOBLEUINT16 value = 0;
#endif
  
  TRACE_M(TF_LIGHT,"Light_HslRange_Status callback received for element %d \r\n", elementIndex); 
  TRACE_M(TF_SERIAL_CTRL,"#827E!\r\n");
  
    
#if ( CFG_DEBUG_TRACE != 0 )
  for(i = 0; i < pLength; i++)
  {
    if(i == 0)
    {
      TRACE_M(TF_LIGHT,"Status Code: 0x%x (%d)\n\r",
              pHslRange_status[i],
              pHslRange_status[i]);
    }
    else if((i == 1) || (i == 3) || (i == 5) || (i == 7))
    {
      value = pHslRange_status[i];
    }
    else if(i == 2)
    {
      TRACE_M(TF_LIGHT,"Hue Range Min: 0x%x (%d)\n\r",
              value | pHslRange_status[i] << 8,
              value | pHslRange_status[i] << 8);
    }
    else if(i == 4)
    {
      TRACE_M(TF_LIGHT,"Hue Range Max: 0x%x (%d)\n\r",
              value | pHslRange_status[i] << 8,
              value | pHslRange_status[i] << 8);
    }
    else if(i == 6)
    {
      TRACE_M(TF_LIGHT,"Saturation Range Min: 0x%x (%d)\n\r",
              value | pHslRange_status[i] << 8,
              value | pHslRange_status[i] << 8);
    }
    else if(i == 8)
    {
      TRACE_M(TF_LIGHT,"Saturation Range Max: 0x%x (%d)\n\r",
              value | pHslRange_status[i] << 8,
              value | pHslRange_status[i] << 8);
    }
  }
#endif

  return MOBLE_RESULT_SUCCESS;
}


/**
* @}
*/

/**
* @}
*/


