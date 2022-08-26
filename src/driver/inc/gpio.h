#ifndef __F1C200S_GPIO_H__
#define __F1C200S_GPIO_H__

#include "types.h"
#include "intc.h"

enum {
    GPIO_PIN_0 = 0,
    GPIO_PIN_1,
    GPIO_PIN_2,
    GPIO_PIN_3,
    GPIO_PIN_4,
    GPIO_PIN_5,
    GPIO_PIN_6,
    GPIO_PIN_7,
    GPIO_PIN_8,
    GPIO_PIN_9,
    GPIO_PIN_10,
    GPIO_PIN_11,
    GPIO_PIN_12,
    GPIO_PIN_13,
    GPIO_PIN_14,
    GPIO_PIN_15,
    GPIO_PIN_16,
    GPIO_PIN_17,
    GPIO_PIN_18,
    GPIO_PIN_19,
    GPIO_PIN_20,
    GPIO_PIN_21,
    GPIO_PIN_MAX,
};

struct gpio_cfg {
    uint8 func_cfg  : 3;
    uint8 multi_drv : 2;
    uint8 pull      : 2;
};

struct gpio_ext_cfg {
    uint8 ext_mode;
    intc_func func;
};

struct gpio_type {
    __IO uint32 cfg[4];
    __IO uint32 data;
    __IO uint32 drv[2];
    __IO uint32 pull[2];
};

struct gpio_ext_type {
    __IO uint32 ext_cfg[4];
    __IO uint32 ext_en;
    __IO uint32 ext_pending;
    __IO uint32 ext_debounce;
};

#define GPIOA           ((struct gpio_type *)(0x01C20800 + 0 * 0x24))
#define GPIOB           ((struct gpio_type *)(0x01C20800 + 1 * 0x24))
#define GPIOC           ((struct gpio_type *)(0x01C20800 + 2 * 0x24))
#define GPIOD           ((struct gpio_type *)(0x01C20800 + 3 * 0x24))
#define GPIOE           ((struct gpio_type *)(0x01C20800 + 4 * 0x24))
#define GPIOF           ((struct gpio_type *)(0x01C20800 + 5 * 0x24))

#define GPIOD_EXT       ((struct gpio_ext_type *)(0x01C20800 + 0x200))
#define GPIOE_EXT       ((struct gpio_ext_type *)(0x01C20800 + 1 * 0x20 + 0x200))
#define GPIOF_EXT       ((struct gpio_ext_type *)(0x01C20800 + 2 * 0x20 + 0x200))

/* interface */
bool gpio_init(struct gpio_type *gpio,int num,struct gpio_cfg *cfg);
bool gpio_ext_init(struct gpio_type *gpio,int num,struct gpio_ext_cfg *cfg);
bool gpio_set_value(struct gpio_type *gpio,int num,int value);

#endif