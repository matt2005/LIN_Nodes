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
#include <unistd.h>
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

int
request(uint8_t bRequest, uint16_t wValue, uint16_t wIndex)
{
    return libusb_control_transfer(handle,
                                   LIBUSB_REQUEST_TYPE_VENDOR | LIBUSB_RECIPIENT_DEVICE | LIBUSB_ENDPOINT_IN,
                                   bRequest,
                                   wValue,
                                   wIndex,
                                   nullptr,
                                   0,
                                   5000);
}

int
request_in(uint8_t bRequest, uint16_t wValue, uint16_t wIndex, unsigned char *data, uint16_t wLength)
{
    return libusb_control_transfer(handle,
                                   LIBUSB_REQUEST_TYPE_VENDOR | LIBUSB_RECIPIENT_DEVICE | LIBUSB_ENDPOINT_IN,
                                   bRequest,
                                   wValue,
                                   wIndex,
                                   data,
                                   wLength,
                                   5000);
}

int
request_out(uint8_t bRequest, uint16_t wValue, uint16_t wIndex, unsigned char *data, uint16_t wLength)
{
    return libusb_control_transfer(handle,
                                   LIBUSB_REQUEST_TYPE_VENDOR | LIBUSB_RECIPIENT_DEVICE | LIBUSB_ENDPOINT_OUT,
                                   bRequest,
                                   wValue,
                                   wIndex,
                                   data,
                                   wLength,
                                   5000);
}

uint8_t
get_status()
{
    uint8_t status = 0;

    int result = request_in(kUSBRequestStatus, 0, 0, &status, sizeof(status));

    if (result < 0) {
        errx(1, "get_status: USB error %s", libusb_strerror((enum libusb_error)result));
    }

    warnx("status: 0x%02x", status);
    return status;
}

void
set_node(uint8_t node)
{
    int result = request(kUSBRequestSelectNode, node, 0);

    if (result < 0) {
        errx(1, "set_node: USB error %s", libusb_strerror((enum libusb_error)result));
    }
}

bool
write_data(uint16_t index, uint16_t value)
{
    int result = request(kUSBRequestWriteData, value, index);

    if (result < 0) {
        errx(1, "set: USB error %s", libusb_strerror((enum libusb_error)result));
    }

    return true;
}

bool
read_data(uint16_t index, uint16_t &value)
{
    int result;

    result = request(kUSBRequestReadData, 0, index);

    if (result < 0) {
        errx(1, "read_data: setup, USB error %s", libusb_strerror((enum libusb_error)result));
    }

    // spin waiting for the transaction to complere
    for (unsigned tries = 0; tries < 10; tries++) {
        usleep(10000);                  // 10ms per try
        uint8_t status = get_status();

        if (status & RQ_STATUS_DATA_ERROR) {
            warnx("read_data: LIN error");
            break;
        }

        if (status & RQ_STATUS_DATA_READY) {
            result = request_in(kUSBRequestReadResult, 0, 0, (uint8_t *)&value, sizeof(value));

            if (result < 0) {
                errx(1, "read_data: fetch, USB error %s", libusb_strerror((enum libusb_error)result));
            }

            if (result != 2) {
                errx(1, "read_data: fetch, data error, %d", result);
            }

            return true;
        }

    }

    return false;
}

int
trace(uint8_t frame[9])
{
    int result = request_in(kUSBRequestGetHistory, 0, 0, frame, 9);

    if (result < 0) {
        errx(1, "trace: USB error %s", libusb_strerror((enum libusb_error)result));
    }

    return result;
}

void
log_history(bool forever = true)
{
    for (;;) {
        uint8_t frame[9];

        int result = trace(frame);

        if (result == 0) {
            if (!forever) {
                return;
            }
            continue;
        }

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

        printf("\n");
    }
}

void
clear_history()
{
    for (;;) {
        uint8_t frame[9];

        int result = trace(frame);

        if (result == 0) {
            return;
        }
    }
}



int
main(int argc, const char *argv[])
{
    init();

    if (!(get_status() & RQ_STATUS_AWAKE)) {
        errx(1, "network not awake");
    }

    clear_history();

    for (unsigned i = 1; i < 20; i++) {

        set_node(i);
        uint16_t value = 0xffff;

        if (read_data(0, value)) {
            warnx("%d: got %d", i, value);

        } else {
            warnx("%d: no response", i);
        }
        log_history(false);
    }
}

