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
;* <h2><center>&copy; COPYRIGHT(c) 2019 STMicroelectronics</center></h2>
;*
;* Redistribution and use in source and binary forms, with or without modification,
;* are permitted provided that the following conditions are met:
;*   1. Redistributions of source code must retain the above copyright notice,
;*      this list of conditions and the following disclaimer.
;*   2. Redistributions in binary form must reproduce the above copyright notice,
;*      this list of conditions and the following disclaimer in the documentation
;*      and/or other materials provided with the distribution.
;*   3. Neither the name of STMicroelectronics nor the names of its contributors
;*      may be used to endorse or promote products derived from this software
;*      without specific prior written permission.
;*
;* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
;* AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
;* IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
;* DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
;* FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
;* DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
;* SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
;* CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
;* OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
;* OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
;*
;* Initial BlueNRG-Mesh is built over Motorola’s Mesh over Bluetooth Low Energy 
;* (MoBLE) technology. The present solution is developed and maintained for both 
;* Mesh library and Applications solely by STMicroelectronics.
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
;/******************* (C) COPYRIGHT 2019 STMicroelectronics *****END OF FILE****/
