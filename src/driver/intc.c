#include "inc/intc.h"
#include "inc/uart.h"

static intc_func intc_funcs[INTC_MAX];

static void default_handler(void)
{
    
}

static void __intc_mask(uint8 module,uint8 mask)
{
    if (mask) {
        INTC->mask[module / 32] |= mask << (module % 32);
    } else {
        INTC->mask[module / 32] &= ~(mask << (module % 32));
    }
}
static void __intc_enable(uint8 module,uint8 en)
{
    if (en) {
        INTC->en[module / 32] |= en << (module % 32);
    } else {
        INTC->en[module / 32] &= ~(en << (module % 32));
    }
}
static void __intc_set_priority(uint8 module,uint8 level)
{
    
}
void intc_enable(uint8 module,uint8 en,intc_func func)
{
    __intc_mask(module,!en);
    __intc_enable(module,en);
    intc_funcs[module] = func;
    uart_send(UART2,"%d\n",module);
}

void intc_init(void)
{
    uint32 i = 0;
    for (i = 0;i < INTC_MAX;i++) {
        intc_funcs[i] = default_handler;
    }
}

/**
 * irq run function
 */
void do_irq(void)
{
    uint32 pending,i,index;
    for (i = 0;i < 2;i++) {
        pending = INTC->pending[i];
        index = 0;
        while(pending) {
            if (pending & 0x1) {
                // run handler
                intc_funcs[index + i *32]();
                // clear pending
                INTC->pending[i] = 1 << index;
            }
            index++;
            pending >>= 1;
        }
    }
}

