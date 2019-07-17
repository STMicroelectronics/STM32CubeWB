/**
  ******************************************************************************
  * @file    appli_mesh.c
  * @author  BLE Mesh Team
  * @brief   User Application file 
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
#include "appli_mesh.h"
#include "types.h"
#include "ble_mesh.h"
#include "ble_hal_aci.h"
#include "ble_hci_le.h"
#include <string.h>
#include "models_if.h"
#include "mesh_cfg.h"

#include "stm32_seq.h"


/** @addtogroup BLE_Mesh
*  @{
*/

/** @addtogroup Application_Callbacks_BLE
*  @{
*/

/* Private define ------------------------------------------------------------*/

/*********** Macros to be modified according to the requirement *************/
#define BOUNCE_THRESHOLD                20U
#define LONG_PRESS_THRESHOLD            1000U
#define MANUAL_UNPROVISION_TIMER        3000U
#define FLASH_ERASE_TIME                100U
#define DISCOVER_TIMER                  10*60*1000 /* 10 minutes */
#define DEFAULT_DELAY_PACKET_FROM         500U
#define DEFAULT_DELAY_PACKET_RANDOM_TIME  500U

/* Private macro -------------------------------------------------------------*/
#define MAX_APPLI_BUFF_SIZE             8 
#define MAX_PENDING_PACKETS_QUE_SIZE    2
#define DATA_BUFFER_LENGTH              8
#define MAX_NUMB_ELEMENTS 3

/**********************Friendship callbacks macros ****************************/
#define FN_CLEARED_REPEAT_REQUEST   1
#define FN_CLEARED_POLL_TIMEOUT     2
#define FN_CLEARED_FRIEND_CLEAR     3
#define LPN_CLEARED_NO_RESPONSE     1

/* Private variables ---------------------------------------------------------*/

enum ButtonState
{
  BS_OFF,
  BS_DEBOUNCE,
  BS_SHORT_PRESS,
  BS_LONG_PRESS
};

enum ButtonState buttonState = BS_OFF;
tClockTime tBounce = 0;
MOBLEUINT8 Appli_LedState = 0;
MOBLEUINT8 ProxyFlag = 0;
MOBLEUINT8 ProvisionFlag = 0;

/*Number Of Elements selected per Node. Maximum Elements supported = 3*/
MOBLEUINT8 NumberOfElements = APPLICATION_NUMBER_OF_ELEMENTS;
/*Select Node as Sniffer, Means able to sniff all the packets*/
MOBLEUINT8 DisableFilter = 0;

#if LOW_POWER_FEATURE
MOBLEINT32 BluenrgMesh_sleepTime;
MOBLEUINT32 SysRefCount;
#endif

MOBLEUINT8 network_conf[0x1000];
MOBLEUINT8 nvm_operation;
MOBLEUINT8 nvm_flash_page;
MOBLEUINT8 provisioning_completion;

/********************* Application configuration **************************/
#if defined(__GNUC__) || defined(__IAR_SYSTEMS_ICC__) || defined(__CC_ARM)
MOBLEUINT8 bdaddr[8];
extern const MOBLEUINT8 _bdaddr[];

#ifdef INTERNAL_UNIQUE_NUMBER_MAC
static void Appli_GetMACfromUniqueNumber(void);
#endif 

extern const char _mobleNvmBase_data[];
const void *mobleNvmBase = (const void *)0x0807E000;
#else
#error "Unknown compiler"
#endif /* __GNUC__ || defined(__IAR_SYSTEMS_ICC__) || defined(__CC_ARM) */

/* Private function prototypes -----------------------------------------------*/
static void Appli_LongButtonPress(void);
static void Appli_ShortButtonPress(void);

/* Private functions ---------------------------------------------------------*/

/************************* Button Control functions ********************/
/**
* @brief  Function calls when a button is pressed for short duration  
* @param  void
* @retval void
*/ 
static void Appli_ShortButtonPress(void)
{
  BLEMesh_ModelsCommand();
}
  
/**
* @brief  Function calls when a button is pressed for Long duration  
* @param  void
* @retval void
*/ 
static void Appli_LongButtonPress(void)
{
  /* User Implementation */
}

/**
* @brief  Updates the button status  
* @param  int isPressed
* @retval void
*/ 
static void Appli_UpdateButtonState(int isPressed)
{
  /* Check for button state */
  switch (buttonState)
  {
    /* Case for Button State off */
  case BS_OFF:
    /* if button is pressed */
    if (isPressed)
    {
      buttonState = BS_DEBOUNCE;
      tBounce = Clock_Time();
    }
    break;
    
    /* Case for Button Debouncing */
  case BS_DEBOUNCE:
    if (isPressed)
    {
      /* Debouncing Delay check */
      if (Clock_Time() - tBounce > BOUNCE_THRESHOLD)
      {   
        if (BSP_PB_GetState(BUTTON_SW1) == BUTTON_PRESSED)    
          buttonState = BS_SHORT_PRESS;
      }
      else
      {
        break;
      }
    }
    else
    {
      buttonState = BS_OFF;
      break;
    }
    /* Case if Button is pressed for small duration */
  case BS_SHORT_PRESS:
    if (isPressed && ((Clock_Time() - tBounce) > LONG_PRESS_THRESHOLD))
    {
      buttonState = BS_LONG_PRESS;
    }
    else
    {
      if (!isPressed)
      {
        Appli_ShortButtonPress();
      }
      break;
    }
    /* Case if Button is pressed for long duration */
  case BS_LONG_PRESS:
    if (!isPressed)
    {
      Appli_LongButtonPress();
    }
    break;
    /* Default case */
  default:
    buttonState = BS_OFF;
    break;
  }
  if (!isPressed)
  {
    buttonState = BS_OFF;
  }
}

/**
* @brief  task for the BLE MESH, the MESH Models and the Appli processes  
* @param  void
* @retval void
*/ 
static void Appli_Mesh_Process()
{
  BLEMesh_Process();
  BLEMesh_ModelsProcess(); /* Models Processing */
  Appli_Process();
  
  /* Set the task in the scheduler for the next execution */
  UTIL_SEQ_SetTask( 1<<CFG_TASK_MESH_REQ_ID, CFG_SCH_PRIO_0);
}

/************************* LED Control functions ********************/
/**
* @brief  Controls the state of on board LED
* @param  void
* @retval void
*/ 
void Appli_LedCtrl(void)
{
  if(Appli_LedState)
  {
    BSP_LED_On(LED_BLUE);

  }
  else
  {
    BSP_LED_Off(LED_BLUE);

  }
}

/************* BLE-Mesh library callback functions ***************/

/**
* @brief  Blinks the on board LED  
* @param  none
* @retval MOBLE_RESULT status of result
*/ 
MOBLE_RESULT Appli_LedBlink(void)
{
  /* Switch On the LED */
  Appli_LedState = 1;
  Appli_LedCtrl();
  Clock_Wait(500);
  
  /* Switch Off the LED */
  Appli_LedState = 0;
  Appli_LedCtrl();
  Clock_Wait(500);
  
  return MOBLE_RESULT_SUCCESS;
}

/**
* @brief  Callback function Sets the state of the bulb 
* @param  MOBLEUINT16 ctrl which sets the lighting state of LED
* @retval MOBLE_RESULT status of result
*/ 
MOBLE_RESULT Appli_LedStateCtrlCb(MOBLEUINT16 ctrl)
{
  /* Switch On the LED */
  if(ctrl > 0)
  {
    Appli_LedState = 1;
  }
  /* Switch Off the LED */
  else
  {
    Appli_LedState = 0;
  }
  
  Appli_LedCtrl();
  
  return MOBLE_RESULT_SUCCESS;
}

/**
* @brief  initializes BLE Stack    
* @param  none
* @retval MOBLE_RESULT status of result
*/ 
MOBLE_RESULT Appli_BleStackInitCb()
{
  /* BLE Stack initialization */
  /* Not Required in BLE */
  return MOBLE_RESULT_SUCCESS;
}

/**
* @brief  Sets transmission power of RF 
* @param  none
* @retval MOBLE_RESULT status of result
*/ 
MOBLE_RESULT Appli_BleSetTxPowerCb(void)
{
  tBleStatus status;
  /* API to change the Transmission power of BLE Device */
  /* uint8_t en_high_power, uint8_t pa_level
  en_high_power Can be only 0 or 1. Set high power bit on or off.
  pa_level Can be from 0 to 7. Set the PA level value
  */ 
  status = aci_hal_set_tx_power_level(1, 0x18);
  if (status)
    return MOBLE_RESULT_FAIL;
  else
    return MOBLE_RESULT_SUCCESS;
}

/**
* @brief  SET UUID value 
* @param  uuid_prefix_data : Pointer of UUID buffer data
* @retval MOBLE_RESULT status of result
*/ 
MOBLE_RESULT Appli_BleSetUUIDCb(MOBLEUINT8 *uuid_prefix_data)
{
  /* UUID is 128 bits (16 bytes) and can guarantee uniqueness across space and time.
     It can be “Time-based “ UUID or “truly-random or pseudo-random numbers”
 
     In this buffer user need to fill 10 bytes of UUID data. 
     Remaining 6 bytes, library fill this data with BDaddress.

    For example :
        F81D4FAE-7DEC-XBC4-Y12F-17D1AD07A961 (16 bytes)
        <any number> |_|  |_|   <BDAddress>

      X = 1 i.e. “Time Base UUID” 
      X = 4 i.e. “Random Number“
      Y = 4 i.e. Conforming to the current spec 
    For UUID information refer RFC4122
  */
  
  /* copy UUID prefix data*/
  uint8_t uuid_buff[10] = {0xF8,0x1D,0x4F,0xAE,0x7D,0xEC};/*random value by user */

   uuid_buff[6] = 0x4B;  /* 0x4B => 4 : Random */
   uuid_buff[7] = 'S' ;  /* User define value */
   uuid_buff[8] = 0xA1;  /* 0xA1 => A : Conforming to the current Spec */
   uuid_buff[9] = 'T' ;  /* User define value */  

   memcpy(uuid_prefix_data,uuid_buff,sizeof(uuid_buff));
   memcpy(&uuid_prefix_data[10],bdaddr,6);  /* Copy bdaddr to last 6 bytes of UUID */
   return MOBLE_RESULT_SUCCESS;
}


/**
* @brief  SET CID , PID and VID values 
* @param  company_product_info : vendor fill product information in this buffer
* @retval MOBLE_RESULT status of result
*/
MOBLE_RESULT Appli_BleSetProductInfoCB(MOBLEUINT8 *company_product_info)
{
  /* CID : provide 16-bit company identifier assigned by the Bluetooth SIG */
  uint16_t company_id                   = COMPANY_ID;
  
  /* PID : Provide 16-bit vendor-assigned product identifier */
  uint16_t product_id                   = PRODUCT_ID;
  
  /* VID : Provide 16-bit vendor-assigned product version identifier*/
  uint16_t product_version_id           = PRODUCT_VERSION_ID;
  
  memcpy(company_product_info + 0,(void *)&company_id,sizeof(company_id));
  memcpy(company_product_info + 2 ,(void *)&product_id,sizeof(product_id));
  memcpy(company_product_info + 4 ,(void *)&product_version_id,sizeof(product_version_id));
  
  return MOBLE_RESULT_SUCCESS;
}


/**
* @brief  Call back function called when GATT Connection Created  
* @param  none
* @retval none
*/ 
void Appli_BleGattConnectionCompleteCb(void)
{ 
  ProvisionFlag = 1;
  /* Proxy Node, will be called whenever Gatt connection is established */
  ProxyFlag = 1;
//  BSP_LED_On(LED_GREEN);

}

/**
* @brief  Call back function called when GATT Disconnection Complete  
* @param  none
* @retval none
*/ 
void Appli_BleGattDisconnectionCompleteCb(void)
{ 
  ProvisionFlag = 0;
  /* Proxy Node, will be called whenever Gatt disconnected */

  ProxyFlag = 0;
//  BSP_LED_Off(LED_GREEN);

}

/**
* @brief Unprovisioned Node Identification
* @param MOBLEUINT8 data 
* @retval none
*/
void Appli_BleUnprovisionedIdentifyCb(MOBLEUINT8 data)
{
#ifndef DISABLE_TRACES
  printf("Unprovisioned Node Identifier received: %02x\n\r", data);  
#endif   
}

/**
* @brief  Set Number of Elements in a Node
* @param  none
* @retval Sending the value to BLE Mesh Library
*/ 
MOBLEUINT8 Appli_BleSetNumberOfElementsCb(void)
{
  if(NumberOfElements > MAX_NUMB_ELEMENTS)
  {
#ifndef DISABLE_TRACES
    printf("Currently Three Elements per node are supported!\r\n"); 
#endif
    return MAX_NUMB_ELEMENTS;
  }
  else
  {
  return NumberOfElements;
  }
}

/**
* @brief  Sets the Attention Timer Callback function
* @param  none
* @retval MOBLE_RESULT status of result
*/ 
MOBLE_RESULT Appli_BleAttentionTimerCb(void)
{
#ifndef DISABLE_TRACES
  printf("Attention timer callback received \r\n");
#endif
  
  return MOBLE_RESULT_SUCCESS;
}

/**
* @brief  Enable Node as Sniffer to Capture all the packets
* @param  none
* @retval Sending the value to BLE Mesh Library
*/
MOBLEUINT8 Appli_BleDisableFilterCb(void)
{
    return DisableFilter;
}

/**
* @brief  Checks and do Manual Unprovision of board  
* @param  void
* @retval void
*/ 
void Appli_CheckForUnprovision(void)
{
  /* Checks if the User button is pressed or not at the startup */
  if (BSP_PB_GetState(BUTTON_SW1) == BUTTON_PRESSED)
  {
    BSP_LED_On(LED_BLUE);
    tClockTime t = Clock_Time();
    int interrupted = 0;
    
    /*Wait to check if user is pressing the button persistently*/
    while ((Clock_Time() - t) < MANUAL_UNPROVISION_TIMER)
    {
      if (BSP_PB_GetState(BUTTON_SW1) != BUTTON_PRESSED)
      {
        interrupted = 1;
        break;
      }
    }
    /* Unprovision, show that unprovisioning is completed, and 
    wait until user releases button*/
    if (!interrupted)
    {
      BLEMesh_Unprovision();
#ifndef DISABLE_TRACES
      printf("Device is unprovisioned by application \r\n");
#endif
      t = Clock_Time();
      while ((Clock_Time() - t) < FLASH_ERASE_TIME);
      
      while (BSP_PB_GetState(BUTTON_SW1) == BUTTON_PRESSED)
      {
        Appli_LedBlink();
      }
    }
    BSP_LED_Off(LED_BLUE);
  }
  
  /* Register the task for all MESH dedicated processes */
  UTIL_SEQ_RegTask( 1<< CFG_TASK_MESH_REQ_ID, UTIL_SEQ_RFU, Appli_Mesh_Process );
  /* Set the task in the scheduler for the next scheduling */
  UTIL_SEQ_SetTask( 1<<CFG_TASK_MESH_REQ_ID, CFG_SCH_PRIO_0);  
}

/**
* @brief  Application processing 
*         This function should be called in main loop
* @param  void
* @retval void
*/ 
void Appli_Process(void)
{
  Appli_UpdateButtonState(BSP_PB_GetState(BUTTON_SW1) == BUTTON_PRESSED);
}


/**
* @brief  Checks and updates Mac address to generate MAC Address
* @param  void
* @retval MOBLEUINT8 sum return the sum calculated mac
*/ 
int Appli_CheckBdMacAddr(void)
{
  MOBLEUINT8 sum = 239;
  MOBLEUINT8 result = 0;
  
#ifdef EXTERNAL_MAC_ADDR_MGMT
  memcpy(bdaddr, (MOBLEUINT8 *)CFG_ADV_BD_ADDRESS, 7);
  bdaddr[7] = EXTERNAL_MAC_ADDR_MGMT;
#endif
  
#ifdef INTERNAL_UNIQUE_NUMBER_MAC
  aci_hal_read_config_data(0x00, &bdaddr[7], bdaddr); 
  Appli_GetMACfromUniqueNumber();
  bdaddr[7] = INTERNAL_UNIQUE_NUMBER_MAC;
#endif

  for (int i = 0; i < 6; ++i)
  {
    sum = (sum << 1) + bdaddr[i];
  }

  if (sum == bdaddr[6])
  {
    result = 1;
  }
  
#ifdef GENERATE_STATIC_RANDOM_MAC
  bdaddr[5] |= 0xC0;    /* 2 Msb bits shall be 11b for Static Random Address */
  
  bdaddr[7] = GENERATE_STATIC_RANDOM_MAC;   
                      /* Do nothing for bdaddr, just pass the identification */
  result = 1;         /* This will overwrite the above for loop result, 
                          which is redundant for this case */
#endif  
  
  return result;
}


/**
* @brief  Reads the unique serial number of the device
* @param  void
* @retval void
*/
#ifdef INTERNAL_UNIQUE_NUMBER_MAC
static void Appli_GetMACfromUniqueNumber(void)
{
#ifdef STM32L152xE
  /************************STM32L152RE 96 bit Unique ID addresses***************/
#define DEVICE_ID_REG0 (0x1FF800D0)
#define DEVICE_ID_REG1 (0x1FF800D4)
#define DEVICE_ID_REG2 (0x1FF800E4)
#endif
  
  /*
  The unique serial number is a six bytes value generated from STM32 UID 
  stored at DEVICE_ID_REG0, DEVICE_ID_REG1 and DEVICE_ID_REG2. It is generated 
  cancating 4bytes from DEVICE_ID_REG2 and first bytes of DEVICE_ID_REG0 and 
  DEVICE_ID_REG1. Since the UID at DEVICE_ID_REG0 and 
  DEVICE_ID_REG1 could be identical for same batch samples, 
  4 bytes have been taken from DEVICE_ID_REG2
  */
  
  uint8_t i;
  uint8_t sum = 239;
#if 0
  uint8_t *ls_addr;
  
  ls_addr = (uint8_t*)DEVICE_ID_REG2;
  
  for(i = 0; i < 4; i += 1) 
  {
    bdaddr[i] = *ls_addr;
    ls_addr++;
  }
  
  bdaddr[4] = *(uint8_t*)(DEVICE_ID_REG0);
  bdaddr[5] = (*(uint8_t*)(DEVICE_ID_REG1)) | 0xC0; 
#endif
  /* Static Device Address: 
  The two most significant bits 
  of the address shall be equal to 1, 
  hence ORed with 0xC0)*/
  
  for (i = 0; i < 6; ++i)
  {
    sum = (sum << 1) + bdaddr[i];
  }
  bdaddr[6] = sum;
}
#endif


/**
* @brief  callback for unprovision the node by provisioner.
* @param  status reserved for future 
* @retval void
*/
void BLEMesh_UnprovisionCallback(MOBLEUINT8 status)
{
  ProvisionFlag = 0; 

#ifndef DISABLE_TRACES
  printf("Device is unprovisioned by provisioner \n\r");
#endif
}

/**
* @brief  callback for provision the node by provisioner.
* @param  void
* @retval void
*/
void BLEMesh_ProvisionCallback(void)
{
  ProvisionFlag = 1;
#ifndef DISABLE_TRACES
  printf("Device is provisioned by provisioner \r\n");
#endif
}

/**
* @brief  callback for friendship established by friend node
* @param  address of corresponding low power node
* @param  receive delay of low power node (unit ms)
* @param  poll timeout of low power node (unit 100ms)
* @param  number of elements of low power node
* @param  previous friend address of low power node (can be invalid address)
* @retval void
*/
void BLEMesh_FnFriendshipEstablishedCallback(MOBLE_ADDRESS lpnAddress,
                                             MOBLEUINT8 lpnReceiveDelay,
                                             MOBLEUINT32 lpnPollTimeout,
                                             MOBLEUINT8 lpnNumElements,
                                             MOBLE_ADDRESS lpnPrevFriendAddress)
{ 
  TRACE_M(TF_LPN_FRND,"Friendship established. Low power node address 0x%.4X \r\n", lpnAddress);
  TRACE_M(TF_LPN_FRND,"Low power node receive delay %dms \r\n", lpnReceiveDelay);
  TRACE_M(TF_LPN_FRND,"Low power node poll timeout %dms \r\n", lpnPollTimeout*100);
  TRACE_M(TF_LPN_FRND,"Low power node number of elements %d \r\n", lpnNumElements);
  if (lpnPrevFriendAddress != MOBLE_ADDRESS_UNASSIGNED)
  {
    TRACE_M(TF_LPN_FRND,"Low power node previous friend address 0x%.4X \r\n", lpnPrevFriendAddress);
  }
}

/**
* @brief  callback for friendship clear by friend node
* @param  reason of friendship clear
*         0: reserved,
*         1: friend request received from existing low power node (friend)
*         2: low power node poll timeout occurred
*         3: friend clear received
* @param  previous friend address of low power node (can be invalid address)
* @retval void
*/
void BLEMesh_FnFriendshipClearedCallback(MOBLEUINT8 reason, MOBLE_ADDRESS lpnAddress)
{
  TRACE_M(TF_LPN_FRND,"Friendship cleared. Low power node address 0x%.4X \r\n", lpnAddress);
  
  switch(reason)
  {
  case FN_CLEARED_REPEAT_REQUEST: 
    TRACE_M(TF_LPN_FRND,"Reason: New friend request received from existing low power node \r\n");
    break;
  case FN_CLEARED_POLL_TIMEOUT:
    TRACE_M(TF_LPN_FRND,"Reason: Low power node poll timeout occurred \r\n");
    break;
  case FN_CLEARED_FRIEND_CLEAR:
    TRACE_M(TF_LPN_FRND,"Reason: Friend clear received \r\n");
    break;
  default:
    TRACE_M(TF_LPN_FRND,"Reason: Invalid \r\n");
    break;
  }
}

/**
* @brief  callback for friendship established by low power node
* @param  address of corresponding friend node
* @retval void
*/
void BLEMesh_LpnFriendshipEstablishedCallback(MOBLE_ADDRESS fnAddress)
{
  /* Friendship established */
}

/**
* @brief  callback for friendship cleare by low power node
* @param  reason of friendship clear.
*         0: reserved
*         1: No response received from friend node
* @retval void
*/
void BLEMesh_LpnFriendshipClearedCallback(MOBLEUINT8 reason, MOBLE_ADDRESS fnAddress)
{ 
  TRACE_M(TF_LPN_FRND,"Friendship cleared. Friend node address 0x%.4x \r\n", fnAddress);
  
  if (reason == LPN_CLEARED_NO_RESPONSE)
  {
    TRACE_M(TF_LPN_FRND,"Reason: Friend node not responding \r\n");
  }
  else
  {
    TRACE_M(TF_LPN_FRND,"Reason: Invalid \r\n");
  } 
}

/** 
* @brief  New neighbor appeared callback in neighbor table.
          If MAC address of neighbor changes it appears as new neighbor.
* @param  MAC address of neighbor.
*         is neighbor provisioned or unprovisioned device.
*         uuid of neighbor. NULL if not available
*         network address of neighbor. MOBLE_ADDRESS_UNASSIGNED if not available
*         last updated rssi value.
* @retval void
*/
void BLEMesh_NeighborAppearedCallback(const MOBLEUINT8* bdAddr,
                                          MOBLEBOOL provisioned,
                                          const MOBLEUINT8* uuid,
                                          MOBLE_ADDRESS networkAddress,
                                          MOBLEINT8 rssi)
{
  TRACE_M(TF_NEIGHBOUR,"New neighbor appeared. Neighbor MAC address:");
  
  for (MOBLEUINT8 count=0 ; count<6; count++)
  {
    TRACE_M(TF_NEIGHBOUR,"%.2x ", bdAddr[count]);
  }
  
  if (provisioned == MOBLE_TRUE)
  {
    TRACE_M(TF_NEIGHBOUR,"-> Provisioned node. \n\r");
  }
  else
  {
    TRACE_M(TF_NEIGHBOUR,"-> Unprovisioned device. \n\r");
  }
  
  TRACE_M(TF_NEIGHBOUR,"rssi: %d. ", rssi);
  
  if (networkAddress != MOBLE_ADDRESS_UNASSIGNED)
  {
    TRACE_M(TF_NEIGHBOUR,"Network address: 0x%.4x\n\r", networkAddress);
  }
  else
  {
    TRACE_M(TF_NEIGHBOUR,"Network address not available\n\r");
  }

  for (MOBLEUINT8 i=0; i<16; i++)
  {
    if (uuid[i] == 0x00)
    {
      continue;
    }
    else
    {
      TRACE_M(TF_NEIGHBOUR,"UUID: ");
      
      for (MOBLEUINT8 count=0 ; count<16; count++)
      {
        TRACE_M(TF_NEIGHBOUR,"%.2x ", uuid[count]);
      }
      
      break;
    }
  }
  
  TRACE_M(TF_NEIGHBOUR,"\n\r");
}

/** 
* @brief  Existing neighbor refreshed callback in neighbor table.
          If MAC address of neighbor changes it appears as new neighbor.
* @param  MAC address of neighbor.
*         is neighbor provisioned or unprovisioned device.
*         uuid of neighbor. NULL if not available
*         network address of neighbor. MOBLE_ADDRESS_UNASSIGNED if not available
*         last updated rssi value.
* @retval void
*/
void BLEMesh_NeighborRefreshedCallback(const MOBLEUINT8* bdAddr,
                                          MOBLEBOOL provisioned,
                                          const MOBLEUINT8* uuid,
                                          MOBLE_ADDRESS networkAddress,
                                          MOBLEINT8 rssi)
{
  TRACE_M(TF_NEIGHBOUR,"Existing neighbor refreshed. Neighbor MAC address:");
  
  for (MOBLEUINT8 count=0 ; count<6; count++)
  {
    TRACE_M(TF_NEIGHBOUR,"%.2x ", bdAddr[count]);
  }
  
  if (provisioned == MOBLE_TRUE)
  {
    TRACE_M(TF_NEIGHBOUR,"-> Provisioned node. \n\r");
  }
  else
  {
    TRACE_M(TF_NEIGHBOUR,"-> Unprovisioned device. \n\r");
  }
  
  TRACE_M(TF_NEIGHBOUR,"rssi: %d. ", rssi);
  
  if (networkAddress != MOBLE_ADDRESS_UNASSIGNED)
  {
    TRACE_M(TF_NEIGHBOUR,"Network address: 0x%.4x\n\r", networkAddress);
  }
  else
  {
    TRACE_M(TF_NEIGHBOUR,"Network address not available\n\r");
  }
  
  for (MOBLEUINT8 i=0; i<16; i++)
  {
    if (uuid[i] == 0x00)
    {
      continue;
    }
    else
    {
      TRACE_M(TF_NEIGHBOUR,"UUID: ");
      
      for (MOBLEUINT8 count=0 ; count<16; count++)
      {
        TRACE_M(TF_NEIGHBOUR,"%.2x ", uuid[count]);
      }
      
      break;
    }
  }
  
  TRACE_M(TF_NEIGHBOUR,"\n\r");
}

/**
* @}
*/

/**
* @}
*/
/******************* (C) COPYRIGHT 2019 STMicroelectronics *****END OF FILE****/
