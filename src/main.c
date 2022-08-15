#include "inc/gpio.h"
#include "inc/ccu.h"
#include "inc/uart.h"

extern void delay();

int main()
{
    struct uart_cfg cfg = {
        .data_len = 8,
        .stop_bits = 0,
        .parity_bits = 0,
        .band_rate = 115200,
    };
    system_set_clock_default();

    uart_init(UART2,&cfg);

    while(1) {
        delay();
        delay();
        uart_send(UART2,"hello world -> 0x%x\n",0xaa);
        //uart_send_byte(UART2,0xaa);
    }

    return 0;
}