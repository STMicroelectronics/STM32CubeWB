/*
 * gpio_lld.h
 *
 */

#ifndef GPIO_LLD_H_
#define GPIO_LLD_H_

#if defined (USE_SIMU) || defined (USE_FPGA)
/* Be careful with GPIO used on SIMU plateform */
// GPIOA port is used for CRC management on MASTER only
// GPIOC ad GPIOD ports are used to send messages between the 2 STM32WBx
#define GPIO_TX_PIN            GPIO_PIN_8
#define GPIO_TX_PORT           GPIOB

#define GPIO_1_PIN             GPIO_PIN_9
#define GPIO_1_PORT            GPIOB
#else /* on Nucleo boards */
/* Use GPIO PB.8 to monitor TX time during valid on Boards */
#define GPIO_TX_PIN            GPIO_PIN_8
#define GPIO_TX_PORT           GPIOB

#define GPIO_HARD_FAULT_PIN    GPIO_PIN_4
#define GPIO_HARD_FAULT_PORT   GPIOA

#define GPIO_MCO_PIN           GPIO_PIN_15
#define GPIO_MCO_PORT          GPIOA

#ifdef STM32WB35xx
#define GPIO_1_PIN             GPIO_PIN_3
#define GPIO_1_PORT            GPIOB

#define GPIO_2_PIN             GPIO_PIN_4
#define GPIO_2_PORT            GPIOB
#else
#define GPIO_1_PIN             GPIO_PIN_2
#define GPIO_1_PORT            GPIOC

#define GPIO_2_PIN             GPIO_PIN_3
#define GPIO_2_PORT            GPIOC
#endif
#endif

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
void gpio_lld_phy_extPaEn_up(void);
void gpio_lld_phy_extPaEn_down(void);

#ifdef USE_SIMU
void gpio_lld_SimuMaster_init(void);
void gpio_lld_SimuSlave_init(void);
#endif

void gpio_lld_lpuart_init(void);
void gpio_lld_lpuart_deInit(void);

void gpio_lld_usart_init(void);
void gpio_lld_usart_deInit(void);

#if !defined (USE_SIMU) && !defined(USE_FPGA)
void gpio_lld_pa2_init(uint8_t mode, uint8_t speed);
void gpio_lld_pa2_deInit(void);

void gpio_lld_dtb_init(uint8_t dtbMode);
void gpio_lld_dtb_deInit(void);
#endif

void gpio_lld_led1_toggle(void);
void gpio_lld_led2_toggle(void);
void gpio_lld_led3_toggle(void);

void gpio_lld_phy_dbg_init(void);
void gpio_lld_phy_pa0_up(void);
void gpio_lld_phy_pa0_down(void);
void gpio_lld_phy_pa1_up(void);
void gpio_lld_phy_pa1_down(void);
void gpio_lld_phy_pa5_up(void);
void gpio_lld_phy_pa5_down(void);
void gpio_lld_phy_pa7_up(void);
void gpio_lld_phy_pa7_down(void);
#ifndef STM32WB35xx
void gpio_lld_phy_pb12_up(void);
void gpio_lld_phy_pb12_down(void);
void gpio_lld_phy_pb13_up(void);
void gpio_lld_phy_pb13_down(void);
void gpio_lld_phy_pb14_up(void);
void gpio_lld_phy_pb14_down(void);
void gpio_lld_phy_pb15_up(void);
void gpio_lld_phy_pb15_down(void);
void gpio_lld_phy_pc5_up(void);
void gpio_lld_phy_pc5_down(void);
#endif
#endif /* GPIO_LLD_H_ */
