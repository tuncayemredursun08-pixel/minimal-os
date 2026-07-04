/* Linker script for minimal OS kernel */

ENTRY(start)

SECTIONS
{
    /* Start at 1MB (standard kernel load address) */
    . = 1M;

    /* Kernel code section */
    .text :
    {
        *(.multiboot)
        *(.text)
        *(.rodata)
    }

    /* Kernel data section */
    .data :
    {
        *(.data)
    }

    /* BSS section (uninitialized data) */
    .bss :
    {
        *(COMMON)
        *(.bss)
    }
}
