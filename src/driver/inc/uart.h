#ifndef __F1C200S_UART_H__
#define __F1C200S_UART_H__

#include "types.h"
#include "gpio.h"

#define UART_NUMS   3

struct uart_cfg {
    uint8 data_len;
    uint8 stop_bits;
    uint8 parity_bits;
    uint32 band_rate;
};

struct uart_type {
    union {
        uint32 rbr;
        uint32 thr;
        uint32 dll;
    };
    union {
        uint32 dlh;
        uint32 irq_en;
    };
    union {
        uint32 irq_identity;
        uint32 fifo_ctrl;
    };
    uint32 line_ctrl;
    uint32 modem_ctrl;
    uint32 line_status;
    uint32 modem_status;
    uint32 scratch;
};

#define UART0       ((struct uart_type *)0x01c25000)
#define UART1       ((struct uart_type *)0x01c25400)
#define UART2       ((struct uart_type *)0x01c25800)

/* interface */
void uart_init(struct uart_type * uart,const struct uart_cfg *uart_cfg);
bool uart_send_byte(struct uart_type * uart,uint8 data);
void uart_send_bytes(struct uart_type * uart,const uint8 *data);
void uart_send(struct uart_type *uart,const char *fmt,...);


#endif
