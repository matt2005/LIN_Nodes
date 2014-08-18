/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <msmith@purgatory.org> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return.
 * ----------------------------------------------------------------------------
 */

#include <stdint.h>
#include <libusb-1.0/libusb.h>

#include "../usbconfig.h"
#include "../requests.h"

class Link
{
public:
    // connect to the USB link
    void    connect();

    // low-level request methods
    int     request(uint8_t bRequest, uint16_t wValue, uint16_t wIndex);
    int     request_in(uint8_t bRequest, uint16_t wValue, uint16_t wIndex, unsigned char *data, uint16_t wLength);
    int     request_out(uint8_t bRequest, uint16_t wValue, uint16_t wIndex, unsigned char *data, uint16_t wLength);

    // link primitives
    void        enable_master(bool enable);
    uint8_t     get_status(unsigned which = RQ_STATUS_FLAGS, unsigned index = 0);
    void        set_node(uint8_t node);
    void        write_data(uint16_t index, uint16_t value);
    uint16_t    read_data(uint16_t index);
    void        bulk_data(uint8_t *bytes);

private:
    libusb_context              *_usb_ctx = nullptr;
    struct libusb_device_handle *_usb_handle = nullptr;
};
