///@file menu.h

#pragma once

#include "display.h"

class ParamEditor
{
public:
    ParamEditor();

    /// @return         the currently selected node address
    uint8_t         getNode() const { return _node; }

    /// @return         the currently selected parameter ID
    uint8_t         getParam() const { return _param; }

    /// @return         the value of the currently selected parameter
    uint16_t        getValue() const { return _value; }

    /// @param value    new value for the parameter
    void            setValue(uint16_t value) { if (_writable) { _value = value; _changed = true; } }

    /// @return         true if the parameter has been changed
    bool            isChanged() const { return _changed; }

    /// @return         true if a node is present at the current address
    bool            isNodePresent() const { return _present; }

    /// @return         true if the node is present and supports the
    ///                 current parameter ID
    bool            isParamValid() const { return _valid; }

    /// @return         true if the node is present, the parameter ID
    ///                 is valid and the parameter can be changed
    bool            isParamWritable() const { return _writable; }

    /// Initialise the editor
    void            init() { _setNode(0); }

    /// Move to the previous node
    void            prevNode() { if (_node > 0) _setNode(_node - 1); }

    /// Move to the next node
    void            nextNode() { if (_node < 255) _setNode(_node + 1); }

    /// Move to the previous parameter
    void            prevParam() { if (_param > 0) _setParam(_param - 1); }

    /// Move to the next parameter
    void            nextParam() { if (_param < 255) _setParam(_param + 1); }

    /// Write the parameter back to the node
    void            saveValue() { if (_present && _valid && _changed && _save()) _changed = false; }

private:
    uint8_t         _node;          ///< address of the current node
    uint8_t         _param;         ///< index of the current parameter
    uint16_t        _value;         ///< value of the current parameter
    bool            _changed;       ///< true if the parameter has been changed
    bool            _present;       ///< true if the node is present
    bool            _valid;         ///< true if the parameter is valid for the node
    bool            _writable;      ///< true if the parameter is writable

    /// Set a new node address
    void            _setNode(uint8_t node);

    /// Set a new parameter ID
    void            _setParam(uint8_t param);

    /// Save the parameter to the node
    bool            _save();

    /// Load the parameter from the node
    bool            _load();
};

class Mode
{
public:
    virtual void    enter();
    virtual Mode    *action(Display &disp, Display::Button bp);

protected:
    bool            _want_refresh;
};

class IdleMode : public Mode
{
public:
    virtual void    enter() override;
    virtual Mode    *action(Display &disp, Display::Button bp);
};

class ParameterMode : public Mode
{
public:
    virtual void    enter() override;
    virtual Mode    *action(Display &disp, Display::Button bp);

private:
    enum SubMode {
        // enum values are also marker column values
        SM_NODE     = 0,    ///< editing parameter node 
        SM_PARAM    = 5,    ///< editing parameter ID
        SM_VALUE    = 10,   ///< editing parameter value
    };

    ParamEditor     _editor; ///< parameter editor state
    SubMode         _submode;       ///< current submode
};

class Menu
{
public:
    Menu(Display &disp);

    /// Tickle the menu state machine
    ///
    void            tick();

private:

    Display         &_disp; ///< display we are using

    Mode            *_mode;         ///< current mode

    ///
    void            _m_go_idle();
    void            _m_do_idle(Display::Button bp);

    void            _m_go_param();
    void            _m_do_param(Display::Button bp);
};

