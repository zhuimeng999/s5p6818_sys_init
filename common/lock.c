/*
 * lock.c
 *
 *  Created on: 2017Äê8ÔÂ29ÈÕ
 *      Author: lucius
 */
#include "lock.h"
#include "arm.h"

void spin_lock(spin_lock_t *lock)
{
	__asm__ __volatile__ ("dmb\n\t");
	arm_irq_disable();
	arm_fiq_disable();

	__asm__ __volatile__ (
			"loop: ldrex r2,[%0]\n\t"
			"cmp r2,#0\n\t"
			"bne loop\n\t"
			"mov r2,#1\n\t"
			"strex r3, r2,[%0]\n\t"
			"cmp r3,#0\n\t"
			"bne loop\n\t"
			:
			:"r" (lock)
			:"r1", "r2", "r3", "cc"
			);
//	__asm__ __volatile__ ("dmb\n\t");

}

void spin_unlock(spin_lock_t *lock)
{
//	__asm__ __volatile__ ("dmb\n\t");

	__asm__ __volatile__ (
			"mov r1,#0\n\t"
			"str r1,[%0]\n\t"
			:
			:"r" (lock)
			: "r1"
			);
	arm_irq_enable();
	arm_fiq_enable();
//	__asm__ __volatile__ ("dsb\n\t");
}
