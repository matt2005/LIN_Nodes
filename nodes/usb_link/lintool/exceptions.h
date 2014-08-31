/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <msmith@purgatory.org> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return.
 * ----------------------------------------------------------------------------
 */

// ideas from https://github.com/nojhan/exceptions

#pragma once

#include <string>
#include <sstream>
#include <exception>

#define EXCEPTION(Super,Current)                                    \
    class Current : public Super {                                  \
    public: Current ( const std::string & desc) : Super (desc) {}   \
    }; struct __hack_##Current

#define RAISE(Err, msg) { std::ostringstream oss; oss << msg; throw (Err(oss.str())); }

class Exception : public std::exception
{
protected:
    std::string description;

public:
    Exception(const std::string &desc) : description(desc) {}
    virtual ~Exception() throw() {}
    virtual const char *what() const throw() override
    {
        return description.c_str();
    }
};
