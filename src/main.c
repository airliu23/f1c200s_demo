#include "inc/gpio.h"
#include "inc/ccu.h"
#include "inc/uart.h"

extern void en_irq();
extern void delay();
extern int get_sp();
extern uint32 get_cpsr();
extern uint32 data_start;
extern uint32 data_end;
extern uint32 bss_start;
extern uint32 bss_end;
void default_handler(void);

int main()
{
    static uint32 count = 0;
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
    uint32 *i;
    system_set_clock_default();
    gpio_init(GPIOD,GPIO_PIN_0,&gpio_cfg);

    uart_init(UART2,&cfg);


    for (i = 0x00;i < 0x20;i++) {
        *i = 0xea000016;
        uart_send(UART2,"addr:0x%x -> 0x%x\n",i,*i);
    }

    uart_send(UART2,"sp addr:0x%x\n",get_sp());

    en_irq();
    count++;
    
    //set_pc();

    //0x01C20400
    //*((uint32 *)0x01C20404) = 0x10000;
    *((uint32 *)(0x01C20400 + 0x24)) |= 1 << 6;
    *((uint32 *)(0x01C20800 + 0x200)) |= 2;
    *((uint32 *)(0x01C20800 + 0x210)) |= 1;

    i = 10/0;
    

    while(true) {
        delay();
        uart_send(UART2,"cpsr -> %x\n",get_cpsr());
        uart_send(UART2,"%x,%x\n",*((uint32 *)(0x01C20400 + 0x14)),*((uint32 *)(0x01C20800 + 0x214)));
    }
    return 0;
}

void show_reg(uint32 cpsr,uint32 lr,uint32 c1)
{
    uart_send(UART2,"cpsr = %x,lr = %x,c1 = %x\n",cpsr,lr,c1);
    //*((uint32 *)(0x01C20800 + 0x214)) |= 1;
    //*((uint32 *)(0x01C20400 + 0x14)) |= 1 << 6;
}
void default_handler(void)
{
    *((uint32 *)(0x01C20800 + 0x214)) |= 1;
    *((uint32 *)(0x01C20400 + 0x14)) |= 1 << 6;
    uart_send(UART2,"irq pending\n");
}