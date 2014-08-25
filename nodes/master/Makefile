DEBUG		 = YES

PROG		 = master
SRCS		 = main.cpp \
		   master.cpp \
		   relays.cpp \
		   switches.cpp

SRCS		+= lin_defs.cpp

SRCS		+= board.cpp \
		   lin_dev.cpp \
		   lin_slave.cpp \
		   mc33972.cpp \
		   print.cpp \
		   softserial.cpp \
		   timer.cpp \
		   util.cpp

BOARD		 = MASTER_V1
#FUSES		 = $(RES_FUSES)

build: fw

include ../rules.mk
