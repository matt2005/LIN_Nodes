#
# Build the v1 dashboard controller firmware
#

#
# What we build
#
NAME		 = dashboard
DIRS		 = . lib chip gfx data
FONTS		 = $(wildcard fonts/*.bdf)

#
# Tools
#
TOOLPREFIX	 = arm-none-eabi
CC	 	 = $(TOOLPREFIX)-gcc
CXX	 	 = $(TOOLPREFIX)-g++
OBJCOPY		 = $(TOOLPREFIX)-objcopy
SIZE		 = $(TOOLPREFIX)-size
UPLOAD		 = lpc21isp
CPPCHECK	 = cppcheck
#SERIALPORT	 = /dev/cu.usbserial-AE01AO9V
SERIALPORT	 = /dev/cu.SLAB_USBtoUART


#
# Flags we pass to invocations of the compiler
#
ARCHFLAGS	 = -g					\
		   -mthumb				\
		   -mcpu=cortex-m0

DEFINES		 = -DCFG_CPU_CCLK=48000000
CHECKOPTS	 = --enable=warning			\
		   --enable=performance			\
		   --enable=information			\
		   --enable=style			\
		   --inconclusive			\
		   --inline-suppr

FLAGS		 = $(ARCHFLAGS)				\
		   -O3					\
		   -Wall				\
		   -Wextra				\
		   -Werror				\
		   -ffunction-sections			\
		   -fdata-sections			\
		   -fmessage-length=0			\
		   -ffreestanding			\
		   -fno-common				\
		   -flto				\
		   -I..					\
		   $(addprefix -I,$(DIRS))		\
		   $(DEFINES)

# XXX CFG_CPU_CCLK needs to be authoritative

#
# Flags unique to C compilation
#
CFLAGS		 = $(FLAGS)				\
		   -std=gnu99

#
# Flags unique to C++ compilation
#
CPPFLAGS	 = $(FLAGS)				\
		   -std=gnu++11				\
		   -fno-rtti				\
		   -fno-exceptions			\
		   -fno-delete-null-pointer-checks	\
		   -Wno-unused-parameter		\
		   -Wno-missing-field-initializers	\
		   -Wno-pmf-conversions

#
# Flags unique to linking
#
LIBS		+= gcc c
LDFLAGS		 = $(ARCHFLAGS)				\
		   -nostartfiles			\
		   -nostdlib				\
		   -fno-rtti				\
		   -fno-exceptions			\
		   -flto				\
		   -Wl,-gc-sections			\
		   -Wl,-Map,$@.map			\
		   -Wl,--cref				\
		   -Wl,--no-define-common		\
		   -Wl,--warn-common			\
		   $(addprefix -l,$(LIBS))		\
		   -Tchip/lpc1114fn28.ld


BIN		 = build/$(NAME).bin
ELF		 = $(BIN:.bin=.elf)

HDRS		 = $(foreach dir,$(wildcard $(DIRS)),$(wildcard $(dir)/*.h))
SRCS		 = $(foreach dir,$(wildcard $(DIRS)),$(wildcard $(dir)/*.c $(dir)/*.cpp))

OBJS		 = $(addprefix build/,$(addsuffix .o,$(SRCS)))
COBJS		 = $(filter %.c.o, $(OBJS))
CPPOBJS		 = $(filter %.cpp.o, $(OBJS))

ifeq ($(VERBOSE),)
q		= @
endif

.SUFFIXES:
$(BIN) $(ELF) $(OBJS):	$(MAKEFILE_LIST)

build:	$(ELF)

prog:	$(BIN)
	$(UPLOAD) -control -bin $(BIN) $(SERIALPORT) 115200 12000

#.INTERMEDIATE:	$(COBJ) $(CPPOBJ) fonts.c fonts.h

$(BIN):	$(ELF)
	$q $(OBJCOPY) -Obinary $< $@

$(ELF):	$(OBJS)
	echo LINK $(notdir $@)
	$q $(CC) -o $@ $(OBJS) $(LDFLAGS)
	$q $(SIZE) $@

$(filter %.c.o,$(OBJS)): build/%.c.o: %.c
	@echo CC $(notdir $<)
	$q @mkdir -p $(dir $@)
	$q $(CC) -c -o $@ -MMD $(CFLAGS) $<

$(filter %.cpp.o,$(OBJS)): build/%.cpp.o: %.cpp
	@echo CXX $(notdir $<)
	$q @mkdir -p $(dir $@)
	$q $(CXX) -c -o $@ -MMD $(CPPFLAGS) $<

.PHONY: clean
clean:
	@rm -rf build

.PHONY: check
check:
	@echo CHECK $(NAME)
	$q $(CPPCHECK) $(CHECKOPTS) $(DEFINES) -I.. $(addprefix -I,$(DIRS)) $(SRCS)

.PHONY: reformat
reformat:
	@../etc/reformat.sh $(SRCS) $(HDRS)

-include $(wildcard $(foreach dir,$(DIRS),build/$(dir)/*.d))
