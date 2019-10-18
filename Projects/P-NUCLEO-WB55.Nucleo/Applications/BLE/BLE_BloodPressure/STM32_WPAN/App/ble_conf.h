/**
 ******************************************************************************
  * File Name          : App/ble_conf.h
  * Description        : Configuration file for BLE Middleware.
  *
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
#ifndef BLE_CONF_H
#define BLE_CONF_H

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
 * Blood Pressure Service (BLS)
 ******************************************************************************/
/**
 * MEASUREMENT STATUS
 * When set to '1', the MEASUREMENT STATUS field  shall be added and the Measurement status Info flag shall not be present
 * When set to '0', the MEASUREMENT STATUS field shall not be added and the Measurement status Info flag shall be present
 * The same logic apply to PULSE RATE, Time Stamp and USER ID fields
 */
#define BLE_CFG_BLS_MEASUREMENT_STATUS_FLAG                 1/**< Measurement Status info flag */
#define BLE_CFG_BLS_PULSE_RATE_FLAG                         1/**< Pulse Rate info flag */
#define BLE_CFG_BLS_USER_ID_FLAG                            1/**< User ID info flag */
#define BLE_CFG_BLS_TIME_STAMP_FLAG                         1/**< Time Stamp Info Flag */
#define BLE_CFG_BLS_INTERMEDIATE_CUFF_PRESSURE              1/**< Intermediate Cuff Pressure characteristic */
#define BLE_CFG_BLS_SUPPORTED_FEATURES                      1/**< Blood Pressure Feature characteristic */
#define BLE_CFG_BLS_PULSE_RATE_LOWER_LIMIT                 40/**< Lower Pulse Rate supported */
#define BLE_CFG_BLS_PULSE_RATE_UPPER_LIMIT                 180/**< Upper Pluse Rate supported */

/******************************************************************************
 * GAP Service - Apprearance
 ******************************************************************************/

#define BLE_CFG_UNKNOWN_APPEARANCE                  (0)
#define BLE_CFG_HR_SENSOR_APPEARANCE                (832)
#define BLE_CFG_GAP_APPEARANCE                      (BLE_CFG_UNKNOWN_APPEARANCE)

#endif /*BLE_CONF_H */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
