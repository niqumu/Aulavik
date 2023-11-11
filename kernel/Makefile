# kernel makefile

CFLAGS?=-O2 -g
CPPFLAGS?=
LDFLAGS?=
NASMFLAGS?=
LIBS?=
 
CFLAGS:=$(CFLAGS) -ffreestanding -Wall -Wextra
CPPFLAGS:=$(CPPFLAGS) -D__is_kernel -Iinclude
LDFLAGS:=$(LDFLAGS)
NASMFLAGS:=$(NASMFLAGS) -felf32
LIBS:=$(LIBS) -nostdlib -lk -lgcc

include bin/make.config
include driver/Makefile
include file/make.config
include gdt/make.config
include graphics/make.config
include interrupts/make.config
include memory/make.config
include $(ARCHDIR)/make.config
 
CFLAGS:=$(CFLAGS) $(KERNEL_ARCH_CFLAGS)
CPPFLAGS:=$(CPPFLAGS) $(KERNEL_ARCH_CPPFLAGS)
LDFLAGS:=$(LDFLAGS) $(KERNEL_ARCH_LDFLAGS)
NASMFLAGS:=$(NASMFLAGS) $(KERNEL_ARCH_NASMFLAGS)
LIBS:=$(LIBS) $(KERNEL_ARCH_LIBS)
 
KERNEL_OBJS=\
$(KERNEL_ARCH_OBJS) \
$(KERNEL_BIN_OBJS) \
$(KERNEL_DRIVER_OBJS) \
$(KERNEL_FILE_OBJS) \
$(KERNEL_GDT_OBJS) \
$(KERNEL_GRAPHICS_OBJS) \
$(KERNEL_INTERRUPT_OBJS) \
$(KERNEL_MEMORY_OBJS) \
kernel.o \
logger.o \
watchdog.o \
 
OBJS=\
$(ARCHDIR)/crti.o \
$(ARCHDIR)/crtbegin.o \
$(KERNEL_OBJS) \
$(ARCHDIR)/crtend.o \
$(ARCHDIR)/crtn.o \
 
LINK_LIST=\
$(ARCHDIR)/crti.o \
$(ARCHDIR)/crtbegin.o \
$(KERNEL_OBJS) \
$(LIBS) \
$(ARCHDIR)/crtend.o \
$(ARCHDIR)/crtn.o \
 
.PHONY: all clean install install-headers install-kernel
.SUFFIXES: .o .c .asm
 
all: aulavik.kernel
 
aulavik.kernel: $(OBJS) $(ARCHDIR)/linker.ld
	$(CC) -T $(ARCHDIR)/linker.ld -o $@ $(CFLAGS) $(LINK_LIST)
	grub-file --is-x86-multiboot aulavik.kernel
 
$(ARCHDIR)/crtbegin.o $(ARCHDIR)/crtend.o:
	OBJ=`$(CC) $(CFLAGS) $(LDFLAGS) -print-file-name=$(@F)` && cp "$$OBJ" $@
 
.c.o:
	$(CC) -MD -c $< -o $@ -std=gnu11 $(CFLAGS) $(CPPFLAGS)
 
.asm.o:
	nasm $(NASMFLAGS) $< -o $@
 
clean:
	rm -f aulavik.kernel
	rm -f $(OBJS) *.o */*.o */*/*.o
	rm -f $(OBJS:.o=.d) *.d */*.d */*/*.d
 
install: install-headers install-kernel
 
install-headers:
	mkdir -p $(DESTDIR)$(INCLUDEDIR)
	cp -R --preserve=timestamps include/. $(DESTDIR)$(INCLUDEDIR)/.
 
install-kernel: aulavik.kernel
	mkdir -p $(DESTDIR)$(BOOTDIR)
	cp aulavik.kernel $(DESTDIR)$(BOOTDIR)
 
-include $(OBJS:.o=.d)
