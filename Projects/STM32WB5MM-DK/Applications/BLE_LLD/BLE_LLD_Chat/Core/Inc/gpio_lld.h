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

#endif /* GPIO_LLD_H_ */
