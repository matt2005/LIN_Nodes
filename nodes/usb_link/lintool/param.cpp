/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <msmith@purgatory.org> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return.
 * ----------------------------------------------------------------------------
 */

#include <cstdio>
#include <cstdlib>
#include <err.h>

#include "param.h"
#include "link.h"

ParamSet::ParamSet(unsigned node) :
    _node(node)
{
    Link::set_node(_node);
    Link::enable_master(true);

    _function = Link::read_data(Generic::kParamBoardFunction);    

    // XXX magic numbers
    for (auto address = 0; address < 0x10000U; address++) {
        if (Param(address, _function).exists()) {
            auto p = new Param(address, _function);

            _params.push_back(p);
        }
    }

    // get parameters
    sync();
}

ParamSet::~ParamSet()
{
    for (;;) {
        auto p = _params.front();
        if (p == nullptr) {
            break;
        }
        _params.pop_front();
        delete p;
    }
}

char *
ParamSet::identity() const
{
    char *str;
    asprintf(&str, "[%u:%u:%s]\n", _node, _function, Encoding::info(kEncoding_board_function, _function));
    return str;
}

void
ParamSet::write(FILE *fp) const
{
    char *str = identity(); 
    fprintf(fp, "%s", str);
    free(str);

    for (auto p : _params) {
        if (p->is_valid()) {
            auto str = p->format();
            fprintf(fp, "%s\n", str);
            delete str;
        }
    }
}

void
ParamSet::read(FILE *fp)
{
    // rewind the file
    fseek(fp, 0, SEEK_SET);

    // scan the file looking for our parameters
    char line[100];
    bool reading = false;
    while (!feof(fp)) {
        line[0] = '\0';
        fgets(line, sizeof(line), fp);

        unsigned node, address, function, value;
        char name[100];
        char info[100];

        // is this an address line?
        if (sscanf(line, "[%u:%u:%s]", &node, &function, name) == 3) {
            if (reading) {
                break;
            } else if ((node == _node) && (function == _function)) {
                reading = true;
            }
        } else if (sscanf(line, "%u %s %u %s", &address, name, &value, info) != 4) {
            if (reading) {
                // XXX here is where we would do parameter conversions...
                for (auto p : _params) {
                    if (p->address() == address) {
                        p->set(value);
                        break;
                    }
                }
            }
        }
    }
}

void
ParamSet::sync()
{
    for (auto p : _params) {
        p->sync();
    }
}

bool
ParamSet::is_dirty() const
{
    for (auto p : _params) {
        if (p->is_dirty()) {
            return true;
        }
    }
    return false;
}

char *
Param::format() const
{
    char *str;
    const char *info = Encoding::info(encoding(), _value);
    if (info == nullptr) {
        info = "-";
    }

    asprintf(&str, "%u %s %u %s", _address, name(), _value, info);
    return str;
}

void
Param::sync()
{
    if (!_valid) {
        fetch();
    } else if (_dirty) {
        store();
    }
}

void
Param::set(unsigned value) 
{
    // XXX validation
    _value = value;
    _dirty = true;
    _valid = true;
}

bool
Param::exists() const
{
    if (Generic::param_exists(_address)) {
        return true;
    }

    switch (_function) {
    case board_function::kMaster:
        return Master::param_exists(_address);
    case board_function::kPowerV1:
        return PowerV1::param_exists(_address);
    case board_function::kPowerV3:
        return PowerV3::param_exists(_address);
    case board_function::kUnconfigured:
        return Bootloader::param_exists(_address);
    default:
        break;
    }
    return false;
}

void
Param::fetch()
{
    _dirty = false;
    _valid = true;
    try {
        _value = Link::read_data(_address);
    } catch (Link::NoParam) {
        _valid = false;
    } catch (...) {
        _valid = false;
        throw;
    }
}

void
Param::store()
{
    // XXX magic numbers
    if ((_address >= 0x400) && (_address < 0x500)) {
        Link::write_data(_address, _value);
        _dirty = false;
    }
}

unsigned
Param::encoding() const
{
    unsigned value = kEncoding_none;

    switch (_function) {
    case board_function::kMaster:
        value = Master::param_encoding(_address);
        break;
    case board_function::kPowerV1:
        value = PowerV1::param_encoding(_address);
        break;
    case board_function::kPowerV3:
        value = PowerV3::param_encoding(_address);
        break;
    case board_function::kUnconfigured:
        value = Bootloader::param_encoding(_address);
        break;
    }

    return value;
}

const char *
Param::name() const
{
    const char *str = nullptr;

    switch (_function) {
    case board_function::kMaster:
        str = Master::param_name(_address);
        break;
    case board_function::kPowerV1:
        str = PowerV1::param_name(_address);
        break;
    case board_function::kPowerV3:
        str = PowerV3::param_name(_address);
        break;
    case board_function::kUnconfigured:
        str = Bootloader::param_name(_address);
        break;
    }

    if (str == nullptr) {
        str = Generic::param_name(_address);
    }

    return str;    
}

