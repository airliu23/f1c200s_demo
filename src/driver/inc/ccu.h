#ifndef __F1C200S_CCU_H__
#define __F1C200S_CCU_H__

#include "types.h"

#define CCU_BASE                0x01C20000

/* PPL CTRL */
#define PLL_CPU_CTRL            (*((uint32 *)(CCU_BASE + 0x0000)))
#define PLL_AUDIO_CTRL          (*((uint32 *)(CCU_BASE + 0x0008)))
#define PLL_VIDEO_CTRL          (*((uint32 *)(CCU_BASE + 0x0010)))
#define PLL_VE_CTRL             (*((uint32 *)(CCU_BASE + 0x0018)))
#define PLL_DDR_CTRL            (*((uint32 *)(CCU_BASE + 0x0020)))
#define PLL_PERIPH_CTRL         (*((uint32 *)(CCU_BASE + 0x0028)))

#define CLK_SELECT              (*((uint32 *)(CCU_BASE + 0x0050)))
#define BUS_DIV_CFG             (*((uint32 *)(CCU_BASE + 0x0054)))


enum {
    DMA_MODULE = 6,
    SD0_MODULE = 8,
    SD1_MODULE,
    SDRAM_MODULE = 14,
    SPIO_MODULE = 21,
    USB_OTG_MODULE = 24,
    VA_MODULE = 32,
    LCD_MODULE = 36,
    DEINTRLACE_MODULE,
    CSI_MODULE = 40,
    TVD_MODULE,
    TVE_MODULE,
    DEBE_MODULE = 44,
    DEFE_MODULE = 46,
    MODULE_OFFSET_MAX = 64,
    AUDIO_CODEC_MODULE = 64,
    OWA_MODULE,
    CIR_MODULE,
    RSB_MODULE,
    DAUDIO_MODULE = 76,
    TWI0_MODULE = 80,
    TWI1_MODULE,
    TWI2_MODULE,
    UART0_MODULE = 84,
    UART1_MODULE,
    UART2_MODULE,
};

#define PLL_CLK_GATE_BASE       (CCU_BASE + 0x0060)
#define PLL_CLK_SOFTRESET_BASE  (CCU_BASE + 0x02C0)

#define PLL_STABLE_TIME         (*((uint32 *)(CCU_BASE + 0x0200)))


/* set default clock : 408MHz */
void system_set_clock_default(void);
void pll_module_init(int module);
int pll_get_apb_clock(void);

#endif
