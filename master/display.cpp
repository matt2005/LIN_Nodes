
#include "USI_TWI_Master.h"
#include "display.h"

bool
Display::init()
{
    USI_TWI_Master_Initialise();

    // try to talk to the board and read its ID
    uint8_t id;
    if (!sendCommand(kGetID) ||
        !getResponse(id) ||
        (id != kBoardID)) {
        return false;
    }
    return true;
}

uint8_t
Display::getButtons()
{
    sendCommand(kGetButtons);
    uint8_t buttons = 0;
    getResponse(buttons);
    return buttons;
}

void
Display::write(uint8_t x, uint8_t y, char c)
{
    _x = x;
    _y = y;
    write(c);
}

void
Display::write(char c)
{
    sendCommand(kLCDWrite, _y, _x, (uint8_t)c);
    if (++_x >= kWidth) {
        _x = 0;
        if (++_y >= kHeight) {
            _y = 0;
        }
    }
}

void
Display::write(uint8_t x, uint8_t y, const char *s)
{
    _x = x;
    _y = y;
    write(s);
}

void
Display::write(const char *s)
{
    while (*s != '\0') {
        write(*s++);
    }
}

void
Display::writeP(uint8_t x, uint8_t y, PGM_P s)
{
    _x = x;
    _y = y;
    writeP(s);
}

void
Display::writeP(PGM_P s)
{
    while (*s != '\0') {
        write(pgm_read_byte(s++));
    }
}

bool
Display::sendCommand(Command cmd)
{
    uint8_t msg[2] = { kWriteAddress, cmd };

    return USI_TWI_Start_Transceiver_With_Data(msg, 2) == TRUE;
}

bool
Display::sendCommand(Command cmd, uint8_t param0)
{
    uint8_t msg[3] = { kWriteAddress, cmd, param0 };

    return USI_TWI_Start_Transceiver_With_Data(msg, sizeof(msg)) == TRUE;
}

bool
Display::sendCommand(Command cmd, uint8_t param0, uint8_t param1)
{
    uint8_t msg[4] = { kWriteAddress, cmd, param0, param1};

    return USI_TWI_Start_Transceiver_With_Data(msg, sizeof(msg)) == TRUE;
}

bool
Display::sendCommand(Command cmd, uint8_t param0, uint8_t param1, uint8_t param2)
{
    uint8_t msg[5] = { kWriteAddress, cmd, param0, param1, param2};

    return USI_TWI_Start_Transceiver_With_Data(msg, sizeof(msg)) == TRUE;
}

bool
Display::getResponse(uint8_t &value)
{
    uint8_t msg[2] = { kReadAddress, 0 };

    uint8_t result = USI_TWI_Start_Transceiver_With_Data(msg, sizeof(msg));

    if (result == TRUE) {
        value = msg[1];
        return true;
    }
    return false;
}

