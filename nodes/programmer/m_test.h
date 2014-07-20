///@file m_test.h

#pragma once

#include "timer.h"

#include "menu.h"

namespace Menu
{

class TestMode : public Mode
{
public:
    virtual Mode    *action(Encoder::Event bp) override;

    void            init(uint8_t nad) { _nad = nad; }

private:
    uint8_t         _nad;
    uint8_t         _index;
    Timestamp       _refreshed;

    void            draw();
};

} // namespace Menu

