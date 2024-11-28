/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : tsl_user.c
  * Description        : User configuration file for TOUCHSENSING
  *                      middleWare.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2019-2021 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

#include "tsl_user.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/*============================================================================*/
/* Channels                                                                   */
/*============================================================================*/

/* Source and Configuration (ROM) */
CONST TSL_ChannelSrc_T MyChannels_Src[TSLPRM_TOTAL_CHANNELS] =
{
    { CHANNEL_0_SRC, CHANNEL_0_IO_MSK, CHANNEL_0_GRP_MSK },
};

/* Destination (ROM) */
CONST TSL_ChannelDest_T MyChannels_Dest[TSLPRM_TOTAL_CHANNELS] =
{
    { CHANNEL_0_DEST },
};

/* Data (RAM) */
TSL_ChannelData_T MyChannels_Data[TSLPRM_TOTAL_CHANNELS];

/*============================================================================*/
/* Banks                                                                      */
/*============================================================================*/

/* List (ROM) */
CONST TSL_Bank_T MyBanks[TSLPRM_TOTAL_BANKS] = {
/* TOUCHKEYS bank(s) definition*/
   {&MyChannels_Src[0], &MyChannels_Dest[0], MyChannels_Data, BANK_0_NBCHANNELS, BANK_0_MSK_CHANNELS, BANK_0_MSK_GROUPS},
};
/*============================================================================*/
/* Touchkey sensors                                                           */
/*============================================================================*/

/* Data (RAM) */
TSL_TouchKeyData_T MyTKeys_Data[TSLPRM_TOTAL_TKEYS];

/* Parameters (RAM) */
TSL_TouchKeyParam_T MyTKeys_Param[TSLPRM_TOTAL_TKEYS];

/* State Machine (ROM) */

void MyTKeys_ErrorStateProcess(void);
void MyTKeys_OffStateProcess(void);

CONST TSL_State_T MyTKeys_StateMachine[] =
{
  /* Calibration states */
  /*  0 */ { TSL_STATEMASK_CALIB,              TSL_tkey_CalibrationStateProcess },
  /*  1 */ { TSL_STATEMASK_DEB_CALIB,          TSL_tkey_DebCalibrationStateProcess },
  /* Release states */
  /*  2 */ { TSL_STATEMASK_RELEASE,            TSL_tkey_ReleaseStateProcess },
#if TSLPRM_USE_PROX > 0
  /*  3 */ { TSL_STATEMASK_DEB_RELEASE_PROX,   TSL_tkey_DebReleaseProxStateProcess },
#else
  /*  3 */ { TSL_STATEMASK_DEB_RELEASE_PROX,   0 },
#endif
  /*  4 */ { TSL_STATEMASK_DEB_RELEASE_DETECT, TSL_tkey_DebReleaseDetectStateProcess },
  /*  5 */ { TSL_STATEMASK_DEB_RELEASE_TOUCH,  TSL_tkey_DebReleaseTouchStateProcess },
#if TSLPRM_USE_PROX > 0
  /* Proximity states */
  /*  6 */ { TSL_STATEMASK_PROX,               TSL_tkey_ProxStateProcess },
  /*  7 */ { TSL_STATEMASK_DEB_PROX,           TSL_tkey_DebProxStateProcess },
  /*  8 */ { TSL_STATEMASK_DEB_PROX_DETECT,    TSL_tkey_DebProxDetectStateProcess },
  /*  9 */ { TSL_STATEMASK_DEB_PROX_TOUCH,     TSL_tkey_DebProxTouchStateProcess },
#else
  /*  6 */ { TSL_STATEMASK_PROX,               0 },
  /*  7 */ { TSL_STATEMASK_DEB_PROX,           0 },
  /*  8 */ { TSL_STATEMASK_DEB_PROX_DETECT,    0 },
  /*  9 */ { TSL_STATEMASK_DEB_PROX_TOUCH,     0 },
#endif
  /* Detect states */
  /* 10 */ { TSL_STATEMASK_DETECT,             TSL_tkey_DetectStateProcess },
  /* 11 */ { TSL_STATEMASK_DEB_DETECT,         TSL_tkey_DebDetectStateProcess },
  /* Touch state */
  /* 12 */ { TSL_STATEMASK_TOUCH,              TSL_tkey_TouchStateProcess },
  /* Error states */
  /* 13 */ { TSL_STATEMASK_ERROR,              MyTKeys_ErrorStateProcess },
  /* 14 */ { TSL_STATEMASK_DEB_ERROR_CALIB,    TSL_tkey_DebErrorStateProcess },
  /* 15 */ { TSL_STATEMASK_DEB_ERROR_RELEASE,  TSL_tkey_DebErrorStateProcess },
  /* 16 */ { TSL_STATEMASK_DEB_ERROR_PROX,     TSL_tkey_DebErrorStateProcess },
  /* 17 */ { TSL_STATEMASK_DEB_ERROR_DETECT,   TSL_tkey_DebErrorStateProcess },
  /* 18 */ { TSL_STATEMASK_DEB_ERROR_TOUCH,    TSL_tkey_DebErrorStateProcess },
  /* Other states */
  /* 19 */ { TSL_STATEMASK_OFF,                MyTKeys_OffStateProcess }
};

/* Methods for "extended" type (ROM) */
CONST TSL_TouchKeyMethods_T MyTKeys_Methods =
{
  TSL_tkey_Init,
  TSL_tkey_Process
};

/* TouchKeys list (ROM) */

CONST TSL_TouchKey_T MyTKeys[TSLPRM_TOTAL_TOUCHKEYS] =
{
  { &MyTKeys_Data[0], &MyTKeys_Param[0], &MyChannels_Data[CHANNEL_0_DEST], MyTKeys_StateMachine, &MyTKeys_Methods }
};

/*============================================================================*/
/* Generic Objects                                                            */
/*============================================================================*/

/* List (ROM) */
CONST TSL_Object_T MyObjects[TSLPRM_TOTAL_OBJECTS] =
{
  { TSL_OBJ_TOUCHKEY, (TSL_TouchKey_T *)&MyTKeys[0] }
};

/* Group (RAM) */
TSL_ObjectGroup_T MyObjGroup =
{
  &MyObjects[0],        /* First object */
  TSLPRM_TOTAL_OBJECTS, /* Number of objects */
  0x00,                 /* State mask reset value */
  TSL_STATE_NOT_CHANGED /* Current state */
};

/*============================================================================*/
/* TSL Common Parameters placed in RAM or ROM                                 */
/* --> external declaration in tsl_conf.h                                     */
/*============================================================================*/

TSL_Params_T TSL_Params =
{
  TSLPRM_ACQ_MIN,
  TSLPRM_ACQ_MAX,
  TSLPRM_CALIB_SAMPLES,
  TSLPRM_DTO,
#if TSLPRM_TOTAL_TKEYS > 0
  MyTKeys_StateMachine,   /* Default state machine for TKeys */
  &MyTKeys_Methods,       /* Default methods for TKeys */
#endif
#if TSLPRM_TOTAL_LNRTS > 0
  MyLinRots_StateMachine, /* Default state machine for LinRots */
  &MyLinRots_Methods      /* Default methods for LinRots */
#endif
};

/* Private functions prototype -----------------------------------------------*/

/* Global variables ----------------------------------------------------------*/
/* USER CODE BEGIN Global variables */

/* USER CODE END Global variables */
  static uint32_t IdxBank = 0;
  static uint32_t ConfigDone = 0;

__IO TSL_tTick_ms_T ECSLastTick; /* Hold the last time value for ECS */

/**
  * @brief  Initialize the STMTouch Driver
  * @param  None
  * @retval None
  */
void tsl_user_Init(void)
{
  IdxBank = 0;
  ConfigDone = 0;
  TSL_obj_GroupInit(&MyObjGroup); /* Init Objects */

  TSL_Init(MyBanks); /* Init acquisition module */

  tsl_user_SetThresholds(); /* Init thresholds for each object individually (optional) */
}

/**
  * @brief  Execute STMTouch Driver main State machine
  * @param  None
  * @retval status Return TSL_STATUS_OK if the acquisition is done
  */
tsl_user_status_t tsl_user_Exec(void)
{
  tsl_user_status_t status = TSL_USER_STATUS_BUSY;

  /* Configure and start bank acquisition */
  if (!ConfigDone)
  {
/* USER CODE BEGIN not config_done start*/

/* USER CODE END not config_done start*/
    TSL_acq_BankConfig(IdxBank);
    TSL_acq_BankStartAcq();
    ConfigDone = 1;
/* USER CODE BEGIN not config_done */

/* USER CODE END not config_done */
  }

  /* Check end of acquisition (polling mode) and read result */
  if (TSL_acq_BankWaitEOC() == TSL_STATUS_OK)
  {
/* USER CODE BEGIN end of acquisition start*/

/* USER CODE END end of acquisition start*/
    STMSTUDIO_LOCK;
    TSL_acq_BankGetResult(IdxBank, 0, 0);
    STMSTUDIO_UNLOCK;
    IdxBank++; /* Next bank */
    ConfigDone = 0;
/* USER CODE BEGIN end of acquisition */

/* USER CODE END end of acquisition */
  }

  /* Process objects, DxS and ECS
     Check if all banks have been acquired
  */
  if (IdxBank > TSLPRM_TOTAL_BANKS-1)
  {
/* USER CODE BEGIN before reset*/

/* USER CODE END before reset*/
    /* Reset flags for next banks acquisition */
    IdxBank = 0;
    ConfigDone = 0;

    /* Process Objects */
    TSL_obj_GroupProcess(&MyObjGroup);

    /* DxS processing (if TSLPRM_USE_DXS option is set) */
    TSL_dxs_FirstObj(&MyObjGroup);

    /* ECS every TSLPRM_ECS_DELAY (in ms) */
    if (TSL_tim_CheckDelay_ms(TSLPRM_ECS_DELAY, &ECSLastTick) == TSL_STATUS_OK)
    {
      if (TSL_ecs_Process(&MyObjGroup) == TSL_STATUS_OK)
      {
        status = TSL_USER_STATUS_OK_ECS_ON;
      }
      else
      {
        status = TSL_USER_STATUS_OK_ECS_OFF;
      }
    }
    else
    {
      status = TSL_USER_STATUS_OK_NO_ECS;
    }
/* USER CODE BEGIN Process objects */

/* USER CODE END Process objects */
  }
  else
  {
    status = TSL_USER_STATUS_BUSY;
/* USER CODE BEGIN TSL_USER_STATUS_BUSY */

/* USER CODE END TSL_USER_STATUS_BUSY */
  }

  return status;
}

/**
  * Interrupt TSC management. See below code example for N sensors in main.c:
while (1)
{
  #define TKEY_DET(NB) (MyTKeys[(NB)].p_Data->StateId == TSL_STATEID_DETECT)
  #define TKEY_PRX(NB) (MyTKeys[(NB)].p_Data->StateId == TSL_STATEID_PROX)
  #define TKEY_REL(NB) (MyTKeys[(NB)].p_Data->StateId == TSL_STATEID_RELEASE)
  #define TKEY_CAL(NB) (MyTKeys[(NB)].p_Data->StateId == TSL_STATEID_CALIB)
  if(tsl_user_Exec_IT() != TSL_USER_STATUS_BUSY){
    int id;
    for(id=0; id < TSLPRM_TOTAL_CHANNELS; id++){
      if(!TKEY_CAL(id)){
        printf("Sensor%d: Delta %3d Ref %3d Measurement %3d StateId %3d\n"
        ,id
        ,MyTKeys[id].p_ChD->Delta
        ,MyTKeys[id].p_ChD->Ref
        ,MyTKeys[id].p_ChD->Meas
        ,MyTKeys[id].p_Data->StateId);
        if(TKEY_DET(id)){
    // we detect a touch
        }else if(TKEY_REL(id)){
        // No more detection
        }
      }
    }
  }else{
    HAL_Delay(1); //Can be replace by __WFI()
  }
}
  */

/**
 * Local variable used for interrupt acquisition mode
 */
uint32_t idx_bank_it = 0; // Bank acquisition number (from 0 to TSLPRM_TOTAL_BANKS-1)
uint32_t config_done_it = 0; // Start first TSC acquisition done using bank 0
uint32_t acq_done_it = 0; // TSLPRM_TOTAL_BANKS banks acquisition done

/**
  * @brief  Acquisition completed callback in non blocking mode
  * @param  htsc: pointer to a TSC_HandleTypeDef structure that contains
  *         the configuration information for the specified TSC.
  * @retval None
  */
void HAL_TSC_ConvCpltCallback(TSC_HandleTypeDef* htsc)
{
/* USER CODE BEGIN HAL_TSC_ConvCpltCallback start*/

/* USER CODE END HAL_TSC_ConvCpltCallback start*/
  TSL_acq_BankGetResult(idx_bank_it, 0, 0);
  idx_bank_it++;
  if (idx_bank_it > TSLPRM_TOTAL_BANKS-1)
  {
    // End of all banks acquisition, restart bank 0 in while(1) loop (This is a choice)
    idx_bank_it=0;
    acq_done_it++;
  }else{
    // We restart next bank acquisition
    TSL_acq_BankConfig(idx_bank_it);
    TSL_acq_BankStartAcq_IT();
  }
/* USER CODE BEGIN HAL_TSC_ConvCpltCallback*/

/* USER CODE END HAL_TSC_ConvCpltCallback*/
}

/**
  * @brief  Execute STMTouch Driver main State machine using TSC Interrupt
  * @param  None
  * @retval status Return TSL_STATUS_OK if the acquisition is done
  */
tsl_user_status_t tsl_user_Exec_IT(void)
{
  tsl_user_status_t status = TSL_USER_STATUS_BUSY;

  /* Configure and start bank acquisition */
  if (!config_done_it)
  {
/* USER CODE BEGIN not config_done start*/

/* USER CODE END not config_done start*/
    idx_bank_it = 0;
    acq_done_it = 0;
    TSL_acq_BankConfig(idx_bank_it);
    TSL_acq_BankStartAcq_IT();
    config_done_it = 1;
/* USER CODE BEGIN not config_done */

/* USER CODE END not config_done */
  }

  /* Check end of all group acquisitions (interrupt mode) */
  if (acq_done_it)
  {
/* USER CODE BEGIN acq_done_it start*/

/* USER CODE END acq_done_it start*/

    /* Process Objects */
    TSL_obj_GroupProcess(&MyObjGroup);

    /* DxS processing (if TSLPRM_USE_DXS option is set) */
    TSL_dxs_FirstObj(&MyObjGroup);

    /* ECS every TSLPRM_ECS_DELAY (in ms) */
    if (TSL_tim_CheckDelay_ms(TSLPRM_ECS_DELAY, &ECSLastTick) == TSL_STATUS_OK)
    {
      if (TSL_ecs_Process(&MyObjGroup) == TSL_STATUS_OK)
      {
        status = TSL_USER_STATUS_OK_ECS_ON;
      }
      else
      {
        status = TSL_USER_STATUS_OK_ECS_OFF;
      }
    }
    else
    {
      status = TSL_USER_STATUS_OK_NO_ECS;
    }

/* USER CODE BEGIN prepare_acq_done_it */

/* USER CODE END prepare_acq_done_it */

    // Restart TSLPRM_TOTAL_BANKS banks acquisition
    idx_bank_it = 0;
    acq_done_it = 0;
    TSL_acq_BankConfig(idx_bank_it);
    TSL_acq_BankStartAcq_IT();
/* USER CODE BEGIN acq_done_it */

/* USER CODE END acq_done_it */
  }
  else
  {
    status = TSL_USER_STATUS_BUSY;
/* USER CODE BEGIN TSL_USER_STATUS_BUSY */

/* USER CODE END TSL_USER_STATUS_BUSY */
  }

/* USER CODE BEGIN Process objects */

/* USER CODE END Process objects */

  return status;
}

/**
  * @brief  Set thresholds for each object (optional).
  * @param  None
  * @retval None
  */
void tsl_user_SetThresholds(void)
{
/* USER CODE BEGIN Tsl_user_SetThresholds */
  /* Example: Decrease the Detect thresholds for the TKEY 0
  MyTKeys_Param[0].DetectInTh -= 10;
  MyTKeys_Param[0].DetectOutTh -= 10;
  */
/* USER CODE END Tsl_user_SetThresholds */
  }

/**
  * @brief  Executed when a sensor is in Error state
  * @param  None
  * @retval None
  */
  void MyTKeys_ErrorStateProcess(void)
{
/* USER CODE BEGIN MyTKeys_ErrorStateProcess */
  /* Add here your own processing when a sensor is in Error state */
/* USER CODE END MyTKeys_ErrorStateProcess */
}

/**
  * @brief  Executed when a sensor is in Off state
  * @param  None
  * @retval None
  */
void MyTKeys_OffStateProcess(void)
{
/* USER CODE BEGIN MyTKeys_OffStateProcess */
  /* Add here your own processing when a sensor is in Off state */
/* USER CODE END MyTKeys_OffStateProcess */
}

