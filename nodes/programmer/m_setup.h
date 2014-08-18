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
    void            init(uint8_t nad);

private:
    enum Flavour : uint8_t {
        kFlavourMaster,
        kFlavourPowerV1,
        kFlavourPowerV3
    };

    uint8_t         _nad;
    Flavour         _flavour;

    uint8_t         _index;
    Parameter       _param;
    uint16_t        _value;

    bool            _editing;
    bool            _readError;

    void            draw();

    void            print_title();
    uint8_t         param_encoding(Parameter::Address addr);
    const char      *param_name(Parameter::Address addr);
    Parameter       param(uint8_t index);
    bool            adjust_value(int16_t offset);
};

} // namespace Menu

