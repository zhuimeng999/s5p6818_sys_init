/*
 * rstcon.c
 *
 *  Created on: 2017Äê8ÔÂ7ÈÕ
 *      Author: lucius
 */

#include "rstcon.h"

struct s5p6818_iprst_reg
{
  volatile uint32_t con[3];
};

#define IPRST_BASE (0xC0012000)
#define IPRST ((struct s5p6818_iprst_reg *)IPRST_BASE)

#define IPRST_UART0 (1<<17)

void rstcon_set_rst(rstcon_t index, rstcon_state_t status)
{
  struct s5p6818_iprst_reg *regrst = IPRST;
  uint32_t setValue = (status == RSTCON_NEGATE)?1:0;
  uint32_t regvalue;

  regvalue = regrst->con[index >> 5];

  regvalue &= ~(1ul << (index & 0x1f));
  regvalue |= setValue << (index & 0x1f);

  regrst->con[index >> 5] = regvalue;
}

rstcon_state_t	rstcon_get_rst(rstcon_t index)
{
  struct s5p6818_iprst_reg *regrst = IPRST;
  uint32_t reg_value = regrst->con[index >> 5];

  return (reg_value & (1ul << (index & 0x1f)))?RSTCON_NEGATE:RSTCON_ASSERT;
}

