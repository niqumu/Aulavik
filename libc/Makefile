# libc makefile

CFLAGS?=-O2 -g
CPPFLAGS?=
LDFLAGS?=
NASMFLAGS?=
LIBS?=

CFLAGS:=$(CFLAGS) -ffreestanding -Wall -Wextra
CPPFLAGS:=$(CPPFLAGS) -D__AULAVIK_LIBC -Iinclude
LIBK_CFLAGS:=$(CFLAGS)
NASMFLAGS:=$(NASMFLAGS) -felf32
LIBK_CPPFLAGS:=$(CPPFLAGS) -D__AULAVIK_LIBK

include $(ARCHDIR)/make.config

include stdio/make.config
include stdlib/make.config
include string/make.config

CFLAGS:=$(CFLAGS) $(ARCH_CFLAGS)
CPPFLAGS:=$(CPPFLAGS) $(ARCH_CPPFLAGS)
LIBK_CFLAGS:=$(LIBK_CFLAGS) $(KERNEL_ARCH_CFLAGS)
LIBK_CPPFLAGS:=$(LIBK_CPPFLAGS) $(KERNEL_ARCH_CPPFLAGS)
 
FREEOBJS=\
$(ARCH_FREEOBJS) \
$(LIBC_STDIO_OBJS) \
$(LIBC_STDLIB_OBJS) \
$(LIBC_STRING_OBJS) \
 
HOSTEDOBJS=\
$(ARCH_HOSTEDOBJS) \
 
OBJS=\
$(FREEOBJS) \
$(HOSTEDOBJS) \
 
LIBK_OBJS=$(FREEOBJS:.o=.libk.o)
 
BINARIES=libc.a libk.a
 
.PHONY: all clean install install-headers install-libs
.SUFFIXES: .o .libc.o .libk.o .c .asm
 
all: $(BINARIES)
 
libc.a: $(OBJS)
	$(AR) rcs $@ $(OBJS)
 
libk.a: $(LIBK_OBJS)
	$(AR) rcs $@ $(LIBK_OBJS)
 
.c.o:
	$(CC) -MD -c $< -o $@ -std=gnu11 $(CFLAGS) $(CPPFLAGS)
 
.asm.o:
	nasm $(NASMFLAGS) $< -o $@
 
.c.libk.o:
	$(CC) -MD -c $< -o $@ -std=gnu11 $(LIBK_CFLAGS) $(LIBK_CPPFLAGS)
 
.asm.libk.o:
	$(CC) -MD -c $< -o $@ $(LIBK_CFLAGS) $(LIBK_CPPFLAGS)
 
clean:
	rm -f $(BINARIES) *.a
	rm -f $(OBJS) $(LIBK_OBJS) *.o */*.o */*/*.o
	rm -f $(OBJS:.o=.d) $(LIBK_OBJS:.o=.d) *.d */*.d */*/*.d
 
install: install-headers install-libs
 
install-headers:
	mkdir -p $(DESTDIR)$(INCLUDEDIR)
	cp -R --preserve=timestamps include/. $(DESTDIR)$(INCLUDEDIR)/.
 
install-libs: $(BINARIES)
	mkdir -p $(DESTDIR)$(LIBDIR)
	cp $(BINARIES) $(DESTDIR)$(LIBDIR)
 
-include $(OBJS:.o=.d)
-include $(LIBK_OBJS:.o=.d)
