/**
******************************************************************************
* @file    vendor.c
* @author  BLE Mesh Team
* @version V1.09.000
* @date    15-Oct-2018
* @brief   Vendor model middleware file
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
#include "ble_mesh.h"
#include "vendor.h"
#include "generic.h"
#include "sensors.h"
#include "models_if.h"
#include "mesh_cfg.h"

#include <string.h>

/** @addtogroup BLE_Mesh
 *  @{
 */

/** @addtogroup models_BLE
 *  @{
 */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
   
/*Appli Buffer - Toggle the LEDs*/
/*0x03 = SubCommand for LED Toggle*/
MOBLEUINT8 AppliBuffer[DATA_BUFFER_LENGTH] = {0x01,0x00};
MOBLEUINT16 CommandStatus = 0;
extern MOBLEUINT8 NumberOfElements;
extern MOBLEUINT8 ResponseBuffer[8];
extern MOBLEUINT8 BuffLength;
extern MOBLEUINT8 Appli_LedState;

/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/

/**
* @brief  Call back function called when action is required on node itself  
* @param  peer_addr: Address of the peer
* @param  dst_peer: destination send by peer for this node. It can be a
*                                                     unicast or group address 
* @param  command: Receiving command code
* @param  *data: Pointer to the data received from peer_addr
* @param  length: Length of the data
* @param  response: 1 if command acknowledgement is expected by peer_addr 
* @retval MOBLE_RESULT status of result
*/ 
MOBLE_RESULT Vendor_WriteLocalDataCb(MOBLE_ADDRESS peer_addr, 
                                    MOBLE_ADDRESS dst_peer, 
                                    MOBLEUINT8 command, 
                                    MOBLEUINT8 const *data, 
                                    MOBLEUINT32 length, 
                                    MOBLEBOOL response)
{
  
  MOBLE_COMMAND_STATUS status = STATUS_SUCCESS;
   
#if !defined(DISABLE_TRACES)
  /* Traces for the Data */
  uint16_t idx = 0;
  printf("Vendor_WriteLocalDataCb: peer_addr=[%02x], dst_peer=[%02x],\
         command=[%02x], \n\r", peer_addr, dst_peer, command);
           printf("DATA_RECEIVED length = %d\n\r",length);
         for (idx=0; idx<length; idx++)
         {
           printf("data[%d]= %d",idx,data[idx]);
           printf("\n\r");
         }
#endif
         
    if(ADDRESS_IS_UNICAST(dst_peer))
     {
         MOBLEUINT8 elementNumber;
         elementNumber=BLEMesh_ModelsGetElementNumber(dst_peer);
           
         switch (command)
         {
             
           case APPLI_TEST_CMD:
             {
               /*Insert Test Commands here*/
               VendorAppli_cb.TestCommand_cb(data, length);
               break;
             }    
             
           case APPLI_LED_CONTROL_STATUS_CMD:
             {
               /* 
       Meesage Received     B0     B1    B2      B3    B4    B5    B6     B7 
                   B0 - Sub-Cmd LED
                   B1-B7 - Data Bytes       
               */          
               VendorAppli_cb.LEDControlCommand_Cb(data,length,elementNumber,dst_peer);
               break;
             }
             
             /* Default case - Not valid command */
           default:
             {
               status = STATUS_INVALID_COMMAND;
               break;
             }
          }
      }
         
    else if(ADDRESS_IS_GROUP(dst_peer))
    {
      
      MOBLEUINT8 elementNumber;
      MOBLE_RESULT status1;
      
       /*Check the Subscription of Group Address for all the Elements. 
      In case of Multi Elements, all elements may be subscribed to same Group Address. 
       e.g 0xC000 is assigned to all elements
      or different Group addresses. Need to check subscription for each element */
    for (elementNumber=FIRST_ELEMENT;elementNumber<=NumberOfElements; elementNumber++)
    {
       /*If Received Address is Broadcast Address*/
       if(ADDRESS_IS_ALL_NODES(dst_peer))
       {
          status1 =  MOBLE_RESULT_SUCCESS;
       }
       else
       {
          status1 = BLEMesh_ModelsCheckSubscription(dst_peer,elementNumber);
       }
        
        if(status1 == MOBLE_RESULT_SUCCESS)
        { 
          switch (command)
          { 
            
          case APPLI_TEST_CMD:
            { /*Insert Test Commands here*/
              break;
            }    
          case APPLI_LED_CONTROL_STATUS_CMD:
            {
        /* 
       Meesage Received     B0     B1    B2      B3    B4    B5    B6     B7 
                   B0 - Sub-Cmd LED
                   B1-B7 - Data Bytes       

       */
              VendorAppli_cb.LEDControlCommand_Cb(data, length, elementNumber,dst_peer);
              break; 
            }        
            /* Default case - Not valid command */
          default:
            {
              status = STATUS_INVALID_COMMAND;
              break;
            }
          }           
        }
      }
    }
    /* this expression is used to send the status of data received wheather it
       is correct or not , First 3 bits are for status and last 5 bit are for sub commands
    */
      ResponseBuffer[0] = ResponseBuffer[0] | (status << 5);
      
      VendorAppli_cb.LEDControl_cb(); /* Controls the LED State */
        /* 
       If response is true, Message response will be sent      
       Message Response     B0     B1    B2      B3    B4    B5    B6     B7 
                   B0 - Sub-Cmd 
                   B1-B7 - Response Bytes if any 
       */
         if (response == MOBLE_TRUE)
         {
           if (ADDRESS_IS_UNICAST(dst_peer))
           {
             BLEMesh_SendResponse(peer_addr, dst_peer, command, ResponseBuffer, BuffLength);
           }
           else
           {
             /* Randomize responses*/
             BLEMesh_ModelsDelayPacket(peer_addr, dst_peer, command, ResponseBuffer, BuffLength);
           }
#if !defined(DISABLE_TRACES)		
           printf("Sending Response \n\r");
#endif
         }
         
         if (status == STATUS_SUCCESS)
         {
           return MOBLE_RESULT_SUCCESS;  
         }
         else
         {
           return MOBLE_RESULT_FAIL;
         }
         
}
         
         
/**
* @brief  Call back function called when some data is required from node    
* @param  peer_addr: Address of the peer
* @param  dst_peer: destination set by peer
* @param  command: Receiving command code 
* @param  data: Pointer to the data
* @param  length: Length of the data
* @param  response: Response Enable. For future use
* @retval MOBLE_RESULT status of result
*/
MOBLE_RESULT Vendor_ReadLocalDataCb(MOBLE_ADDRESS peer_addr, 
                                   MOBLE_ADDRESS dst_peer, 
                                   MOBLEUINT8 command, 
                                   MOBLEUINT8 const *data, 
                                   MOBLEUINT32 length, 
                                   MOBLEBOOL response)
{  
  
MOBLE_RESULT status = MOBLE_RESULT_SUCCESS;
  
#if !defined(DISABLE_TRACES)
  /* Traces for the Data */
  printf("Vendor_ReadLocalDataCb: peer_addr=[%02x], dst_peer_addr=[%02x],\
  command=[%02x] \n\r", peer_addr, dst_peer, command);
  printf("DATA_RECEIVED length = %d\n\r",length);
  for (MOBLEUINT8 idx=0; idx<length; idx++)
  {
    printf("data[%d]= %d",idx,data[idx]);
    printf("\n\r");
  }
#endif
  
 if(ADDRESS_IS_UNICAST(dst_peer))
  {
    MOBLEUINT8 elementNumber;
    elementNumber=BLEMesh_ModelsGetElementNumber(dst_peer);
    
   /* Check which command is in packet */
   switch(command)
   {
    case APPLI_TEST_CMD:
      {
       /* 
       Meesage Received     B0     B1    B2      B3    B4    B5    B6     B7 
                   B0 - Sub-Cmd for APPLI_TEST_CMD
                   B1-B7 - Data Bytes if any  

       */
       status =  VendorAppli_cb.TestCommand_cb(data, length);
        break;
      }
    case APPLI_DEVICE_INFO_CMD:     
      {
        /*
       Meesage Received     B0     B1    B2      B3    B4    B5    B6     B7 
                   B0 - Sub-Cmd for APPLI_DEVICE_INFO_CMD
                   B1-B7 - Data Bytes if any      

       */
       status =  VendorAppli_cb.DeviceInfoCommand_cb(data, length);
        break;
      }
    case APPLI_SENSOR_CNTRL_CMD:
      {
          /*Insert Sensors related commands here*/
          break;
      }
   case APPLI_ELEMENT_TYPE_CMD:
     {
       /*
       Meesage Received with Command APPLI_ELEMENT_TYPE_CMD
       */
        ResponseBuffer[0] = ELEMENT_TYPE_LED;
        ResponseBuffer[1] = ELEMENT_TYPE_LED;
        ResponseBuffer[2] = ELEMENT_TYPE_SWITCH;
        BuffLength  = 3;      
        break;
     }
   case APPLI_LED_CONTROL_STATUS_CMD:
    {
        /*
         Meesage Received with Command APPLI_LED_CONTROL_STATUS_CMD
         ResponseBuffer will send the response of selected Element
       */
        if(elementNumber == FIRST_ELEMENT)
        {
          ResponseBuffer[0] = data[0];
          ResponseBuffer[1] = Appli_LedState;
        }
        
        else if(elementNumber == SECOND_ELEMENT)
        {
          /*Send LED Status*/
        }
        else if(elementNumber == THIRD_ELEMENT) 
        {
          /*Sent Button State*/
        }
        BuffLength = 1;  
        
        break;
    }
      
    default:
      {
        status = MOBLE_RESULT_INVALIDARG;
        break;
      }   
    
  }
  }
   /* this expression is used to send the status of data received wheather it
      is correct or not , First 3 bits are for status and last 5 bit are for sub commands
  */
   ResponseBuffer[0] = ResponseBuffer[0] | (status << 5);
  /* Check if the command executed sucessfully or not */ 
   
  if (MOBLE_RESULT_SUCCESS == status)
  {
    
       /* 
       Read Command will always be reliable      
       Message Response     B0     B1    B2      B3    B4    B5    B6     B7 
                   B0 - Sub-Cmd for which response is needed
                   B1-B7 - Data Bytes if any 
       */
      if (ADDRESS_IS_UNICAST(dst_peer))
      {
        BLEMesh_SendResponse(peer_addr, dst_peer, command, ResponseBuffer, BuffLength);
      }
      else
      {
        /* Randomize responses*/
        BLEMesh_ModelsDelayPacket(peer_addr, dst_peer, command, ResponseBuffer, BuffLength);
      }
  }
  /* Command not successfully processed */
  else
  {
      if (ADDRESS_IS_UNICAST(dst_peer))
      {
        BLEMesh_SendResponse(peer_addr, dst_peer, command, ResponseBuffer, 1);
      }
      else
      {
        /* Randomize responses*/
        BLEMesh_ModelsDelayPacket(peer_addr, dst_peer, command, ResponseBuffer, 1);
      }
  }
  
  if (status == MOBLE_RESULT_SUCCESS)
  {
    return MOBLE_RESULT_SUCCESS;  
  }
  else
  {
    return MOBLE_RESULT_FAIL;
  }
  
}

         
/**
* @brief  Call back function called when some data is send by the node to app   
* @param  peer_addr: Address of the peer
* @param  elementIndex: Index of the selected element
* @param  data: Pointer to the data
* @param  length: Length of the data
* @retval MOBLE_RESULT status of result
*/  
MOBLE_RESULT Vendor_OnResponseDataCb(MOBLE_ADDRESS peer_addr, 
                                    MOBLEUINT8 elementIndex,
                                    MOBLEUINT8 const * data,
                                    MOBLEUINT32 length)
{
  
 #if !defined(DISABLE_TRACES)
  /* Traces for the Data */
  printf("Vendor_OnResponseDataCb: peer_addr=[%02x], \
   \n\r", peer_addr);
  printf("DATA_RECEIVED length = %d\n\r", length);
  for (MOBLEUINT8 idx=0; idx<length; idx++)
  {
    printf("data[%d]= %d",idx,data[idx]);
    printf("\n\r");
  }
#endif 
  return MOBLE_RESULT_SUCCESS;
} 
         

/**
* @brief  State machine for Vendor Model
* @param  void
* @retval void
*/ 
void Vendor_Process(void)
{
  BLEMesh_ModelsSendDelayedPacket();
}

         
/**
* @brief  Publish Command for Vendor Model
* @param  publishAddress: Publish Address of the message 
* @param  elementIndex: index of the element
* @retval void
*/          
void Vendor_Publish(MOBLE_ADDRESS publishAddress, MOBLEUINT8 elementIndex)
{
  
  /* changes the LED status on other nodes in the network */
  if(CommandStatus == APPLI_CMD_ON)
  {
    AppliBuffer[0] = APPLI_CMD_OFF;
  }
  else
  {
    AppliBuffer[0] = APPLI_CMD_ON;
  }
  
   
  BLEMesh_SetRemoteData(publishAddress,elementIndex,
                            APPLI_LED_CONTROL_STATUS_CMD , 
                            AppliBuffer, sizeof(AppliBuffer), MOBLE_FALSE , MOBLE_TRUE);
  
                            
  
  CommandStatus = AppliBuffer[0];
 
}
         
/**
 * @}
 */

/**
 * @}
 */

/******************* (C) COPYRIGHT 2017 STMicroelectronics *****END OF FILE****/

         




