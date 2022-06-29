/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    ble_conf.h
  * @author  MCD Application Team
  * @brief   Configuration file for BLE Middleware.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2020-2021 STMicroelectronics.
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
 * Device Information Service (DIS)
 ******************************************************************************/
/**< Options: Supported(1) or Not Supported(0) */
#define BLE_CFG_DIS_MANUFACTURER_NAME_STRING                                   1
#define BLE_CFG_DIS_MODEL_NUMBER_STRING                                        1
#define BLE_CFG_DIS_SERIAL_NUMBER_STRING                                       0
#define BLE_CFG_DIS_HARDWARE_REVISION_STRING                                   0
#define BLE_CFG_DIS_FIRMWARE_REVISION_STRING                                   0
#define BLE_CFG_DIS_SOFTWARE_REVISION_STRING                                   0
#define BLE_CFG_DIS_SYSTEM_ID                                                  1
#define BLE_CFG_DIS_IEEE_CERTIFICATION                                         0
#define BLE_CFG_DIS_PNP_ID                                                     0

/**
 * device information service characteristic lengths
 */
#define BLE_CFG_DIS_SYSTEM_ID_LEN_MAX                                        (8)
#define BLE_CFG_DIS_MODEL_NUMBER_STRING_LEN_MAX                              (32)
#define BLE_CFG_DIS_SERIAL_NUMBER_STRING_LEN_MAX                             (32)
#define BLE_CFG_DIS_FIRMWARE_REVISION_STRING_LEN_MAX                         (32)
#define BLE_CFG_DIS_HARDWARE_REVISION_STRING_LEN_MAX                         (32)
#define BLE_CFG_DIS_SOFTWARE_REVISION_STRING_LEN_MAX                         (32)
#define BLE_CFG_DIS_MANUFACTURER_NAME_STRING_LEN_MAX                         (32)
#define BLE_CFG_DIS_IEEE_CERTIFICATION_LEN_MAX                               (32)
#define BLE_CFG_DIS_PNP_ID_LEN_MAX                                           (7)

/******************************************************************************
 * Health Thermometer Service (HTS)
 ******************************************************************************/
/**
 * TEMPERATURE TYPE
 * When set to '1', the TEMPERATURE TYPE characteristic shall be added and the Temperature type Info flag shall not be present
 * When set to '0', the TEMPERATURE TYPE characteristic shall not be supported and the Temperature type Info flag shall be present
 */

#define BLE_CFG_HTS_TEMPERATURE_TYPE_VALUE_STATIC               0

#define BLE_CFG_HTS_INTERMEDIATE_TEMPERATURE               1  /**< Intermediate Temperature characteristic */
#define BLE_CFG_HTS_MEASUREMENT_INTERVAL               0  /**< Measurement Interval characteristic */
#define BLE_CFG_HTS_MEASUREMENT_INTERVAL_IND_PROP               0  /**< Measurement Interval characteristic Indicate property*/
#define BLE_CFG_HTS_MEASUREMENT_INTERVAL_WR_PROP               0  /**< Measurement Interval characteristic Write property */
#define BLE_CFG_HTS_TIME_STAMP_FLAG               1  /**< Time Stamp Info Flag */
#define BLE_CFG_HTS_TEMPERATURE_INTERVAL_MIN_VALUE               1  /**< Min Interval Value supported */
#define BLE_CFG_HTS_TEMPERATURE_INTERVAL_MAX_VALUE               UINT16_MAX /**< Max Interval Value supported */

/******************************************************************************
 * GAP Service - Appearance
 ******************************************************************************/

#define BLE_CFG_UNKNOWN_APPEARANCE                  (0)
#define BLE_CFG_HR_SENSOR_APPEARANCE                (832)
#define BLE_CFG_GAP_APPEARANCE                      (BLE_CFG_UNKNOWN_APPEARANCE)

#endif /*BLE_CONF_H */
