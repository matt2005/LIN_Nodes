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
OBJCOPY		 = $(TOOLPREFIX)-objcopy
SIZE		 = $(TOOLPREFIX)-size
UPLOAD		 = lpc21isp
#SERIALPORT	 = /dev/cu.usbserial-AE01AO9V
SERIALPORT	 = /dev/cu.SLAB_USBtoUART


#
# Flags we pass to invocations of the compiler
#
ARCHFLAGS	 = -g					\
		   -mthumb				\
		   -mcpu=cortex-m0

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
		   -DCFG_CPU_CCLK=48000000		\
		   -I..					\
		   $(addprefix -I,$(DIRS))

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
		   -std=gnu++0x				\
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

.SUFFIXES:
$(BIN) $(ELF) $(OBJS):	$(MAKEFILE_LIST)

build:	$(BIN)

prog:	$(BIN)
	$(UPLOAD) -control -bin $(BIN) $(SERIALPORT) 115200 12000

#.INTERMEDIATE:	$(COBJ) $(CPPOBJ) fonts.c fonts.h

$(BIN):	$(ELF)
	$(OBJCOPY) -Obinary $< $@

$(ELF):	$(OBJS)
	$(CC) -o $@ $(OBJS) $(LDFLAGS)
	$(SIZE) $@

$(filter %.c.o,$(OBJS)): build/%.c.o: %.c
	@mkdir -p $(dir $@)
	$(CC) -c -o $@ -MMD $(CFLAGS) $<

$(filter %.cpp.o,$(OBJS)): build/%.cpp.o: %.cpp
	@mkdir -p $(dir $@)
	$(CC) -c -o $@ -MMD $(CPPFLAGS) $<

clean:
	@rm -rf build

reformat:
	@../etc/reformat.sh $(SRCS) $(HDRS)

-include $(wildcard $(foreach dir,$(DIRS),build/$(dir)/*.d))
