#include <avr/pgmspace.h>

#include "board.h"
#include "lin_protocol.h"
#include "mc33972.h"

#include "menu.h"
#include "master.h"
#include "display.h"
#include "switches.h"

namespace Menu
{

class Mode
{
public:
    /// Called when the mode is activated.
    ///
    virtual void    enter(Mode *from) = 0;

    /// Called to give the mode CPU cycles
    Mode            *tick() {
        Display::Button bp = gDisplay.getButtonPress();
        return action(bp);
    }

protected:
    /// Kick the mode state machine.
    ///
    /// @param disp     The display the mode is using.
    /// @param bp       The oldest un-handled button press; may be
    ///                 Display::kButtonNone if no button press has
    ///                 been received.
    /// @return         The mode that is current following processing of
    ///                 this call; should return this if no mode change.
    ///
    virtual Mode    *action(Display::Button bp) = 0;

};

class TopMode : public Mode
{
public:
    virtual void    enter(Mode *from) override;
    virtual Mode    *action(Display::Button bp);
private:
    uint8_t         _index;
    void            draw();
};

class ExploreMode : public Mode
{
public:
    virtual void    enter(Mode *from) override;
    virtual Mode    *action(Display::Button bp);

private:
    uint8_t         _node;
    bool            _present;
    void            check();
};

class ParameterMode : public Mode
{
public:
    virtual void    enter(Mode *from) override;
    virtual Mode    *action(Display::Button bp);

private:
    enum SubMode : uint8_t {
        // enum values are also marker column values
        SM_NODE     = 0,            ///< editing parameter node 
        SM_PARAM    = 5,            ///< editing parameter ID
        SM_VALUE    = 10,           ///< editing parameter value
    };

    uint16_t        _value;         ///< value of the current parameter
    uint8_t         _node;          ///< address of the current node
    uint8_t         _param;         ///< index of the current parameter
    SubMode         _submode;       ///< current submode

    bool            _changed:1;     ///< true if the parameter has been changed
    bool            _present:1;     ///< true if the node is present

    /// Set a new node address. Resets the current parameter ID to zero
    /// and attempts to load it from the node. Updates _present, _valid, _writable 
    /// and _value, clears _changed and discards any previously changed parameter.
    ///
    /// @param node     The new node address.
    ///
    void            setNode(uint8_t node);

    /// Set a new parameter ID and attempts to load it from the node. Updates
    /// _valid, _writable and _value, clears _changed and discards any previously 
    /// changed parameter.
    ///
    /// @param param    The new parameter ID.
    /// @return         True if the node responds to the parameter load.
    ///
    bool            setParam(uint8_t param);

    /// Set a new parameter value.
    ///
    /// @param value    new value for the parameter
    ///
    void            setValue(uint16_t value);

    /// Write the parameter back to the node
    void            saveValue();

    /// Move to the previous node
    void            prevNode() { if (_node > 0) setNode(_node - 1); }

    /// Move to the next node
    void            nextNode() { if (_node < Parameter::maxIndex) setNode(_node + 1); }

    /// Move to the previous parameter
    void            prevParam() { if (_param > 0) setParam(_param - 1); }

    /// Move to the next parameter
    void            nextParam() { if (_param < 255) setParam(_param + 1); }

    /// Send the parameter to the node.
    ///
    void            save();

    /// Load the parameter from the node. Updates _valid, _writable and _value,
    /// clears _changed and discards any previously changed parameter.
    ///
    /// @return         True if the node responds to the request.
    ///
    bool            load();

    /// Draw the edit display
    ///
    void            draw();
};

class SwitchMode : public Mode
{
public:
    virtual void    enter(Mode *from) override;
    virtual Mode    *action(Display::Button bp);

private:
    void            draw();
};

static TopMode      _modeTop;
static ExploreMode  _modeExplore;
static ParameterMode _modeParameter;
static SwitchMode   _modeSwitches;

static Mode         *_mode;     ///< current mode

////////////////////////////////////////////////////////////////////////////////
// Toplevel menu engine
//
void
tick()
{
    // lazily init to idle mode
    if (_mode == nullptr) {
        _mode = &_modeTop;
        _mode->enter(nullptr);
    }

    // kick the mode state machine
    Mode *newmode = _mode->tick();

    // mode change?
    if (newmode != _mode) {
        newmode->enter(_mode);
        _mode = newmode;
    }
}

////////////////////////////////////////////////////////////////////////////////
// Top menu mode
//

void
TopMode::enter(Mode *from)
{
    _index = 0;
    draw();    
}

Mode *
TopMode::action(Display::Button bp)
{
    bool wantDraw = false;
    switch (bp) {

    case Display::kButtonDown:
        if (_index < 3) {
            _index++;
            wantDraw = true;
        }
        break;
    case Display::kButtonUp:
        if (_index > 0) {
            _index--;
            wantDraw = true;
        }
        break;
    case Display::kButtonEnter:
        switch (_index) {
        case 0:
            return &_modeParameter;
        case 1:
            return &_modeExplore;
        case 2:
            return &_modeSwitches;
        default:
            // ... oops
            _index = 0;
            wantDraw = true;
            break;
        }
        break;
    default:
        break;
    }

    if (wantDraw) {
        draw();
    }

    return this;
}

void
TopMode::draw()
{
    const char *txt;

    switch (_index) {
    default:
        _index = 0;
        // FALLTHROUGH
    case 0:
        txt = PSTR("System Setup");
        break;
    case 1:
        txt = PSTR("Explore Network");
        break;
    case 2:
        txt = PSTR("Switch Test");
        break;
    case 3:
        txt = PSTR("Relay Test");
        break;
    }

    gDisplay.clear();
    gDisplay.printf(txt);
#ifdef DEBUG
    gDisplay.move(8, 1);
    gDisplay.printf(PSTR("free %3u"), Board::freemem());
#endif
}

////////////////////////////////////////////////////////////////////////////////
// Explore mode
//

void
ExploreMode::enter(Mode *from)
{
    gDisplay.clear();
    _node = LIN::kNADMaster;
    check();
}

Mode *
ExploreMode::action(Display::Button bp)
{
    switch (bp) {
    case Display::kButtonCancel:
        return &_modeTop;

    case Display::kButtonDown:
        if (_node > LIN::kNADMaster) {
            _node--;
            check();
        }
        break;

    case Display::kButtonUp:
        if (_node < (LIN::kNADMaster + 15)) {
            _node++;
            check();
        }
        break;

    default:
        break;
    }

    return this;
}

void
ExploreMode::check()
{
    gDisplay.clear();
    gDisplay.move(0, 0);

    LIN::Frame f = LIN::Frame::makeReadByIDRequest(_node, LIN::kRBIProductID);
    if (!gMaster.doRequestResponse(f)) {
        _present = false;
        gDisplay.printf(PSTR("%u missing"), _node);
    } else {
        _present = true;
        gDisplay.printf(PSTR("%u found"), _node);
    }
}

////////////////////////////////////////////////////////////////////////////////
// Parameter mode
//

void
ParameterMode::enter(Mode *from)
{
    _submode = SM_NODE;
    setNode(0);
    gDisplay.clear();
    gDisplay.printf(PSTR("Node Parm Value"));
    draw();
}

Mode *
ParameterMode::action(Display::Button bp)
{
    switch (bp) {

    case Display::kButtonCancel:
        return &_modeTop;

    case Display::kButtonEnter:
        saveValue();
        break;

    case Display::kButtonDown:
        switch (_submode) {
        case SM_NODE:
            prevNode();
            break;
        case SM_PARAM:
            prevParam();
            break;
        case SM_VALUE:
            setValue(_value - 1);
            break;
        }
        break;

    case Display::kButtonUp:
        switch (_submode) {
        case SM_NODE:
            nextNode();
            break;
        case SM_PARAM:
            nextParam();
            break;
        case SM_VALUE:
            setValue(_value + 1);
            break;
        }
        break;

    case Display::kButtonLeft:
        switch (_submode) {
        case SM_PARAM:
            _submode = SM_NODE;
            break;
        case SM_VALUE:
            _submode = SM_PARAM;
            break;
        default:
            break;
        }
        break;

    case Display::kButtonRight:
        switch (_submode) {
        case SM_NODE:
            if (_present) {
                _submode = SM_PARAM;
            }
            break;
        case SM_PARAM:
            _submode = SM_VALUE;
            break;
        default:
            break;
        }
        break;

    case Display::kButtonNone:
    default:
        return this;        // do nothing
    }

    // update the display to handle changes
    draw();

    // no mode change
    return this;
}

void
ParameterMode::setNode(uint8_t node)
{
    // parameter 0 should always be valid
    _node = node;
    _present = setParam(0);
}

bool
ParameterMode::setParam(uint8_t param)
{
    _param = param;
    return load();
}

void
ParameterMode::setValue(uint16_t value)
{
    _value = value;
    _changed = true; 
}

void
ParameterMode::saveValue()
{
    if (_present && _changed) {
        // send the parameter to the node...
        save();

        // read it back, clears _changed on success
        load();
    }
}

bool
ParameterMode::load()
{
//    LIN::DataDumpRequest f(_node,
//                          LIN::kDataDumpGetParam,
//                          _param);

//    //if (!gMaster.doRequestResponse(f)) {
//    //    return false;
//    //}

//    _changed = false;
//    _value = 2 * _node;
    //_value = f.d2();
    //_value = (_value << 8) | f.d1();

    return true;
}

void
ParameterMode::save()
{
//    LIN::DataDumpRequest f(_node,
//                           LIN::kDataDumpSetParam,
//                           _param,
//                           _value & 0xff,
//                           _value >> 8);
//
    //gMaster.doRequest(f);
}

void
ParameterMode::draw()
{
    // clear edit line
    gDisplay.move(0, 1);
    gDisplay.printf(PSTR("                "));     // XXX inefficient

    // mode marker
    gDisplay.move((uint8_t)_submode, 1);
    gDisplay.printf(PSTR(">"));

    // values
    gDisplay.move(1, 1);
    gDisplay.printf(PSTR("%u"), _node);
    if (_present) {
        gDisplay.move(6, 1);

        gDisplay.printf(PSTR("%u %u"), _param, _value);
    }

    // unsaved marker
    gDisplay.move(14, 1);
    gDisplay.printf(_changed ? PSTR("*") : PSTR(" "));
}

////////////////////////////////////////////////////////////////////////////////
// Switch test
//
void
SwitchMode::enter(Mode *from)
{
    gDisplay.clear();
    draw();    
}

Mode *
SwitchMode::action(Display::Button bp)
{

    if (bp == Display::kButtonCancel) {
        return &_modeTop;
    }

    MC33972::scan();

    if (MC33972::changed) {
        draw();
    }

    return this;
}

void
SwitchMode::draw()
{
    gDisplay.move(0, 0);
    gDisplay.printf(PSTR("P "));
    char c = '0';
    for (uint8_t i = MC33972::kInputSP0; i <= MC33972::kInputSP7; i++) {
        if (MC33972::test(i)) {
            gDisplay.putc(c);
        } else {
            gDisplay.putc(' ');
        }
        c++;
    }
    gDisplay.move(0, 1);
    gDisplay.printf(PSTR("G "));
    c = '0';
    for (uint8_t i = MC33972::kInputSG0; i <= MC33972::kInputSG13; i++) {
        if (MC33972::test(i)) {
            gDisplay.putc(c);
        } else {
            gDisplay.putc(' ');
        }
        c++;
    }
}

} // namespace Menu
