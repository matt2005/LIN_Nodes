Firmware and PCB designs for various LIN network nodes intended for retrofitting older vehicles.

```
boards/
    Eagle schematic and PCB files.

common/
    Protocol definitions and generator.

dashboard/
    Dashboard display using a 64x32 RGB LED panel driven by an NXP LPC1114.

Documentation/
    Various datasheets relevant to the nodes.

lintool
    Host-side configuration tool. Talks to the usb_link hardware.

nodes/
    All of the nodes in this part of the tree are based on the Atmel ATTiny167.

    bl_core/
        LIN bootloader core, consumed by bl_installer.

    bl_installer/
        Bootloader installer; flashed onto a fresh board this will the bootloader.
        The installed bootloader comes up on NAD 32 and can be flashed with lintool
        by explicitly supplying the node address and firmware file.

    lib/
        Shared code.

    master/
        Network master node.

    power_v3/
        5-output power switching node using Freescale smart high-side switches.
        Board limited to 5.5A, but code should work with the 10A device as well.

    usb_link/
        USB bridge with support for configuration and node firmware updates.

```
