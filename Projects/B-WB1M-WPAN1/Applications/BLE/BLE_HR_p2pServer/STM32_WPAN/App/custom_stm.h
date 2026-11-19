/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    App/custom_stm.h
  * @author  MCD Application Team
  * @brief   Header for custom_stm.c module.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef CUSTOM_STM_H
#define CUSTOM_STM_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
typedef enum
{
  /* My_P2P_Server */
  CUSTOM_STM_LED_C,
  CUSTOM_STM_SWITCH_C,
  CUSTOM_STM_LONG_C,
  /* My_Heart_Rate */
  CUSTOM_STM_HRS_M,
  CUSTOM_STM_HRS_SL,
  CUSTOM_STM_HRS_CTRL,
} Custom_STM_Char_Opcode_t;

typedef enum
{
  /* My_LED_Char */
  CUSTOM_STM_LED_C_READ_EVT,
  CUSTOM_STM_LED_C_WRITE_NO_RESP_EVT,
  /* My_Switch_Char */
  CUSTOM_STM_SWITCH_C_NOTIFY_ENABLED_EVT,
  CUSTOM_STM_SWITCH_C_NOTIFY_DISABLED_EVT,
  /* MyLongChar */
  CUSTOM_STM_LONG_C_NOTIFY_ENABLED_EVT,
  CUSTOM_STM_LONG_C_NOTIFY_DISABLED_EVT,
  /* My_HRS_Meas */
  CUSTOM_STM_HRS_M_NOTIFY_ENABLED_EVT,
  CUSTOM_STM_HRS_M_NOTIFY_DISABLED_EVT,
  /* My_Sensor_Loc */
  CUSTOM_STM_HRS_SL_READ_EVT,
  /* My_HRS_CTRL_Point */
  CUSTOM_STM_HRS_CTRL_WRITE_EVT,
  CUSTOM_STM_NOTIFICATION_COMPLETE_EVT,

  CUSTOM_STM_BOOT_REQUEST_EVT
} Custom_STM_Opcode_evt_t;

typedef struct
{
  uint8_t * pPayload;
  uint8_t   Length;
} Custom_STM_Data_t;

typedef struct
{
  Custom_STM_Opcode_evt_t       Custom_Evt_Opcode;
  Custom_STM_Data_t             DataTransfered;
  uint16_t                      ConnectionHandle;
  uint8_t                       ServiceInstance;
  uint16_t                      AttrHandle;
} Custom_STM_App_Notification_evt_t;

/* USER CODE BEGIN ET */
typedef enum
{
  CUSTOM_STM_HRS_HRM_VALUE_FORMAT_UINT16       = 1,
  CUSTOM_STM_HRS_HRM_SENSOR_CONTACTS_PRESENT   = 2,
  CUSTOM_STM_HRS_HRM_SENSOR_CONTACTS_SUPPORTED = 4,
  CUSTOM_STM_HRS_HRM_ENERGY_EXPENDED_PRESENT   = 8,
  CUSTOM_STM_HRS_HRM_RR_INTERVAL_PRESENT       = 0x10
} Custom_STM_HRS_HrmFlags_t;

typedef enum
{
  CUSTOM_STM_HRS_BODY_SENSOR_LOCATION_OTHER    = 0,
  CUSTOM_STM_HRS_BODY_SENSOR_LOCATION_CHEST    = 1,
  CUSTOM_STM_HRS_BODY_SENSOR_LOCATION_WRIST    = 2,
  CUSTOM_STM_HRS_BODY_SENSOR_LOCATION_FINGER   = 3,
  CUSTOM_STM_HRS_BODY_SENSOR_LOCATION_HAND     = 4,
  CUSTOM_STM_HRS_BODY_SENSOR_LOCATION_EAR_LOBE = 5,
  CUSTOM_STM_HRS_BODY_SENSOR_LOCATION_FOOT     = 6
} Custom_STM_HRS_BodySensorLocation_t;

typedef struct
{
  uint16_t    MeasurementValue;
#if (BLE_CFG_HRS_ENERGY_EXPENDED_INFO_FLAG == 1)
  uint16_t    EnergyExpended;
#endif
#if (BLE_CFG_HRS_ENERGY_RR_INTERVAL_FLAG != 0)
  uint16_t    aRRIntervalValues[BLE_CFG_HRS_ENERGY_RR_INTERVAL_FLAG + BLE_CFG_HRS_ENERGY_EXPENDED_INFO_FLAG];
  uint8_t     NbreOfValidRRIntervalValues;
#endif
  uint8_t     Flags;
} Custom_STM_HRS_MeasVal_t;

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
extern uint16_t SizeLed_C;
extern uint16_t SizeSwitch_C;
extern uint16_t SizeLong_C;
extern uint16_t SizeHrs_M;
extern uint16_t SizeHrs_Sl;
extern uint16_t SizeHrs_Ctrl;

/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* External variables --------------------------------------------------------*/
/* USER CODE BEGIN EV */

/* USER CODE END EV */

/* Exported macros -----------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions ------------------------------------------------------- */
void SVCCTL_InitCustomSvc(void);
void Custom_STM_App_Notification(Custom_STM_App_Notification_evt_t *pNotification);
tBleStatus Custom_STM_App_Update_Char(Custom_STM_Char_Opcode_t CharOpcode,  uint8_t *pPayload);
tBleStatus Custom_STM_App_Update_Char_Variable_Length(Custom_STM_Char_Opcode_t CharOpcode, uint8_t *pPayload, uint8_t size);
tBleStatus Custom_STM_App_Update_Char_Ext(uint16_t Connection_Handle, Custom_STM_Char_Opcode_t CharOpcode, uint8_t *pPayload);
/* USER CODE BEGIN EF */

/* USER CODE END EF */

#ifdef __cplusplus
}
#endif

#endif /*CUSTOM_STM_H */
