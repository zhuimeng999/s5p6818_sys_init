/*
 * arm_gic.c
 *
 *  Created on: 2017Äê8ÔÂ21ÈÕ
 *      Author: lucius
 */

#include "arm_gic.h"
#include "uart.h"
#include "timer.h"

#define PHY_BASEADDR_INTC_MODULE                            0xC0008000

void irq_enable(void)
{
	uint32_t tmp;

	__asm__ __volatile__(
		"mrs %0, cpsr\n"
		"bic %0, %0, #(1<<7)\n"
		"msr cpsr_cxsf, %0"
		: "=r" (tmp)
		:
		: "memory");
}

void irq_disable(void)
{
	uint32_t tmp;

	__asm__ __volatile__(
		"mrs %0, cpsr\n"
		"orr %0, %0, #(1<<7)\n"
		"msr cpsr_cxsf, %0"
		: "=r" (tmp)
		:
		: "memory");
}

void fiq_enable(void)
{
	uint32_t tmp;

	__asm__ __volatile__(
		"mrs %0, cpsr\n"
		"bic %0, %0, #(1<<6)\n"
		"msr cpsr_cxsf, %0"
		: "=r" (tmp)
		:
		: "memory");
}

void fiq_disable(void)
{
	uint32_t tmp;

	__asm__ __volatile__(
		"mrs %0, cpsr\n"
		"orr %0, %0, #(1<<6)\n"
		"msr cpsr_cxsf, %0"
		: "=r" (tmp)
		:
		: "memory");
}

struct {
	uint32_t itlinesnumber;
	uint32_t irq_num;
	uint32_t irq_mask;
	irq_handler_t irq_handler[100];
} irq_data;

void arm_gic_irq_enable(arm_gic_irq_index_t index)
{
	arm_gic_reg_set_t *reg_set = (arm_gic_reg_set_t *)PHY_BASEADDR_INTC_MODULE;
	arm_gic_dist_t *gicd = &reg_set->gicd;

	gicd->isenable[index/32] |= (1 << (index%32));
}

void arm_gic_irq_disable(arm_gic_irq_index_t index)
{
	arm_gic_reg_set_t *reg_set = (arm_gic_reg_set_t *)PHY_BASEADDR_INTC_MODULE;
	arm_gic_dist_t *gicd = &reg_set->gicd;

	gicd->icenable[index/32] |= (1 << (index%32));
}

typedef enum {
	ARM_GIC_TYPE_LEVEL = 0x00,
	ARM_GIC_TYPE_EDGE = 0x01
} arm_gic_type_t;

void arm_gic_irq_set_type(arm_gic_irq_index_t index, arm_gic_type_t type)
{
	arm_gic_reg_set_t *reg_set = (arm_gic_reg_set_t *)PHY_BASEADDR_INTC_MODULE;
	arm_gic_dist_t *gicd = &reg_set->gicd;
	uint32_t loc = index/16;
	uint32_t bit_offset = index%16;
	uint32_t tmp;

	bit_offset = bit_offset * 2 + 1;

	tmp = gicd->icfgr[loc];
	tmp &= ~(0x3 << bit_offset);
	tmp |= (type << bit_offset);
	gicd->icfgr[loc] = tmp;
}

typedef enum {
	ARM_GIC_GROUP_0 = 0x00,
	ARM_GIC_GROUP_1 = 0x01
} arm_gic_group_t;

void arm_gic_irq_set_group(arm_gic_irq_index_t index, arm_gic_group_t group)
{
	arm_gic_reg_set_t *reg_set = (arm_gic_reg_set_t *)PHY_BASEADDR_INTC_MODULE;
	arm_gic_dist_t *gicd = &reg_set->gicd;

	if(group == ARM_GIC_GROUP_0){
		gicd->isenable[index/32] &= ~(group << (index%32));
	}
	else {
		gicd->isenable[index/32] |= (group << (index%32));
	}
}

void arm_gic_irq_set_priority(arm_gic_irq_index_t index, uint32_t priority)
{
	arm_gic_reg_set_t *reg_set = (arm_gic_reg_set_t *)PHY_BASEADDR_INTC_MODULE;
	arm_gic_dist_t *gicd = &reg_set->gicd;
	uint32_t loc = index/4;
	uint32_t bit_offset = index%4;
	uint32_t tmp;

	bit_offset = bit_offset * 8;

	tmp = gicd->icfgr[loc];
	tmp &= ~(0xff << bit_offset);
	tmp |= (priority << bit_offset);
	gicd->icfgr[loc] = tmp;
}

void arm_gic_irq_set_target(arm_gic_irq_index_t index, uint32_t target)
{
	arm_gic_reg_set_t *reg_set = (arm_gic_reg_set_t *)PHY_BASEADDR_INTC_MODULE;
	arm_gic_dist_t *gicd = &reg_set->gicd;
	uint32_t loc = index/4;
	uint32_t bit_offset = index%4;
	uint32_t tmp;

	bit_offset = bit_offset * 8;

	tmp = gicd->itargetsr[loc];
	tmp &= ~(0xff << bit_offset);
	tmp |= (target << bit_offset);
	gicd->itargetsr[loc] = tmp;
}

void arm_gic_irq_set_priority_mask(uint32_t mask)
{
	arm_gic_reg_set_t *reg_set = (arm_gic_reg_set_t *)PHY_BASEADDR_INTC_MODULE;
	arm_gic_cpu_t *gicc = &reg_set->gicc;

	gicc->PMR = mask;
}

void arm_gic_cpu_init(arm_gic_cpu_t *gicc)
{
	gicc->PMR = 0xFF;
	irq_data.irq_mask = gicc->PMR;
	uart_printf("irq mask = %X\n", irq_data.irq_mask);
	gicc->CTLR = 0x3;
}

void arm_gic_dist_init(arm_gic_dist_t *gicd)
{
	irq_data.itlinesnumber = gicd->typer.reg_field.itlinesnumber;
	irq_data.irq_num = (irq_data.itlinesnumber + 1)*32;

	uart_printf("max irq number:%u\n", irq_data.irq_num);

	gicd->ctrl.reg_value = 0;

	for(uint32_t i = 0; i <= irq_data.itlinesnumber; i++)
	{
		gicd->isenable[i] = 0xFFFFFFFF;
		uart_printf("isenable: %X\n", gicd->isenable[i]);
	}

	for(uint32_t i = 0; i <= irq_data.itlinesnumber; i++)
	{
		gicd->icenable[i] = 0xFFFFFFFF;
		uart_printf("icenable: %X\n", gicd->icenable[i]);
	}

	for(uint32_t i = 0; i < irq_data.irq_num; i++)
	{
		arm_gic_irq_disable(i);
		arm_gic_irq_set_type(i, ARM_GIC_TYPE_EDGE);
		arm_gic_irq_set_priority(i, 0xFF);
		arm_gic_irq_set_group(i, ARM_GIC_GROUP_1);
		arm_gic_irq_set_target(i, 0x01);
	}

	gicd->ctrl.reg_field.enablegrp0 = 1;
	gicd->ctrl.reg_field.enablegrp1 = 1;
}

void arm_gic_init(void)
{
	arm_gic_reg_set_t *reg_set = (arm_gic_reg_set_t *)PHY_BASEADDR_INTC_MODULE;

	arm_gic_dist_init(&reg_set->gicd);
	arm_gic_cpu_init(&reg_set->gicc);
	irq_enable();
	fiq_enable();
}

void arm_gic_check_pend(void)
{
	arm_gic_reg_set_t *reg_set = (arm_gic_reg_set_t *)PHY_BASEADDR_INTC_MODULE;
	arm_gic_dist_t *gicd = &reg_set->gicd;

	for(uint32_t i = 0; i < 16; i++)
	{
		if(gicd->ispendr[i] != 0)
		{
			uart_printf("%u:%X\n", i, gicd->ispendr[i]);
			gicd->icpendr[i] = 0xFFFFFFFF;
		}
	}
}

void do_irq(void)
{
	arm_gic_reg_set_t *reg_set = (arm_gic_reg_set_t *)PHY_BASEADDR_INTC_MODULE;
	arm_gic_cpu_t *gicc = &reg_set->gicc;

	uint32_t irq_num = gicc->IAR & 0x3FF;
	irq_data.irq_handler[irq_num]();
	gicc->EOIR = irq_num;
}

void request_irq(arm_gic_irq_index_t index, irq_handler_t handler)
{
	irq_data.irq_handler[index] = handler;
	arm_gic_irq_enable(index);
	arm_gic_irq_set_priority(index, 0);
}
