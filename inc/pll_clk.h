/*
 * pll_clk.h
 *
 *  Created on: 2017Äê7ÔÂ26ÈÕ
 *      Author: lucius
 */

#ifndef PLL_CLK_H_
#define PLL_CLK_H_

#include "stdint.h"

#define S5PV6818_OSC_INPUT ((uint32_t)24*1000)
#define PHY_BASEADDR_CLKPWR_MODULE 0xC0010000

typedef enum {
	PLL_INDEX_0 = 0,
	PLL_INDEX_1 = 1,
	PLL_INDEX_2 = 2,
	PLL_INDEX_3 = 3
} pll_index_t;

typedef union {
	struct {
		uint32_t update_pll0 : 1;
		uint32_t update_pll1 : 1;
		uint32_t update_pll2 : 1;
		uint32_t update_pll3 : 1;
		uint32_t reserved : 27;
		uint32_t wait_update_pll : 1;
	} reg_field;
	uint32_t reg_value;
} clkmodereg_t;

typedef union {
	struct {
		uint32_t sdiv : 8;
		uint32_t mdiv : 10;
		uint32_t pdiv : 6;
		uint32_t plloutdiv : 4;
		uint32_t npllbypass : 1;
		uint32_t pd : 1;
		uint32_t sscg_en : 1;
		uint32_t reserved : 1;
	} reg_field;
	uint32_t reg_value;
} pllSetReg_t;

typedef union {
	struct {
		uint32_t sel_pf : 2;
		uint32_t mrr : 6;
		uint32_t mfr : 8;
		int32_t k : 16;
	} reg_field;
	uint32_t reg_value;
} pllSetReg_sscg_t;

typedef union {
	struct {
		volatile uint32_t reserved : 12;
		volatile uint32_t swrst : 1;
		volatile uint32_t reserved1 : 2;
		volatile uint32_t chgpll : 1;
		volatile uint32_t reserved2 : 16;
	} reg_field;
	uint32_t reg_value;
} pwrmode_t;

typedef union {
	struct {
		volatile uint32_t clksel_bclk : 3;
		volatile uint32_t clkdiv_bclk : 6;
		volatile uint32_t clkdiv_pclk : 6;
		volatile uint32_t reserved0 : 17;
	} reg_field;
	uint32_t reg_value;
} clkdivreg_t;

/// @brief	Clock & Power Manager Module's Register List
// assign	PSEL[0]	= i_PSEL &amp; ( i_PADDR[10:9] == 0 ); //	CLKCTR :0
// assign	PSEL[1]	= i_PSEL &amp; ( i_PADDR[10:9] == 1 ); //	ALIVE  : 512
typedef struct
{
		volatile clkmodereg_t clkmodereg0;				///< 0x000 : Clock Mode Register 0
		volatile uint32_t __Reserved0;				///< 0x004
		volatile pllSetReg_t pllsetreg[4];		///< 0x008 ~ 0x014 : PLL Setting Register
		volatile uint32_t __Reserved1[2];			///< 0x018 ~ 0x01C
		volatile uint32_t DVOREG[9];					///< 0x020 ~ 0x040 : Divider Setting Register
		volatile uint32_t __Reserved2;				///< 0x044
		volatile pllSetReg_sscg_t pllsetreg_sscg[6];			///< 0x048 ~ 0x05C
		volatile uint32_t __reserved3[8];			///< 0x060 ~ 0x07C
		volatile uint8_t __Reserved4[0x200-0x80];	// padding (0x80 ~ 0x1FF)
		volatile uint32_t GPIOWAKEUPRISEENB;			///< 0x200 : GPIO Rising Edge Detect Enable Register
		volatile uint32_t GPIOWAKEUPFALLENB;			///< 0x204 : GPIO Falling Edge Detect Enable Register
		volatile uint32_t GPIORSTENB;				///< 0x208 : GPIO Reset Enable Register
		volatile uint32_t GPIOWAKEUPENB;				///< 0x20C : GPIO Wakeup Source Enable
		volatile uint32_t GPIOINTENB;				///< 0x210 : Interrupt Enable Register
		volatile uint32_t GPIOINTPEND;				///< 0x214 : Interrupt Pend Register
		volatile uint32_t RESETSTATUS;				///< 0x218 : Reset Status Register
		volatile uint32_t INTENABLE;				///< 0x21C : Interrupt Enable Register
		volatile uint32_t INTPEND;					///< 0x220 : Interrupt Pend Register
		volatile uint32_t PWRCONT;					///< 0x224 : Power Control Register
		volatile pwrmode_t pwrmode;					///< 0x228 : Power Mode Register
		volatile uint32_t __Reserved5;				///< 0x22C : Reserved Region
		volatile uint32_t SCRATCH[3];				///< 0x230 ~ 0x238	: Scratch Register
		volatile uint32_t SYSRSTCONFIG;				///< 0x23C : System Reset Configuration Register.
		volatile uint8_t  __Reserved6[0x2A0-0x240];	// padding (0x240 ~ 0x29F)
		volatile uint32_t CPUPOWERDOWNREQ;			///< 0x2A0 : CPU Power Down Request Register
		volatile uint32_t CPUPOWERONREQ;			///< 0x2A4 : CPU Power On Request Register
		volatile uint32_t CPURESETMODE;				///< 0x2A8 : CPU Reset Mode Register
		volatile uint32_t CPUWARMRESETREQ;			///< 0x2AC : CPU Warm Reset Request Register
		volatile uint32_t __Reserved7;				///< 0x2B0
		volatile uint32_t CPUSTATUS;					///< 0x2B4 : CPU Status Register
		volatile uint8_t  __Reserved8[0x400-0x2B8];	// padding (0x2B8 ~ 0x33F)
} clkpwr_regset_t;

#define _PLL_CLK_PLL_DEFINE(_XX) \
				_XX(2000, 01, 3, 250, 0) \
				_XX(1900, 01, 6, 475, 0) \
				_XX(1800, 01, 3, 255, 0) \
				_XX(1700, 01, 6, 425, 0) \
				_XX(1600, 01, 3, 200, 0) \
				_XX(1500, 01, 4, 250, 0) \
				_XX(1400, 01, 3, 175, 0) \
				_XX(1300, 01, 6, 325, 0) \
				_XX(1200, 01, 3, 300, 1) \
				_XX(1100, 01, 3, 275, 1) \
				_XX(1000, 01, 3, 250, 1) \
				_XX( 900, 01, 3, 225, 1) \
				_XX( 800, 01, 3, 200, 1) \
				_XX( 780, 01, 3, 195, 1) \
				_XX( 760, 01, 3, 190, 1) \
				_XX( 740, 01, 3, 185, 1) \
				_XX( 720, 01, 3, 180, 1) \
				_XX( 700, 01, 3, 175, 1) \
				_XX( 666, 01, 4, 222, 1) \
				_XX( 600, 01, 3, 300, 2) \
				_XX( 550, 01, 3, 275, 2) \
				_XX( 533, 01, 6, 533, 2) \
				_XX( 490, 01, 3, 245, 2) \
				_XX( 470, 01, 3, 235, 2) \
				_XX( 460, 01, 3, 230, 2) \
				_XX( 450, 01, 3, 225, 2) \
				_XX( 440, 01, 3, 220, 2) \
				_XX( 420, 01, 3, 210, 2) \
				_XX( 400, 01, 3, 200, 2) \
				_XX( 390, 01, 3, 190, 2) \
				_XX( 370, 01, 3, 185, 2) \
				_XX( 350, 01, 3, 175, 2) \
				_XX( 330, 01, 3, 165, 2) \
				_XX( 300, 01, 3, 300, 3) \
				_XX( 266, 01, 3, 266, 3) \
				_XX( 250, 01, 3, 250, 3) \
				_XX( 220, 01, 3, 220, 3) \
				_XX( 200, 01, 3, 200, 3) \
				_XX( 166, 01, 3, 166, 3) \
				_XX(147_5, 01, 3, 295, 4) \
				_XX( 133, 01, 3, 266, 4) \
				_XX( 125, 01, 3, 250, 4) \
				_XX( 100, 01, 3, 200, 4) \
				_XX(  96, 01, 3, 192, 4) \
				_XX(  48, 01, 3,  96, 4) \
				_XX(1600, 23, 3, 200, 0) \
				_XX( 800, 23, 3, 200, 1)

#define _PLL_GEN_ENUM(_out, _pll, _p, _m, _1) PLL ## _pll ## _FREQ_ ## _out ## MHZ,

typedef enum {
	_PLL_CLK_PLL_DEFINE(_PLL_GEN_ENUM)
	PLL_FREQ_MAX
} pll_freq_t;

#undef _PLL_GEN_ENUM

typedef enum {
	PLL_CLK_SYS_CPU0 = 0,
	PLL_CLK_SYS_SYS = 1,
	PLL_CLK_SYS_MCLK = 2,
	PLL_CLK_SYS_GR3D = 3,
	PLL_CLK_SYS_MPEG = 4,
	PLL_CLK_SYS_DISP = 5,
	PLL_CLK_SYS_HDMI = 6,
	PLL_CLK_SYS_CPU1 = 7,
	PLL_CLK_SYS_FASTCLK = 8
} pll_clk_sys_index_t;

uint32_t pll_clk_pll_get_freq(pll_index_t index);
void pll_clk_pll_set_freq(pll_index_t index, pll_freq_t freq);

pll_index_t pll_clk_sys_get_src(pll_clk_sys_index_t clk_index);
uint32_t pll_clk_sys_get_bclk_freq(pll_clk_sys_index_t index);
uint32_t pll_clk_sys_get_pclk_freq(pll_clk_sys_index_t index);
void pll_clk_sys_select_src(pll_clk_sys_index_t clk_index, pll_index_t src_index);
void pll_clk_sys_set_bclk_freq(pll_clk_sys_index_t index, uint32_t freq);
void pll_clk_sys_set_pclk_freq(pll_clk_sys_index_t index, uint32_t freq);


#endif /* PLL_CLK_H_ */
