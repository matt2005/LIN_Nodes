/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <msmith@purgatory.org> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return.
 * ----------------------------------------------------------------------------
 */

#include <unistd.h>
#include <err.h>

#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <stdexcept>

#include <lin_defs.h>

#include "firmware.h"


std::list<Firmware *>   Firmware::_firmwares;

Firmware::Firmware(const char *fromFile)
{
    char functionName[64];
    char revision[64];

    FILE *fp = fopen(fromFile, "r");

    if (fp == nullptr) {
        throw (std::runtime_error("can't open file"));
    }

    if (fscanf(fp, "Node: %64s %64s\n", functionName, revision) != 2) {
        throw (std::runtime_error("invalid format"));
    }

    _functionName = strdup(functionName);

    // read 16-bit address intelhex
    while (!feof(fp)) {
        char line[100];

        line[0] = '\0';
        fgets(line, sizeof(line), fp);

        unsigned count;
        unsigned address;
        unsigned type;

        if (sscanf(line, ":%2x%4x%2x", &count, &address, &type) != 3) {
            continue;
        }

        unsigned sum = count + (address >> 8) + (address & 0xff) + type;
        unsigned pos = 9;

        if (type == 1) {
            break;
        }

        if (type == 0) {
            unsigned byte;

            while (count--) {
                if (sscanf(line + pos, "%2x", &byte) != 1) {
                    throw (std::runtime_error("malformed intelhex"));
                }

                sum += byte;
                _bytes[address] = byte;
                address++;
                pos += 2;
            }

            if (sscanf(line + pos, "%2x", &byte) != 1) {
                throw (std::runtime_error("malformed intelhex"));
            }

            sum = (~sum + 1) & 0xff;

            if (sum != byte) {
                throw (std::runtime_error("intelhex checksum error"));
            }
        }
    }

    if (_bytes.empty()) {
        throw (std::runtime_error("no data in file"));
    }

    // add ourselves to the list
    _firmwares.push_back(this);
}

Firmware::~Firmware()
{
    if (_functionName) {
        delete _functionName;
    }
}

Firmware *
Firmware::for_function(unsigned function)
{
    if (_firmwares.size() == 0) {
        throw (std::runtime_error("no firmware file loaded"));
    }

    const char *name = Encoding::info(kEncoding_board_function, function);

    for (auto p : _firmwares) {
        if (!strcasecmp(name, p->_functionName)) {
            return p;
        }
    }

    return nullptr;
}

Firmware *
Firmware::implied_firmware()
{
    if (_firmwares.size() == 0) {
        throw (std::runtime_error("no firmware file loaded"));
    }
    if (_firmwares.size() == 1) {
        return _firmwares.front();
    }
    throw (std::runtime_error("too many firmware files loaded"));
}

bool
Firmware::get_bytes(unsigned base, unsigned count, uint8_t *buf) const
{
    bool result = false;

    while (count--) {
        auto it = _bytes.find(base);

        if (it == _bytes.end()) {
            *buf = 0xff;

        } else {
            *buf = it->second;
            result = true;
        }

        base++;
        buf++;
    }

    return result;
}
