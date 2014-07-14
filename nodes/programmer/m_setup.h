///@file m_setup.h

#pragma once

#include "menu.h"

namespace Menu
{

class SetupMode : public Mode
{
public:
    SetupMode() :
        _param(0),
        _value(0),
        _nad(0),
        _max_param(0),
        _editing(false)
    {}

    virtual Mode    *action(Encoder::Event bp) override;

protected:
    virtual void    printTitle() const;
    virtual Parameter param() const;
    virtual PGM_P   paramName() const;
    virtual PGM_P   paramFormat() const;
    
    void            _init(uint8_t nad, uint8_t max_param);

    uint8_t         _param;
    uint8_t         _value;

private:
    uint8_t         _nad;
    uint8_t         _max_param;
    bool            _editing;

    void            draw();
};

class SetupMasterMode : public SetupMode
{
public:
    void            init();

protected:
    virtual void    printTitle() const override;
    virtual Parameter param() const override;
    virtual PGM_P   paramName() const override;
    virtual PGM_P   paramFormat() const override;
};

class SetupPowerMode : public SetupMode
{
public:
    SetupPowerMode() :
        _ident(0),
        _flavour(kFlavourV1)
    {
    }
    void            init(uint8_t nad);

protected:
    virtual void    printTitle() const override;
    virtual Parameter param() const override;
    virtual PGM_P   paramName() const override;
    virtual PGM_P   paramFormat() const override;

private:
    enum Flavour {
        kFlavourV1,
        kFlavourV3
    };

    uint8_t         _ident;
    Flavour         _flavour;

    PGM_P           paramNames() const;
    PGM_P           paramFormats() const;
};

extern SetupMasterMode  modeSetupMaster;
extern SetupPowerMode   modeSetupPower;

} // namespace Menu

