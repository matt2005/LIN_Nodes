PROG		 = lintool

SRCROOT		:= $(abspath $(dir $(lastword $(MAKEFILE_LIST))))
TOPDIR		:= $(abspath $(SRCROOT)/..)
BUILDDIR	:= $(TOPDIR)/build/$(PROG)
COMMON		:= $(TOPDIR)/common
NODES		:= $(TOPDIR)/nodes

CXX		 = c++

USBFLAGS	:= $(shell pkg-config --cflags libusb-1.0)
USBLIBS		:= $(shell pkg-config --libs libusb-1.0)

DEFINES		 = -DLIN_DEFS_WITH_STRINGS
INCLUDES	 = $(COMMON) $(NODES)

CXXFLAGS	 = -std=gnu++11			\
		   $(USBFLAGS)			\
		   -O0				\
		   -g				\
		   -Wall			\
		   $(addprefix -I,$(INCLUDES))	\
		   -MMD				\
		   $(DEFINES)

vpath %.cpp	$(SRCROOT) $(COMMON)
vpath %.h	$(SRCROOT) $(SRCROOT/..) $(COMMON)

MACHO		 = $(BUILDDIR)/$(PROG)
DSYM		 = $(MACHO).dSYM

SRCS		 = $(wildcard *.cpp)		\
		   lin_defs.cpp

HDRS		 = $(shell $(CXX) -E -H $(DEFINES) $(INCLUDES) $(SRCS))

OBJS		 = $(foreach src,$(SRCS),$(BUILDDIR)/$(notdir $(src)).o)
DEPS		 = $(OBJS:.o=.d)

NO_FORMAT_SRCS	 = json% lin_defs.cpp

ifeq ($(VERBOSE),)
q		= @
endif

.SUFFIXES:

build:	$(MACHO)

$(PROG) $(OBJS): $(MAKEFILE_LIST)

$(MACHO):	$(OBJS)
	@mkdir -p $(dir $@)
	@echo LINK $(notdir $@)
	$q $(CXX) -g -o $@ $(OBJS) $(USBLIBS)

$(filter %.cpp.o,$(OBJS)): $(BUILDDIR)/%.o: %
	@mkdir -p $(dir $@)
	@echo CXX $(notdir $<)
	$q $(CXX) -c -o $@ $(CXXFLAGS) $(abspath $<)


.PHONY: clean
clean:
	@echo clean $(BUILDDIR)
	$q rm -rf $(MACHO) $(DSYM) $(OBJS) $(DEPS) 

.PHONY: reformat
reformat:
	../etc/reformat.sh \
		$(filter-out $(NO_FORMAT_SRCS),$(SRCS)) \
		$(filter-out $(NO_FORMAT_SRCS),$(HDRS))

-include $(DEPS)
