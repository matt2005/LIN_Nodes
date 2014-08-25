/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <msmith@purgatory.org> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return.
 * ----------------------------------------------------------------------------
 */

#include <stdint.h>
#include <list>

#include "../../../common/lin_defs.h"

class Param;

class ParamSet
{
public:
    ParamSet(unsigned node);
    ~ParamSet();

    void        sync();
    void        print();

private:
    unsigned                _node;
    unsigned                _function = board_function::kUnknown;
    std::list<Param *>      _params;
};

class Param
{
public:
    static const unsigned   kMaxAddress = 0xffff;

    Param(unsigned address, unsigned function) :
        _address(address),
        _function(function)
    {}

    const char          *format();

    void                fetch();
    void                store();
    void                set(unsigned value);
    unsigned            get() const { return _value; }
    bool                exists() const;

private:
    unsigned    _address;
    unsigned    _function;
    unsigned    _value = 0;

    unsigned            encoding();
    const char          *name();
};
