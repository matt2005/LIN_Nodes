/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <msmith@purgatory.org> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return.
 * ----------------------------------------------------------------------------
 */

///@file switches.h

#pragma once

#include <stdint.h>

namespace MC33972
{
enum Input : uint8_t
{
    kInputSG0   = 0,
    kInputSG1,
    kInputSG2,
    kInputSG3,
    kInputSG4,
    kInputSG5,
    kInputSG6,
    kInputSG7,
    kInputSG8,
    kInputSG9,
    kInputSG10,
    kInputSG11,
    kInputSG12,
    kInputSG13,
    kInputSP0,
    kInputSP1,
    kInputSP2,
    kInputSP3,
    kInputSP4,
    kInputSP5,
    kInputSP6,
    kInputSP7,
    kInputMax
};

void configure();
void scan();
bool test(uint8_t inp);

extern bool changed;

} //namespace MC33972
