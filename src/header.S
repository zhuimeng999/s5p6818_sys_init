/*
 * header.S
 *
 *  Created on: 2017Äê7ÔÂ25ÈÕ
 *      Author: lucius
 */

#define GPIO_BASE (0xC0010000)
#define GPIOx_BASE(x) (GPIO_BASE + 0xA000 +0x1000*(x))
#define GPIOA_BASE GPIOx_BASE(0)
#define GPIOB_BASE GPIOx_BASE(1)
#define GPIOC_BASE GPIOx_BASE(2)
#define GPIOD_BASE GPIOx_BASE(3)
#define GPIOE_BASE GPIOx_BASE(4)

#define GPIOxOUT        0x0
#define GPIOxOUTENB     0x4
#define GPIOxALTFN0     0x20

#define PHY_BASEADDR_INTC_MODULE                            0xC0008000


#define PE_MODE_USER		0x10
#define PE_MODE_FIQ			0x11
#define PE_MODE_IRQ			0x12
#define PE_MODE_SVC			0x13
#define PE_MODE_MON			0x16
#define PE_MODE_ABT			0x17
#define PE_MODE_HYP			0x1A
#define PE_MODE_UND			0x1B
#define PE_MODE_SYS			0x1F


#define CPSR_IRQ			(1 << 7)
#define CPSR_FIQ			(1 << 6)
//.arm
.global Vectors

Vectors:
	ldr		pc, ResetV			/* reset */
	ldr		pc, UndefV			/* undefined */
	ldr		pc, SWIV 			/* software interrupt */
	ldr		pc, PAbortV			/* prefech abort */
	ldr		pc, DAbortV			/* data abort */
	ldr		pc, UnusedV			/* unused */
	ldr		pc, IRQV			/* irq */
	ldr		pc, FIQV			/* fiq */

ResetV:
	.word reset_handler
UndefV:
	.word panic
SWIV:
	.word panic
PAbortV:
	.word panic
DAbortV:
	.word _dapt_handler
UnusedV:
	.word panic
IRQV:
	.word _irq_handler
FIQV:
	.word _fiq_handler

DeviceReadAddr:
	.word 0x200
LOAD_SIZE:
	.word __image_size__
LOAD_ADDR:
	.word 0xFFFF0000
LAUNCH_ADDR:
	.word 0xFFFF0000

reset_handler:
	/* get clustor cpu id */
	mrc		p15, 0, r7, c0, c0, 5
	and		r7, r7, #0xFF

	cmp		r7, #0
	bne		setup_stack

	/* init gpio, should only do once */
	movw	r0, #(GPIOC_BASE & 0xFFFF)
	movt	r0, #(GPIOC_BASE >> 16)

    // set GPIOC12 as ALTFN1
    ldr		r1, [r0, #GPIOxALTFN0]
    bic		r1, r1, #(3 << 24)
    orr		r1, r1, #(1 << 24)
    str		r1, [r0, #GPIOxALTFN0]
    // set GPIOC12 as output mode
    ldr     r1, [r0, #GPIOxOUTENB]
    orr     r1, r1, #(1 << 12)
    str     r1, [r0, #GPIOxOUTENB]
    // set GPIOC12 as high level (means close blue led on board)
    ldr     r1, [r0, #GPIOxOUT]
    orr     r1, r1, #(1 << 12)
    str     r1, [r0, #GPIOxOUT]

	/*clear bss, this should only do once */
	ldr		r4, =__bss_start__
	ldr		r5, =__bss_end__
	mov		r6, #0
clr_bss:
	cmp		r4, r5
	strlo	r6, [r4], #4
	bne		clr_bss

setup_stack:
	/* setup main stack, check if this is main cpu */
	cmp		r7, #0

	ldr		r4,	_stack_end
	ldr		r5, _stack_per_cpu_size
	subne	r6, r7, #1
	moveq	r6, #7
	mul		r5, r5, r6
	sub		r4,	r4, r5

	msr		cpsr_c, #(PE_MODE_FIQ | CPSR_IRQ | CPSR_FIQ)
	mov		sp,	r4
	sub		r4, #256

	msr		cpsr_c, #(PE_MODE_IRQ | CPSR_IRQ | CPSR_FIQ)
	mov		sp, r4
	sub		r4, #256

	/* disable irq, fiq, change to svc mode */
	msr		cpsr_c, #(PE_MODE_SVC | CPSR_IRQ | CPSR_FIQ)
	mov		sp, r4

	mov		r0, r7
	mov		r1, sp

	cmp		r7, #0

	bleq	boot_main
	blne	sub_cpu_main

	b		.
	
panic:
	movw	r0, #(GPIOC_BASE & 0xFFFF)
	movt	r0, #(GPIOC_BASE >> 16)

    // set GPIOC12 as low level (means turn on blue led on board)
    ldr     r1, [r0, #GPIOxOUT]
    bic     r1, r1, #(1 << 12)
    str     r1, [r0, #GPIOxOUT]
	b		.
	
_und_handler:
	ldr sp, =__stack_end
	mov	r0, #2
	mov	r1, lr
	bl	printf_lr
	b .
	
_pabt_handler:
	ldr sp, =__stack_end
	mov	r0, #3
	mov	r1, lr
	bl	printf_lr
	b	.
	
_dapt_handler:
	ldr sp, =__stack_end
	sub r0, lr, #8	/* abort location */
	ldr	r1, [r0]	/* abort instruct */
	mrc	p15, 0, r2, c5, c0, 0	/*dfsr*/
	mrc	p15, 0, r3, c6, c0, 0   /*dfar*/
	bl	report_data_abort
	b .
	
_irq_handler:
	/* only save r0 - r3, gcc will do last */
	sub		lr, lr, #4
	stmfd	sp!, {r0-r12, lr}
	bl		do_irq
	ldmfd	sp!, {r0-r12, pc}^

_fiq_handler:
	mov	r0, #1
	mov	r1, lr
	bl	printf_lr
	b .

stub:
	.space	0x1fc - (stub - Vectors)
signature:
	.word   (('N' << 0) | ('S' << 8) | ('I' << 16) | ('H' << 24))
	
_mmu_init:
	/* disable dcache*/
	mrc		p15, 0, r0, c1, c0, 0
	bic		r0, r0, #(1 << 2)
	mcr		p15, 0, r0, c1, c0, 0
	
	/* get dcache info */
	mov		r0, #0
	mcr		p15, 2, r0, c0, c0, 0
	mrc		p15, 1, r4, c0, c0, 0
	and		r1, r4, #0x7
	add		r1, r1, #0x4
	ldr		r3, =((1 << 15) - 1)
	and		r2, r3, r4, lsr #13
	ldr		r3, =((1 << 10) - 1)
	and		r3, r3, r4, lsr #3
	clz		r4, r3
	mov		r5, #0
	
	/* invalid dcache */
way_loop:
	mov		r6, #0
set_loop:
	orr		r7, r0, r5, lsr r4
	orr		r7, r7, r6, lsr r1
	mcr		p15, 0, r7, c7,c6, 2
	add		r6, r6, #1
	cmp		r6, r2
	ble		set_loop
	add		r5, r5, #1
	cmp 	r5, r3
	ble		way_loop
	
	/* setting tlb */
	mov		r0, #0
	mcr		p15, 0, r0, c2, c0, 2
	
	ldr		r1, =0x55555555
	mcr		p15, 0, r1, c3, c0, 0
	
	mrc		p15, 0, r1, c1, c0, 0
	bic		r1, r1, #(1 << 29)
	mcr		p15, 0, r1, c1, c0, 0
	
	ldr		r0, _tlb_base
	mov		r1, #0x2B
	orr		r1, r0, r1
	mcr		p15, 0, r1, c2, c0, 0
	
	ldr		r4, =0x00100000
	ldr		r2, =0x00015C06
	ldr		r3, =1024
	
loop:
	str		r2, [r0], #4
	add		r2,	r2,	r4
	subs	r3, #1
	bne		loop
	
	ldr		r2, =0x40010C02
	ldr		r3, =3071
loop2:
	str		r2, [r0], #4
	add		r2, r2, r4
	subs	r3, #1
	bne		loop2
	
	ldr		r2,=0x00015C06
	str		r2, [r0], #4

	mrrc	p15, 1, r0, r1, c15
	orr		r0, r0, #(1 << 6)
	mcrr	p15, 1, r0, r1, c15
	
	mrc		p15, 0, r1, c1, c0, 0
	orr		r1, r1, #(1 << 2)
	orr		r1, r1, #(1 << 12)
	orr		r1, r1, #0x1
	mcr		p15, 0, r1, c1, c0, 0

	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	
	bx		lr
	
.align	2
_stack_start:
	.word	__stack_start
_stack_end:
	.word	__stack_end

.global _tlb_base
_tlb_base:
    .word   __tlb_base

_stack_per_cpu_size:
	.word	__stack_percpu_size
