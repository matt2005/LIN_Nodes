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

#pragma once

#pragma pack(push, 1)

enum USBRequest {
    kUSBRequestStatus,              // wValue = kind index or ignored, wIndex = status kind, IN = uint8_t
    kUSBRequestClearStatus,         // wValue = ignored, wIndex = status kind
    kUSBRequestGetHistory,          // wValue = ignored, wIndex = ignored, IN = RQHistory
    kUSBRequestSelectNode,          // wValue = node address, wIndex = ignored
    kUSBRequestReadData,            // wValue = ignored, wIndex = parameter address
    kUSBRequestReadResult,          // wValue = ignored, wIndex = parameter address, IN = uint16_t
    kUSBRequestWriteData,           // wValue = value, wIndex = parameter address
    kUSBRequestSendBulk,            // wValue = data0,1, wIndex = data2,3
    kUSBRequestEnableMaster,        // wValue = 1 to enable, 0 to disable
};

#define RQ_STATUS_FLAGS         0
#define RQ_STATUS_DATA_READY        (1<<0)
#define RQ_STATUS_DATA_ERROR        (1<<1)
#define RQ_STATUS_DATA_REJECTED     (1<<2)
#define RQ_STATUS_AWAKE             (1<<3)
#define RQ_STATUS_WAITING           (1<<4)
#define RQ_STATUS_MASTER            (1<<5)

#define RQ_STATUS_FREEMEM       1
#define RQ_STATUS_LINERR        2   // wValue = error counter index

// kUSBRequestGetHistory -> IN
struct RQHistory {
    uint16_t time;                  // internal milliseconds time the frame started
    uint8_t frame[9];               // frame data bytes including FID
#define RQ_HISTORY_FRAME_VALID      (1<<7)  //< set in frame[0] when the response is valid
#define RQ_HISTORY_RESPONSE_VALID   (1<<6)  //< set in frame[0] when the response is valid
#define RQ_HISTORY_FID_MASK         (0x3f)  //< mask for frame[0] to extract FID
};

#pragma pack(pop)
