///@file m_setup.h

#pragma once

#include "menu.h"

namespace Menu
{

class SetupMode : public Mode
{
public:

    virtual Mode    *action(Encoder::Event bp) override;

protected:
    virtual void    print_title() const;
    virtual Parameter param() const;
    virtual PGM_P   param_name() const;
    virtual PGM_P   param_format() const;

    static void     _init(uint8_t nad, uint8_t max_param);

    static uint8_t  _param;
    static uint8_t  _value;
    static uint8_t  _nad;

private:
    static uint8_t  _max_param;
    static bool     _editing;

    void            draw();
};

class SetupMasterMode : public SetupMode
{
public:
    static void     init();

protected:
    virtual void    print_title() const override;
    virtual Parameter param() const override;
    virtual PGM_P   param_name() const override;
    virtual PGM_P   param_format() const override;
};

class SetupPowerMode : public SetupMode
{
public:
    static void     init(uint8_t nad);

protected:
    virtual void    print_title() const override;
    virtual Parameter param() const override;
    virtual PGM_P   param_name() const override;
    virtual PGM_P   param_format() const override;

private:
    enum Flavour : uint8_t {
        kFlavourV1,
        kFlavourV3
    };

    static Flavour  _flavour;

    static uint8_t  ident() { return _nad - LIN::kNodeAddressPowerBase; }
    static PGM_P    param_names();
    static PGM_P    param_formats();
};

} // namespace Menu

