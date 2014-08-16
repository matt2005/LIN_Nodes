PROG		 = bl_updater
INSTALLER	 = $(abspath $(BUILDDIR)/../bl_installer/bl_installer.hex)
UPDATER		 = $(BUILDDIR)/updater.hex

BUILD_TARGET	 = $(UPDATER)
UPLOAD_HEX	 = $(UPDATER)

include ../rules.mk

$(UPDATER):	prefix.hex $(INSTALLER)
	@mkdir -p $(dir $@)
	cat prefix.hex $(INSTALLER) > $@

verify:
	avrdude -vvvv -p attiny167 -c jtag2isp -C ../../etc/attiny167.conf -U flash:r:download.bin:r

