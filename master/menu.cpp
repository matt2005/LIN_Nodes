
#include "board.h"
#include "lin_protocol.h"

#include "menu.h"
#include "master.h"

////////////////////////////////////////////////////////////////////////////////
// Toplevel menu engine
//
Menu::Menu(Display &disp, Master &master) :
    _disp(disp),
    _master(master),
    _modeIdle(*this),
    _modeParameter(*this),
    _modeExplore(*this),
    _mode(&_modeIdle)
{
    _mode->enter();
}

void
Menu::tick()
{
    Display::Button bp = _disp.getButtonPress();

    // kick the mode state machine
    Mode *newmode = _mode->action(bp);

    // mode change?
    if (newmode != _mode) {
        _mode = newmode;
        _mode->enter();
    }
}

////////////////////////////////////////////////////////////////////////////////
// Idle mode
//

void
Menu::IdleMode::enter()
{
    _parent._disp.clear();
    _parent._disp.setBacklight(0);
}

Menu::Mode *
Menu::IdleMode::action(Display::Button bp)
{
    if (bp == Display::kButtonEnter) {
        _parent._disp.setBacklight(10);
        return &_parent._modeParameter;
    }
    if (bp == Display::kButtonDown) {
        _parent._disp.setBacklight(10);
        return &_parent._modeExplore;
    }

    return this;
}

////////////////////////////////////////////////////////////////////////////////
// Explore mode
//

void
Menu::ExploreMode::enter()
{
    _parent._disp.clear();
    _node = LIN::kNADMaster;
    check();
}

Menu::Mode *
Menu::ExploreMode::action(Display::Button bp)
{
    switch (bp) {
    case Display::kButtonCancel:
        return &_parent._modeIdle;

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
Menu::ExploreMode::check()
{
    _parent._disp.clear();
    _parent._disp.move(0, 0);
    _parent._disp.write(_node);
    _parent._disp.move(4, 0);

    LIN::Frame f = LIN::Frame::makeReadByIDRequest(_node, LIN::kRBIProductID);
    if (!_parent._master.doRequestResponse(f)) {
        _present = false;
        _parent._disp.writeP(PSTR("missing"));
    } else {
        _present = true;
        _parent._disp.writeP(PSTR("found"));
    }
}

////////////////////////////////////////////////////////////////////////////////
// Parameter mode
//

void
Menu::ParameterMode::enter()
{
    _submode = SM_NODE;
    setNode(0);
    _parent._disp.clear();
    _parent._disp.writeP(PSTR("Node Parm Value"));
    draw();
}

Menu::Mode *
Menu::ParameterMode::action(Display::Button bp)
{
    switch (bp) {

    case Display::kButtonCancel:
        return &_parent._modeIdle;

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
Menu::ParameterMode::setNode(uint8_t node)
{
    // parameter 0 should always be valid
    _node = node;
    _present = setParam(0);
}

bool
Menu::ParameterMode::setParam(uint8_t param)
{
    _param = param;
    return load();
}

void
Menu::ParameterMode::setValue(uint16_t value)
{
    _value = value;
    _changed = true; 
}

void
Menu::ParameterMode::saveValue()
{
    if (_present && _changed) {
        // send the parameter to the node...
        save();

        // read it back, clears _changed on success
        load();
    }
}

bool
Menu::ParameterMode::load()
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
Menu::ParameterMode::save()
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
Menu::ParameterMode::draw()
{
    // clear edit line
    _parent._disp.move(0, 1);
    _parent._disp.writeP(PSTR("                "));     // XXX inefficient

    // mode marker
    _parent._disp.move((uint8_t)_submode, 1);
    _parent._disp.writeP(PSTR(">"));

    // values
    _parent._disp.move(1, 1);
    _parent._disp.write(_node);
    if (_present) {
        _parent._disp.move(6, 1);
        _parent._disp.write(_param);
        _parent._disp.move(11, 1);
        _parent._disp.write(_value);
    }

    // unsaved marker
    _parent._disp.move(14, 1);
    _parent._disp.writeP(_changed ? PSTR("*") : PSTR(" "));
}
