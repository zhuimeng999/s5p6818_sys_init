/*
 * subCpuBoot.c
 *
 *  Created on: 2017Äê8ÔÂ28ÈÕ
 *      Author: lucius
 */

#include "subCpuBoot.h"
#include "pll_clk.h"
#include "uart.h"
#include "tieoff.h"
#include "arm.h"

clkpwr_regset_t *pReg_ClkPwr = (clkpwr_regset_t *) PHY_BASEADDR_CLKPWR_MODULE;
struct s5p6818_tieoff_reg *pReg_Tieoff = TIEOFF;

#define ReadIO32(Addr)          (*(volatile unsigned int  *)(Addr))

void BringUpSlaveCPU(uint32_t  CPUID)
{
	//WriteIO32(&pReg_ClkPwr->CPURESETMODE, 0x1);
	pReg_ClkPwr->CPURESETMODE = 0x1;
	//WriteIO32(&pReg_ClkPwr->CPUPOWERDOWNREQ, (1 << CPUID));
	pReg_ClkPwr->CPUPOWERDOWNREQ = (1 << CPUID);
	//WriteIO32(&pReg_ClkPwr->CPUPOWERONREQ, (1 << CPUID));
	pReg_ClkPwr->CPUPOWERONREQ = (1 << CPUID);
}

void SetVectorLocation(uint32_t CPUID, uint32_t LowHigh)
{
	uint32_t bits, regvalue;

	if (CPUID & 0x4) // cpu 4, 5, 6, 7
	{
		regvalue = pReg_Tieoff->con[95];
		bits = 1 << (12 + (CPUID & 0x3));
	} else {
		regvalue = pReg_Tieoff->con[78];
		bits = 1 << (20 + (CPUID & 0x3));
	}

	if (LowHigh)
		regvalue |= bits;
	else
		regvalue &= ~bits;

	if (CPUID & 0x4) // cpu 4, 5, 6, 7
	{
		pReg_Tieoff->con[95] = regvalue;
	} else {
		pReg_Tieoff->con[78] = regvalue;
	}

}

void subCpuBoot(void)
{
	uart_printf("cpu id: %X\n", getCupId());
	SetVectorLocation(1, 1);
	BringUpSlaveCPU(1);
}
