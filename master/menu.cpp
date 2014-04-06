
#include "menu.h"

Menu::Menu(Display &disp) :
    _disp(disp),
    _mode(M_IDLE),
    _submode(SM_NONE),
    _want_refresh(true)
{
}

void
Menu::tick()
{
    Display::Button bp = _disp.getButtonPress();

    switch(_mode) {
    case M_IDLE:
        _m_do_idle(bp);
        break;
    case M_PARAM:
        _m_do_param(bp);
        break;
    }
}

void
Menu::_m_go_idle()
{
    _mode = M_IDLE;
    _want_refresh = true;
}

void
Menu::_m_do_idle(Display::Button bp)
{
    if (_want_refresh) {
        _disp.clear();
        _disp.setBacklight(0);
        _want_refresh = false;
    }

    if (bp == Display::kButtonEnter) {
        _disp.setBacklight(10);
        _m_go_param();
    }
}

void
Menu::_m_go_param()
{
    _disp.clear();
    _mode = M_PARAM;
    _submode = SM_NODE;
    _editor.init();
    _want_refresh = true;
}

void
Menu::_m_do_param(Display::Button bp)
{
    if (_want_refresh) {
        _disp.clear();
        _disp.writeP(PSTR("Node Parm Value"));

        // mode marker
        _disp.move((uint8_t)_submode, 1);
        _disp.writeP(PSTR(">"));

        // unsaved marker
        if (_editor.isChanged()) {
            _disp.move(15, 0);
            _disp.writeP(PSTR("*"));
        }

        // values
        _disp.move(1, 1);
        _disp.write(_editor.getNode());
        if (_editor.isNodePresent()) {
            _disp.move(6, 1);
            _disp.write(_editor.getParam());

            if (_editor.isParamValid()) {
                _disp.move(11, 1);
                _disp.write(_editor.getValue());
            }
        }

        _want_refresh = false;
    }

    switch (bp) {
    case Display::kButtonNone:
        return;

    case Display::kButtonCancel:
        _m_go_idle();
        break;

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

    default:
        break;
    }

    _want_refresh = true;
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
