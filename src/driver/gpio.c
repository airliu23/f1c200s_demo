#include "inc/gpio.h"
#include "inc/uart.h"

static intc_func intc_funcs[GPIO_PIN_MAX * 3];

bool gpio_init(struct gpio_type *gpio,int num,struct gpio_cfg *cfg)
{
    if (gpio == NULL || cfg == NULL)
        return false;
    // clear bits
    gpio->cfg[(num * 4 / 32)] &= ~(0xf << (num * 4));
    gpio->drv[num / 16] &= ~(0x3 << (num % 16));
    gpio->pull[num / 16] &= ~(0x3 << (num % 16));

    // set bit
    gpio->cfg[(num * 4 / 32)] |= (cfg->func_cfg << (num * 4));
    gpio->drv[num / 16] |= (cfg->multi_drv << (num % 16));
    gpio->pull[num / 16] |= (cfg->pull << (num % 16));    
    return true;
}

void do_gpio_irq(void);
bool gpio_ext_init(struct gpio_type *gpio,int num,struct gpio_ext_cfg *cfg)
{
    struct gpio_cfg ext_cfg = {
        .func_cfg = 0x06,
        .multi_drv = 3,
        .pull = 0,
    };
    struct gpio_ext_type *gpio_ext;
    if (cfg->func == NULL)
        return false;
    gpio_init(gpio,num,&ext_cfg);

    if (gpio == GPIOD) {
        gpio_ext = GPIOD_EXT;
        intc_funcs[num] = cfg->func;
    } else if (gpio == GPIOE) {
        gpio_ext = GPIOE_EXT;
        intc_funcs[GPIO_PIN_MAX + num] = cfg->func;
    } else if (gpio == GPIOF) {
        gpio_ext = GPIOF_EXT;
        intc_funcs[2 * GPIO_PIN_MAX + num] = cfg->func;
    }

    /* init ext */
    gpio_ext->ext_cfg[num / 8] |= cfg->ext_mode << ((num % 8) * 4);
    gpio_ext->ext_en |= 1 << num;

    intc_enable(INTC_PIOD,1,do_gpio_irq);

    return true;
}

bool gpio_set_value(struct gpio_type *gpio,int num,int value) 
{
    if (value) {
        gpio->data |= 1 << num;
    } else {
        gpio->data &= ~(1 << num);
    }
    return true;
}

static void __do_gpio_irq(struct gpio_ext_type *ext,int num)
{
    uint32 pending,index;
    pending = ext->ext_pending;
    index = 0;
    while (pending) {
        if (pending & 0x01) {
            intc_funcs[index + num * GPIO_PIN_MAX]();
            ext->ext_pending |= 1 << index;
        }
        index++;
        pending >>= 1;
    }
}
void do_gpio_irq(void)
{
    struct gpio_ext_type *ext = GPIOD_EXT;
    uint32 index;
    for (index = 0;index < 3;index++) {
        __do_gpio_irq(ext,index);
        ext = (struct gpio_ext_type *)(((uint8 *)ext) + 0x20);
    }
}