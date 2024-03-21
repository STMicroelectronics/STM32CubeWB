/*
 * gpio_lld.h
 *
 */

#ifndef GPIO_LLD_H_
#define GPIO_LLD_H_

#define GPIO_HARD_FAULT_PIN    GPIO_PIN_4
#define GPIO_HARD_FAULT_PORT   GPIOA

// External PA TX/RX pin is fixed by the chip
#define GPIO_EXT_PA_TX_PIN     GPIO_PIN_0
#define GPIO_EXT_PA_TX_PORT    GPIOB
// External PA enable pin is chosen by user
#define GPIO_EXT_PA_EN_PIN     GPIO_PIN_9
#define GPIO_EXT_PA_EN_PORT    GPIOB

void gpio_lld_phy_init( void );
void gpio_lld_phy_gpioTx_up(void);
void gpio_lld_phy_gpioTx_down(void);
void gpio_lld_phy_gpioHardFault_up(void);
void gpio_lld_phy_gpioHardFault_down(void);
void gpio_lld_phy_gpio1_up(void);
void gpio_lld_phy_gpio1_down(void);
void gpio_lld_phy_gpio2_up(void);
void gpio_lld_phy_gpio2_down(void);
void gpio_lld_phy_deInit(void);

void gpio_lld_mco_init(uint32_t mcoSource, uint32_t mcoDiv);
void gpio_lld_mco_deInit(void);

void gpio_lld_extPa_init(void);
void gpio_lld_extPa_deInit(void);

#ifdef USE_SIMU
void gpio_lld_SimuMaster_init(void);
void gpio_lld_SimuSlave_init(void);
#endif

void gpio_lld_lpuart_init(void);
void gpio_lld_lpuart_deInit(void);

void gpio_lld_usart_init(void);
void gpio_lld_usart_deInit(void);

#if !defined (USE_SIMU) && !defined(USE_FPGA)
void gpio_lld_pa2_init(uint8_t mode);
void gpio_lld_pa2_deInit(void);

void gpio_lld_dtb_init(uint8_t dtbMode);
void gpio_lld_dtb_deInit(void);
#endif

void gpio_lld_led1_toggle(void);
void gpio_lld_led2_toggle(void);
void gpio_lld_led3_toggle(void);

#ifdef STM32WB15xx
#define BUTTON_SW1_EXTI_IRQHandler              EXTI0_IRQHandler
#define BUTTON_SW2_EXTI_IRQHandler              EXTI4_IRQHandler
#define BUTTON_SW3_EXTI_IRQHandler              EXTI9_5_IRQHandler

/**
  * @brief  USART pins
  */
#define USART_CLK_ENABLE()                    __HAL_RCC_USART1_CLK_ENABLE()

#define USART_TX_AF                           GPIO_AF7_USART1
#define USART_TX_GPIO_PORT                    GPIOA
#define USART_TX_PIN                          GPIO_PIN_9
#define USART_TX_GPIO_CLK_ENABLE()            __HAL_RCC_GPIOA_CLK_ENABLE()
#define USART_TX_GPIO_CLK_DISABLE()           __HAL_RCC_GPIOA_CLK_DISABLE()

#define USART_RX_AF                           GPIO_AF7_USART1
#define USART_RX_GPIO_PORT                    GPIOA
#define USART_RX_PIN                          GPIO_PIN_10
#define USART_RX_GPIO_CLK_ENABLE()            __HAL_RCC_GPIOA_CLK_ENABLE()
#define USART_RX_GPIO_CLK_DISABLE()           __HAL_RCC_GPIOA_CLK_DISABLE()

/**
  * @brief  LPUART pins
  */
#define LPUART_CLK_ENABLE()                    __HAL_RCC_LPUART1_CLK_ENABLE()

#define LPUART_TX_AF                          GPIO_AF8_LPUART1
#define LPUART_TX_GPIO_PORT                   GPIOA
#define LPUART_TX_PIN                         GPIO_PIN_2
#define LPUART_TX_GPIO_CLK_ENABLE()           __HAL_RCC_GPIOA_CLK_ENABLE()
#define LPUART_TX_GPIO_CLK_DISABLE()          __HAL_RCC_GPIOA_CLK_DISABLE()

#define LPUART_RX_AF                          GPIO_AF8_LPUART1
#define LPUART_RX_GPIO_PORT                   GPIOA
#define LPUART_RX_PIN                         GPIO_PIN_3
#define LPUART_RX_GPIO_CLK_ENABLE()           __HAL_RCC_GPIOA_CLK_ENABLE()
#define LPUART_RX_GPIO_CLK_DISABLE()          __HAL_RCC_GPIOA_CLK_DISABLE()
#endif

#ifdef STM32WB35xx
#define BUTTON_SW1_EXTI_IRQHandler              EXTI0_IRQHandler
#define BUTTON_SW2_EXTI_IRQHandler              EXTI4_IRQHandler
#define BUTTON_SW3_EXTI_IRQHandler              EXTI9_5_IRQHandler

/**
  * @brief  USART pins
  */
#define USART_CLK_ENABLE()                    __HAL_RCC_USART1_CLK_ENABLE()

#define USART_TX_AF                           GPIO_AF7_USART1
#define USART_TX_GPIO_PORT                    GPIOB
#define USART_TX_PIN                          GPIO_PIN_6
#define USART_TX_GPIO_CLK_ENABLE()            __HAL_RCC_GPIOB_CLK_ENABLE()
#define USART_TX_GPIO_CLK_DISABLE()           __HAL_RCC_GPIOB_CLK_DISABLE()

#define USART_RX_AF                           GPIO_AF7_USART1
#define USART_RX_GPIO_PORT                    GPIOB
#define USART_RX_PIN                          GPIO_PIN_7
#define USART_RX_GPIO_CLK_ENABLE()            __HAL_RCC_GPIOB_CLK_ENABLE()
#define USART_RX_GPIO_CLK_DISABLE()           __HAL_RCC_GPIOB_CLK_DISABLE()

/**
  * @brief  LPUART pins
  */
#define LPUART_CLK_ENABLE()                    __HAL_RCC_LPUART1_CLK_ENABLE()

#define LPUART_TX_AF                          GPIO_AF8_LPUART1
#define LPUART_TX_GPIO_PORT                   GPIOB
#define LPUART_TX_PIN                         GPIO_PIN_5
#define LPUART_TX_GPIO_CLK_ENABLE()           __HAL_RCC_GPIOB_CLK_ENABLE()
#define LPUART_TX_GPIO_CLK_DISABLE()          __HAL_RCC_GPIOB_CLK_DISABLE()

#define LPUART_RX_AF                          GPIO_AF8_LPUART1
#define LPUART_RX_GPIO_PORT                   GPIOA
#define LPUART_RX_PIN                         GPIO_PIN_3
#define LPUART_RX_GPIO_CLK_ENABLE()           __HAL_RCC_GPIOA_CLK_ENABLE()
#define LPUART_RX_GPIO_CLK_DISABLE()          __HAL_RCC_GPIOA_CLK_DISABLE()
#endif

#ifdef STM32WB55xx
#define BUTTON_SW1_EXTI_IRQHandler              EXTI4_IRQHandler
#define BUTTON_SW2_EXTI_IRQHandler              EXTI0_IRQHandler
#define BUTTON_SW3_EXTI_IRQHandler              EXTI1_IRQHandler

/**
  * @brief  USART pins
  */
#define USART_CLK_ENABLE()                    __HAL_RCC_USART1_CLK_ENABLE()

#define USART_TX_AF                           GPIO_AF7_USART1
#define USART_TX_GPIO_PORT                    GPIOB
#define USART_TX_PIN                          GPIO_PIN_6
#define USART_TX_GPIO_CLK_ENABLE()            __HAL_RCC_GPIOB_CLK_ENABLE()
#define USART_TX_GPIO_CLK_DISABLE()           __HAL_RCC_GPIOB_CLK_DISABLE()

#define USART_RX_AF                           GPIO_AF7_USART1
#define USART_RX_GPIO_PORT                    GPIOB
#define USART_RX_PIN                          GPIO_PIN_7
#define USART_RX_GPIO_CLK_ENABLE()            __HAL_RCC_GPIOB_CLK_ENABLE()
#define USART_RX_GPIO_CLK_DISABLE()           __HAL_RCC_GPIOB_CLK_DISABLE()

/**
  * @brief  LPUART pins
  */
#define LPUART_CLK_ENABLE()                    __HAL_RCC_LPUART1_CLK_ENABLE()

#define LPUART_TX_AF                          GPIO_AF8_LPUART1
#define LPUART_TX_GPIO_PORT                   GPIOA
#define LPUART_TX_PIN                         GPIO_PIN_2
#define LPUART_TX_GPIO_CLK_ENABLE()           __HAL_RCC_GPIOA_CLK_ENABLE()
#define LPUART_TX_GPIO_CLK_DISABLE()          __HAL_RCC_GPIOA_CLK_DISABLE()

#define LPUART_RX_AF                          GPIO_AF8_LPUART1
#define LPUART_RX_GPIO_PORT                   GPIOA
#define LPUART_RX_PIN                         GPIO_PIN_3
#define LPUART_RX_GPIO_CLK_ENABLE()           __HAL_RCC_GPIOA_CLK_ENABLE()
#define LPUART_RX_GPIO_CLK_DISABLE()          __HAL_RCC_GPIOA_CLK_DISABLE()
#endif

#endif /* GPIO_LLD_H_ */
