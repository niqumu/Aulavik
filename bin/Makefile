# bin makefile

CFLAGS?=-O2 -g
CPPFLAGS?=
LDFLAGS?=
NASMFLAGS?=
LIBS?=
 
CFLAGS:=$(CFLAGS) -ffreestanding -Wall -Wextra
CPPFLAGS:=$(CPPFLAGS) -Iinclude
LDFLAGS:=$(LDFLAGS)
NASMFLAGS:=$(NASMFLAGS) -felf32
LIBS:=$(LIBS) -nostdlib -lc -lgcc

include ted/make.config

CFLAGS:=$(CFLAGS) $(ARCH_CFLAGS)
CPPFLAGS:=$(CPPFLAGS) $(ARCH_CPPFLAGS)
LDFLAGS:=$(LDFLAGS) $(ARCH_LDFLAGS)
NASMFLAGS:=$(NASMFLAGS) $(ARCH_NASMFLAGS)
LIBS:=$(LIBS)
 
OBJS=\
$(BIN_TED_OBJS)
 
LINK_LIST=\
$(OBJS) \
$(LIBS)

BINARIES=\
ted.elf
 
.PHONY: all clean install install-headers install-bins
.SUFFIXES: .o .c .asm
 
all: $(BINARIES)

ted.elf: $(OBJS)
	$(CC) -o $@ $(CFLAGS) $(LINK_LIST)

.c.o:
	$(CC) -MD -c $< -o $@ -std=gnu11 $(CFLAGS) $(CPPFLAGS)

.asm.o:
	nasm $(NASMFLAGS) $< -o $@

clean:
	rm -f $(BINARIES) *.a
	rm -f $(OBJS) *.o */*.o */*/*.o
	rm -f $(OBJS:.o=.d) *.d */*.d */*/*.d
 
install: install-headers install-bins

install-headers:

install-bins: $(BINARIES)
	mkdir -p $(DESTDIR)$(BINDIR)
	cp $(BINARIES) $(DESTDIR)$(BINDIR)

-include $(OBJS:.o=.d)
