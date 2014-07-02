///@file mc17xsf500.h

#pragma once

#include <stdint.h>

namespace MC17XSF500
{

void configure();
void set(uint8_t channel, uint8_t duty_cycle);

}
