
//
// LIN network programmer
//
// Master polls the programmer with MasterRequest/DataDump/DDIDGetRequest.
//
// Programmer responds with DataDump/ReportRequest/GetParameter/NAD/Parameter or
// DataDump/ReportRequest/SetParameter/NAD/Parameter/Value if it wants to read/write
// parameters.
//
// Master will then run NAD/DataDump/GetParameter/Parameter or
// NAD/DataDump/SetParameter/Parameter/Value. For GetParameter, the addressed
// slave will then respond with ReportParameter/Parameter/Value, which is snooped
// by the programmer to obtain the value.
//
// In the special case where the NAD for references the master, SetParameter is
// handled by the master internally, and SetParameter will cause the master to send
// the ReportParameter response itself.
//
// Programmer uses GetParameter/NAD/0 to probe for nodes on the network.
//

#include <avr/wdt.h>

#include "board.h"

#include "hd44780.h"

HD44780 lcd;
Display &gDisplay = lcd;

void
main(void)
{
    Board::init();
    lcd.init();

    gDisplay.printf(PSTR("test"));

    unsigned i = 0;

    for (;;) {
        wdt_reset();

        gDisplay.move(0, 1);
        gDisplay.printf(PSTR("count %u    "), i++);
    }

}

