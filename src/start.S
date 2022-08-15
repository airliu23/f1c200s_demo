/**
 *	SPDX-License-Identifier:	GPL-2.0+
 */


#define STACK_BASE              (0x00010400)        //part of SRAM can be used as stack

#define CCU_BASE                (0x01C20000)        // CCU start address
#define CCU_PLL_STABLE_TIME     (CCU_BASE + 0x200)  
#define CCU_PLL_PERIPH_REG      (CCU_BASE + 0x28)
#define CCU_PLL_AHB_APB_REG     (CCU_BASE + 0x54)


#define GPIO_BASE               (0x01c20800)        // GPIO start address
#define GPIOE_BASE              (GPIO_BASE + 0x24 * 3)  // GPIOE start address
#define GPIOE_CFG0_REG          (GPIOE_BASE + 0x00)
#define GPIOE_DATA_REG          (GPIOE_BASE + 0x10)
#define GPIOE_DRV0_REG          (GPIOE_BASE + 0x14)
#define GPIOE_PUL0_REG          (GPIOE_BASE + 0x1c)

    .section .text
    .align 4
	.global _start
    .global main

_start:
    ldr r0,=stackend
    mov sp,r0

    bl main

    // GPIO init
    ldr r0,= GPIOE_CFG0_REG
    ldr r1,= 0x1111 // all output
    str r1,[r0]

    ldr r0,= GPIOE_DRV0_REG
    ldr r1,= 0xff // all drive level 3
    str r1,[r0]

    ldr r0,= GPIOE_PUL0_REG
    ldr r1,= 0x55 // all pull-up
    str r1,[r0]

    ldr r0,= GPIOE_DATA_REG
    mov r1,#0x0f
    str r1,[r0]
    
loop:
    ldr r0,= GPIOE_DATA_REG
    mov r1,#0x0f
    str r1,[r0]
    bl delay


    ldr r0,= GPIOE_DATA_REG
    mov r1,#0x00
    str r1,[r0]
    bl delay

    b loop

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
    .global stackbase
    .global stackend
stackbase:
    .space 0x400
stackend:


