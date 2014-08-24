#!/usr/bin/env python
#
# Firmware updater for LIN Nodes
#

import usb.core
import usb.util
import array
import time
import argparse
import json
import base64
import sys

# requests - must match requests.h
kUSBRequestStatus       = 0
kUSBRequestClearStatus  = 1
kUSBRequestGetHistory   = 2
kUSBRequestSelectNode   = 3
kUSBRequestReadData     = 4
kUSBRequestReadResult   = 5
kUSBRequestWriteData    = 6
kUSBRequestSendBulk     = 7
kUSBRequestEnableMaster = 8

RQ_STATUS_FLAGS         = 0
RQ_STATUS_DATA_READY        = (1<<0)
RQ_STATUS_DATA_ERROR        = (1<<1)
RQ_STATUS_AWAKE             = (1<<2)
RQ_STATUS_WAITING           = (1<<3)
RQ_STATUS_MASTER            = (1<<4)

RQ_STATUS_FREEMEM       = 1
RQ_STATUS_LINERR        = 2

# XXX TBD - get new values!
USB_CFG_VENDOR_ID = 0x16c0
USB_CFG_DEVICE_ID = 0x05dc

# parameters - must match lin_defs.h
kParamProtocolVersion   = 0x0000
kParamBoardFunction     = 0x0001
kParamBootloaderMode    = 0x0002
kParamFirmwareVersion   = 0x0003
kParamFirmwarePageSize  = 0x0004

kParamStatus            = 0x0300
kParamMemory            = 0x0301
kParamEEPROM            = 0x0302
kParamPageAddress       = 0x0400
kParamPageOffset        = 0x0401
kParamPageCRC           = 0x0402
kParamDebugPointer      = 0x0403

# kParamStatus values
kWaitingForProgrammer   = 0
kReadyForPage           = 1
kPageInProgress         = 2
kPageCRCError           = 3
kPageAddressError       = 4
kNumEncodings           = 5

class Link (object):
    _dev = None

    def __init__(self):
        self._dev = usb.core.find(idVendor = USB_CFG_VENDOR_ID, idProduct = USB_CFG_DEVICE_ID)
        if self._dev is None:
            raise ValueError('USB Link not found')
        self._dev.set_configuration();

    def _request(self, request, value = 0, index = 0, outData = None, inLen = 0):
        requestType = usb.util.CTRL_TYPE_VENDOR | usb.util.CTRL_RECIPIENT_DEVICE
        if outData is not None:
            requestType |= usb.util.CTRL_OUT
            result = self._dev.ctrl_transfer(bmRequestType = requestType,
                                             bRequest = request,
                                             wValue = value,
                                             wIndex = index,
                                             data_or_wLength = outData)
        elif inLen > 0:
            requestType |= usb.util.CTRL_IN
            result = self._dev.ctrl_transfer(bmRequestType = requestType,
                                             bRequest = request,
                                             wValue = value,
                                             wIndex = index,
                                             data_or_wLength = inLen)
        else:
            result = self._dev.ctrl_transfer(bmRequestType = requestType,
                                             bRequest = request,
                                             wValue = value,
                                             wIndex = index)

        return result

    @property
    def status(self):
        value = self._request(request = kUSBRequestStatus, index = RQ_STATUS_FLAGS, inLen = 1)
        assert len(value) == 1
        return value[0]

    @property
    def is_data_ready(self):
        if self.status & RQ_STATUS_DATA_READY:
            return True
        return False

    @property
    def is_data_error(self):
        if self.status & RQ_STATUS_DATA_ERROR:
            return True
        return False

    @property
    def is_awake(self):
        if self.status & RQ_STATUS_AWAKE:
            return True
        return False

    @property
    def is_waiting(self):
        if self.status & RQ_STATUS_WAITING:
            return True
        return False

    @property
    def is_master(self):
        if self.status & RQ_STATUS_MASTER:
            return True
        return False

    @property
    def free_memory(self):
        value = self._request(request = kUSBRequestStatus, index = RQ_STATUS_FREEMEM, inLen = 1)
        return value[0]

    def set_node(self, node):
        self._request(request = kUSBRequestSelectNode, value = node)

    def set_master(self, enable):
        if enable:
            enableFlag = 1
        else:
            enableFlag = 0
        self._request(request = kUSBRequestEnableMaster, value = enableFlag)
        tries = 0
        while tries < 20:
            time.sleep(0.05)
            if not self.is_awake:
                return
            tries += 1

        raise RuntimeError('failed to take control of the bus')

    def get_parameter(self, paramIndex):
        self._wait_data_ready()
        tries = 0
        while tries < 3:
            self._request(request = kUSBRequestReadData, index = paramIndex)
            polls = 0
            while polls < 10:
                time.sleep(0.01)
                raw = self._request(request = kUSBRequestReadResult, inLen = 2)
                if len(raw) == 2:
                    return raw[0] + (raw[1] << 8)
                if self.is_data_error:
                    raise RuntimeError('data error reading parameter {:#x}'.format(paramIndex))
                polls += 1
            tries += 1

        raise RuntimeError('timed out reading parameter {}'.format(paramIndex))

    def set_parameter(self, paramIndex, paramValue, noReadback = False):
        self._wait_data_ready()
        self._request(request = kUSBRequestWriteData, value = paramValue, index = paramIndex)
        if not noReadback:
            if self.get_parameter(paramIndex) != paramValue:
                raise RuntimeError('parameter readback error')

    def send_bulk(self, bytes):
        self._wait_data_ready()
        assert len(bytes) == 4
        value = bytes[0] + (bytes[1] << 8)
        index = bytes[2] + (bytes[3] << 8)
        self._request(request = kUSBRequestSendBulk, value = value, index = index)

    def _wait_data_ready(self):
        tries = 0
        while tries < 20:
            time.sleep(0.01)
            if self.is_data_ready:
                return
            tries += 1
        raise RuntimeError('timed out waiting for link ready')

class Bundle(object):
    _images = dict()
    _names = dict()

    @classmethod
    def load(cls, filename):
        f = open(filename, 'r')
        desc = json.load(f)
        f.close()

        for key, value in desc.iteritems():
            if key.startswith('id_'):
                name = key[3:]
                bytes = bytearray(base64.b64decode(desc['image_' + name]))
                cls._images[value] = bytes
                cls._names[value] = name
                print "{}:{} bytes".format(name, len(bytes))

    @classmethod
    def name_for_id(cls, ident):
        if ident in cls._names:
            return cls._names[ident]
        return 'unknown'

    @classmethod
    def firmware_for_id(cls, ident):
        if ident in cls._images:
            return cls._images[ident]
        return None

    @classmethod
    def id_for_name(cls, searchName):
        for ident,name in cls._names.iteritems():
            if name == searchName:
                return ident
        return None

class Node(object):
    _nodes = dict()
    _link = None

    def __init__(self, address, function):
        self._function = function
        self._address = address
        self._kind = Bundle.name_for_id(self._function)
        print self._ident

    @property
    def _ident(self):
        return '{}@{}'.format(self._kind, self._address)

    def _parameter(self, paramIndex, newValue = None, noReadback = False):
        self._link.set_node(self._address)
        if newValue is None:
            return self._link.get_parameter(paramIndex)
        self._link.set_parameter(paramIndex, newValue, noReadback)

    def _enter_bootloader(self):
        if self._parameter(kParamBootloaderMode) == 0:
            self._parameter(kParamBootloaderMode, 0x4f42, True)
            tries = 0
            while tries < 20:
                time.sleep(0.05)
                try:
                    if self._parameter(kParamBootloaderMode) == 1:
                        return
                except:
                    pass
                print 'not bootloader yet'
                tries += 1
            raise RuntimeError('{} failed to enter bootloader mode'.format(self._ident))

    def update(self):
        self._link.set_master(True)
        try:
            self._link.set_node(self._address)
            self._enter_bootloader()
            uploader = Uploader(self._link, self._function)
            uploader.upload()

        except Exception as ex:
            print "While updating {}:".format(self._ident)
            raise
        self._link.set_master(False)
    
    @classmethod
    def scan(cls, link, addressList = range(1, 18)):
        cls._link = link
        link.set_master(True)
        for address in addressList:
            try:
                cls._link.set_node(address)
                protocol = cls._link.get_parameter(0)
                if protocol != 1:
                    print "{}: unexpected protocol {}".format(address, protocol)
                    continue
                function = cls._link.get_parameter(1)
                cls._nodes[address] = Node(address, function)
            except Exception as ex:
                pass
        link.set_master(False)

    @classmethod
    def update_all(cls):
        for address,node in cls._nodes.iteritems():
            node.update()

class Uploader(object):
    def __init__(self, link, function):
        self._link = link

        # check the function reported by the bootloader
        nodeFunction = self._link.get_parameter(kParamBoardFunction)
        if (nodeFunction != 0xff) and (nodeFunction != function):
            raise RuntimeError('{} claims to be of type {}'.format(ident, Bundle.name_for_id(nodeFunction)))

        # get the flash pagesize
        self._pagesize = self._link.get_parameter(kParamFirmwarePageSize);

        # get the firmware we are going to upload and pad it out to the flash pagesize
        self._bytes = Bundle.firmware_for_id(function)
        while (len(self._bytes) % self._pagesize) != 0:
            self._bytes.append(0xff)

    def upload(self):
        self._reset_bootloader()
        if self._status != kWaitingForProgrammer:
            raise RuntimeError('bootloader in unexpected state {}'.format(self._status))

        print "uploading {} bytes / {} pages".format(len(self._bytes), self._pages)
        for page in range(1, self._pages - 1):
            self._program_page(page)
        #self._program_page(0)

    def _program_page(self, page):
        print 'page {}'.format(page)

        # set the page address
        tries = 0
        while tries < 3:
            base = page * self._pagesize
            self._wait_until_ready
            self._set_address(base)

            # upload page bytes
            pagebytes = self._bytes[base:base + self._pagesize]
            pagecrc = self._crc_xmodem(pagebytes)
            while len(pagebytes) > 0:
                self._link.send_bulk(pagebytes[0:4])
                pagebytes = pagebytes[4:]

            # program the page
            try:
                self._link.set_parameter(kParamPageCRC, pagecrc)
                return
            except:
                print 'upload CRC error, retrying...'

            tries += 1

        raise RuntimeError('too many CRC errors, aborting.')

    # algorithm translated directly from the avrlibc documentation
    def _crc_xmodem(self, data, initial = 0):
        crc = initial
        for byte in data:
            crc = crc ^ (byte << 8)
            for i in range(0, 8):
                if crc & 0x8000:
                    crc = (crc << 1) ^ 0x1021
                else:
                    crc <<= 1
                crc &= 0xffff
        return crc

    def _wait_until_ready(self):
        tries = 0
        while tries < 50:
            if self._is_ready:
                return
            time.sleep(0.01)
            tries += 1
        raise RuntimeError('timed out waiting for bootloader to be ready, status {}'.format(self._status))

    def _set_address(self, address):
        self._link.set_parameter(kParamPageAddress, address)
        if self._is_error:
            raise RuntimeError('error setting address {}'.format(address))
        #selectedAddress = self._link.get_parameter(kParamPageAddress)
        #currentCRC = self._link.get_parameter(kParamPageCRC)
        #if not self._is_busy:
        #    raise RuntimeError('failed to set address {}, status {}, address {}, crc {}'.format(address, self._status, selectedAddress, currentCRC))
        #if selectedAddress != address:
        #    raise RuntimeError('failed to set address {}, got address {} instead'.format(address, selectedAddress))
        #if currentCRC != 0:
        #    raise RuntimeError('bad initial page CRC {:#x}'.format(currentCRC))

    def _reset_bootloader(self):
        self._link.set_parameter(kParamBootloaderMode, 1)

    @property
    def _status(self):
        return self._link.get_parameter(kParamStatus)

    @property
    def _crc(self):
        return self._link.get_parameter(kParamPageCRC)

    @property
    def _is_busy(self):
        return self._status == kPageInProgress

    @property
    def _is_error(self):
        status = self._status
        return (status == kPageCRCError) or (status == kPageAddressError)

    @property
    def _is_ready(self):
        status = self._status
        return (status == kWaitingForProgrammer) or (status == kReadyForPage)

    @property
    def _pages(self):
        return len(self._bytes) / self._pagesize



parser = argparse.ArgumentParser(description='LIN Nodes firmware updater')
parser.add_argument('--fw', required=True, action='store', help='firmware bundle')
parser.add_argument('--recover', action='store', help='recover a node stuck in bootloader mode')
args = parser.parse_args()

link = Link()
Bundle.load(args.fw)

# Deal with a single node already in the bootloader
if args.recover is not None:
    Node.scan(link, [])
    node = Node(32, Bundle.id_for_name(args.recover))
    node.update()
    sys.exit(0)

Node.scan(link)
Node.update_all()
