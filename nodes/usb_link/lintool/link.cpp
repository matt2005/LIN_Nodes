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
#include "link.h"

constexpr uint16_t bytes_to_short(uint8_t low, uint8_t high) { return ((uint16_t)high << 8) + low; }

Link::Link()
{
}

void
Link::connect()
{
    libusb_init(&_usb_ctx);

    // discover devices
    libusb_device **list = nullptr;
    ssize_t cnt = libusb_get_device_list(NULL, &list);
    ssize_t i = 0;
    int err = 0;

    if (cnt < 0) {
        throw(std::runtime_error("usb: no devices"));
    }

    for (i = 0; i < cnt; i++) {
        libusb_device *device = list[i];
        struct libusb_device_descriptor desc;

        if (libusb_get_device_descriptor(device, &desc)) {
            throw(std::runtime_error("usb: can't get descriptor"));
        }

        // match on vendor/device ID for now
        if ((desc.idVendor == bytes_to_short(USB_CFG_VENDOR_ID)) &&
            (desc.idProduct == bytes_to_short(USB_CFG_DEVICE_ID))) {

            err = libusb_open(device, &_usb_handle);

            if (err) {
                throw(std::runtime_error("usb: open failed"));
            }

            return;
        }
    }

    throw(std::runtime_error("LIN interface not found"));
}


int
Link::request(uint8_t bRequest, uint16_t wValue, uint16_t wIndex)
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
Link::request_in(uint8_t bRequest, uint16_t wValue, uint16_t wIndex, unsigned char *data, uint16_t wLength)
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
Link::request_out(uint8_t bRequest, uint16_t wValue, uint16_t wIndex, unsigned char *data, uint16_t wLength)
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
Link::enable_master(bool enable)
{
    int result = request(kUSBRequestEnableMaster, enable ? 1 : 0, 0);

    if (result < 0) {
        throw(std::runtime_error("enable_master: USB error"));
    }
}

uint8_t
Link::get_status(unsigned which, unsigned index)
{
    uint8_t status = 0;

    int result = request_in(kUSBRequestStatus, index, which, &status, sizeof(status));

    if (result < 0) {
        throw(std::runtime_error("get_status: USB error"));
    }
    return status;
}

void
Link::set_node(uint8_t node)
{
    int result = request(kUSBRequestSelectNode, node, 0);

    if (result < 0) {
        throw(std::runtime_error("set_node: USB error"));
    }
}

void
Link::write_data(uint16_t index, uint16_t value)
{
    int result = request(kUSBRequestWriteData, value, index);

    if (result < 0) {
        throw(std::runtime_error("write_data: USB error"));
    }
}

void
Link::bulk_data(uint8_t *bytes)
{
    uint8_t value, index;

    value = bytes[0] + ((uint16_t)(bytes[1]) << 8);
    index = bytes[2] + ((uint16_t)(bytes[3]) << 8);

    int result = request(kUSBRequestSendBulk, value, index);

    if (result < 0) {
        throw(std::runtime_error("bulk_data: USB error"));
    }

}

uint16_t
Link::read_data(uint16_t index)
{
    int result;
    uint16_t value;

    result = request(kUSBRequestReadData, 0, index);

    if (result < 0) {
        throw(std::runtime_error("read_data: USB error in setup"));
    }

    // spin waiting for the transaction to complere
    for (unsigned tries = 0; tries < 10; tries++) {
        usleep(10000);                  // 10ms per try
        uint8_t status = get_status();

        if (status & RQ_STATUS_DATA_ERROR) {
            throw(std::runtime_error("read_data: LIN error"));
        }

        if (status & RQ_STATUS_DATA_READY) {
            result = request_in(kUSBRequestReadResult, 0, 0, (uint8_t *)&value, sizeof(value));

            if (result < 0) {
                throw(std::runtime_error("read_data: USB error in fetch"));
            }

            if (result != 2) {
                throw(std::runtime_error("read_data: data error in fetch"));
            }
            return value;
        }
    }
    throw(std::runtime_error("read_data: timed out"));
}