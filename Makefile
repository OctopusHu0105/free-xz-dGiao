CC = gcc
CFLAGS = -g -O -Wall

OBJ := free_test.o  main.o
TOPDIR := $(shell pwd)
OBJDIR := $(TOPDIR)/build
COMDIR := $(TOPDIR)/components
BIN := main

SUBDIR := examples/get-started  $(COMDIR)/free_test 
OBJLINK := --std=c99


export CC TOPDIR OBJDIR BINDIR BIN OBJLINK OBJ

all:CHECKDIR $(SUBDIR)
CHECKDIR:
	mkdir -p $(SUBDIR) $(BINDIR)
$(SUBDIR):RUN
	make -C $@
RUN:

$()

clean:
	rm -rf $(OBJDIR)/*.o $(BINDIR)

