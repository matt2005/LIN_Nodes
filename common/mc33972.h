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

} //namespace MC33972
