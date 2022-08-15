#include "inc/gpio.h"

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

bool gpio_set_value(struct gpio_type *gpio,int num,int value) 
{
    if (value) {
        gpio->data |= 1 << num;
    } else {
        gpio->data &= ~(1 << num);
    }
    return true;
}