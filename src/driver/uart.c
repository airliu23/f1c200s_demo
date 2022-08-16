#include "inc/uart.h"
#include "inc/ccu.h"
#include "stdarg.h"

uint8 hex_num[] = {'0','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f'};

struct uart_pin_cfg{
    uint8 rx;
    uint8 rx_func_cfg;
    struct gpio_type *gpio_rx;
    uint8 tx;
    uint8 tx_func_cfg;
     struct gpio_type *gpio_tx;
} uart_pin[3] = {
    {},
    {},
    {
        .rx = GPIO_PIN_8,
        .rx_func_cfg = 3,
        .gpio_rx = GPIOE,
        .tx = GPIO_PIN_7,
        .tx_func_cfg = 3,
        .gpio_tx = GPIOE,
    },
};

void uart_init(struct uart_type * uart,const struct uart_cfg *uart_cfg)
{
    struct gpio_cfg cfg;
    int index = -1;
    int module = -1;
    if (uart == UART0) {
        index = 0;
        module = UART0_MODULE;
    } else if (uart == UART1) {
        index = 1;
        module = UART1_MODULE;
    } else if (uart == UART2) {
        index = 2;
        module = UART2_MODULE;
    } else {
        return;
    }
    pll_module_init(module);
    /* rx gpio init */
    cfg.func_cfg = uart_pin[index].rx_func_cfg;
    cfg.multi_drv = 3;
    cfg.pull = 1;
    gpio_init(uart_pin[index].gpio_rx,uart_pin[index].rx,&cfg);
    /* tx gpio init */
    cfg.func_cfg = uart_pin[index].tx_func_cfg;
    gpio_init(uart_pin[index].gpio_tx,uart_pin[index].tx,&cfg);

    uart->irq_en = 0x00;  // disable all interrupt
    uart->modem_ctrl = 0x00;         // reset Modem

    // set divisor of buadrate
    // Divisor Latch : [UART_DLH_REG:UART_DLL_REG]
    // formula : Divisor Latch = APBCLK/(buadrate*16)
    uart->line_ctrl |= (1<<7);      // set DLAB to 1
    uart->dll = (pll_get_apb_clock()/(uart_cfg->band_rate*16)) & 0xff;
    uart->dlh = (pll_get_apb_clock()/(uart_cfg->band_rate*16)) >> 8;
    uart->line_ctrl &= ~(1<<7);     // set DLAB to 0

    uart->line_ctrl &= ~0x2f;       // reset parity,stopbit,databit
    // data bits
    uart->line_ctrl |=  (uart_cfg->data_len - 5);
    // stopbit
    uart->line_ctrl |= (uart_cfg->stop_bits) << 2;
    // parity
    uart->line_ctrl |= (uart_cfg->parity_bits) << 3;
}

bool uart_send_byte(struct uart_type *uart,uint8 data)
{
    if (!uart)  return false;
    while(!(uart->line_status & (1 << 5)));
    uart->thr = data;
    return true;
}

void uart_send_bytes(struct uart_type *uart,const uint8 *data)
{
    while(*data) {
        if (!uart_send_byte(uart,*data))
            return;
        data++;
    }
}

void uart_send_num(struct uart_type *uart,int num,int decimal,int8 len)
{
    int data = 0;
    if (num == 0 && len <= 0) {
        return;
    }
    data = num % decimal;
    num = num / decimal;
    len = len - 1;
    uart_send_num(uart,num,decimal,len);
    switch (decimal){
    case 16:
        uart_send_byte(uart,hex_num[data]);
        break;
    case 10:
    case 8:
        uart_send_byte(uart,data + 0x30);
        break;
    default:
        break;
    }
}

void uart_send(struct uart_type *uart,const char *fmt,...)
{
    bool format = false;
    va_list args;
    va_start(args,fmt);
    while(*fmt) {
        if (*fmt == '%') {
            format = true;
            goto inc_data;
        }
        if (format) {
            format = false;
            switch (*fmt) {
            case 'd':
                uart_send_num(uart,va_arg(args,int),10,1);
                break;
            case 'x':
                uart_send_num(uart,va_arg(args,int),16,1);
                break;
            case 's':
                uart_send_bytes(uart,va_arg(args,char *));
                break;
            case 'c':
                uart_send_byte(uart,va_arg(args,int));
                break;
            default:
                break;
            }
            goto inc_data;
        }
        uart_send_byte(uart,*fmt);
inc_data:
        fmt++;
    }
    va_end(args);
}