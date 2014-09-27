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

char *
Param::format() const
{
    char *str;

    asprintf(&str, "%u %s %u %s", _address, name(), _value, info());
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
    if (encoding() != kEncoding_none) {
        if (Encoding::invalid(encoding(), value)) {
            RAISE(ExBadValue, "invalid value " << value << " for " << name());
        }

        _value = value;
        _dirty = true;
        _valid = true;
    }

    RAISE(ExNotSettable, "cannot set " << name());
}

unsigned
Param::get()
{
    if (_valid) {
        return _value;
    }

    RAISE(ExNotValid, name() << " is not valid at this time");
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
        _value = Link::read_param(_address);

    } catch (Link::ExNoParam) {
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
        Link::write_param(_address, _value);
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

const char *
Param::info() const
{
    const char *str = Encoding::info(encoding(), _value);

    if (str == nullptr) {
        str = "";
    }

    return str;
}

////////////////////////////////////////////////////////////////////////////////

ParamSet::ParamSet(unsigned node) :
    _node(node)
{
    Link::set_node(_node);

    _function = Link::read_param(Generic::kParamBoardFunction);

    // XXX magic numbers
    for (auto address = 0; address < 0x10000U; address++) {
        if (Param(address, _function).exists()) {
            auto p = new Param(address, _function);

            _params.push_back(p);
        }
    }
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

Param *
ParamSet::find(unsigned address)
{
    for (auto p : _params) {
        if (p->address() == address) {
            return p;
        }
    }
    return nullptr;
}

void
ParamSet::sync()
{
    Link::set_node(_node);
    for (auto p : _params) {
        p->sync();
    }
}

void
ParamSet::set(Jzon::Node &fromNode)
{
    const char *name = fromNode.get("name").toString().c_str();
    unsigned value = fromNode.get("value").toInt();

    Link::set_node(_node);

    for (auto p : _params) {
        if (!strcmp(p->name(), name)) {
            p->set(value);
            return;
        }
    }

    RAISE(Param::ExNonexistent, "parameter " << name << "does not exist in this context");
}

////////////////////////////////////////////////////////////////////////////////

ParamDB::ParamDB() :
    _rootNode(Jzon::array())
{
}

ParamDB::~ParamDB()
{
}

void
ParamDB::read(const char *path)
{
    Jzon::Parser parser;

    _rootNode = parser.parseFile(path);

    if (!_rootNode.isValid()) {
        RAISE(ExJSONInvalid, "parse error");
    }
}

void
ParamDB::write(const char *path)
{
    const Jzon::Format compactFormat = { true, true, false, 4 };
    Jzon::Writer writer(compactFormat);

    writer.writeFile(_rootNode, path);
}

void
ParamDB::store(ParamSet &pset)
{
    auto node = Jzon::object();

    node.add("name", Encoding::info(kEncoding_board_function, pset.function()));
    node.add("node", (int)pset.node());
    node.add("function", (int)pset.function());

    auto params = Jzon::array();

    for (auto p : pset.list()) {

        if (p->is_valid()) {
            auto param = Jzon::object();

            param.add("name", p->name());
            param.add("address", (int)p->address());
            param.add("value", (int)p->get());
            param.add("info", p->info());

            params.add(param);
        }
    }

    node.add("parameters", params);
    _rootNode.add(node);
}

void
ParamDB::fetch(ParamSet &pset)
{

}
