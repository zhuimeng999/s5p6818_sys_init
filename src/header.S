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
	.word panic
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


	mrc		p15, 0, r8, c0, c0, 5
	and		r8, r8, #0xFF
	
	/*clear bss*/
	ldr		r4, =__bss_start__
	ldr		r5, =__bss_end__
	mov		r6, #0
clrbss:
	cmp		r4, r5
	strlo	r6, [r4], #4
	bne		clrbss

	/* setup stack */
	msr		cpsr_c, #(PE_MODE_IRQ | CPSR_IRQ | CPSR_FIQ)
	ldr		sp, =__stack_irq_end

	msr		cpsr_c, #(PE_MODE_FIQ | CPSR_IRQ | CPSR_FIQ)
	ldr		sp, =__stack_fiq_end

	/* disable irq, fiq, change to svc mode */
	msr		cpsr_c, #(PE_MODE_SVC | CPSR_IRQ | CPSR_FIQ)
	ldr		sp, =__stack_svc_end

	bl		main
	b		.

panic:
	movw	r0, #(GPIOC_BASE & 0xFFFF)
	movt	r0, #(GPIOC_BASE >> 16)

    // set GPIOC12 as low level (means turn on blue led on board)
    ldr     r1, [r0, #GPIOxOUT]
    bic     r1, r1, #(1 << 12)
    str     r1, [r0, #GPIOxOUT]
	b		.
	
//.align	5
_irq_handler:
	/* only save r0 - r3, gcc will do last */
	sub		lr, lr, #4
	stmfd	sp!, {r0-r12, lr}
	bl		do_irq
	ldmfd	sp!, {r0-r12, pc}^
	


_fiq_handler:
	ldr sp, =0xFFFF8000
	mov	r0, #1
	mov	r1, lr
	bl	printf_lr
	b .
_und_handler:
	ldr sp, =0xFFFF8000
	mov	r0, #2
	mov	r1, lr
	bl	printf_lr
	b .
_pabt_handler:
	ldr sp, =0xFFFF8000
	mov	r0, #3
	mov	r1, lr
	bl	printf_lr
	b	.
_dapt_handler:
	ldr sp, =0xFFFF8000
	mov	r0, #4
	mov	r1, lr
	bl	printf_lr
	b .


stub:
	.space	0x1fc - (stub - Vectors)
signature:
	.word   (('N' << 0) | ('S' << 8) | ('I' << 16) | ('H' << 24))