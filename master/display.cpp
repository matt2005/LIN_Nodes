
#include <util/delay.h>
#include <avr/wdt.h>
#include <avr/interrupt.h>

#include "USI_TWI_Master.h"
#include "display.h"

#include "board.h"

Display::Display()
{
    // the display takes some time to boot, so wait for it...
    Board::msDelay(500);

    for (uint8_t tries = 0; tries < 5; tries++) {

        USI_TWI_Master_Initialise();

        // ping the display
        uint8_t pkt[] = { 0x00, 1, 0x5a };
        if (send(&pkt[0]) == FALSE) {
            continue;
        }
        Board::msDelay(5);

        // check the ping response
        uint8_t buf[3];
        if ((recv(&buf[0], 3) == FALSE) || (buf[0] != 0x40) || (buf[1] != 1) || (buf[2] != 0x5a)) {
            continue;
        }
        clear();
        return;
    }
    Board::panic(3);
}

void
Display::clear()
{
    uint8_t pkt[] = { kOPClear, 0 };
    send(&pkt[0]);
    waitAck(kOPClear);
    move(0, 0);
}

void
Display::setBacklight(uint8_t value)
{
    uint8_t pkt[] = { kOPBacklight, 1, value };
    send(&pkt[0]);
    waitAck(kOPBacklight);
}

Display::Button
Display::getButtonPress()
{
    uint8_t pkt[] = { 0x18, 0 };
    send(&pkt[0]);

    uint8_t buf[5];
    recv(&buf[0], sizeof(buf));

    return (Button)buf[3];
}

bool
Display::send(const uint8_t *pkt)
{
    uint8_t len = pkt[1] + 5;
    uint8_t buf[len];
    bool result;

    buf[0] = kWriteAddress;

    // copy packet
    for (uint8_t i = 0; i < (len - 3); i++) {
        buf[1 + i] = pkt[i];
    }

    // generate packet CRC
    crc(buf + 1);

    cli();
    result = USI_TWI_Start_Transceiver_With_Data(buf, len) == TRUE;
    sei();

    return result;
}

bool
Display::recv(uint8_t *pkt, uint8_t pktlen)
{
    bool result;
    uint8_t len = pktlen + 3;
    uint8_t buf[len];

    buf[0] = kReadAddress;

    result = USI_TWI_Start_Transceiver_With_Data(buf, len);

    if (result == TRUE) {
        for (uint8_t i = 0; i < pktlen; i++) {
            pkt[i] = buf[i + 1];
        }
    }
    // XXX check checksum?

    return result;
}

bool
Display::waitAck(uint8_t opcode)
{
    uint8_t buf[2];

    if (!recv(&buf[0], 2)) {
        return false;
    }
    if (buf[0] != (opcode | kACK)) {
        return false;
    }
    return true;
}

void
Display::crc(uint8_t *ptr)
{
    uint8_t  count; // packet size
    uint16_t crc;   // calculated CRC
    uint8_t  i;     // loop count, bits in byte
    uint8_t  data;  // current byte being shifted

    crc = 0xFFFF;   // Preset to all 1's, prevent loss of leading zeros
    count = ptr[1] + 2; // get count from packet

    while (count--) {
        data = *ptr++;
        i = 8;
        do {
            if ((crc ^ data) & 0x01) {
                crc >>= 1;
                crc ^= 0x8408;
            } else {
                crc >>= 1;
            }
            data >>= 1;
        } while (--i != 0);
    }
    crc = ~crc;
    *ptr++ = crc & 0xff;
    *ptr++ = crc >> 8;
}

void
Display::_write(const char *s, Reader r)
{
    uint8_t pkt[20] = { kOPWrite, 2, _x, _y };

    // copy bytes via the reader into the packet
    for (uint8_t pos = 4; (pos < 20); pos++) {
        uint8_t c = r(s++);
        if (c == 0) {
            break;
        }
        pkt[pos] = c;
        pkt[1]++;
        _x++;
    }

    // send packet and wait for an acknowledgement
    send(&pkt[0]);
    waitAck(kOPWrite);
}
