/**
  ******************************************************************************
  * File Name          : env_server_app.c
  * Description        : Handle HW/Environmental Service/Char
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

/* Includes ------------------------------------------------------------------*/
#include "app_common.h"
#include "ble.h"
#include "dbg_trace.h"

#include "motenv_server_app.h"
#include "env_server_app.h"

#include "b_wb1m_wpan1.h"
#include "b_wb1m_wpan1_bus.h"
#include "b_wb1m_wpan1_env_sensors.h"
  
#include "custom_stm.h"

/* Private defines -----------------------------------------------------------*/
#define TEMPERATURE_BYTES       (2)

#define VALUE_LEN_ENV           (2+TEMPERATURE_BYTES/*Temp1*/)

/* Private typedef -----------------------------------------------------------*/

/**
 * @brief  HW/Environmental Service/Char Context structure definition
 */
typedef struct
{
  uint8_t  NotificationStatus;

  int32_t PressureValue;
  uint16_t HumidityValue;
  int16_t TemperatureValue[1];
  uint8_t hasPressure;
  uint8_t hasHumidity;
  uint8_t hasTemperature;
} ENV_Server_App_Context_t;

/* Private macros ------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/**
 * @brief  Environmental Capabilities
 */

static ENV_Server_App_Context_t ENV_Server_App_Context;

/* Global variables ----------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
static void ENV_Handle_Sensor(void);
static void EnvSensor_GetCaps(void);

/* Functions Definition ------------------------------------------------------*/

/* Public functions ----------------------------------------------------------*/

/**
 * @brief  Init the HW/Environmental Service/Char Context
 * @param  None
 * @retval None
 */
void ENV_Context_Init(void)
{
  /* Env Sensors */

  ENV_Server_App_Context.hasPressure = 0;
  ENV_Server_App_Context.hasHumidity = 0;
  ENV_Server_App_Context.hasTemperature = 0;

  ENV_Set_Notification_Status(0);

  /* Check Env caps */
  EnvSensor_GetCaps();
}

/**
 * @brief  Set the notification status (enabled/disabled)
 * @param  status The new notification status
 * @retval None
 */
void ENV_Set_Notification_Status(uint8_t status)
{
  ENV_Server_App_Context.NotificationStatus = status;
}

/**
 * @brief  Send a notification for Environmental char
 * @param  None
 * @retval None
 */
void ENV_Send_Notification_Task(void)
{

  if(ENV_Server_App_Context.NotificationStatus)
  {
    APP_DBG_MSG("-- ENV APPLICATION SERVER : NOTIFY CLIENT WITH NEW ENV PARAMETER VALUE \n ");
    APP_DBG_MSG(" \n\r");
    ENV_Update();
  }
  else
  {
    APP_DBG_MSG("-- ENV APPLICATION SERVER : CAN'T INFORM CLIENT - NOTIFICATION DISABLED\n ");
  }

  return;
}

/**
 * @brief  Update the Environmental char value
 * @param  None
 * @retval None
 */
void ENV_Update(void)
{
  uint8_t tempIndex = 0;
  uint8_t value[VALUE_LEN_ENV];
  uint8_t BuffPos = 2;
  /* Read ENV values */
  ENV_Handle_Sensor();

  /* Timestamp */
  STORE_LE_16(value, (HAL_GetTick()>>3));

  for(tempIndex = 0; tempIndex < ENV_Server_App_Context.hasTemperature; tempIndex++)
  {
    STORE_LE_16(&value[BuffPos], ENV_Server_App_Context.TemperatureValue[tempIndex]);
    BuffPos += TEMPERATURE_BYTES;
  }
  
  Custom_STM_App_Update_Char(CUSTOM_STM_ENV_C, (uint8_t *)&value);

  return;
}

/* Private functions ---------------------------------------------------------*/

/**
 * @brief  Parse the values read by Environmental sensors
 * @param  None
 * @retval None
 */
static void ENV_Handle_Sensor(void)
{
  float temperature = 0.0;
  int32_t decPart= 0, intPart=0;
 
  if(ENV_Server_App_Context.hasTemperature >= 1)
  {    
    BSP_ENV_SENSOR_GetValue(ENV_SENSOR_STTS22H_0, ENV_TEMPERATURE, &temperature);
    MCR_BLUEMS_F2I_1D(temperature, intPart, decPart);
    ENV_Server_App_Context.TemperatureValue[0] = intPart*10+decPart; ;
  }
}

/**
 * @brief  Check the Environmental active capabilities and set the ADV data accordingly
 * @param  None
 * @retval None
 */
static void EnvSensor_GetCaps(void)
{
  ENV_Server_App_Context.hasPressure = 0;
  ENV_Server_App_Context.hasHumidity = 0;
  ENV_Server_App_Context.hasTemperature = 1;
}

