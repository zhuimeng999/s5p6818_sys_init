/*
 * arm_gic.h
 *
 *  Created on: 2017Äê8ÔÂ21ÈÕ
 *      Author: lucius
 */

#ifndef ARM_GIC_H_
#define ARM_GIC_H_

#include <stdint.h>

typedef union {
	struct {
		volatile uint32_t enablegrp0 : 1;
		volatile uint32_t enablegrp1 : 1;
		volatile uint32_t reserved0 : 30;
	} reg_field;
	volatile uint32_t reg_value;
} gicd_ctrl_t;

typedef union {
	struct {
		volatile uint32_t itlinesnumber : 5;
		volatile uint32_t cpunumber : 3;
		volatile uint32_t reserved0 : 2;
		volatile uint32_t securityextn : 1;
		volatile uint32_t lspi : 5;
		volatile uint32_t reserved1 : 16;
	} reg_field;
	volatile uint32_t reg_value;
} gicd_typer_t;

typedef struct	// Distributor
{
	volatile gicd_ctrl_t ctrl;				// 0x000 Distributor Control
	volatile gicd_typer_t typer;				// 0x004 Interrupt Controller Type
	volatile uint32_t iidr;				// 0x008 Identification 0x0200143B
	volatile uint32_t _Reserved0[29];	// 0x00C~0x07C
	volatile uint32_t igroupr[16];		// 0x080~0x0BC Interrupt Group
	volatile uint32_t _Reserved1[16];	// 0x0C0~0x0FC
	volatile uint32_t isenable[16];		// 0x100~0x13C Interrupt Set-Enable
	volatile uint32_t _Reserved2[16];	// 0x140~0x17C
	volatile uint32_t icenable[16];	// 0x180~0x1BC Interrupt Clear-Enable
	volatile uint32_t _Reserved3[16];	// 0x1C0~0x1FC
	volatile uint32_t ispendr[16];		// 0x200~0x23C Interrupt Set-Pending
	volatile uint32_t _Reserved4[16];	// 0x240~0x27C
	volatile uint32_t icpendr[16];		// 0x280~0x2BC Interrupt Clear-Pending
	volatile uint32_t _Reserved5[16];	// 0x2C0~0x2FC
	volatile uint32_t isactiver[16];		// 0x300~0x33C Interrupt Set-Active
	volatile uint32_t _Reserved6[16];	// 0x340~0x37C
	volatile uint32_t icactiver[16];	// 0x380~0x3BC Interrupt Clear-Active
	volatile uint32_t _Reserved7[16];	// 0x3C0~0x3FC
	volatile uint32_t ipriorityr[128];	// 0x400~0x5FC Interrupt Priority
	volatile uint32_t _Reserved8[128];	// 0x600~0x7FC
	volatile uint32_t itargetsr[8];	// 0x800~0x81C Interrupt Processor Targets
	volatile uint32_t _Reserved9[248];	// 0x820~0xBFC
	volatile uint32_t icfgr[32];		// 0xC00~0xC7C Interrupt Configuration
	volatile uint32_t _Reserved10[32];	// 0xC80~0xCFC
	volatile uint32_t ppisr[16];// 0xD00~0xD3C Private & Shared Peripheral Interrupt Status
	volatile uint32_t _Reserved11[112];	// 0xD40~0xEFC
	volatile uint32_t sgir;				// 0xF00 Software Generated Interrupt
	volatile uint32_t _Reserved12[3];	// 0xF04~0xF0C
	volatile uint32_t cpendsgir[4];		// 0xF10~0xF1C SGI Clear-Pending
	volatile uint32_t spendsgir[4];		// 0xF20~0xF2C SGI Set-Pending
	volatile uint32_t _Reserved13[40];	// 0xF30~0xFCC
	volatile uint32_t pidr[8];			// 0xFD0~0xFEC Peripheral IDx
	volatile uint32_t cidr[4];			// 0xFF0~0xFFC Component IDx
} arm_gic_dist_t;

typedef struct NX_GIC400_GICC_RegisterSet	// CPU Interface
{
	volatile uint32_t CTLR;				// 0x0000 CPU Interface Control
	volatile uint32_t PMR;				// 0x0004 Interrupt Priority Mask
	volatile uint32_t BPR;				// 0x0008 Binary Point
	volatile uint32_t IAR;				// 0x000C Interrupt Acknowledge
	volatile uint32_t EOIR;				// 0x0010 End of Interrupt
	volatile uint32_t RPR;				// 0x0014 Running Priority
	volatile uint32_t HPPIR;		// 0x0018 Highest Priority Pending Interrupt
	volatile uint32_t ABPR;				// 0x001C Aliased Binary Point
	volatile uint32_t AIAR;				// 0x0020 Aliased Interrupt Acknowledge
	volatile uint32_t AEOIR;				// 0x0024 Aliased End of Interrupt
	volatile uint32_t AHPPIR;// 0x0028 Aliased Highest Priority Pending Interrupt
	volatile uint32_t _Reserved0[41];	// 0x002C~0x00CC
	volatile uint32_t APR0;				// 0x00D0 Active Priority
	volatile uint32_t _Reserved1[3];		// 0x00D4~0x00DC
	volatile uint32_t NSAPR0;			// 0x00E0 Non-Secure Active Priority
	volatile uint32_t _Reserved2[6];		// 0x00E4~0x00F8
	volatile uint32_t IIDR;	// 0x00FC CPU Interface Identification, 0x0202143B
	volatile uint32_t _Reserved3[0x3C0];	// 0x0100~0x0FFC
	volatile uint32_t DIR;				// 0x1000 Deactivate Interrupt
	volatile uint32_t _Reserved4[0x3FF];	// 0x1004~0x1FFC
} arm_gic_cpu_t;

typedef struct	// Virtual Interface Control
{
	volatile uint32_t HCR;				// 0x000 Hypervisor Control
	volatile uint32_t VTR;				// 0x004 VGIC Type
	volatile uint32_t VMCR;				// 0x008 Virtual Machine Control
	volatile uint32_t _Reserved0;		// 0x00C
	volatile uint32_t MISR;				// 0x010 Maintenance Interrupt Status
	volatile uint32_t _Reserved1[3];		// 0x014~0x01C
	volatile uint32_t EISR0;				// 0x020 End of Interrupt Status
	volatile uint32_t _Reserved2[3];		// 0x024~0x02C
	volatile uint32_t ELSR0;				// 0x030 Empty List register Status
	volatile uint32_t _Reserved3[47];	// 0x034~0x0EC
	volatile uint32_t APR0;				// 0x0F0 Active Priority
	volatile uint32_t _Reserved4[3];		// 0x0F4~0x0FC
	volatile uint32_t LR[4];				// 0x100~0x10C List
	volatile uint32_t _Reserved5[0x3C];	// 0x110~0xFFC
} arm_gic_gich_t;

typedef struct {
	volatile uint32_t _Reserved0[0x400];			// 0x0000~0x0FFC Reserved
	arm_gic_dist_t gicd;		// 0x1000~0x1FFC Distributor
	arm_gic_cpu_t gicc;		// 0x2000~0x3FFC CPU interfaces
	arm_gic_gich_t gich;	// 0x4000~0x41FC Virtual interface control block, for the processor that is performing the access
	volatile uint32_t _Reserved1[0x380];				// 0x4200~0x4FFC
	arm_gic_gich_t GICHv[8];	// 0x5000~0x5FFC Virtual interface control block, for the processor selected by address bits[11:9]
	struct NX_GIC400_GICC_RegisterSet GICCv;// 0x6000~0x7FFC Virtual CPU interface
} arm_gic_reg_set_t;

typedef enum {
	ARM_GIC_IRQ_INDEX_TIMER0 = 55,
	ARM_GIC_IRQ_INDEX_TIMER1 = 56,
	ARM_GIC_IRQ_INDEX_TIMER2 = 57,
	ARM_GIC_IRQ_INDEX_TIMER3 = 58,
	ARM_GIC_IRQ_INDEX_TIMER4 = 96
} arm_gic_irq_index_t;

typedef void (*irq_handler_t)(void);

void arm_gic_generic_init(void);
void arm_gic_percpu_init(uint32_t cpu_id);

void arm_gic_check_pend(void);
void request_irq(arm_gic_irq_index_t index, irq_handler_t handler);

#endif /* ARM_GIC_H_ */
