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
#include "log.h"

Node::List  Node::_list;
static std::list<unsigned> knownAddresses = {
    Master::kNodeAddress,
    PowerV3::kNodeAddress + 0,
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
    _list.clear();

    if (address == kNoNode) {

        for (auto addr : knownAddresses) {
            Node::_scan(addr);
        }

    } else {
        Node::_scan(address);
    }

    if (nodes().empty()) {
        RAISE(ExScanFailed, "no nodes found");
    }
}

void
Node::_scan(unsigned address)
{
    Link::set_node(address);

    try {
        Link::read_param(Generic::kParamProtocolVersion);
        new Node(address);

    } catch (Link::ExLINError &e) {
        // probably a timeout, ignore this
    } catch (Exception &e) {
        // fatal link error
        RAISE(ExScanFailed, "scan failed: " << e.what());

    } catch (...) {
        throw;
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
Node::update(bool verify, bool save_params)
{
    // get the firmware
    Firmware *fw;

    try {
        if (function() == board_function::kUnconfigured) {
            fw = Firmware::implied_firmware();

        } else {
            fw = Firmware::for_function(function());
        }

    } catch (Exception &e) {
        RAISE(ExUpdateFailed, e.what());
    }

    char *ident = params().identity();  // XXX leaks on exception...

    if (fw == nullptr) {
        warnx("%s no firmware loaded, skipping", ident);

    } else {

        // make a copy of all the node's parameters for later use
        if (save_params) {
            params().sync();
        }

        warnx("%s updating firmware", ident);

        // select the node
        Link::set_node(address());

        // upload firmware to the selected node
        Upload::upload(fw, verify);

        // write parameters back unconditionally
        if (save_params) {
            warnx("%s restoring configuration", ident);
            params().sync(true);
        }
    }

    free(ident);
}

