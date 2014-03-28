#
# Makefile include fragment
#

TOPDIR		:= $(realpath $(dir $(lastword $(MAKEFILE_LIST))))
BUILDDIR	:= $(TOPDIR)/build/$(PROG)

CC		 = avr-gcc
CXX		 = avr-g++
SIZE		 = avr-size

MCU		 = attiny167
ARCHFLAGS	 = -mmcu=$(MCU)
DEFINES		 = -DBOARD_$(BOARD)
COMPILEFLAGS	 = $(ARCHFLAGS)			\
		   -gdwarf-2			\
		   -O2				\
		   -funsigned-char		\
		   -funsigned-bitfields		\
		   -fshort-enums		\
		   -Wall			\
		   -ffunction-sections		\
		   -fdata-sections		\
		   -MMD				\
		   -I$(TOPDIR)/common		\
		   $(DEFINES)			\
		   -DF_CPU=8000000UL

CFLAGS		 = $(COMPILEFLAGS)		\
		   -std=gnu11			\
		   -Wstrict-prototypes

CXXFLAGS	 = $(COMPILEFLAGS)		\
		   -std=gnu++11			\
		   -fno-exceptions		\
		   -fno-rtti

LDFLAGS		 = $(ARCHFLAGS)			\
		   -gdwarf-2			\
		   -Wl,-gc-sections

vpath %.c	$(TOPDIR)/common
vpath %.cpp	$(TOPDIR)/common
vpath %.h	$(TOPDIR)/common

.SUFFIXES:

OBJS		:= $(foreach src,$(SRCS),$(BUILDDIR)/$(notdir $(src)).o)
DEPS		:= $(OBJS:.o=.d)
ELF			:= $(BUILDDIR)/$(PROG).elf

build:	$(ELF)

$(ELF):	$(OBJS) $(MAKEFILE_LIST)
	@echo LINK $(notdir $@)
	@$(CXX) -o $@ $(LDFLAGS) $(OBJS) -Wl,-Map,$@.map
	@$(SIZE) $@

clean:
	@echo CLEAN $(BUILDDIR)
	@rm -f $(ELF) $(OBJS) $(DEPS)

$(filter %.c.o,$(OBJS)): $(BUILDDIR)/%.o: %
	@mkdir -p $(dir $@)
	@echo CC $(notdir $<)
	@$(CC) -c -o $@ $(CFLAGS) $(abspath $<)

$(filter %.cpp.o,$(OBJS)): $(BUILDDIR)/%.o: %
	@mkdir -p $(dir $@)
	@echo CXX $(notdir $<)
	@$(CXX) -c -o $@ $(CXXFLAGS) $(abspath $<)

-include $(DEPS)
