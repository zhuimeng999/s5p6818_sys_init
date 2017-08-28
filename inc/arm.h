/*
 * arm.h
 *
 *  Created on: 2017��8��7��
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

#endif /* ARM_H_ */
