############################################################
#Makefile
############################################################
TARGET = cmdline

CROSS_COMPILE=/opt/xilinx-arm-linux-toolchains/bin/arm-xilinx-linux-gnueabi-
CC :=$(CROSS_COMPILE)gcc
LD :=$(CROSS_COMPILE)ld

debug = y
ifeq ($(debug), y)
CFLAGS += -g -DDEBUG
else
CFLAGS += -O2 -s
endif

PWD := $(shell pwd)

CFLAGS 	 += -Wall 
LDFLAGS   = 
CPPFLAGS := -I$(PWD)

SRC_PATH :=$(PWD)

include com.mk
