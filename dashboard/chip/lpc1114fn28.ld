/*
 * Copyright (c) 2012-2015, Mike Smith, <msmith@purgatory.org>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 * o Redistributions of source code must retain the above copyright notice,
 *   this list of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright
 *   notice, this list of conditions and the following disclaimer in the
 *   documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
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
