/*
 * gpio.h
 *
 *  Created on: 2017Äê8ÔÂ7ÈÕ
 *      Author: lucius
 */

#ifndef GPIO_H_
#define GPIO_H_

#include <stdint.h>

#define GPIO_BASE (0xC0010000)
#define GPIOx_BASE(x) (GPIO_BASE + 0xA000 +0x1000*(x))
#define GPIOA_BASE GPIOx_BASE(0)
#define GPIOB_BASE GPIOx_BASE(1)
#define GPIOC_BASE GPIOx_BASE(2)
#define GPIOD_BASE GPIOx_BASE(3)
#define GPIOE_BASE GPIOx_BASE(4)

struct s5p6818_gpio_reg {
	volatile uint32_t out;
	volatile uint32_t outenb;
	volatile uint32_t detmode0;
	volatile uint32_t detmode1;
	volatile uint32_t intenb;
	volatile uint32_t det;
	volatile uint32_t pad;
	volatile uint32_t reserved[1];
	volatile uint32_t altfun0;
	volatile uint32_t altfun1;
	volatile uint32_t detmodeex;
	volatile uint32_t detenb;
	volatile uint32_t slew;
	volatile uint32_t slew_disable_default;
	volatile uint32_t drv1;
	volatile uint32_t drv1_disable_default;
	volatile uint32_t drv0;
	volatile uint32_t drv0_disable_default;
	volatile uint32_t pullsel;
	volatile uint32_t pullsel_disable_default;
	volatile uint32_t pullenb;
	volatile uint32_t pullenb_disable_default;
};

#define GPIOA ((struct s5p6818_gpio_reg *)GPIOA_BASE)
#define GPIOB ((struct s5p6818_gpio_reg *)GPIOB_BASE)
#define GPIOC ((struct s5p6818_gpio_reg *)GPIOC_BASE)
#define GPIOD ((struct s5p6818_gpio_reg *)GPIOD_BASE)
#define GPIOE ((struct s5p6818_gpio_reg *)GPIOE_BASE)

void gpio_set_cfg(struct s5p6818_gpio_reg * gpio, uint32_t pin_no, uint32_t cfg);
void gpio_set_output(struct s5p6818_gpio_reg * gpio, uint32_t pin_no);
void gpio_set_input(struct s5p6818_gpio_reg * gpio, uint32_t pin_no);
void gpio_set_pull(struct s5p6818_gpio_reg * gpio, uint32_t pin_no,
		uint32_t pull);
void gpio_output(struct s5p6818_gpio_reg * gpio, uint32_t pin_no,
		uint32_t state);

#endif /* GPIO_H_ */
