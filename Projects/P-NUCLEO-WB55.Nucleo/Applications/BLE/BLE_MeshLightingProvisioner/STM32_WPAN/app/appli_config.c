/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    appli_config.c
  * @author  MCD Application Team
  * @brief   Config Model APIs callback from ST BLE Mesh Library
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
#include <string.h>
#include "hal_common.h"
#include "ble_mesh.h"
#include "appli_config.h"
#include "mesh_cfg.h"

/** @addtogroup ST_BLE_Mesh
*  @{
*/

/** @addtogroup Application_Mesh_Models
*  @{
*/

/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/


/*****************************Config Model Callbacks***************************/
/**
* @brief  Callback from Config Model to receive the AppKey Add Command
* @param  Structure Pointer having parameters - netKeyIndex, appKeyIndex
*                                               appKey, keySize & status
* @retval none
*/ 
void Appli_GetAppKeyAddParamsCb(model_securityKeyParams_t* appKeyAddParams)
{
  TRACE_I(TF_CONFIG_SERVER,"AppKey Add received for netKeyIndex %.2x, appKeyIndex %.2x \r\n",\
          appKeyAddParams->netKeyIndex, appKeyAddParams->appKeyIndex);

  TRACE_I(TF_CONFIG_SERVER,"Status is : %.2x \r\n", appKeyAddParams->status);
  /*Success Command*/
  if(appKeyAddParams->status == 0x00)
  {
    TRACE_I(TF_CONFIG_SERVER,"AppKey is: ");
    for (MOBLEUINT8 count=0 ; count<appKeyAddParams->keySize; count++)
    {
      TRACE_I(TF_CONFIG_SERVER,"%.2x", *(appKeyAddParams->appKey));
      appKeyAddParams->appKey++;
    }  
    TRACE_I(TF_CONFIG_SERVER," \r\n");
  }
}

/**
* @brief  Callback from Config Model to receive the AppKey Delete Command
* @param  Structure Pointer having parameters - netKeyIndex, appKeyIndex
*                                               appKey, keySize & status
* @retval none
*/ 
void Appli_GetAppKeyDeleteParamsCb(model_securityKeyParams_t* appKeyDeleteParams)
{
  TRACE_I(TF_CONFIG_SERVER,"App Key Delete received for netKeyIndex %.2x, appKeyIndex %.2x \r\n",\
          appKeyDeleteParams->netKeyIndex, appKeyDeleteParams->appKeyIndex);
  
  TRACE_I(TF_CONFIG_SERVER,"Status is : %.2x \r\n", appKeyDeleteParams->status);
}

/**
* @brief  Callback from Config Model to receive the AppKey List
* @param  Structure Pointer having parameters - netKeyIndex, appKeyIndexes & appKeyListParams Size
* @retval none
*/ 
void Appli_GetAppKeyListParamsCb(model_appKeyListParams_t* appKeyListParams)
{
  TRACE_I(TF_CONFIG_SERVER,"App Key List for netKeyIndex %.2x is \r\n", appKeyListParams->netKeyIndex);
  TRACE_I(TF_CONFIG_SERVER,"Status is : %.2x \r\n", appKeyListParams->status);
  /*Success*/
  if(appKeyListParams->status == 0x00)
  {
    TRACE_I(TF_CONFIG_SERVER,"AppKey Indexes are: \r\n");
    for (MOBLEUINT8 count=0 ; count< appKeyListParams->appKeyIndexList_size; count++)
    {
      TRACE_I(TF_CONFIG_SERVER,"%.2x", *(appKeyListParams->appKeyIndexList));
      appKeyListParams->appKeyIndexList++;
    }  
   TRACE_I(TF_CONFIG_SERVER," \r\n");
  }
}

/**
* @brief  Callback from Config Model to receive the Updated App Key
* @param  Structure Pointer having parameters - netKeyIndex, appKeyIndex
*                                               appKey, keySize & status
* @retval none
*/ 
void Appli_GetAppKeyUpdateParamsCb(model_securityKeyParams_t* appKeyUpdatedParams)
{
  TRACE_I(TF_CONFIG_SERVER,"App Key Update received for netKeyIndex %.2x, appKeyIndex %.2x \r\n",\
          appKeyUpdatedParams->netKeyIndex, appKeyUpdatedParams->appKeyIndex);

  TRACE_I(TF_CONFIG_SERVER,"Status is : %.2x \r\n", appKeyUpdatedParams->status);
  
  /*Success Command*/
  if(appKeyUpdatedParams->status == 0x00)
  {
    TRACE_I(TF_CONFIG_SERVER,"Updated AppKey is: ");
    for (MOBLEUINT8 count=0 ; count<appKeyUpdatedParams->keySize; count++)
    {
      TRACE_I(TF_CONFIG_SERVER,"%.2x", *(appKeyUpdatedParams->appKey));
      appKeyUpdatedParams->appKey++;
    }
    TRACE_I(TF_CONFIG_SERVER," \r\n");
    TRACE_I(TF_CONFIG_SERVER,"It will be replaced by old appKey after Key Refresh \r\n");
  }
  
}

/**
* @brief  Callback from Config Model to receive the NetKey Add Command
* @param  Structure Pointer having parameters - netKeyIndex & netKey
* @retval none
*/ 
void Appli_GetNetKeyAddParamsCb(model_securityKeyParams_t* netKeyAddParams)
{
  TRACE_I(TF_CONFIG_SERVER,"Net Key Add received for netKeyIndex %.2x \r\n",\
                                               netKeyAddParams->netKeyIndex);

  TRACE_I(TF_CONFIG_SERVER,"Status is : %.2x \r\n", netKeyAddParams->status);

  /*Success Command*/
  if(netKeyAddParams->status == 0x00)
  {
    TRACE_I(TF_CONFIG_SERVER,"NetKey is: ");
    for (MOBLEUINT8 count=0 ; count < netKeyAddParams->keySize; count++)
    {
      TRACE_I(TF_CONFIG_SERVER,"%.2x", *(netKeyAddParams->netKey));
      netKeyAddParams->netKey++;
    } 
    TRACE_I(TF_CONFIG_SERVER," \r\n");
  }
}

/**
* @brief  Callback from Config Model to receive the NetKey Delete Command
* @param  Structure Pointer having parameters - netKeyIndex & netKey
* @retval none
*/ 
void Appli_GetNetKeyDeleteParamsCb(model_securityKeyParams_t* netKeyDeleteParams)
{ 
   TRACE_I(TF_CONFIG_SERVER,"Net Key Delete received for netKeyIndex %d \r\n",\
                                               netKeyDeleteParams->netKeyIndex);
   TRACE_I(TF_CONFIG_SERVER,"Status is : %.2x \r\n", netKeyDeleteParams->status);
}

/**
* @brief  Callback from Config Model to receive the NetKey List
* @param  Structure Pointer having parameters - netKeyIndexes & netKeyListParams Size
* @retval none
*/ 
void Appli_GetNetKeyListParamsCb(model_netKeyListParams_t* netKeyListParams)
{
  TRACE_I(TF_CONFIG_SERVER,"Status is : %.2x \r\n", netKeyListParams->status);
  TRACE_I(TF_CONFIG_SERVER,"NetKey Indexes are: \r\n");
  for (MOBLEUINT8 count=0 ; count< netKeyListParams->netKeyIndexList_size; count++)
  {
    TRACE_I(TF_CONFIG_SERVER,"%.2x", *(netKeyListParams->netKeyIndexList));
    netKeyListParams->netKeyIndexList++;
  }
  TRACE_I(TF_CONFIG_SERVER," \r\n");  
}


/**
* @brief  Callback from Config Model to receive the Updated NetKey Command
* @param  Structure Pointer having parameters - netKeyIndex & netKey
* @retval none
*/ 
void Appli_GetNetKeyUpdateParamsCb(model_securityKeyParams_t* netKeyUpdatedParams)
{
  TRACE_I(TF_CONFIG_SERVER,"Net Key Update received for netKeyIndex %d \r\n",\
                                     netKeyUpdatedParams->netKeyIndex);
  
  TRACE_I(TF_CONFIG_SERVER,"Status is : %.2x \r\n", netKeyUpdatedParams->status);
  if(netKeyUpdatedParams->status == 0)
  {
    TRACE_I(TF_CONFIG_SERVER,"Updated NetKey is:");
    for (MOBLEUINT8 count=0; count<netKeyUpdatedParams->keySize; count++)
    {
      TRACE_I(TF_CONFIG_SERVER,"%.2x", *(netKeyUpdatedParams->netKey));
      netKeyUpdatedParams->netKey++;
    }
  TRACE_I(TF_CONFIG_SERVER," \r\n");
  TRACE_I(TF_CONFIG_SERVER,"It will be replaced by old netKey after Key Refresh \r\n");
  }
}

/**
* @brief  Callback from Config Model to receive the AppKey binding params
* @param  Structure Pointer having parameters - elementIndex, appKeyIndex & modelID
* @retval none
*/ 
void Appli_GetAppKeyBindingParamsCb(model_appKeyBindingParams_t* appKeyBindParams)
{
  TRACE_I(TF_CONFIG_SERVER,"AppKey binding received for elementIndex %d, modelID %.2lx,\
          appKeyIndex %.2x & status %.2x \r\n", appKeyBindParams->elementIndex,\
          appKeyBindParams->modelID, appKeyBindParams->appKeyIndex, appKeyBindParams->status);
}

/**
* @brief  Callback from Config Model to receive the AppKey Unbinding params
* @param  Structure Pointer having parameters - elementIndex, appKeyIndex & modelID
* @retval none
*/ 
void Appli_GetAppKeyUnBindingParamsCb(model_appKeyBindingParams_t* appKeyUnbindParams)
{
  TRACE_I(TF_CONFIG_SERVER,"AppKey unbinding received for elementIndex %d, modelID %.2lx,\
          appKeyIndex %.2x & status %.2x \r\n",appKeyUnbindParams->elementIndex,\
          appKeyUnbindParams->modelID, appKeyUnbindParams->appKeyIndex,\
          appKeyUnbindParams->status);  
}

/**
* @brief  Callback from Config Model to receive the Subscription Add params
* @param  Structure Pointer having parameters - elementIndex, appKeyIndex, modelID
*            subAddress, isVirtual & allSubDeleted
* @retval none
*/ 
void Appli_GetSubAddParamsCb(model_subParams_t* subAddParams)
{
  
  TRACE_I(TF_CONFIG_SERVER,"Status is : %.2x \r\n", subAddParams->status); 
  if(subAddParams->isVirtual)
  {
    TRACE_I(TF_CONFIG_SERVER,"Subscription Virtual Add received for elementIndex %d, modelID %.2lx & subAddress %.2x \r\n",\
                              subAddParams->elementIndex, subAddParams->modelID,\
                                                 subAddParams->subAddress);     
  }
  
  else
  {
    TRACE_I(TF_CONFIG_SERVER,"Subscription Add received for elementIndex %d, modelID %.2lx & subAddress %.2x \r\n",\
                              subAddParams->elementIndex, subAddParams->modelID,\
                                                 subAddParams->subAddress); 
  }
}

/**
* @brief  Callback from Config Model to receive the Subscription Delete params
* @param  Structure Pointer having parameters - elementIndex, appKeyIndex, modelID
*            subAddress, isVirtual & allSubDeleted
* @retval none
*/ 
void Appli_GetSubDeleteParamsCb(model_subParams_t* subDeleteParams)
{
  TRACE_I(TF_CONFIG_SERVER,"Status is : %.2x \r\n", subDeleteParams->status); 
  if(subDeleteParams->isVirtual)
  {
    TRACE_I(TF_CONFIG_SERVER,"Subscription Virtual Add Delete received for elementIndex %d, modelID %.2lx & subAddress %.2x \r\n",\
                              subDeleteParams->elementIndex, subDeleteParams->modelID,\
                                                 subDeleteParams->subAddress);     
    
  }
  
  else if(subDeleteParams->allSubDeleted)
  {
    
     TRACE_I(TF_CONFIG_SERVER,"Delete All Subscription received for elementIndex %d, modelID %.2lx \r\n",\
                              subDeleteParams->elementIndex, subDeleteParams->modelID);        
    
  }
  else
  {
    TRACE_I(TF_CONFIG_SERVER,"Subscription Delete received for elementIndex %d, modelID %.2lx & subAddress %.2x \r\n",\
                              subDeleteParams->elementIndex, subDeleteParams->modelID,\
                                                 subDeleteParams->subAddress); 
  }  
}

/**
* @brief  Callback from Config Model to receive the Subscription Overwrite params
* @param  Structure Pointer having parameters - elementIndex, appKeyIndex, modelID
*            subAddress, isVirtual & allSubDeleted
* @retval none
*/ 
void Appli_GetSubOverwriteParamsCb(model_subParams_t* subOverwriteParams)
{
  TRACE_I(TF_CONFIG_SERVER,"Status is : %.2x \r\n", subOverwriteParams->status); 
  if(subOverwriteParams->isVirtual)
  {
    TRACE_I(TF_CONFIG_SERVER,"Subscription Virtual Overwrite received for elementIndex %d, modelID %.2lx & subAddress %.2x \r\n",\
                    subOverwriteParams->elementIndex, subOverwriteParams->modelID,\
                                                 subOverwriteParams->subAddress);     
    
  }
  
  else
  {
    TRACE_I(TF_CONFIG_SERVER,"Subscription Overwrite received for elementIndex %d, modelID %.2lx & subAddress %.2x \r\n",\
                     subOverwriteParams->elementIndex, subOverwriteParams->modelID,\
                                                 subOverwriteParams->subAddress); 
  }  
}

/**
* @brief  Callback from Config Model to receive the Publication Parameters
* @param  Structure Pointer having parameters - modelID, elementAddress,
*         Publish Address, PublishPeriod, PublishTTL, Credential Flag, 
*         Publish AppKeyIndex, Retransmit Count, Restransmit Interval Steps & status
* @retval none
*/ 
void Appli_GetPublicationSetParamsCb(model_publicationparams_t* pPubParameters)
{
  if(pPubParameters->modelID == SENSOR_SERVER_MODEL_ID)
  {
    Sensor_ModelPublishSet(pPubParameters);
  }
}

/**
* @brief  Callback from Config Model to receive the Publication Parameters
* @param  Structure Pointer having parameters - modelID, elementAddress,
*         Publish Address, PublishPeriod, PublishTTL, Credential Flag, 
*         Publish AppKeyIndex, Retransmit Count, Restransmit Interval Steps & status
* @retval none
*/ 
void Appli_GetPublicationGetParamsCb(model_publicationparams_t* pPubParameters)
{
  TRACE_I(TF_CONFIG_SERVER,"Status is : %.2x \r\n", pPubParameters->status); 
  TRACE_I(TF_CONFIG_SERVER,"Received Publication parameters:  credentialFlag = %.2x,\
          elementAddress = %.2x, modelID = %.2lx, appKeyIndex = %.2x,\
          publishPeriod = %.2x, publishRetransmitCount = %.2x, publishRetransmitIntervalSteps = %.2x,\
          publishTTL = %.2x \r\n", pPubParameters->credentialFlag, pPubParameters->elementAddress, pPubParameters->modelID,\
          pPubParameters->publishAppKeyIndex, pPubParameters->publishPeriod, pPubParameters->publishRetransmitCount,\
          pPubParameters->publishRetransmitIntervalSteps, pPubParameters->publishTTL);
}

/**
* @}
*/

/**
* @}
*/
