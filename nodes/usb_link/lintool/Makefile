USBFLAGS	:= $(shell pkg-config --cflags libusb-1.0)
USBLIBS		:= $(shell pkg-config --libs libusb-1.0)

SRCS		 = lintool.cpp			\
		   firmware.cpp			\
		   link.cpp			\
		   log.cpp			\
		   param.cpp			\
		   ../../../common/lin_defs.cpp

CXX		 = c++
FLAGS		 = -std=gnu++11 $(USBFLAGS) -O0 -g -Wall

PROG		 = lintool

$(PROG):	$(SRCS)
	$(CXX) -o $@ $(FLAGS) $(SRCS) $(USBLIBS)

.PHONY: clean
clean:
	rm -rf $(PROG) $(PROG).dSYM

.PHONY: reformat
reformat:
	../../../etc/reformat.sh $(SRCS)
