/**
******************************************************************************
* @file    mesh_cfg.c
* @author  BLE Mesh Team
* @version V1.09.000
* @date    15-Oct-2018
* @brief   User configurable settings for BLE-Mesh
******************************************************************************
* @attention
*
* <h2><center>&copy; COPYRIGHT(c) 2017 STMicroelectronics</center></h2>
*
* Redistribution and use in source and binary forms, with or without modification,
* are permitted provided that the following conditions are met:
*   1. Redistributions of source code must retain the above copyright notice,
*      this list of conditions and the following disclaimer.
*   2. Redistributions in binary form must reproduce the above copyright notice,
*      this list of conditions and the following disclaimer in the documentation
*      and/or other materials provided with the distribution.
*   3. Neither the name of STMicroelectronics nor the names of its contributors
*      may be used to endorse or promote products derived from this software
*      without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
* AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
* IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
* DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
* FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
* DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
* SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
* CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
* OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
* OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*
* Initial BLE-Mesh is built over Motorola’s Mesh over Bluetooth Low Energy 
* (MoBLE) technology. The present solution is developed and maintained for both 
* Mesh library and Applications solely by STMicroelectronics.
*
******************************************************************************
*/

/* Includes ------------------------------------------------------------------*/
#include "mesh_cfg.h"
#include "types.h"
#include "ble_mesh.h"
#include "compiler.h"

/* Note: Please use Full Library configuration in project options to use the full 
         configuration of the C/C++ runtime library for printf and scanf functionality */
#if (ENABLE_SERIAL_CONTROL)
#if (!ENABLE_USART)
#error "Serial Interface also needed for Serial Control Application. Enable it using "ENABLE_USART""
#else
#include "serial_ctrl.c"
#include "serial_if.c"
#endif
#endif

/* Private define ------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

__attribute__((aligned(4))) 
MOBLEUINT8 dyn_buffer_m[DYNAMIC_MEMORY_SIZE + FRIEND_BUFF_DYNAMIC_MEMORY_SIZE + MAX_APPLICATION_PACKET_SIZE + SAR_BUFFER_SIZE ] = {0};

#if defined(BLUENRG1_DEVICE) || defined(BLUENRG2_DEVICE) /* BLUENRG1 or BLUENRG2 */
ALIGN(4)
#endif
const DynBufferParam_t DynBufferParam =
{
  dyn_buffer_m,
  (MOBLEUINT16) (DYNAMIC_MEMORY_SIZE + SAR_BUFFER_SIZE ),
  (MOBLEUINT16) FRIEND_BUFF_DYNAMIC_MEMORY_SIZE,
  (MOBLEUINT16) MAX_APPLICATION_PACKET_SIZE,
};
/* 16 octets Static OOB information to be input here. Used during provisioning by Library */
const uint8_t StaticOobBuff[STATIC_OOB_SIZE] = {0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01};

const tr_params_t TrParams = TRANSMIT_RECEIVE_PARAMS;
#if defined(BLUENRG1_DEVICE) || defined(BLUENRG2_DEVICE) /* BLUENRG1 or BLUENRG2 */
const lpn_params_t LpnParams = LOW_POWER_NODE_PARAMS;
#else
const lpn_params_t LpnParams;
#endif
const fn_params_t FnParams = FRIEND_NODE_PARAMS;
const unprov_node_info_params_t UnprovNodeInfoParams = UNPROV_NODE_INFO_PARAMS;
/* Private functions ---------------------------------------------------------*/

/* PLEASE REFER TO THE .h file for different settings */

/******************* (C) COPYRIGHT 2017 STMicroelectronics *****END OF FILE****/
