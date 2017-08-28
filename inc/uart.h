/*
 * uart.h
 *
 *  Created on: 2017Äê8ÔÂ7ÈÕ
 *      Author: lucius
 */

#ifndef UART_H_
#define UART_H_

void uart_init(void);
void uart_write(char *str, int len);
int uart_printf(char *fmt, ...);
int uart_is_busy(void);

#endif /* UART_H_ */
