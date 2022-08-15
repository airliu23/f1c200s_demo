/* rom start addr must 0x60 */
MEMORY { .rom : ORIGIN = 0x00060, LENGTH = 32k }
OUTPUT_ARCH(arm)
ENTRY(_start)

SECTIONS
{
    .text :
    {
        . = ALIGN(4);
        src/start.o(.text)
        *(.text*)
    } > .rom
    .rodata :
    {
        *(.rodata*)
    } > .rom
    .data :
    {
        . = ALIGN(4);
        *(.data*)
    } > .rom
    .bss :
    {
        . = ALIGN(4);
        *(.bss*)
        *(.bss.*)
    } > .rom
}