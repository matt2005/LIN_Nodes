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
#include <cstdio>

#include <lin_defs.h>

#include "Jzon.h"
#include "exceptions.h"

class Param
{
public:
    typedef std::list<Param *>  List;
    static const unsigned   kMaxAddress = 0xffff;

    Param(unsigned address, unsigned function = board_function::kUnknown) :
        _address(address),
        _function(function)
    {}

    EXCEPTION(Exception, ExBadValue);
    EXCEPTION(Exception, ExNotSettable);
    EXCEPTION(Exception, ExNotValid);
    EXCEPTION(Exception, ExNonexistent);

    char                *format() const;

    void                sync();
    void                set(unsigned value);
    unsigned            get();
    bool                exists() const;

    unsigned            address() const { return _address; }
    const char          *name() const;
    const char          *info() const;
    bool                is_valid() const { return _valid; }
    bool                is_dirty() const { return _dirty; }

private:
    bool                _valid = false;
    bool                _dirty = false;
    unsigned            _value = 0;

    unsigned            _address;
    unsigned            _function;

    void                fetch();
    void                store();
    unsigned            encoding() const;
};

class ParamSet
{
public:
    typedef std::list<ParamSet *>   List;

    ParamSet(unsigned node);
    ~ParamSet();

    char                *identity() const;

    void                sync();
    bool                is_dirty() const;

    Param               *find(unsigned address);

    void                set(Jzon::Node &fromNode);

    unsigned            node() const { return _node; }
    unsigned            function() const { return _function; }
    Param::List         &list() { return _params; }

private:
    unsigned            _node;
    unsigned            _function = board_function::kUnknown;
    Param::List         _params;
};

class ParamDB
{
public:
    ParamDB();
    ~ParamDB();

    EXCEPTION(Exception, ExJSONInvalid);

    void                read(const char *path);
    void                write(const char *path);

    void                store(ParamSet &pset);
    void                fetch(ParamSet &pset);

    Jzon::Node          &nodes() { return _rootNode; }

private:
    Jzon::Node          _rootNode;
};
