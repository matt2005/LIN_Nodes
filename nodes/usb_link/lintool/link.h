/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <msmith@purgatory.org> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return.
 * ----------------------------------------------------------------------------
 */
#pragma once

#include <stdint.h>
#include <stdexcept>
#include <libusb-1.0/libusb.h>

#include "../usbconfig.h"
#include "../requests.h"

namespace Link
{

class NoParam : public std::runtime_error
{
public:
    NoParam() : std::runtime_error("parameter not found") {}
};

// connect to the USB link
extern void     connect();

// low-level request methods
extern int      request(uint8_t bRequest, uint16_t wValue, uint16_t wIndex);
extern int      request_in(uint8_t bRequest, uint16_t wValue, uint16_t wIndex, unsigned char *data, uint16_t wLength);
extern int      request_out(uint8_t bRequest, uint16_t wValue, uint16_t wIndex, unsigned char *data, uint16_t wLength);

// link primitives
extern void     enable_master(bool enable = true);
extern uint8_t  get_status(unsigned which = RQ_STATUS_FLAGS, unsigned index = 0);
extern void     set_node(uint8_t node);
extern void     write_data(uint16_t index, uint16_t value);
extern uint16_t read_data(uint16_t index);
extern void     bulk_data(uint8_t *bytes);

// diagnostics
extern void     print_status();

} // namespace Link
