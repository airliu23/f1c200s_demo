/* rom start addr must 0x60 */
MEMORY { .rom : ORIGIN = 0x00060, LENGTH = 32k }
OUTPUT_ARCH(arm)
ENTRY(__start)
/**
 * 链接文件中表示的都是地址。例如：
 * bss_start = .;  表示bss_start变量的存放地址为当前地址
 */
SECTIONS
{
    .text :
    {
        . = ALIGN(4);
        *(.vectors*)
        *(.text*)
    } > .rom
    .rodata :
    {
        *(.rodata*)
    } > .rom
    .data :
    {
        . = ALIGN(4);
        data_start = .;
        *(.data*)
        data_end = .;
    } > .rom
    .bss :
    {
        . = ALIGN(4);
        bss_start = .;
        *(.bss*)
        *(.bss.*)
        bss_end = .;
    } > .rom
}