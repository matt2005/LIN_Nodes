/*
 * Copyright (c) 2012-2015, Mike Smith, <msmith@purgatory.org>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 * o Redistributions of source code must retain the above copyright notice,
 *   this list of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright
 *   notice, this list of conditions and the following disclaimer in the
 *   documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <lin_dev.h>

/* polled-mode LIN slave */
class BLSlave : public LINDev
{
public:
    BLSlave() : LINDev(true) {}

    static bool         is_program_valid();
    static bool         is_bootloader_forced();
    static void         run_program() __attribute__((__noreturn__));

protected:
    virtual void        st_header_received() override;
    virtual void        st_response_received(Response &frame) override;

private:
    static const uint16_t kNoSendResponse = 0xffff;

    static uint8_t      _nad;
    static uint16_t     _sendIndex;
    static uint16_t     _pageAddress;
    static uint8_t      _pageOffset;
    static uint16_t     _runningCrc;
    static uint16_t     _pageStatus;
    static uint16_t     _programEnd;
    static uint16_t     _resetVector;
    static uint16_t     _pageBuffer[];
    static uint16_t     _memoryPointer;
    static uint8_t      _reason;

    static uint8_t      nad();
    static uint8_t      function();
    static void         set_page_address(uint16_t address);
    static bool         add_page_byte(uint8_t byte);
    static void         program_page();
    static void         update_program_info();
    static uint16_t     get_program_crc(uint16_t length);

    void                send_response();
};
