/*
 * pll_clk.c
 *
 *  Created on: 2017Äê7ÔÂ26ÈÕ
 *      Author: lucius
 */

#include "pll_clk.h"

#define PLL_CLK_BYPASS_ENABLE (0u)
#define PLL_CLK_BYPASS_DISABLE (1u)
#define PLL_CLK_UPDATE_SRC (1u)

#define _PLL_ARRAY_GEN(_out, _pll, _p, _m, _k) \
		[PLL ## _pll ## _FREQ_ ## _out ## MHZ] = { \
				.reg_field = {\
						.pdiv = (_p),\
						.mdiv = (_m),\
						.sdiv = (_k)\
				}\
		},

const pllSetReg_t pll01_pms_predef[] = {
_PLL_CLK_PLL_DEFINE(_PLL_ARRAY_GEN) };

typedef char sizecheck_pll_predef[
		(sizeof(pll01_pms_predef) / sizeof(pll01_pms_predef[0]) - PLL_FREQ_MAX) ?
				-1 : 1];

uint32_t pll_clk_pll_get_freq(pll_index_t index) {
	clkpwr_regset_t *reg_set = (clkpwr_regset_t *) PHY_BASEADDR_CLKPWR_MODULE;
	pllSetReg_t pllsetreg_val = reg_set->pllsetreg[index];
	uint32_t temp = 0;

	if (index > PLL_INDEX_1) {
		pllSetReg_sscg_t pllsetreg_sscg_val = reg_set->pllsetreg_sscg[index];
		temp = (uint32_t) (((((pllsetreg_sscg_val.reg_field.k * 1000) / 65536)
				* S5PV6818_OSC_INPUT) / pllsetreg_val.reg_field.pdiv)
				>> pllsetreg_val.reg_field.sdiv);
	}

	temp = (uint32_t) (((((pllsetreg_val.reg_field.mdiv * S5PV6818_OSC_INPUT)
			/ pllsetreg_val.reg_field.pdiv) >> pllsetreg_val.reg_field.sdiv)
			* 1000) + temp);

	return temp;
}

void pll_clk_pll_set_freq(pll_index_t index, pll_freq_t freq) {
	clkpwr_regset_t *reg_set = (clkpwr_regset_t *) PHY_BASEADDR_CLKPWR_MODULE;
	pllSetReg_t pllsetreg_val = reg_set->pllsetreg[index];

	pllsetreg_val.reg_field.pdiv = pll01_pms_predef[freq].reg_field.pdiv;
	pllsetreg_val.reg_field.mdiv = pll01_pms_predef[freq].reg_field.mdiv;
	pllsetreg_val.reg_field.sdiv = pll01_pms_predef[freq].reg_field.sdiv;

	reg_set->pllsetreg[index] = pllsetreg_val;

	reg_set->clkmodereg0.reg_value |= (1 << index);
	while (reg_set->clkmodereg0.reg_field.wait_update_pll == 1u)
		;

	if ((index == PLL_INDEX_0) || (index == PLL_INDEX_1)) {
		reg_set->pwrmode.reg_field.chgpll = 1;

		while (reg_set->pwrmode.reg_field.chgpll == 1)
			;
	}
}

pll_index_t pll_clk_sys_get_src(pll_clk_sys_index_t clk_index) {
	clkpwr_regset_t *reg_set = (clkpwr_regset_t *) PHY_BASEADDR_CLKPWR_MODULE;
	clkdivreg_t *clkdivreg = (clkdivreg_t *) &reg_set->DVOREG[clk_index];
	return (pll_index_t) clkdivreg->reg_field.clksel_bclk;
}

uint32_t pll_clk_sys_get_bclk_freq(pll_clk_sys_index_t index) {
	clkpwr_regset_t *reg_set = (clkpwr_regset_t *) PHY_BASEADDR_CLKPWR_MODULE;
	clkdivreg_t *clkdivreg = (clkdivreg_t *) &reg_set->DVOREG[index];
	uint32_t src_freq = pll_clk_pll_get_freq(
			(pll_index_t) clkdivreg->reg_field.clksel_bclk);

	return src_freq / (clkdivreg->reg_field.clkdiv_bclk + 1u);
}

uint32_t pll_clk_sys_get_pclk_freq(pll_clk_sys_index_t index) {
	clkpwr_regset_t *reg_set = (clkpwr_regset_t *) PHY_BASEADDR_CLKPWR_MODULE;
	clkdivreg_t *clkdivreg = (clkdivreg_t *) &reg_set->DVOREG[index];
	uint32_t src_freq = pll_clk_pll_get_freq(
			(pll_index_t) clkdivreg->reg_field.clksel_bclk);
	uint32_t bclk = src_freq / (clkdivreg->reg_field.clkdiv_bclk + 1u);

	return bclk / (clkdivreg->reg_field.clkdiv_pclk + 1u);
}

void pll_clk_sys_select_src(pll_clk_sys_index_t clk_index,
		pll_index_t src_index) {
	clkpwr_regset_t *reg_set = (clkpwr_regset_t *) PHY_BASEADDR_CLKPWR_MODULE;
	clkdivreg_t *clkdivreg = (clkdivreg_t *) &reg_set->DVOREG[clk_index];
	clkdivreg->reg_field.clksel_bclk = (uint32_t) src_index;
}

void pll_clk_sys_set_bclk_freq(pll_clk_sys_index_t index, uint32_t freq) {
	clkpwr_regset_t *reg_set = (clkpwr_regset_t *) PHY_BASEADDR_CLKPWR_MODULE;
	clkdivreg_t *clkdivreg = (clkdivreg_t *) &reg_set->DVOREG[index];
	uint32_t src_freq = pll_clk_pll_get_freq(
			(pll_index_t) clkdivreg->reg_field.clksel_bclk);

	clkdivreg->reg_field.clkdiv_bclk = src_freq / freq - 1u;
}

void pll_clk_sys_set_pclk_freq(pll_clk_sys_index_t index, uint32_t freq) {
	clkpwr_regset_t *reg_set = (clkpwr_regset_t *) PHY_BASEADDR_CLKPWR_MODULE;
	clkdivreg_t *clkdivreg = (clkdivreg_t *) &reg_set->DVOREG[index];
	uint32_t src_freq = pll_clk_pll_get_freq(
			(pll_index_t) clkdivreg->reg_field.clksel_bclk);
	uint32_t bclk = src_freq / (clkdivreg->reg_field.clkdiv_bclk + 1u);

	clkdivreg->reg_field.clkdiv_pclk = bclk / freq - 1u;
}
