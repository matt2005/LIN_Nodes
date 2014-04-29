#
# Makefile include fragment
#

TOPDIR		:= $(realpath $(dir $(lastword $(MAKEFILE_LIST))))
BUILDDIR	:= $(TOPDIR)/build/$(PROG)

CC		 = avr-gcc
CXX		 = avr-g++
SIZE		 = avr-size
AVRDUDE		 = avrdude

MCU		 = attiny167

# default fusing is for the internal oscillator
OSC_FUSES	 = -U hfuse:w:0xdf:m -U efuse:w:0xff:m -U lfuse:w:0x62:m
RES_FUSES	 = -U hfuse:w:0xdf:m -U efuse:w:0xff:m -U lfuse:w:0x6c:m
FUSES		?= $(OSC_FUSES)

ARCHFLAGS	 = -mmcu=$(MCU)
DEFINES		 = -DBOARD_$(BOARD)

# -O2 gives best code size, -O3 gives best RAM usage
COMPILEFLAGS	 = $(ARCHFLAGS)			\
		   -gdwarf-2			\
		   -O2				\
		   -funsigned-char		\
		   -funsigned-bitfields		\
		   -fshort-enums		\
		   -fno-inline-small-functions	\
		   -fno-stack-protector		\
		   -ffreestanding		\
		   -Wall			\
		   -ffunction-sections		\
		   -fdata-sections		\
		   -MMD				\
		   -I$(TOPDIR)/common		\
		   $(DEFINES)			\
		   -DF_CPU=8000000UL		\
		   $(if $(DEBUG),-DDEBUG)

CFLAGS		 = $(COMPILEFLAGS)		\
		   -std=gnu11			\
		   -Wstrict-prototypes

CXXFLAGS	 = $(COMPILEFLAGS)		\
		   -std=gnu++11			\
		   -fno-exceptions		\
		   -fno-rtti

LDFLAGS		 = $(ARCHFLAGS)			\
		   -gdwarf-2			\
		   -Wl,-gc-sections		\
		   -Wl,--relax

vpath %.c	$(TOPDIR)/common
vpath %.cpp	$(TOPDIR)/common
vpath %.h	$(TOPDIR)/common

.SUFFIXES:

OBJS		:= $(foreach src,$(SRCS),$(BUILDDIR)/$(notdir $(src)).o)
DEPS		:= $(OBJS:.o=.d)
ELF		:= $(BUILDDIR)/$(PROG).elf
ifeq ($(VERBOSE),)
q		= @
endif

build:	$(ELF)

upload: $(ELF)
	$(AVRDUDE) -p $(MCU) -c jtag2isp -C $(TOPDIR)/etc/$(MCU).conf -U flash:w:$< $(FUSES)

$(ELF):	$(OBJS) $(MAKEFILE_LIST)
	@echo LINK $(notdir $@)
	$q $(CXX) -o $@ $(LDFLAGS) $(OBJS) -Wl,-Map,$@.map
	$q $(SIZE) $@

clean:
	@echo CLEAN $(BUILDDIR)
	$q rm -f $(ELF) $(OBJS) $(DEPS)

$(filter %.c.o,$(OBJS)): $(BUILDDIR)/%.o: %
	@mkdir -p $(dir $@)
	@echo CC $(notdir $<)
	$q $(CC) -c -o $@ $(CFLAGS) $(abspath $<)

$(filter %.cpp.o,$(OBJS)): $(BUILDDIR)/%.o: %
	@mkdir -p $(dir $@)
	@echo CXX $(notdir $<)
	$q $(CXX) -c -o $@ $(CXXFLAGS) $(abspath $<)

-include $(DEPS)
