/**
******************************************************************************
* @file   appli_test.c
* @author  BLE Mesh Team
* @brief   Serial Interface file 
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
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include "hal_common.h"
#include "mesh_cfg.h"
#include "common.h"
#include "appli_test.h"
#include <string.h>
#if ENABLE_SERIAL_CONTROL
#include "serial_ctrl.h"
#endif
#if ENABLE_UT
#include "serial_ut.h"
#endif

/** @addtogroup BlueNRG_Mesh
*  @{
*/

/** @addtogroup Application
*  @{
*/

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
#define CMD_INDEX_RES_01                  1
#define CMD_INDEX_RES_02                  2
#define CMD_INDEX_RES_03                  3
#define CMD_INDEX_RES_04                  4

#define CMD_SET_OFFSET                    7
#define CMD_RES_OFFSET                    5
#define CMD_RES_COUNT                     4

/* Private variables ---------------------------------------------------------*/
MOBLEUINT8 TestNumber = 0;
MOBLEUINT32 TestCount = 0;
MOBLEUINT32 RecvCount = 0;
MOBLEUINT32 Totaltest = 0;
/* Private function prototypes -----------------------------------------------*/
static MOBLE_RESULT SerialResponse_doubleHexToHex(MOBLEUINT8* hexArray,MOBLEUINT8* outputArray, MOBLEUINT8 length);
static MOBLEUINT16 SerialResponse_GetFunctionIndex(char *text);
MOBLE_RESULT Read_CommandCount(MOBLE_ADDRESS src ,MOBLE_ADDRESS dst);
MOBLEUINT32 timeStampSend = 0;
MOBLE_ADDRESS srcAddress;
MOBLE_ADDRESS destAddress;
MOBLEUINT8 ReadFlag = 0;
extern MOBLEUINT8 successCounter=0;
extern MOBLEUINT8 sendCounter=0;
/* Private functions ---------------------------------------------------------*/

/**
* @brief  SerialResponse_Process: This function extracts the command and variables from
the received string and passes it to BluenrgMesh library.
* @param  rcvdStringBuff: Pointer to the received ascii character array from the user
* @param  rcvdStringSize: Size of the received array
* @retval void
*/ 
void SerialResponse_Process(char *rcvdStringBuff, uint16_t rcvdStringSize)
{
  MOBLEUINT8 testFunctionParm[10]= {'\0'} ;
  MOBLEUINT8 asciiFunctionParameter[16] = {'\0'} ;
  successCounter = 0;
  sendCounter = 0;
  
    MOBLEUINT16 commandIndex = SerialResponse_GetFunctionIndex(rcvdStringBuff+5);
    if(commandIndex != 0x00)
    {
  sscanf(rcvdStringBuff + CMD_RES_OFFSET + CMD_SET_OFFSET, "%8s %4s %4s ", asciiFunctionParameter, asciiFunctionParameter+8,asciiFunctionParameter+12);   
  /*SerialResponse_doubleHexToHex
  Function will convert the asci string into orinal hex format.
  eg- send-01 12 3456
  return 0x12,0x34,0x56       
  */
        if(MOBLE_RESULT_SUCCESS == SerialResponse_doubleHexToHex(asciiFunctionParameter,testFunctionParm,16))
        { 
  TestCount = (MOBLEUINT32)(testFunctionParm[0] << 24); 
  TestCount |= (MOBLEUINT32)(testFunctionParm[1] << 16);
  TestCount |= (MOBLEUINT32)(testFunctionParm[2] << 8);
  TestCount |= (MOBLEUINT32)(testFunctionParm[3] << 0);
  TestNumber = commandIndex;
  Totaltest = TestCount;
  srcAddress = (MOBLEUINT16)((testFunctionParm[4] << 8) | testFunctionParm[5]);  
  destAddress = (MOBLEUINT16)((testFunctionParm[6] << 8) | testFunctionParm[7]);
}
        else
        {
          TRACE_I(TF_SERIAL_CTRL, "Wrong command typed \n\r");
        }
    }
    else
    {
      TRACE_I(TF_SERIAL_CTRL, "Wrong command typed \n\r");
    }
}

/**
* @brief  SerialResponse_GetFunctionIndex: This function returns the calculated index 
of the command received by the user
* @param  rcvdStringBuff: Pointer to the received ascii character array from the user
* @param  rcvdStringSize: Size of the received array
* @retval MOBLEUINT16
*/  
static MOBLEUINT16 SerialResponse_GetFunctionIndex(char *text)
{
  MOBLEINT16 index = 0;
  
  if (!strncmp(text, "SET-",4))
  {   
    sscanf(text, "SET-%hd", &index);
    index = (index<=CMD_RES_COUNT)? index : 0;
    
  }
  else
  {
    return 0x00;
  }
  
  return index;
}

/**
* @brief  SerialResponse_doubleHexToHex: This function converts two 4-bit hex integers 
to one 8-bit hex integer 
* @param  hexArray: Pointer to input hex array 
* @param  outputArray: Pointer to output hex array 
* @param  length: length of the input hex array
* @retval void
*/ 
static MOBLE_RESULT SerialResponse_doubleHexToHex(MOBLEUINT8* hexArray,MOBLEUINT8* outputArray, MOBLEUINT8 length)
{
  MOBLE_RESULT status = MOBLE_RESULT_SUCCESS;
  MOBLEUINT8 counter =0,position = 0;
  MOBLEUINT8 msb = 0,lsb = 0;
  while (counter <length)
  {
    msb = Serial_CharToHexConvert(hexArray[counter]);
    lsb = Serial_CharToHexConvert(hexArray[counter + 1 ]);
    if((msb == 0xFF) && (lsb == 0xFF))
    {
      return MOBLE_RESULT_FALSE;
    }
    outputArray[position] = msb<<4;
    outputArray[position] |= lsb;
    TRACE_I(TF_SERIAL_CTRL, "data scanned is  %02hx \n\r",outputArray[position]);
    counter+=2;
    position++;
  }
  return status;
}

/**
* @brief  Test_ApplicationTest_Set01: This function sends the vendor test 
  command till the testCount become 0.this test is used to calculate the round trip
  time for the packet send and received response.
* @param  testCount: Pointer to the test count value
* @param  src: source address
* @param  dst: destination address
* @retval MOBLE_RESULT
*/
MOBLE_RESULT Test_ApplicationTest_Set01(MOBLEUINT32 testCount ,MOBLE_ADDRESS src ,MOBLE_ADDRESS dst)
{
  MOBLEUINT8 elementIndex = 0;

  /* The function will called until the testcount will not become zero */     
  if(testCount != 0)
  {
    if(processDelay(TEST_1_WAIT_PERIOD) == 0x01)
    {
      /* Taking the timestamp while sending the command for vendor model */       
      timeStampSend = Clock_Time();      
      Vendor_TestRemoteData(src,dst,elementIndex);  

      TestCount--;                       
    }     
  }

  return MOBLE_RESULT_SUCCESS;
}

/**
* @brief  Test_ApplicationTest_Set02: This function sends the vendor test 
  command till the testCount become 0.This function is used to send the bunch of 
  command to the receiver node and then called a function Read_CommandCount to get 
  the number of command received by receiver. 
* @param  testCount: Pointer to the test count value
* @param  src: source address
* @param  dst: destination address
* @retval MOBLE_RESULT
*/
MOBLE_RESULT Test_ApplicationTest_Set02(MOBLEUINT32 testCount ,MOBLE_ADDRESS src ,MOBLE_ADDRESS dst)                                             
{
  MOBLEUINT8 elementIndex = 0;
   
  if(testCount != 0)
  {
    if(processDelay(TEST_2_WAIT_PERIOD) == 0x01)
    {  
      /* Sending the command one after one in defined interval of time */         
      Vendor_TestCounterInc(src,dst,elementIndex);  

      TestCount--;              
      TRACE_I(TF_SERIAL_CTRL, "Sending No Response command  \r\n");

      if(TestCount == 0)
      {
        /* Flag to call the function Read_CommandCount after some duration of time */          
        ReadFlag = 1;
      }
                
   }     
 }
   
 return MOBLE_RESULT_SUCCESS;
}

/**
* @brief  Read_CommandCount: This function read the number of 
  command received by the receiver successfully.
* @param  src: source address of the node
* @param  dst: destination address of the node
* @retval MOBLE_RESULT
*/
MOBLE_RESULT Read_CommandCount(MOBLE_ADDRESS src ,MOBLE_ADDRESS dst)
{
  MOBLEUINT8 readData[2];
  MODEL_MessageHeader_t msgParam;
   
  msgParam.elementIndex = 0;
  msgParam.peer_addr = src;
  msgParam.dst_peer = dst;
  msgParam.ttl = 0;
  msgParam.rssi = 0;
  msgParam.rcvdAppKeyOffset = 0;
  msgParam.rcvdNetKeyOffset = 0;

  readData[0] = APPLI_TEST_INC_COUNTER;

  if(processDelay(TEST_READ_PERIOD) == 0x01)
  {   
    TRACE_I(TF_SERIAL_CTRL, " NUMBER OF COMMANDS SEND     %d \r\n",Totaltest);
 
    BLEMesh_ReadRemoteData(&msgParam,APPLI_TEST_CMD,readData,sizeof(readData));
      
    ReadFlag = 0;
  }     

  return MOBLE_RESULT_SUCCESS;
}

/**
* @brief  Packet_ResponseTimeStamp: This function called in vendor_response
  it calculates all the packet sending ,receiving, average time.this function called
  to take the response received time stamp.
* @param  rcvTimeStamp: Pointer to the response time stamp received
* @retval MOBLE_RESULT
*/
MOBLE_RESULT Packet_ResponseTimeStamp(MOBLEUINT32 rcvTimeStamp)
{
  static MOBLEUINT32 totalTimeStamp = 0;
  MOBLEUINT32 DiffTimeStamp;
  RecvCount ++;
  DiffTimeStamp = rcvTimeStamp - timeStampSend; 
      
  /* Adding the difference time stamp for calculation of average time of packet 
    sending and receiving 
  */ 
  totalTimeStamp = totalTimeStamp + DiffTimeStamp;
  TRACE_I(TF_SERIAL_CTRL, "Response Got                        %d out of %d\r\n", RecvCount, Totaltest);
  TRACE_I(TF_SERIAL_CTRL, "Packet send time stamp              %d \r\n",timeStampSend);
  TRACE_I(TF_SERIAL_CTRL, "Response receive time stamp         %d \r\n",rcvTimeStamp);
  TRACE_I(TF_SERIAL_CTRL, "Round trip Time %d \r\n \r\n",DiffTimeStamp); 

  if(TestCount == 0)
  {   
    totalTimeStamp = totalTimeStamp/Totaltest; 
    TRACE_I(TF_SERIAL_CTRL, "Maximum Wait Time             %d ms\r\n", TEST_1_WAIT_PERIOD);
    TRACE_I(TF_SERIAL_CTRL, "AVERAGE ROUND TRIP TIME       %d \r\n",totalTimeStamp);
    TRACE_I(TF_SERIAL_CTRL, "PACKET SINGLE TRIP TIME       %d \r\n",totalTimeStamp/2);
    totalTimeStamp = 0;
  }
  return MOBLE_RESULT_SUCCESS;
}

/**
* @brief  Test_ApplicationTest_Set03: This function sends the number of byte
  and get back the same data in response.
* @param  timeStamp: Pointer to the response received time stamp
* @param  src: source address of the node
* @param  dst: destination address of the node
* @retval MOBLE_RESULT
*/
MOBLE_RESULT Test_ApplicationTest_Set03(MOBLE_ADDRESS src ,MOBLE_ADDRESS dst)
{
  MOBLEUINT8 txDataArray[DATA_BYTE_SEND] = {0};
  txDataArray[0] = APPLI_TEST_ECHO; 
  MOBLE_RESULT result = MOBLE_RESULT_SUCCESS;
  MOBLEUINT16 idx;

  for (idx=1; idx<DATA_BYTE_SEND; idx++)
      {
    txDataArray[idx] = idx;
    TRACE_I(TF_SERIAL_CTRL,"data[%d]= %d \r\n",idx,txDataArray[idx]);             
      }

  result = BLEMesh_SetRemoteData(dst, 0,
                                 APPLI_TEST_CMD, txDataArray, 
                                 sizeof(txDataArray), MOBLE_TRUE, 
                                 MOBLE_TRUE);


      if(result)
      {
        TRACE_I(TF_SERIAL_CTRL,"Publication Error \r\n");
      }

        TestNumber = 0;
  
  return MOBLE_RESULT_SUCCESS;
}

/**
* @brief  Function used to calculate the delay.
* @param  MOBLEUINT16
* @retval MOBLEUINT8
*/
MOBLEUINT8 processDelay(MOBLEUINT16 waitPeriod)
{
  static MOBLEUINT8 Clockflag = 0;
  static MOBLEUINT32 Check_time;


  if(Clockflag == CLOCK_FLAG_DISABLE)
  {
    Check_time = Clock_Time();
    Clockflag = CLOCK_FLAG_ENABLE;
  } 
  /* The function will called until the testcount will not become zero */     

  if(((Clock_Time()- Check_time) >= waitPeriod))
  {
    Clockflag = CLOCK_FLAG_DISABLE;
    return 0x01;
            
  }
  return 0x00;
}

/**
* @brief  Function used to run the testing function for Packet response time
* @param  void
* @retval void
*/
void Test_Process(void)
{
  if(TestNumber)
  {

    if(TestNumber == CMD_INDEX_RES_01)
    {
      Test_ApplicationTest_Set01(TestCount,srcAddress,destAddress);
    }
    else if(TestNumber == CMD_INDEX_RES_02)
    {
      Test_ApplicationTest_Set02(TestCount,srcAddress,destAddress);
      if(ReadFlag == 1)
      {
        Read_CommandCount(srcAddress , destAddress);     
      }
    }
    else if(TestNumber == CMD_INDEX_RES_03)
    {
      Test_ApplicationTest_Set03(srcAddress,destAddress);
    }
    else
    {
      TRACE_I(TF_SERIAL_CTRL,"Invalid Command\n\r");
    }
  }
}

