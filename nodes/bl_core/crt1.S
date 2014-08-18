/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <msmith@purgatory.org> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return.
 * ----------------------------------------------------------------------------
 */

#include <avr/io.h>

#ifdef __AVR_HAVE_JMP_CALL__
  #define XJMP jmp
#else
  #define XJMP rjmp
#endif 

    /* Reset vector in the usual location */

    .section .vectors,"ax",@progbits
    .global __vectors
    .func   __vectors
__vectors:
    XJMP    __init
    .endfunc

    .section .init0,"ax",@progbits
    .weak   __init
__init:
    .section .init0,"ax",@progbits
    clr     r1
    .section .init9,"ax",@progbits
    XJMP    main
