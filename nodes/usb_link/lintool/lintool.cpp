/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <msmith@purgatory.org> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return.
 * ----------------------------------------------------------------------------
 */

#include <stdint.h>
#include <stdio.h>
#include <err.h>
#include <libusb-1.0/libusb.h>

#include "../usbconfig.h"
#include "../requests.h"
#include "../../../common/lin_protocol.h"
#include "../../../common/protocol.h"

libusb_context *usb_ctx;
struct libusb_device_handle *handle;

constexpr uint16_t bytes_to_short(uint8_t low, uint8_t high) { return ((uint16_t)high << 8) + low; }

void
init()
{
    libusb_init(&usb_ctx);

    // discover devices
    libusb_device **list;
    ssize_t cnt = libusb_get_device_list(NULL, &list);
    ssize_t i = 0;
    int err = 0;
    if (cnt < 0) {
        errx(1, "usb: no devices");
    }
    for (i = 0; i < cnt; i++) {
        libusb_device *device = list[i];
        struct libusb_device_descriptor desc;

        if (libusb_get_device_descriptor(device, &desc)) {
            errx(1, "usb: descriptor");
        }

        // match on vendor/device ID for now
        if ((desc.idVendor == bytes_to_short(USB_CFG_VENDOR_ID)) &&
            (desc.idProduct == bytes_to_short(USB_CFG_DEVICE_ID))) {

            err = libusb_open(device, &handle);
            if (err) {
                errx(1, "usb: open");
            }
            goto found;
        }
    }
    errx(1, "LIN interface not found");
found:
    libusb_free_device_list(list, 1);
}

bool
set(uint16_t index, uint16_t value)
{
    int result = libusb_control_transfer(handle,
                                 LIBUSB_REQUEST_TYPE_VENDOR | LIBUSB_RECIPIENT_DEVICE | LIBUSB_ENDPOINT_OUT,
                                 kUSBRequestWriteData,
                                 value,
                                 index,
                                 nullptr,
                                 0,
                                 5000);

    // XXX some errors should be OK
    if (result < 0) {
        errx(1, "set: USB error %s", libusb_strerror((enum libusb_error)result));
    }
    return true;
}

bool
get(uint16_t index, uint16_t &value)
{
    int result = libusb_control_transfer(handle,
                                 LIBUSB_REQUEST_TYPE_VENDOR | LIBUSB_RECIPIENT_DEVICE | LIBUSB_ENDPOINT_IN,
                                 kUSBRequestReadData,
                                 0,
                                 index,
                                 (uint8_t *)&value,
                                 sizeof(value),
                                 5000);

    // XXX errors might be OK
    if (result < 0) {
        errx(1, "get: USB error %s", libusb_strerror((enum libusb_error)result));
    }
    return (result == 2) ? true : false;
}

int
trace(uint8_t frame[9])
{
    int result = libusb_control_transfer(handle,
                                 LIBUSB_REQUEST_TYPE_VENDOR | LIBUSB_RECIPIENT_DEVICE | LIBUSB_ENDPOINT_IN,
                                 kUSBRequestGetHistory,
                                 0,
                                 0,
                                 frame,
                                 9,
                                 5000);
    if (result < 0) {
        errx(1, "trace: USB error %s", libusb_strerror((enum libusb_error)result));
    }
    return result;
}

int
main(int argc, const char *argv[])
{
    init();

    for (;;) {
        uint8_t frame[9];

        int result = trace(frame);

        if (result == 0) {
            continue;
        }

        if (frame[0] & RQ_HISTORY_FRAME_VALID) {
            printf("%02x: ", frame[0] & RQ_HISTORY_FID_MASK);
            if (frame[0] & RQ_HISTORY_RESPONSE_VALID) {
                printf("%02x %02x %02x %02x %02x %02x %02x %02x",
                    frame[1],
                    frame[2],
                    frame[3],
                    frame[4],
                    frame[5],
                    frame[6],
                    frame[7],
                    frame[8]);
            }
        } else {
            printf("??");
        }
        printf("\n");
    }
}
