///@file m_edit.h

#pragma once

#include "menu.h"

namespace Menu
{

class EditMode : public Mode
{
public:
    void            init(uint8_t *value,
                         uint8_t x,
                         uint8_t y,
                         uint8_t min,
                         uint8_t max,
                         const char *fmt,
                         const char * const *strings) 
    {
        _value = value;
        _x = x;
        _y = y;
        _min = min;
        _max = max;
        _fmt = fmt;
        _strings = strings;
    }
    virtual void    enter(Mode *from) override;
    virtual Mode    *action(Encoder::Event bp);

private:
    Mode            *_from;
    uint8_t         *_value;
    uint8_t         _x;
    uint8_t         _y;
    uint8_t         _min;
    uint8_t         _max;
    const char      *_fmt;
    const char * const * _strings;

    void            draw();
};

extern EditMode  modeEdit;

} // namespace Menu

