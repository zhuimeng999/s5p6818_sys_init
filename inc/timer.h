/*
 * timer.h
 *
 *  Created on: 2017Äê8ÔÂ10ÈÕ
 *      Author: lucius
 */

#ifndef TIMER_H_
#define TIMER_H_

#include <stdint.h>

typedef union {
	struct {
		volatile uint32_t prescale0 : 8;
		volatile uint32_t prescale1 : 8;
		volatile uint32_t deadzone_length : 8;
		volatile uint32_t reserved0 : 8;
	} reg_field;
	volatile uint32_t reg_value;
} tcfg0_t;


typedef union {
	struct {
		volatile uint32_t divider_mux0 : 4;
		volatile uint32_t divider_mux1 : 4;
		volatile uint32_t divider_mux2 : 4;
		volatile uint32_t divider_mux3 : 4;
		volatile uint32_t divider_mux4 : 4;
		volatile uint32_t dma_mode : 4;
		volatile uint32_t reserved : 8;
	} reg_field;
	volatile uint32_t reg_value;
} tcfg1_t;


typedef union {
	struct {
		volatile uint32_t timer0_start : 1;
		volatile uint32_t timer0_manual_update : 1;
		volatile uint32_t timer0_output_inverte : 1;
		volatile uint32_t timer0_auto_reload : 1;
		volatile uint32_t dead_zone : 1;
		volatile uint32_t reserved0 : 3;
		volatile uint32_t timer1_start : 1;
		volatile uint32_t timer1_manual_update : 1;
		volatile uint32_t timer1_output_inverte : 1;
		volatile uint32_t timer1_auto_reload : 1;
		volatile uint32_t timer2_start : 1;
		volatile uint32_t timer2_manual_update : 1;
		volatile uint32_t timer2_output_inverte : 1;
		volatile uint32_t timer2_auto_reload : 1;
		volatile uint32_t timer3_start : 1;
		volatile uint32_t timer3_manual_update : 1;
		volatile uint32_t timer3_output_inverte : 1;
		volatile uint32_t timer3_auto_reload : 1;
		volatile uint32_t timer4_start : 1;
		volatile uint32_t timer4_manual_update : 1;
		volatile uint32_t timer4_auto_reload : 1;
		volatile uint32_t reserved1 : 9;
	} reg_field;
	volatile uint32_t reg_value;
} tcon_t;

typedef char sizeCheck_tcon_t[(sizeof(tcon_t) - 4)?-1:1];

typedef union {
	volatile struct {
		volatile uint32_t timer0_irq_enable : 1;
		volatile uint32_t timer1_irq_enable : 1;
		volatile uint32_t timer2_irq_enable : 1;
		volatile uint32_t timer3_irq_enable : 1;
		volatile uint32_t timer4_irq_enable : 1;
		volatile uint32_t timer0_irq_status : 1;
		volatile uint32_t timer1_irq_status : 1;
		volatile uint32_t timer2_irq_status : 1;
		volatile uint32_t timer3_irq_status : 1;
		volatile uint32_t timer4_irq_status : 1;
		volatile uint32_t reserved0 : 22;
	} reg_field;
	volatile uint32_t reg_value;
} tint_cstat_t;
typedef char sizeCheck_tint_cstat_t[(sizeof(tint_cstat_t) - 4)?-1:1];
//------------------------------------------------------------------------------
/// @brief  TIMER register set structure
//------------------------------------------------------------------------------
typedef struct
{
	volatile tcfg0_t tcfg0		;	// 0x00	// R/W	// Timer Configuration Register 0 that configures the two 8-bit Prescaler and DeadZone Length
	volatile tcfg1_t tcfg1		;	// 0x04	// R/W	// Timer Configuration Register 1 that controls 5 MUX and DMA Mode Select Bit
	volatile tcon_t tcon       ;	// 0x08	// R/W	// Timer Control Register
	volatile uint32_t tcntb0     ;	// 0x0C	// R/W	// Timer 0 Count Buffer Register
	volatile uint32_t tcmpb0     ;	// 0x10	// R/W	// Timer 0 Compare Buffer Register
	volatile uint32_t tcnto0     ;	// 0x14	// R	// Timer 0 Count Observation Register
 	volatile uint32_t tcntb1     ;	// 0x18	// R/W	// Timer 1 Count Buffer Register
	volatile uint32_t tcmpb1     ;	// 0x1C	// R/W	// Timer 1 Compare Buffer Register
	volatile uint32_t tcnto1     ;	// 0x20	// R	// Timer 1 Count Observation Register
	volatile uint32_t tcntb2     ;	// 0x24	// R/W	// Timer 2 Count Buffer Register
	volatile uint32_t tcmpb2     ;	// 0x28	// R/W	// Timer 2 Compare Buffer Register
	volatile uint32_t tcnto2     ;	// 0x2C	// R	// Timer 2 Count Observation Register
	volatile uint32_t tcntb3     ;	// 0x30	// R/W	// Timer 3 Count Buffer Register
	volatile uint32_t tcmpb3     ;	// 0x34	// R/W	// Timer 3 Compare Buffer Register
	volatile uint32_t tcnto3     ;	// 0x38	// R	// Timer 3 Count Observation Register
	volatile uint32_t tcntb4     ;	// 0x3C	// R/W	// Timer 4 Count Buffer Register
	volatile uint32_t tcnto4     ;	// 0x40	// R	// Timer 4 Count Observation Register
	volatile tint_cstat_t tint_cstat ;	// 0x44	// R/W	// Timer Interrupt Control and Status Register
	volatile uint8_t _Reserved0[0x1000-0x48];	// 0x48~0x1000
} timer_register_t;

typedef enum {
	TIMER_INDEX_0,
	TIMER_INDEX_1,
	TIMER_INDEX_2,
	TIMER_INDEX_3,
	TIMER_INDEX_4
} timer_index_t;

typedef enum {
	TIMER_DIVIDER_1 = 0,
	TIMER_DIVIDER_2 = 1,
	TIMER_DIVIDER_4 = 2,
	TIMER_DIVIDER_8 = 3,
	TIMER_DIVIDER_16 = 4,
	TIMER_DIVIDER_TCLK1 = 5,
	TIMER_DIVIDER_TCLK2 = 6,
	TIMER_DIVIDER_TCLK3 = 7
} timer_div_t;

void timer_module_enable(void);

void timer_start(timer_index_t index);
void timer_stop(timer_index_t index);
void timer_autoreload_enable(timer_index_t index);
void timer_autoreload_disable(timer_index_t index);
void timer_interrupt_enable(timer_index_t index);
void timer_interrupt_disable(timer_index_t index);
void timer_interrupt_clear(timer_index_t index);
uint32_t timer_interrupt_get(timer_index_t index);

uint32_t timer_get_count(timer_index_t index);
uint32_t timer_get_count_buffer(timer_index_t index);
void timer_set_count_buffer(timer_index_t index, uint32_t count);
uint32_t timer_helper_freq_to_count(timer_index_t index, uint32_t freq);
uint32_t timer_helper_get_freq(timer_index_t index);

#endif /* TIMER_H_ */
