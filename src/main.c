#include "inc/gpio.h"
#include "inc/ccu.h"
#include "inc/uart.h"
#include "inc/intc.h"

void en_irq(void) 
{
    asm(
        "mrs r0, cpsr\n"
        "bic r0,r0,#0x80\n"
        "msr cpsr, r0\n"
    );
}
extern void delay();
extern void gpio_intc(void);

int main()
{
    uint32 *i = 0;
    struct uart_cfg cfg = {
        .data_len = 8,
        .stop_bits = 0,
        .parity_bits = 0,
        .band_rate = 115200,
    };
    struct gpio_ext_cfg ext_cfg = {
        .ext_mode = 0,
        .func = gpio_intc,
    };

    system_set_clock_default();
    //gpio_init(GPIOD,GPIO_PIN_0,&gpio_cfg);
    intc_init();

    uart_init(UART2,&cfg);
    //gpio_ext_init(GPIOD,GPIO_PIN_0,&ext_cfg);

    *((uint32 *)0x01C20824)=0x00007222;
    *((uint32 *)0x01C20AC0)=0x00000AAA;
    *((uint32 *)0x01C20020)=0x90100C00;
    *((uint32 *)0x01C20060)=0x01004000;
    *((uint32 *)0x01C202C0)=0x01000000;
    *((uint32 *)0x01C202C0)=0x01004000;
    *((uint32 *)0x01C20AC4)=0x00010501;

    *((uint32 *)0x01c01004) = 0xB7CEEDC2;
    *((uint32 *)0x01c01008) = 0x00A70008;
    *((uint32 *)0x01C01000) = 0x0001318A;
    *((uint32 *)0x01C0100C) = 0x000800c8;
    *((uint32 *)0x01C0100C) = 0x000800c9;
    *((uint32 *)0x01C01024) = 0x00161600;
    *((uint32 *)0x01C01010) = 0x000004C0;

    
    for (i = 0x80000000;i < (uint32 *)0x90000000;i++) {
        *i = 0x55555555;
        uart_send(UART2,"addr:0x%x -> 0x%x\n",i,*i);
    }

    en_irq();

    //intc_enable(INTC_PIOD,1,gpio_intc);
    //*((uint32 *)(0x01C20400 + 0x24)) |= 1 << 6;
    //*((uint32 *)(0x01C20800 + 0x200)) |= 0;
    //*((uint32 *)(0x01C20800 + 0x210)) |= 1;
    

    while(true) {
        delay();
        uart_send(UART2,"uart recv -> %x",UART2->line_status);
    }
    return 0;
}

void gpio_intc(void)
{
    uart_send(UART2,"%s running\n",__func__);
    //*((uint32 *)(0x01C20800 + 0x214)) = 0xffffffff;
}