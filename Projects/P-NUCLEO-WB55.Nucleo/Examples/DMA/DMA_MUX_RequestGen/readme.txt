/**
  @page DMA_MUX_RequestGen DMA & DMAMUX request generator Example
  
  @verbatim
  ******************************************************************************
  * @file    DMA/DMA_MUX_RequestGen/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the DMA & request generator Example.
  ******************************************************************************
  *
  * Copyright (c) 2019-2021 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  @endverbatim

@par Example Description 

How to use the DMA with the DMAMUX request generator to generate DMA transfer
requests upon an External line 4 rising edge signal.

The example uses the DMA1_Channel1 configured in memory to peripheral mode.
The DMA request is set to the DMAMUX request generator 0.


At the beginning of the main program the HAL_Init() function is called to reset 
all the peripherals, initialize the Flash interface and the systick.
Then the SystemClock_Config() function is used to configure the system
clock (SYSCLK) to run at 64 MHz for STM32WBxx Devices.

The DMA1_Channel1 is configured in memory to peripheral mode to ensure data transfer from the source transmission 
buffer (SRC_Buffer_LED2_Toggle) to the LED2 GPIO ODR register (in order to toggle LED2).
The DMA is configured in circular mode so the transfer will restart automatically each time the amount of data 
to be transmitted has been reached.

The DMAMUX request generator block is configured using function "HAL_DMAEx_ConfigMuxRequestGenerator" 
with the following parameters :
- SignalID      : set to HAL_DMAMUX1_REQ_GEN_EXTI4 which corresponds to External line 4 signal.
- Polarity  : Set to RISING to use rising edge the External line 4 for DMA requests generation.
- RequestNumber : 1 i.e on each rising edge of the External line 4 signal a DMA request is generated.

The DMA request generator is then enabled using function "HAL_DMAEx_EnableMuxRequestGenerator".

The function BSP_PB_Init is then used to configure the PC.04 pin to  
external Interrupt Mode with Rising edge trigger detection. 

Then the DMA transfer is started in non-blocking mode using the HAL function "HAL_DMA_Start_IT"
Note that PC.04 pin is connected to the User push-button (SW1) of the board.
Each time the User push-button (SW1) is pressed an External line 4 event is generated and the DMAMUX will generate a DMA request
upon the rising edge of the External line 4 signal.
As consequence the DMA will serve the request and write a new value to the LED2 GPIO ODR register to toggle the LED2 
without any CPU intervention.

The CPU is only used to intercept a DMA transfer interrupt error or a DMAMUX overrun interrupt error if any.
Then it sets the LED3 (Red LED) to On in this case.


P-NUCLEO-WB55 board's LEDs can be used to monitor the transfer status:
 - LED2 toggles each time the PC.04 is pressed.
 - LED3 is ON when there is an error during the DMA transfer.

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.
	  
@par Keywords

System, DMA, Data Transfer, Memory to memory, Channel, Flash

@par Directory contents 

  - DMA/DMAMUX_RequestGen/Inc/stm32wbxx_hal_conf.h    HAL configuration file
  - DMA/DMAMUX_RequestGen/Inc/stm32wbxx_it.h          DMA interrupt handlers header file
  - DMA/DMAMUX_RequestGen/Inc/main.h                  Header for main.c module  
  - DMA/DMAMUX_RequestGen/Src/stm32wbxx_it.c          DMA interrupt handlers
  - DMA/DMAMUX_RequestGen/Src/main.c                  Main program
  - DMA/DMAMUX_RequestGen/Src/system_stm32wbxx.c      STM32WBxx system source file
  - DMA/DMAMUX_RequestGen/Src/stm32wbxx_hal_msp.c     HAL MSP module

@par Hardware and Software environment

  - This example runs on STM32WB55RGVx devices.
    
  - This example has been tested with P-NUCLEO-WB55 board and can be
    easily tailored to any other supported device and development board.

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
