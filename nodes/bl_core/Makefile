PROG		 = bl_core
SRCS		 = crt1.S		\
		   main.cpp		\
		   board.cpp		\
		   bl_slave.cpp		\
		   lin_dev.cpp

BL_ADDR		 = 0x3400
DEFINES		+= -DBL_ADDR=$(BL_ADDR)

# Start address adjusted to the bootloader area; reset vector
# is initially managed by the installer.
EXTRA_LDFLAGS	 = -nostartfiles				\
		   -Wl,--section-start=.text=$(BL_ADDR)

# Required for <board.h>
BOARD		 = MASTER_V1

build: header

include ../rules.mk
