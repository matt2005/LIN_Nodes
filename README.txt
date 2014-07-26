Firmware and PCB designs for various LIN network nodes intended for retrofitting older vehicles.

boards/
    Eagle schematic and PCB files.
common/
    Protocol definitions and generator.
dashboard/
    Dashboard display using a 64x32 RGB LED panel driven by an NXP LPC1114.
Documentation/
    Various datasheets relevant to the nodes.
nodes/
    All of the nodes in this part of the tree are based on the Atmel ATTiny167.
    lib/
        Shared code.
    master/
        Network master node.
    power_v1/
        Simple 4-output power switching node using an ST driver. Low current due to
        high Rdson FETs in the ST family.
    power_v3/
        5-output power switching node using Freescale smart high-side switches.
        Board limited to 5.5A, but code should work with the 10A device as well.
    programmer/
        Hand-held configuration tool with 20x4 LCD. Simple field configuration only.
    usb_link/
        USB bridge with support for configuration and node firmware updates.
        lintool/
            Command-line tool for the USB link.
