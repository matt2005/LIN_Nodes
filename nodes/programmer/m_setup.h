/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <msmith@purgatory.org> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return.
 * ----------------------------------------------------------------------------
 */

///@file m_setup.h

#pragma once

#include "util.h"

#include "menu.h"

namespace Menu
{

class SetupMode : public Mode
{
public:

    virtual Mode    *action(Encoder::Event bp) override;
    static void     init(uint8_t nad);

private:
    enum Flavour : uint8_t {
        kFlavourMaster,
        kFlavourPowerV1,
        kFlavourPowerV3
    };

    static uint8_t  _nad;
    static uint8_t  _index;
    static uint16_t _value;

    static Flavour  _flavour;
    static bool     _editing;
    static bool     _readError;

    void            draw();

    static void     print_title();
    static Parameter param(uint8_t index);
};

} // namespace Menu

