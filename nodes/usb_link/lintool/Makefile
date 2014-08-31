USBFLAGS	:= $(shell pkg-config --cflags libusb-1.0)
USBLIBS		:= $(shell pkg-config --libs libusb-1.0)
COMMON		:= ../../../common

SRCS		 = $(wildcard *.cpp)		\
		   $(COMMON)/lin_defs.cpp

HDRS		 = $(wildcard *.h)

CXX		 = c++
FLAGS		 = -std=gnu++11 $(USBFLAGS) -O0 -g -Wall -I$(COMMON)

PROG		 = lintool

$(PROG):	$(SRCS) $(HDRS)
	$(CXX) -o $@ $(FLAGS) $(SRCS) $(USBLIBS)

.PHONY: clean
clean:
	rm -rf $(PROG) $(PROG).dSYM

.PHONY: reformat
reformat:
	../../../etc/reformat.sh $(SRCS)
