/*
 * delay.c
 *
 *  Created on: 2017Äê8ÔÂ13ÈÕ
 *      Author: lucius
 */

#include "tick.h"
#include "timer.h"
#include "arm_gic.h"
#include "uart.h"
#include "tieoff.h"

#define CNTCLK_ENABLE (1 << 13)

void timer4_handler(void)
{
	timer_interrupt_clear(TIMER_INDEX_4);
}

void tick_init(void)
{
	struct s5p6818_tieoff_reg *tie_off = TIEOFF;

	tie_off->con[91] &= ~CNTCLK_ENABLE;
	for(volatile uint32_t i = 0; i < 1000; i++);
	tie_off->con[91] |= CNTCLK_ENABLE;
	for(volatile uint32_t i = 0; i < 1000; i++);
	tie_off->con[91] &= ~CNTCLK_ENABLE;
	for(volatile uint32_t i = 0; i < 1000; i++);
	tie_off->con[91] |= CNTCLK_ENABLE;

	timer_set_count_buffer(TIMER_INDEX_4,
			timer_helper_freq_to_count(TIMER_INDEX_4, 1000));
	timer_autoreload_enable(TIMER_INDEX_4);
	uart_printf("timer4 freq = %u\n", timer_helper_get_freq(TIMER_INDEX_4));
	timer_interrupt_enable(TIMER_INDEX_4);

	request_irq(ARM_GIC_IRQ_INDEX_TIMER4, timer4_handler);

	timer_start(TIMER_INDEX_4);
}
