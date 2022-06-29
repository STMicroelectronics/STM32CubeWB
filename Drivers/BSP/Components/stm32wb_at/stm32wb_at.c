/**
  ******************************************************************************
  * @file    stm32wb_at.c
  * @author  MCD Application Team
  * @brief   Transport layer.
  *
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32wb_at.h"
#include "stm32wb_at_ll.h"
#include "stm32wb_at_client.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static uint8_t *buffer_rx;
static uint8_t buffer_rx_size;
static uint8_t buffer_rx_cursor;
static char str_received[64];

float tab_conv_tx_power[32] = {-40.00f, -20.85f, -19.75f, -18.85f, 
                               -17.60f, -16.50f, -15.25f, -14.10f,
                               -13.15f, -12.05f, -10.90f, -9.90f,
                               -8.85f, -7.80f, -6.90f, -5.90f,
                               -4.95f, -4.00f, -3.15f, -2.45f,
                               -1.80f, -1.30f, -0.85f, -0.50f,
                               -0.15f, 0.00f, 1.00f, 2.00f,
                               3.00f, 4.00f, 5.00f, 6.00f};
/* Private function prototypes -----------------------------------------------*/

/* Exported functions --------------------------------------------------------*/

/**
 * @brief Init the at module
 * @param None
 * @retval 0 in case of success, an error code otherwise
 */
uint8_t stm32wb_at_Init(uint8_t *buff_rx, uint8_t buff_rx_size)
{
  uint8_t status;

  if(buff_rx_size >= sizeof(str_received))
  {
    buffer_rx_size = buff_rx_size;
    buffer_rx = buff_rx;
    (void)memset(&buffer_rx[0], 0, buffer_rx_size);
    buffer_rx_cursor = 0;
    (void)memset(&str_received[0], 0, sizeof(str_received));

    stm32wb_at_ll_Async_receive(0);
    status = 0;
  }
  else
  {
    status = 1;
  }
  return status;
}

/**
 * @brief Accumulate bytes received and detects a new frame received
 * @param byte the last byte received
 * @retval 0 in case of success, an error code otherwise
 */
uint8_t stm32wb_at_Received(uint8_t byte)
{
  uint8_t status, i;

  if(buffer_rx_cursor < buffer_rx_size)
  {
    buffer_rx[buffer_rx_cursor] = byte;
    buffer_rx_cursor++;

    /* check if we have received the command delimiter LF */
    if(byte == (uint8_t)'\n')
    {
      /* remove CR if any */
      if(buffer_rx_cursor > 2U)
      {
        if( buffer_rx[buffer_rx_cursor - 2U] == (uint8_t)'\r')
        {
          buffer_rx[buffer_rx_cursor - 1U] = 0U;
          buffer_rx[buffer_rx_cursor - 2U] = (uint8_t)'\n';
          buffer_rx_cursor--;
        }
      }
      /* remove the last \n */
      buffer_rx[buffer_rx_cursor - 1U] = 0U;
      buffer_rx_cursor--;

      /* remove null leading character if any*/
      if( buffer_rx[0] == 0U)
      {
        i = 1U;
        while(buffer_rx[i] != 0U)
        {
          buffer_rx[i - 1U] = buffer_rx[i];
          i++;
        }
        buffer_rx[i - 1U] = 0U;
      }


      /* check if command is long enough */
      if(buffer_rx_cursor > 1U)
      {
        (void)strcpy(&str_received[0], (char*)&buffer_rx[0]);

        stm32wb_at_ll_Async_receive((uint8_t)sizeof(str_received));

        (void)memset(&buffer_rx[0], 0, buffer_rx_size);
        buffer_rx_cursor = 0U;

        (void)stm32wb_at_Process_rx_frame(str_received);
        (void)memset(&str_received[0], 0, sizeof(str_received));
      }
      else
      {
        (void)memset(&buffer_rx[0], 0, buffer_rx_size);
        buffer_rx_cursor = 0U;
        
        stm32wb_at_ll_Async_receive(0);
      }
    }
    else
    {
      stm32wb_at_ll_Async_receive(0);
    }
    status = 0U;
  }
  else
  {
    status = 1U;
  }

  return status;
}

/**
 * @brief Convert a string into a byte tab
 * @param char *str_in      pointer to the string to convert.
 * @param uint8_t *tab d    pointer to hold data output.
 * @param uint8_t* tab_len  provide the tab size and will hold size of tab converted.
 * @retval 0 in case of success, an error code otherwise
 */
uint8_t str_to_byte_tab(char *str_in, uint8_t *tab, uint8_t* tab_len)
{
  uint8_t status = 0;
  uint16_t tab_limit, cpt, val;
  char *str_p;
  
  str_p = str_in;
  
  if( strncmp(str_p, "0x", 2) == 0 )
  {
    cpt = 0;
    tab_limit = *tab_len;
    *tab_len = 0;
    str_p++;
    str_p++;
    
    (void)memset(tab, 0, tab_limit);
    
    do
    {
      if(*str_p == ' ')
      {
        str_p++;
        continue;
      }
      else if( (*str_p >= '0') &&  (*str_p <= '9') )
      {
        val = (uint16_t)(*str_p - '0');
      }
      else if( (*str_p >= 'a') &&  (*str_p <= 'f') )
      {
        val = (uint16_t)(*str_p - 'a' + 10);
      }
      else if( (*str_p >= 'A') &&  (*str_p <= 'F') )
      {
        val = (uint16_t)(*str_p - 'A' + 10);
      }
      else
      {
        if( (cpt % 2U) != 0)
        {
          tab[cpt >> 1U] /= 16U;
        }
        status = 0;
        break;
      }
      
      if( (cpt % 2U) == 0U)
      {
        tab[cpt >> 1] += (uint8_t)(val * 16U);
      }
      else
      {
        tab[cpt >> 1] += (uint8_t)val;
      }
      cpt++;
      str_p++;
    }while( cpt < (tab_limit * 2) );
    
    if(cpt == 1U)
    {
      *tab_len = 1;
    }
    else
    {
      *tab_len = (uint8_t)(cpt >> 1);
    }
  }
  else
  {
    status = 1;
  }

  return status;
}

/**
 * @brief Process a frame received
 * @param str pointer string to process
 * @retval 0 in case of success, an error code otherwise
 */
__weak uint8_t stm32wb_at_Process_rx_frame(char * str)
{
  (void)stm32wb_at_client_Process_rx_frame(str);

  return 0;
}
