#ifndef __F1C200S_INTC_H__
#define __F1C200S_INTC_H__

#include "types.h"

enum {
    INTC_NMI = 0,
    INTC_UART0,
    INTC_UART1,
    INTC_UART2,
    INTC_OWA = 5,
    INTC_CIR,
    INTC_TWI0,
    INTC_TWI1,
    INTC_TWI2,
    INTC_SPI0,
    INTC_SPI1,
    INTC_TIMER0 = 13,
    INTC_TIMER1,
    INTC_TIMER2,
    INTC_WATCHDOG,
    INTC_RSB,
    INTC_DMA,
    INTC_TOUCH_PANEL = 20,
    INTC_AUDIO_CODEC,
    INTC_KEYADC,
    INTC_SDC0,
    INTC_SDC1,
    INTC_USB_OTG = 26,
    INTC_TVE,INTC_TCON,
    INTC_DE_FE,
    INTC_DE_BE,
    INTC_CSI,
    INTC_DE_INTERLACER,
    INTC_VE,
    INTC_DAUDIO,
    INTC_PIOD = 38,
    INTC_PIOE,
    INTC_PIOF,
    INTC_MAX,
};

enum priority_level {
    PRIO_LEVEL_0 = 0,
    PRIO_LEVEL_1,
    PRIO_LEVEL_2,
    PRIO_LEVEL_3,
};

#define INTC_BASE_ADDR      0x01C20400

struct intc_type {
    __IO uint32 vector;
    __IO uint32 base_addr;
    __IO uint32 unused0;
    __IO uint32 ctrl;
    __IO uint32 pending[2];
    __IO uint32 unused1[2];
    __IO uint32 en[2];
    __IO uint32 unused2[2];
    __IO uint32 mask[2];
    __IO uint32 unused3[2];
    __IO uint32 response[2];
    __IO uint32 unused4[2];
    __IO uint32 fast_force[2];
    __IO uint32 unused5[2];
    __IO uint32 src_priority[4];
};

#define INTC        ((struct intc_type *)INTC_BASE_ADDR)

typedef void (*intc_func)(void);

void intc_init(void);
void intc_enable(uint8 module,uint8 en,intc_func func);

#endif
