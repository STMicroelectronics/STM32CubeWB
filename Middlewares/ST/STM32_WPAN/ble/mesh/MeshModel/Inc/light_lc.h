/**
******************************************************************************
* @file    light_control.h
* @author  BLE Mesh Team
* @version V1.12.000
* @date    06-12-2019
* @brief   Header file for the user application file 
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __LIGHT_LC_H
#define __LIGHT_LC_H

/* Includes ------------------------------------------------------------------*/
#include "types.h"
#include "ble_mesh.h"


/* Exported macro ------------------------------------------------------------*/
/******************************************************************************/
/********** Following Section defines the Opcodes for the Messages ************/
/******************************************************************************/
#define LIGHT_LC_MODE_GET                                                 0X8291
#define LIGHT_LC_MODE_SET                                                 0X8292
#define LIGHT_LC_MODE_SET_UNACK                                           0X8293
#define LIGHT_LC_MODE_STATUS                                              0X8294
#define LIGHT_LC_OM_GET                                                   0X8295
#define LIGHT_LC_OM_SET                                                   0X8296
#define LIGHT_LC_OM_SET_UNACK                                             0X8297
#define LIGHT_LC_OM_STATUS                                                0X8298
#define LIGHT_LC_ON_OFF_GET                                               0X8299
#define LIGHT_LC_ON_OFF_SET                                               0X829A
#define LIGHT_LC_ON_OFF_SET_UNACK                                         0X829B
#define LIGHT_LC_ON_OFF_STATUS                                            0X829C
#define LIGHT_LC_PROPERTY_GET                                             0X829D
#define LIGHT_LC_PROPERTY_SET                                               0X62
#define LIGHT_LC_PROPERTY_SET_UNACK                                         0X63
#define LIGHT_LC_PROPERTY_STATUS                                            0X64

#define LIGHT_MODEL_SERVER_LC_MODEL_ID                                    0x130F
#define LIGHT_MODEL_SERVER_LC_SETUP_MODEL_ID                              0x1310

#define LIGHT_LC_TRANSITION_STOP                                            0X00
#define LIGHT_LC_ON_OFF_TRANSITION_START                                    0X01

/* Property id for the light LC model */
#define LIGHT_CONTROL_LUX_LEVEL_ON_ID                                     0X002B
#define LIGHT_CONTROL_LUX_LEVEL_PROLONG_ID                                0X002C
#define LIGHT_CONTROL_LUX_LEVEL_STANDBY_ID                                0X002D   
#define LIGHT_CONTROL_LIGHTNESS_ON_ID                                     0X002E
#define LIGHT_CONTROL_LIGHTNESS_PROLONG_ID                                0X002F
#define LIGHT_CONTROL_LIGHTNESS_STANDBY_ID                                0X0030
#define LIGHT_CONTROL_REGULATOR_ACCURACY_ID                               0X0031
#define LIGHT_CONTROL_REGULATOR_KID_ID                                    0X0032
#define LIGHT_CONTROL_REGULATOR_KIU_ID                                    0X0033
#define LIGHT_CONTROL_REGULATOR_KPD_ID                                    0X0034
#define LIGHT_CONTROL_REGULATOR_KPU_ID                                    0X0035
#define LIGHT_CONTROL_TIME_FADE_ID                                        0X0036
#define LIGHT_CONTROL_TIME_FADE_ON_ID                                     0X0037  
#define LIGHT_CONTROL_TIME_FADE_STANDBY_AUTO_ID                           0X0038
#define LIGHT_CONTROL_TIME_FADE_STANDBY_MANUAL_ID                         0X0039
#define LIGHT_CONTROL_TIME_PROLONG_ID                                     0X003B
#define LIGHT_CONTROL_TIME_RUN_ON_ID                                      0X003C

/* Macros  ------------------------------------------------------- */
#define LIGHT_CONTROL_AMBIENT_LUXLEVEL_ON                                 0XEA60
#define LIGHT_CONTROL_AMBIENT_LUXLEVEL_PROLONG                            0XC350 
#define LIGHT_CONTROL_AMBIENT_LUXLEVEL_STANDBY                            0X0000
#define LIGHT_CONTROL_LIGHTNESS_ON_VALUE                                  0xFFFF 
#define LIGHT_CONTROL_LIGHTNESS_PROLONG_VALUE                  0x3A98
#define LIGHT_CONTROL_LIGHTNESS_STANDBY_VALUE                               0x01
#define LIGHT_CONTROL_TIME_FADE_ON_VALUE                       0xa    /* 20 steps */
#define LIGHT_CONTROL_TIME_RUN_ON_VALUE                        0x1388  /* 5 second */
#define LIGHT_CONTROL_TIME_FADE_VALUE                          0Xa
#define LIGHT_CONTROL_TIME_PROLONG_VALUE                       0X0bb8  /* 3 second */
#define LIGHT_CONTROL_TIME_FADE_STANDBY_AUTO_VALUE             0xa
#define LIGHT_CONTROL_TIME_FADE_STANDBY_MANUAL_VALUE           0Xa
#define LIGHT_CONTROL_KID                                                   0X01
#define LIGHT_CONTROL_KIU                                                   0X02
#define LIGHT_CONTROL_KPD                                                   0X03
#define LIGHT_CONTROL_KPU                                                   0X04
#define LIGHT_CONTROL_REGULATOR_ACCURACY_VALUE                             0X100

/* Property IDs by SIG ------------------------------------------------------- */
#define MOTION_SENSED_PROPERTY        0X0042
#define PEOPLE_COUNT_PROPERTY         0X004C
#define PRESENCE_DETECTED_PROPERTY    0X004D

#define TRANSITION_STEP_VALUE                                               0X0A
#define LC_MODE_ENABLE                                                      0X01
#define LC_MODE_DISABLE                                                     0X00
/* Light control mode messages*/
#pragma pack(1)
typedef struct
{
  MOBLEUINT8 LC_mode;
  MOBLEUINT8 LC_OM;
  MOBLEUINT8 Light_OnOff;
  MOBLEUINT8 Tid;
  MOBLEUINT8 Transition_Time;
  MOBLEUINT8 Delay;
}Light_LC_Param_t; 

typedef struct
{
  MOBLEUINT32 Property_Value; 
}Light_LC_Value_t;

/* Property values for variable length */
typedef struct
{
  MOBLEUINT16 Property_ID;
  MOBLEUINT16 Property_Value_16;
}
Light_LC_PropertyTable16_t;

typedef struct
{
  MOBLEUINT16 Property_ID;
  float Property_Value_float;
}
Light_LC_PropertyTablefloat_t;

typedef struct
{
  MOBLEUINT16 Property_ID;
  MOBLEUINT16 Property_Value_8b;
}
Light_LC_PropertyTable8b_t;

typedef struct
{
  MOBLEUINT16 Property_ID;
  MOBLEUINT32 Property_Value_24b;
}
Light_LC_PropertyTable24b_t;


/* Table for all the Property id and corresponding value */
typedef struct {
Light_LC_PropertyTable8b_t LC_PropertyTable8b[1];
Light_LC_PropertyTable16_t LC_PropertyTable16b[3];
Light_LC_PropertyTable24b_t LC_PropertyTable24b[9];
Light_LC_PropertyTablefloat_t LC_PropertyTableFloat[4];
}Light_Property_Table_t;

typedef struct
{
  MOBLEUINT8  Present_OnOff_State;
  MOBLEUINT16 Target_OnOff;
  MOBLEUINT8 RemainingTime;
}Light_LC_OnOffState_t;

/* Transition time variables */
typedef struct
{
  MOBLEINT8   StepValue ;
  MOBLEUINT8  ResBitValue;
  MOBLEUINT32 Res_Value;
  MOBLEUINT32 TotalTime;
}Light_LC_TimeParam_t;

#pragma pack(4)

/* Light LC state machine states */
typedef enum 
{
  LC_STANDBY,
  LC_FADE_ON,
  LC_RUN,
  LC_FADE,
  LC_PROLONG,
  LC_FADE_STANDBY_AUTO,
  LC_FADE_STANDBY_MANUAL
}Lc_States_e;

/* Light LC state machine states */
typedef enum 
{
  LC_NO_EVENT,
  LC_LIGHT_ON,
  LC_OCCUPANCY_ON,
  LC_LIGHT_OFF,
  LC_OCCUPANCY_OFF,
  LC_TIMER_OFF
}Lc_Event_e;

typedef struct
{
  Lc_States_e Lc_States;
}Light_LC_States;


#pragma pack(1)
typedef struct
{  
  /* Pointer to the function Appli_LightLC_Mode_Set used for callback 
     from the middle layer to Application layer
  */
  MOBLE_RESULT (*LightLC_Mode_Set_cb)(Light_LC_Param_t*, MOBLEUINT8);
  
  /* Pointer to the function Appli_LightLC_OM_Set used for callback 
     from the middle layer to Application layer
  */
  MOBLE_RESULT (*LightLC_OM_Set_cb)(Light_LC_Param_t*, MOBLEUINT8);
  
  /* Pointer to the function Appli_LightLC_OnOff_Set used for callback 
     from the middle layer to Application layer
  */
  MOBLE_RESULT (*LightLC_OnOff_Set_cb)(Light_LC_Param_t*, MOBLEUINT8);
  
  /* Pointer to the function Appli_LightLC_Property_Set used for callback 
     from the middle layer to Application layer
  */
   
} Appli_Light_Ctrl_cb_t;

typedef struct
{   
  MOBLE_RESULT (*GetLightLC_ModeState_cb)(MOBLEUINT8*);
  MOBLE_RESULT (*GetLightLC_OMState_cb)(MOBLEUINT8*);
  MOBLE_RESULT (*GetLightLC_OnOffState_cb)(MOBLEUINT8*);
  MOBLEUINT16 (*GetLightLC_AmbientLuxLevel_cb)(void);
  MOBLEUINT16 (*GetLightLC_PIregulatorOut_cb)(MOBLEUINT16,MOBLEUINT16);
}Appli_LightLC_GetStatus_cb_t;
#pragma pack(4)

extern const Appli_Light_Ctrl_cb_t LightLCAppli_cb;
extern const Appli_LightLC_GetStatus_cb_t Appli_LightLC_GetStatus_cb;

MOBLE_RESULT Light_LC_ModelServer_GetOpcodeTableCb(const MODEL_OpcodeTableParam_t **data, 
                                                 MOBLEUINT16 *length);

MOBLE_RESULT Light_LC_ModelServer_GetStatusRequestCb(MOBLE_ADDRESS peer_addr, 
                                    MOBLE_ADDRESS dst_peer, 
                                    MOBLEUINT16 opcode, 
                                    MOBLEUINT8 *pResponsedata, 
                                    MOBLEUINT32 *plength, 
                                    MOBLEUINT8 const *pRxData,
                                    MOBLEUINT32 dataLength,
                                    MOBLEBOOL response);

MOBLE_RESULT Light_LC_ModelServer_ProcessMessageCb(MOBLE_ADDRESS peer_addr, 
                                                 MOBLE_ADDRESS dst_peer, 
                                                 MOBLEUINT16 opcode, 
                                                 MOBLEUINT8 const *pRxData, 
                                                 MOBLEUINT32 dataLength, 
                                                 MOBLEBOOL response
                                                   );

MOBLE_RESULT Light_LC_ModeSet(MOBLEUINT8 const *lcMode_param, MOBLEUINT32 length);
MOBLE_RESULT Light_LC_ModeStatus(MOBLEUINT8* lcMode_status, MOBLEUINT32 *plength);
MOBLE_RESULT Light_LC_OMSet(MOBLEUINT8 const *lcOM_param, MOBLEUINT32 length);
MOBLE_RESULT Light_LC_OMStatus(MOBLEUINT8* lcOM_status, MOBLEUINT32 *plength);
MOBLE_RESULT Light_LC_OnOffSet(MOBLEUINT8 const *lcOnOff_param, MOBLEUINT32 length);
MOBLE_RESULT Light_LC_OnOffStatus(MOBLEUINT8* lcOnOff_status, MOBLEUINT32 *plength);
MOBLE_RESULT Light_LC_PropertySet(MOBLEUINT8 const *lcProp_param, MOBLEUINT32 length);
MOBLE_RESULT Light_LC_PropertyStatus( MOBLEUINT8* lcData_param, MOBLEUINT32* plength ,
                                        MOBLEUINT8 const *pData, MOBLEUINT32 length);

MOBLE_RESULT Light_LC_SetPropertyID_value(MOBLEUINT32 Prop_Value,
                                                            MOBLEUINT16 prop_ID);

MOBLEUINT32 Light_LC_GetPropertyID_value(MOBLEUINT16 property_ID);                                                         
MOBLEUINT32 Light_LC_GetStepValue(MOBLEUINT8 stepParam);
MOBLEUINT32 Get_TimeToWait(MOBLEUINT16 Proprety_ID);
MOBLEUINT16 Light_LC_LuxLevelOutputValue(MOBLEUINT16 property_ID);
MOBLEUINT16 Light_LC_MaxLightnessValue(MOBLEUINT16 Param1,MOBLEUINT16 Param2);
void Light_LC_Fsm(void);

void Light_control_Process(void);

MOBLE_RESULT Light_LC_Client_Mode_Status(MOBLEUINT8 const *pLightLc_status, MOBLEUINT32 pLength);
MOBLE_RESULT Light_LC_Client_OM_Status(MOBLEUINT8 const *pLightLc_status, MOBLEUINT32 pLength);
MOBLE_RESULT Light_LC_Client_ON_OFF_Status(MOBLEUINT8 const *pLightLc_status, MOBLEUINT32 pLength);
MOBLE_RESULT Light_LC_Client_Property_Status(MOBLEUINT8 const *pLightLc_status, MOBLEUINT32 pLength);


#endif /* __LIGHT_LC_H */

/******************* (C) COPYRIGHT 2017 STMicroelectronics *****END OF FILE****/

