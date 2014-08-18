/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <msmith@purgatory.org> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return.
 * ----------------------------------------------------------------------------
 */

#include <stdexcept>

#include "log.h"
#include "link.h"

#include "../../../common/lin_defs.h"

namespace Log 
{

void
log(bool forever)
{
    for (;;) {
        struct RQHistory hist;

        int result = Link::request_in(kUSBRequestGetHistory, 0, 0, (unsigned char *)&hist, sizeof(hist));

        if (result < 0) {
            throw(std::runtime_error("trace: USB error"));
        }
        if (result == 0) {
            if (!forever) {
                return;
            }
            continue;
        }
        if (result != sizeof(hist)) {
            throw(std::runtime_error("trace: bad packet size"));
        }

        uint8_t fid = hist.frame[0] & RQ_HISTORY_FID_MASK;
        const char *fname = Frame::name(fid);

        printf("%05d:%02x: ", hist.time, fid);

        if (hist.frame[0] & RQ_HISTORY_RESPONSE_VALID) {
            printf("%02x %02x %02x %02x %02x %02x %02x %02x : <%s>\n",
                   hist.frame[1],
                   hist.frame[2],
                   hist.frame[3],
                   hist.frame[4],
                   hist.frame[5],
                   hist.frame[6],
                   hist.frame[7],
                   hist.frame[8],
                   fname ? : "unknown");

            Response resp;
            resp._bytes[0] = hist.frame[1];
            resp._bytes[1] = hist.frame[2];
            resp._bytes[2] = hist.frame[3];
            resp._bytes[3] = hist.frame[4];
            resp._bytes[4] = hist.frame[5];
            resp._bytes[5] = hist.frame[6];
            resp._bytes[6] = hist.frame[7];
            resp._bytes[7] = hist.frame[8];

            switch (fid) {
            case kFrameIDMasterRequest:
                printf("                                nad: %u pci: %u length: %u sid: 0x%02x <%s>\n",
                    resp.MasterRequest.nad,
                    resp.MasterRequest.pci,
                    resp.MasterRequest.length,
                    resp.MasterRequest.sid,
                    Encoding::info(kEncoding_service_id, resp.MasterRequest.sid) ? : "unknown");
                printf("                               ");
                if (resp.MasterRequest.length >= 2) {
                    printf(" d1: %u", resp.MasterRequest.d1);
                }
                if (resp.MasterRequest.length >= 3) {
                    printf(" d2: %u", resp.MasterRequest.d2);
                }
                if (resp.MasterRequest.length >= 4) {
                    printf(" d3: %u", resp.MasterRequest.d3);
                }
                if (resp.MasterRequest.length >= 5) {
                    printf(" d4: %u", resp.MasterRequest.d4);
                }
                if (resp.MasterRequest.length >= 6) {
                    printf(" d5: %u", resp.MasterRequest.d5);
                }
                printf("\n");
                break;
            case kFrameIDSlaveResponse:
                printf("                                nad: %u pci: %u length: %u",
                    resp.SlaveResponse.nad,
                    resp.SlaveResponse.pci,
                    resp.SlaveResponse.length);
                if (resp.SlaveResponse.sid == service_id::kErrorResponse) {
                    printf(" sid: 0x%02x <%s> error: 0x%02x <%s>\n",
                        resp.ServiceError.original_sid,
                        Encoding::info(kEncoding_service_id, resp.ServiceError.original_sid) ? : "unknown",
                        resp.ServiceError.error,
                        Encoding::info(kEncoding_service_error, resp.ServiceError.error));
                } else if (resp.SlaveResponse.sid & service_id::kResponseOffset) {
                    uint8_t sid = resp.SlaveResponse.sid & ~service_id::kResponseOffset;
                    printf(" sid: 0x%02x <%s>\n",
                        resp.SlaveResponse.sid,
                        Encoding::info(kEncoding_service_id, sid) ? : "unknown");
                    printf("                               ");
                    if (resp.SlaveResponse.length >= 2) {
                        printf(" d1: %u", resp.SlaveResponse.d1);
                    }
                    if (resp.SlaveResponse.length >= 3) {
                        printf(" d2: %u", resp.SlaveResponse.d2);
                    }
                    if (resp.SlaveResponse.length >= 4) {
                        printf(" d3: %u", resp.SlaveResponse.d3);
                    }
                    if (resp.SlaveResponse.length >= 5) {
                        printf(" d4: %u", resp.SlaveResponse.d4);
                    }
                    if (resp.SlaveResponse.length >= 6) {
                        printf(" d5: %u", resp.SlaveResponse.d5);
                    }
                    printf("\n");
                } else {
                    printf(" sid 0x%02x <malformed>\n", resp.SlaveResponse.sid);
                }
                break;
            }
        } else {
            printf("                          <%s>\n",
                   fname ? : "unknown");
        }
    }
}

void
clear()
{
    for (;;) {
        RQHistory hist;

        int result = Link::request_in(kUSBRequestGetHistory, 0, 0, (uint8_t *)&hist, sizeof(hist));

        if (result < 0) {
            throw(std::runtime_error("trace: USB error"));
        }

        if (result == 0) {
            return;
        }
    }
}

} // namespace Log

