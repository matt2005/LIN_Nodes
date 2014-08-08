/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <msmith@purgatory.org> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return.
 * ----------------------------------------------------------------------------
 */

///@file m_edit.h

#pragma once

#include "print.h"
#include "util.h"

#include "menu.h"

namespace Menu
{

class EditMode : public Mode
{
public:
    static void init(Mode *parent, Display::Region r, uint16_t *value)
    {
        _from = parent;
        _region = r;
        _value = value;
    }

    static void set_encoding(uint8_t encoding) { _encoding = encoding; }
    static void draw();

    virtual Mode    *action(Encoder::Event bp) override;

private:
    static Mode             *_from;
    static Display::Region  _region;
    static uint8_t          _encoding;
    static uint16_t         *_value;

    static void            increment();
    static void            decrement();
};

} // namespace Menu

