DEBUG		 = YES

PROG		 = PowerV3
SRCS		 = main.cpp \
		   mc17xsf500.cpp \
		   slave.cpp

SRCS		+= lin_defs.cpp

SRCS		+= board.cpp \
		   lin_dev.cpp \
		   lin_slave.cpp \
		   print.cpp \
		   softserial.cpp \
		   timer.cpp \
		   util.cpp

BOARD		 = POWER_V3

build: fw
	
include ../rules.mk
