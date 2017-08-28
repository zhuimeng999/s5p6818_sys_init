/*
 * rtc.c
 *
 *  Created on: 2017Äê8ÔÂ12ÈÕ
 *      Author: lucius
 */

#include "rtc.h"

#define RTC_REGISTER_BASE 0xC0010C00

#define RTC_WRITE_ENABLE (1u)
#define RTC_WRITE_DISABLE (0u)
#define RTC_WRITE_BUSY (1u)
#define RTC_WRITE_IDLE (0u)

uint32_t rtc_get_count(void)
{
	uint32_t count;
	rtc_register_t *reg_set = (rtc_register_t *)RTC_REGISTER_BASE;
	reg_set->rtcctrl.reg_field.rtccntwriteenb = RTC_WRITE_ENABLE;
	count = reg_set->rtccntread;
	reg_set->rtcctrl.reg_field.rtccntwriteenb = RTC_WRITE_DISABLE;

	return count;
}

void rtc_set_count(uint32_t count)
{
	rtc_register_t *reg_set = (rtc_register_t *)RTC_REGISTER_BASE;

	reg_set->rtcctrl.reg_field.rtccntwriteenb = RTC_WRITE_ENABLE;
	while(reg_set->rtcctrl.reg_field.rtccntwait);
	reg_set->rtccntwrite = count;
	while(reg_set->rtcctrl.reg_field.rtccntwait);
	reg_set->rtcctrl.reg_field.rtccntwriteenb = RTC_WRITE_DISABLE;
}
