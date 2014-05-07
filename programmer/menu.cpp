#include <avr/pgmspace.h>

#include "board.h"
#include "lin_protocol.h"
#include "mc33972.h"

#include "hd44780.h"
#include "menu.h"

#include "m_top.h"
#include "m_explore.h"

namespace Menu
{

class ParameterMode : public Mode
{
public:
    virtual void    enter(Mode *from) override;
    virtual Mode    *action(Button bp);

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


static Mode         *_mode;     ///< current mode

////////////////////////////////////////////////////////////////////////////////
// Toplevel menu wrapper
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
ParameterMode::action(Button bp)
{
    switch (bp) {

//    case kButtonCancel:
//        return &_modeTop;

    case kButtonSelect:
        saveValue();
        break;

    case kButtonDown:
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

    case kButtonUp:
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

} // namespace Menu
