USBFLAGS	:= $(shell libusb-config --cflags)
USBLIBS		:= $(shell libusb-config --libs)

SRCS		 = lintool.c opendevice.c

CC		 = cc
CFLAGS		 = $(USBFLAGS) -O -g -Wall

PROG		 = lintool

$(PROG):	$(SRCS)
	$(CC) -o $@ $(CFLAGS) $(SRCS) $(USBLIBS)

clean:
	rm -rf $(PROG) $(PROG).dSYM
