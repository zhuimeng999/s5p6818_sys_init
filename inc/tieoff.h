/*
 * tieoff.h
 *
 *  Created on: 2017Äê8ÔÂ6ÈÕ
 *      Author: lucius
 */

#ifndef TIEOFF_H_
#define TIEOFF_H_

struct s5p6818_tieoff_reg
{
  volatile uint32_t con[131];
};

#define TIEOFF_BASE (0xC0011000)
#define TIEOFF ((struct s5p6818_tieoff_reg *)TIEOFF_BASE)

#define TIEOFF_SMARTCARD_UART0 (1<<20)
#define TIEOFF_SMARTCARD_TX_UART0 (1<<21)
#define TIEOFF_SMARTCARD_RX_UART0 (1<<22)

#endif /* TIEOFF_H_ */
