/*
 * timer.c
 *
 *  Created on: 2017Äê8ÔÂ10ÈÕ
 *      Author: lucius
 */

#include "timer.h"
#include "rstcon.h"
#include "clk_gen.h"

#define TIMER_ADDR_BASE (0xC0017000)

#define TIMER_START (1u)
#define TIMER_STOP (0u)

#define TIMER_MANUAL_UPDATE_ENABLE (1u)
#define TIMER_MANUAL_UPDATE_DISABLE (0u)

#define TIMER_AUTORELOAD_ENABLE (1u)
#define TIMER_AUTORELOAD_DISABLE (0u)

#define TIMER_INTERRUPT_ENABLE (1u)
#define TIMER_INTERRUPT_DISABLE (0u)

void timer_module_enable(void)
{
	rstcon_set_rst(RSTCON_TIMER, RSTCON_ASSERT);
	rstcon_set_rst(RSTCON_TIMER, RSTCON_NEGATE);

	clk_gen_enable(CLK_GEN_TIMER0);
	clk_gen_enable(CLK_GEN_TIMER1);
	clk_gen_enable(CLK_GEN_TIMER2);
	clk_gen_enable(CLK_GEN_TIMER3);

	clk_gen_set_src(CLK_GEN_TIMER0, CLK_GEN_SRC_PLL3);
	clk_gen_set_src(CLK_GEN_TIMER1, CLK_GEN_SRC_PLL3);
	clk_gen_set_src(CLK_GEN_TIMER2, CLK_GEN_SRC_PLL3);
	clk_gen_set_src(CLK_GEN_TIMER3, CLK_GEN_SRC_PLL3);
}

void timer_start(timer_index_t index)
{
	timer_register_t *reg_set = (timer_register_t *)TIMER_ADDR_BASE;

	switch(index)
	{
	case TIMER_INDEX_0:
		reg_set->tcon.reg_field.timer0_start = TIMER_START;
		break;
	case TIMER_INDEX_1:
		reg_set->tcon.reg_field.timer1_start = TIMER_START;
		break;
	case TIMER_INDEX_2:
		reg_set->tcon.reg_field.timer2_start = TIMER_START;
		break;
	case TIMER_INDEX_3:
		reg_set->tcon.reg_field.timer3_start = TIMER_START;
		break;
	case TIMER_INDEX_4:
		reg_set->tcon.reg_field.timer4_start = TIMER_START;
		break;
	}
}

void timer_stop(timer_index_t index)
{
	timer_register_t *reg_set = (timer_register_t *)TIMER_ADDR_BASE;

	switch(index)
	{
	case TIMER_INDEX_0:
		reg_set->tcon.reg_field.timer0_start = TIMER_STOP;
		break;
	case TIMER_INDEX_1:
		reg_set->tcon.reg_field.timer1_start = TIMER_STOP;
		break;
	case TIMER_INDEX_2:
		reg_set->tcon.reg_field.timer2_start = TIMER_STOP;
		break;
	case TIMER_INDEX_3:
		reg_set->tcon.reg_field.timer3_start = TIMER_STOP;
		break;
	case TIMER_INDEX_4:
		reg_set->tcon.reg_field.timer4_start = TIMER_STOP;
		break;
	}
}

void timer_autoreload_enable(timer_index_t index)
{
	timer_register_t *reg_set = (timer_register_t *)TIMER_ADDR_BASE;

	switch(index)
	{
	case TIMER_INDEX_0:
		reg_set->tcon.reg_field.timer0_auto_reload = TIMER_AUTORELOAD_ENABLE;
		break;
	case TIMER_INDEX_1:
		reg_set->tcon.reg_field.timer1_auto_reload = TIMER_AUTORELOAD_ENABLE;
		break;
	case TIMER_INDEX_2:
		reg_set->tcon.reg_field.timer2_auto_reload = TIMER_AUTORELOAD_ENABLE;
		break;
	case TIMER_INDEX_3:
		reg_set->tcon.reg_field.timer3_auto_reload = TIMER_AUTORELOAD_ENABLE;
		break;
	case TIMER_INDEX_4:
		reg_set->tcon.reg_field.timer4_auto_reload = TIMER_AUTORELOAD_ENABLE;
		break;
	}
}

void timer_autoreload_disable(timer_index_t index)
{
	timer_register_t *reg_set = (timer_register_t *)TIMER_ADDR_BASE;

	switch(index)
	{
	case TIMER_INDEX_0:
		reg_set->tcon.reg_field.timer0_auto_reload = TIMER_AUTORELOAD_DISABLE;
		break;
	case TIMER_INDEX_1:
		reg_set->tcon.reg_field.timer1_auto_reload = TIMER_AUTORELOAD_DISABLE;
		break;
	case TIMER_INDEX_2:
		reg_set->tcon.reg_field.timer2_auto_reload = TIMER_AUTORELOAD_DISABLE;
		break;
	case TIMER_INDEX_3:
		reg_set->tcon.reg_field.timer3_auto_reload = TIMER_AUTORELOAD_DISABLE;
		break;
	case TIMER_INDEX_4:
		reg_set->tcon.reg_field.timer4_auto_reload = TIMER_AUTORELOAD_DISABLE;
		break;
	}
}

void timer_interrupt_enable(timer_index_t index)
{
	timer_register_t *reg_set = (timer_register_t *)TIMER_ADDR_BASE;

	switch(index)
	{
	case TIMER_INDEX_0:
		reg_set->tint_cstat.reg_field.timer0_irq_enable = TIMER_INTERRUPT_ENABLE;
		break;
	case TIMER_INDEX_1:
		reg_set->tint_cstat.reg_field.timer1_irq_enable = TIMER_INTERRUPT_ENABLE;
		break;
	case TIMER_INDEX_2:
		reg_set->tint_cstat.reg_field.timer2_irq_enable = TIMER_INTERRUPT_ENABLE;
		break;
	case TIMER_INDEX_3:
		reg_set->tint_cstat.reg_field.timer3_irq_enable = TIMER_INTERRUPT_ENABLE;
		break;
	case TIMER_INDEX_4:
		reg_set->tint_cstat.reg_field.timer4_irq_enable = TIMER_INTERRUPT_ENABLE;
		break;
	}
}

void timer_interrupt_disable(timer_index_t index)
{
	timer_register_t *reg_set = (timer_register_t *)TIMER_ADDR_BASE;

	switch(index)
	{
	case TIMER_INDEX_0:
		reg_set->tint_cstat.reg_field.timer0_irq_enable = TIMER_INTERRUPT_DISABLE;
		break;
	case TIMER_INDEX_1:
		reg_set->tint_cstat.reg_field.timer1_irq_enable = TIMER_INTERRUPT_DISABLE;
		break;
	case TIMER_INDEX_2:
		reg_set->tint_cstat.reg_field.timer2_irq_enable = TIMER_INTERRUPT_DISABLE;
		break;
	case TIMER_INDEX_3:
		reg_set->tint_cstat.reg_field.timer3_irq_enable = TIMER_INTERRUPT_DISABLE;
		break;
	case TIMER_INDEX_4:
		reg_set->tint_cstat.reg_field.timer4_irq_enable = TIMER_INTERRUPT_DISABLE;
		break;
	}
}

void timer_interrupt_clear(timer_index_t index)
{
	timer_register_t *reg_set = (timer_register_t *)TIMER_ADDR_BASE;

	switch(index)
	{
	case TIMER_INDEX_0:
		reg_set->tint_cstat.reg_field.timer0_irq_status = 1;
		break;
	case TIMER_INDEX_1:
		reg_set->tint_cstat.reg_field.timer1_irq_status = 1;
		break;
	case TIMER_INDEX_2:
		reg_set->tint_cstat.reg_field.timer2_irq_status = 1;
		break;
	case TIMER_INDEX_3:
		reg_set->tint_cstat.reg_field.timer3_irq_status = 1;
		break;
	case TIMER_INDEX_4:
		reg_set->tint_cstat.reg_field.timer4_irq_status = 1;
		break;
	}
}

uint32_t timer_interrupt_get(timer_index_t index)
{
	timer_register_t *reg_set = (timer_register_t *)TIMER_ADDR_BASE;
	uint32_t status = 0;

	switch(index)
	{
	case TIMER_INDEX_0:
		status = reg_set->tint_cstat.reg_field.timer0_irq_status;
		break;
	case TIMER_INDEX_1:
		status = reg_set->tint_cstat.reg_field.timer1_irq_status;
		break;
	case TIMER_INDEX_2:
		status = reg_set->tint_cstat.reg_field.timer2_irq_status;
		break;
	case TIMER_INDEX_3:
		status = reg_set->tint_cstat.reg_field.timer3_irq_status;
		break;
	case TIMER_INDEX_4:
		status = reg_set->tint_cstat.reg_field.timer4_irq_status;
		break;
	}
	return status;
}

uint32_t timer_get_count(timer_index_t index)
{
	timer_register_t *reg_set = (timer_register_t *)TIMER_ADDR_BASE;
	uint32_t count = 0;

	switch(index)
	{
	case TIMER_INDEX_0:
		count = reg_set->tcnto0;
		break;
	case TIMER_INDEX_1:
		count = reg_set->tcnto1;
		break;
	case TIMER_INDEX_2:
		count = reg_set->tcnto2;
		break;
	case TIMER_INDEX_3:
		count = reg_set->tcnto3;
		break;
	case TIMER_INDEX_4:
		count = reg_set->tcnto4;
		break;
	}
	return count;
}

uint32_t timer_get_count_buffer(timer_index_t index)
{
	timer_register_t *reg_set = (timer_register_t *)TIMER_ADDR_BASE;
	uint32_t count = 0;

	switch(index)
	{
	case TIMER_INDEX_0:
		count = reg_set->tcntb0;
		break;
	case TIMER_INDEX_1:
		count = reg_set->tcntb1;
		break;
	case TIMER_INDEX_2:
		count = reg_set->tcntb2;
		break;
	case TIMER_INDEX_3:
		count = reg_set->tcntb3;
		break;
	case TIMER_INDEX_4:
		count = reg_set->tcntb4;
		break;
	}
	return count;
}

void timer_set_count_buffer(timer_index_t index, uint32_t count)
{
	timer_register_t *reg_set = (timer_register_t *)TIMER_ADDR_BASE;

	switch(index)
	{
	case TIMER_INDEX_0:
		reg_set->tcntb0 = count;
		reg_set->tcon.reg_field.timer0_manual_update = TIMER_MANUAL_UPDATE_ENABLE;
		reg_set->tcon.reg_field.timer0_manual_update = TIMER_MANUAL_UPDATE_DISABLE;
		break;
	case TIMER_INDEX_1:
		reg_set->tcntb1 = count;
		reg_set->tcon.reg_field.timer1_manual_update = TIMER_MANUAL_UPDATE_ENABLE;
		reg_set->tcon.reg_field.timer1_manual_update = TIMER_MANUAL_UPDATE_DISABLE;
		break;
	case TIMER_INDEX_2:
		reg_set->tcntb2 = count;
		reg_set->tcon.reg_field.timer2_manual_update = TIMER_MANUAL_UPDATE_ENABLE;
		reg_set->tcon.reg_field.timer2_manual_update = TIMER_MANUAL_UPDATE_DISABLE;
		break;
	case TIMER_INDEX_3:
		reg_set->tcntb3 = count;
		reg_set->tcon.reg_field.timer3_manual_update = TIMER_MANUAL_UPDATE_ENABLE;
		reg_set->tcon.reg_field.timer3_manual_update = TIMER_MANUAL_UPDATE_DISABLE;
		break;
	case TIMER_INDEX_4:
		reg_set->tcntb4 = count;
		reg_set->tcon.reg_field.timer4_manual_update = TIMER_MANUAL_UPDATE_ENABLE;
		reg_set->tcon.reg_field.timer4_manual_update = TIMER_MANUAL_UPDATE_DISABLE;
		break;
	}
}

uint32_t timer_get_input_freq(timer_index_t index)
{
	timer_register_t *reg_set = (timer_register_t *)TIMER_ADDR_BASE;
	uint32_t src_freq = pll_clk_sys_get_pclk_freq(PLL_CLK_SYS_SYS);

	switch(index)
	{
	case TIMER_INDEX_0:
		src_freq = src_freq/(reg_set->tcfg0.reg_field.prescale0 + 1);
		src_freq = src_freq >> (reg_set->tcfg1.reg_field.divider_mux0);
		break;
	case TIMER_INDEX_1:
		src_freq = src_freq/(reg_set->tcfg0.reg_field.prescale0 + 1);
		src_freq = src_freq >> (reg_set->tcfg1.reg_field.divider_mux1);
		break;
	case TIMER_INDEX_2:
		src_freq = src_freq/(reg_set->tcfg0.reg_field.prescale1 + 1);
		src_freq = src_freq >> (reg_set->tcfg1.reg_field.divider_mux2);
		break;
	case TIMER_INDEX_3:
		src_freq = src_freq/(reg_set->tcfg0.reg_field.prescale1 + 1);
		src_freq = src_freq >> (reg_set->tcfg1.reg_field.divider_mux3);
		break;
	case TIMER_INDEX_4:
		src_freq = src_freq/(reg_set->tcfg0.reg_field.prescale1 + 1);
		src_freq = src_freq >> (reg_set->tcfg1.reg_field.divider_mux3);
		break;
	default:
		src_freq = 0;
		break;
	}

	return src_freq;
}

uint32_t timer_helper_freq_to_count(timer_index_t index, uint32_t freq)
{
	return timer_get_input_freq(index)/freq;
}

uint32_t timer_helper_get_freq(timer_index_t index)
{
	timer_register_t *reg_set = (timer_register_t *)TIMER_ADDR_BASE;
	uint32_t src_freq = pll_clk_sys_get_pclk_freq(PLL_CLK_SYS_SYS);

	switch(index)
	{
	case TIMER_INDEX_0:
		src_freq = src_freq/(reg_set->tcfg0.reg_field.prescale0 + 1);
		src_freq = src_freq >> (reg_set->tcfg1.reg_field.divider_mux0);
		src_freq = src_freq/reg_set->tcntb0;
		break;
	case TIMER_INDEX_1:
		src_freq = src_freq/(reg_set->tcfg0.reg_field.prescale0 + 1);
		src_freq = src_freq >> (reg_set->tcfg1.reg_field.divider_mux1);
		src_freq = src_freq/reg_set->tcntb1;
		break;
	case TIMER_INDEX_2:
		src_freq = src_freq/(reg_set->tcfg0.reg_field.prescale1 + 1);
		src_freq = src_freq >> (reg_set->tcfg1.reg_field.divider_mux2);
		src_freq = src_freq/reg_set->tcntb2;
		break;
	case TIMER_INDEX_3:
		src_freq = src_freq/(reg_set->tcfg0.reg_field.prescale1 + 1);
		src_freq = src_freq >> (reg_set->tcfg1.reg_field.divider_mux3);
		src_freq = src_freq/reg_set->tcntb3;
		break;
	case TIMER_INDEX_4:
		src_freq = src_freq/(reg_set->tcfg0.reg_field.prescale1 + 1);
		src_freq = src_freq >> (reg_set->tcfg1.reg_field.divider_mux3);
		src_freq = src_freq/reg_set->tcntb4;
		break;
	default:
		src_freq = 0;
		break;
	}

	return src_freq;
}
