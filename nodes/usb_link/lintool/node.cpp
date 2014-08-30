/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <msmith@purgatory.org> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return.
 * ----------------------------------------------------------------------------
 */

#include <err.h>

#include "node.h"
#include "link.h"

Node::List  Node::_list;

void
Node::scan(unsigned address)
{

    if (address == kNoNode) {
        _list.clear();
        for (address = 1; address <= 20; address++) {
            Node::scan(address);
        }
    } else {
        Link::set_node(address);
        Link::enable_master(true);

        try {
            Link::read_data(Generic::kParamProtocolVersion);
            new Node(address);
        } catch (...) {
        }
    }
}

Node::Node(unsigned address) :
    _address(address)
{
    _list.push_back(this);
}

Node::~Node()
{
    _list.remove(this);
}
