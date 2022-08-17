/**
 *	SPDX-License-Identifier:	GPL-2.0+
 */

#define CONFIG_STACK_LENGTH     0x400

    .section .text
    .align 4
__vectors:
    b _start
    b undefined_instruction
    b software_interrupt
    b prefetch_abort
    b data_abort
    b not_used
    b irq
    b fiq

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
    ldr sp,=0x2000
    stmfd   sp!, {r0-r12,lr}
    mrc p15, 0, r2, c1, c0, 0
    mov r1,lr
    mrs r0,cpsr
    bl show_reg
    ldmfd   sp!, {r0-r12,lr}
    subs pc, lr, #4

fiq:
    b .


_start:
    /*
	 * set the cpu to SVC32 mode
	 */
	mrs	r0,cpsr
	bic	r0,r0,#0x1f
	orr	r0,r0,#0xd3
	msr	cpsr,r0

    mov r0, #0
    mcr p15, 0, r0, c7, c7, 0
    mcr p15, 0, r0, c8, c7, 0

    /* Set vector to the low address */
	mrc p15, 0, r0, c1, c0, 0
	bic r0, #(1<<13)
	mcr p15, 0, r0, c1, c0, 0


    /* disable MMU stuff and caches */
    mrc p15, 0, r0, c1, c0, 0
    bic r0, r0, #0x00002300
    bic r0, r0, #0x00000087
    //orr r0, r0, #0x00000002
    //orr r0, r0, #0x00001000
    mcr p15, 0, r0, c1, c0, 0

    /* mask all IRQs source */
    ldr r1, =0x0
    ldr r0, =0x01C20430
    str r1, [r0], #0x04
    str r1, [r0]
    
    ldr r0, =__vectors
    mrc p15, 0, r2, c1, c0, 0
    ands r2, r2, #(1 << 13)
    ldreq r1, =0x00000000
    ldrne r1, =0xffff0000
    ldmia r0!, {r2-r8, r10}
    stmia r1!, {r2-r8, r10}
    ldmia r0!, {r2-r8, r10}
    stmia r1!, {r2-r8, r10}
    ldr r1, =0x00020
    ldmia r0!, {r2-r8, r10}
    stmia r1!, {r2-r8, r10}
    ldmia r0!, {r2-r8, r10}
    stmia r1!, {r2-r8, r10}

    ldr r0, =0x01C20CB8
    mov     r1, #0x0
    str     r1, [r0]

    /**
     * bss section clear 0
     */
    ldr r0,=bss_start
    ldr r1,=bss_end
    mov r3,#0
b_loop:
    str r3,[r0]
    add r0,#4
    cmp r0,r1
    /* 不相等时跳转 */
    bne b_loop
    /**
     * set sp pointer
     */
    ldr r0,=stackend
    mov sp,r0

    /**
     * goto c world
     */
    bl main


    .align 4
    .global en_irq
    .type en_irq,%function
en_irq:
    mrs r0, cpsr
    bic r0,r0,#0x80
    msr cpsr, r0
    bx lr

    .align 4
    .global get_cpsr
    .type get_cpsr,%function
get_cpsr:
    mrs r0,cpsr
    bx lr

    .align 4
    .global set_pc
    .type set_pc,%function
set_pc:
    ldr pc,=0x08


    .align 2
    .section .text
    .weak default_handler
    .type default_handler,%function
default_handler:
    bx lr
    .size default_handler,. - default_handler

    .align 4
    .global delay
    .type	delay, %function
delay:
    ldr r2,=200000
delay_loop:
    cmp r2,#0
    moveq pc,lr
    sub r2, r2, #1
    b delay_loop

    .align 4
    .global get_sp
get_sp:
    mov r0,sp
    bx lr


    .section .bss
    .align 4
stackbase:
    .space CONFIG_STACK_LENGTH
stackend:


