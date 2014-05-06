///@file switches.h

#pragma once

#include <stdint.h>

namespace Switches
{

void init();
void scan();
bool test(uint8_t id);
bool changed(uint8_t id);
bool changed();

} //namespace Switches
