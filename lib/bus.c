/*
 * bus.c
 *
 *  Created on: 2017Äê8ÔÂ29ÈÕ
 *      Author: lucius
 */

#include <bus.h>
#include "uart.h"

void init_bus(void)
{
	static struct cci_400_regs * const cci_400 = (struct cci_400_regs *)0xE0090000;

	enum BUS_ID {
		BUS_ID_CORESIGHT = 0,
		BUS_ID_CODA960,
		BUS_ID_TOP,
		BUS_ID_CPU_CLUSTER_0,
		BUS_ID_CPU_CLUSTER_1
	};

	uart_printf("Initializing CCI-400 bus\n");

	cci_400->speculation_control |= (1 << 0);
	cci_400->speculation_control |= (1 << 1);

	cci_400->control_override = (1 << 3);

	cci_400->slave[BUS_ID_CORESIGHT].snoop_control = 0;
	cci_400->slave[BUS_ID_CODA960].snoop_control = 0;
	cci_400->slave[BUS_ID_TOP].snoop_control = 0;

	cci_400->slave[BUS_ID_CPU_CLUSTER_0].snoop_control = 0x3;
	while (cci_400->status & (1 << 0));

	cci_400->slave[BUS_ID_CPU_CLUSTER_1].snoop_control = 0x3;
	while (cci_400->status & (1 << 0));
}
