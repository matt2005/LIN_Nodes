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
static std::list<unsigned> knownAddresses = {
    Master::kNodeAddress,
    PowerV3::kNodeAddress + 0,  // range shared with PowerV1
    PowerV3::kNodeAddress + 1,
    PowerV3::kNodeAddress + 2,
    PowerV3::kNodeAddress + 3,
    PowerV3::kNodeAddress + 4,
    PowerV3::kNodeAddress + 5,
    PowerV3::kNodeAddress + 6,
    PowerV3::kNodeAddress + 7,
    PowerV3::kNodeAddress + 8,
    PowerV3::kNodeAddress + 9,
    PowerV3::kNodeAddress + 10,
    PowerV3::kNodeAddress + 11,
    PowerV3::kNodeAddress + 12,
    PowerV3::kNodeAddress + 13,
    PowerV3::kNodeAddress + 14,
    ECUBridge::kNodeAddress,
    Dashboard::kNodeAddress,
    Bootloader::kNodeAddress
};

void
Node::scan(unsigned address)
{

    if (address == kNoNode) {
        _list.clear();
        for (auto addr : knownAddresses) {
            Node::scan(addr);
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

bool
Node::exists(unsigned address)
{
    for (auto n : _list) {
        if (n->_address == address) {
            return true;
        }
    }
    return false;
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
