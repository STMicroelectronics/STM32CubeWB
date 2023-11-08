/**
  ******************************************************************************
  * @file    phy_test.h
  * @author  MCD Application Team
  * @brief   This file contains the PHY interface shared between
  *          M0 and M4.
  ******************************************************************************
  * @attention
 *
 * Copyright (c) 2023 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */


#ifndef PHY_TEST_H_

uint8_t phyTestMode(uint8_t enableDisable);
uint8_t phyTestSetChannel(uint8_t channel_nb);

uint8_t phyTestContinuousWaveStart(uint16_t frq_mhz);
uint8_t phyTestContinuousWaveStop(void);

uint8_t phyTestTxStart(uint32_t nb_frames, uint8_t size_of_frame, uint8_t *tx_frame);

uint8_t phyTestRxStart(void);
uint32_t phyTestRxStop(void);

#endif // PHY_TEST_H_
