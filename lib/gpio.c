/*
 * gpio.c
 *
 *  Created on: 2017Äê8ÔÂ7ÈÕ
 *      Author: lucius
 */

#include "gpio.h"

void gpio_set_cfg(struct s5p6818_gpio_reg * gpio, uint32_t pin_no, uint32_t cfg) {
	uint32_t offset;
	uint32_t temp;
	if (cfg >= 4) {
		return;
	}
	if (pin_no < 16) {
		temp = gpio->altfun0;
		offset = pin_no << 0x01;
		temp &= (~(0x3 << offset));
		temp |= (cfg << offset);
		gpio->altfun0 = temp;
	} else if (pin_no < 32) {
		temp = gpio->altfun1;
		offset = (pin_no - 16) << 0x01;
		temp &= (~(0x3 << offset));
		temp |= (cfg << offset);
		gpio->altfun1 = temp;
	}
}

void gpio_set_output(struct s5p6818_gpio_reg * gpio, uint32_t pin_no) {
	uint32_t val;
	if (pin_no >= 32) {
		return;
	}
	val = gpio->outenb;
	val |= (1 << pin_no);
	gpio->outenb = val;
}

void gpio_set_input(struct s5p6818_gpio_reg * gpio, uint32_t pin_no) {
	uint32_t val;
	if (pin_no >= 32) {
		return;
	}
	val = gpio->outenb;
	val &= (~(1 << pin_no));
	gpio->outenb = val;
}

void gpio_set_pull(struct s5p6818_gpio_reg * gpio, uint32_t pin_no,
		uint32_t pull) {
	(void) gpio;
	(void) pin_no;
	(void) pull;
}

void gpio_output(struct s5p6818_gpio_reg * gpio, uint32_t pin_no,
		uint32_t state) {
	if (state) {
		gpio->out |= (1 << pin_no);
	} else {
		gpio->out &= (~(1 << pin_no));
	}
}
