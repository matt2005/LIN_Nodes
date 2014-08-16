#
# Makefile include fragment
#

SRCROOT		:= $(abspath $(dir $(lastword $(MAKEFILE_LIST))))
TOPDIR		:= $(abspath $(SRCROOT)/..)
BUILDDIR	:= $(TOPDIR)/build/$(PROG)

CC		 = avr-gcc
CXX		 = avr-g++
CPPCHECK	 = cppcheck
SIZE		 = avr-size
OBJCOPY		 = avr-objcopy
AVRDUDE		 = avrdude
XXD		 = xxd

MCU		 = attiny167

# default fusing is for the internal oscillator @ 8MHz
OSC_FUSES	 = -U hfuse:w:0xd7:m -U efuse:w:0xfe:m -U lfuse:w:0x62:m
FUSES		?= $(OSC_FUSES)
F_CPU		?= 8000000

ARCHFLAGS	 = -mmcu=$(MCU)
DEFINES		+= -DBOARD_$(BOARD)		\
		   -DF_CPU=$(F_CPU)UL		\
		   $(if $(DEBUG),-DDEBUG)	\
		   -DGIT_VERSION=$(shell git log -1 --pretty=format:%h) \
		   -DGIT_HEX_VERSION=0x$(shell git log -1 --pretty=format:%h) \
		   -DRELEASE_VERSION=0x0001


CHECKOPTS	 = --enable=warning		\
		   --enable=performance		\
		   --enable=information		\
		   --enable=style
#		   --inconclusive

INCLUDES	 = -I$(TOPDIR)/common		\
		   -I$(SRCROOT)/lib		\
		   -I$(SRCROOT)/$(PROG)		\
		   $(foreach dir,$(SUBDIRS),-I$(SRCROOT)/$(PROG)/$(dir)) \
		   $(foreach dir,$(EXTRA_INCLUDES),-I$(dir))

# -O2 gives best code size, -O3 gives best RAM usage
COMPILEFLAGS	 = $(ARCHFLAGS)			\
		   -gdwarf-2			\
		   -O2				\
		   -funsigned-char		\
		   -funsigned-bitfields		\
		   -fno-inline-small-functions	\
		   -fshort-enums		\
		   -fno-stack-protector		\
		   -ffreestanding		\
		   -Wall			\
		   -ffunction-sections		\
		   -fdata-sections		\
		   -MMD				\
		   $(INCLUDES)			\
		   $(DEFINES)

CFLAGS		 = $(COMPILEFLAGS)		\
		   -std=gnu11			\
		   -Wstrict-prototypes		\
		   $(EXTRA_CFLAGS)

CXXFLAGS	 = $(COMPILEFLAGS)		\
		   -std=gnu++11			\
		   -fno-exceptions		\
		   -fno-rtti			\
		   $(EXTRA_CXFLAGS)

ASFLAGS		 = $(ARCHFLAGS)			\
		   $(INCLUDES)			\
		   $(DEFINES)			\
		   $(EXTRA_ASFLAGS)	

LDFLAGS		 = $(ARCHFLAGS)			\
		   -gdwarf-2			\
		   -Wl,-gc-sections		\
		   -Wl,--relax			\
		   -fno-exceptions		\
		   -fno-rtti			\
		   $(EXTRA_LDFLAGS)

vpath %.c	$(SRCROOT)/lib $(SRCROOT)/../common $(foreach dir,$(SUBDIRS),$(SRCROOT)/$(PROG)/$(dir))
vpath %.cpp	$(SRCROOT)/lib $(SRCROOT)/../common $(foreach dir,$(SUBDIRS),$(SRCROOT)/$(PROG)/$(dir))
vpath %.S	$(SRCROOT)/lib $(foreach dir,$(SUBDIRS),$(SRCROOT)/$(PROG)/$(dir))
vpath %.h	$(SRCROOT)/lib $(foreach dir,$(SUBDIRS),$(SRCROOT)/$(PROG)/$(dir))

PROGNAME	?= $(PROG)
ELF		:= $(BUILDDIR)/$(PROGNAME).elf
HEX		:= $(BUILDDIR)/$(PROGNAME).hex
BIN		:= $(BUILDDIR)/$(PROGNAME).bin
HEADER		:= $(BUILDDIR)/$(PROGNAME).h
MAP		:= $(ELF).map

OBJS		:= $(foreach src,$(SRCS),$(BUILDDIR)/$(notdir $(src)).o)
DEPS		:= $(OBJS:.o=.d)

BUILD_TARGET	?= $(ELF)
UPLOAD_HEX	?= $(HEX)

HDRS		 = $(shell $(CC) -E -H $(DEFINES) $(INCLUDES) $(SRCS))

ifeq ($(VERBOSE),)
q		= @
endif

.SUFFIXES:

build:	$(BUILD_TARGET)
hex:	$(HEX)
bin:	$(BIN)
header:	$(HEADER)

$(ELF) $(OBJS):	$(MAKEFILE_LIST)

upload: $(UPLOAD_HEX)
#	$(AVRDUDE) -p $(MCU) -c usbasp -C $(TOPDIR)/etc/$(MCU).conf -U flash:w:$< $(FUSES)
	$(AVRDUDE) -p $(MCU) -c jtag2isp -C $(TOPDIR)/etc/$(MCU).conf -U flash:w:$<:i $(FUSES)

usbload: $(HEX)
	micronucleus $<

$(HEX): $(ELF)
	@echo HEX $(notdir $@)
	$q $(OBJCOPY) -j .text -j .data -O ihex $< $@

$(BIN): $(ELF)
	@echo BIN $(notdir $@)
	$q $(OBJCOPY) -j .text -j .data -O binary $< $@

$(HEADER): $(BIN)
	@echo HEADER $(notdir $@)
	$q cd $(dir $<) && $(XXD) -i $(notdir $<) > $@

$(ELF):	$(OBJS) $(MAKEFILE_LIST)
	@echo LINK $(notdir $@)
	$q $(CXX) -o $@ $(LDFLAGS) $(OBJS) -Wl,-Map,$@.map
	$q $(SIZE) $@

.PHONY: clean
clean:
	@echo CLEAN $(BUILDDIR)
	$q rm -f $(ELF) $(BIN) $(HEX) $(HEADER) $(OBJS) $(DEPS) $(MAP) $(BUILD_TARGET)

.PHONY: check
check:
	@echo CHECK $(PROG)
	$q $(CPPCHECK) $(CHECKOPTS) $(DEFINES) $(INCLUDES) $(SRCS)

$(filter %.c.o,$(OBJS)): $(BUILDDIR)/%.o: %
	@mkdir -p $(dir $@)
	@echo CC $(notdir $<)
	$q $(CC) -c -o $@ $(CFLAGS) $(abspath $<)

$(filter %.cpp.o,$(OBJS)): $(BUILDDIR)/%.o: %
	@mkdir -p $(dir $@)
	@echo CXX $(notdir $<)
	$q $(CXX) -c -o $@ $(CXXFLAGS) $(abspath $<)

$(filter %.S.o,$(OBJS)): $(BUILDDIR)/%.o: %
	@mkdir -p $(dir $@)
	@echo CXX $(notdir $<)
	$q $(CC) -c -o $@ $(ASFLAGS) $(abspath $<)

-include $(DEPS)
