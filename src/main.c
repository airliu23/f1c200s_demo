#include "inc/gpio.h"
#include "inc/ccu.h"
#include "inc/uart.h"

extern void delay();
extern int get_sp();
extern uint32 data_start;
extern uint32 data_end;
extern uint32 bss_start;
extern uint32 bss_end;


int main()
{
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
    int *i;
    system_set_clock_default();
    gpio_init(GPIOD,GPIO_PIN_0,&gpio_cfg);

    uart_init(UART2,&cfg);

    for (i = &bss_start;i < &bss_end;i++) {
        uart_send(UART2,"addr:0x%x -> 0x%x\n",i,*i);
    }

    uart_send(UART2,"sp addr:0x%x\n",get_sp());
    //0x01C20400
    //*((uint32 *)0x01C20404) = 0x10000;
    *((uint32 *)(0x01C20400 + 0x24)) |= 1 << 6;
    *((uint32 *)(0x01C20800 + 0x200)) |= 2;
    *((uint32 *)(0x01C20800 + 0x210)) |= 1;
    uart_send(UART2,"vec -> 0x%x\n",*((uint32 *)0x01C20400));
    uart_send(UART2,"irq -> 0x%x\n",*((uint32 *)0x00));

    while(true) {
        delay();
        uart_send(UART2,"irq -> %x\n",*((uint32 *)(0x01C20400 + 0x14)));
        uart_send(UART2,"%x\n",*((uint32 *)(0x01C20800 + 0x214)));
    }
    return 0;
}

void default_handler(void)
{
    uart_send(UART2,"irq pending\n");
}