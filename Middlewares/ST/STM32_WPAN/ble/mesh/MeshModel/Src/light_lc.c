/**
******************************************************************************
* @file    light_lc.c
* @author  BLE Mesh Team
* @brief   Light Control model middleware file
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
#include "ble_const.h"
#include "hal_common.h"
#include "mesh_cfg.h"
#include "generic.h"
#include "light.h"
#include "models_if.h"
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

/* Private typedef -----------------------------------------------------------*/
/**
  * LC states enum
 */
typedef enum 
{
  OFF = 0,
  STANDBY,
  POST_STANDBY,
  PRE_FADE_ON,
  FADE_ON,
  RUN,
  FADE,
  PROLONG,
  FADE_STANDBY_AUTO,
  PRE_FADE_STANDBY_MANUAL,
  FADE_STANDBY_MANUAL,
} lc_state_e;

/**
  * LC events enum
 */
typedef enum 
{
  NO_EVENT = 0,
  MODE_OFF,
  MODE_ON,
  LIGHT_OFF,
  LIGHT_ON,
  OCCUPANCY_ON,
  AMBIENT_LUX_MODIFIED,
  PROPERTY_MODIFIED,
  TIMER_ON,
  TIMER_TRIGGER,
  TIMER_OFF
} lc_event_e;

/**
  * LC property id structure
 */
typedef struct
{
  MOBLEUINT16 id;
  MOBLEUINT8 size;
  MOBLEUINT8 valueIndex;
} light_lc_propertyId_t;

/**
  * LC Server param structure
 */
typedef struct
{
  /* LC server callback reference */
  const light_lc_cb_t* p_cb;
  /* Reference to LC property Id table */
  const light_lc_propertyId_t* pPropertyId;
  /* LC property value table */
  MOBLEUINT32 propertyValue[LC_PROPERTY_TABLE_COUNT];
  /* LC TID struct */
  tid_param_t lastMsgTidParams;
  /* LC transition struct */
  transition_params_t transitionParams;
  /* Actual value to be multiplied with 0.01 */
  MOBLEUINT32 ambientLuxLevel;
  /* Light Level (Lux) out from state machine */
  MOBLEUINT32 luxLevelOut;
  /* Used to calculate intermediate lux value in transition */
  MOBLEUINT32 initialLightLuxOut;
  /* Used to calculate intermediate lux value in transition */
  MOBLEUINT32 targetLightLuxOut;
  /* Last lightness update tick in PI regulator */
  MOBLEUINT32 piLightnessUpdateTick;
  /* Last internal sum in PI regulator */
  MOBLEUINT16 piLastInternalSum;
  /* Lightness Linear out not regulated with regulator */
  MOBLEUINT16 lightnessLinearUnreg;
  /* Used to calculate intermedicate lightness in transition */
  MOBLEUINT16 initialLightnessOut;
  /* Used to calculate intermediate lightness in transition */
  MOBLEUINT16 targetLightnessOut;
  /* Temporary variable for LC state machine */
  MOBLEUINT16 lcActualSmOut;
  /* LC linear out binded with instance of Light Lightness Linear */
  MOBLEUINT16 lcLinearOut;
  /* LC server element index, configured by mesh_cfg_usr.h */
  MOBLEUINT8 elementIndex;
  /* LC state machine current state */
  lc_state_e state;
  /* LC occupancy mode */
  MOBLEUINT8 occupancyMode;
  /* Temporary variable for LC state machine */
  MOBLEUINT8 updateLcLinearOut;
  /* LC OnOff status publish */
  MOBLEUINT8 publish;
  MOBLEUINT8 res[3];
} lc_param_t;

/* Private define ------------------------------------------------------------*/
#define LC_REGULATOR_ACCURACY_PID_SIZE                         1
#define LC_LIGHTNESS_ON_PID_SIZE                               2
#define LC_LIGHTNESS_PROLONG_PID_SIZE                          2
#define LC_LIGHTNESS_STANDBY_PID_SIZE                          2
#define LC_AMBIENT_LUXLEVEL_ON_PID_SIZE                        3
#define LC_AMBIENT_LUXLEVEL_PROLONG_PID_SIZE                   3
#define LC_AMBIENT_LUXLEVEL_STANDBY_PID_SIZE                   3
#define LC_TIME_FADE_PID_SIZE                                  3
#define LC_TIME_FADE_ON_PID_SIZE                               3
#define LC_TIME_FADE_STANDBY_AUTO_PID_SIZE                     3
#define LC_TIME_FADE_STANDBY_MANUAL_PID_SIZE                   3
#define LC_TIME_OCCUPANCY_DELAY_PID_SIZE                       3
#define LC_TIME_PROLONG_PID_SIZE                               3
#define LC_TIME_RUN_ON_PID_SIZE                                3
#define LC_REGULATOR_KID_PID_SIZE                              4
#define LC_REGULATOR_KIU_PID_SIZE                              4
#define LC_REGULATOR_KPD_PID_SIZE                              4
#define LC_REGULATOR_KPU_PID_SIZE                              4

#define LC_REGULATOR_ACCURACY_PID_INDEX                        0
#define LC_LIGHTNESS_ON_PID_INDEX                              1
#define LC_LIGHTNESS_PROLONG_PID_INDEX                         2
#define LC_LIGHTNESS_STANDBY_PID_INDEX                         3
#define LC_AMBIENT_LUXLEVEL_ON_PID_INDEX                       4
#define LC_AMBIENT_LUXLEVEL_PROLONG_PID_INDEX                  5
#define LC_AMBIENT_LUXLEVEL_STANDBY_PID_INDEX                  6
#define LC_TIME_FADE_PID_INDEX                                 7
#define LC_TIME_FADE_ON_PID_INDEX                              8
#define LC_TIME_FADE_STANDBY_AUTO_PID_INDEX                    9
#define LC_TIME_FADE_STANDBY_MANUAL_PID_INDEX                  10
#define LC_TIME_OCCUPANCY_DELAY_PID_INDEX                      11
#define LC_TIME_PROLONG_PID_INDEX                              12
#define LC_TIME_RUN_ON_PID_INDEX                               13
#define LC_REGULATOR_KID_PID_INDEX                             14
#define LC_REGULATOR_KIU_PID_INDEX                             15
#define LC_REGULATOR_KPD_PID_INDEX                             16
#define LC_REGULATOR_KPU_PID_INDEX                             17

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
#ifdef ENABLE_LIGHT_MODEL_SERVER_LC

lc_param_t* LcServerP = NULL;

MODEL_OpcodeTableParam_t Light_LC_Opcodes_Table[] = {
/* model_id                       opcode                       reliable      min_payload_size max_payload_size response_opcode           min_response_size max_response_size */
 {LIGHT_LC_SERVER_MODEL_ID,       LIGHT_LC_MODE_GET,           MOBLE_TRUE,   0,               0,               LIGHT_LC_MODE_STATUS  ,   1,                1},
 {LIGHT_LC_SERVER_MODEL_ID,       LIGHT_LC_MODE_SET,           MOBLE_TRUE,   1,               1,               LIGHT_LC_MODE_STATUS  ,   1,                1},
 {LIGHT_LC_SERVER_MODEL_ID,       LIGHT_LC_MODE_SET_UNACK,     MOBLE_FALSE,  1,               1,               LIGHT_LC_MODE_STATUS  ,   1,                1},
 {LIGHT_LC_SERVER_MODEL_ID,       LIGHT_LC_MODE_STATUS,        MOBLE_FALSE,  1,               1,               0                     ,   1,                1},
 {LIGHT_LC_SERVER_MODEL_ID,       LIGHT_LC_OM_GET,             MOBLE_TRUE,   0,               0,               LIGHT_LC_OM_STATUS    ,   1,                1},
 {LIGHT_LC_SERVER_MODEL_ID,       LIGHT_LC_OM_SET,             MOBLE_TRUE,   1,               1,               LIGHT_LC_OM_STATUS    ,   1,                1},
 {LIGHT_LC_SERVER_MODEL_ID,       LIGHT_LC_OM_SET_UNACK,       MOBLE_FALSE,  1,               1,               LIGHT_LC_OM_STATUS    ,   1,                1},
 {LIGHT_LC_SERVER_MODEL_ID,       LIGHT_LC_OM_STATUS,          MOBLE_FALSE,  1,               1,               0                     ,   1,                1},
 {LIGHT_LC_SERVER_MODEL_ID,       LIGHT_LC_ON_OFF_GET,         MOBLE_TRUE,   0,               0,               LIGHT_LC_ON_OFF_STATUS,   1,                3},
 {LIGHT_LC_SERVER_MODEL_ID,       LIGHT_LC_ON_OFF_SET,         MOBLE_TRUE,   2,               4,               LIGHT_LC_ON_OFF_STATUS,   1,                3},
 {LIGHT_LC_SERVER_MODEL_ID,       LIGHT_LC_ON_OFF_SET_UNACK,   MOBLE_FALSE,  2,               4,               LIGHT_LC_ON_OFF_STATUS,   1,                3},
 {LIGHT_LC_SERVER_MODEL_ID,       LIGHT_LC_ON_OFF_STATUS,      MOBLE_FALSE,  1,               3,               0                     ,   1,                3},
 {LIGHT_LC_SERVER_MODEL_ID,       SENSOR_STATUS,               MOBLE_FALSE,  0,               65,              0                     ,   1,                1},                                                                                                 
 {LIGHT_LC_SETUP_SERVER_MODEL_ID, LIGHT_LC_PROPERTY_GET,       MOBLE_TRUE,   2,               2,               LIGHT_LC_PROPERTY_STATUS, 2,                10},
 {LIGHT_LC_SETUP_SERVER_MODEL_ID, LIGHT_LC_PROPERTY_SET,       MOBLE_TRUE,   2,               10,              LIGHT_LC_PROPERTY_STATUS, 2,                10},
 {LIGHT_LC_SETUP_SERVER_MODEL_ID, LIGHT_LC_PROPERTY_SET_UNACK, MOBLE_FALSE,  2,               10,              LIGHT_LC_PROPERTY_STATUS, 2,                10},
 {LIGHT_LC_SETUP_SERVER_MODEL_ID, LIGHT_LC_PROPERTY_STATUS,    MOBLE_FALSE,  2,               10,              0                       , 2,                10}, 
 {0}
};

/* 
  Light LC property table
    Property ID
    Property Size - one byte, two byte, three byte or four byte
    Property index within table
  Table is shared for all elements, only property values would be different
*/
const light_lc_propertyId_t LC_PropertyId[LC_PROPERTY_TABLE_COUNT] = 
{
  {
    LIGHT_CONTROL_REGULATOR_ACCURACY_PID,
    LC_REGULATOR_ACCURACY_PID_SIZE,
    LC_REGULATOR_ACCURACY_PID_INDEX
  },
  {
    LIGHT_CONTROL_LIGHTNESS_ON_PID,
    LC_LIGHTNESS_ON_PID_SIZE,
    LC_LIGHTNESS_ON_PID_INDEX
  },
  {
    LIGHT_CONTROL_LIGHTNESS_PROLONG_PID,
    LC_LIGHTNESS_PROLONG_PID_SIZE,
    LC_LIGHTNESS_PROLONG_PID_INDEX
  },
  {
    LIGHT_CONTROL_LIGHTNESS_STANDBY_PID,
    LC_LIGHTNESS_STANDBY_PID_SIZE,
    LC_LIGHTNESS_STANDBY_PID_INDEX
  },
  {
    LIGHT_CONTROL_AMBIENT_LUXLEVEL_ON_PID,
    LC_AMBIENT_LUXLEVEL_ON_PID_SIZE,
    LC_AMBIENT_LUXLEVEL_ON_PID_INDEX
  },
  {
    LIGHT_CONTROL_AMBIENT_LUXLEVEL_PROLONG_PID,
    LC_AMBIENT_LUXLEVEL_PROLONG_PID_SIZE,
    LC_AMBIENT_LUXLEVEL_PROLONG_PID_INDEX
  },
  {
    LIGHT_CONTROL_AMBIENT_LUXLEVEL_STANDBY_PID,
    LC_AMBIENT_LUXLEVEL_STANDBY_PID_SIZE,
    LC_AMBIENT_LUXLEVEL_STANDBY_PID_INDEX
  },
  {
    LIGHT_CONTROL_TIME_FADE_PID,
    LC_TIME_FADE_PID_SIZE,
    LC_TIME_FADE_PID_INDEX
  },
  {
    LIGHT_CONTROL_TIME_FADE_ON_PID,
    LC_TIME_FADE_ON_PID_SIZE,
    LC_TIME_FADE_ON_PID_INDEX
  },
  {
    LIGHT_CONTROL_TIME_FADE_STANDBY_AUTO_PID,
    LC_TIME_FADE_STANDBY_AUTO_PID_SIZE,
    LC_TIME_FADE_STANDBY_AUTO_PID_INDEX
  },
  {
    LIGHT_CONTROL_TIME_FADE_STANDBY_MANUAL_PID,
    LC_TIME_FADE_STANDBY_MANUAL_PID_SIZE,
    LC_TIME_FADE_STANDBY_MANUAL_PID_INDEX
  },
  {
    LIGHT_CONTROL_TIME_OCCUPANCY_DELAY_PID,
    LC_TIME_OCCUPANCY_DELAY_PID_SIZE,
    LC_TIME_OCCUPANCY_DELAY_PID_INDEX
  },
  {
    LIGHT_CONTROL_TIME_PROLONG_PID,
    LC_TIME_PROLONG_PID_SIZE,
    LC_TIME_PROLONG_PID_INDEX
  },
  {
    LIGHT_CONTROL_TIME_RUN_ON_PID,
    LC_TIME_RUN_ON_PID_SIZE,
    LC_TIME_RUN_ON_PID_INDEX
  },
  {
    LIGHT_CONTROL_REGULATOR_KID_PID,
    LC_REGULATOR_KID_PID_SIZE,
    LC_REGULATOR_KID_PID_INDEX
  },
  {
    LIGHT_CONTROL_REGULATOR_KIU_PID,
    LC_REGULATOR_KIU_PID_SIZE,
    LC_REGULATOR_KIU_PID_INDEX
  },
  {
    LIGHT_CONTROL_REGULATOR_KPD_PID,
    LC_REGULATOR_KPD_PID_SIZE,
    LC_REGULATOR_KPD_PID_INDEX
  },
  {
    LIGHT_CONTROL_REGULATOR_KPU_PID,
    LC_REGULATOR_KPU_PID_SIZE,
    LC_REGULATOR_KPU_PID_INDEX
  }
};
#else
lc_param_t* LcServerP = NULL;
const light_lc_propertyId_t* LC_PropertyId = NULL;
const MODEL_OpcodeTableParam_t* Light_LC_Opcodes_Table = NULL;
#endif /* ENABLE_LIGHT_MODEL_SERVER_LC */

/* Private function prototypes -----------------------------------------------*/
static
MOBLE_RESULT Light_LC_ModeGet(lc_param_t* pLcParams,
                              MOBLEUINT8 const *pRxData,
                              MOBLEUINT32 length,
                              MODEL_MessageHeader_t *pmsgParam);
static
MOBLE_RESULT Light_LC_ModeSet(lc_param_t* pLcParams,
                              MOBLEUINT8 const *pRxData,
                              MOBLEUINT32 length,
                              MODEL_MessageHeader_t *pmsgParam);
static
MOBLE_RESULT Light_LC_ModeSetUnack(lc_param_t* pLcParams,
                                   MOBLEUINT8 const *pRxData,
                                   MOBLEUINT32 length,
                                   MODEL_MessageHeader_t *pmsgParam,
                                   MOBLEUINT8 reply);
static
MOBLE_RESULT Light_LC_OmGet(lc_param_t* pLcParams,
                            MOBLEUINT8 const *pRxData,
                            MOBLEUINT32 length,
                            MODEL_MessageHeader_t *pmsgParam);
static
MOBLE_RESULT Light_LC_OmSet(lc_param_t* pLcParams,
                            MOBLEUINT8 const *pRxData,
                            MOBLEUINT32 length,
                            MODEL_MessageHeader_t *pmsgParam);
static
MOBLE_RESULT Light_LC_OmSetUnack(lc_param_t* pLcParams,
                                 MOBLEUINT8 const *pRxData,
                                 MOBLEUINT32 length,
                                 MODEL_MessageHeader_t *pmsgParam,
                                 MOBLEUINT8 reply);
static
MOBLE_RESULT Light_LC_OnOffGet(lc_param_t* pLcParams,
                               MOBLEUINT8 const *pRxData,
                               MOBLEUINT32 length,
                               MODEL_MessageHeader_t *pmsgParam);
static
MOBLE_RESULT Light_LC_OnOffSet(lc_param_t* pLcParams,
                               MOBLEUINT8 const *pRxData,
                               MOBLEUINT32 length,
                               MODEL_MessageHeader_t *pmsgParam);
static
MOBLE_RESULT Light_LC_OnOffSetUnack(lc_param_t* pLcParams,
                                    MOBLEUINT8 const *pRxData,
                                    MOBLEUINT32 length,
                                    MODEL_MessageHeader_t *pmsgParam,
                                    MOBLEUINT8 reply);
static
MOBLE_RESULT Light_LC_PropertyGet(lc_param_t* pLcParams,
                                  MOBLEUINT8 const *pRxData,
                                  MOBLEUINT32 length,
                                  MODEL_MessageHeader_t *pmsgParam);
static
MOBLE_RESULT Light_LC_PropertySet(lc_param_t* pLcParams,
                                  MOBLEUINT8 const *pRxData,
                                  MOBLEUINT32 length,
                                  MODEL_MessageHeader_t *pmsgParam);
static
MOBLE_RESULT Light_LC_PropertySetUnack(lc_param_t* pLcParams,
                                       MOBLEUINT8 const *pRxData,
                                       MOBLEUINT32 length,
                                       MODEL_MessageHeader_t *pmsgParam,
                                       MOBLEUINT8 reply);
static
MOBLE_RESULT Light_LC_Fsm(lc_event_e event,
                          lc_param_t* pLcParams,
                          MOBLEUINT8 optionalParamsOnOffMsg,
                          MOBLEUINT32 delayMsOOnOD,
                          MOBLEUINT32 stepResMsOnOffMsg,
                          MOBLEUINT32 transitionTimeMsOnOffMsg);
static
MOBLE_RESULT Light_LC_PropertyValueGet(const light_lc_propertyId_t* pPropertyIdTable,
                                       MOBLEUINT32* pPropertyValueTable,
                                       MOBLEUINT16 propertyId,
                                       MOBLEUINT8 propertyIndex,
                                       MOBLEUINT32* value);
static
MOBLE_RESULT Light_LC_PropertyValueSet(const light_lc_propertyId_t* pPropertyIdTable,
                                       MOBLEUINT32* pPropertyValueTable,
                                       MOBLEUINT16 propertyId,
                                       MOBLEUINT8 propertySize,
                                       MOBLEUINT32 value);

/* Extern function prototypes ------------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/**
  * @brief  LightLcServer_GetOpcodeTableCb: This function is call-back 
  *         from the library to send Model Opcode Table info to library
  * @param  MODEL_OpcodeTableParam_t:  Pointer to the Generic Model opcode array 
  * @param  length: Pointer to the Length of Generic Model opcode array
  * @retval MOBLE_RESULT
  */
__weak
MOBLE_RESULT LightLcServer_GetOpcodeTableCb(const MODEL_OpcodeTableParam_t **data, 
                                            MOBLEUINT16 *length)
{
  *data = Light_LC_Opcodes_Table;
  *length = sizeof(Light_LC_Opcodes_Table)/sizeof(MODEL_OpcodeTableParam_t);
  return MOBLE_RESULT_SUCCESS;
}


/**
  * @brief  LightLcServer_GetStatusRequestCb : This function is call-back 
  *         from the library to send response to the message from peer
  * @param  pmsgParam: Pointer to structure of message header for parameters:
  *         elementIndex, src, dst addresses, TTL, RSSI, netkey & appkey Offset 
  * @param  opcode: Received opcode of the Status message callback
  * @param  pResponsedata: Pointer to the buffer to be updated with status
  * @param  plength: Pointer to the Length of the data, to be updated by application
  * @param  pRxData: Pointer to the data received in packet.
  * @param  dataLength: length of the data in packet.
  * @param  response: Value to indicate whether message is acknowledged meassage or not.
  * @retval MOBLE_RESULT_SUCCESS
  */
__weak
MOBLE_RESULT LightLcServer_GetStatusRequestCb(MODEL_MessageHeader_t *pmsgParam, 
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
  * @brief  LightLcServer_ProcessMessageCb callback from access layer
  * @param  pmsgParams: message parameters
  * @param  opcode: message opcode
  * @param  pRxData: data received in packet
  * @param  dataLength: length of the data
  * @param  response: not applicable
  * @retval MOBLE_RESULT as set by callee
  */
__weak
MOBLE_RESULT LightLcServer_ProcessMessageCb(MODEL_MessageHeader_t *pmsgParam, 
                                            MOBLEUINT16 opcode, 
                                            MOBLEUINT8 const *pRxData, 
                                            MOBLEUINT32 dataLength, 
                                            MOBLEBOOL response)
{
  MOBLE_RESULT result = MOBLE_RESULT_SUCCESS;
  lc_param_t* pLcParams = LcServerP; /* single instance of LC server supported */
  
  if((pLcParams != NULL) && (pmsgParam->elementIndex == pLcParams->elementIndex))
  {
    result = MOBLE_RESULT_SUCCESS;
  }
  else
  {
    TRACE_M(TF_LIGHT_LC_M, "LC server not supported on element %d", pmsgParam->elementIndex);
    result = MOBLE_RESULT_FAIL;
  }
  
  if(MOBLE_FAILED(result))
  {
    /* TRACE_M(TF_LIGHT_LC_M, "Element with LC server not found\r\n"); */
  }
  else
  {
    switch(opcode)
    {
    case LIGHT_LC_MODE_GET:
      result = Light_LC_ModeGet(pLcParams, pRxData, dataLength, pmsgParam);
      break;
    case LIGHT_LC_MODE_SET:
      result = Light_LC_ModeSet(pLcParams, pRxData, dataLength, pmsgParam);
      break;
    case LIGHT_LC_MODE_SET_UNACK:
      result = Light_LC_ModeSetUnack(pLcParams, pRxData, dataLength, pmsgParam, 0);
      break;
    case LIGHT_LC_OM_GET:
      result = Light_LC_OmGet(pLcParams, pRxData, dataLength, pmsgParam);
      break;
    case LIGHT_LC_OM_SET:
      result = Light_LC_OmSet(pLcParams, pRxData, dataLength, pmsgParam);
      break;
    case LIGHT_LC_OM_SET_UNACK:
      result = Light_LC_OmSetUnack(pLcParams, pRxData, dataLength, pmsgParam, 0);
      break;
    case LIGHT_LC_ON_OFF_GET:
      result = Light_LC_OnOffGet(pLcParams, pRxData, dataLength, pmsgParam);
      break;
    case LIGHT_LC_ON_OFF_SET:
      result = Light_LC_OnOffSet(pLcParams, pRxData, dataLength, pmsgParam);
      break;
    case LIGHT_LC_ON_OFF_SET_UNACK:
      result = Light_LC_OnOffSetUnack(pLcParams, pRxData, dataLength, pmsgParam, 0);
      break;
    case LIGHT_LC_PROPERTY_GET:
      Light_LC_PropertyGet(pLcParams, pRxData, dataLength, pmsgParam);
      break;
    case LIGHT_LC_PROPERTY_SET:
      Light_LC_PropertySet(pLcParams, pRxData, dataLength, pmsgParam);
      break;
    case LIGHT_LC_PROPERTY_SET_UNACK:
      Light_LC_PropertySetUnack(pLcParams, pRxData, dataLength, pmsgParam, 0);
      break;
    case SENSOR_STATUS:
      {
        MOBLEUINT8 propertyID;
        if((pRxData[0] & 0x1) == 0)
        { 
          /* Decode Format A */
          propertyID = pRxData[0] >> 5;
          propertyID |= pRxData[1] << 3;
          
          TRACE_M(TF_LIGHT_LC_M, "Decoded PropertyID: 0x%04x  \r\n", propertyID);
          if(propertyID == PRESENCE_DETECTED_PID)
          {
            if(dataLength > 2)
            {
              Light_LC_SensorPropertyUpdate(pmsgParam->elementIndex,
                                            PRESENCE_DETECTED_PID,
                                            pRxData[2]);
            }
          }
        }
      }
      break;
    default:
      break;
    }
  }
  
  return result;
}


/**
  * @brief  Light_LC_ModeStatus message as per model specification v1.0.1
  *         Sent in response of LC Mode Get, LC Mode Set or LC Mode SetUnack to
  *         publish group
  * @param  Reference to msg parameters
  * @param  LC mode
  * @param  status to be sent only to client or to be published or to do both
  * @retval False if reply or publish not successful else success
  */
static
MOBLE_RESULT Light_LC_ModeStatus(MODEL_MessageHeader_t *pmsgParam,
                                 MOBLEUINT8 mode,
                                 status_send_e statusSend)
{
  MOBLE_RESULT result = MOBLE_RESULT_SUCCESS;
  MOBLEUINT16 dataBufLen = 0;
  MOBLEUINT8 data[1];
  MOBLE_ADDRESS srcElementAddr = BLEMesh_GetAddress() + pmsgParam->elementIndex;
  MOBLE_ADDRESS dstAddr;
  
  TRACE_M(TF_LIGHT_LC_M, "LC mode %d\r\n", mode);
  
  if(srcElementAddr == MOBLE_ADDRESS_UNASSIGNED)
  {
    TRACE_M(TF_LIGHT_LC_M, "Can't send from unassigned source address\r\n");
  }
  
  data[0] = mode;
  dataBufLen = 1;
  
  if (statusSend == STATUS_SEND_REPLY ||
      statusSend == STATUS_SEND_REPLY_PUBLISH)
  {
    dstAddr = pmsgParam->peer_addr;
    
    if (dstAddr != MOBLE_ADDRESS_UNASSIGNED)
    {
      result = BLEMesh_ModelSendMessage(srcElementAddr,
                                            dstAddr,
                                            LIGHT_LC_SERVER_MODEL_ID,
                                            (MOBLEUINT16)LIGHT_LC_MODE_STATUS,
                                            data,
                                            dataBufLen);
      
      if (result == MOBLE_RESULT_SUCCESS)
      {
        TRACE_M(TF_LIGHT_LC_M, "Reply sent\r\n");
      }
      else
      {
        TRACE_M(TF_LIGHT_LC_M, "Reply failed\r\n");
      }
    }
    else
    {
      TRACE_M(TF_LIGHT_LC_M, "Reply can't be sent. Client destination address is unassigned\r\n");
      result = MOBLE_RESULT_FAIL;
    }
  }
  
  if (statusSend == STATUS_SEND_PUBLISH ||
      statusSend == STATUS_SEND_REPLY_PUBLISH)
  {
    result = BLEMesh_SetRemotePublication(LIGHT_LC_SERVER_MODEL_ID,
                                              srcElementAddr,
                                              (MOBLEUINT16)LIGHT_LC_MODE_STATUS,
                                              data,
                                              dataBufLen,
                                              MOBLE_FALSE,
                                              MOBLE_FALSE);
    if (result == MOBLE_RESULT_SUCCESS)
    {
      TRACE_M(TF_LIGHT_LC_M, "Published\r\n");
    }
    else
    {
      TRACE_M(TF_LIGHT_LC_M, "Publication failed\r\n");
    }
  }
  
  return result;
}


/**
  * @brief  Light_LC_OmStatus message as per model specification v1.0.1
  *         Sent in response of LC OM Get, LC OM Set or LC OM SetUnack to
  *         publish group
  * @param  Reference to msg parameters
  * @param  LC occupancy mode
  * @param  status to be sent only to client or to be published or to do both
  * @retval False if reply or publish not successful else success
  */
static
MOBLE_RESULT Light_LC_OmStatus(MODEL_MessageHeader_t *pmsgParam,
                               MOBLEUINT8 occupancyMode,
                               status_send_e statusSend)
{
  MOBLE_RESULT result = MOBLE_RESULT_SUCCESS;
  MOBLEUINT16 dataBufLen = 0;
  MOBLEUINT8 data[1];
  MOBLE_ADDRESS srcElementAddr = BLEMesh_GetAddress() + pmsgParam->elementIndex;
  MOBLE_ADDRESS dstAddr;
  
  TRACE_M(TF_LIGHT_LC_M, "LC occupancy mode %d\r\n", occupancyMode);
  
  if(srcElementAddr == MOBLE_ADDRESS_UNASSIGNED)
  {
    TRACE_M(TF_LIGHT_LC_M, "Can't send from unassigned source address\r\n");
  }
  
  data[0] = occupancyMode;
  dataBufLen = 1;
  
  if (statusSend == STATUS_SEND_REPLY ||
      statusSend == STATUS_SEND_REPLY_PUBLISH)
  {
    dstAddr = pmsgParam->peer_addr;
    
    if (dstAddr != MOBLE_ADDRESS_UNASSIGNED)
    {
      result = BLEMesh_ModelSendMessage(srcElementAddr,
                                            dstAddr,
                                            LIGHT_LC_SERVER_MODEL_ID,
                                            (MOBLEUINT16)LIGHT_LC_OM_STATUS,
                                            data,
                                            dataBufLen);
      if (result == MOBLE_RESULT_SUCCESS)
      {
        TRACE_M(TF_LIGHT_LC_M, "Reply sent\r\n");
      }
      else
      {
        TRACE_M(TF_LIGHT_LC_M, "Reply failed\r\n");
      }
    }
    else
    {
      TRACE_M(TF_LIGHT_LC_M, "Reply can't be sent. Client destination address is unassigned\r\n");
      result = MOBLE_RESULT_FAIL;
    }
  }
  
  if (statusSend == STATUS_SEND_PUBLISH ||
      statusSend == STATUS_SEND_REPLY_PUBLISH)
  {
    result = BLEMesh_SetRemotePublication(LIGHT_LC_SERVER_MODEL_ID,
                                              srcElementAddr,
                                              (MOBLEUINT16)LIGHT_LC_OM_STATUS,
                                              data,
                                              dataBufLen,
                                              MOBLE_FALSE,
                                              MOBLE_FALSE);
    if (result == MOBLE_RESULT_SUCCESS)
    {
      TRACE_M(TF_LIGHT_LC_M, "Published\r\n");
    }
    else
    {
      TRACE_M(TF_LIGHT_LC_M, "Publication failed\r\n");
    }
  }
  
  return result;
}


/**
  * @brief  Light_LC_OnOffStatus message as per model specification v1.0.1
  *         Sent in response of LC OnOff Get, LC OnOff Set or LC OnOff SetUnack to
  *         publish group
  * @param  Reference to LC parameters
  * @param  Reference to msg parameters
  * @param  status to be sent only to client or to be published or to do both
  * @retval False if reply or publish not successful else success
  */
static
MOBLE_RESULT Light_LC_OnOffStatus(lc_param_t* pLcParams,
                                  MODEL_MessageHeader_t *pmsgParam,
                                  status_send_e statusSend)
{
  MOBLE_RESULT result = MOBLE_RESULT_FAIL;
  MOBLEUINT16 dataBufLen = 0;
  MOBLEUINT8 data[3];
  MOBLE_ADDRESS srcElementAddr = BLEMesh_GetAddress() + pmsgParam->elementIndex;
  MOBLE_ADDRESS dstAddr;
  MOBLEUINT8 remTimeStatusMsg = 0;
  MOBLEUINT8 presentOnOff = 0;
  MOBLEUINT8 targetOnOff = 0;
  MOBLEUINT32 remainingTimeMs = 0;
  
  if(srcElementAddr == MOBLE_ADDRESS_UNASSIGNED)
  {
    TRACE_M(TF_LIGHT_LC_M, "Can't send from unassigned source address\r\n");
  }
  
  /* Compute present on off state from LC state */
  if(pLcParams->state == OFF ||
     pLcParams->state == STANDBY ||
     pLcParams->state == POST_STANDBY)
  {
    presentOnOff = 0;
  }
  else
  {
    presentOnOff = 1;
  }
  
  data[0] = presentOnOff;
  dataBufLen = 1;
  
  /* Compute target OnOff state from LC state and remaining time from transition parameters */
  if(pLcParams->state != STANDBY &&
     pLcParams->state != RUN &&
     pLcParams->state != PROLONG)
  {
    if (pLcParams->state == FADE_STANDBY_AUTO ||
        pLcParams->state == FADE_STANDBY_MANUAL)
    {
      targetOnOff = 0;
    }
    else
    {
      targetOnOff = 1;
    }
    
    data[dataBufLen] = targetOnOff;
    dataBufLen += 1;

    remainingTimeMs = Transition_RemainingTimeGet(&pLcParams->transitionParams);
    
    TRACE_M(TF_LIGHT_LC_M, "LC present on off state %d target on off state %d remaining time %ldms\r\n", presentOnOff, targetOnOff, remainingTimeMs);
    
    result = TimeDttFGet(remainingTimeMs, &remTimeStatusMsg);
    if(result != MOBLE_RESULT_SUCCESS)
    {
      TRACE_M(TF_LIGHT_LC_M, "Remaining time %ldms > max possible value\r\n", remainingTimeMs);
    }    
    
    data[dataBufLen] = remTimeStatusMsg;
    dataBufLen += 1;
  }
  else
  {
    /* do nothing */
    TRACE_M(TF_LIGHT_LC_M, "LC present on off state %d\r\n", presentOnOff);
  }
  
  if (statusSend == STATUS_SEND_REPLY ||
      statusSend == STATUS_SEND_REPLY_PUBLISH)
  {
    dstAddr = pmsgParam->peer_addr;
    
    if (dstAddr != MOBLE_ADDRESS_UNASSIGNED)
    {
      result = BLEMesh_ModelSendMessage(srcElementAddr,
                                            dstAddr,
                                            LIGHT_LC_SERVER_MODEL_ID,
                                            (MOBLEUINT16)LIGHT_LC_ON_OFF_STATUS,
                                            data,
                                            dataBufLen);
      if (result == MOBLE_RESULT_SUCCESS)
      {
        TRACE_M(TF_LIGHT_LC_M, "Reply sent\r\n");
      }
      else
      {
        TRACE_M(TF_LIGHT_LC_M, "Reply failed\r\n");
      }
    }
    else
    {
      TRACE_M(TF_LIGHT_LC_M, "Reply can't be sent. Client destination address is unassigned\r\n");
      result = MOBLE_RESULT_FAIL;
    }
  }
  
  if (statusSend == STATUS_SEND_PUBLISH ||
      statusSend == STATUS_SEND_REPLY_PUBLISH)
  {
    result = BLEMesh_SetRemotePublication(LIGHT_LC_SERVER_MODEL_ID,
                                              srcElementAddr,
                                              (MOBLEUINT16)LIGHT_LC_ON_OFF_STATUS,
                                              data,
                                              dataBufLen,
                                              MOBLE_FALSE,
                                              MOBLE_FALSE);
    if (result == MOBLE_RESULT_SUCCESS)
    {
      TRACE_M(TF_LIGHT_LC_M, "Published\r\n");
    }
    else
    {
      TRACE_M(TF_LIGHT_LC_M, "Publication failed\r\n");
    }
  }
  
  return result;
}


/**
  * @brief  Light_LC_OnOffStatus message as per model specification v1.0.1
  *         Sent in response of LC OnOff Get, LC OnOff Set or LC OnOff SetUnack to
  *         publish group
  * @param  Reference to msg parameters
  * @param  Property ID
  * @param  Property size (in bytes)
  * @param  Property value
  * @param  status to be sent only to client or to be published or to do both
  * @retval Success if property size is appropriate, reply is success and publication is success
  */
static
MOBLE_RESULT Light_LC_PropertyStatus(MODEL_MessageHeader_t *pmsgParam,
                                     MOBLEUINT16 propertyId,
                                     MOBLEUINT8 propertySize,
                                     MOBLEUINT32 propertyValue,
                                     status_send_e statusSend)
{
  MOBLE_RESULT result = MOBLE_RESULT_SUCCESS;
  MOBLEUINT16 dataBufLen = 0;
  MOBLEUINT8 data[6];
  MOBLE_ADDRESS srcElementAddr = BLEMesh_GetAddress() + pmsgParam->elementIndex;
  MOBLE_ADDRESS dstAddr;
  
  if(srcElementAddr == MOBLE_ADDRESS_UNASSIGNED)
  {
    TRACE_M(TF_LIGHT_LC_M, "Can't send from unassigned source address\r\n");
  }
  
  TRACE_M(TF_LIGHT_LC_M, "Id %x size %d value %lx\r\n", propertyId, propertySize, propertyValue);
  
  if(propertySize > FOUR_BYTE_VALUE)
  {
    result = MOBLE_RESULT_INVALIDARG;
  }
  
  if(result == MOBLE_RESULT_SUCCESS)
  {
    data[0] = propertyId;
    data[1] = propertyId >> 8;
    dataBufLen = 2;
    
    PutLittleEndian(data+dataBufLen, propertyValue, propertySize);
    dataBufLen += propertySize;

    if (statusSend == STATUS_SEND_REPLY ||
        statusSend == STATUS_SEND_REPLY_PUBLISH)
    {
      dstAddr = pmsgParam->peer_addr;
      
      if (dstAddr != MOBLE_ADDRESS_UNASSIGNED)
      {
        result = BLEMesh_ModelSendMessage(srcElementAddr,
                                              dstAddr,
                                              LIGHT_LC_SETUP_SERVER_MODEL_ID,
                                              (MOBLEUINT16)LIGHT_LC_PROPERTY_STATUS, 
                                              data,
                                              dataBufLen);
        if (result == MOBLE_RESULT_SUCCESS)
        {
          TRACE_M(TF_LIGHT_LC_M, "Reply sent\r\n");
        }
        else
        {
          TRACE_M(TF_LIGHT_LC_M, "Reply failed\r\n");
        }
      }
      else
      {
        TRACE_M(TF_LIGHT_LC_M, "Reply can't be sent. Client destination address is unassigned\r\n");
        result = MOBLE_RESULT_FAIL;
      }
    }
      
    if (statusSend == STATUS_SEND_PUBLISH ||
        statusSend == STATUS_SEND_REPLY_PUBLISH)
    {
      result = BLEMesh_SetRemotePublication(LIGHT_LC_SETUP_SERVER_MODEL_ID,
                                                srcElementAddr,
                                                (MOBLEUINT16)LIGHT_LC_PROPERTY_STATUS,
                                                data,
                                                dataBufLen,
                                                MOBLE_FALSE,
                                                MOBLE_FALSE);
      if (result == MOBLE_RESULT_SUCCESS)
      {
        TRACE_M(TF_LIGHT_LC_M, "Published\r\n");
      }
      else
      {
        TRACE_M(TF_LIGHT_LC_M, "Publication failed\r\n");
      }
    }
  }

  return result;
}


/**
  * @brief  Light_LC_ModeGet callback
  *         response to be sent only to client node and not to pulish group
  * @param  Reference to LC parameters
  * @param  Parameters as set by client
  * @param  Parameters length
  * @param  Reference to message parameters
  * @retval Invalid if length is not appropriate, else as set by status function
  */
static
MOBLE_RESULT Light_LC_ModeGet(lc_param_t* pLcParams,
                              MOBLEUINT8 const *pRxData,
                              MOBLEUINT32 length,
                              MODEL_MessageHeader_t *pmsgParam)
{
  MOBLE_RESULT result = MOBLE_RESULT_SUCCESS;
  MOBLEUINT8 mode;
  
  TRACE_M(TF_LIGHT_LC_M, "length %ld, peer %.4x, dst %.4x, element index %d \r\n",
          length, pmsgParam->peer_addr, pmsgParam->dst_peer, pmsgParam->elementIndex);
  
  if(length == 0)
  {
    if(pLcParams->state == OFF)
    {
      mode = 0;
    }
    else
    {
      mode = 1;
    }
    
    /* Application callback */
    if(pLcParams->p_cb->LightLCs_ModeGet_cb != NULL)
    {
      (pLcParams->p_cb->LightLCs_ModeGet_cb)(pmsgParam);
    }
  }
  else
  {
    result = MOBLE_RESULT_FAIL;
    TRACE_M(TF_LIGHT_LC_M, "Invalid message length %ld\r\n", length);
  }
  
  if(MOBLE_SUCCEEDED(result))
  {
    result = Light_LC_ModeStatus(pmsgParam,
                                 mode,
                                 STATUS_SEND_REPLY);
  }
  
  return result;
}


/**
  * @brief  Light_LC_ModeSet callback
  *         response to be sent to client node and publish to pulish group
  * @param  Reference to LC parameters
  * @param  Parameters as set by client
  * @param  Parameters length
  * @param  Reference to message parameters
  * @retval As set by unack function
  */
static
MOBLE_RESULT Light_LC_ModeSet(lc_param_t* pLcParams,
                              MOBLEUINT8 const *pRxData,
                              MOBLEUINT32 length,
                              MODEL_MessageHeader_t *pmsgParam)
{
  MOBLE_RESULT result = MOBLE_RESULT_FAIL;
  
  TRACE_M(TF_LIGHT_LC_M, "length %ld, peer %.4x, dst %.4x, element index %d \r\n",
          length, pmsgParam->peer_addr, pmsgParam->dst_peer, pmsgParam->elementIndex);
  
  result = Light_LC_ModeSetUnack(pLcParams, pRxData, length, pmsgParam, 1);
  
  if (MOBLE_SUCCEEDED(result))
  {
    /* Application callback */
    if(pLcParams->p_cb->LightLCs_ModeSet_cb != NULL)
    {
      (pLcParams->p_cb->LightLCs_ModeSet_cb)(pRxData, pmsgParam);
    }
  }
  
  return result;
}


/**
  * @brief  Light_LC_ModeSetUnack callback
  *         Publish to pulish group if called by process message
  *         Reply to client as well if called by set function
  * @param  Reference to LC parameters
  * @param  Parameters as set by client
  * @param  Parameters length
  * @param  Reference to message parameters
  * @param  reply to client flag
  * @retval Fail if length is inappropriate else as set by fsm or status function
  */
static
MOBLE_RESULT Light_LC_ModeSetUnack(lc_param_t* pLcParams,
                                   MOBLEUINT8 const *pRxData,
                                   MOBLEUINT32 length,
                                   MODEL_MessageHeader_t *pmsgParam,
                                   MOBLEUINT8 reply)
{
  MOBLEUINT8 mode;
  
  MOBLE_RESULT result = MOBLE_RESULT_FAIL;
  
  if(reply == 0)
  {
    TRACE_M(TF_LIGHT_LC_M, "length %ld, peer %.4x, dst %.4x, element index %d \r\n",
            length, pmsgParam->peer_addr, pmsgParam->dst_peer, pmsgParam->elementIndex);
  }
  
  if(length == 1)
  {
    TRACE_I(TF_LIGHT_LC_M, "LC mode %d\r\n", pRxData[0]);
    
    if(pRxData[0] == 1)
    {
      result = Light_LC_Fsm(MODE_ON, LcServerP, 0, 0, 0, 0);
    }
    else if(pRxData[0] == 0)
    {
      result = Light_LC_Fsm(MODE_OFF, LcServerP, 0, 0, 0, 0);
    }
    else
    {
      TRACE_I(TF_LIGHT_LC_M,"Invalid mode value\r\n");
    }
  }
  else
  {
    result = MOBLE_RESULT_FAIL;
    TRACE_I(TF_LIGHT_LC_M, "Invalid message length %ld\r\n", length);
  }
  
  if (MOBLE_SUCCEEDED(result))
  {
    if(pLcParams->state == OFF)
    {
      mode = 0;
    }
    else
    {
      mode = 1;
    }
    
    /* Application callback */
    if(pLcParams->p_cb->LightLCs_ModeSetUnack_cb != NULL)
    {
      (pLcParams->p_cb->LightLCs_ModeSetUnack_cb)(pRxData, pmsgParam);
    }
    
    if(reply == 0)
    {
      result = Light_LC_ModeStatus(pmsgParam,
                                   mode,
                                   STATUS_SEND_PUBLISH);
    }
    else
    {
      result = Light_LC_ModeStatus(pmsgParam,
                                   mode,
                                   STATUS_SEND_REPLY_PUBLISH);
    }
  }
  
  return result;
}


/**
  * @brief  Light_LC_OmGet callback
  *         response to be sent only to client node and not to pulish group
  * @param  Reference to LC parameters
  * @param  Parameters as set by client
  * @param  Parameters length
  * @param  Reference to message parameters
  * @retval Invalid if length is not appropriate, else as set by status function
  */
static
MOBLE_RESULT Light_LC_OmGet(lc_param_t* pLcParams,
                            MOBLEUINT8 const *pRxData,
                            MOBLEUINT32 length,
                            MODEL_MessageHeader_t *pmsgParam)
{
  MOBLE_RESULT result = MOBLE_RESULT_SUCCESS;
  MOBLEUINT8 occupancyMode = 0;
  
  TRACE_M(TF_LIGHT_LC_M, "length %ld, peer %.4x, dst %.4x, element index %d \r\n",
          length, pmsgParam->peer_addr, pmsgParam->dst_peer, pmsgParam->elementIndex);
  
  if(length == 0)
  {
    occupancyMode = pLcParams->occupancyMode;
  }
  else
  {
    result = MOBLE_RESULT_FAIL;
    TRACE_M(TF_LIGHT_LC_M, "Invalid message length %ld\r\n", length);
  }
  
  if(MOBLE_SUCCEEDED(result))
  {
    /* Application callback */
    if(pLcParams->p_cb->LightLCs_OmGet_cb != NULL)
    {
      (pLcParams->p_cb->LightLCs_OmGet_cb)(pmsgParam);
    }
    
    result = Light_LC_OmStatus(pmsgParam,
                               occupancyMode,
                               STATUS_SEND_REPLY);
  }
  
  return result;
}


/**
  * @brief  Light_LC_OmSet callback
  *         response to be sent to client node and publish to pulish group
  * @param  Reference to LC parameters
  * @param  Parameters as set by client
  * @param  Parameters length
  * @param  Reference to message parameters
  * @retval As set by unack function
  */
static
MOBLE_RESULT Light_LC_OmSet(lc_param_t* pLcParams,
                            MOBLEUINT8 const *pRxData,
                            MOBLEUINT32 length,
                            MODEL_MessageHeader_t *pmsgParam)
{
  MOBLE_RESULT result = MOBLE_RESULT_FAIL;
  
  TRACE_M(TF_LIGHT_LC_M, "length %ld, peer %.4x, dst %.4x, element index %d \r\n",
          length, pmsgParam->peer_addr, pmsgParam->dst_peer, pmsgParam->elementIndex);
  
  result = Light_LC_OmSetUnack(pLcParams, pRxData, length, pmsgParam, 1);
  
  if (MOBLE_SUCCEEDED(result))
  {
    /* Application callback */
    if(pLcParams->p_cb->LightLCs_OmSet_cb != NULL)
    {
      (pLcParams->p_cb->LightLCs_OmSet_cb)(pRxData, pmsgParam);
    }
  }
  
  return result;
}


/**
  * @brief  Light_LC_OmSetUnack callback
  *         Publish to pulish group if called by process message
  *         Reply to client as well if called by set function
  * @param  Reference to LC parameters
  * @param  Parameters as set by client
  * @param  Parameters length
  * @param  Reference to message parameters
  * @param  reply to client flag
  * @retval Fail if length is inappropriate else as set by or status function
  */
static
MOBLE_RESULT Light_LC_OmSetUnack(lc_param_t* pLcParams,
                                 MOBLEUINT8 const *pRxData,
                                 MOBLEUINT32 length,
                                 MODEL_MessageHeader_t *pmsgParam,
                                 MOBLEUINT8 reply)
{
  MOBLE_RESULT result = MOBLE_RESULT_FAIL;
  
  if(reply == 0)
  {
    TRACE_M(TF_LIGHT_LC_M, "length %ld, peer %.4x, dst %.4x, element index %d \r\n",
            length, pmsgParam->peer_addr, pmsgParam->dst_peer, pmsgParam->elementIndex);
  }
  
  if(length == 1)
  {
    TRACE_I(TF_LIGHT_LC_M, "LC occupancy mode %d\r\n", pRxData[0]);
    
    if(pRxData[0] == 0 ||
       pRxData[0] == 1)
    {
      pLcParams->occupancyMode = pRxData[0];
      result = MOBLE_RESULT_SUCCESS;
    }
    else
    {
      TRACE_I(TF_LIGHT_LC_M,"Invalid occupancy mode value\r\n");
    }
    
    NvmStatePowerFlag_Set(LIGHT_LC_NVM_FLAG, pLcParams->elementIndex);
  }
  else
  {
    result = MOBLE_RESULT_FAIL;
    TRACE_I(TF_LIGHT_LC_M, "Invalid message length %ld\r\n", length);
  }
  
  if (MOBLE_SUCCEEDED(result))
  {
    /* Application callback */
    if(pLcParams->p_cb->LightLCs_OmSetUnack_cb != NULL)
    {
      (pLcParams->p_cb->LightLCs_OmSetUnack_cb)(pRxData, pmsgParam);
    }
    
    if(reply == 0)
    {
      result = Light_LC_OmStatus(pmsgParam,
                                 pLcParams->occupancyMode,
                                 STATUS_SEND_PUBLISH);
    }
    else
    {
      result = Light_LC_OmStatus(pmsgParam,
                                 pLcParams->occupancyMode,
                                 STATUS_SEND_REPLY_PUBLISH);
    }
  }
  
  return result;
}


/**
  * @brief  Light_LC_OnOffGet callback
  *         response to be sent only to client node and not to pulish group
  * @param  Reference to LC parameters
  * @param  Parameters as set by client
  * @param  Parameters length
  * @param  Reference to message parameters
  * @retval Invalid if length is not appropriate, else as set by status function
  */
static
MOBLE_RESULT Light_LC_OnOffGet(lc_param_t* pLcParams,
                               MOBLEUINT8 const *pRxData,
                               MOBLEUINT32 length,
                               MODEL_MessageHeader_t *pmsgParam)
{
  MOBLE_RESULT result = MOBLE_RESULT_FAIL;
  
  TRACE_M(TF_LIGHT_LC_M, "length %ld, peer %.4x, dst %.4x, element index %d \r\n",
          length, pmsgParam->peer_addr, pmsgParam->dst_peer, pmsgParam->elementIndex);
  
  if(length == 0)
  {
    result = MOBLE_RESULT_SUCCESS;
  }
  else
  {
    TRACE_M(TF_LIGHT_LC_M, "Invalid message length %ld\r\n", length);
  }
  
  if(result == MOBLE_RESULT_SUCCESS)
  {
    /* Application callback */
    if(pLcParams->p_cb->LightLCs_OnOffGet_cb != NULL)
    {
      (pLcParams->p_cb->LightLCs_OnOffGet_cb)(pmsgParam);
    }
    
    result = Light_LC_OnOffStatus(pLcParams,
                                  pmsgParam,
                                  STATUS_SEND_REPLY);
  }
  
  return result;
}


/**
  * @brief  Light_LC_OnOffSet callback
  *         response to be sent to client node and publish to pulish group
  * @param  Reference to LC parameters
  * @param  Parameters as set by client
  * @param  Parameters length
  * @param  Reference to message parameters
  * @retval As set by unack function
  */
static
MOBLE_RESULT Light_LC_OnOffSet(lc_param_t* pLcParams,
                               MOBLEUINT8 const *pRxData,
                               MOBLEUINT32 length,
                               MODEL_MessageHeader_t *pmsgParam)
{
  MOBLE_RESULT result = MOBLE_RESULT_FAIL;
  
  TRACE_M(TF_LIGHT_LC_M, "length %ld, peer %.4x, dst %.4x, element index %d \r\n",
          length, pmsgParam->peer_addr, pmsgParam->dst_peer, pmsgParam->elementIndex);
  
  result = Light_LC_OnOffSetUnack(pLcParams, pRxData, length, pmsgParam, 1);
  
  if (MOBLE_SUCCEEDED(result))
  {
    /* Application callback */
    if(pLcParams->p_cb->LightLC_OnOffSet_cb != NULL)
    {
      (pLcParams->p_cb->LightLC_OnOffSet_cb)(pRxData, pmsgParam);
    }
  }
  
  return result;
}


/**
  * @brief  Light_LC_ModeSetUnack callback
  *         Publish to pulish group if called by process message
  *         Reply to client as well if called by set function
  * @param  Reference to LC parameters
  * @param  Parameters as set by client
  * @param  Parameters length
  * @param  Reference to message parameters
  * @param  reply to client flag
  * @retval Fail if length is inappropriate, TID check fail, step resolution 0, 
  *         else as set by fsm or status function
  */
static
MOBLE_RESULT Light_LC_OnOffSetUnack(lc_param_t* pLcParams,
                                    MOBLEUINT8 const *pRxData,
                                    MOBLEUINT32 length,
                                    MODEL_MessageHeader_t *pmsgParam,
                                    MOBLEUINT8 reply)
{
  MOBLE_RESULT result = MOBLE_RESULT_SUCCESS;
  MOBLEUINT8 optionalParams;
  MOBLEUINT32 delayMsOnOffMsg = 0;
  MOBLEUINT8 noOfSteps = 0; /* temporary */
  MOBLEUINT32 stepResMsOnOffMsg = 0;
  MOBLEUINT32 trTimeMsOnOffMsg = 0;
  
  MOBLEUINT8 onOff = pRxData[0];
  MOBLEUINT8 tid = pRxData[1];

  if(reply == 0)
  {
    TRACE_M(TF_LIGHT_LC_M, "length %ld, peer %.4x, dst %.4x, element index %d \r\n",
            length, pmsgParam->peer_addr, pmsgParam->dst_peer, pmsgParam->elementIndex);
  }
  
  if(length == 2)
  {
    TRACE_I(TF_LIGHT_LC_M, "Target %d tid %d\r\n", onOff, tid);
    
    optionalParams = 0;
  }
  else if(length == 4)
  {
    TRACE_I(TF_LIGHT_LC_M, "Target %d tid %d transition time %x delay %d \r\n", 
            onOff, tid, pRxData[2], pRxData[3]);
    
    optionalParams = 1;
    noOfSteps = pRxData[2] & 0x3F;
    stepResMsOnOffMsg = Get_StepResolutionValue((pRxData[2] & 0xC0) >> 6);
    delayMsOnOffMsg = pRxData[3]*5; /* 5 milliseconds step */
    trTimeMsOnOffMsg = noOfSteps*stepResMsOnOffMsg;
    
    if(stepResMsOnOffMsg == 0)
    {
      result = MOBLE_RESULT_FAIL;
      TRACE_I(TF_LIGHT_LC_M, "Invalid step resolution %d\r\n", (pRxData[2] & 0xC0) >> 6);
    }
  }
  else
  {
    result = MOBLE_RESULT_FAIL;
    TRACE_I(TF_LIGHT_LC_M, "Invalid message length %ld\r\n", length);
  }

  if(result == MOBLE_RESULT_SUCCESS)
  {
    /* TID check and update */
    if(Tid_CheckAndUpdate(tid, pmsgParam->peer_addr, pmsgParam->dst_peer, &pLcParams->lastMsgTidParams) == 0)
    {
      result = MOBLE_RESULT_FAIL;
      TRACE_I(TF_LIGHT_LC_M, "Old message dropped\r\n");
    }
  }

  if(result == MOBLE_RESULT_SUCCESS)
  {
    if(onOff == 0)
    {
      result = Light_LC_Fsm(LIGHT_OFF, 
                            pLcParams, 
                            optionalParams, 
                            delayMsOnOffMsg, 
                            stepResMsOnOffMsg, 
                            trTimeMsOnOffMsg);
    }
    else if(onOff == 1)
    {
      result = Light_LC_Fsm(LIGHT_ON, 
                            pLcParams, 
                            optionalParams, 
                            delayMsOnOffMsg, 
                            stepResMsOnOffMsg, 
                            trTimeMsOnOffMsg);
    }
    else /* invalid value */
    {
      result = MOBLE_RESULT_FAIL;
      TRACE_I(TF_LIGHT_LC_M, "Invalid on off field %d\r\n", pRxData[0]);
    }
    
    /* Application callback */
    if(pLcParams->p_cb->LightLCs_OnOffSetUnack_cb != NULL)
    {
      (pLcParams->p_cb->LightLCs_OnOffSetUnack_cb)(pRxData, pmsgParam);
    }
    
    if(MOBLE_SUCCEEDED(result))
    {
      if(reply == 0)
      {
        result = Light_LC_OnOffStatus(pLcParams,
                                      pmsgParam,
                                      STATUS_SEND_PUBLISH);
      }
      else
      {
        result = Light_LC_OnOffStatus(pLcParams,
                                      pmsgParam,
                                      STATUS_SEND_REPLY_PUBLISH);
      }      
    }
  }
  
  return result;
}


/**
  * @brief  Light_LC_PropertyGet callback
  *         response to be sent only to client node and not to pulish group
  * @param  Reference to LC parameters
  * @param  Parameters as set by client
  * @param  Parameters length
  * @param  Reference to message parameters
  * @retval Fail if length is not appropriate or inconsistency with property id, 
  *         else as set by status function
  */
static
MOBLE_RESULT Light_LC_PropertyGet(lc_param_t* pLcParams,
                                  MOBLEUINT8 const *pRxData,
                                  MOBLEUINT32 length,
                                  MODEL_MessageHeader_t *pmsgParam)
{
  MOBLE_RESULT result = MOBLE_RESULT_FAIL;
  MOBLEUINT16 rcvdPropertyId = 0x0000;
  MOBLEUINT32 propertyValue = 0;
  MOBLEUINT8 propertySize = 0;
  
  TRACE_M(TF_LIGHT_LC_M, "length %ld, peer %.4x, dst %.4x, element index %d \r\n",
          length, pmsgParam->peer_addr, pmsgParam->dst_peer, pmsgParam->elementIndex);
  
  if(length == 2)
  {
    result = ExtractPropertyId(pRxData, &rcvdPropertyId);
    
    if(result == MOBLE_RESULT_SUCCESS)
    {
      for(MOBLEUINT8 count=0; count<LC_PROPERTY_TABLE_COUNT; count++)
      {
        /* extract property index */
        if(pLcParams->pPropertyId[count].id == rcvdPropertyId)
        {
          propertySize = pLcParams->pPropertyId[count].size;
          
          result = Light_LC_PropertyValueGet(pLcParams->pPropertyId,
                                             pLcParams->propertyValue,
                                             rcvdPropertyId,
                                             count,
                                             &propertyValue);        
          
          if(result == MOBLE_RESULT_SUCCESS)
          {
            TRACE_M(TF_LIGHT_LC_M, "Rcvd property id %x property size %d property value %lx\r\n",
                    rcvdPropertyId, propertySize, propertyValue);
            break;
          }
        }
      }
    }
    else
    {
      TRACE_M(TF_LIGHT_LC_M, "Prohibited property id\r\n");
    }
  }
  else
  {
    TRACE_M(TF_LIGHT_LC_M, "Invalid message length %ld\r\n", length);
  }
  
  if (MOBLE_SUCCEEDED(result))
  {
    /* Application callback */
    if(pLcParams->p_cb->LightLCs_PropertyGet_cb != NULL)
    {
      (pLcParams->p_cb->LightLCs_PropertyGet_cb)(pRxData, pmsgParam);
    }
    
    result = Light_LC_PropertyStatus(pmsgParam,
                                     rcvdPropertyId,
                                     propertySize,
                                     propertyValue,
                                     STATUS_SEND_REPLY);
  }
  
  return result;
}


/**
  * @brief  Light_LC_PropertySet callback
  *         response to be sent to client node and publish to pulish group
  * @param  Reference to LC parameters
  * @param  Parameters as set by client
  * @param  Parameters length
  * @param  Reference to message parameters
  * @retval As set by unack function
  */
static
MOBLE_RESULT Light_LC_PropertySet(lc_param_t* pLcParams,
                                  MOBLEUINT8 const *pRxData,
                                  MOBLEUINT32 length,
                                  MODEL_MessageHeader_t *pmsgParam)
{
  MOBLE_RESULT result = MOBLE_RESULT_FAIL;
  
  TRACE_M(TF_LIGHT_LC_M, "length %ld, peer %.4x, dst %.4x, element index %d \r\n",
          length, pmsgParam->peer_addr, pmsgParam->dst_peer, pmsgParam->elementIndex);
  
  result = Light_LC_PropertySetUnack(pLcParams, pRxData, length, pmsgParam, 1);
  
  if (MOBLE_SUCCEEDED(result))
  {
    /* Application callback */
    if(pLcParams->p_cb->LightLCs_PropertySet_cb != NULL)
    {
      (pLcParams->p_cb->LightLCs_PropertySet_cb)(pRxData, pmsgParam);
    }
  }
  
  return result;
}


/**
  * @brief  Light_LC_PropertySetUnack callback
  *         Publish to pulish group if called by process message
  *         Reply to client as well if called by set function
  *         Limitation: No call to LC state machine
  *         Effects caused by changes in LC properties would be visible in next call
  * @param  Reference to LC parameters
  * @param  Parameters as set by client
  * @param  Parameters length
  * @param  Reference to message parameters
  * @param  reply to client flag
  * @retval Fail if length mismatch or property id inappropriate else as set by 
  *         status function
  */
static
MOBLE_RESULT Light_LC_PropertySetUnack(lc_param_t* pLcParams,
                                       MOBLEUINT8 const *pRxData,
                                       MOBLEUINT32 length,
                                       MODEL_MessageHeader_t *pmsgParam,
                                       MOBLEUINT8 reply)
{
  MOBLE_RESULT result = MOBLE_RESULT_FAIL;
  MOBLEUINT16 rcvdPropertyId = 0x0000;
  MOBLEUINT8 rcvdPropertySize = 0;
  MOBLEUINT32 rcvdPropertyValue = 0;
  
  if(reply == 0)
  {
    TRACE_M(TF_LIGHT_LC_M, "length %ld, peer %.4x, dst %.4x, element index %d \r\n",
            length, pmsgParam->peer_addr, pmsgParam->dst_peer, pmsgParam->elementIndex);
  }
  
  if(length > 2 &&
     length <= 6)
  {
    result = ExtractPropertyId(pRxData, &rcvdPropertyId);
    
    if(result == MOBLE_RESULT_SUCCESS)
    {
      rcvdPropertySize = length - PROPERTY_ID_LENGTH;
      
      GetLittleEndian(pRxData+2, rcvdPropertySize, &rcvdPropertyValue);
      
      TRACE_I(TF_LIGHT_LC_M, "Rcvd property id %x property size %d property value %lx\r\n",
              rcvdPropertyId, rcvdPropertySize, rcvdPropertyValue);
      
      result = Light_LC_PropertyValueSet(pLcParams->pPropertyId,
                                         pLcParams->propertyValue,
                                         rcvdPropertyId,
                                         rcvdPropertySize,
                                         rcvdPropertyValue);
    }
    else
    {
      TRACE_I(TF_LIGHT_LC_M, "Prohibited property id\r\n");
    }
  }
  else
  {
    TRACE_I(TF_LIGHT_LC_M, "Invalid message length %ld\r\n", length);
  }
  
  if (MOBLE_SUCCEEDED(result))
  {
    /* Application callback */
    if(pLcParams->p_cb->LightLCs_PropertySetUnack_cb != NULL)
    {
      (pLcParams->p_cb->LightLCs_PropertySetUnack_cb)(pRxData, pmsgParam);
    }
    
    if(reply == 0)
    {
      result = Light_LC_PropertyStatus(pmsgParam,
                                       rcvdPropertyId,
                                       rcvdPropertySize,
                                       rcvdPropertyValue,
                                       STATUS_SEND_PUBLISH);
    }
    else
    {
      result = Light_LC_PropertyStatus(pmsgParam,
                                       rcvdPropertyId,
                                       rcvdPropertySize,
                                       rcvdPropertyValue,
                                       STATUS_SEND_REPLY_PUBLISH);
    }
  }
  
  return result;
}


/**
  * @brief  Get property id value given the size of property from property value table
  *         Extra care required to pass parameters as parameters may seem redundant
  * @param  Reference to property ID table
  * @param  Reference to property Value table
  * @param  Property id
  * @param  Property Index (in property table) for above property id
  * @retval Fail if property id doesn't match with table property id else success
  */
static
MOBLE_RESULT Light_LC_PropertyValueGet(const light_lc_propertyId_t* pPropertyIdTable,
                                       MOBLEUINT32* pPropertyValueTable,
                                       MOBLEUINT16 propertyId,
                                       MOBLEUINT8 propertyIndex,
                                       MOBLEUINT32* value)
{
  MOBLE_RESULT result = MOBLE_RESULT_SUCCESS;
  
  if(pPropertyIdTable[propertyIndex].id == propertyId)
  {
    *value = pPropertyValueTable[propertyIndex];
  }
  else
  {
    result = MOBLE_RESULT_FAIL;
  }

  if(result == MOBLE_RESULT_SUCCESS)
  {
    /* TRACE_M(TF_LIGHT_LC_M, "Property id %x value %x\r\n",
            pPropertyIdTable[propertyIndex].id, pPropertyValueTable[propertyIndex]); */
  }
  else
  {
    TRACE_M(TF_LIGHT_LC_M, "Property id %x not found at index %d", propertyId, propertyIndex);
  }

  return result;
}


/**
  * @brief  Set property id value in property value table
  * @param  Reference to property value table
  * @param  Reference to property id table
  * @param  Property id
  * @param  Property size
  * @retval Fail if property id does not match with any property id in table
  */
static
MOBLE_RESULT Light_LC_PropertyValueSet(const light_lc_propertyId_t* pPropertyIdTable,
                                       MOBLEUINT32* pPropertyValueTable,
                                       MOBLEUINT16 propertyId,
                                       MOBLEUINT8 propertySize,
                                       MOBLEUINT32 value)
{
  MOBLE_RESULT result = MOBLE_RESULT_FAIL;
  MOBLEUINT8 count = 0;
  
  for(count=0; count<LC_PROPERTY_TABLE_COUNT; count++)
  {
    if(propertyId == pPropertyIdTable[count].id)
    {
      if(propertySize == pPropertyIdTable[count].size)
      {
        result = MOBLE_RESULT_SUCCESS;
        pPropertyValueTable[count] = value;
        
        if(propertyId == LIGHT_CONTROL_TIME_RUN_ON_PID && 
           pPropertyValueTable[count] != 0)
        {
          /* 600ms added in RUN to pass round 9 of MMDL/SR/LLC/BV-11-C 
             To confirm with PTS Errata team */
          pPropertyValueTable[count] += 600;
        }
        
        break;
      }
    }
  }
  
  if(result == MOBLE_RESULT_SUCCESS)
  {
    /* TRACE_M(TF_LIGHT_LC_M, "id %x index %d value %x\r\n",
            pPropertyIdTable[count].id,
            pPropertyIdTable[count].valueIndex,
            pPropertyValueTable[count]); */
  }
  else
  {
    TRACE_M(TF_LIGHT_LC_M, "Not able to set id %x index %d value %lx\r\n",
            propertyId, count, value);
  }

  return result;
}


/**
  * @brief  Light LC transition stop or start
  * @param  Reference to LC parameters
  * @param  Stop transition flag
  * @param  Start transition flag
  * @param  Delay in milliseconds
  * @param  Transition time in milliseconds
  * @param  Transition step resolution in milliseconds
  * @retval Transition event as returned by transition state machine
  */
static
transition_event_e Light_LC_TransitionUpdate(lc_param_t* pLcParams,
                                             MOBLEUINT8 stopTransitionFlag,
                                             MOBLEUINT8 startTransitionFlag,
                                             MOBLEUINT32 delayMs,
                                             MOBLEUINT32 transitionTimeMs,
                                             MOBLEUINT32 stepResolutionMs)
{
  transition_event_e transitionEvent = TRANSITION_EVENT_NO;
  transition_params_t* pTrParams = &pLcParams->transitionParams;
  
  if(stopTransitionFlag == 1)
  {
    transitionEvent = Transition_Stop(&pTrParams->trStatus);
  }
  
  if(startTransitionFlag == 1)
  {
    pTrParams->trTimeMs = transitionTimeMs;
    pTrParams->stepResolutionMs = stepResolutionMs;
    transitionEvent = Transition_Sm(pTrParams, delayMs);
  }
  
  return transitionEvent;
}


/**
  * @brief  Set initial and final lightness level and light level given the PIDs
  * @param  Reference to LC parameters
  * @param  Initial level update flag
  * @param  Initial lightness level property id
  * @param  Initial lightness level property index in property table
  * @param  Initial light level property id
  * @param  Initial light level property index in property table
  * @param  Final level update flag
  * @param  Final lightness level property id
  * @param  Final lightness level property index in property table
  * @param  Final light level property id
  * @param  Final light level property index in property table
  * @retval Success if property id and property index are appropriate else fail
  */
static
MOBLE_RESULT Light_LC_InitialTargetValueUpdatePid(lc_param_t* pLcParams,
                                                  MOBLEUINT8 initialLevelUpdateFlag,
                                                  MOBLEUINT16 initialLightnessPid,
                                                  MOBLEUINT8 initialLightnessIndex,
                                                  MOBLEUINT16 initialLightPid,
                                                  MOBLEUINT8 initialLightIndex,
                                                  MOBLEUINT8 finalLevelUpdateFlag,
                                                  MOBLEUINT16 finalLightnessPid,
                                                  MOBLEUINT8 finalLightnessIndex,
                                                  MOBLEUINT16 finalLightPid,
                                                  MOBLEUINT8 finalLightIndex)
{
  MOBLE_RESULT result = MOBLE_RESULT_SUCCESS;
  MOBLEUINT32 tempVal = 0;
    
  if(initialLevelUpdateFlag == 1)
  {
    if(initialLightnessPid != PROPERTY_ID_PROHIBITED)
    {
      result = Light_LC_PropertyValueGet(pLcParams->pPropertyId,
                                         pLcParams->propertyValue,
                                         initialLightnessPid,
                                         initialLightnessIndex,
                                         &tempVal);
      if(result == MOBLE_RESULT_SUCCESS)
      {
        pLcParams->initialLightnessOut = (MOBLEUINT16)tempVal;
      }
    }
    else
    {
      TRACE_M(TF_LIGHT_LC_M, "Prohibited property id to extract initial lightness\r\n");
      result = MOBLE_RESULT_FAIL;
    }
    
    if(initialLightPid != PROPERTY_ID_PROHIBITED)
    {
      result = Light_LC_PropertyValueGet(pLcParams->pPropertyId,
                                         pLcParams->propertyValue,
                                         initialLightPid,
                                         initialLightIndex,
                                         &tempVal);
      if(result == MOBLE_RESULT_SUCCESS)
      {
        pLcParams->initialLightLuxOut = tempVal;
      }
    }
    else
    {
      TRACE_M(TF_LIGHT_LC_M, "Prohibited property id to extract initial light\r\n");
      result = MOBLE_RESULT_FAIL;
    }
  }
  
  if(result == MOBLE_RESULT_SUCCESS)
  {
    if(finalLevelUpdateFlag == 1)
    {
      if(finalLightnessPid != PROPERTY_ID_PROHIBITED)
      {
        result = Light_LC_PropertyValueGet(pLcParams->pPropertyId,
                                           pLcParams->propertyValue,
                                           finalLightnessPid,
                                           finalLightnessIndex,
                                           &tempVal);
        if(result == MOBLE_RESULT_SUCCESS)
        {
          pLcParams->targetLightnessOut = (MOBLEUINT16)tempVal;
        }
      }
      else
      {
        TRACE_M(TF_LIGHT_LC_M, "Prohibited property id to extract final lightness\r\n");
        result = MOBLE_RESULT_FAIL;
      }
      
      if(finalLightPid != PROPERTY_ID_PROHIBITED)
      {
        result = Light_LC_PropertyValueGet(pLcParams->pPropertyId,
                                           pLcParams->propertyValue,
                                           finalLightPid,
                                           finalLightIndex,
                                           &tempVal);
        if(result == MOBLE_RESULT_SUCCESS)
        {
          pLcParams->targetLightLuxOut = tempVal;
        }
      }
      else
      {
        TRACE_M(TF_LIGHT_LC_M, "Prohibited property id to extract final light\r\n");
        result = MOBLE_RESULT_FAIL;
      }
    }
  }
  
  if(result == MOBLE_RESULT_SUCCESS)
  {
    /* TRACE_M(TF_LIGHT_LC_M, "Lightness level initial %x and final %x Light level initial %x and final %x\r\n",
                             pLcParams->initialLightnessOut, pLcParams->targetLightnessOut, 
                             pLcParams->initialLightLuxOut, pLcParams->targetLightLuxOut); */
  }
  
  return result;
}


/**
  * @brief  PI regulator to adjust ambient level
  *         Magnified values are used for optimizing case when regulator input is 0
  * @param  Reference to LC parameters
  * @retval PI regulator output
  */
static
MOBLEUINT16 Light_LC_PIregulator(lc_param_t* pLcParams)
{
  MOBLEUINT32 tempU32i = 0;
  MOBLEUINT32 tempU32p = 0;
  MOBLEUINT32* pU32temp = 0;
  float* pFtemp;
  float tempFloat = 0;
  MOBLEUINT16 regulatorOutput = 0;
  MOBLEINT32 error = 0;
  MOBLEINT32 accuracy = 0;
  MOBLEINT32 regulatorInput = 0;
  MOBLEUINT32 summationIntervalMs = 0; /* actual value in seconds */
  
  /* Lux level out magnified 100 times to match resolution of ambient lux level */
  error = (pLcParams->luxLevelOut)*100 - pLcParams->ambientLuxLevel;
  
  Light_LC_PropertyValueGet(pLcParams->pPropertyId,
                            pLcParams->propertyValue,
                            LIGHT_CONTROL_REGULATOR_ACCURACY_PID,
                            LC_REGULATOR_ACCURACY_PID_INDEX,
                            &tempU32i);
  /* accuracy is 2 times actual value, accuracy multiplied by 50 to match error scale */
  accuracy = (MOBLEUINT8)tempU32i * pLcParams->luxLevelOut * 50;

  /* calculate regulator input (100x) */
  if(error > accuracy) 
  {
    regulatorInput = error - accuracy;
  }
  else if(error < (-1)*accuracy)
  {
    regulatorInput = error + accuracy;
  }
  else
  {
    regulatorInput = 0;
  }
    
  if(regulatorInput != 0)
  {
    /* calculate summation interval */
    if(Clock_Time() >= pLcParams->piLightnessUpdateTick)
    {
      summationIntervalMs = Clock_Time() - pLcParams->piLightnessUpdateTick;
    }
    else
    {
      summationIntervalMs  = Clock_Time() + (MAX_U32_VALUE - pLcParams->piLightnessUpdateTick);
    }
    
    if(summationIntervalMs > 100)
    {
      summationIntervalMs = 100;
    }
    else if(summationIntervalMs < 10)
    {
      summationIntervalMs = 0;
    }
        
    pLcParams->piLightnessUpdateTick = Clock_Time();
    
    
    /* calculate regulator output */
    if(regulatorInput >= 0) /* Coefficients for UP case */
    {
      Light_LC_PropertyValueGet(pLcParams->pPropertyId,
                                pLcParams->propertyValue,
                                LIGHT_CONTROL_REGULATOR_KIU_PID,
                                LC_REGULATOR_KIU_PID_INDEX,
                                &tempU32i);
      
      Light_LC_PropertyValueGet(pLcParams->pPropertyId,
                                pLcParams->propertyValue,
                                LIGHT_CONTROL_REGULATOR_KPU_PID,
                                LC_REGULATOR_KPU_PID_INDEX,
                                &tempU32p);
    }
    else /* Coefficients for DOWN case */
    {
      Light_LC_PropertyValueGet(pLcParams->pPropertyId,
                                pLcParams->propertyValue,
                                LIGHT_CONTROL_REGULATOR_KID_PID,
                                LC_REGULATOR_KID_PID_INDEX,
                                &tempU32i);
      
      Light_LC_PropertyValueGet(pLcParams->pPropertyId,
                                pLcParams->propertyValue,
                                LIGHT_CONTROL_REGULATOR_KPD_PID,
                                LC_REGULATOR_KPD_PID_INDEX,
                                &tempU32p);
    }
        
    /* Regulator input is 100x
       Time is 1000x */
    pU32temp = &tempU32i;
    pFtemp = (float*)pU32temp;
    tempFloat = (MOBLEINT32)pLcParams->piLastInternalSum + \
                (regulatorInput * summationIntervalMs * (*pFtemp))/100000;

    if(tempFloat < 0) /* summation interval range is 0 - 65535 */
    {
      pLcParams->piLastInternalSum = 0;
    }
    else if(tempFloat > 65535)
    {
      pLcParams->piLastInternalSum = 65535;
    }
    else
    {
      pLcParams->piLastInternalSum = (MOBLEUINT16)tempFloat;
    }

    /* Regulator input is 100x */
    pU32temp = &tempU32p;
    pFtemp = (float*)pU32temp;
    tempFloat = (pLcParams->piLastInternalSum + regulatorInput * (*pFtemp))/100;

    if(tempFloat < 0)
    {
      regulatorOutput = 0;
    }
    else if(tempFloat > 65535)
    {
      regulatorOutput = 65535;
    }
    else
    {
      regulatorOutput = (MOBLEUINT16)tempFloat;
    }
  }
  else /* regulator input is 0, no change in output */
  {
    regulatorOutput = pLcParams->piLastInternalSum;
  }
  
  return regulatorOutput;
}


/**
  * @brief  Lighting control state machine
  *         Sources of events
  *             Solicited change in binded Light Lightness - mode off
  *             Light LC messages - mode off, light on, light off
  *             Occupancy sensor - occupancy on
  *             Transition state machine - timer on, timer triggered, timer off
  *         Execution may result in events from transition state machine
  *             Such events are handled in same function call via recursion for
  *             immediate processing of pending events
  *         In case of light on or light off, delay and transition parameters
  *         can come from either on off message or from corresponding properties
  * @param  LC event
  *         Is optional parameters present in on off message or not
  *         Delay in milliseconds received in on off message else 0
  *         Step resolution in milliseconds in on off message else 0
  *         Transition time in milliseconds calculated from parameters in on off message
  * @retval Success if all property ids and property indexes are appropriate else fail
  */
static
MOBLE_RESULT Light_LC_Fsm(lc_event_e event,
                          lc_param_t* pLcParams,
                          MOBLEUINT8 optionalParamsOnOffMsg,
                          MOBLEUINT32 delayMsOOnOD,
                          MOBLEUINT32 stepResMsOnOffMsg,
                          MOBLEUINT32 transitionTimeMsOnOffMsg)
{
  MOBLE_RESULT result = MOBLE_RESULT_FALSE;
//  MOBLEUINT32 currentTick = Clock_Time();
  transition_event_e transitionEvent = TRANSITION_EVENT_NO;
  transition_params_t* pTrParams = &pLcParams->transitionParams;
  MOBLEUINT32 tempVal = 0;
  MOBLEUINT32 timerMs = 0;
  
  /* TRACE_M(TF_LIGHT_LC_M, "Current state %d LC event %d\r\n", 
          pLcParams->state, event); */
  
  /* Event specific changes, events which are common in many states are handled in event specific changes
     Transition may not be running and stop transition results in no action
     occupancy event is not relevant in fade standby manual */
  
  if (event == MODE_OFF && pLcParams->state != OFF)
  {
    /* Stop transition if in delay or running */
    Light_LC_TransitionUpdate(pLcParams, 1, 0, 0, 0, 0); /* transition event not updated */
    pLcParams->publish = 1;
    Binding_LightLcLightOnOff_GenericOnOff(pLcParams->elementIndex, 0);
    
    /* Unidirectional binding between LC Linear out and Lightness Linear breaks 
       Lightness not to be updated */
    
    NvmStatePowerFlag_Set(LIGHT_LC_NVM_FLAG, pLcParams->elementIndex);
    TRACE_M(TF_LIGHT_LC_M, "Any state -> OFF\r\n");
    pLcParams->state = OFF;
  }
  else if(event == AMBIENT_LUX_MODIFIED && pLcParams->state != OFF)
  {
    pLcParams->lcLinearOut = MAX(pLcParams->lightnessLinearUnreg, Light_LC_PIregulator(pLcParams));
    Binding_LcLinearOut_LightLightnessLinear(pLcParams->elementIndex, pLcParams->lcLinearOut);
  }
  else
  {
    /* State specific changes */
    switch(pLcParams->state)
    {
    case OFF:
      if(event == MODE_ON)
      {
        result = Light_LC_InitialTargetValueUpdatePid
          (pLcParams, 
           0, 0, 0, 0, 0,
           1,
           LIGHT_CONTROL_LIGHTNESS_STANDBY_PID,
           LC_LIGHTNESS_STANDBY_PID_INDEX,
           LIGHT_CONTROL_AMBIENT_LUXLEVEL_ON_PID,
           LC_AMBIENT_LUXLEVEL_ON_PID_INDEX);
        
        /* current lightness value */
        pLcParams->lcActualSmOut = pLcParams->targetLightnessOut;
        pLcParams->luxLevelOut = pLcParams->targetLightLuxOut;
        pLcParams->updateLcLinearOut = 1;
        
        /* sync Generic OnOff state with LC OnOff state */
        Binding_LightLcLightOnOff_GenericOnOff(pLcParams->elementIndex, 0);
        
        /* Update LC mode in NVM */
        NvmStatePowerFlag_Set(LIGHT_LC_NVM_FLAG, pLcParams->elementIndex);
        pLcParams->state = STANDBY;
        TRACE_M(TF_LIGHT_LC_M, "OFF -> STANDBY\r\n");
      }
      else
      {
        TRACE_M(TF_LIGHT_LC_M, "No action in OFF, event %d\r\n", event);
      }
      break;
    case STANDBY:
      if(event == LIGHT_ON)
      {
        result = Light_LC_InitialTargetValueUpdatePid
          (pLcParams, 
           1, 
           LIGHT_CONTROL_LIGHTNESS_STANDBY_PID,
           LC_LIGHTNESS_STANDBY_PID_INDEX,
           LIGHT_CONTROL_AMBIENT_LUXLEVEL_STANDBY_PID,
           LC_AMBIENT_LUXLEVEL_STANDBY_PID_INDEX,
           1, 
           LIGHT_CONTROL_LIGHTNESS_ON_PID,
           LC_LIGHTNESS_ON_PID_INDEX, 
           LIGHT_CONTROL_AMBIENT_LUXLEVEL_ON_PID,
           LC_AMBIENT_LUXLEVEL_ON_PID_INDEX);
        
        if(optionalParamsOnOffMsg == 1)
        {
          transitionEvent = Light_LC_TransitionUpdate
            (pLcParams, 1, 1, delayMsOOnOD, transitionTimeMsOnOffMsg, stepResMsOnOffMsg);
        }
        else /* optional parameters not present, delay = 0 */
        {
          Light_LC_PropertyValueGet(pLcParams->pPropertyId,
                                    pLcParams->propertyValue,
                                    LIGHT_CONTROL_TIME_FADE_ON_PID,
                                    LC_TIME_FADE_ON_PID_INDEX,
                                    &tempVal);
          
          transitionEvent = Light_LC_TransitionUpdate
            (pLcParams, 0, 1, 0, tempVal, LIGHT_LC_DEFAULT_TRANSITION_RES_MS);
        }
        
        NvmStatePowerFlag_Set(LIGHT_LC_NVM_FLAG, pLcParams->elementIndex);
        pLcParams->state = POST_STANDBY;
        TRACE_M(TF_LIGHT_LC_M, "STANDBY -> POST STANDBY light on\r\n");
      }
      else if(event == OCCUPANCY_ON)
      {
        result = Light_LC_InitialTargetValueUpdatePid
          (pLcParams, 
           1,
           LIGHT_CONTROL_LIGHTNESS_STANDBY_PID,
           LC_LIGHTNESS_STANDBY_PID_INDEX,
           LIGHT_CONTROL_AMBIENT_LUXLEVEL_STANDBY_PID,
           LC_AMBIENT_LUXLEVEL_STANDBY_PID_INDEX,
           1, 
           LIGHT_CONTROL_LIGHTNESS_ON_PID,
           LC_LIGHTNESS_ON_PID_INDEX,
           LIGHT_CONTROL_AMBIENT_LUXLEVEL_ON_PID,
           LC_AMBIENT_LUXLEVEL_ON_PID_INDEX);
        
        Light_LC_PropertyValueGet(pLcParams->pPropertyId,
                                  pLcParams->propertyValue,
                                  LIGHT_CONTROL_TIME_FADE_ON_PID,
                                  LC_TIME_FADE_ON_PID_INDEX,
                                  &tempVal);
        
        transitionEvent = Light_LC_TransitionUpdate
          (pLcParams, 0, 1, delayMsOOnOD, tempVal, LIGHT_LC_DEFAULT_TRANSITION_RES_MS);
        
        NvmStatePowerFlag_Set(LIGHT_LC_NVM_FLAG, pLcParams->elementIndex);
        pLcParams->state = POST_STANDBY;
        TRACE_M(TF_LIGHT_LC_M, "STANDBY -> POST STANDBY oocupancy on\r\n");
      }
      else
      {
        TRACE_M(TF_LIGHT_LC_M, "No action in STANDBY, event %d\r\n", event);
      }
      break;
    case POST_STANDBY: /* No break */
    case PRE_FADE_ON:
      if(event == TIMER_ON)
      {
        if(pLcParams->state == POST_STANDBY)
        {
          Binding_LightLcLightOnOff_GenericOnOff(pLcParams->elementIndex, 1);
          TRACE_M(TF_LIGHT_LC_M, "POST STANDBY -> FADE ON\r\n");
        }
        else
        {
          TRACE_M(TF_LIGHT_LC_M, "PRE FADE ON -> FADE ON\r\n");
        }
        
        pLcParams->state = FADE_ON;
      }
      else if(event == TIMER_OFF)
      {
        pLcParams->lcActualSmOut = pLcParams->targetLightnessOut;
        pLcParams->luxLevelOut = pLcParams->targetLightLuxOut;
        pLcParams->updateLcLinearOut = 1;
        
        result = Light_LC_PropertyValueGet(pLcParams->pPropertyId,
                                           pLcParams->propertyValue,
                                           LIGHT_CONTROL_TIME_RUN_ON_PID,
                                           LC_TIME_RUN_ON_PID_INDEX,
                                           &tempVal);
        /* step resolution = transition time to avoid intermediate trigger in run */
        transitionEvent = Light_LC_TransitionUpdate
          (pLcParams, 0, 1, 0, tempVal, tempVal);
        
        pLcParams->publish = 1;
        
        if(pLcParams->state == POST_STANDBY)
        {
          Binding_LightLcLightOnOff_GenericOnOff(pLcParams->elementIndex, 1);
          TRACE_M(TF_LIGHT_LC_M, "POST STANDBY -> RUN\r\n");
        }
        else
        {
          TRACE_M(TF_LIGHT_LC_M, "PRE FADE ON -> RUN\r\n");
        }
        
        pLcParams->state = RUN;
      }
      else if(event == LIGHT_OFF)
      {
        if(pLcParams->state == PRE_FADE_ON)
        {
          result = Light_LC_InitialTargetValueUpdatePid
            (pLcParams, 
             0, 0, 0, 0, 0,
             1,
             LIGHT_CONTROL_LIGHTNESS_STANDBY_PID,
             LC_LIGHTNESS_STANDBY_PID_INDEX,
             LIGHT_CONTROL_AMBIENT_LUXLEVEL_STANDBY_PID,
             LC_AMBIENT_LUXLEVEL_STANDBY_PID_INDEX);
          
          if(optionalParamsOnOffMsg == 1)
          {
            transitionEvent = Light_LC_TransitionUpdate
              (pLcParams, 1, 1, delayMsOOnOD, transitionTimeMsOnOffMsg, stepResMsOnOffMsg);
          }
          else /* optional parameters not present, delay = 0 */
          {
            Light_LC_PropertyValueGet(pLcParams->pPropertyId,
                                      pLcParams->propertyValue,
                                      LIGHT_CONTROL_TIME_FADE_STANDBY_MANUAL_PID,
                                      LC_TIME_FADE_STANDBY_MANUAL_PID_INDEX,
                                      &tempVal);
            
            transitionEvent = Light_LC_TransitionUpdate
              (pLcParams, 1, 1, 0, tempVal, LIGHT_LC_DEFAULT_TRANSITION_RES_MS);
          }
          
          TRACE_M(TF_LIGHT_LC_M, "PRE FADE ON -> PRE FADE STANDBY MANUAL\r\n");
          pLcParams->state = PRE_FADE_STANDBY_MANUAL;
        }
        else /* POST STANDBY */
        {
          transitionEvent = Light_LC_TransitionUpdate
            (pLcParams, 1, 0, 0, 0, 0);
          NvmStatePowerFlag_Set(LIGHT_LC_NVM_FLAG, pLcParams->elementIndex);
          pLcParams->state = STANDBY;
          TRACE_M(TF_LIGHT_LC_M, "POST STANDBY -> STANDBY\r\n");
        }
      }
      else if(event == LIGHT_ON)
      {
        if(optionalParamsOnOffMsg == 1)
        {
          transitionEvent = Light_LC_TransitionUpdate
            (pLcParams, 1, 1, delayMsOOnOD, transitionTimeMsOnOffMsg, stepResMsOnOffMsg);
        }
        else
        {
          Light_LC_PropertyValueGet(pLcParams->pPropertyId,
                                    pLcParams->propertyValue,
                                    LIGHT_CONTROL_TIME_FADE_ON_PID,
                                    LC_TIME_FADE_ON_PID_INDEX,
                                    &tempVal);
          
          transitionEvent = Light_LC_TransitionUpdate
            (pLcParams, 1, 1, 0, tempVal, LIGHT_LC_DEFAULT_TRANSITION_RES_MS);
        }
        
        if(pLcParams->state == POST_STANDBY)
        {
          TRACE_M(TF_LIGHT_LC_M, "POST STANDBY -> POST STANDBY light on\r\n");
        }
        else
        {
          TRACE_M(TF_LIGHT_LC_M, "PRE FADE ON -> PRE FADE ON light on\r\n");
        }
      }
      else
      {
        if(pLcParams->state == POST_STANDBY)
        {
          TRACE_M(TF_LIGHT_LC_M, "No action in POST STANDBY, event %d\r\n", event);
        }
        else
        {
          TRACE_M(TF_LIGHT_LC_M, "No action in PRE FADE ON, event %d\r\n", event);
        }
      }
      break;
    case FADE_ON:
      if(event == TIMER_TRIGGER) /* Transition step */
      {
        timerMs = Transition_TimerGet(pTrParams);
        /* update current lightness value in transition */
        pLcParams->lcActualSmOut = (MOBLEUINT16)Transition_StateValueGet
          (pLcParams->targetLightnessOut, 
           pLcParams->initialLightnessOut,
           timerMs,
           pTrParams->trTimeMs);
        pLcParams->luxLevelOut = Transition_StateValueGet
          (pLcParams->targetLightLuxOut, 
           pLcParams->initialLightLuxOut,
           timerMs,
           pTrParams->trTimeMs);
        pLcParams->updateLcLinearOut = 1;
        
        /* continue in fade on */
      }
      else if(event == TIMER_OFF)
      {
        result = Light_LC_PropertyValueGet(pLcParams->pPropertyId,
                                           pLcParams->propertyValue,
                                           LIGHT_CONTROL_TIME_RUN_ON_PID,
                                           LC_TIME_RUN_ON_PID_INDEX,
                                           &tempVal);
        
        /* step resolution = run time to avoid intermediate triggers in run state */
        transitionEvent = Light_LC_TransitionUpdate
          (pLcParams, 0, 1, 0, tempVal, tempVal);
        
        pLcParams->lcActualSmOut = pLcParams->targetLightnessOut;
        pLcParams->luxLevelOut = pLcParams->targetLightLuxOut;
        pLcParams->updateLcLinearOut = 1;
        pLcParams->publish = 1;
        pLcParams->state = RUN;
        TRACE_M(TF_LIGHT_LC_M, "FADE ON -> RUN\r\n");
      }
      else if(event == LIGHT_OFF)
      {
        /* update initial with current lightness level during transition */
        pLcParams->initialLightnessOut = (MOBLEUINT16)Transition_StateValueGet
          (pLcParams->targetLightnessOut, 
           pLcParams->initialLightnessOut,
           Transition_TimerGet(pTrParams),
           pTrParams->trTimeMs);
        
        /* Update target lightness value */
        result = Light_LC_InitialTargetValueUpdatePid
          (pLcParams, 
           0, 0, 0, 0, 0,
           1,
           LIGHT_CONTROL_LIGHTNESS_STANDBY_PID,
           LC_LIGHTNESS_STANDBY_PID_INDEX,
           LIGHT_CONTROL_AMBIENT_LUXLEVEL_STANDBY_PID,
           LC_AMBIENT_LUXLEVEL_STANDBY_PID_INDEX);
        
        if(optionalParamsOnOffMsg == 1)
        {
          transitionEvent = Light_LC_TransitionUpdate
            (pLcParams, 1, 1, delayMsOOnOD, transitionTimeMsOnOffMsg, stepResMsOnOffMsg);
        }
        else /* optional parameters not present, delay = 0 */
        {
          Light_LC_PropertyValueGet(pLcParams->pPropertyId,
                                    pLcParams->propertyValue,
                                    LIGHT_CONTROL_TIME_FADE_STANDBY_MANUAL_PID,
                                    LC_TIME_FADE_STANDBY_MANUAL_PID_INDEX,
                                    &tempVal);
          
          transitionEvent = Light_LC_TransitionUpdate
            (pLcParams, 1, 1, 0, tempVal, LIGHT_LC_DEFAULT_TRANSITION_RES_MS);
        }
                
        TRACE_M(TF_LIGHT_LC_M, "FADE ON -> PRE FADE STANDBY MANUAL\r\n");
        pLcParams->state = PRE_FADE_STANDBY_MANUAL;
      }
      else if(event == LIGHT_ON)
      {
        pLcParams->initialLightnessOut = (MOBLEUINT16)Transition_StateValueGet
          (pLcParams->targetLightnessOut, 
           pLcParams->initialLightnessOut,
           Transition_TimerGet(pTrParams),
           pTrParams->trTimeMs);
        
        if(optionalParamsOnOffMsg == 1)
        {
          transitionEvent = Light_LC_TransitionUpdate
            (pLcParams, 1, 1, delayMsOOnOD, transitionTimeMsOnOffMsg, stepResMsOnOffMsg);
        }
        else
        {
          Light_LC_PropertyValueGet(pLcParams->pPropertyId,
                                    pLcParams->propertyValue,
                                    LIGHT_CONTROL_TIME_FADE_ON_PID,
                                    LC_TIME_FADE_ON_PID_INDEX,
                                    &tempVal);
          
          transitionEvent = Light_LC_TransitionUpdate
            (pLcParams, 1, 1, 0, tempVal, LIGHT_LC_DEFAULT_TRANSITION_RES_MS);
        }
        
        pLcParams->publish = 1;
        TRACE_M(TF_LIGHT_LC_M, "FADE ON -> PRE FADE ON\r\n");
        pLcParams->state = PRE_FADE_ON;
      }
      else
      {
        TRACE_M(TF_LIGHT_LC_M, "No action in FADE ON, event %d\r\n", event);
      }
      break;
    case RUN:
      if (event == TIMER_OFF)
      {
        result = Light_LC_InitialTargetValueUpdatePid
          (pLcParams, 
           1,
           LIGHT_CONTROL_LIGHTNESS_ON_PID,
           LC_LIGHTNESS_ON_PID_INDEX,
           LIGHT_CONTROL_AMBIENT_LUXLEVEL_ON_PID,
           LC_AMBIENT_LUXLEVEL_ON_PID_INDEX,
           1, 
           LIGHT_CONTROL_LIGHTNESS_PROLONG_PID,
           LC_LIGHTNESS_PROLONG_PID_INDEX,
           LIGHT_CONTROL_AMBIENT_LUXLEVEL_PROLONG_PID,
           LC_AMBIENT_LUXLEVEL_PROLONG_PID_INDEX);
        
        Light_LC_PropertyValueGet(pLcParams->pPropertyId,
                                  pLcParams->propertyValue,
                                  LIGHT_CONTROL_TIME_FADE_PID,
                                  LC_TIME_FADE_PID_INDEX,
                                  &tempVal);
        
        transitionEvent = Light_LC_TransitionUpdate
          (pLcParams, 0, 1, 0, tempVal, LIGHT_LC_DEFAULT_TRANSITION_RES_MS);
        
        TRACE_M(TF_LIGHT_LC_M, "RUN -> FADE\r\n");
        pLcParams->state = FADE;
      }
      else if(event == LIGHT_OFF)
      {
        if(optionalParamsOnOffMsg == 1)
        {
          transitionEvent = Light_LC_TransitionUpdate
            (pLcParams, 1, 1, delayMsOOnOD, transitionTimeMsOnOffMsg, stepResMsOnOffMsg);
        }
        else /* optional parameters not present, delay = 0 */
        {
          Light_LC_PropertyValueGet(pLcParams->pPropertyId,
                                    pLcParams->propertyValue,
                                    LIGHT_CONTROL_TIME_FADE_STANDBY_MANUAL_PID,
                                    LC_TIME_FADE_STANDBY_MANUAL_PID_INDEX,
                                    &tempVal);
          
          transitionEvent = Light_LC_TransitionUpdate
            (pLcParams, 1, 1, 0, tempVal, LIGHT_LC_DEFAULT_TRANSITION_RES_MS);
        }
        
        result = Light_LC_InitialTargetValueUpdatePid
          (pLcParams, 
           1,
           LIGHT_CONTROL_LIGHTNESS_ON_PID,
           LC_LIGHTNESS_ON_PID_INDEX,
           LIGHT_CONTROL_AMBIENT_LUXLEVEL_ON_PID,
           LC_AMBIENT_LUXLEVEL_ON_PID_INDEX,
           1, 
           LIGHT_CONTROL_LIGHTNESS_STANDBY_PID,
           LC_LIGHTNESS_STANDBY_PID_INDEX,
           LIGHT_CONTROL_AMBIENT_LUXLEVEL_STANDBY_PID,
           LC_AMBIENT_LUXLEVEL_STANDBY_PID_INDEX);
        
        TRACE_M(TF_LIGHT_LC_M, "RUN -> PRE FADE STANDBY MANUAL\r\n");
        pLcParams->state = PRE_FADE_STANDBY_MANUAL;
      }
      else if(event == LIGHT_ON)
      {
        Light_LC_PropertyValueGet(pLcParams->pPropertyId,
                                  pLcParams->propertyValue,
                                  LIGHT_CONTROL_TIME_RUN_ON_PID,
                                  LC_TIME_RUN_ON_PID_INDEX,
                                  &tempVal);
        
        if(optionalParamsOnOffMsg == 1)
        {
          /* step resolution = run time to avoid intermediate triggers */
          transitionEvent = Light_LC_TransitionUpdate
            (pLcParams, 1, 1, delayMsOOnOD, transitionTimeMsOnOffMsg, transitionTimeMsOnOffMsg);
        }
        else /* optional parameters not present, delay = 0 */
        {
          /* step resolution = run time to avoid intermediate triggers */
          transitionEvent = Light_LC_TransitionUpdate
            (pLcParams, 1, 1, 0, tempVal, tempVal);        
        }
        
        TRACE_M(TF_LIGHT_LC_M, "RUN -> RUN light on\r\n");
      }
      else if(event == OCCUPANCY_ON)
      {        
        Light_LC_PropertyValueGet(pLcParams->pPropertyId,
                                  pLcParams->propertyValue,
                                  LIGHT_CONTROL_TIME_RUN_ON_PID,
                                  LC_TIME_RUN_ON_PID_INDEX,
                                  &tempVal);
        
        /* step resolution = run time to avoid intermediate triggers */
        transitionEvent = Light_LC_TransitionUpdate
          (pLcParams, 1, 1, delayMsOOnOD, tempVal, tempVal);
        
        TRACE_M(TF_LIGHT_LC_M, "RUN -> RUN occupancy on\r\n");
      }
      else
      {
        TRACE_M(TF_LIGHT_LC_M, "No action in RUN, event %d\r\n", event);
      }
      break;
    case FADE:
      if(event == TIMER_TRIGGER) /* Transition step */
      {
        timerMs = Transition_TimerGet(pTrParams);
        /* update current lightness value in transition */
        pLcParams->lcActualSmOut = (MOBLEUINT16)Transition_StateValueGet
          (pLcParams->targetLightnessOut, 
           pLcParams->initialLightnessOut,
           timerMs,
           pTrParams->trTimeMs);
        pLcParams->luxLevelOut = Transition_StateValueGet
          (pLcParams->targetLightLuxOut, 
           pLcParams->initialLightLuxOut,
           timerMs,
           pTrParams->trTimeMs);
        pLcParams->updateLcLinearOut = 1;
      }
      else if (event == TIMER_OFF)
      {
        result = Light_LC_PropertyValueGet(pLcParams->pPropertyId,
                                           pLcParams->propertyValue,
                                           LIGHT_CONTROL_TIME_PROLONG_PID,
                                           LC_TIME_PROLONG_PID_INDEX,
                                           &tempVal);
        
        /* step resolution = run time to avoid intermediate triggers in prolong state */
        transitionEvent = Light_LC_TransitionUpdate
          (pLcParams, 0, 1, 0, tempVal, tempVal);
        
        pLcParams->lcActualSmOut = (MOBLEUINT16)pLcParams->targetLightnessOut;
        pLcParams->luxLevelOut = pLcParams->targetLightLuxOut;
        pLcParams->updateLcLinearOut = 1;
        pLcParams->publish = 1;
        
        TRACE_M(TF_LIGHT_LC_M, "FADE -> PROLONG\r\n");
        pLcParams->state = PROLONG;
      }
      else if (event == LIGHT_OFF)
      {
        result = Light_LC_InitialTargetValueUpdatePid
          (pLcParams, 
           0, 0, 0, 0, 0,
           1,
           LIGHT_CONTROL_LIGHTNESS_STANDBY_PID,
           LC_LIGHTNESS_STANDBY_PID_INDEX,
           LIGHT_CONTROL_AMBIENT_LUXLEVEL_STANDBY_PID,
           LC_AMBIENT_LUXLEVEL_STANDBY_PID_INDEX);
        
        /* update initial with current value of transition parameters */
        pLcParams->initialLightnessOut = (MOBLEUINT16)Transition_StateValueGet
          (pLcParams->targetLightnessOut, 
           pLcParams->initialLightnessOut,
           Transition_TimerGet(pTrParams),
           pTrParams->trTimeMs);
        
        if(optionalParamsOnOffMsg == 1)
        {
          transitionEvent = Light_LC_TransitionUpdate
            (pLcParams, 1, 1, delayMsOOnOD, transitionTimeMsOnOffMsg, stepResMsOnOffMsg);
        }
        else /* optional parameters not present, delay = 0 */
        {
          Light_LC_PropertyValueGet(pLcParams->pPropertyId,
                                    pLcParams->propertyValue,
                                    LIGHT_CONTROL_TIME_FADE_STANDBY_MANUAL_PID,
                                    LC_TIME_FADE_STANDBY_MANUAL_PID_INDEX,
                                    &tempVal);
          
          transitionEvent = Light_LC_TransitionUpdate
            (pLcParams, 1, 1, 0, tempVal, LIGHT_LC_DEFAULT_TRANSITION_RES_MS);
        }
        
        pLcParams->publish = 1;
        TRACE_M(TF_LIGHT_LC_M, "FADE -> PRE FADE STANDBY MANUAL\r\n");
        pLcParams->state = PRE_FADE_STANDBY_MANUAL;
      }
      else if(event == LIGHT_ON)
      {
        if(optionalParamsOnOffMsg == 1)
        {
          transitionEvent = Light_LC_TransitionUpdate
            (pLcParams, 1, 1, delayMsOOnOD, transitionTimeMsOnOffMsg, stepResMsOnOffMsg);
        }
        else /* optional parameters not present, delay = 0 */
        {
          Light_LC_PropertyValueGet(pLcParams->pPropertyId,
                                    pLcParams->propertyValue,
                                    LIGHT_CONTROL_TIME_FADE_ON_PID,
                                    LC_TIME_FADE_ON_PID_INDEX,
                                    &tempVal);
          
          transitionEvent = Light_LC_TransitionUpdate
            (pLcParams, 1, 1, 0, tempVal, LIGHT_LC_DEFAULT_TRANSITION_RES_MS);
        }
        
        result = Light_LC_InitialTargetValueUpdatePid
          (pLcParams, 
           0, 0, 0, 0, 0,
           1,
           LIGHT_CONTROL_LIGHTNESS_ON_PID,
           LC_LIGHTNESS_ON_PID_INDEX,
           LIGHT_CONTROL_AMBIENT_LUXLEVEL_ON_PID,
           LC_AMBIENT_LUXLEVEL_ON_PID_INDEX);
        
        /* update initial with current value of transition parameters */
        pLcParams->initialLightnessOut = (MOBLEUINT16)Transition_StateValueGet
          (pLcParams->targetLightnessOut, 
           pLcParams->initialLightnessOut,
           Transition_TimerGet(pTrParams),
           pTrParams->trTimeMs);
        
        pLcParams->publish = 1;
        TRACE_M(TF_LIGHT_LC_M, "FADE -> PRE FADE ON light on\r\n");
        pLcParams->state = PRE_FADE_ON;
      }
      else if(event == OCCUPANCY_ON)
      {
        result = Light_LC_InitialTargetValueUpdatePid
          (pLcParams, 
           0, 0, 0, 0, 0,
           1,
           LIGHT_CONTROL_LIGHTNESS_ON_PID,
           LC_LIGHTNESS_ON_PID_INDEX,
           LIGHT_CONTROL_AMBIENT_LUXLEVEL_ON_PID,
           LC_AMBIENT_LUXLEVEL_ON_PID_INDEX);
        
        /* update initial with current value of transition parameters */
        pLcParams->initialLightnessOut = (MOBLEUINT16)Transition_StateValueGet
          (pLcParams->targetLightnessOut, 
           pLcParams->initialLightnessOut,
           Transition_TimerGet(pTrParams),
           pTrParams->trTimeMs);
        
        Light_LC_PropertyValueGet(pLcParams->pPropertyId,
                                  pLcParams->propertyValue,
                                  LIGHT_CONTROL_TIME_FADE_ON_PID,
                                  LC_TIME_FADE_ON_PID_INDEX,
                                  &tempVal);
        
        transitionEvent = Light_LC_TransitionUpdate
          (pLcParams, 1, 1, delayMsOOnOD, tempVal, LIGHT_LC_DEFAULT_TRANSITION_RES_MS);
        
        pLcParams->publish = 1;
        TRACE_M(TF_LIGHT_LC_M, "FADE -> PRE FADE ON occupancy on\r\n");
        pLcParams->state = PRE_FADE_ON;
      }
      else
      {
        TRACE_M(TF_LIGHT_LC_M, "No action in FADE, event %d\r\n", event);
      }
      break;
    case PROLONG:
      if (event == TIMER_OFF)
      {
        result = Light_LC_InitialTargetValueUpdatePid
          (pLcParams, 
           1,
           LIGHT_CONTROL_LIGHTNESS_PROLONG_PID,
           LC_LIGHTNESS_PROLONG_PID_INDEX,
           LIGHT_CONTROL_AMBIENT_LUXLEVEL_PROLONG_PID,
           LC_AMBIENT_LUXLEVEL_PROLONG_PID_INDEX,
           1,
           LIGHT_CONTROL_LIGHTNESS_STANDBY_PID,
           LC_LIGHTNESS_STANDBY_PID_INDEX,
           LIGHT_CONTROL_AMBIENT_LUXLEVEL_STANDBY_PID,
           LC_AMBIENT_LUXLEVEL_STANDBY_PID_INDEX);
        
        Light_LC_PropertyValueGet(pLcParams->pPropertyId,
                                  pLcParams->propertyValue,
                                  LIGHT_CONTROL_TIME_FADE_STANDBY_AUTO_PID,
                                  LC_TIME_FADE_STANDBY_AUTO_PID_INDEX,
                                  &tempVal);
        
        transitionEvent = Light_LC_TransitionUpdate
          (pLcParams, 0, 1, 0, tempVal, LIGHT_LC_DEFAULT_TRANSITION_RES_MS);
        
        TRACE_M(TF_LIGHT_LC_M, "PROLONG -> FADE STANDBY AUTO\r\n");
        pLcParams->state = FADE_STANDBY_AUTO;
      }
      else if (event == LIGHT_OFF)
      {
        result = Light_LC_InitialTargetValueUpdatePid
          (pLcParams, 
           1,
           LIGHT_CONTROL_LIGHTNESS_PROLONG_PID,
           LC_LIGHTNESS_PROLONG_PID_INDEX,
           LIGHT_CONTROL_AMBIENT_LUXLEVEL_PROLONG_PID,
           LC_AMBIENT_LUXLEVEL_PROLONG_PID_INDEX,
           1,
           LIGHT_CONTROL_LIGHTNESS_STANDBY_PID,
           LC_LIGHTNESS_STANDBY_PID_INDEX,
           LIGHT_CONTROL_AMBIENT_LUXLEVEL_STANDBY_PID,
           LC_AMBIENT_LUXLEVEL_STANDBY_PID_INDEX);
        
        if(optionalParamsOnOffMsg == 1)
        {
          transitionEvent = Light_LC_TransitionUpdate
            (pLcParams, 1, 1, delayMsOOnOD, transitionTimeMsOnOffMsg, stepResMsOnOffMsg);
        }
        else /* optional parameters not present, delay = 0 */
        {
          Light_LC_PropertyValueGet(pLcParams->pPropertyId,
                                    pLcParams->propertyValue,
                                    LIGHT_CONTROL_TIME_FADE_STANDBY_MANUAL_PID,
                                    LC_TIME_FADE_STANDBY_MANUAL_PID_INDEX,
                                    &tempVal);
          
          transitionEvent = Light_LC_TransitionUpdate
            (pLcParams, 1, 1, 0, tempVal, LIGHT_LC_DEFAULT_TRANSITION_RES_MS);
        }
        
        TRACE_M(TF_LIGHT_LC_M, "PROLONG -> PRE FADE STANDBY MANUAL\r\n");
        pLcParams->state = PRE_FADE_STANDBY_MANUAL;
      }
      else if(event == LIGHT_ON)
      {
        if(optionalParamsOnOffMsg == 1)
        {
          transitionEvent = Light_LC_TransitionUpdate
            (pLcParams, 1, 1, delayMsOOnOD, transitionTimeMsOnOffMsg, stepResMsOnOffMsg);
        }
        else /* optional parameters not present, delay = 0 */
        {
          Light_LC_PropertyValueGet(pLcParams->pPropertyId,
                                    pLcParams->propertyValue,
                                    LIGHT_CONTROL_TIME_FADE_ON_PID,
                                    LC_TIME_FADE_ON_PID_INDEX,
                                    &tempVal);
          
          transitionEvent = Light_LC_TransitionUpdate
            (pLcParams, 1, 1, 0, tempVal, LIGHT_LC_DEFAULT_TRANSITION_RES_MS);
        }
        
        result = Light_LC_InitialTargetValueUpdatePid
          (pLcParams, 
           1,
           LIGHT_CONTROL_LIGHTNESS_PROLONG_PID,
           LC_LIGHTNESS_PROLONG_PID_INDEX,
           LIGHT_CONTROL_AMBIENT_LUXLEVEL_PROLONG_PID,
           LC_AMBIENT_LUXLEVEL_PROLONG_PID_INDEX,
           1,
           LIGHT_CONTROL_LIGHTNESS_ON_PID,
           LC_LIGHTNESS_ON_PID_INDEX,
           LIGHT_CONTROL_AMBIENT_LUXLEVEL_STANDBY_PID,
           LC_AMBIENT_LUXLEVEL_STANDBY_PID_INDEX);
        
        TRACE_M(TF_LIGHT_LC_M, "PROLONG -> PRE FADE ON light on\r\n");
        pLcParams->state = PRE_FADE_ON;
      }
      else if(event == OCCUPANCY_ON)
      {
        Light_LC_PropertyValueGet(pLcParams->pPropertyId,
                                  pLcParams->propertyValue,
                                  LIGHT_CONTROL_TIME_FADE_ON_PID,
                                  LC_TIME_FADE_ON_PID_INDEX,
                                  &tempVal);
        
        transitionEvent = Light_LC_TransitionUpdate
          (pLcParams, 1, 1, delayMsOOnOD, tempVal, LIGHT_LC_DEFAULT_TRANSITION_RES_MS);
        
        result = Light_LC_InitialTargetValueUpdatePid
          (pLcParams, 
           1,
           LIGHT_CONTROL_LIGHTNESS_PROLONG_PID,
           LC_LIGHTNESS_PROLONG_PID_INDEX,
           LIGHT_CONTROL_AMBIENT_LUXLEVEL_PROLONG_PID,
           LC_AMBIENT_LUXLEVEL_PROLONG_PID_INDEX,
           1,
           LIGHT_CONTROL_LIGHTNESS_ON_PID,
           LC_LIGHTNESS_ON_PID_INDEX,
           LIGHT_CONTROL_AMBIENT_LUXLEVEL_ON_PID,
           LC_AMBIENT_LUXLEVEL_ON_PID_INDEX);
        
        TRACE_M(TF_LIGHT_LC_M, "PROLONG -> PRE FADE ON occupancy on\r\n");
        pLcParams->state = PRE_FADE_ON;
      }
      else
      {
        TRACE_M(TF_LIGHT_LC_M, "No action in PROLONG, event %d\r\n", event);
      }
      break;
    case FADE_STANDBY_AUTO:
      if(event == TIMER_TRIGGER)
      {
        timerMs = Transition_TimerGet(pTrParams);
        pLcParams->lcActualSmOut = (MOBLEUINT16)Transition_StateValueGet
          (pLcParams->targetLightnessOut, 
           pLcParams->initialLightnessOut,
           timerMs,
           pTrParams->trTimeMs);
        pLcParams->luxLevelOut = Transition_StateValueGet
          (pLcParams->targetLightLuxOut, 
           pLcParams->initialLightLuxOut,
           timerMs,
           pTrParams->trTimeMs);
        pLcParams->updateLcLinearOut = 1;
      }
      else if(event == TIMER_OFF)
      {
        pLcParams->lcActualSmOut = pLcParams->targetLightnessOut;
        pLcParams->luxLevelOut = pLcParams->targetLightLuxOut;
        pLcParams->updateLcLinearOut = 1;
        pLcParams->publish = 1;
        Binding_LightLcLightOnOff_GenericOnOff(pLcParams->elementIndex, 0);
        pLcParams->state = STANDBY;
        
        NvmStatePowerFlag_Set(LIGHT_LC_NVM_FLAG, pLcParams->elementIndex);
        TRACE_M(TF_LIGHT_LC_M, "FADE STANDBY AUTO -> STANDBY\r\n");
      }
      else if (event == LIGHT_OFF)
      {
        result = Light_LC_InitialTargetValueUpdatePid
          (pLcParams, 
           0, 0, 0, 0, 0,
           1,
           LIGHT_CONTROL_LIGHTNESS_STANDBY_PID,
           LC_LIGHTNESS_STANDBY_PID_INDEX,
           LIGHT_CONTROL_AMBIENT_LUXLEVEL_STANDBY_PID,
           LC_AMBIENT_LUXLEVEL_STANDBY_PID_INDEX);
        
        /* update initial with current value of transition parameters */
        pLcParams->initialLightnessOut = (MOBLEUINT16)Transition_StateValueGet
          (pLcParams->targetLightnessOut, 
           pLcParams->initialLightnessOut,
           Transition_TimerGet(pTrParams),
           pTrParams->trTimeMs);
        
        if(optionalParamsOnOffMsg == 1)
        {
          transitionEvent = Light_LC_TransitionUpdate
            (pLcParams, 1, 1, delayMsOOnOD, transitionTimeMsOnOffMsg, stepResMsOnOffMsg);
        }
        else /* optional parameters not present, delay = 0 */
        {
          Light_LC_PropertyValueGet(pLcParams->pPropertyId,
                                    pLcParams->propertyValue,
                                    LIGHT_CONTROL_TIME_FADE_STANDBY_MANUAL_PID,
                                    LC_TIME_FADE_STANDBY_MANUAL_PID_INDEX,
                                    &tempVal);
          
          transitionEvent = Light_LC_TransitionUpdate
            (pLcParams, 1, 1, 0, tempVal, LIGHT_LC_DEFAULT_TRANSITION_RES_MS);
        }
        
        pLcParams->publish = 1;
        TRACE_M(TF_LIGHT_LC_M, "FADE STANDBY AUTO ->PRE FADE STANDBY MANUAL\r\n");
        pLcParams->state = PRE_FADE_STANDBY_MANUAL;
      }
      else if(event == LIGHT_ON)
      {
        result = Light_LC_InitialTargetValueUpdatePid
          (pLcParams, 
           0, 0, 0, 0, 0,
           1,
           LIGHT_CONTROL_LIGHTNESS_ON_PID,
           LC_LIGHTNESS_ON_PID_INDEX,
           LIGHT_CONTROL_AMBIENT_LUXLEVEL_ON_PID,
           LC_AMBIENT_LUXLEVEL_ON_PID_INDEX);
        
        /* update initial with current value of transition parameters */
        pLcParams->initialLightnessOut = (MOBLEUINT16)Transition_StateValueGet
          (pLcParams->targetLightnessOut, 
           pLcParams->initialLightnessOut,
           Transition_TimerGet(pTrParams),
           pTrParams->trTimeMs);
        
        if(optionalParamsOnOffMsg == 1)
        {
          transitionEvent = Light_LC_TransitionUpdate
            (pLcParams, 1, 1, delayMsOOnOD, transitionTimeMsOnOffMsg, stepResMsOnOffMsg);
        }
        else /* optional parameters not present, delay = 0 */
        {
          Light_LC_PropertyValueGet(pLcParams->pPropertyId,
                                    pLcParams->propertyValue,
                                    LIGHT_CONTROL_TIME_FADE_ON_PID,
                                    LC_TIME_FADE_ON_PID_INDEX,
                                    &tempVal);
          
          transitionEvent = Light_LC_TransitionUpdate
            (pLcParams, 1, 1, 0, tempVal, LIGHT_LC_DEFAULT_TRANSITION_RES_MS);
        }
        
        pLcParams->publish = 1;
        TRACE_M(TF_LIGHT_LC_M, "FADE STANDBY AUTO -> PRE FADE ON light on\r\n");
        pLcParams->state = PRE_FADE_ON;
      }
      else if(event == OCCUPANCY_ON)
      {
        result = Light_LC_InitialTargetValueUpdatePid
          (pLcParams, 
           0, 0, 0, 0, 0,
           1,
           LIGHT_CONTROL_LIGHTNESS_ON_PID,
           LC_LIGHTNESS_ON_PID_INDEX,
           LIGHT_CONTROL_AMBIENT_LUXLEVEL_ON_PID,
           LC_AMBIENT_LUXLEVEL_ON_PID_INDEX);
        
        /* update initial with current value of transition parameters */
        pLcParams->initialLightnessOut = (MOBLEUINT16)Transition_StateValueGet
          (pLcParams->targetLightnessOut, 
           pLcParams->initialLightnessOut,
           Transition_TimerGet(pTrParams),
           pTrParams->trTimeMs);
        
        Light_LC_PropertyValueGet(pLcParams->pPropertyId,
                                  pLcParams->propertyValue,
                                  LIGHT_CONTROL_TIME_FADE_ON_PID,
                                  LC_TIME_FADE_ON_PID_INDEX,
                                  &tempVal);
        
        transitionEvent = Light_LC_TransitionUpdate
          (pLcParams, 1, 1, delayMsOOnOD, tempVal, LIGHT_LC_DEFAULT_TRANSITION_RES_MS);
        
        pLcParams->publish = 1;
        TRACE_M(TF_LIGHT_LC_M, "FADE STANDBY AUTO -> PRE FADE ON occupancy on\r\n");
        pLcParams->state = PRE_FADE_ON;
      }
      else
      {
        TRACE_M(TF_LIGHT_LC_M, "No action in FADE STANDBY AUTO, event %d\r\n", event);
      }
      break;
    case PRE_FADE_STANDBY_MANUAL:
      if(event == TIMER_ON)
      {
        TRACE_M(TF_LIGHT_LC_M, "PRE FADE STANDBY MANUAL -> FADE STANDBY MANUAL\r\n");
        pLcParams->state = FADE_STANDBY_MANUAL;
      }
      else if(event == TIMER_OFF)
      {
        result = Light_LC_InitialTargetValueUpdatePid
          (pLcParams, 
           0, 0, 0, 0, 0,
           1,
           LIGHT_CONTROL_LIGHTNESS_STANDBY_PID,
           LC_LIGHTNESS_STANDBY_PID_INDEX,
           LIGHT_CONTROL_AMBIENT_LUXLEVEL_STANDBY_PID,
           LC_AMBIENT_LUXLEVEL_STANDBY_PID_INDEX);
        pLcParams->lcActualSmOut = pLcParams->targetLightnessOut;
        pLcParams->luxLevelOut = pLcParams->targetLightLuxOut;
        pLcParams->updateLcLinearOut = 1;
        
        pLcParams->publish = 1;
        Binding_LightLcLightOnOff_GenericOnOff(pLcParams->elementIndex, 0);
        NvmStatePowerFlag_Set(LIGHT_LC_NVM_FLAG, pLcParams->elementIndex);
        TRACE_M(TF_LIGHT_LC_M, "PRE FADE STANDBY MANUAL -> STANDBY\r\n");
        pLcParams->state = STANDBY;
      }
      else if(event == LIGHT_ON)
      {
        result = Light_LC_InitialTargetValueUpdatePid
          (pLcParams, 0, 0, 0, 0, 0,
           1,
           LIGHT_CONTROL_LIGHTNESS_ON_PID,
           LC_LIGHTNESS_ON_PID_INDEX,
           LIGHT_CONTROL_AMBIENT_LUXLEVEL_ON_PID,
           LC_AMBIENT_LUXLEVEL_ON_PID_INDEX);
        
        if(optionalParamsOnOffMsg == 1)
        {
          transitionEvent = Light_LC_TransitionUpdate
            (pLcParams, 1, 1, delayMsOOnOD, transitionTimeMsOnOffMsg, stepResMsOnOffMsg);
        }
        else /* optional parameters not present, delay = 0 */
        {
          Light_LC_PropertyValueGet(pLcParams->pPropertyId,
                                    pLcParams->propertyValue,
                                    LIGHT_CONTROL_TIME_FADE_ON_PID,
                                    LC_TIME_FADE_ON_PID_INDEX,
                                    &tempVal);
          
          transitionEvent = Light_LC_TransitionUpdate
            (pLcParams, 1, 1, 0, tempVal, LIGHT_LC_DEFAULT_TRANSITION_RES_MS);
        }
        
        TRACE_M(TF_LIGHT_LC_M, "PRE FADE STANDBY MANUAL -> PRE FADE ON\r\n");
        pLcParams->state = PRE_FADE_ON;
      }
      else
      {
        TRACE_M(TF_LIGHT_LC_M, "No action in PRE FADE STANDBY MANUAL, event %d\r\n", event);
      }
      break;
    case FADE_STANDBY_MANUAL:
      if(event == TIMER_TRIGGER) /* Transition step */
      {
        timerMs = Transition_TimerGet(pTrParams);
        /* update current lightness value in transition */
        pLcParams->lcActualSmOut = (MOBLEUINT16)Transition_StateValueGet
          (pLcParams->targetLightnessOut, 
           pLcParams->initialLightnessOut,
           timerMs,
           pTrParams->trTimeMs);
        pLcParams->luxLevelOut = Transition_StateValueGet
          (pLcParams->targetLightLuxOut, 
           pLcParams->initialLightLuxOut,
           timerMs,
           pTrParams->trTimeMs);
        pLcParams->updateLcLinearOut = 1;
      }
      else if (event == TIMER_OFF)
      {
        pLcParams->lcActualSmOut = pLcParams->targetLightnessOut;
        pLcParams->luxLevelOut = pLcParams->targetLightLuxOut;
        pLcParams->updateLcLinearOut = 1;
        pLcParams->publish = 1;
        Binding_LightLcLightOnOff_GenericOnOff(pLcParams->elementIndex, 0);
        NvmStatePowerFlag_Set(LIGHT_LC_NVM_FLAG, pLcParams->elementIndex);
        TRACE_M(TF_LIGHT_LC_M, "FADE STANDBY MANUAL -> STANDBY\r\n");
        pLcParams->state = STANDBY;
      }
      else if(event == LIGHT_ON)
      {
        /* update initial with current value of transition parameters */
        pLcParams->initialLightnessOut = (MOBLEUINT16)Transition_StateValueGet
          (pLcParams->targetLightnessOut, 
           pLcParams->initialLightnessOut,
           Transition_TimerGet(pTrParams),
           pTrParams->trTimeMs);
        
        result = Light_LC_InitialTargetValueUpdatePid
          (pLcParams, 
           0, 0, 0, 0, 0,
           1,
           LIGHT_CONTROL_LIGHTNESS_ON_PID,
           LC_LIGHTNESS_ON_PID_INDEX,
           LIGHT_CONTROL_AMBIENT_LUXLEVEL_ON_PID,
           LC_AMBIENT_LUXLEVEL_ON_PID_INDEX);
        
        if(optionalParamsOnOffMsg == 1)
        {
          transitionEvent = Light_LC_TransitionUpdate
            (pLcParams, 1, 1, delayMsOOnOD, transitionTimeMsOnOffMsg, stepResMsOnOffMsg);
        }
        else /* optional parameters not present, delay = 0 */
        {
          Light_LC_PropertyValueGet(pLcParams->pPropertyId,
                                    pLcParams->propertyValue,
                                    LIGHT_CONTROL_TIME_FADE_ON_PID,
                                    LC_TIME_FADE_ON_PID_INDEX,
                                    &tempVal);
          
          transitionEvent = Light_LC_TransitionUpdate
            (pLcParams, 1, 1, 0, tempVal, LIGHT_LC_DEFAULT_TRANSITION_RES_MS);
        }
        
        pLcParams->publish = 1;
        TRACE_M(TF_LIGHT_LC_M, "FADE STANDBY MANUAL -> PRE FADE ON\r\n");
        pLcParams->state = PRE_FADE_ON;
      }
      else
      {
        TRACE_M(TF_LIGHT_LC_M, "No action in FADE STANDBY MANUAL, event %d\r\n", event);
      }
      break;
    default:
      break;
    }
  }
  
  /* TRACE_M(TF_LIGHT_LC_M, "Updated state %d transition event %d publish status %d\r\n", 
          pLcParams->state, transitionEvent, pLcParams->publish); */
  
  /* Possibility of recursion 
     Changes, if any pending, would happen except TRANSITION_EVENT_PUBLISH */
  if (transitionEvent == TRANSITION_EVENT_TIMER_START)
  {
    Light_LC_Fsm(TIMER_ON, pLcParams, 0, 0, 0, 0);
  }
  else if(transitionEvent == TRANSITION_EVENT_TIMER_TRIG)
  {
    Light_LC_Fsm(TIMER_TRIGGER, pLcParams, 0, 0, 0, 0);
  }
  else if(transitionEvent == TRANSITION_EVENT_TIMER_STOP)
  {
    Light_LC_Fsm(TIMER_OFF, pLcParams, 0, 0, 0, 0);
  }
  else if(transitionEvent == TRANSITION_EVENT_PUBLISH)
  {
    LcServerP->publish = 1;
  }
  else
  {
    /* time to exit */
  }
  
  if(pLcParams->updateLcLinearOut == 1)
  {
    pLcParams->updateLcLinearOut = 0;
    
    /* Lightness Linear = Ceiling (65535*(Light Lightness Actual/65535)^2) */
    pLcParams->lightnessLinearUnreg = ((pLcParams->lcActualSmOut*pLcParams->lcActualSmOut)+65534)/65535;
    pLcParams->lcLinearOut = MAX(pLcParams->lightnessLinearUnreg, Light_LC_PIregulator(pLcParams));
    Binding_LcLinearOut_LightLightnessLinear(pLcParams->elementIndex, pLcParams->lcLinearOut);
  }
  
  return result;
}


/**
  * @brief  Light LC property table initialization
  * @param  Reference to LC parameters
  * @retval Fail if property id or index inappropriate, else success
  */
static
MOBLE_RESULT Light_LC_PropertyTableInit(lc_param_t* pLcParams)
{
  float tempFloat = 0;
  const float* pFtemp = &tempFloat;
  const MOBLEUINT32* pU32temp = (MOBLEUINT32*)pFtemp;
  
  MOBLE_RESULT result = MOBLE_RESULT_FAIL;
  
  result = Light_LC_PropertyValueSet(pLcParams->pPropertyId,
                                     pLcParams->propertyValue,
                                     LIGHT_CONTROL_REGULATOR_ACCURACY_PID,
                                     LC_REGULATOR_ACCURACY_PID_SIZE,
                                     4); /* Default value 2.0, resolution 0.5 */
  if(result == MOBLE_RESULT_SUCCESS)
  {
    tempFloat = 25.0; /* Default value */
    result = Light_LC_PropertyValueSet(pLcParams->pPropertyId,
                                       pLcParams->propertyValue,
                                       LIGHT_CONTROL_REGULATOR_KID_PID,
                                       LC_REGULATOR_KID_PID_SIZE,
                                       *pU32temp);
  }
  
  if(result == MOBLE_RESULT_SUCCESS)
  {
    tempFloat = 250.0; /* Default value */
    result = Light_LC_PropertyValueSet(pLcParams->pPropertyId,
                                       pLcParams->propertyValue,
                                       LIGHT_CONTROL_REGULATOR_KIU_PID,
                                       LC_REGULATOR_KIU_PID_SIZE,
                                       *pU32temp);
  }
  
  if(result == MOBLE_RESULT_SUCCESS)
  {
    tempFloat = 80.0; /* Default value */
    result = Light_LC_PropertyValueSet(pLcParams->pPropertyId,
                                       pLcParams->propertyValue,
                                       LIGHT_CONTROL_REGULATOR_KPD_PID,
                                       LC_REGULATOR_KPD_PID_SIZE,
                                       *pU32temp);
  }
  
  if(result == MOBLE_RESULT_SUCCESS)
  {
    tempFloat = 80.0; /* Default value */
    result = Light_LC_PropertyValueSet(pLcParams->pPropertyId,
                                       pLcParams->propertyValue,
                                       LIGHT_CONTROL_REGULATOR_KPU_PID,
                                       LC_REGULATOR_KPU_PID_SIZE,
                                       *pU32temp);
  }
  
  if(result == MOBLE_RESULT_SUCCESS)
  {
    /* Lightness On non-zero value required by PTS */
    result = Light_LC_PropertyValueSet(pLcParams->pPropertyId,
                                       pLcParams->propertyValue,
                                       LIGHT_CONTROL_LIGHTNESS_ON_PID,
                                       LC_LIGHTNESS_ON_PID_SIZE,
                                       0xffff);
  }
  
  if(result == MOBLE_RESULT_SUCCESS)
  {
    result = Light_LC_PropertyValueSet(pLcParams->pPropertyId,
                                       pLcParams->propertyValue,
                                       LIGHT_CONTROL_LIGHTNESS_STANDBY_PID,
                                       LC_LIGHTNESS_STANDBY_PID_SIZE,
                                       0x0000);
  }
  
  if(result == MOBLE_RESULT_SUCCESS)
  {
    /* Lightness Prolong non-zero value for init demo */
    result = Light_LC_PropertyValueSet(pLcParams->pPropertyId,
                                       pLcParams->propertyValue,
                                       LIGHT_CONTROL_LIGHTNESS_PROLONG_PID,
                                       LC_LIGHTNESS_PROLONG_PID_SIZE,
                                       0x3fff);
  }
  
  if(result == MOBLE_RESULT_SUCCESS)
  {
    /* Fade On non-zero value for init demo */
    result = Light_LC_PropertyValueSet(pLcParams->pPropertyId,
                                       pLcParams->propertyValue,
                                       LIGHT_CONTROL_TIME_FADE_ON_PID,
                                       LC_TIME_FADE_ON_PID_SIZE,
                                       3000);
  }
  
  if(result == MOBLE_RESULT_SUCCESS)
  {
    /* Run On non-zero value for init demo */
    result = Light_LC_PropertyValueSet(pLcParams->pPropertyId,
                                       pLcParams->propertyValue,
                                       LIGHT_CONTROL_TIME_RUN_ON_PID,
                                       LC_TIME_RUN_ON_PID_SIZE,
                                       5000);
  }
  
  if(result == MOBLE_RESULT_SUCCESS)
  {
    /* Fade non-zero value for init demo */
    result = Light_LC_PropertyValueSet(pLcParams->pPropertyId,
                                       pLcParams->propertyValue,
                                       LIGHT_CONTROL_TIME_FADE_PID,
                                       LC_TIME_FADE_PID_SIZE,
                                       2000);
  }
  
  if(result == MOBLE_RESULT_SUCCESS)
  {
    /* Prolong non-zero value for init demo */
    result = Light_LC_PropertyValueSet(pLcParams->pPropertyId,
                                       pLcParams->propertyValue,
                                       LIGHT_CONTROL_TIME_PROLONG_PID,
                                       LC_TIME_PROLONG_PID_SIZE,
                                       2000);
  }
  
  if(result == MOBLE_RESULT_SUCCESS)
  {
    /* Fade Standby Auto non-zero value for init demo */
    result = Light_LC_PropertyValueSet(pLcParams->pPropertyId,
                                       pLcParams->propertyValue,
                                       LIGHT_CONTROL_TIME_FADE_STANDBY_AUTO_PID,
                                       LC_TIME_FADE_STANDBY_AUTO_PID_SIZE,
                                       1000);
  }
  
  if(result == MOBLE_RESULT_SUCCESS)
  {
    /* Fade Standby Manual non-zero value for init demo */
    result = Light_LC_PropertyValueSet(pLcParams->pPropertyId,
                                       pLcParams->propertyValue,
                                       LIGHT_CONTROL_TIME_FADE_STANDBY_MANUAL_PID,
                                       LC_TIME_FADE_STANDBY_MANUAL_PID_SIZE,
                                       1000);
  }
  
  return result;
}


/**
  * @brief  Get LC parameters to be stored in nvm
  * @param  element index
  * @param  buffer for parameters
  * @param  size of buffer as set
  * @retval None
  */
__weak
void LightLC_SaveModelStates(MOBLEUINT8 elementIndex,
                            MOBLEUINT8* lightLcNvmParamsBuff, 
                             MOBLEUINT8 lcNvmParamsSize)
{
  MOBLEUINT8 mode = 0;
  MOBLEUINT8 onOff = 0;
  
  if(LcServerP != NULL)
  {
    /* only for element index for which LC server is supported */
    if(elementIndex == LcServerP->elementIndex
       || lcNvmParamsSize >= 4)
    {
      if(LcServerP->state == OFF)
      {
        mode = 0;
      }
      else
      {
        mode = 1;
      }
      
      if(LcServerP->state == OFF ||
         LcServerP->state == STANDBY ||
         LcServerP->state == FADE_STANDBY_AUTO ||
         LcServerP->state == PRE_FADE_STANDBY_MANUAL ||
         LcServerP->state == FADE_STANDBY_MANUAL)
      {
        onOff = 0; /* off */
      }
      else
      {
        onOff = 1;
      }

      lightLcNvmParamsBuff[0] = mode;
      lightLcNvmParamsBuff[1] = LcServerP->occupancyMode;
      lightLcNvmParamsBuff[2] = onOff;
      
      /* 
         lc mode 1 byte
         lc occupancy mode 1 byte 
         lc on off 1 byte
         reserved 1 byte
      */
      
      TRACE_M(TF_LIGHT_LC_M, "mode %d occupancy mode %d light onOff %d\r\n",
              lightLcNvmParamsBuff[0], lightLcNvmParamsBuff[1], lightLcNvmParamsBuff[2]);
    }
    else
    {
      TRACE_M(TF_LIGHT_LC_M, "Element index %d not supported by LC Server\r\n", elementIndex);
    }
  }
  else
  {
    TRACE_M(TF_LIGHT_LC_M, "LC Server not initialized\r\n");
  }
}


/**
  * @brief  Function used to restore the light LC with respect to Power on off
  *         value
  * @param  Element index
  * @param  Array of saved data
  * @param  Binded Generic onPowerUp
  * @param  Binded Light Lightness Default
  * @param  Binded Light Lightness Last
  * @param  Binded Light Lightness Actual last known value
  * @param  Reference to Light Actual to be set
  * @retval If set Light Actual to be updated
  */
__weak
MOBLEUINT8 Light_LC_OnPowerUp(MOBLEUINT8 elementIndex,
                        MOBLEUINT8 const *pModelValue_Load, 
                        MOBLEUINT8 genericOnPowerUp,
                        MOBLEUINT16 lightDefault,
                        MOBLEUINT16 lightLast,
                              MOBLEUINT16 lightActualLKV,
                              MOBLEUINT16* pLightActualToBeSet)
{
  MOBLE_RESULT result = MOBLE_RESULT_SUCCESS;
  MOBLEUINT8 onOff = 0; /* target */
  MOBLEUINT16 lightActual = 0;
  MOBLEUINT8 updateLightActual = 0;
  MOBLEUINT8 lcModeLKV = pModelValue_Load[0]; /* last known value */
  MOBLEUINT8 lcOmLKV = pModelValue_Load[1]; /* last known value */
  MOBLEUINT8 lcOnOffLKV = pModelValue_Load[2]; /* last known value */
  MOBLEUINT8 mode = 0;
  lc_param_t* pLcParams = NULL;
  
  if(LcServerP != NULL)
  {
    if(elementIndex == LcServerP->elementIndex)
    {
      TRACE_M(TF_LIGHT_LC_M, "OnPowerUp %d Mode lkv %d OM lkv %d OnOff lkv %d\r\n",
                             genericOnPowerUp, lcModeLKV, lcOmLKV, lcOnOffLKV);
      
      pLcParams = LcServerP;

      /* 
      6.5.1.2 PowerUp sequence behavior
      */
      
      if(genericOnPowerUp == GENERIC_POWER_OFF_STATE)
      {
        /* Set Light Lightness Actual to 0 */
        lightActual = 0;
        updateLightActual = 1;
        
        /* Assuming LC SM is off by default
           Mode off
           Occupancy mode as last known value
           OnOff 0 */
        mode = 0;
        pLcParams->occupancyMode = lcOmLKV;
        onOff = 0;
      }
      else if(genericOnPowerUp == GENERIC_POWER_ON_STATE)
      {
        if(lightDefault == 0)
        {
          /* If light lightness default is 0, set Light Lightness Actual to Light Lightness Last */
          lightActual = lightLast;
        }
        else
        {
          /* else Set Light Lightness Actual to Light Lightness Default */
          lightActual = lightDefault;
        }
        
        updateLightActual = 1;
        
        /* LC SM off
           Mode off
           Occupancy mode as last known value
           OnOff 0 */
        mode = 0;
        pLcParams->occupancyMode = lcOmLKV;
        onOff = 0;
      }
      else if(genericOnPowerUp == GENERIC_POWER_RESTORE_STATE)
      {
        if(lcModeLKV == 0)
        {
          lightActual = lightActualLKV;
          updateLightActual = 1;
          
          /* LC SM off
             Mode off */
          mode = 0;
        }
        else
        {
          /* LC SM off
             Mode on 
             equivalent to standby */
          mode = 1;
          
          /* 
             Workaround for not saving property values in nvm
             Inserts lag in state machine after reset
          */
          Light_LC_PropertyValueSet(pLcParams->pPropertyId,
                                    pLcParams->propertyValue,
                                    LIGHT_CONTROL_TIME_RUN_ON_PID,
                                    LC_TIME_RUN_ON_PID_SIZE,
                                    20000);
        }
        
        /* Occupancy mode as last known value
          OnOff 0 */
        pLcParams->occupancyMode = lcOmLKV;
        onOff = lcOnOffLKV;
      }
      else
      {
        result = MOBLE_RESULT_INVALIDARG;
        TRACE_M(TF_LIGHT_LC_M, "Generic OnPowerUp invalid %d\r\n", genericOnPowerUp);
      }
    }
    else
    {
      result = MOBLE_RESULT_INVALIDARG;
      TRACE_M(TF_LIGHT_LC_M, "Element index %d not supported by LC Server\r\n", elementIndex);
    }
  }
  else
  {
    result = MOBLE_RESULT_FAIL;
    TRACE_M(TF_LIGHT_LC_M, "LC Server not initialized\r\n");
  }
  
  TRACE_M(TF_LIGHT_LC_M, "Updated Mode %d OM %d OnOff %d light actual %d\r\n",
           mode, pLcParams->occupancyMode, onOff, lightActual);
  
  if (result == MOBLE_RESULT_SUCCESS)
  {
    if(updateLightActual == 1)
    {
      /* Set Light Actual value */
      *pLightActualToBeSet = lightActual;
    }
    
    if(mode == 0)
    {
      TRACE_M(TF_LIGHT_LC_M, "LC initialized in OFF\r\n");
    }
    else
    {
      Light_LC_Fsm(MODE_ON, pLcParams, 0, 0, 0, 0);
      
      if(onOff == 0)
      {
        Light_LC_Fsm(LIGHT_OFF, pLcParams, 0, 0, 0, 0);
      }
      else
      {
        Light_LC_Fsm(LIGHT_ON, pLcParams, 0, 0, 0, 0);
      }
    }
  }
  else
  {
    /*  */
  }
  
  return updateLightActual;
}


/**
  * @brief  Set LC Light OnOff Transition Time to 0
  *         Transitions are immediate and run time set to 20 seconds (PTS requirement)
  *         Used for PTS test requirements
  * @param  Element index
  * @retval result as set by Light_LC_PropertySet
  */
__weak
MOBLE_RESULT Light_LC_SetTransitionTimeZero(MOBLEUINT8 elementIndex)
{
  MOBLE_RESULT result = MOBLE_RESULT_SUCCESS;
  MOBLEUINT8 lcsElementIndex = 1;
  lc_param_t* pLcParams = LcServerP;
  
  if(LcServerP != NULL &&
     lcsElementIndex == elementIndex)
  {
    result = Light_LC_PropertyValueSet(pLcParams->pPropertyId,
                                       pLcParams->propertyValue,
                                       LIGHT_CONTROL_TIME_FADE_ON_PID,
                                       LC_TIME_FADE_ON_PID_SIZE,
                                       0);
    result = Light_LC_PropertyValueSet(pLcParams->pPropertyId,
                                       pLcParams->propertyValue,
                                       LIGHT_CONTROL_TIME_FADE_PID,
                                       LC_TIME_FADE_PID_SIZE,
                                       0);
    result = Light_LC_PropertyValueSet(pLcParams->pPropertyId,
                                       pLcParams->propertyValue,
                                       LIGHT_CONTROL_TIME_FADE_STANDBY_AUTO_PID,
                                       LC_TIME_FADE_STANDBY_AUTO_PID_SIZE,
                                       0);
    result = Light_LC_PropertyValueSet(pLcParams->pPropertyId,
                                       pLcParams->propertyValue,
                                       LIGHT_CONTROL_TIME_FADE_STANDBY_MANUAL_PID,
                                       LC_TIME_FADE_STANDBY_MANUAL_PID_SIZE,
                                       0);
    result = Light_LC_PropertyValueSet(pLcParams->pPropertyId,
                                       pLcParams->propertyValue,
                                       LIGHT_CONTROL_TIME_PROLONG_PID,
                                       LC_TIME_PROLONG_PID_SIZE,
                                       0);
    result = Light_LC_PropertyValueSet(pLcParams->pPropertyId,
                                       pLcParams->propertyValue,
                                       LIGHT_CONTROL_TIME_RUN_ON_PID,
                                       LC_TIME_RUN_ON_PID_SIZE,
                                       20000);
  }
  else
  {
    TRACE_M(TF_LIGHT_LC_M, "LC server not initialized or wrong element index %d\r\n", elementIndex);
    result = MOBLE_RESULT_FAIL;
  }
  
  return result;
}


/**
  * @brief  Light LC Server also support Sensor Client processing of properties
  *         MOTION_SENSED_PID, PEOPLE_COUNT_PID, PRESENCE_DETECTED_PID,
  *         TIME_SINCE_MOTION_SENSED_PID and PRESENT_AMBIENT_LIGHT_LEVEL_PID
  *         Used for reporting occupancy sensing
  *         Mandatory requirement for LC Server to process sensor client with these property IDs
  * @param  Targeted LC Server lement index
  * @param  Property id
  * @param  Property value
  * @retval Success if property id and reported values are appropriate
  */
__weak
MOBLE_RESULT Light_LC_SensorPropertyUpdate(MOBLEUINT8 lcsElementIndex,
                                           MOBLEUINT16 sensorPropertyId,
                                           MOBLEUINT32 value)
{
  MOBLE_RESULT result = MOBLE_RESULT_SUCCESS;
  MOBLEUINT8 occupancyState = 0;
  MOBLEUINT32 tempVal = 0;
  lc_param_t* pLcParams = NULL;
  MOBLEUINT32 delayMotionSenseMs = 0;
  
  if(LcServerP != NULL &&
     LcServerP->elementIndex == lcsElementIndex)
  {
    pLcParams = LcServerP;
  }
  else
  {
    result = MOBLE_RESULT_FAIL;
    TRACE_M(TF_LIGHT_LC_M, "LC server not initialized or wrong element index %d\r\n", lcsElementIndex);
  }
  
  if(result == MOBLE_RESULT_SUCCESS)
  {
    switch(sensorPropertyId)
    {
    case MOTION_SENSED_PID:
      if(value <= 100 || value == 0xFF)
      {
        if(value > 0)
        {
          occupancyState = 1;
        }
        else
        {
          result = MOBLE_RESULT_FALSE;
        }
      }
      else /* Prohibited values */
      {
        result = MOBLE_RESULT_FAIL;
      }
      break;
    case PEOPLE_COUNT_PID:
      if(value > 0)
      {
        occupancyState = 1;
      }
      else
      {
        result = MOBLE_RESULT_FALSE;
      }
      break;
    case PRESENCE_DETECTED_PID:
      if(value < 2)
      {
        if(value == 1)
        {
          occupancyState = 1;
        }
        else
        {
          result = MOBLE_RESULT_FALSE;
        }
      }
      else /* Prohibited values */
      {
        result = MOBLE_RESULT_FAIL;
      }
      break;
    case TIME_SINCE_MOTION_SENSED_PID:
      if(value < 65535)
      {
        delayMotionSenseMs = value*1000;/* seconds to milliseconds */
        occupancyState = 1;
      }
      break;
    case PRESENT_AMBIENT_LIGHT_LEVEL_PID:
      if(value < 0xFFFFFF)
      {
        pLcParams->ambientLuxLevel = value;
        result = Light_LC_Fsm(AMBIENT_LUX_MODIFIED,
                              pLcParams,
                              0, 0, 0, 0);
      }
      break;
    default:
      result = MOBLE_RESULT_FAIL;
      TRACE_M(TF_LIGHT_LC_M, "Property (%x) not relevant for LC server model\r\n", sensorPropertyId);
      break;
    }
  }
  
  if(occupancyState == 1)
  {
    if(pLcParams->occupancyMode == 1)
    {
      /* Get time occupancy delay property value */
      Light_LC_PropertyValueGet(pLcParams->pPropertyId,
                                pLcParams->propertyValue,
                                LIGHT_CONTROL_TIME_OCCUPANCY_DELAY_PID, 
                                LC_TIME_OCCUPANCY_DELAY_PID_INDEX,
                                &tempVal);
      
      /* Adjust delay for occupancy detect event */
      if(delayMotionSenseMs < tempVal)
      {
        tempVal = tempVal - delayMotionSenseMs;
      }
      
      result = Light_LC_Fsm(OCCUPANCY_ON,
                            pLcParams,
                            0,
                            tempVal,/* delay in milliseconds for processing of occupancy on */
                            0, 0);
      
      TRACE_M(TF_LIGHT_LC_M, "Occupancy detected\r\n");
    }
    else
    {
      TRACE_M(TF_LIGHT_LC_M, "Occupancy rejected\r\n");
    }
  }
  
  return result;
}


/**
  * @brief  Unsolicited change in Light Lightness Linear disables binding
  *         between LC Linear Out and Light Lightness Linear and
  *         LC state machine mode = 0b0
  * @param  Binded Light Lightness Linear element index. It should be on main element
  * @retval None
  */
__weak
void Light_LC_LigtnessLinearUnsolicitedChange(MOBLEUINT8 lightnessLinearelementIndex)
{
  TRACE_M(TF_LIGHT_LC_M, "Light Lightness Linear element index %d\r\n", lightnessLinearelementIndex);
  
  if(LcServerP != NULL)
  {
    Light_LC_Fsm(MODE_OFF, LcServerP, 0, 0, 0, 0);
  }
  else
  {
    TRACE_M(TF_LIGHT_LC_M, "LC server not initialized");
  }
}


/**
  * @brief  Light_LC_SleepDurationMs_Get
  *         returns max time after which call to process Light_LC_Process is required
  *         to adjust internal states or to adjust Light Lightness or to publish status
  * @param  None
  * @retval sleep time in milliseconds
  */
__weak
MOBLEUINT32 Light_LC_SleepDurationMs_Get(void)
{
  MOBLEUINT32 sleepDuration = MAX_U32_VALUE;
  lc_param_t* pLcParams = NULL;
  
  if(LcServerP != NULL)
  {
    pLcParams = LcServerP;
    sleepDuration = Transition_TimeToNextActionGet(&pLcParams->transitionParams);
  }
  
  return sleepDuration;
}


/**
  * @brief  Publish Light LC status
  * @param  element index for which Light LC status to be published
  * @retval Fail if LC server not supported on element else as set by status
  */
__weak
MOBLE_RESULT Light_LC_Send(MOBLEUINT8 elementIndex)
{
  MOBLE_RESULT result = MOBLE_RESULT_SUCCESS;
  MODEL_MessageHeader_t pmsgParam;
  
  if(LcServerP != NULL)
  {
    if(LcServerP->elementIndex == elementIndex)
    {
      pmsgParam.elementIndex = elementIndex;
      TRACE_M(TF_LIGHT_LC_M, "Publish status for element index %d\r\n", pmsgParam.elementIndex);
    }
    else
    {
      TRACE_M(TF_LIGHT_LC_M, "Publish status element index %d not supported\r\n", elementIndex);
      result = MOBLE_RESULT_FAIL;
    }
    
    if(MOBLE_SUCCEEDED(result))
    {
      result = Light_LC_OnOffStatus(LcServerP,
                                    &pmsgParam,
                                    STATUS_SEND_PUBLISH);
    }
  }
  else
  {
    result = MOBLE_RESULT_FAIL;
    TRACE_M(TF_LIGHT_LC_M, "LC server not initialized");
  }
  return result;
}


/**
  * @brief  To update LC on off state
  *         Can be used for updating binded LC on off with Generic on off
  * @param  Element index
  * @param  On off state
  * @param  Optional parameters flag
  * @param  Delay from on off message
  * @param  Step resolution from on off message
  * @param  Transition time from on off message
  */
__weak
MOBLE_RESULT Light_LC_LcOnOffUpdate(MOBLEUINT8 elementIndex, 
                                    MOBLEUINT8 onOff,
                                    MOBLEUINT8 optionalParams,
                                    MOBLEUINT32 delayMsOnOffMsg,
                                    MOBLEUINT32 stepResMsOnOffMsg,
                                    MOBLEUINT32 trTimeMsOnOffMsg)
{
  MOBLE_RESULT result = MOBLE_RESULT_FAIL;
  
  if(LcServerP != NULL)
  {
    if(elementIndex == LcServerP->elementIndex)
    {
      if(onOff == 0)
      {
        result = Light_LC_Fsm(LIGHT_OFF, 
                              LcServerP, 
                              optionalParams, 
                              delayMsOnOffMsg, 
                              stepResMsOnOffMsg, 
                              trTimeMsOnOffMsg);
      }
      else if(onOff == 1)
      {
        result = Light_LC_Fsm(LIGHT_ON, 
                              LcServerP, 
                              optionalParams, 
                              delayMsOnOffMsg, 
                              stepResMsOnOffMsg, 
                              trTimeMsOnOffMsg);
      }
    }
    else
    {
      TRACE_M(TF_LIGHT_LC_M, "Element index mismatch %d/%d", elementIndex, LcServerP->elementIndex);
      result = MOBLE_RESULT_INVALIDARG;
    }
  }
  else
  {
    result = MOBLE_RESULT_FAIL;
    TRACE_M(TF_LIGHT_LC_M, "LC server not initialized");
  }
  
  return result;
}


/**
  * @brief  Execute transition and update publish status
  * @param  None
  * @retval None
  */
__weak
void Light_LC_Process(void)
{
  transition_event_e transitionEvent = TRANSITION_EVENT_NO;
  
  if(LcServerP != NULL)
  {
    /* One instance of LC server is supported */
    if(LcServerP->transitionParams.trStatus != TRANSITION_STATUS_STOP)
    {
      transitionEvent = Transition_Sm(&LcServerP->transitionParams, 0);
      
      if (transitionEvent == TRANSITION_EVENT_TIMER_START)
      {
        Light_LC_Fsm(TIMER_ON, LcServerP, 0, 0, 0, 0);
      }
      else if(transitionEvent == TRANSITION_EVENT_TIMER_TRIG)
      {
        Light_LC_Fsm(TIMER_TRIGGER, LcServerP, 0, 0, 0, 0);
      }
      else if(transitionEvent == TRANSITION_EVENT_TIMER_STOP)
      {
        Light_LC_Fsm(TIMER_OFF, LcServerP, 0, 0, 0, 0);
      }
      else if(transitionEvent == TRANSITION_EVENT_PUBLISH)
      {
        LcServerP->publish = 1;
      }
      else
      {
        /* time to exit */
      }
    }
    else
    {
      /* Timer is stopped */
    }
    
    /* LC Publish */
    if(LcServerP->publish == 1)
    {
      Light_LC_Send(LcServerP->elementIndex);
      LcServerP->publish = 0;
    }
  }
}


/**
  * @brief  Initialize Light LC server model
  * @param  Reference to LC Server buffer
  * @param  LC server element index
  * @param  Reference to application callbacks of LC server
  * @param  Buffer size allocated to LC Server
  * @retval success if initialization successful
  *         else fail
  */
__weak
MOBLE_RESULT Light_LCs_Init(void* lcsBuff,
                            MOBLEUINT8 lcsElementIndex,
                            const light_lc_cb_t* lcs_cb,
                            MOBLEUINT16 sizeBuff)
{
  MOBLE_RESULT result = MOBLE_RESULT_SUCCESS;
  MOBLEUINT16 requiredBuffSize = sizeof(lc_param_t);
  
  TRACE_M(TF_LIGHT_LC_M, "Element index %d Required buffer size %d\r\n", lcsElementIndex, requiredBuffSize);
  
  if(sizeBuff < requiredBuffSize)
  {
    result = MOBLE_RESULT_FAIL;
    TRACE_M(TF_LIGHT_LC_M, "Buffer size %d not sufficient\r\n", sizeBuff);
  }
  
  if(result == MOBLE_RESULT_SUCCESS)
  {
    LcServerP = lcsBuff;
    
    if(LC_PropertyId != NULL)
    {
      LcServerP->pPropertyId = LC_PropertyId;
    }
    else
    {
      result = MOBLE_RESULT_FAIL;
      TRACE_M(TF_LIGHT_LC_M, "Property Id table not found\r\n");
    }
    
    if(result == MOBLE_RESULT_SUCCESS)
    {
      if(lcs_cb != NULL)
      {
        LcServerP->p_cb = lcs_cb;
      }
      
      LcServerP = (lc_param_t*)lcsBuff;
      LcServerP->elementIndex = lcsElementIndex;
      
      /* Initialize property IDs */
      result = Light_LC_PropertyTableInit(LcServerP);
      
      /* Set default values of properties */
      LcServerP->occupancyMode = 1;
      
      LcServerP->ambientLuxLevel = 0xFFFFFF; /* corresponds to value not known */
      
      /* Retrieving property values from NVM pending */
    }
  }
  
  if(result == MOBLE_RESULT_SUCCESS)
  {
    TRACE_M(TF_LIGHT_LC_M, "Light LC Server initialized on element index %d\r\n", LcServerP->elementIndex);
    
    for(MOBLEUINT8 count=0; count<LC_PROPERTY_TABLE_COUNT; count++)
    {
      TRACE_M(TF_LIGHT_LC_M, "LC property id %x size %d value index %d value %lx\r\n",
              LcServerP->pPropertyId[count].id,
              LcServerP->pPropertyId[count].size,
              LcServerP->pPropertyId[count].valueIndex,
              LcServerP->propertyValue[count]);
    }
  }
  
  return result;
}


/**
  * @brief  Dependent on user configuration
  *         To check avaialability and consistency of supported model required for LC Server
  *         Limitation: Check for sensor client to process sensor status
  * @param  Reference to LC Server element index, LC Server would be initialized on this element
  * @param  No of elements
  * @param  LC Server elements as defined by user
  * @param  LC Setup Server elements as defined by user
  * @param  Generic On Off Server elements as defined by user
  * @param  Generic On Off Setup Server elements as defined by user
  * @param  Light Lightness Server elements as defined by user
  * @retval Success if dependencies are appropriate
  */
__weak
MOBLE_RESULT ExtractLcServerElementIndex(MOBLEUINT8* pLcsElementIndex,
                                         MOBLEUINT8 noOfElements,
                                         MOBLEUINT8 lcServer,
                                         MOBLEUINT8 lcSetupServer,
                                         MOBLEUINT8 genericOnOffServer,
                                         MOBLEUINT8 genericPowerOnOffServer,
                                         MOBLEUINT8 lightLightnessServer)
{
  MOBLE_RESULT result = MOBLE_RESULT_FAIL;
  MOBLEUINT8 lcServerCount = 0;
  
  if(noOfElements < 2)
  {
    result = MOBLE_RESULT_FAIL;
    TRACE_M(TF_LIGHT_LC_M, "At least 2 elements are required for LC server\r\n");
  }
  else
  {
    for(uint8_t count = 0; count < noOfElements; count++)
    {
      /* Extract LC server element index */
      if((lcServer & (1 << count)) == (1 << count)) 
      {
        lcServerCount++;
        
        /* Checks only for first instance of Light LC Server */
        if(lcServerCount == 1)
        {
          *pLcsElementIndex = count; /* First element index supporting LC server */
          
          /* LC Setup Server should be supported on same element as of LC Server */
          if((lcSetupServer & (1 << count)) == (1 << count))
          {
            result = MOBLE_RESULT_SUCCESS;
          }
          else
          {
            TRACE_M(TF_LIGHT_LC_M, "LC Setup Server not found, element index %d\r\n", count);
            result = MOBLE_RESULT_FAIL;
          }
          
          /* Generic OnOff Server should be supported on LC Server element */
          if(result == MOBLE_RESULT_SUCCESS )
          {
            if((genericOnOffServer & (1 << count)) == (1 << count))
            {
              result = MOBLE_RESULT_SUCCESS;
            }
            else
            {
              TRACE_M(TF_LIGHT_LC_M, "Generic OnOff Server not found, element index %d\r\n", count);
              result = MOBLE_RESULT_FAIL;
            }
          }
          
          /* Light Lightness should be supported on main element (different from Light LC Server element) */
          if(result == MOBLE_RESULT_SUCCESS)
          {
            if((lightLightnessServer & (1 << count)) == (1 << count))
            {
              TRACE_M(TF_LIGHT_LC_M, "Light Lightness Server found on Light LC Server element, element index %d\r\n", count);
              result = MOBLE_RESULT_FAIL;
            }
          }
          
          /* Light Lightness Server is expected on main element and not on Light LC element */
          if(result == MOBLE_RESULT_SUCCESS)
          {
            if((lightLightnessServer & 1) != (LIGHT_LIGHTNESS_SERVER_MAIN_ELEMENT_INDEX+1))
            {
              TRACE_M(TF_LIGHT_LC_M, "Light Lightness Server not found on main element\r\n");
              result = MOBLE_RESULT_FAIL;
            }
          }
        }
      }
      else
      {
        /* Light LC Server not found on this element */
      }
    }
  }
  
  /* LC server and LC Server setup are available on same element */
  if(MOBLE_SUCCEEDED(result)) 
  {
    if(lcServerCount > 1)
    {
      result = MOBLE_RESULT_FALSE;
      /* 
         LC Server is supported on more than one element
         Only one instance of LC server is supported by module
         First element with LC server is used for initialiazing LC server
     */
    }
  }
  else
  {
    /* */
  }
  
  return result;
}

/**
* @}
*/

/**
* @}
*/



