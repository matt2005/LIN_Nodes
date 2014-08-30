/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <msmith@purgatory.org> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return.
 * ----------------------------------------------------------------------------
 */
#pragma once

#include <list>

#include "param.h"

class Node
{
public:
    typedef std::list<Node *>   List;
    static const unsigned       kNoNode = ~0U;

    Node(unsigned address);
    ~Node();

    static void     scan(unsigned address = kNoNode);
    static void     clear() { _list.clear(); }
    static List     &nodes() { return _list; }

    unsigned        address() const { return _address; }
    ParamSet        *params() const { return new ParamSet(_address); }

private:
    static List     _list;

    unsigned        _address;
};

