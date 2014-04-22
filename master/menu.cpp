
#include "lin_protocol.h"

#include "menu.h"
#include "master.h"

// UI modes
static IdleMode modeIdle;
static ParameterMode modeParameter;

////////////////////////////////////////////////////////////////////////////////
// Toplevel menu engine
//
Menu::Menu(Display &disp) :
    _disp(disp),
    _mode(&modeIdle)
{
}

void
Menu::tick()
{
    Display::Button bp = _disp.getButtonPress();

    // kick the mode state machine
    Mode *newmode = _mode->action(_disp, bp);

    // mode change?
    if (newmode != _mode) {
        _mode = newmode;
        _mode->enter(_disp);
    }
}

////////////////////////////////////////////////////////////////////////////////
// Idle mode
//

void
IdleMode::enter(Display &disp)
{
        disp.clear();
        disp.setBacklight(0);
}

Mode *
IdleMode::action(Display &disp, Display::Button bp)
{
    if (bp == Display::kButtonEnter) {
        disp.setBacklight(10);
        return &modeParameter;
    }
    return this;
}

////////////////////////////////////////////////////////////////////////////////
// Parameter mode
//

void
ParameterMode::enter(Display &disp)
{
    _submode = SM_NODE;
    _setNode(0);
    _draw(disp);
}

Mode *
ParameterMode::action(Display &disp, Display::Button bp)
{
    switch (bp) {

    case Display::kButtonCancel:
        return &modeIdle;

    case Display::kButtonEnter:
        _saveValue();
        break;

    case Display::kButtonDown:
        switch (_submode) {
        case SM_NODE:
            _prevNode();
            break;
        case SM_PARAM:
            _prevParam();
            break;
        case SM_VALUE:
            _setValue(_value - 1);
            break;
        }
        break;

    case Display::kButtonUp:
        switch (_submode) {
        case SM_NODE:
            _nextNode();
            break;
        case SM_PARAM:
            _nextParam();
            break;
        case SM_VALUE:
            _setValue(_value + 1);
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
    _draw(disp);

    // no mode change
    return this;
}

void
ParameterMode::_setNode(uint8_t node)
{
    // parameter 0 should always be valid
    _node = node;
    _present = _setParam(0);
}

bool
ParameterMode::_setParam(uint8_t param)
{
    _param = param;
    return _load();
}

void
ParameterMode::_setValue(uint16_t value)
{
    _value = value;
    _changed = true; 
}

void
ParameterMode::_saveValue()
{
    if (_present && _changed) {
        // send the parameter to the node...
        _save();

        // read it back, clears _changed on success
        _load();
    }
}

bool
ParameterMode::_load()
{
    LIN::DataDumpRequest f(_node,
                          LIN::kDataDumpGetParam,
                          _param);

    if (!gMaster.doRequestResponse(f)) {
        return false;
    }

    _changed = false;
    _value = f.d2();
    _value = (_value << 8) | f.d1();

    return true;
}

void
ParameterMode::_save()
{
    LIN::DataDumpRequest f(_node,
                           LIN::kDataDumpSetParam,
                           _param,
                           _value & 0xff,
                           _value >> 8);

    gMaster.doRequest(f);
}

void
ParameterMode::_draw(Display &disp)
{
    disp.clear();
    disp.writeP(PSTR("Node Parm Value"));

    // mode marker
    disp.move((uint8_t)_submode, 1);
    disp.writeP(PSTR(">"));

    // unsaved marker
    if (_changed) {
        disp.move(15, 0);
        disp.writeP(PSTR("*"));
    }

    // values
    disp.move(1, 1);
    disp.write(_node);
    if (_present) {
        disp.move(6, 1);
        disp.write(_param);
        disp.move(11, 1);
        disp.write(_value);
    }
}