/*
 * rtc.h
 *
 *  Created on: 2017Äê8ÔÂ12ÈÕ
 *      Author: lucius
 */

#ifndef RTC_H_
#define RTC_H_

#include <stdint.h>

typedef union {
	struct {
		volatile uint32_t rtccntwriteenb : 1;
		volatile uint32_t reserved0 : 2;
		volatile uint32_t alarmcntwait : 1;
		volatile uint32_t rtccntwait : 1;
		volatile uint32_t reserved1 : 27;
	} reg_field;
	volatile uint32_t reg_value;
} rtcctl_t;
	/// @brief	RTC Module's Register List
typedef struct {
		volatile uint32_t rtccntwrite;		///< 0x00 : RTC counter register (Write only)
		volatile uint32_t rtccntread;		///< 0x04 : RTC counter register (Read only)
		volatile uint32_t RTCALARM;			///< 0x08 : RTC alarm register
		volatile rtcctl_t rtcctrl;			///< 0x0C : RTC control register
		volatile uint32_t RTCINTENB;			///< 0x10 : RTC interrupt enable register
		volatile uint32_t RTCINTPND;			///< 0x14 : RTC interrupt pending register (Read only)
		volatile uint32_t RTCCORERSTIMESEL;	///< 0x18 : RTC Core POR time select register
		volatile uint32_t RTCSCRATCH;
		volatile uint32_t RTCSCRATCHSTATUS;
} rtc_register_t ;

uint32_t rtc_get_count(void);
void rtc_set_count(uint32_t count);

#endif /* RTC_H_ */
