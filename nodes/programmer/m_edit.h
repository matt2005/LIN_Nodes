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
    void    init(Mode *parent,
                 uint8_t *value,
                 Display::Region r,
                 uint8_t min = 0,
                 uint8_t max = 255,
                 const char *fmt = nullptr)
    {
        _from = parent;
        _value = value;
        _region = r;
        _min = min;
        _max = max;
        _fmt = fmt ? fmt : PSTR("%3u");
        _stringtab = nullptr;

        if ((*_value) < _min) {
            (*_value) = _min;

        } else if ((*_value) > _max) {
            (*_value) = _max;
        }
    }

    void    init(Mode *parent,
                 uint8_t *value,
                 Display::Region r,
                 const char *stringtab,
                 const char *fmt = nullptr)
    {
        _from = parent;
        _value = value;
        _region = r;
        _min = 0;
        _max = 255;
        _fmt = fmt ? fmt : "%3u";
        _stringtab = stringtab;

        if ((*_value) >= Util::strtablen(_stringtab)) {
            (*_value) = Util::strtablen(_stringtab);
        }
    }

    virtual Mode    *action(Encoder::Event bp) override;
    void     draw();

private:
    static Mode            *_from;
    static uint8_t         *_value;
    static Display::Region _region;
    static uint8_t         _min;
    static uint8_t         _max;
    static const char      *_fmt;
    static const char      *_stringtab;

    static void            increment();
    static void            decrement();
};

} // namespace Menu

