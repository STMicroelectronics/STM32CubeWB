;/**
;******************************************************************************
;* @file    data.s
;* @author  BLE Mesh Team
;* @version V1.07.000
;* @date    15-June-2018
;* @brief   Memory management 
;******************************************************************************
;* @attention
;*
;* Copyright (c) 2019 STMicroelectronics.
;* All rights reserved.
;*
;* This software is licensed under terms that can be found in the LICENSE file
;* in the root directory of this software component.
;* If no LICENSE file comes with this software, it is provided AS-IS.
;*
;******************************************************************************
;*/
_bdaddr EQU 0x0807E000
    EXPORT _bdaddr

    AREA    CONFIG, READONLY
_mobleNvmBase_data
    EXPORT _mobleNvmBase_data
    FILL 4096, 0x00

    END
