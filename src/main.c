#include "inc/gpio.h"
#include "inc/ccu.h"
#include "inc/uart.h"

void en_irq(void) 
{
    asm(
        "mrs r0, cpsr\n"
        "bic r0,r0,#0x80\n"
        "msr cpsr, r0\n"
    );
}
extern void delay();

int main()
{
    uint32 *i = 0;
    struct uart_cfg cfg = {
        .data_len = 8,
        .stop_bits = 0,
        .parity_bits = 0,
        .band_rate = 115200,
    };
    struct gpio_cfg gpio_cfg = {
        .func_cfg = 6,
        .multi_drv =3,
        .pull = 0,
    };

    system_set_clock_default();
    gpio_init(GPIOD,GPIO_PIN_0,&gpio_cfg);

    uart_init(UART2,&cfg);


    for (i = 0x00;i < 0x20;i++) {
        uart_send(UART2,"addr:0x%x -> 0x%x\n",i,*i);
    }

    en_irq();

    *((uint32 *)(0x01C20400 + 0x24)) |= 1 << 6;
    *((uint32 *)(0x01C20800 + 0x200)) |= 0;
    *((uint32 *)(0x01C20800 + 0x210)) |= 1;

    while(true) {
        delay();
        uart_send(UART2,"%x,%x\n",*((uint32 *)(0x01C20400 + 0x14)),*((uint32 *)(0x01C20800 + 0x214)));
    }
    return 0;
}

void do_irq(void)
{
    uart_send(UART2,"irq running\n");
    *((uint32 *)(0x01C20800 + 0x214)) = 0xffffffff;
    *((uint32 *)(0x01C20400 + 0x24)) = 0xffffffff;
}