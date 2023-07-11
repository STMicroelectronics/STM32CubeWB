/**
******************************************************************************
* @file    generic.h
* @author  BLE Mesh Team
* @brief   Header file for the user application file 
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __GENERIC_H
#define __GENERIC_H

/* Includes ------------------------------------------------------------------*/
#include "types.h"
#include "ble_mesh.h"


/* Exported macro ------------------------------------------------------------*/

/******************************************************************************/
/********** Following Section defines the Opcodes for the Messages ************/
/******************************************************************************/

/* 7.1 Messages summary Page 298 */
/* Generic OnOff Server Model Opcode */
#define GENERIC_ON_OFF_GET                                                0x8201
#define GENERIC_ON_OFF_SET_ACK                                            0x8202
#define GENERIC_ON_OFF_SET_UNACK                                          0x8203
#define GENERIC_ON_OFF_STATUS                                             0x8204

/* Generic Level Server Model Opcode */
#define GENERIC_LEVEL_GET                                                 0x8205
#define GENERIC_LEVEL_SET_ACK                                             0x8206
#define GENERIC_LEVEL_SET_UNACK                                           0x8207
#define GENERIC_LEVEL_STATUS                                              0x8208
#define GENERIC_DELTA_SET             0x8209
#define GENERIC_DELTA_SET_UNACK       0x820A
#define GENERIC_MOVE_SET        0x820B
#define GENERIC_MOVE_SET_UNACK  0x820C

/* Generic Default Transition Time Server Model Opcode */
#define GENERIC_DEFAULT_TRANSITION_TIME_GET                               0x820D
#define GENERIC_DEFAULT_TRANSITION_TIME_SET                               0x820E
#define GENERIC_DEFAULT_TRANSITION_TIME_SET_UNACK                         0x820F
#define GENERIC_DEFAULT_TRANSITION_TIME_STATUS                            0x8210

/* Generic Power OnOff Server Model Opcode */
#define GENERIC_POWER_ON_OFF_GET                                          0x8211
#define GENERIC_POWER_ON_OFF_STATUS                                       0x8212

/* Generic Power OnOff Setup Server Model Opcode */
#define GENERIC_POWER_ON_OFF_SET                                          0x8213
#define GENERIC_POWER_ON_OFF_SET_UNACK                                    0x8214

/* Generic Power Level Server Model Opcode */
#define GENERIC_POWER_LEVEL_GET                                           0X8215
#define GENERIC_POWER_LEVEL_SET                                           0X8216
#define GENERIC_POWER_LEVEL_SET_UNACK                                     0X8217
#define GENERIC_POWER_LEVEL_STATUS                                        0X8218
#define GENERIC_POWER_LAST_GET                                            0X8219
#define GENERIC_POWER_LAST_STATUS                                         0X821A
#define GENERIC_POWER_DEFAULT_GET                                         0X821B
#define GENERIC_POWER_DEFAULT_STATUS                                      0X821C
#define GENERIC_POWER_RANGE_GET                                           0X821D
#define GENERIC_POWER_RANGE_STATUS                                        0X821E


/* Generic Power Level Setup Server Model Opcode */
#define GENERIC_POWER_DEFAULT_SET                                         0X821F
#define GENERIC_POWER_DEFAULT_SET_UNACK                                   0X8220
#define GENERIC_POWER_RANGE_SET                                           0X8221
#define GENERIC_POWER_RANGE_SET_UNACK                                     0X8222


/* Generic Battery Server Model Opcode */
#define GENERIC_BATTERY_GET                                               0X8223
#define GENERIC_BATTERY_STATUS                                            0X8224

/* Generic Location Server Model Opcode */
#define GENERIC_LOCATION_GLOBAL_GET                                       0X8225
#define GENERIC_LOCATION_GLOBAL_STATUS                                      0X40
#define GENERIC_LOCATION_LOCAL_GET                                        0X8226
#define GENERIC_LOCATION_LOCAL_STATUS                                     0X8227

/* Generic Location Setup Server Model Opcode */
#define GENERIC_LOCATION_GLOBAL_SET                                         0X41
#define GENERIC_LOCATION_GLOBAL_SET_UNACK                                   0X42
#define GENERIC_LOCATION_LOCAL_SET                                        0X8228
#define GENERIC_LOCATION_LOCAL_SET_UNACK                                  0X8229

/* Generic Manufacturer Property Server Model Opcode */
#define GENERIC_MANUFACTURER_PROPERTIES_GET                               0X822A
#define GENERIC_MANUFACTURER_PROPERTIES_STATUS                              0X43
#define GENERIC_MANUFACTURER_PROPERTY_GET                                 0X822B
#define GENERIC_MANUFACTURER_PROPERTY_SET                                   0X44
#define GENERIC_MANUFACTURER_PROPERTY_SET_UNACK                             0X45
#define GENERIC_MANUFACTURER_PROPERTY_STATUS                                0X46


/* Generic Admin Property Server Model Opcode */
#define GENERIC_ADMIN_PROPERTIES_GET                                      0X822C
#define GENERIC_ADMIN_PROPERTIES_STATUS                                     0X47
#define GENERIC_ADMIN_PROPERTY_GET                                        0X822D
#define GENERIC_ADMIN_PROPERTY_SET                                          0X48
#define GENERIC_ADMIN_PROPERTY_SET_UNACK                                    0X49
#define GENERIC_ADMIN_PROPERTY_STATUS                                       0X4A

/* Generic User Property Server Model Opcode */
#define GENERIC_USER_PROPERTIES_GET                                       0X822E
#define GENERIC_USER_PROPERTIES_STATUS                                      0X4B
#define GENERIC_USER_PROPERTY_GET                                         0X822F
#define GENERIC_USER_PROPERTY_SET                                           0X4C
#define GENERIC_USER_PROPERTY_SET_UNACK                                     0X4D
#define GENERIC_USER_PROPERTY_STATUS                                        0X4E

#define MAXSTEPVALUE                                                        0X3E
#define MINSTEPVALUE                                                        0X00
#define UNDEFSTEPVAL                                                        0x3F

#define STEP_HEX_VALUE_0                                                    0X00
#define STEP_HEX_VALUE_1                                                    0X01
#define STEP_HEX_VALUE_2                                                    0X02
#define STEP_HEX_VALUE_3                                                    0X03

/****Generic Power on off states****/
#define GENERIC_POWER_OFF_STATE                                             0X00
#define GENERIC_POWER_ON_STATE                                              0X01
#define GENERIC_POWER_RESTORE_STATE                                         0X02

#define LEVEL_MIN_VALID_RANGE                                             0x8000
#define LEVEL_MAX_VALID_RANGE                                             0x7FFF

#define TRANSITION_MIN_VALID_RANGE                                          0x00
#define TRANSITION_MAX_VALID_RANGE                                          0xFE   

#define MAX_BATTERY_LEVEL                                                   0X64
#define PROHIBITED_MIN_BATTERY_LEVEL                                        0X65
#define PROHIBITED_MAX_BATTERY_LEVEL                                        0XFE
#define UNKNOWN_BATTERY_LEVEL                                               0XFF

#define BATTERY_VALID_CHARGE_TIME                                         0XFFFE
#define BATTERY_UNKLNOWN_CHARGE_TIME                                      0XFFFF

#define BATTERY_VALID_DISCHARGE_TIME                                      0XFFFE
#define BATTERY_UNKLNOWN_DISCHARGE_TIME                                   0XFFFF

/* Generic Client Property Server Model Opcode */
#define GENERIC_CLIENT_PROPERTIES_GET                                       0X4F
#define GENERIC_CLIENT_PROPERTIES_STATUS                                    0X50

#define APPLI_LED_OFF                                                       0X00
#define APPLI_LED_ON                                                        0X01

#define NO_TRANSITION                                                       0X01
#define IN_TRANSITION                                                       0X02
#define DEFAULT_TRANSITION                                                  0X03

#define GENERIC_ON_OFF_TRANSITION_START                                     0X01
#define GENERIC_LEVEL_TRANSITION_START                                      0X02

#define BINDING_GENERIC_ON_OFF_SET                                          0X00
#define BINDING_GENERIC_LEVEL_SET                                           0X01

#define PACKET_CACHE_SIZE                                                      2
/* Exported variables  ------------------------------------------------------ */

/* Temporary  message to store the application variables*/
#pragma pack(push, 1)

typedef struct
{
  MOBLEUINT8  RemainingTime;
  MOBLEINT16 PresentValue16;
  MOBLEINT16 TargetValue16;
} Generic_TemporaryStatus_t;

/* Transition time variables */
typedef struct
{
  MOBLEINT8   StepValue ;
  MOBLEUINT8  ResBitValue;
  MOBLEUINT32 Res_Value;
  MOBLEUINT32 TotalTime;
}Generic_TimeParam_t;

/* Transition Flag variables */
typedef struct
{
  MOBLEUINT8 GenericTransitionFlag;
  MOBLEUINT8 GenericOptionalParam;
  MOBLEUINT8 Generic_Trnsn_Cmplt;
}Generic_ModelFlag_t;
/*****************************************************/

/* Generic On Off state Messages */
typedef struct
{
  MOBLEUINT8 TargetOnOffState;
  MOBLEUINT8 Generic_TID;
  MOBLEUINT8 Transition_Time;
  MOBLEUINT8 Delay_Time;
} Generic_OnOffParam_t;

typedef struct
{
  MOBLEUINT8  Present_OnOff_State;
  MOBLEUINT16 Present_OnOff_Value;
  MOBLEUINT16 Target_OnOff;
  MOBLEUINT8 RemainingTime;
}Generic_OnOffStatus_t;

/* Generic Level state Messages */
typedef struct
{
  MOBLEINT16 TargetLevel;
  MOBLEUINT8  Generic_TID;
  MOBLEUINT8  Transition_Time;
  MOBLEUINT8  Delay_Time;
} Generic_LevelParam_t;

typedef struct
{
  MOBLEINT16 Present_Level16;
  MOBLEINT16 Target_Level16;
  MOBLEUINT8 RemainingTime;
  MOBLEUINT8 Last_Level_TID;
  MOBLEINT16 Last_Present_Level16;  
  MOBLEINT32 Last_delta_level;
}Generic_LevelStatus_t;

typedef struct
{
  MOBLEINT32 TargetDeltaLevel32;
  MOBLEUINT8  Generic_TID;
  MOBLEUINT8  Transition_Time;
  MOBLEUINT8  Delay_Time;
} Generic_DeltaLevelParam_t;

typedef struct
{
  MOBLEUINT16 TargetMoveLevel16;
  MOBLEUINT8  Generic_TID;
  MOBLEUINT8  Transition_Time;
  MOBLEUINT8  Delay_Time;
} Generic_LevelMoveParam_t;
/******************************************************/
/* Generic Power on off messages*/
typedef struct
{
  MOBLEUINT8 PowerOnOffState;
}Generic_PowerOnOffParam_t; 
  
/******************************************************/
/* Generic Default transition time messages*/
typedef struct
{    
  MOBLEUINT8 DefaultTransitionTime;
}Generic_DefaultTransitionParam_t; 

typedef union  {
  Generic_LevelParam_t sGeneric_LevelParam;
  MOBLEUINT8 a_Level_param[sizeof(Generic_LevelParam_t)]; 
} _Generic_LevelParam;

/******************************************************/
typedef struct
{
  /* Pointer to the function Appli_Generic_OnOff_Set used for callback 
     from the middle layer to Application layer
  */
  MOBLE_RESULT (*OnOff_Set_cb)(Generic_OnOffStatus_t*, uint8_t, uint16_t, \
                                                                       uint8_t);  
   
  MOBLE_RESULT (*OnOff_Status_cb)(MOBLEUINT8 const *, MOBLEUINT32, uint16_t,\
                                                                        uint8_t); 
  /* Pointer to the function Appli_Generic_Level_Set used for callback 
     from the middle layer to Application layer
  */
  MOBLE_RESULT (*Level_Set_cb)(Generic_LevelStatus_t*, MOBLEUINT8, uint16_t,\
                                                                        uint8_t);
  
  /* Pointer to the function Appli_Generic_Delta_Set used for callback 
     from the middle layer to Application layer
  */
  MOBLE_RESULT (*LevelDelta_Set_cb)(Generic_LevelStatus_t*, MOBLEUINT8, uint16_t,\
                                                                        uint8_t);
  
  /* Pointer to the function Appli_Generic_Move_Set used for callback 
     from the middle layer to Application layer
  */
  MOBLE_RESULT (*LevelDeltaMove_Set_cb)(Generic_LevelStatus_t*, MOBLEUINT8, \
                                                          uint16_t, uint8_t);
  
  MOBLE_RESULT (*Level_Status_cb)(MOBLEUINT8 const *, MOBLEUINT32, uint16_t, \
                                                                        uint8_t);
  /* Pointer to the function Appli_Generic_PowerOnOff_Set used for callback 
     from the middle layer to Application layer
  */
  MOBLE_RESULT (*GenericPowerOnOff_cb)(Generic_PowerOnOffParam_t*, MOBLEUINT8, \
                                                          uint16_t, uint8_t);
  

  MOBLE_RESULT (*GenericPowerOnOff_Status_cb)(MOBLEUINT8 const *, MOBLEUINT32, \
                                                          uint16_t, uint8_t);
  
  /* Pointer to the function Generic_DefaultTransitionTime_Set used for callback 
     from the middle layer to Application layer
  */
  MOBLE_RESULT (*GenericDefaultTransition_cb)(Generic_DefaultTransitionParam_t*,\
                                              MOBLEUINT8, uint16_t, uint8_t);
  
  MOBLE_RESULT (*GenericDefaultTransition_Status_cb)(MOBLEUINT8 const *, MOBLEUINT32, \
                                                            uint16_t, uint8_t);
} Appli_Generic_cb_t;

typedef struct
{ 
  MOBLE_RESULT (*GetOnOffStatus_cb)(MOBLEUINT8*, uint16_t, uint8_t);
  
  MOBLE_RESULT (*GetOnOffValue_cb)(MOBLEUINT8*, uint16_t, uint8_t);
  
  MOBLE_RESULT (*GetLevelStatus_cb)(MOBLEUINT8*, uint16_t, uint8_t);
  
  MOBLE_RESULT (*GetPowerOnOffStatus_cb)(MOBLEUINT8*, uint16_t, uint8_t);
  
  MOBLE_RESULT (*GetDefaultTransitionStatus_cb)(MOBLEUINT8*, uint16_t, uint8_t);
  
}Appli_Generic_State_cb_t;

#pragma pack(pop)

extern const Appli_Generic_State_cb_t Appli_GenericState_cb;
extern const Appli_Generic_cb_t GenericAppli_cb;

/* Exported Functions Prototypes ---------------------------------------------*/

void BLEMesh_GenericModelAppliCb (Appli_Generic_cb_t* map );


MOBLE_RESULT Generic_OnOff_Set(MOBLEUINT8 const*, MOBLEUINT32, MODEL_MessageHeader_t*);
MOBLE_RESULT Generic_OnOff_Status(MOBLEUINT8*, MOBLEUINT32*, MODEL_MessageHeader_t*);

MOBLE_RESULT Generic_Level_Set(const MOBLEUINT8*, MOBLEUINT32, MODEL_MessageHeader_t*);
MOBLE_RESULT Generic_Delta_Set(const MOBLEUINT8*, MOBLEUINT32, MODEL_MessageHeader_t*);
MOBLE_RESULT Generic_Move_Set(const MOBLEUINT8*, MOBLEUINT32, MODEL_MessageHeader_t*);
MOBLE_RESULT Generic_Level_Status(MOBLEUINT8*, MOBLEUINT32*, MODEL_MessageHeader_t*);
MOBLE_RESULT Generic_PowerOnOff_Set(const MOBLEUINT8*, MOBLEUINT32 length, MODEL_MessageHeader_t*); 
MOBLE_RESULT Generic_PowerOnOff_Status(MOBLEUINT8*, MOBLEUINT32*, MODEL_MessageHeader_t*);
MOBLE_RESULT Generic_DefaultTransitionTime_Set(const MOBLEUINT8*, MOBLEUINT32 length,\
                                                         MODEL_MessageHeader_t*); 
MOBLE_RESULT Generic_DefaultTransitionTime_Status(MOBLEUINT8 *pTransition_status , \
                                                           MOBLEUINT32 *plength, 
                                                         MODEL_MessageHeader_t*); 


MOBLE_RESULT GenericModelServer_GetOpcodeTableCb(const MODEL_OpcodeTableParam_t **data, 
                                    MOBLEUINT16 *length);

MOBLE_RESULT GenericModelServer_GetStatusRequestCb(MODEL_MessageHeader_t *pmsgParam, 
                                    MOBLEUINT16 opcode, 
                                    MOBLEUINT8 *pResponsedata, 
                                    MOBLEUINT32 *plength, 
                                    MOBLEUINT8 const *pData,
                                    MOBLEUINT32 length,
                                    MOBLEBOOL response);

MOBLE_RESULT GenericModelServer_ProcessMessageCb(MODEL_MessageHeader_t *pmsgParam, 
                                    MOBLEUINT16 opcode, 
                                    MOBLEUINT8 const *data, 
                                    MOBLEUINT32 length, 
                                    MOBLEBOOL response);
void Generic_Process(void);
void Generic_Publish(MOBLE_ADDRESS srcAddress);

MOBLE_RESULT BLEMesh_AddGenericModels(void);

void Generic_GetStepValue(MOBLEUINT8 stepParam, MOBLEUINT8 elementIndex);

void GenericOnOff_LightActualBinding(Generic_OnOffStatus_t* onOff_param, MOBLEUINT8 elementIndex,
                                     MOBLEUINT8 trnsnFlag,MOBLEUINT8 trnsnCmplt);
void GenericLevel_LightBinding(Generic_LevelStatus_t* gLevel_param , MOBLEUINT8 flag, MOBLEUINT8 elementIndex,
                                     MOBLEUINT8 trnsnFlag,MOBLEUINT8 trnsnCmplt);
void GenericLevel_CtlTempBinding(Generic_LevelStatus_t * bLevelParam,MOBLEUINT8 elementIndex);
void GenericLevel_HslHueBinding(Generic_LevelStatus_t * bLevelParam,MOBLEUINT8 elementIndex);
void GenericLevel_HslSaturationBinding(Generic_LevelStatus_t * bLevelParam,MOBLEUINT8 elementIndex);
MOBLE_RESULT Generic_TransitionBehaviour(MOBLEUINT8 *GetValue, MOBLEUINT8 elementIndex);
MOBLE_RESULT Generic_OnOffStateUpdate_Process(MOBLEUINT8 elementIndex);
MOBLE_RESULT Generic_LevelStateUpdate_Process(MOBLEUINT8 elementIndex);
void Generic_OnOffDefaultTransitionValue(MOBLEUINT8 elementIndex);
void Generic_LevelDefaultTransitionValue(MOBLEUINT8 elementIndex, MOBLEUINT16 levelValue); 
void Generic_DeltaDefaultTransitionValue(MOBLEUINT8 elementIndex, MOBLEUINT32 deltaValue); 
void Generic_OnOffUpdate(MOBLEUINT8 elementIndex, MOBLEUINT8 onOff);

void Generic_TransitionParameterReset(MOBLEUINT8 elementIndex);
void Generic_RestoreModelStates(void *model_State,MOBLEUINT8 elementIndex);
void Generic_SaveModelStates(void *model_State,MOBLEUINT8 elementIndex);

#endif /* __GENERIC_H */



