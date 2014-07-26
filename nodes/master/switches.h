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

namespace Switches
{

void init();
void scan();
bool test(uint8_t id);
bool changed(uint8_t id);
bool changed_to_on(uint8_t id);
bool changed_to_off(uint8_t id);
bool changed();

} //namespace Switches
