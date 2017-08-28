/*
 * clk_gen.c
 *
 *  Created on: 2017Äê8ÔÂ10ÈÕ
 *      Author: lucius
 */

#include "clk_gen.h"
#include "pll_clk.h"

#define CLK_GEN_ENABLE (1u)
#define CLK_GEN_DISABLE (0u)

typedef enum {
	CLK_GEN_LEVEL_0,
	CLK_GEN_LEVEL_1,
	CLK_GEN_LEVEL_2,
} clk_gen_level_t;

typedef struct {
	char * name;
	clk_gen_level_t level;
	uint32_t base_addr;
} pll_gen_predef_t;

#define _CLK_GEN_PREDEF_ARRAY(_module, _level, _base, _offset) \
		[CLK_GEN_ ## _module] = { \
				.name = #_module , \
				.level = CLK_GEN_LEVEL_ ## _level, \
				.base_addr = (_base) + (_offset)\
		},

const pll_gen_predef_t pll_gen_predef[] = {
		_CLK_GEN_DEFINE(_CLK_GEN_PREDEF_ARRAY)
};

typedef union {
	struct {
		volatile uint32_t reserved0 : 2;
		volatile uint32_t clkenb : 1;
		volatile uint32_t reserved1 : 29;
	} reg_field;
	volatile uint32_t reg_value;
} clkenb_t;

typedef union {
	struct {
		volatile uint32_t reserved0 : 1;
		volatile uint32_t outclkinv0 : 1;
		volatile uint32_t clksrcsel0 : 3;
		volatile uint32_t clkdiv0	 : 8;
		volatile uint32_t reserved1	 : 2;
		volatile uint32_t outclkenb	 : 1;
		volatile uint32_t reserved2	 : 16;
	} reg_field;
	volatile uint32_t reg_value;
} clkgen0l_t;

typedef union {
	struct {
		volatile uint32_t outclksel : 1;
		volatile uint32_t outclkinv1 : 1;
		volatile uint32_t clksrcsel1 : 3;
		volatile uint32_t clkdiv1	 : 8;
		volatile uint32_t reserved0	 : 19;
	} reg_field;
	volatile uint32_t reg_value;
} clkgen1l_t;

typedef struct {
	volatile clkenb_t clkenb;
	volatile clkgen0l_t clkgen0l;
	volatile uint32_t reserved[1];
	volatile clkgen1l_t clkgen1l;
} clk_gen_register_t;

void clk_gen_enable(clk_gen_index_t index)
{
	clk_gen_register_t *cur_pll_gen = (clk_gen_register_t *)pll_gen_predef[index].base_addr;
	cur_pll_gen->clkenb.reg_field.clkenb = CLK_GEN_ENABLE;
}

void clk_gen_disable(clk_gen_index_t index)
{
	clk_gen_register_t *cur_pll_gen = (clk_gen_register_t *)pll_gen_predef[index].base_addr;
	cur_pll_gen->clkenb.reg_field.clkenb = CLK_GEN_DISABLE;
}

uint32_t clk_gen_isEnable(clk_gen_index_t index)
{
	clk_gen_register_t *cur_pll_gen = (clk_gen_register_t *)pll_gen_predef[index].base_addr;
	return cur_pll_gen->clkenb.reg_field.clkenb;
}

void clk_gen_set_src(clk_gen_index_t index, clk_gen_src_t src_index)
{
	const pll_gen_predef_t *predef_info = &pll_gen_predef[index];
	clk_gen_register_t *cur_pll_gen;

	switch(predef_info->level)
	{
	case CLK_GEN_LEVEL_0:
	case CLK_GEN_LEVEL_1:
		cur_pll_gen = (clk_gen_register_t *)predef_info->base_addr;
		cur_pll_gen->clkgen0l.reg_field.clksrcsel0 = src_index;
		break;
	case CLK_GEN_LEVEL_2:
		break;
	}
}

uint32_t clk_gen_get_freq(clk_gen_index_t index)
{
	const pll_gen_predef_t *predef_info = &pll_gen_predef[index];
	clk_gen_register_t *cur_pll_gen;
	uint32_t freq = 0;

	switch(predef_info->level)
	{
	case CLK_GEN_LEVEL_0:
	case CLK_GEN_LEVEL_1:
		cur_pll_gen = (clk_gen_register_t *)predef_info->base_addr;
		if(clk_gen_isEnable(index) == CLK_GEN_ENABLE)
		{
			freq = pll_clk_pll_get_freq((pll_index_t )cur_pll_gen->clkgen0l.reg_field.clksrcsel0);
			freq = freq/(cur_pll_gen->clkgen0l.reg_field.clkdiv0 + 1);
		}
		break;
	case CLK_GEN_LEVEL_2:
		break;
	}

	return freq;
}
