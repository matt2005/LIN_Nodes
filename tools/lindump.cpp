#include <stdio.h>
#include <termios.h>
#include <err.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdint.h>

#include "../common/lin_protocol.h"

static void decode(uint8_t *buf, unsigned len);

int
main(int argc, const char *argv[])
{
    int     fd;

    if ((fd = open(argv[1], O_RDONLY)) < 0)
        err(1, "open");

    struct termios t;
    if (tcgetattr(fd, &t) < 0)
        err(1, "tcgetattr");
    cfsetspeed(&t, 19200);
    if (tcsetattr(fd, TCSANOW, &t) < 0)
        err(1, "tcsetattr");

    uint8_t buf[32];
    unsigned len = 0;

    for (;;) {

        if ((read(fd, &buf[len++], 1)) < 1)
            err(1, "read");

        // if we just saw 0x00,0x55 then it's almost certainly a frame header; back up
        // and print everything prior
        if ((len >= 2) && (buf[len - 1] == 0x55) && (buf[len - 2] == 0x00)) {
            printf("%02x : ", buf[0] & 0x3f);
            for (unsigned i = 1; i <= 9; i++) {
                if (i < (len - 2)) {
                    printf("%02x ", buf[i]);
                } else {
                    printf("-- ");
                }
            }
            decode(&buf[0], len - 2);
            printf("\n");
            len = 0;            
        }

        if (len >= sizeof(buf)) {
            // overflow; dump the buffer and start again
            len = 0;
        }
    }
}

void
decode(uint8_t *buf, unsigned len)
{
    LIN::Frame f(buf + 1);

    switch(buf[0] & 0x3f) {
    case LIN::kFrameIDRelays:
        printf("relays\n");
        break;
    case LIN::kFrameIDECUData:
        printf("ECU data");
        break;
    case LIN::kFrameIDProxyRequest:
        printf("proxy ");
        // FALLTHROUGH

    case LIN::kFrameIDMasterRequest:
        printf("request ");
        if (len < 8) {
            printf("<short>");
            break;
        }
        printf("to node %u ", f.nad());
        switch(f.sid()) {
        case LIN::kServiceIDReadDataByID:
            printf("ReadDataByID %u", f.d1() + (f.d2() << 8));
            break;
        case LIN::kServiceIDWriteDataByID:
            printf("WriteDataByID %u = %u", f.d1() + (f.d2() << 8), f.d3() + (f.d4() << 8));
            break;
        case LIN::kServiceIDTesterPresent:
            printf("TesterPresent ");
            break;
        case LIN::kServiceIDReadByID:
            printf("ReadByID ");
            break;
        case LIN::kServiceIDDataDump:
            printf("DataDump ");
            break;
        }
        break;

    case LIN::kFrameIDSlaveResponse:
        printf("response ");
        if (len < 8) {
            printf("<short>");
            break;
        }
        printf("from node %u ", f.nad());
        switch(f.sid() & ~LIN::kServiceIDResponseOffset) {
        case LIN::kServiceIDReadDataByID:
            printf("ReadDataByID %u = %u", f.d1() + (f.d2() << 8), f.d3() + (f.d4() << 8));
            break;
        case LIN::kServiceIDWriteDataByID:
            printf("WriteDataByID %u", f.d1() + (f.d2() << 8));
            break;
        case LIN::kServiceIDTesterPresent:
            printf("TesterPresent");
            break;
        case LIN::kServiceIDReadByID:
            printf("ReadByID ");
            break;
        case LIN::kServiceIDDataDump:
            printf("DataDump ");
            break;
        case LIN::kServiceIDErrorResponse:
            printf("ERROR %2x,%2x", f.d1(), f.d2());
            break;
        }
        break;
    default:
        printf("??");
    }
}
