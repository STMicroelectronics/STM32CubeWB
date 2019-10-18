/**
  ******************************************************************************
  * @file    ble_conf.h
  * @author  MCD Application Team
  * @brief   BLE configuration file
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


/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __BLE_CONF_H
#define __BLE_CONF_H

#include "app_conf.h"

/******************************************************************************
 *
 * BLE SERVICES CONFIGURATION
 * blesvc
 *
 ******************************************************************************/

/**
 * This setting shall be set to '1' if the device needs to support the Peripheral Role
 * In the MS configuration, both BLE_CFG_PERIPHERAL and BLE_CFG_CENTRAL shall be set to '1'
 */
#define BLE_CFG_PERIPHERAL                                                     1

/**
 * This setting shall be set to '1' if the device needs to support the Central Role
 * In the MS configuration, both BLE_CFG_PERIPHERAL and BLE_CFG_CENTRAL shall be set to '1'
 */
#define BLE_CFG_CENTRAL                                                        0

/**
 * There is one handler per service enabled
 * Note: There is no handler for the Device Information Service
 *
 * This shall take into account all registered handlers
 * (from either the provided services or the custom services)
 */
#define BLE_CFG_SVC_MAX_NBR_CB                                                 7

#define BLE_CFG_CLT_MAX_NBR_CB                                                 0

/******************************************************************************
 * Human Interface Device Service (HIDS)
 ******************************************************************************/
#define BLE_CFG_HIDS_NUMBER                                                    1
#define BLE_CFG_HIDS_REPORT_MAP_MAX_LEN                                       80
#define BLE_CFG_HIDS_REPORT_REFERENCE_LEN                                      2
#define BLE_CFG_HIDS_REPORT_MAX_LEN                                           80
#define BLE_CFG_HIDS_EXTERNAL_REPORT_REFERENCE_LEN                             2
#define BLE_CFG_HIDS_BOOT_KEYBOARD_INPUT_REPORT_MAX_LEN                       80
#define BLE_CFG_HIDS_BOOT_KEYBOARD_OUTPUT_REPORT_MAX_LEN                      80
#define BLE_CFG_HIDS_BOOT_MOUSE_INPUT_REPORT_MAX_LEN                          80
#define BLE_CFG_HIDS_INFORMATION_LEN                                           4
#define BLE_CFG_HIDS_CONTROL_POINT_LEN                                         1
#define BLE_CFG_HIDS_PROTOCOL_MODE_LEN                                         1

#define BLE_CFG_HIDS_PROTOCOL_MODE_CHAR                                        0 /**< Support of Protocol Mode Characteristic */
#define BLE_CFG_HIDS_INPUT_REPORT_NB                                           1 /**< Number of Input Report */
#define BLE_CFG_HIDS_INPUT_WRITE                                               1 /**< Support of Input Write property */
#define BLE_CFG_HIDS_OUTPUT_REPORT_NB                                          0 /**< Number of Output Report */
#define BLE_CFG_HIDS_FEATURE_REPORT_NB                                         0 /**< Number of Feature Report */
#define BLE_CFG_HIDS_EXTERNAL_REPORT_REFERENCE                                 0 /**< Support of EXTERNAL REPORT Reference Descriptor */
#define BLE_CFG_HIDS_KEYBOARD_DEVICE                                           0 /**< Support of BOOT KEYBOARD Report Characteristic */
#define BLE_CFG_HIDS_KEYBOARD_INPUT_WRITE                                      0 /**< Support of Keyboard Input Write property */
#define BLE_CFG_HIDS_MOUSE_DEVICE                                              0 /**< Support of BOOT MOUSE Report Characteristic */
#define BLE_CFG_HIDS_MOUSE_INPUT_WRITE                                         0 /**< Support of Mouse Input Write property */
#define BLE_CFG_HIDS_REPORT_CHAR               BLE_CFG_HIDS_INPUT_REPORT_NB   +\
  BLE_CFG_HIDS_OUTPUT_REPORT_NB  +\
  BLE_CFG_HIDS_FEATURE_REPORT_NB
#define BLE_CFG_HIDS_PROTOCOL_MODE                                             1 /**< Report Protocol Mode */


#endif /*__BLE_CONF_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
