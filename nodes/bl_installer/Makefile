PROG		 = bl_installer
SRCS		 = main.cpp board.cpp

BL_ADDR		 = 0x3400
DEFINES		+= -DBL_ADDR=$(BL_ADDR)

# Start address is padded so that we can safely erase the first page
EXTRA_LDFLAGS	 = -Wl,--section-start=.text=0x100

# Required for <board.h>, any valid board will do.
BOARD		 = MASTER_V1

# For our payload
CORE		 = $(BUILDDIR)/../bl_core/bl_core.h
EXTRA_INCLUDES	 = $(dir $(CORE))

# The actual installer
INSTALLER	 = $(BUILDDIR)/installer.hex
BUILD_TARGET	 = $(INSTALLER)
UPLOAD_HEX	 = $(INSTALLER)

include ../rules.mk

build:	$(INSTALLER)

$(INSTALLER):	hex $(CORE)
	@echo Generate $@
	@cat prefix.hex $(HEX) > $@

verify:
	avrdude -p attiny167 -c jtag2isp -C ../../etc/attiny167.conf -U flash:r:download.bin:r
