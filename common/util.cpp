
#include <avr/pgmspace.h>

#include "util.h"

namespace Util
{

const char *
strtab(const char *table, uint8_t index)
{
    const char *p = table;

    while (index-- > 0) {
        // end of table?
        if (pgm_read_byte(p) == '\0') {
            return nullptr;
        }
        // skip past next nul
        while (pgm_read_byte(p++) != '\0') {
        }
    }
    return p;
}

uint8_t
strtablen(const char *table)
{
    const char *p = table;
    uint8_t count = 0;

    for (;;) {
        if (pgm_read_byte(p) == '\0') {
            break;
        }
        while (pgm_read_byte(p++) != '\0') {
        }
        count++;
    }
    return count;
}


} // namespace Util
