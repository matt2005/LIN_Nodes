/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <msmith@purgatory.org> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return.
 * ----------------------------------------------------------------------------
 */
#pragma once

#include <stdint.h>
#include <list>

#include "../../../common/lin_defs.h"

class Param
{
public:
    typedef std::list<Param *>  List;
    static const unsigned   kMaxAddress = 0xffff;

    Param(unsigned address, unsigned function) :
        _address(address),
        _function(function)
    {}

    const char          *format();

    void                sync();
    void                set(unsigned value);
    unsigned            get() const { return _value; }
    bool                exists() const;
    bool                valid() const { return _valid; }
    bool                dirty() const { return _dirty; }

private:
    bool        _valid = false;
    bool        _dirty = false;
    unsigned    _value = 0;

    unsigned    _address;
    unsigned    _function;

    void                fetch();
    void                store();
    unsigned            encoding();
    const char          *name();
};

class ParamSet
{
public:
    ParamSet(unsigned node);
    ~ParamSet();

    void                sync();
    void                print();
    Param::List         list();

    bool                dirty() const;

private:
    unsigned            _node;
    unsigned            _function = board_function::kUnknown;
    Param::List         _params;
};
