/*
 * clk_gen.h
 *
 *  Created on: 2017Äê8ÔÂ10ÈÕ
 *      Author: lucius
 */

#ifndef CLK_GEN_H_
#define CLK_GEN_H_

#include <stdint.h>
#include "pll_clk.h"

#define UART_CLKGEN_BASE (0xC0000000)
#define UART_CLKGEN_ENB_UART0 (UART_CLKGEN_BASE + 0xA9000)
#define UART_CLKGEN_ENB_UART1 (UART_CLKGEN_BASE + 0xA8000)
#define UART_CLKGEN_ENB_UART2 (UART_CLKGEN_BASE + 0xAA000)
#define UART_CLKGEN_ENB_UART3 (UART_CLKGEN_BASE + 0xAB000)
#define UART_CLKGEN_ENB_UART4 (UART_CLKGEN_BASE + 0x6E000)
#define UART_CLKGEN_ENB_UART5 (UART_CLKGEN_BASE + 0x84000)

#define UART_CLKGEN_0L_UART0 (UART_CLKGEN_ENB_UART0 + 0x4)
#define UART_CLKGEN_0L_UART1 (UART_CLKGEN_ENB_UART1 + 0x4)
#define UART_CLKGEN_0L_UART2 (UART_CLKGEN_ENB_UART2 + 0x4)
#define UART_CLKGEN_0L_UART3 (UART_CLKGEN_ENB_UART3 + 0x4)
#define UART_CLKGEN_0L_UART4 (UART_CLKGEN_ENB_UART4 + 0x4)
#define UART_CLKGEN_0L_UART5 (UART_CLKGEN_ENB_UART5 + 0x4)

#define _CLK_GEN_DEFINE(_XX) \
			_XX(UART0, 1, 0xC0000000, 0xA9000) \
			_XX(TIMER0, 1, 0xC00B0000, 0x9000) \
			_XX(TIMER1, 1, 0xC00B0000, 0xB000) \
			_XX(TIMER2, 1, 0xC00B0000, 0xC000) \
			_XX(TIMER3, 1, 0xC00B0000, 0xD000)

#define _CLK_GEN_GEN_ENUM(_module, _level, _base, _offset) CLK_GEN_ ## _module,

typedef enum {
	_CLK_GEN_DEFINE(_CLK_GEN_GEN_ENUM)
	CLK_GEN_MAX
} clk_gen_index_t;

#undef _CLK_GEN_GEN_ENUM

typedef enum {
	CLK_GEN_SRC_PLL0 = PLL_INDEX_0,
	CLK_GEN_SRC_PLL1 = PLL_INDEX_1,
	CLK_GEN_SRC_PLL2 = PLL_INDEX_2,
	CLK_GEN_SRC_PLL3 = PLL_INDEX_3
} clk_gen_src_t;

void clk_gen_enable(clk_gen_index_t index);
void clk_gen_disable(clk_gen_index_t index);

void clk_gen_set_src(clk_gen_index_t index, clk_gen_src_t src_index);

uint32_t clk_gen_get_freq(clk_gen_index_t index);

#endif /* CLK_GEN_H_ */
