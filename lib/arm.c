/*
 * arm.c
 *
 *  Created on: 2017Äê8ÔÂ7ÈÕ
 *      Author: lucius
 */

#include "arm.h"

uint64_t
getCntpct (void)
{
  uint32_t tmp;
  uint32_t tmp1;
  uint64_t ret;
  __asm__ __volatile__ (
      "mrrc p15,0,%0,%1,c14\n\t"
      : "=r" (tmp), "=r" (tmp1)
      :
      :
  );

  ret = (tmp << 31) + tmp1;
  return ret;
}
