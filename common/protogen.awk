#!/usr/bin/awk
#
# Process the protocol.txt file and generate the protocol.h header
#

BEGIN {
    printf "// AUTOMATICALLY GENERATED - DO NOT EDIT\n"
    n_frames = 0
    n_switches = 0
    n_relays = 0
}

$1 == "Revision" {
    revision = $2
}

$1 == "Frames:" {
    mode = "frames"
    next
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
        if (mode == "frames") {
            frame_names[n_frames] = $1
            frame_ids[n_frames] = $2
            frame_lengths[n_frames] = $3
            n_frames++
        }
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
    printf "#pragma once\n"
    printf "#ifdef __AVR__\n"
    printf "# include <avr/pgmspace.h>\n"
    printf "#else\n"
    printf "# define PROGMEM\n"
    printf "#endif\n"
    printf "#include <stdint.h>\n\n"
    printf "namespace LIN {\n\n"

    printf "static const uint8_t protocolRevision = %d;\n\n", revision

    printf "enum FrameID : uint8_t {\n"
    printf "    kFIDNone = 0x00,\n"
    for (i = 0; i < n_frames; i++) {
        printf "    kFID%s = 0x%02x,\n", frame_names[i], frame_ids[i]
    }
    printf "};\n\n"

    printf "enum FrameLen : uint8_t {\n"
    for (i = 0; i < n_frames; i++) {
        printf "    kFLen%s = 0x%02x,\n", frame_names[i], frame_lengths[i]
    }
    printf "};\n\n"

    printf "enum SwitchID : uint8_t {\n"
    for (i = 0; i < n_switches; i++) {
        printf "    kSW%s = %d,\n", switches[i], i
    }
    printf "    kSWMax,\n"
    printf "    kSWNone = 0xff,\n"
    printf "};\n\n"

    printf "enum RelayID : uint8_t {\n"
    for (i = 0; i < n_relays; i++) {
        printf "    kRelay%s = %d,\n", relays[i], i
    }
    printf "    kRelayMax,\n"
    printf "    kRelayNone = 0xff,\n"
    printf "};\n\n"

    printf "} //namespace LIN\n\n"

    printf "static PROGMEM const char switchNames[] = \n"
    for (i = 0; i < n_switches; i++) {
        printf "    \"%s\\0\"\n", switches[i]
    }
    printf "    \"\\0\";\n\n"

    printf "static PROGMEM const char relayNames[] = \n"
    for (i = 0; i < n_relays; i++) {
        printf "    \"%s\\0\"\n", relays[i]
    }
    printf "    \"\\0\";\n\n"

}
