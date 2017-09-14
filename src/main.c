/*
 ============================================================================
 Name        : main.c
 Author      : Lucius
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C
 ============================================================================
 */

#include <stdio.h>
#include <stdint.h>
#include <stdarg.h>
#include "arm.h"

#include "gpio.h"
#include "rstcon.h"
#include "pll_clk.h"
#include "tieoff.h"
#include "uart.h"
#include "rtc.h"
#include "timer.h"
#include "arm_gic.h"
#include "tick.h"
#include "pll_clk.h"
#include "subCpuBoot.h"
#include "bus.h"
#include "mmu.h"

/*
 *
 * Print a greeting message on standard output and exit.
 *
 * On embedded platforms this might require semi-hosting or similar.
 *
 * For example, for toolchains derived from GNU Tools for Embedded,
 * to enable semi-hosting, the following was added to the linker:
 *
 * --specs=rdimon.specs -Wl,--start-group -lgcc -lc -lc -lm -lrdimon -Wl,--end-group
 *
 * Adjust it for other toolchains.
 *
 */

void led_init(void)
{
  /*
   * LED1 -> S5P6818_GPIOB(26)
   * LED2 -> S5P6818_GPIOC(11)
   * LED3 -> S5P6818_GPIOC(7)
   * LED4 -> S5P6818_GPIOC(12)
   */
  gpio_set_cfg(GPIOB, 26, 1);
  gpio_set_cfg(GPIOC, 11, 1);
  gpio_set_output(GPIOB, 26);
  gpio_set_output(GPIOC, 11);
  gpio_output(GPIOB, 26, 0);
  gpio_output(GPIOC, 11, 0);
}

void printClkInfo(void)
{
  uart_printf(" PLL0: %d   PLL1: %d   PLL2: %d   PLL3: %d\r\n\r\n",
          pll_clk_pll_get_freq(PLL_INDEX_0), pll_clk_pll_get_freq(PLL_INDEX_1),
          pll_clk_pll_get_freq(PLL_INDEX_2), pll_clk_pll_get_freq(PLL_INDEX_3));

  uart_printf(" Divider0 PLL: %d CPU Group0:%d   CPU BUS:%d\r\n",
          pll_clk_sys_get_src(0), pll_clk_sys_get_bclk_freq(0),
          pll_clk_sys_get_pclk_freq(0));

  uart_printf(" Divider7 PLL: %d CPU Group1:%d   CPU BUS:%d\r\n",
          pll_clk_sys_get_src(7), pll_clk_sys_get_bclk_freq(7),
          pll_clk_sys_get_pclk_freq(7));

  uart_printf(" Divider8 PLL: %d FAST BUS BCLK:%d   PCLK:%d\r\n",
          pll_clk_sys_get_src(8), pll_clk_sys_get_bclk_freq(8),
          pll_clk_sys_get_pclk_freq(8));

  uart_printf(" Divider1 PLL: %d BCLK:%d   PCLK:%d\r\n", pll_clk_sys_get_src(1),
          pll_clk_sys_get_bclk_freq(1), pll_clk_sys_get_pclk_freq(1));

  uart_printf(" Divider5 PLL: %d DISPLAY BCLK:%d   PCLK:%d\r\n",
          pll_clk_sys_get_src(5), pll_clk_sys_get_bclk_freq(5),
          pll_clk_sys_get_pclk_freq(5));

//	SYSMSG(" Divider2 PLL: %d MDCLK:%d   MCLK:%d   \r\n\t\t MBCLK:%d   "
//	       "MPCLK:%d\r\n",
//	       NX_CLKPWR_GetSrcPll(2),
//	       getquotient(NX_CLKPWR_GetPLLFreq(NX_CLKPWR_GetSrcPll(2)),
//			   ((NX_CLKPWR_GetDivideValue(2) >> 0) & 0x3F)),
//	       getquotient(
//		   getquotient(NX_CLKPWR_GetPLLFreq(NX_CLKPWR_GetSrcPll(2)),
//			       ((NX_CLKPWR_GetDivideValue(2) >> 0) & 0x3F)),
//		   ((NX_CLKPWR_GetDivideValue(2) >> 8) & 0x3F)),
//	       getquotient(
//		   getquotient(
//		       getquotient(NX_CLKPWR_GetPLLFreq(NX_CLKPWR_GetSrcPll(2)),
//				   ((NX_CLKPWR_GetDivideValue(2) >> 0) & 0x3F)),
//		       ((NX_CLKPWR_GetDivideValue(2) >> 8) & 0x3F)),
//		   ((NX_CLKPWR_GetDivideValue(2) >> 16) & 0x3F)),
//	       getquotient(
//		   getquotient(
//		       getquotient(
//			   getquotient(
//			       NX_CLKPWR_GetPLLFreq(NX_CLKPWR_GetSrcPll(2)),
//			       ((NX_CLKPWR_GetDivideValue(2) >> 0) & 0x3F)),
//			   ((NX_CLKPWR_GetDivideValue(2) >> 8) & 0x3F)),
//		       ((NX_CLKPWR_GetDivideValue(2) >> 16) & 0x3F)),
//		   ((NX_CLKPWR_GetDivideValue(2) >> 24) & 0x3F)));

//	SYSMSG(" Divider3 PLL: %d G3D BCLK:%d\r\n", NX_CLKPWR_GetSrcPll(3),
//	       getquotient(NX_CLKPWR_GetPLLFreq(NX_CLKPWR_GetSrcPll(3)),
//			   ((NX_CLKPWR_GetDivideValue(3) >> 0) & 0x3F)));

  uart_printf(" Divider4 PLL: %d MPEG BCLK:%d   MPEG PCLK:%d\r\n",
          pll_clk_sys_get_src(4), pll_clk_sys_get_bclk_freq(4),
          pll_clk_sys_get_pclk_freq(4));

//	SYSMSG(" Divider6 PLL: %d HDMI PCLK:%d\r\n", NX_CLKPWR_GetSrcPll(6),
//	       getquotient(NX_CLKPWR_GetPLLFreq(NX_CLKPWR_GetSrcPll(6)),
//			   ((NX_CLKPWR_GetDivideValue(6) >> 0) & 0x3F)));
}

void timer0_handler(void)
{
//	register uint32_t tmp0 = 0, tmp1=0, tmp2=0,tmp3=0,tmp4=0,tmp5=0,tmp6=0,tmp7=0,tmp8=0,tmp9=0;
//
//	tmp0 = tmp1 + tmp2 + tmp3 + tmp4 + tmp5 + tmp6 + tmp7 + tmp8 + tmp9;
  timer_interrupt_clear(TIMER_INDEX_0);
  uart_printf("timer0\n");
}

void printf_lr(uint32_t type, uint32_t tmp)
{
  uart_printf("type=%u, lr=%X, val=%X\n", type, tmp, *(uint32_t *) tmp);
}

int boot_main(uint32_t cpu_id, uint32_t current_stack)
{
  uart_init();
  uart_printf("uart init success\n");
  uart_printf("cpu_id = %u, stack_pointer=%X\n", cpu_id, current_stack);

  printClkInfo();
  uart_printf("prepare to change pll 0\n");
  while (uart_is_busy())
    ;

  pll_clk_pll_set_freq(PLL_INDEX_0, PLL01_FREQ_800MHZ);
  pll_clk_pll_set_freq(PLL_INDEX_1, PLL01_FREQ_800MHZ);
  pll_clk_pll_set_freq(PLL_INDEX_2, PLL23_FREQ_800MHZ);
  pll_clk_pll_set_freq(PLL_INDEX_3, PLL23_FREQ_800MHZ);
  pll_clk_sys_set_bclk_freq(PLL_CLK_SYS_CPU0, 800);
  pll_clk_sys_set_pclk_freq(PLL_CLK_SYS_CPU0, 200);
  uart_init();
  uart_printf("pll 0 change done, uart re init success\n");
  while (uart_is_busy())
    ;
  arm_gic_generic_init();
  arm_gic_percpu_init(cpu_id);

  timer_module_enable();

  tick_init();
//  subCpuBoot();
  init_bus();
  mmu_init();

  {
    /* memery test */
#define MEM_TEST_VALUE 0x55555555
    *(volatile uint32_t *)0xFFFFFFFC = MEM_TEST_VALUE;
    uart_printf("mem %X\n", *(volatile uint32_t *)0xFFFFFFFC);
  }
  while (uart_is_busy())
    ;
  {
    static uint32_t lock = 0;
    uint32_t tmp;

    __clear_cache(&lock, &tmp);
    tmp = __sync_bool_compare_and_swap(&lock, 0, 1);
    uart_printf("tmp = %u\n", tmp);
  }
  while (1)
  {

  }
  return 0;
}

int sub_cpu_main(uint32_t cpu_id, uint32_t current_stack)
{
  uart_printf("cpu_id = %u, stack_pointer=%X\n", cpu_id, current_stack);
  while(1)
  {

  }
  return 0;
}
