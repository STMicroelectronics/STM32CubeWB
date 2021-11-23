/**
 ******************************************************************************
 * @file    hids_menu.h
 * @author  MCD Application Team
 * @brief   Header for hids_menu.c module
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
#ifndef __HIDS_MENU_H
#define __HIDS_MENU_H

#ifdef __cplusplus
extern "C" 
{
#endif

  
/* Includes ------------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* External variables --------------------------------------------------------*/
/* Exported macros -----------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void HIDS_Menu (uint8_t * action_type, uint8_t * report);


#ifdef __cplusplus
}
#endif

#endif /*__HIDS_MENU_H */


