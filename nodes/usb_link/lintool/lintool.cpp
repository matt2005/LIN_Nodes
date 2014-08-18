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
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <err.h>

#include <stdexcept>

#include "link.h"

#include "../../../common/lin_defs.h"


Link *gLink;

void
log_history(bool forever = true)
{
    for (;;) {
        struct RQHistory hist;

        int result = gLink->request_in(kUSBRequestGetHistory, 0, 0, (unsigned char *)&hist, sizeof(hist));

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
                        resp.SlaveResponse.d1,
                        Encoding::info(kEncoding_service_id, resp.SlaveResponse.d1) ? : "unknown",
                        resp.SlaveResponse.d2,
                        Encoding::info(kEncoding_service_error, resp.SlaveResponse.d2));
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
clear_history()
{
    for (;;) {
        uint8_t frame[9];

        int result = gLink->request_in(kUSBRequestGetHistory, 0, 0, frame, 9);

        if (result < 0) {
            throw(std::runtime_error("trace: USB error"));
        }

        if (result == 0) {
            return;
        }
    }
}

void
print_status()
{    
    warnx("free memory: %u", gLink->get_status(RQ_STATUS_FREEMEM));
    uint8_t status = gLink->get_status();
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
    warnx("  line:            %u", gLink->get_status(RQ_STATUS_LINERR, 0));
    warnx("  checksum:        %u", gLink->get_status(RQ_STATUS_LINERR, 1));
    warnx("  parity:          %u", gLink->get_status(RQ_STATUS_LINERR, 2));
    warnx("  framing:         %u", gLink->get_status(RQ_STATUS_LINERR, 3));
    warnx("  synchronisation: %u", gLink->get_status(RQ_STATUS_LINERR, 4));
    warnx("  protocol:        %u", gLink->get_status(RQ_STATUS_LINERR, 5));
}

void
print_parameters()
{
    gLink->enable_master(true);
    gLink->set_node(1);

    uint16_t value = 0xffff;
    try {
        value = gLink->read_data(Generic::kParamProtocolVersion);
        warnx("%d: got %d", 1, value);
    } catch(std::runtime_error &re) {
        warnx("read failed: %s", re.what());
        log_history(false);        
    }

    gLink->enable_master(false);

}



int
main(int argc, const char *argv[])
{
    gLink = new Link;
    
    gLink->connect();
    clear_history();

    if (argc == 2) {
        if (!strcmp(argv[1], "-status")) {
            print_status();
            exit(0);
        }
        if (!strcmp(argv[1], "-trace")) {
            log_history();
        }
        if (!strcmp(argv[1], "-master")) {
            gLink->enable_master(true);
            exit(0);
        }
        if (!strcmp(argv[1], "-dump")) {
            print_parameters();
            exit(0);
        }
    }

    errx(1, "must supply one of -trace, -dump ...");
}

