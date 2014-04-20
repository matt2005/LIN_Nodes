
#include "menu.h"

static IdleMode modeIdle;
static ParameterMode modeParameter;

Menu::Menu(Display &disp) :
    _disp(disp),
    _mode(&modeIdle)
{
}

void
Menu::tick()
{
    Display::Button bp = _disp.getButtonPress();

    _mode = _mode->action(_disp, bp);
}

void
IdleMode::enter()
{
    _want_refresh = true;
}

Mode *
IdleMode::action(Display &disp, Display::Button bp)
{
    if (_want_refresh) {
        disp.clear();
        disp.setBacklight(0);
        _want_refresh = false;
    }

    if (bp == Display::kButtonEnter) {
        disp.setBacklight(10);
        return &modeParameter;
    }
    return this;
}

void
ParameterMode::enter()
{
    _submode = SM_NODE;
    _editor.init();
    _want_refresh = true;
}

Mode *
ParameterMode::action(Display &disp, Display::Button bp)
{
    if (_want_refresh) {
        disp.clear();
        disp.writeP(PSTR("Node Parm Value"));

        // mode marker
        disp.move((uint8_t)_submode, 1);
        disp.writeP(PSTR(">"));

        // unsaved marker
        if (_editor.isChanged()) {
            disp.move(15, 0);
            disp.writeP(PSTR("*"));
        }

        // values
        disp.move(1, 1);
        disp.write(_editor.getNode());
        if (_editor.isNodePresent()) {
            disp.move(6, 1);
            disp.write(_editor.getParam());

            if (_editor.isParamValid()) {
                disp.move(11, 1);
                disp.write(_editor.getValue());
            }
        }

        _want_refresh = false;
    }

    // XXX should have an idle timer here

    // assume this is true...
    _want_refresh = true;

    switch (bp) {

    case Display::kButtonCancel:
        return &modeIdle;

    case Display::kButtonEnter:
        _editor.saveValue();
        _want_refresh = true;
        break;

    case Display::kButtonDown:
        switch (_submode) {
        case SM_NODE:
            _editor.prevNode();
            break;
        case SM_PARAM:
            _editor.prevParam();
            break;
        case SM_VALUE:
            _editor.setValue(_editor.getValue() - 1);
            break;
        }
        break;

    case Display::kButtonUp:
        switch (_submode) {
        case SM_NODE:
            _editor.nextNode();
            break;
        case SM_PARAM:
            _editor.nextParam();
            break;
        case SM_VALUE:
            _editor.setValue(_editor.getValue() + 1);
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
            if (_editor.isNodePresent()) {
                _submode = SM_PARAM;
            }
            break;
        case SM_PARAM:
            if (_editor.isParamValid() && _editor.isParamWritable()) {
                _submode = SM_VALUE;
            }
            break;
        default:
            break;
        }
        break;

    case Display::kButtonNone:
        _want_refresh = false;
    default:
        break;
    }

    // no mode change
    return this;
}

ParamEditor::ParamEditor() :
    _node(0),
    _param(0),
    _value(0),
    _changed(false),
    _present(true),
    _valid(true)
{
}

void
ParamEditor::_setNode(uint8_t node)
{
    // parameter 0 should always be valid
    _node = node;
    _param = 0;
    _present = _valid = _load();
}

void
ParamEditor::_setParam(uint8_t param)
{
    _param = param;
    _valid = _load();
}

bool
ParamEditor::_load()
{
    // XXX fake it
    if (_node == 0 && (_param == 0 || _param == 3)) {
        _value = 10 + _param;
        _writable = (_param != 0);
        return true;
    }
    return false;
}

bool
ParamEditor::_save()
{
    // XXX fake it
    return true;
}
