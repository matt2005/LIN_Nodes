#
# Bootloader in the micronucleus folder.
# Flash with:
# avrdude -B 8 -p attiny167 -c jtag2isp -C ../../../etc/ATtiny167.conf -U flash:w:main.hex:i \
#         -U hfuse:w:0xd7:m -U efuse:w:0xfe:m -U lfuse:w:0xef:m
#

DEBUG		 = YES

PROG		 = usb_link
SRCS		 = main.cpp \
		   slave.cpp \
		   usbdrv.c \
		   usbdrvasm.S

SRCS		+= board.cpp \
		   lin_dev.cpp \
		   lin_slave.cpp \
		   print.cpp \
		   softserial.cpp \
		   timer.cpp \
		   util.cpp

SUBDIRS		 = usbdrv
BOARD		 = USB_LINK
DEFINES		+= -DGIT_VERSION=$(shell git log -1 --pretty=format:%h)
FUSES		:=
F_CPU		 = 16000000

include ../rules.mk
