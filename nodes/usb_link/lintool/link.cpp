/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <msmith@purgatory.org> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return.
 * ----------------------------------------------------------------------------
 */

#include <stdexcept>
#include <unistd.h>
#include <err.h>

#include "link.h"
#include "log.h"

namespace Link
{
libusb_context              *_usb_ctx = nullptr;
struct libusb_device_handle *_usb_handle = nullptr;

constexpr uint16_t bytes_to_short(uint8_t low, uint8_t high) { return ((uint16_t)high << 8) + low; }

void
connect()
{
    libusb_init(&_usb_ctx);

    // discover devices
    libusb_device **list = nullptr;
    ssize_t cnt = libusb_get_device_list(NULL, &list);
    ssize_t i = 0;
    int err = 0;

    if (cnt < 0) {
        RAISE(ExUSBFailed, "no USB devices found");
    }

    for (i = 0; i < cnt; i++) {
        libusb_device *device = list[i];
        struct libusb_device_descriptor desc;

        if (libusb_get_device_descriptor(device, &desc)) {
            RAISE(ExUSBFailed, "can't get device descriptor");
        }

        // match on vendor/device ID for now
        if ((desc.idVendor == bytes_to_short(USB_CFG_VENDOR_ID)) &&
            (desc.idProduct == bytes_to_short(USB_CFG_DEVICE_ID))) {

            err = libusb_open(device, &_usb_handle);

            if (err) {
                RAISE(ExUSBFailed, "open failed");
            }

            return;
        }
    }

    RAISE(ExConnectFailed, "LIN interface not found");
}


int
request(uint8_t bRequest, uint16_t wValue, uint16_t wIndex)
{
    return libusb_control_transfer(_usb_handle,
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
    return libusb_control_transfer(_usb_handle,
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
    return libusb_control_transfer(_usb_handle,
                                   LIBUSB_REQUEST_TYPE_VENDOR | LIBUSB_RECIPIENT_DEVICE | LIBUSB_ENDPOINT_OUT,
                                   bRequest,
                                   wValue,
                                   wIndex,
                                   data,
                                   wLength,
                                   5000);
}

void
enable_master(bool enable)
{
    LIN_LOG_BLOCK;

    int result = request(kUSBRequestEnableMaster, enable ? 1 : 0, 0);

    if (result < 0) {
        warnx("master failed");
        RAISE(ExUSBFailed, "master: USB error " << result);
    }

    for (unsigned tries = 0; tries < 20; tries++) {
        usleep(50000);

        // check for bus no longer awake
        if (!(get_status() & RQ_STATUS_AWAKE)) {
            return;
        }
    }

    RAISE(ExLinkFailed, "cannot claim the bus");
}

uint8_t
get_status(unsigned which, unsigned index)
{
    uint8_t status = 0;

    int result = request_in(kUSBRequestStatus, index, which, &status, sizeof(status));

    if (result < 0) {
        RAISE(ExUSBFailed, "status: USB error " << result);
    }

    return status;
}

void
set_node(uint8_t node)
{
    int result = request(kUSBRequestSelectNode, node, 0);

    if (result < 0) {
        RAISE(ExUSBFailed, "set node: USB error " << result);
    }
}

void
write_param(uint16_t index, uint16_t value)
{
    LIN_LOG_BLOCK;

    int result = request(kUSBRequestWriteData, value, index);

    if (result < 0) {
        RAISE(ExUSBFailed, "write: USB error " << result);
    }
}

void
bulk_data(uint8_t *bytes)
{
    LIN_LOG_BLOCK;

    uint16_t value, index;

    value = bytes[0] + (((uint16_t)bytes[1]) << 8);
    index = bytes[2] + (((uint16_t)bytes[3]) << 8);

    int result = request(kUSBRequestSendBulk, value, index);

    if (result < 0) {
        RAISE(ExUSBFailed, "bulk: USB error " << result);
    }

}

uint16_t
read_param(uint16_t index)
{
    LIN_LOG_BLOCK;

    int result;
    uint16_t value;
    unsigned tries = 0;
    uint8_t status;

    for (;;) {
        result = request(kUSBRequestReadData, 0, index);

        if (result < 0) {
            RAISE(ExUSBFailed, "read1: USB error " << result);
        }

        status = get_status();

        if (status & RQ_STATUS_DATA_ERROR) {
            if (tries++ < 3) {
                continue;
            }

            RAISE(ExLINError, "persistent error in LIN data phase, status " << unsigned(status));
        }

        break;
    }

    if (status & RQ_STATUS_DATA_REJECTED) {
        RAISE(ExNoParam, "node rejected parameter");
    }

    if (status & RQ_STATUS_DATA_READY) {
        result = request_in(kUSBRequestReadResult, 0, 0, (uint8_t *)&value, sizeof(value));

        if (result < 0) {
            RAISE(ExUSBFailed, "read2: USB error " << result);
        }

        if (result != 2) {
            RAISE(ExLinkFailed, "bad data size in read: " << result);
        }

        return value;
    }

    RAISE(ExLINError, "transfer status error, status " << unsigned(status));

}

void
print_status()
{
    warnx("free memory: %u", Link::get_status(RQ_STATUS_FREEMEM));
    uint8_t status = Link::get_status();

    if (status & RQ_STATUS_DATA_READY) {
        warnx("status: DATA_READY");
    }

    if (status & RQ_STATUS_DATA_ERROR) {
        warnx("status: DATA_ERROR");
    }

    if (status & RQ_STATUS_AWAKE) {
        warnx("status: AWAKE");
    }

    if (status & RQ_STATUS_WAITING) {
        warnx("status: WAITING");
    }

    if (status & RQ_STATUS_MASTER) {
        warnx("status: MASTER");
    }

    warnx("Errors:");
    warnx("  line:            %u", Link::get_status(RQ_STATUS_LINERR, 0));
    warnx("  checksum:        %u", Link::get_status(RQ_STATUS_LINERR, 1));
    warnx("  parity:          %u", Link::get_status(RQ_STATUS_LINERR, 2));
    warnx("  framing:         %u", Link::get_status(RQ_STATUS_LINERR, 3));
    warnx("  synchronisation: %u", Link::get_status(RQ_STATUS_LINERR, 4));
    warnx("  protocol:        %u", Link::get_status(RQ_STATUS_LINERR, 5));
}

} // namespace Link
