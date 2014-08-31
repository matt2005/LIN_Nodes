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
#include "firmware.h"

class Node
{
public:
    typedef std::list<Node *>   List;
    static const unsigned       kNoNode = ~0U;

    Node(unsigned address);
    ~Node();

    static void     scan(unsigned address = kNoNode);
    static bool     exists(unsigned address);

    static void     clear() { _list.clear(); }
    static List     &nodes() { return _list; }

    static Node     *matching(unsigned address, unsigned function);

    unsigned        address() const { return _address; }
    unsigned        function() { return params().function(); }
    ParamSet        &params() { return _params; }

    void            update();

private:
    static List     _list;

    ParamSet        _params;
    unsigned        _address;

    void            upload(Firmware *fw);
};

