/**
  ******************************************************************************
  * @file    ble_hal.h
  * @author  AMS - VMA RF Application team
  * @version V1.0.0
  * @date    04-September-2018
  * @brief   Header file with HAL define for BLE core
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

#ifndef __BLE_HAL_H__
#define __BLE_HAL_H__

/**
 * @defgroup Config_vals Offsets and lengths for configuration values.
 * @brief Offsets and lengths for configuration values.
 * 		  See aci_hal_write_config_data().
 * @{
 */

/**
 * @name Configuration values.
 * See @ref aci_hal_write_config_data().
 * @{
 */
#define CONFIG_DATA_PUBADDR_OFFSET          (0x00) /**< Bluetooth public address */
#define CONFIG_DATA_DIV_OFFSET              (0x06) /**< DIV used to derive CSRK */
#define CONFIG_DATA_ER_OFFSET               (0x08) /**< Encryption root key used to derive LTK and CSRK */
#define CONFIG_DATA_IR_OFFSET               (0x18) /**< Identity root key used to derive LTK and CSRK */
#define CONFIG_DATA_RANDOM_ADDRESS_OFFSET   (0x2E) /**< Static Random Address */

/**
 * @}
 */

/**
 * @name Length for configuration values.
 * See @ref aci_hal_write_config_data().
 * @{
 */
#define CONFIG_DATA_PUBADDR_LEN             (6)
#define CONFIG_DATA_DIV_LEN                 (2)
#define CONFIG_DATA_ER_LEN                  (16)
#define CONFIG_DATA_IR_LEN                  (16)
#define CONFIG_DATA_RANDOM_ADDRESS_LEN      (6)

/**
 * @}
 */

/**
 * @}
 */


/**
 * @}
 */


#endif /* __BLE_HAL_ACI_H__ */
