///@file m_test.h

#pragma once

#include "menu.h"

namespace Menu
{

class TestMode : public Mode
{
public:
    virtual Mode    *action(Encoder::Event bp) override;
private:
};

extern TestMode  modeTest;

class InfoMode : public Mode
{
public:
    virtual Mode    *action(Encoder::Event bp) override;
private:
};

extern InfoMode  modeInfo;

} // namespace Menu

