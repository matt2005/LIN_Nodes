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

#include <err.h>

#include <list>
#include <stdexcept>

#include <lin_defs.h>

#include "log.h"
#include "link.h"

namespace Log
{

unsigned enable;

class Entry
{
public:
    Entry(RQHistory &hist) :
        _time(hist.time),
        _fid(hist.frame[0] & RQ_HISTORY_FID_MASK),
        _responseValid(hist.frame[0] & RQ_HISTORY_RESPONSE_VALID)
    {
        _resp._bytes[0] = hist.frame[1];
        _resp._bytes[1] = hist.frame[2];
        _resp._bytes[2] = hist.frame[3];
        _resp._bytes[3] = hist.frame[4];
        _resp._bytes[4] = hist.frame[5];
        _resp._bytes[5] = hist.frame[6];
        _resp._bytes[6] = hist.frame[7];
        _resp._bytes[7] = hist.frame[8];
    }

    void            print();
    bool            matches(std::vector<unsigned> &filter);

private:
    uint16_t        _time;
    uint8_t         _fid;
    bool            _responseValid;
    Response        _resp;

    const char      *frame_name() const { return Frame::name(_fid) ? : "unknown"; }
};

std::list<Entry *>    history;

const char pad[] = "                                      ";

void
acquire()
{
    if (!enable) {
        return;
    }

    for (;;) {
        struct RQHistory hist;

        auto result = Link::request_in(kUSBRequestGetHistory, 0, 0, (unsigned char *)&hist, sizeof(hist));

        if (result < 0) {
            RAISE(Link::ExUSBFailed, "acquire: USB error " << result);
        }

        if (result == 0) {
            return;
        }

        if (result != sizeof(hist)) {
            RAISE(Link::ExLinkFailed, "bad trace packet size" << result);
        }

        auto ent = new Entry(hist);
        history.push_back(ent);
    }
}

void
print(std::vector<unsigned> &filter)
{
    for (;;) {
        auto ent = history.front();

        if (ent == nullptr) {
            break;
        }

        history.pop_front();

        if (ent->matches(filter)) {
            ent->print();
        }

        delete ent;
    }
}

void
print()
{
    std::vector<unsigned> filter;

    print(filter);
}

void
clear()
{
    acquire();

    for (;;) {
        auto ent = history.front();

        if (ent == nullptr) {
            break;
        }

        history.pop_front();
        delete ent;
    }
}

void
trace(std::vector<unsigned> &filter)
{
    enable = true;

    for (;;) {
        acquire();
        print(filter);
    }
}

void
Entry::print()
{
    if ((enable < 2) && (!_responseValid)) {
        return;
    }

    printf("%05d:%02x: ", _time, _fid);

    if (_responseValid) {
        printf("%02x %02x %02x %02x %02x %02x %02x %02x : %s",
               _resp._bytes[0],
               _resp._bytes[1],
               _resp._bytes[2],
               _resp._bytes[3],
               _resp._bytes[4],
               _resp._bytes[5],
               _resp._bytes[6],
               _resp._bytes[7],
               frame_name());

        switch (_fid) {
        case kFrameIDMasterRequest:
            printf(" nad: %u pci: %u length: %u sid: 0x%02x",
                   _resp.MasterRequest.nad,
                   _resp.MasterRequest.pci,
                   _resp.MasterRequest.length,
                   _resp.MasterRequest.sid);

            if (_resp.MasterRequest.length >= 2) {
                printf(" d1: %u", _resp.MasterRequest.d1);

                if (_resp.MasterRequest.length >= 3) {
                    printf(" d2: %u", _resp.MasterRequest.d2);

                    if (_resp.MasterRequest.length >= 4) {
                        printf(" d3: %u", _resp.MasterRequest.d3);

                        if (_resp.MasterRequest.length >= 5) {
                            printf(" d4: %u", _resp.MasterRequest.d4);

                            if (_resp.MasterRequest.length >= 6) {
                                printf(" d5: %u", _resp.MasterRequest.d5);
                            }
                        }
                    }
                }
            }

            printf("\n%s %s",
                   pad,
                   Encoding::info(kEncoding_service_id, _resp.MasterRequest.sid) ? : "unknown");

            switch (_resp.MasterRequest.sid) {
            case service_id::kReadDataByID:
                printf(" index: 0x%04x", _resp.DataByID.index);
                break;

            case service_id::kWriteDataByID:
                printf(" index: 0x%04x value: 0x%04x", _resp.DataByID.index, _resp.DataByID.value);
                break;

            case service_id::kDataDump:
                printf(" {%02x,%02x,%02x,%02x}",
                       _resp.MasterRequest.d1,
                       _resp.MasterRequest.d2,
                       _resp.MasterRequest.d3,
                       _resp.MasterRequest.d4);
                break;
            }

            break;

        case kFrameIDSlaveResponse:
            printf(" nad: %u pci: %u length: %u",
                   _resp.SlaveResponse.nad,
                   _resp.SlaveResponse.pci,
                   _resp.SlaveResponse.length);

            if (_resp.SlaveResponse.sid == service_id::kErrorResponse) {
                printf(" sid: 0x%02x %s error: 0x%02x %s\n",
                       _resp.ServiceError.original_sid,
                       Encoding::info(kEncoding_service_id, _resp.ServiceError.original_sid) ? : "unknown",
                       _resp.ServiceError.error,
                       Encoding::info(kEncoding_service_error, _resp.ServiceError.error));

            } else if (_resp.SlaveResponse.sid & service_id::kResponseOffset) {
                uint8_t sid = _resp.SlaveResponse.sid & ~service_id::kResponseOffset;
                printf(" sid: 0x%02x", _resp.SlaveResponse.sid);

                if (_resp.SlaveResponse.length >= 2) {
                    printf(" d1: %u", _resp.SlaveResponse.d1);

                    if (_resp.SlaveResponse.length >= 3) {
                        printf(" d2: %u", _resp.SlaveResponse.d2);

                        if (_resp.SlaveResponse.length >= 4) {
                            printf(" d3: %u", _resp.SlaveResponse.d3);

                            if (_resp.SlaveResponse.length >= 5) {
                                printf(" d4: %u", _resp.SlaveResponse.d4);

                                if (_resp.SlaveResponse.length >= 6) {
                                    printf(" d5: %u", _resp.SlaveResponse.d5);
                                }
                            }
                        }
                    }
                }

                printf("\n%s %s",
                       pad,
                       Encoding::info(kEncoding_service_id, sid) ? : "unknown");

                switch (sid) {
                case service_id::kReadDataByID:
                    printf(" index: 0x%04x value: 0x%04x", _resp.DataByID.index, _resp.DataByID.value);
                    break;
                }


            } else {
                printf(" sid 0x%02x <malformed>", _resp.SlaveResponse.sid);
            }

            break;
        }

        printf("\n");

    } else {
        printf("                          %s\n", frame_name());
    }
}

bool
Entry::matches(std::vector<unsigned> &filter)
{
    // always match an empty filter
    if (filter.size() == 0) {
        return true;
    }

    for (auto fid : filter) {
        if (fid == _fid) {
            return true;
        }
    }

    return false;
}

} // namespace Log

