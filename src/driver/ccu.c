#include "inc/ccu.h"

static inline void pll_ctrl_gate(int module,bool en)
{
    uint32 * t = (((uint32 *)PLL_CLK_GATE_BASE) + module / 32);
    if (en) {
        *t |= 1 << (module % 32);
    } else {
        *t &= ~(1 << (module % 32));
    }
}

static inline void pll_reset(int module)
{
    uint32 * t = (((uint32 *)PLL_CLK_SOFTRESET_BASE) + module / 32);
    *t = 1 << (module % 32);
}

void system_set_clock_default(void)
{
    CLK_SELECT |= (1 << 16);
    /* 400M */
    PLL_CPU_CTRL |= (1 << 31);
    PLL_CPU_CTRL &= ~(0x1f << 8);
    PLL_CPU_CTRL |= (0x1c << 8);
    /* 600M */
    PLL_PERIPH_CTRL |= (1 << 31);

    *((uint32 *)CCU_BASE + 0x100) = 0xffffffff;

    pll_module_init(SDRAM_MODULE);

    PLL_DDR_CTRL = 0x90100c00;

    BUS_DIV_CFG = 0x00003090;
}

void pll_module_init(int module)
{
    pll_ctrl_gate(module,true);
    if (module >= MODULE_OFFSET_MAX)
        pll_reset(module + 64);
    else
        pll_reset(module);
}

int pll_get_apb_clock(void)
{
    return 50000000;
}