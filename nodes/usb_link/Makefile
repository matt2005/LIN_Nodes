#
# Bootloader in the micronucleus folder.
# Flash with:
# avrdude -B 8 -p attiny167 -c jtag2isp -C ../../../etc/ATtiny167.conf -U flash:w:main.hex:i \
#         -U hfuse:w:0xd7:m -U efuse:w:0xfe:m -U lfuse:w:0xef:m
#

DEBUG		 = YES
DEFINES		+= -DWITH_TIMER_PREEMPTION

PROG		 = usb_link
SRCS		 = main.cpp \
		   slave.cpp \
		   usbdrv.c \
		   usbdrvasm.S

SRCS		+= lin_defs.cpp

SRCS		+= board.cpp \
		   lin_dev.cpp \
		   lin_slave.cpp \
		   print.cpp \
		   softserial.cpp \
		   timer.cpp \
		   util.cpp

SUBDIRS		 = usbdrv
BOARD		 = USB_LINK
FUSES		:=
F_CPU		 = 16000000

include ../rules.mk
