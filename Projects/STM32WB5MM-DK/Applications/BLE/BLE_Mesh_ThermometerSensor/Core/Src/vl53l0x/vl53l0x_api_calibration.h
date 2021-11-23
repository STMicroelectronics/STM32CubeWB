/**
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

#ifndef _VL53L0X_API_CALIBRATION_H_
#define _VL53L0X_API_CALIBRATION_H_

#include "vl53l0x_def.h"
#include "vl53l0x_platform.h"


#ifdef __cplusplus
extern "C" {
#endif

VL53L0X_Error VL53L0X_perform_xtalk_calibration(VL53L0X_DEV Dev,
		FixPoint1616_t XTalkCalDistance,
		FixPoint1616_t *pXTalkCompensationRateMegaCps);

VL53L0X_Error VL53L0X_perform_offset_calibration(VL53L0X_DEV Dev,
		FixPoint1616_t CalDistanceMilliMeter,
		int32_t *pOffsetMicroMeter);

VL53L0X_Error VL53L0X_set_offset_calibration_data_micro_meter(VL53L0X_DEV Dev,
		int32_t OffsetCalibrationDataMicroMeter);

VL53L0X_Error VL53L0X_get_offset_calibration_data_micro_meter(VL53L0X_DEV Dev,
		int32_t *pOffsetCalibrationDataMicroMeter);

VL53L0X_Error VL53L0X_apply_offset_adjustment(VL53L0X_DEV Dev);

VL53L0X_Error VL53L0X_perform_ref_spad_management(VL53L0X_DEV Dev,
		uint32_t *refSpadCount, uint8_t *isApertureSpads);

VL53L0X_Error VL53L0X_set_reference_spads(VL53L0X_DEV Dev,
		uint32_t count, uint8_t isApertureSpads);

VL53L0X_Error VL53L0X_get_reference_spads(VL53L0X_DEV Dev,
		uint32_t *pSpadCount, uint8_t *pIsApertureSpads);

VL53L0X_Error VL53L0X_perform_phase_calibration(VL53L0X_DEV Dev,
	uint8_t *pPhaseCal, const uint8_t get_data_enable,
	const uint8_t restore_config);

VL53L0X_Error VL53L0X_perform_ref_calibration(VL53L0X_DEV Dev,
	uint8_t *pVhvSettings, uint8_t *pPhaseCal, uint8_t get_data_enable);

VL53L0X_Error VL53L0X_set_ref_calibration(VL53L0X_DEV Dev,
		uint8_t VhvSettings, uint8_t PhaseCal);

VL53L0X_Error VL53L0X_get_ref_calibration(VL53L0X_DEV Dev,
		uint8_t *pVhvSettings, uint8_t *pPhaseCal);

VL53L0X_Error enable_spad_bit(uint8_t spadArray[], uint32_t size,
	uint32_t spadIndex);

VL53L0X_Error count_enabled_spads(uint8_t spadArray[],
		uint32_t byteCount, uint32_t maxSpads,
		uint32_t *pTotalSpadsEnabled, uint8_t *pIsAperture);

VL53L0X_Error set_ref_spad_map(VL53L0X_DEV Dev, uint8_t *refSpadArray);

VL53L0X_Error get_ref_spad_map(VL53L0X_DEV Dev, uint8_t *refSpadArray);

VL53L0X_Error enable_ref_spads(VL53L0X_DEV Dev,
				uint8_t apertureSpads,
				uint8_t goodSpadArray[],
				uint8_t spadArray[],
				uint32_t size,
				uint32_t start,
				uint32_t offset,
				uint32_t spadCount,
				uint32_t *lastSpad);

VL53L0X_Error perform_ref_signal_measurement(VL53L0X_DEV Dev,
		uint16_t *refSignalRate);


VL53L0X_Error VL53L0X_perform_single_ref_calibration(VL53L0X_DEV Dev,
		uint8_t vhv_init_byte);

VL53L0X_Error VL53L0X_ref_calibration_io(VL53L0X_DEV Dev, uint8_t read_not_write,
	uint8_t VhvSettings, uint8_t PhaseCal,
	uint8_t *pVhvSettings, uint8_t *pPhaseCal,
	const uint8_t vhv_enable, const uint8_t phase_enable);

VL53L0X_Error VL53L0X_perform_vhv_calibration(VL53L0X_DEV Dev,
	uint8_t *pVhvSettings, const uint8_t get_data_enable,
	const uint8_t restore_config);

void get_next_good_spad(uint8_t goodSpadArray[], uint32_t size,
			uint32_t curr, int32_t *next);

uint8_t is_aperture(uint32_t spadIndex);

#ifdef __cplusplus
}
#endif

#endif /* _VL53L0X_API_CALIBRATION_H_ */
