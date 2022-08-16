/**
 *	SPDX-License-Identifier:	GPL-2.0+
 */

#define CONFIG_STACK_LENGTH     0x400

    .section .text
    .align 4
    .global main
    .global _start
_start:
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
    beq bc_done
    b b_loop
bc_done:
    /**
     * set sp pointer
     */
    ldr r0,=stackend
    mov sp,r0

    //msr cpsr_c,#0x52

    /**
     * goto c world
     */
    bl main

    .section .vector
    .global __vectors
__vectors:
    bl _start
    bl default_handler
    bl default_handler
    bl default_handler
    bl default_handler
    bl default_handler
    bl default_handler
    bl default_handler



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


