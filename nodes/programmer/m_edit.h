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
    EditMode() :
        _from(nullptr),
        _value(nullptr),
        _region(Display::Position(0, 0), Display::Dimension(0, 0)),
        _min(0),
        _max(0),
        _fmt(nullptr),
        _stringtab(nullptr)
    {}

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
    void            draw();

private:
    Mode            *_from;
    uint8_t         *_value;
    Display::Region _region;
    uint8_t         _min;
    uint8_t         _max;
    const char      *_fmt;
    const char      *_stringtab;

    void            increment();
    void            decrement();
};

} // namespace Menu

