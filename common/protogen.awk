#!/usr/bin/awk
#
# Process the protocol.txt file and generate the protocol.h header
#

BEGIN {
    printf "// AUTOMATICALLY GENERATED - DO NOT EDIT\n"
    n_switches = 0
    n_relays = 0
}

$1 == "Switches:" {
    mode = "switches"
    next
}

$1 == "Relays:" {
    mode = "relays"
    next
}

{
    if ($1 != "") {
        if (mode == "switches") {
            switches[n_switches] = $1
            n_switches++
        }
        if (mode == "relays") {
            relays[n_relays] = $1
            n_relays++
        }
    }
}

END {
    printf("#include <avr/pgmspace.h>\n\n")
    printf("namespace LIN {\n\n")

    printf("enum SwitchID : uint8_t {\n")
    for (i = 0; i < n_switches; i++) {
        printf "    kSW%s = %d,\n", switches[i], i
    }
    printf("    kSWMax,\n")
    printf("    kSWNone = 0xff,\n")
    printf("};\n\n")

    printf("enum RelayID : uint8_t {\n")
    for (i = 0; i < n_relays; i++) {
        printf "    kRelay%s = %d,\n", relays[i], i
    }
    printf("    kRelayMax,\n")
    printf("    kRelayNone = 0xff,\n")
    printf("};\n\n")

    printf("} //namespace LIN\n\n")

    printf("static PROGMEM const char switchNames[] = \n")
    for (i = 0; i < n_switches; i++) {
        printf "    \"%s\\0\"\n", switches[i]
    }
    printf "    \"\\0\";\n\n"

    printf("static PROGMEM const char relayNames[] = \n")
    for (i = 0; i < n_relays; i++) {
        printf "    \"%s\\0\"\n", relays[i]
    }
    printf "    \"\\0\";\n\n"

}
