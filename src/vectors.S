/**
 *	SPDX-License-Identifier:	GPL-2.0+
 *  f1c200s vector
 */

.equ MODE_USR,        0x10
.equ MODE_FIQ,        0x11
.equ MODE_IRQ,        0x12
.equ MODE_SVC,        0x13
.equ MODE_ABT,        0x17
.equ MODE_UND,        0x1B
.equ MODE_SYS,        0x1F
.equ MODEMASK,        0x1F
.equ NOINT,           0xC0

.equ I_BIT,           0x80
.equ F_BIT,           0x40

    .section .vectors
    .align 4
    .global __vectors
__vectors:
    b _start
    ldr pc,=undefined_instruction
    ldr pc,=software_interrupt
    ldr pc,=prefetch_abort
    ldr pc,=data_abort
    ldr pc,=not_used
    ldr pc,=irq
    ldr pc,=fiq

	.balignl 16,0xdeadbeef

undefined_instruction:
    b .
software_interrupt:
    b .
prefetch_abort:
    b .
data_abort:
    b .
not_used:
    b .
irq:
    stmfd   sp!, {r0-r12,lr}
    bl do_irq
    ldmfd   sp!, {r0-r12,lr}
    subs pc, lr, #4
fiq:
    b .

    .section .text
    .align 4
    .global set_stack
    /**
     * set sp pointer
     */
set_stack:
    // irq 10010
    mrs r0,cpsr
    bic r0,r0,#0x1f
    orr r1,r0,#0x12|0xc0
    msr cpsr,r1
    ldr sp,=__irq_stack_end
    // fiq 10001
    orr r1,r0,#0x11|0xc0
    msr cpsr,r1
    ldr sp,=__fiq_stack_end
    // undef 11011
    orr r1,r0,#0x1b|0xc0
    msr cpsr,r1
    ldr sp,=__abort_stack_end
    // abort 10111
    orr r1,r0,#0x17|0xc0
    msr cpsr,r1
    ldr sp,=__undef_stack_end
    // svc
    orr r1,r0,#0xd3
    msr cpsr,r1
    ldr sp,=__svc_stack_end
    
    bx lr


