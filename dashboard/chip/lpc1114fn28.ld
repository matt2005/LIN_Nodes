/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <msmith@purgatory.org> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return.
 * ----------------------------------------------------------------------------
 */

MEMORY {
    flash (rx) : org = 0x00000000, len = 32k
    ram (rwx)  : org = 0x10000000, len = 4k
}

ENTRY(_start)

SECTIONS {
    . = 0;

/*  
    .vectors : ALIGN(16) SUBALIGN(16) {
        KEEP(*(.vectors))
    } > flash
    
    .init_array : ALIGN(4) SUBALIGN(4) {
        PROVIDE(_init_array_start = .);
        KEEP(*(SORT_BY_INIT_PRIORITY(.init_array.*)))
        KEEP(*(.init_array))
        PROVIDE(_init_array_end = .);
    } > flash
*/  
    .text : ALIGN(4) SUBALIGN(4) {
        KEEP(*(.vectors))

        PROVIDE(_init_array_start = .);
        KEEP(*(SORT_BY_INIT_PRIORITY(.init_array.*)))
        KEEP(*(.init_array))
        PROVIDE(_init_array_end = .);

        *(.text.startup.*)
        *(.text)
        *(.text.*)
        *(.rodata)
        *(.rodata.*)
        *(.gcc*)
    } > flash
    
    . = ALIGN(4);
    
    PROVIDE(_data_rom = .);
    
    .data : {
        PROVIDE(_data_start = .);
        *(.data)
        . = ALIGN(4);
        *(.data.*)
        . = ALIGN(4);
        *(.ramtext)
        . = ALIGN(4);
        PROVIDE(_data_end = .);
    } > ram AT > flash
    
    .bss : {
        PROVIDE(_bss_start = .);
        *(.bss)
        . = ALIGN(4);
        *(.bss.*)
        . = ALIGN(4);
        *(COMMON)
        . = ALIGN(4);
        PROVIDE(_bss_end = .);
    } > ram    

    PROVIDE(_stacktop = ORIGIN(ram) + LENGTH(ram) - 16);
}
