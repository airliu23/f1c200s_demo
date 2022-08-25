/**
 *	SPDX-License-Identifier:	GPL-2.0+
 *  start
 */

#define CONFIG_SVC_STACK_LEN        0x400
#define CONFIG_IRQ_STACK_LEN        0x400
#define CONFIG_FIQ_STACK_LEN        0x400
#define CONFIG_ABORT_STACK_LEN      0x400
#define CONFIG_UNDEF_STACK_LEN      0x400
#define CONFIG_SYSTEM_STACK_LEN     0x400

    .section .text
    .align 4
    .global __start
__start:
    /*
	 * set the cpu to SVC32 mode
	 */
	mrs	r0,cpsr
	bic	r0,r0,#0x1f
	orr	r0,r0,#0xd3
	msr	cpsr,r0    

    /* invalidate I/D caches */
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
    /* not use caches */
    //orr r0, r0, #0x00000002
    //orr r0, r0, #0x00001000
    mcr p15, 0, r0, c1, c0, 0

    /* mask all IRQs source */
    ldr r1, =0x0
    ldr r0, =0x01C20430
    str r1, [r0], #0x04
    str r1, [r0]
    
    /* copy vectors */
    ldr r0, =0xea000016
    ldr r1, =0x00
vector_loop:
    cmp r1, #0x20
    beq vector_loop_exit
    str r0,[r1]
    add r1,#4
    b vector_loop
vector_loop_exit:

    /* turn off the watchdog */
    ldr r0, =0x01C20CB8
    mov     r1, #0x0
    str     r1, [r0]

    /* set all mode sp pointer */
    bl set_stack

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
     * goto c world
     */
    bl main

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


    .section .bss
    .align 4
    .global __stackstart
    .global __svc_stack_end
    .global __irq_stack_end
    .global __fiq_stack_end
    .global __abort_stack_end
    .global __undef_stack_end
    .global __system_stack_end
__stackstart:
    .space CONFIG_SVC_STACK_LEN
__svc_stack_end:
    .space CONFIG_IRQ_STACK_LEN
__irq_stack_end:
    .space CONFIG_FIQ_STACK_LEN
__fiq_stack_end:
    .space CONFIG_ABORT_STACK_LEN
__abort_stack_end:
    .space CONFIG_UNDEF_STACK_LEN
__undef_stack_end:
    .space CONFIG_SYSTEM_STACK_LEN
__system_stack_end:


