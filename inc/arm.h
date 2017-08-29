/*
 * arm.h
 *
 *  Created on: 2017Äê8ÔÂ7ÈÕ
 *      Author: lucius
 */

#ifndef ARM_H_
#define ARM_H_

#include <stdint.h>

static inline uint32_t getGenericCounterFreq (void)
{
	uint32_t tmp;
  __asm__ __volatile__ (
      "mrc p15,0,%0,c14,c0,0\n\t"
      : "=r" (tmp)
      :
      :
  );
  return tmp;
}

static inline uint32_t getCupId (void)
{
	uint32_t tmp;
  __asm__ __volatile__ (
      "mrc p15,0,%0,c0,c0,5\n\t"
      : "=r" (tmp)
      :
      :
  );
  return tmp;
}


uint64_t getCntpct(void);

static inline void arm_irq_disable(void)
{
	uint32_t tmp;

	__asm__ __volatile__(
		"mrs %0, cpsr\n"
		"orr %0, %0, #(1<<7)\n"
		"msr cpsr_cxsf, %0"
		: "=r" (tmp)
		:
		: "cc");
}

static inline void arm_fiq_disable(void)
{
	uint32_t tmp;

	__asm__ __volatile__(
		"mrs %0, cpsr\n"
		"orr %0, %0, #(1<<6)\n"
		"msr cpsr_cxsf, %0"
		: "=r" (tmp)
		:
		: "cc");
}

static inline void arm_irq_enable(void)
{
	uint32_t tmp;

	__asm__ __volatile__(
		"mrs %0, cpsr\n"
		"bic %0, %0, #(1<<7)\n"
		"msr cpsr_c, %0"
		: "=r" (tmp)
		:
		: "cc");
}

static inline void arm_fiq_enable(void)
{
	uint32_t tmp;

	__asm__ __volatile__(
		"mrs %0, cpsr\n"
		"bic %0, %0, #(1<<6)\n"
		"msr cpsr_c, %0"
		: "=r" (tmp)
		:
		: "cc");
}
#endif /* ARM_H_ */
