CC       := /usr/bin/gcc -x c
CURDIR   := $(shell pwd)

DEBUGDIR := $(CURDIR)/debug
SRCDIR   := $(CURDIR)/src
BINDIR   := $(CURDIR)/bin

# TI
TI_CCSDIR       := /home/mreenen/.local/share/ccs10/ccs
TI_SL_CC32xxDIR := /home/mreenen/.local/share/ccs10/simplelink_cc32xx_sdk_4_30_00_06
TI_XDCTOOLSDIR  := /home/mreenen/.local/share/ccs10/xdctools_3_61_02_27_core
TI_INCLUDEDIR   := $(TI_SL_CC32xxDIR)/source
TI_KERNEL       := $(TI_SL_CC32xxDIR)/kernel/tirtos
TI_EABI			:= $(TI_KERNEL)/packages/gnu/targets/arm/libs/install-native/arm-none-eabi/include/
TI_CGT			:= $(TI_CCSDIR)/tools/compiler/ti-cgt-arm_20.2.1.LTS/include
TI_CCINCLUDES   := -I"$(TI_INCLUDEDIR)" -I"$(TI_INCLUDEDIR)/ti/posix/gcc" -I"$(TI_KERNEL)/packages" -I"$(TI_XDCTOOLSDIR)/packages" -I"$(TI_CGT)" -I"$(TI_EABI)"

# CC ARGS
CC_INCLUDES := $(TI_CCINCLUDES)
CC_ARGS     := $(CC_INCLUDES)

OBJECTS := $(DEBUGDIR)/main.o $(DEBUGDIR)/communicatieBeheer.o $(DEBUGDIR)/noodstop.o $(DEBUGDIR)/systeemBeheer.o

dirs:
	mkdir -p $(DEBUGDIR) $(SRCDIR) $(BINDIR)

# compile
main.o: src/main.c dirs
	$(CC) $(CC_ARGS) -c $(SRCDIR)/main.c -o $(DEBUGDIR)/main.o
communicatieBeheer.o: src/communicatieBeheer.c dirs
	$(CC) $(CC_ARGS) -c $(SRCDIR)/communicatieBeheer.c -o $(DEBUGDIR)/communicatieBeheer.o
noodstop.o: src/noodstop.c dirs
	$(CC) $(CC_ARGS) -c $(SRCDIR)/noodstop.c -o $(DEBUGDIR)/noodstop.o
systeemBeheer.o: src/systeemBeheer.c dirs
	$(CC) $(CC_ARGS) -c $(SRCDIR)/systeemBeheer.c -o $(DEBUGDIR)/systeemBeheer.o

# link
link: main.o communicatieBeheer.o noodstop.o systeemBeheer.o
	$(CC) $(CC_ARGS) -o $(BINDIR)/binary $(OBJECTS)

all: link

clean:
	rm -p domeTimer $(DEBUGDIR)/*
