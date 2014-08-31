/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <msmith@purgatory.org> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return.
 * ----------------------------------------------------------------------------
 */

#include <err.h>
#include <unistd.h>

#include "node.h"
#include "link.h"
#include "upload.h"

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

Node::Node(unsigned address) :
    _params(address),
    _address(address)
{
    _list.push_back(this);
}

Node::~Node()
{
    _list.remove(this);
}

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
        Link::enable_master();

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
        if (n->address() == address) {
            return true;
        }
    }

    return false;
}

Node *
Node::matching(unsigned address, unsigned function)
{
    for (auto n : _list) {
        if ((n->address() == address) && (n->function() == function)) {
            return n;
        }
    }

    return nullptr;
}

void
Node::update()
{
    // get the firmware
    Firmware *fw;

    if (function() == board_function::kUnconfigured) {
        fw = Firmware::implied_firmware();
    } else {
        fw = Firmware::for_function(function());
    }

    if (fw == nullptr) {
        throw (std::runtime_error("no firmware available"));
    }

    // select the node
    Link::set_node(address());
    Link::enable_master();

    // upload firmware to the selected node
    Upload::upload(fw);
}

