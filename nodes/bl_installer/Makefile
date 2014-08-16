PROG		 = bl_installer
SRCS		 = main.cpp board.cpp

BL_ADDR		 = 0x3400
DEFINES		+= -DBL_ADDR=$(BL_ADDR)

# Start address is padded so that we can safely erase the first page
EXTRA_LDFLAGS	 = -Wl,--section-start=.text=0x100

# For our payload
EXTRA_INCLUDES	 = $(BUILDDIR)/../bl_core/

# Required for <board.h>, any valid board will do.
BOARD		 = MASTER_V1

build:	hex

include ../rules.mk
