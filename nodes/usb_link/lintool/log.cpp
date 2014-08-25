/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <msmith@purgatory.org> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return.
 * ----------------------------------------------------------------------------
 */

#include <stdexcept>
#include <list>
#include <err.h>
#include "log.h"
#include "link.h"

#include "../../../common/lin_defs.h"

namespace Log
{

bool enable;

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
            throw (std::runtime_error("trace: USB error"));
        }

        if (result == 0) {
            return;
        }

        if (result != sizeof(hist)) {
            throw (std::runtime_error("trace: bad packet size"));
        }

        auto ent = new Entry(hist);
        history.push_back(ent);
    }
}

void
print()
{
    for (;;) {
        auto ent = history.front();

        if (ent == nullptr) {
            break;
        }
        history.pop_front();
        ent->print();
        delete ent;
    }
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
trace()
{
    enable = true;
    for (;;) {
        acquire();
        print();
    }
}

void
Entry::print()
{
    printf("%05d:%02x: ", _time, _fid);

    if (_responseValid) {
        printf("%02x %02x %02x %02x %02x %02x %02x %02x : <%s>\n",
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
            printf("%s nad: %u pci: %u length: %u sid: 0x%02x <%s>\n",
                   pad,
                   _resp.MasterRequest.nad,
                   _resp.MasterRequest.pci,
                   _resp.MasterRequest.length,
                   _resp.MasterRequest.sid,
                   Encoding::info(kEncoding_service_id, _resp.MasterRequest.sid) ? : "unknown");
            printf("%s", pad);

            if (_resp.MasterRequest.length >= 2) {
                printf(" d1: %u", _resp.MasterRequest.d1);
            }

            if (_resp.MasterRequest.length >= 3) {
                printf(" d2: %u", _resp.MasterRequest.d2);
            }

            if (_resp.MasterRequest.length >= 4) {
                printf(" d3: %u", _resp.MasterRequest.d3);
            }

            if (_resp.MasterRequest.length >= 5) {
                printf(" d4: %u", _resp.MasterRequest.d4);
            }

            if (_resp.MasterRequest.length >= 6) {
                printf(" d5: %u", _resp.MasterRequest.d5);
            }

            printf("\n");
            break;

        case kFrameIDSlaveResponse:
            printf("%s nad: %u pci: %u length: %u",
                   pad,
                   _resp.SlaveResponse.nad,
                   _resp.SlaveResponse.pci,
                   _resp.SlaveResponse.length);

            if (_resp.SlaveResponse.sid == service_id::kErrorResponse) {
                printf(" sid: 0x%02x <%s> error: 0x%02x <%s>\n",
                       _resp.ServiceError.original_sid,
                       Encoding::info(kEncoding_service_id, _resp.ServiceError.original_sid) ? : "unknown",
                       _resp.ServiceError.error,
                       Encoding::info(kEncoding_service_error, _resp.ServiceError.error));

            } else if (_resp.SlaveResponse.sid & service_id::kResponseOffset) {
                uint8_t sid = _resp.SlaveResponse.sid & ~service_id::kResponseOffset;
                printf(" sid: 0x%02x <%s>\n",
                       _resp.SlaveResponse.sid,
                       Encoding::info(kEncoding_service_id, sid) ? : "unknown");
                printf("%s", pad);

                if (_resp.SlaveResponse.length >= 2) {
                    printf(" d1: %u", _resp.SlaveResponse.d1);
                }

                if (_resp.SlaveResponse.length >= 3) {
                    printf(" d2: %u", _resp.SlaveResponse.d2);
                }

                if (_resp.SlaveResponse.length >= 4) {
                    printf(" d3: %u", _resp.SlaveResponse.d3);
                }

                if (_resp.SlaveResponse.length >= 5) {
                    printf(" d4: %u", _resp.SlaveResponse.d4);
                }

                if (_resp.SlaveResponse.length >= 6) {
                    printf(" d5: %u", _resp.SlaveResponse.d5);
                }

                printf("\n");

            } else {
                printf(" sid 0x%02x <malformed>\n", _resp.SlaveResponse.sid);
            }

            break;
        }

    } else {
        printf("                          <%s>\n", frame_name());
    }
}

} // namespace Log

