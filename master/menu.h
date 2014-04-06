///@file menu.h

#pragma once

#include "display.h"

class ParamEditor
{
public:
    ParamEditor();

    uint8_t         getNode() const { return _node; }
    uint8_t         getParam() const { return _param; }
    uint16_t        getValue() const { return _value; }
    void            setValue(uint16_t value) { if (_writable) { _value = value; _changed = true; } }
    bool            isChanged() const { return _changed; }
    bool            isNodePresent() const { return _present; }
    bool            isParamValid() const { return _valid; }
    bool            isParamWritable() const { return _writable; }

    void            init() { _setNode(0); }

    void            prevNode() { if (_node > 0) _setNode(_node - 1); }
    void            nextNode() { if (_node < 255) _setNode(_node + 1); }
    void            prevParam() { if (_param > 0) _setParam(_param - 1); }
    void            nextParam() { if (_param < 255) _setParam(_param + 1); }
    void            saveValue() { if (_present && _valid && _changed && _save()) _changed = false; }

private:
    uint8_t         _node;          ///< address of the current node
    uint8_t         _param;         ///< index of the current parameter
    uint16_t        _value;         ///< value of the current parameter
    bool            _changed;       ///< true if the parameter has been changed
    bool            _present;       ///< true if the node is present
    bool            _valid;         ///< true if the parameter is valid for the node
    bool            _writable;      ///< true if the parameter is writable

    void            _setNode(uint8_t node);
    void            _setParam(uint8_t param);
    bool            _save();
    bool            _load();
};

class Menu
{
public:
    Menu(Display &disp);

    /// Tickle the menu state machine
    ///
    void            tick();

private:
    enum Mode {
        M_IDLE,
        M_PARAM
    };

    enum SubMode {
        SM_NONE,

        SM_NODE     = 0,    // also marker column values
        SM_PARAM    = 5,
        SM_VALUE    = 10,
    };

    ParamEditor     _editor;

    Display         &_disp;

    Mode            _mode;
    SubMode         _submode;
    bool            _want_refresh;

    void            _m_go_idle();
    void            _m_do_idle(Display::Button bp);
    void            _m_go_param();
    void            _m_do_param(Display::Button bp);
};

