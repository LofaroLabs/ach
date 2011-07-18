PROJECT := ach

VERSION := 0.20110622

SHAREDLIBS := ach

BINFILES := achcat achpipe.bin ach achchand

LC_ALL := ascii
LANG := ascii

all: default

#cc := llvm-gcc

include /usr/share/make-common/common.1.mk

default: $(LIBFILES) $(BINFILES) ach_stream.o ach.pyc

CFLAGS += -O2 --std=gnu99 -fPIC -DACH_VERSION_STRING=\"$(VERSION)\"

LISPDIR = ./lisp

ifneq ($(PLATFORM),Darwin)
CFLAGS += -DHAVE_STRNLEN
endif

$(call LINKLIB, ach, ach.o ach_stream.o, pthread rt)

$(call LINKBIN, test_pub, test_pub.c ach.o, pthread rt)
$(call LINKBIN, test_sub, test_sub.c ach.o, pthread rt)
$(call LINKBIN, achcat, achcat.o ach.o, pthread rt)
$(call LINKBIN, achpipe.bin, achpipe.o ach_stream.o ach.o, pthread rt)
$(call LINKBIN, achchand, achchand.o, pthread rt amino)

$(call LINKBIN, ach, ach.o achtool.o, pthread rt)

ach.pyc: ach.py
	./pycompile

clean:
	rm -fv  *.o  test_pub ach.lisp test_sub $(BINFILES) $(LIBFILES) *.deb *.lzma *.pyc
	rm -rf debian doc $(PROJECT)-$(VERSION) .deps

.PHONY: doc

doc: $(INCLUDEDIR)/ach.h
	doxygen

