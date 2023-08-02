/**
******************************************************************************
* @file    vendor.c
* @author  BLE Mesh Team
* @brief   Vendor model middleware file
******************************************************************************
* @attention
*
* Copyright (c) 2018-2021 STMicroelectronics.
* All rights reserved.
*
* This software is licensed under terms that can be found in the LICENSE file
* in the root directory of this software component.
* If no LICENSE file comes with this software, it is provided AS-IS.
*
******************************************************************************
*/

/* Includes ------------------------------------------------------------------*/
#include "hal_common.h"
#include "mesh_cfg.h"
#include "ble_mesh.h"
#include "vendor.h"
#include "appli_test.h"
#include "appli_vendor.h"
#include "models_if.h"
#include "common.h"
#include <string.h>

/** @addtogroup MODEL_VENDOR
*  @{
*/

/** @addtogroup Vendor_Model_Callbacks
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
MOBLEUINT8 Vendor_SendDataFreqState = 0x00;
extern MOBLEUINT8 NumberOfElements;
extern MOBLEUINT8 ResponseBuffer[VENDOR_DATA_BUFFER_SIZE];
extern MOBLEUINT16 BuffLength;
extern MOBLEUINT8 Appli_LedState;
/*
-------------*******************-------------------------
Vendor Model Opcode Table
*/
MOBLEUINT16 Vendor_Opcodes_Table[] =
{
  APPLI_TEST_CMD,
  APPLI_DEVICE_INFO_CMD,
  APPLI_LED_CONTROL_STATUS_CMD,
  APPLI_ELEMENT_TYPE_CMD,
  APPLI_SENSOR_CNTRL_CMD,
  APPLI_DATA_CNTRL_CMD
};

char *Board_Type[] =
{
  "BLUENRG1_BRD_TYPE",           
  "BLUENRG2_BRD_TYPE",             
  "BLUENRG_MS_BRD_TYPE",
  "STM32WB55XX"
}; 
/* Private function prototypes -----------------------------------------------*/
#if ENABLE_APPLI_TEST
  extern MOBLEUINT8 txDataArray[];
  extern MOBLEUINT8 successCounter;
  extern MOBLEUINT8 sendCounter;
#endif
/* Private functions ---------------------------------------------------------*/

/**
* @brief  Call back function called when action is required on node itself  
* @param  *pmsgParam Pointer to structure of message header for parameters:
*          elementIndex, src, dst addresses, TTL, RSSI, NetKey & AppKey Offset                                     
* @param  command: Receiving command code
* @param  *data: Pointer to the data received from peer_addr
* @param  length: Length of the data
* @param  response: 1 if command acknowledgement is expected by peer_addr 
* @retval MOBLE_RESULT status of result
*/ 
MOBLE_RESULT Vendor_WriteLocalDataCb(MODEL_MessageHeader_t *pmsgParams,  
                                     MOBLEUINT8 command, 
                                     MOBLEUINT8 const *data, 
                                     MOBLEUINT32 length, 
                                     MOBLEBOOL response)
{
  
  MOBLE_COMMAND_STATUS status = STATUS_SUCCESS;
  
  /* Traces for the Data */
  uint16_t idx = 0;
  TRACE_I(TF_VENDOR_M, 
          "Vendor_WriteLocalDataCb: elementIndex=[%02x], peer_addr=[%02x], dst_peer=[%02x], command=[%02x], Response=[%02x]\n\r", 
          pmsgParams->elementIndex,
          pmsgParams->peer_addr, 
          pmsgParams->dst_peer, 
          command, 
          response);
  
  TRACE_I(TF_VENDOR_M, "DATA_RECEIVED length = %ld\n\r",length);
  
         for (idx=0; idx<length; idx++)
         {
           TRACE_I(TF_VENDOR_M,"data[%d]= %d",idx,data[idx]);  
           TRACE_I(TF_VENDOR_M,"\n\r");
         }
         
           switch (command)
           {
             
           case APPLI_TEST_CMD:
             {
               /*This is callback when ever command is coming for test of response
                 time,command reached count, data byte sent
               */          
               VendorAppli_cb.TestCommand_cb(data, length);
               break;
             }    
             
           case APPLI_LED_CONTROL_STATUS_CMD:
             {
               /* 
               Message Received     B0     B1    B2      B3    B4    B5    B6     B7 
               B0 - Sub-Cmd LED
               B1-B7 - Data Bytes       
               */          
               VendorAppli_cb.LEDControlCommand_Cb(data, length, pmsgParams->elementIndex,\
                 pmsgParams->dst_peer);
               break;
             }
           case APPLI_DATA_CNTRL_CMD:
             {
               /*This is callback when ever command is coming for test of response
                 time,command reached count, data byte sent
               */          
               VendorAppli_cb.DataControlCommand_cb(data,length);    
               break;       
             } 
             /* Default case - Not valid command */
           default:
             {
               status = STATUS_INVALID_COMMAND;
               break;
             }
           }
                   
         /* this expression is used to send the status of data received whether it
         is correct or not , First 3 bits are for status and last 5 bit are for sub commands
         */
         ResponseBuffer[0] = ResponseBuffer[0] | (status << 5);
                 
         /* 
         If response is true, Message response will be sent      
         Message Response     B0     B1    B2      B3    B4    B5    B6     B7 
         B0 - Sub-Cmd 
         B1-B7 - Response Bytes if any 
         */
         if (response == MOBLE_TRUE)
         {
           if (ADDRESS_IS_UNICAST(pmsgParams->dst_peer))
           {
             VendorModel_SendResponse(VENDOR_STMICRO_CID, pmsgParams, command, ResponseBuffer, BuffLength);
             TRACE_M(TF_VENDOR,"Sending Response for Unicast \n\r");
           }
           else
           {
             /* Randomize responses*/
             BLEMesh_ModelsDelayPacket(pmsgParams, command, ResponseBuffer, BuffLength);
             TRACE_M(TF_VENDOR,"Sending Response for Multicast \n\r");
           }		
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
* *pmsgParam Pointer to structure of message header for parameters:
*            elementIndex, src, dst addresses, TTL, RSSI, NetKey & AppKey Offset
* @param  command: Receiving command code 
* @param  data: Pointer to the data
* @param  length: Length of the data
* @param  response: Response Enable. For future use
* @retval MOBLE_RESULT status of result
*/
MOBLE_RESULT Vendor_ReadLocalDataCb(MODEL_MessageHeader_t *pmsgParams,  
                                   MOBLEUINT8 command, 
                                   MOBLEUINT8 const *data, 
                                   MOBLEUINT32 length, 
                                   MOBLEBOOL response)
{  
 
 MOBLE_RESULT status = MOBLE_RESULT_SUCCESS;
 MOBLEUINT8 getBuff[5];
 
 /* Traces for the Data */
  TRACE_M(TF_VENDOR_M,
          "Vendor_ReadLocalDataCb: elementIndex=[%02x], peer_addr=[%02x], dst_peer_addr=[%02x], command=[%02x], Response=[%02x]\n\r", 
          pmsgParams->elementIndex,
          pmsgParams->peer_addr, 
          pmsgParams->dst_peer, 
          command, 
          response);
  
  TRACE_I(TF_VENDOR_M,"DATA_RECEIVED length = %ld\n\r",length);
  
        for (MOBLEUINT8 idx=0; idx<length; idx++)
        {
          TRACE_I(TF_VENDOR_M,"data[%d]= %d",idx,data[idx]); 
          TRACE_I(TF_VENDOR_M,"\n\r");
        }
        
          /* Check which command is in packet */
          switch(command)
          {
          case APPLI_TEST_CMD:
            {
              /* 
              Message Received     B0     B1    B2      B3    B4    B5    B6     B7 
              B0 - Sub-Cmd for APPLI_TEST_CMD
              B1-B7 - Data Bytes if any  
              this function gives back the number count of command received by 
              the receiver.
              */
              VendorAppli_cb.GetTestCount(getBuff);
              ResponseBuffer[0] = APPLI_TEST_INC_COUNTER;
              ResponseBuffer[1] = getBuff[0];
              ResponseBuffer[2] = getBuff[1];
              ResponseBuffer[3] = getBuff[2];
              ResponseBuffer[4] = getBuff[3];
              BuffLength = 5;
              break;
            }
          case APPLI_DEVICE_INFO_CMD:     
            {
              /*
              Message Received     B0     B1    B2      B3    B4    B5    B6     B7 
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
              Message Received with Command APPLI_ELEMENT_TYPE_CMD
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
              Message Received with Command APPLI_LED_CONTROL_STATUS_CMD
              ResponseBuffer will send the response of selected Element
              */
      
      TRACE_M(TF_VENDOR,"Appli_LED_Control_Status callback received for elementIndex %d \r\n", pmsgParams->elementIndex);     
      /*User Need to send the status as per the element selected*/
                ResponseBuffer[0] = Appli_LedState;
              BuffLength = 1;  
              
              break;
            }
          case APPLI_DATA_CNTRL_CMD:
            {
              /* 
              Message Received with Command APPLI_DATA_CNTRL_CMD
              Call the Data Read Callback to fill Response Buffer with local data
              */ 
              status =  VendorAppli_cb.DataControlCommand_cb(data, length);
              break;       
            }  
            
          default:
            {
              status = MOBLE_RESULT_INVALIDARG;
              break;
            }   
            
          }
  
        /* this expression is used to send the status of data received whether it
        is correct or not , First 3 bits are for status and last 5 bit are for sub commands
        */
        ResponseBuffer[0] = ResponseBuffer[0] | (status << 5);
        /* Check if the command executed successfully or not */ 
        
        if (MOBLE_RESULT_SUCCESS == status)
        {
          
          /* 
          Read Command will always be reliable      
          Message Response     B0     B1    B2      B3    B4    B5    B6     B7 
          B0 - Sub-Cmd for which response is needed
          B1-B7 - Data Bytes if any 
          */
         if (ADDRESS_IS_UNICAST(pmsgParams->dst_peer))
          {
            VendorModel_SendResponse(VENDOR_STMICRO_CID, pmsgParams, command, ResponseBuffer, BuffLength);
          }
          else
          {
            /* Randomize responses*/
            BLEMesh_ModelsDelayPacket(pmsgParams, command, ResponseBuffer, BuffLength);
          }
        }
        /* Command not successfully processed */
        else
        {
          if (ADDRESS_IS_UNICAST(pmsgParams->dst_peer))
          {
            VendorModel_SendResponse(VENDOR_STMICRO_CID, pmsgParams, command, ResponseBuffer, 1);
          }
          else
          {
            /* Randomize responses*/
            BLEMesh_ModelsDelayPacket(pmsgParams, command, ResponseBuffer, 1);
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
* *pmsgParam Pointer to structure of message header for parameters:
*             elementIndex, src, dst addresses, TTL, RSSI, NetKey & AppKey Offset
* @param  data: Pointer to the data
* @param  length: Length of the data
* @retval MOBLE_RESULT status of result
*/  
MOBLE_RESULT Vendor_OnResponseDataCb(MODEL_MessageHeader_t *pmsgParam,  
                                     MOBLEUINT8 command, 
                                     MOBLEUINT8 const *pRxData, 
                                     MOBLEUINT32 dataLength, 
                                     MOBLEBOOL response)
{
   MOBLEUINT32 timeStampRcv;
   MOBLEUINT8 subCmd = pRxData[0];
   MOBLEUINT16 hitcmdcount = 0;
   MOBLEUINT8 increment = 1;
   MOBLEUINT8 idx;
  /* Traces for the Data */
  TRACE_I(TF_VENDOR_M,
          "Vendor_OnResponseDataCb: elementIndex=[%02x], peer_addr=[%02x], dst_peer_addr=[%02x], command=[%02x], Response=[%02x]\n\r", 
          pmsgParam->elementIndex,
          pmsgParam->peer_addr, 
          pmsgParam->dst_peer, 
          command, 
          response);
  TRACE_I(TF_VENDOR_M,"DATA_RECEIVED length = %ld\n\r", dataLength);
  TRACE_M(TF_VENDOR_M,"#%02hx-%02hx! \n\r",command,pRxData[0]);
  
  switch(command)
  {
    case APPLI_TEST_CMD:
      {
         switch(subCmd)
         {             
             case APPLI_TEST_ECHO: 
               { 
                 /* this case is responsible for printing of the received back data byte from 
                    receiver node.
                 */       
                 for(idx =1;idx<dataLength;idx++)
                 {
                   TRACE_I(TF_VENDOR_M,"Response data %d\n\r", pRxData[idx]);
                 }
                 break;
               }
             case APPLI_TEST_RANDOMIZATION_RANGE:  
               {
                 /*Insert Test related Commands here*/
                 break;
               }
             case APPLI_TEST_COUNTER:
               { 
         /* this case is responsible for the printing the time stamp when the response 
            is received by the sender node.
         */
                 timeStampRcv = Clock_Time();

                 Packet_ResponseTimeStamp(timeStampRcv);
                  
                 /*Insert Test related Commands here*/
                 break;
               }
             case APPLI_TEST_INC_COUNTER: 
               { 
           /* This case is responsible for the printing of the number count of command 
              received by receiver.
           */     
                 hitcmdcount = (MOBLEUINT32)(pRxData[4] << 24);
                 hitcmdcount |=(MOBLEUINT32)( pRxData[3] << 16);
                 hitcmdcount |=(MOBLEUINT32)( pRxData[2] << 8);
                 hitcmdcount |=(MOBLEUINT32)( pRxData[1]);                
                 TRACE_I(TF_VENDOR_M,"NUMBER OF COMMANDS RECEIVED %d \r\n",hitcmdcount);
                 /*Insert Test related Commands here*/
                 break;
               }
             case APPLI_MODEL_PUBLISH_SELECT:
               {
                  
                 break;
               }
             default:
               {              
                  break;
               }
         }
         break;
      }                  
  case APPLI_LED_CONTROL_STATUS_CMD:
    {
      TRACE_I(TF_VENDOR_M,"Led State %d \r\n",pRxData[0]);
      break;
    }
   case APPLI_DEVICE_INFO_CMD:
     {
      switch(subCmd)
         {             
             case IC_TYPE: 
               { 
                 TRACE_I(TF_VENDOR_M,"IC type is %s \r\n" ,Board_Type[pRxData[1]-1]); 
                 break;
               }
         case LIB_VER:
           {
             while(increment!=8)
             {
               TRACE_I(TF_VENDOR_M,"Lib Version is %d \r\n",pRxData[increment]);
               increment++;
             }
             increment=1;
             break;
           }
           case LIB_SUB_VER:
             {
               while(increment!=7)
               {
                 if(pRxData[increment] != R_ASCI_CODE)
                 {
                   TRACE_I(TF_VENDOR_M,"Lib Sub Version is %x \r\n",pRxData[increment]);
                 }
                 else
                 {
                   TRACE_I(TF_VENDOR_M,"Lib Sub Version is %c \r\n",pRxData[increment]);
                 }
                  increment++;
               }
             increment=1;
             break;   
             }
         }
      break;
     }
    case APPLI_DATA_CNTRL_CMD:  
      {
        /** Displays data received **/
        for (MOBLEUINT8 idx=0; idx<dataLength; idx++)
        {
          TRACE_I(TF_VENDOR_M,"data[%d]= 0x%x",idx,pRxData[idx]); 
          TRACE_I(TF_VENDOR_M,"\n\r");
        }
        break;
      }
       default:
    {              
        break;
    }
      
  }   
      
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
  
  if (Vendor_SendDataFreqState == 0xFF)
  {
    Appli_Vendor_SendBigDataPacket();
  }
}
                           
                           
/**
* @brief  Publish Command for Vendor Model
* @param  srcAddress: Source Address of Node 
* @retval void
*/          
void Vendor_Publish(MOBLE_ADDRESS srcAddress)
{
  MOBLE_RESULT result = MOBLE_RESULT_SUCCESS;
  /* changes the LED status on other nodes in the network */
  if(CommandStatus == APPLI_CMD_ON)
  {
    AppliBuffer[0] = APPLI_CMD_OFF;
  }
  else
  {
    AppliBuffer[0] = APPLI_CMD_ON;
  }

  result = BLEMesh_SetRemotePublication(VENDORMODEL_STMICRO_ID1, srcAddress,
                                            APPLI_LED_CONTROL_STATUS_CMD, 
                                            AppliBuffer, sizeof(AppliBuffer),
                                            MOBLE_FALSE, MOBLE_TRUE);

   if(result)
   {
     TRACE_I(TF_VENDOR_M,"Publication Error \r\n");
   }

  CommandStatus = AppliBuffer[0];
}
         
/**
* @brief  Test Command with Vendor Model used to calculate the time of packet to
  reach at destination.
* @param  publishAddress: Publish Address of the message 
* @param  elementIndex: index of the element
* @retval void
*/          
void Vendor_TestRemoteData(MOBLE_ADDRESS src ,MOBLE_ADDRESS dst ,MOBLEUINT8 elementIndex)
{

  /* changes the LED status on other nodes in the network */
    MOBLE_RESULT result = MOBLE_RESULT_SUCCESS;
    AppliBuffer[0] = APPLI_TEST_COUNTER;
   
    result = BLEMesh_SetRemotePublication(VENDORMODEL_STMICRO_ID1, src,
                                          APPLI_TEST_CMD, 
                                          AppliBuffer, sizeof(AppliBuffer),
                                          MOBLE_TRUE, MOBLE_TRUE);

     if(result)
     {
          TRACE_I(TF_VENDOR_M,"Publication Error \r\n");
     }

  CommandStatus = AppliBuffer[0];
}
         
/**
* @brief  Test Command with Vendor Model used to calculate the time of packet to
  reach at destination.
* @param  publishAddress: Publish Address of the message 
* @param  elementIndex: index of the element
* @retval void
*/          
void Vendor_TestCounterInc(MOBLE_ADDRESS src ,MOBLE_ADDRESS dst ,MOBLEUINT8 elementIndex)
{

  /* changes the LED status on other nodes in the network */
    MOBLE_RESULT result = MOBLE_RESULT_SUCCESS;
    AppliBuffer[0] = APPLI_TEST_INC_COUNTER;
  
    result = BLEMesh_SetRemotePublication(VENDORMODEL_STMICRO_ID1, src,
                                          APPLI_TEST_CMD, 
                                          AppliBuffer, sizeof(AppliBuffer),
                                          MOBLE_FALSE, MOBLE_TRUE);

    if(result)
    {
      TRACE_I(TF_VENDOR_M,"Publication Error \r\n");
    }
}         
         
/**
* @brief  Set the state of the Vendor Data frequency
* @param  State: New state of sending vendor packet frequency 
* @retval void
*/          
void Vendor_SendDataFreq(MOBLEUINT8 freq)
{
    /* Data to be sent continuously with out any time gap */ 
    if ((freq == 0xFF) || (freq == 0x00))
    {
        Vendor_SendDataFreqState = freq;
    }
    else
    {
        TRACE_I(TF_VENDOR_M, "Invalid Data send Freq parameter \r\n");
    }
}
         
/**
* @brief   GenericModelServer_GetOpcodeTableCb: This function is call-back 
*          from the library to send Model Opcode Table info to library
* @param  MODEL_OpcodeTableParam_t:  Pointer to the Generic Model opcode array 
* @param  length: Pointer to the Length of Generic Model opcode array
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT VendorModel_PID1_GetOpcodeTableCb(const MODEL_OpcodeTableParam_t **data, 
                                                 MOBLEUINT16 *length)
{ 
  return MOBLE_RESULT_SUCCESS;
}


/**
* @brief  VendorModel_PID1_GetStatusRequestCb : This function is call-back 
from the library to send response to the message from peer
* @param *pmsgParam Pointer to structure of message header for parameters:
*         elementIndex, src, dst addresses, TTL, RSSI, NetKey & AppKey Offset
* @param  opcode: Received opcode of the Status message callback
* @param  pResponsedata: Pointer to the buffer to be updated with status
* @param  plength: Pointer to the Length of the data, to be updated by application
* @param  pRxData: Pointer to the data received in packet.
* @param  dataLength: length of the data in packet.
* @param  response: Value to indicate whether message is acknowledged meassage or not.
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT VendorModel_PID1_GetStatusRequestCb(MODEL_MessageHeader_t *pmsgParams,  
                                    MOBLEUINT16 opcode, 
                                    MOBLEUINT8 *pResponsedata, 
                                    MOBLEUINT32 *plength, 
                                    MOBLEUINT8 const *pRxData,
                                    MOBLEUINT32 dataLength,
                                    MOBLEBOOL response)

{
  return MOBLE_RESULT_SUCCESS;    
}

/**
* @brief  VendorModelServer_ProcessMessageCb: This is a callback function from
the library whenever a Generic Model message is received
* @param *pmsgParam Pointer to structure of message header for parameters:
*         elementIndex, src, dst addresses, TTL, RSSI, NetKey & AppKey Offset
* @param  opcode: Received opcode of the Status message callback
* @param  pData: Pointer to the buffer to be updated with status
* @param  length: Length of the parameters received 
* @param  response: if TRUE, the message is an acknowledged message
* @param  pRxData: Pointer to the data received in packet.
* @param  dataLength: length of the data in packet.
* @param  response: Value to indicate whether message is acknowledged meassage or not.
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT VendorModel_PID1_ProcessMessageCb(MODEL_MessageHeader_t *pmsgParams,  
                                                 MOBLEUINT16 opcode, 
                                                 MOBLEUINT8 const *pRxData, 
                                                 MOBLEUINT32 dataLength, 
                                                 MOBLEBOOL response
                                                   )
{
  
  MOBLE_RESULT result = MOBLE_RESULT_SUCCESS;
  MOBLEUINT8 command;
  MOBLEBOOL cmd_response;
    
  /*tClockTime delay_t = Clock_Time();*/
  
  TRACE_I(TF_VENDOR_M, 
          "elementIndex=[0x%02x], dst_peer=[0x%02x], peer_add=[0x%02x], opcode=[0x%02x]\r\n",
          pmsgParams->elementIndex, 
          pmsgParams->dst_peer,
          pmsgParams->peer_addr, 
          opcode);

  /*  opcode: response  : Read-Write           : Command   
                          R/nW : 1 = Read
                                 0 = Write

               0            1                    Response of data
               1            1                    Read Data  
               0            0                    Write with no Response
               1            0                    Write with Response

            #define VENDOR_CMD_RESPONSE                          0x20
            #define VENDOR_CMD_READ_nWRITE                       0x10
  */
  
  command = (MOBLEUINT8) (opcode & 0x0F);
  if (opcode & VENDOR_CMD_RESPONSE)
  {
    cmd_response = MOBLE_TRUE;
  }
  else
  {
    cmd_response = MOBLE_FALSE;
  }
  
    /* Parse the command */
    if( (cmd_response == MOBLE_FALSE) && (opcode & VENDOR_CMD_READ_nWRITE))
    { /* Response Packet is received */
       TRACE_I(TF_VENDOR_M,"Response received from remote node \n\r");

       Vendor_OnResponseDataCb(pmsgParams, command, pRxData, dataLength, MOBLE_FALSE);
    }
    else if ( (cmd_response == MOBLE_TRUE) && (opcode & VENDOR_CMD_READ_nWRITE))
    { /* Read Packet is received */
       TRACE_M(TF_VENDOR_M,"Read Command received   \n\r");
       result = Vendor_ReadLocalDataCb(pmsgParams, command, pRxData, dataLength, MOBLE_TRUE);
    }
    else 
    {  /* Write Commands with or without response received */
       TRACE_M(TF_VENDOR_M,"Command Write received, process the command\n\r");
       result = Vendor_WriteLocalDataCb(pmsgParams, command, pRxData, dataLength, cmd_response );
    }

return result;
}
    
__weak MOBLE_RESULT Packet_ResponseTimeStamp(MOBLEUINT32 rcvTimeStamp)
{
  return MOBLE_RESULT_SUCCESS;
}

/**
* @}
*/

/**
* @}
*/


                           
                           
                           
                           
                           
                           
                           
