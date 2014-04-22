///@file menu.h

#pragma once

#include "display.h"
#include "master.h"

class Mode
{
public:
    /// Called when the mode is activated.
    ///
    virtual void    enter(Display &disp);

    /// Kick the mode state machine.
    ///
    /// @param disp     The display the mode is using.
    /// @param bp       The oldest un-handled button press; may be
    ///                 Display::kButtonNone if no button press has
    ///                 been received.
    /// @return         The mode that is current following processing of
    ///                 this call; should return this if no mode change.
    ///
    virtual Mode    *action(Display &disp, Display::Button bp);
};

class IdleMode : public Mode
{
public:
    virtual void    enter(Display &disp) override;
    virtual Mode    *action(Display &disp, Display::Button bp);
};

class ParameterMode : public Mode
{
public:
    virtual void    enter(Display &disp) override;
    virtual Mode    *action(Display &disp, Display::Button bp);

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
    void            _setNode(uint8_t node);

    /// Set a new parameter ID and attempts to load it from the node. Updates
    /// _valid, _writable and _value, clears _changed and discards any previously 
    /// changed parameter.
    ///
    /// @param param    The new parameter ID.
    /// @return         True if the node responds to the parameter load.
    ///
    bool            _setParam(uint8_t param);

    /// Set a new parameter value.
    ///
    /// @param value    new value for the parameter
    ///
    void            _setValue(uint16_t value);

    /// Write the parameter back to the node
    void            _saveValue();

    /// Move to the previous node
    void            _prevNode() { if (_node > 0) _setNode(_node - 1); }

    /// Move to the next node
    void            _nextNode() { if (_node < 255) _setNode(_node + 1); }

    /// Move to the previous parameter
    void            _prevParam() { if (_param > 0) _setParam(_param - 1); }

    /// Move to the next parameter
    void            _nextParam() { if (_param < 255) _setParam(_param + 1); }

    /// Send the parameter to the node.
    ///
    void            _save();

    /// Load the parameter from the node. Updates _valid, _writable and _value,
    /// clears _changed and discards any previously changed parameter.
    ///
    /// @return         True if the node responds to the request.
    ///
    bool            _load();

    /// Draw the edit display
    ///
    void            _draw(Display &disp);

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

