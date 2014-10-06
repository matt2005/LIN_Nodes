PROG		 = lintool

SRCROOT		:= $(abspath $(dir $(lastword $(MAKEFILE_LIST))))
TOPDIR		:= $(abspath $(SRCROOT)/../../..)
BUILDDIR	:= $(TOPDIR)/build/$(PROG)
COMMON		:= $(TOPDIR)/common

CXX		 = c++

USBFLAGS	:= $(shell pkg-config --cflags libusb-1.0)
USBLIBS		:= $(shell pkg-config --libs libusb-1.0)

DEFINES		 = -DLIN_DEFS_WITH_STRINGS
INCLUDES	 = $(SRCROOT)/.. $(COMMON)

CXXFLAGS	 = -std=gnu++11			\
		   $(USBFLAGS)			\
		   -O0				\
		   -g				\
		   -Wall			\
		   -I$(COMMON)			\
		   -I..				\
		   $(DEFINES)

vpath %.cpp	$(SRCROOT) $(COMMON)
vpath %.h	$(SRCROOT) $(SRCROOT/..) $(COMMON)

SRCS		 = $(wildcard *.cpp)		\
		   lin_defs.cpp

HDRS		 = $(shell $(CXX) -E -H $(DEFINES) $(INCLUDES) $(SRCS))

OBJS		 = $(foreach src,$(SRCS),$(BUILDDIR)/$(notdir $(src)).o)
DEPS		 = $(OBJS:.o=.d)

NO_FORMAT_SRCS	 = Jzon% $(COMMON)%

ifeq ($(VERBOSE),)
q		= @
endif

.SUFFIXES:

build:	$(PROG)

$(PROG) $(OBJS): $(MAKEFILE_LIST)

$(PROG):	$(OBJS)
	$(CXX) -g -o $@ $(OBJS) $(USBLIBS)

$(filter %.cpp.o,$(OBJS)): $(BUILDDIR)/%.o: %
	@mkdir -p $(dir $@)
	@echo CXX $(notdir $<)
	$q $(CXX) -c -o $@ $(CXXFLAGS) $(abspath $<)


.PHONY: clean
clean:
	@echo clean $(BUILDDIR)
	$q rm -f $(PROG) $(OBJS) $(DEPS)

.PHONY: reformat
reformat:
	@../../../etc/reformat.sh \
		$(filter-out $(NO_FORMAT_SRCS),$(SRCS)) \
		$(filter-out $(NO_FORMAT_SRCS),$(HDRS))
