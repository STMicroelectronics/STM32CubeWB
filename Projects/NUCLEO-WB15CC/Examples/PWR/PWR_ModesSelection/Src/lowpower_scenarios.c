/**
  ******************************************************************************
  * @file    PWR/PWR_ModesSelection/Src/lowpower_scenarios.c
  * @author  MCD Application Team
  * @brief   This file provides low power scenarios
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
/* USER CODE BEGIN 1 */
/* Includes ------------------------------------------------------------------*/
#include "lowpower_scenarios.h"
#include <string.h>
#include <stdlib.h>

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint8_t Scenario_CmdBuff[SCENARIO_CMDBUF_SIZE + 1U] = {DUMMY_PATTERN, 0x00};

/* Private function prototypes -----------------------------------------------*/
static Scenario_StatusTypeDef get_lowpower_cmd(LowPower_ScenarioTypeDef *scenario);
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Print the header msg on Hyper-terminal.
  * @param  scenario : Pointer to a LowPower_ScenarioTypeDef structure that contains system low power information.
  * @retval Scenario Status.
  */
Scenario_StatusTypeDef lowpower_header_scenario(LowPower_ScenarioTypeDef *scenario)
{
  /* Clear scenario variable */
  memset((void *)scenario, 0U, sizeof(LowPower_ScenarioTypeDef));

  /* Clear the terminal */
  Console_Log(clear_term_command);

  /* Print header application */
  Console_Log(empty_line_msg);
  Console_Log(header_limiter_msg);
  Console_Log(header_start_msg);
  Console_Log(header_limiter_msg);
  Console_Log(empty_line_msg);

  return SCENARIO_OK;
}

/**
  * @brief  Print the footer msg on Hyper-terminal.
  * @param  scenario : Pointer to a LowPower_ScenarioTypeDef structure that contains system low power information.
  * @retval Scenario Status.
  */
Scenario_StatusTypeDef lowpower_footer_scenario(LowPower_ScenarioTypeDef *scenario)
{
  /* Print the configuration msg */
  Console_Log(system_lowpower_config_msg);

  /* Print the low power mode selected */
  if (scenario->lowpower_mode_str != NULL)
  {
    Console_Log(empty_space_msg);
    Console_Log(scenario->lowpower_mode_str);
  }

  /* Print the rtc state selected */
  if (scenario->rtc_state_str != NULL)
  {
    Console_Log(empty_space_msg);
    Console_Log(scenario->rtc_state_str);
  }

  /* Print the ram retention selected */
  if (scenario->ram_retention_str != NULL)
  {
    Console_Log(empty_space_msg);
    Console_Log(scenario->ram_retention_str);
  }

  /* Print the regulator selected */
  if (scenario->regulator_str != NULL)
  {
    Console_Log(empty_space_msg);
    Console_Log(scenario->regulator_str);
  }

  /* Print the clock frequency selected */
  if (scenario->clock_str != NULL)
  {
    Console_Log(empty_space_msg);
    Console_Log(scenario->clock_str);
  }

  /* Print measuring msg */
  Console_Log(empty_line_msg);
  Console_Log(start_measure_msg);

  return SCENARIO_OK;
}

/**
  * @brief  Check the configuration selected.
  * @param  scenario : Pointer to a LowPower_ScenarioTypeDef structure that contains system low power information.
  * @retval Scenario Status.
  */
Scenario_StatusTypeDef lowpower_get_scenario(LowPower_ScenarioTypeDef *scenario)
{
  /* Get system low power mode command from a User input */
  if (get_lowpower_cmd(scenario) != SCENARIO_OK)
  {
    return SCENARIO_ERROR;
  }

  return SCENARIO_OK;
}

/**
  * @brief  Configure low power mode.
  * @param  scenario : Pointer to a LowPower_ScenarioTypeDef structure that contains system low power information.
  * @retval Scenario Status.
  */
Scenario_StatusTypeDef lowpower_config_scenario(LowPower_ScenarioTypeDef *scenario)
{
  /* Configure ram retention */
  if (system_ram_retention_config(scenario->lowpower_mode) != SYSTEM_OK)
  {
    return SCENARIO_ERROR;
  }

  /* Configure system frequency and voltage scaling */
  if (system_clock_config(scenario->lowpower_mode) != SYSTEM_OK)
  {
    return SCENARIO_ERROR;
  }

  /* Configure rtc state */
  if (system_rtc_config(scenario->lowpower_mode) != SYSTEM_OK)
  {
    return SCENARIO_ERROR;
  }

  /* General system configuration */
  system_config(scenario->lowpower_mode);

  /* Configure enter low power mode */
  if (system_lowpower_config(scenario->lowpower_mode) != SYSTEM_OK)
  {
    return SCENARIO_ERROR;
  }

  return SCENARIO_OK;
}

/**
  * @brief  Get system low power mode command.
  * @param  scenario : Pointer to a LowPower_ScenarioTypeDef structure that contains system low power information.
  * @retval Scenario Status.
  */
static Scenario_StatusTypeDef get_lowpower_cmd(LowPower_ScenarioTypeDef *scenario)
{
  uint32_t cmd;


  Console_Log(power_mode_select_msg);   /* Print low power mode selection msg */
  Console_Log(index0_msg);              /* Print index msg */
  Console_Log(lowpower_mode_msg[0U]);   /* Print system low power mode */
  Console_Log(index1_msg);
  Console_Log(lowpower_mode_msg[1U]);
  Console_Log(index2_msg);
  Console_Log(lowpower_mode_msg[2U]);
  Console_Log(index3_msg);
  Console_Log(lowpower_mode_msg[3U]);
  Console_Log(index4_msg);
  Console_Log(lowpower_mode_msg[4U]);
  Console_Log(index5_msg);
  Console_Log(lowpower_mode_msg[5U]);
  Console_Log(index6_msg);
  Console_Log(lowpower_mode_msg[6U]);
  Console_Log(index7_msg);
  Console_Log(lowpower_mode_msg[7U]);
  Console_Log(index8_msg);
  Console_Log(lowpower_mode_msg[8U]);
  Console_Log(index9_msg);
  Console_Log(lowpower_mode_msg[9U]);
  Console_Log(user_cursor_msg);         /* Print cursor */

  /* Repeat until getting valid command (digital values, and only in accepted range) */
  do
  {
    /* Receive system low power mode command */
    if (Console_GetCommand(Scenario_CmdBuff, SCENARIO_CMDBUF_SIZE) != CONSOLE_OK)
    {
      return SCENARIO_ERROR;
    }

    /* Convert char to an integer */
    cmd = atoi((char *)Scenario_CmdBuff);

  } while ((Scenario_CmdBuff[0U] < CODE_ASCII_0) || (Scenario_CmdBuff[0U] > CODE_ASCII_9));

  /* Print index selected */
  Console_Log((char *)Scenario_CmdBuff);
  Console_Log(empty_line_msg);
  Console_Log(empty_line_msg);

  if ((System_LowPowerModeScenarioTypeDef)cmd >= SYSTEM_LOWPOWER_NB)
  {
    /* Inform user that selected value is out of range */
    Console_Log(error_selection_msg);
    HAL_Delay(5000);

    /* System reset */
    HAL_NVIC_SystemReset();
  }

  /* Get system low power mode */
  scenario->lowpower_mode     = (System_LowPowerModeScenarioTypeDef)cmd;
  scenario->lowpower_mode_str = (char *)lowpower_mode_msg[cmd];

  /* Initialize command buffer */
  Scenario_CmdBuff[0U] = DUMMY_PATTERN;

  return SCENARIO_OK;
}
/* USER CODE END 1 */