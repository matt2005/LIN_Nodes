DEBUG		 = YES

PROG		 = programmer
SRCS		 = main.cpp \
		   menu.cpp \
		   m_edit.cpp \
		   m_top.cpp \
		   m_explore.cpp \
		   m_info.cpp \
		   m_setup.cpp \
		   m_setup_master.cpp \
		   m_setup_power.cpp \
		   m_test.cpp \
		   slave.cpp

SRCS		+= board.cpp \
		   encoder.cpp \
		   hd44780.cpp \
		   lin_dev.cpp \
		   lin_slave.cpp \
		   param_master.cpp \
		   param_power_v1.cpp \
		   param_power_v3.cpp \
		   print.cpp \
		   softserial.cpp \
		   timer.cpp \
		   util.cpp

BOARD		 = PROGRAMMER_V1
DEFINES		+= -DGIT_VERSION=$(shell git log -1 --pretty=format:%h)

include ../rules.mk
