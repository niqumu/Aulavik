# Default CFLAGS
CFLAGS?=-O2 -g
CFLAGS+= -Wall -Wextra

# All modules of the project
# Each one is expected to be a directory containing its own Makefile
# Each one is expected to have clean and install tasks
MODULES=\
libc\
kernel\

# Targeted architecture for build
export DEFAULT_HOST:=i686-elf
export HOST?=$(DEFAULT_HOST)
export HOST_ARCH!=./target-to-arch.sh $(HOST)
export ARCHDIR=arch/$(HOST_ARCH)

# Architecture-specific toolchain
export AR=$(HOST)-ar
export CC=$(HOST)-gcc

# File system layout for libraries and headers
export PREFIX=/usr
export EXEC_PREFIX=$(PREFIX)
export BOOTDIR=/boot
export LIBDIR=$(EXEC_PREFIX)/lib
export INCLUDEDIR=$(PREFIX)/include

# Configure compiler to use system root
export SYSROOT!=pwd
export SYSROOT:=$(SYSROOT)/sysroot
export CC+= --sysroot=$(SYSROOT) -isystem=$(INCLUDEDIR)
export DESTDIR=$(SYSROOT)

.PHONY: all build clean iso run

build:
	mkdir -p $(SYSROOT)
	$(foreach module, $(MODULES), $(MAKE) -C $(module) install-headers;) \
	$(foreach module, $(MODULES), $(MAKE) -C $(module) install;)

clean:
	$(foreach module, $(MODULES), $(MAKE) -C $(module) clean;) \
	rm -rf ./sysroot 		\
    rm -rf ./isodir 		\
    rm -rf ./out 			\
    rm -rf ./aulavik.iso

iso:
	mkdir -p ./isodir/boot/grub
	cp ./sysroot/boot/aulavik.kernel ./isodir/boot/aulavik.kernel
	cp ./grub.cfg ./isodir/boot/grub/grub.cfg
	grub-mkrescue -o aulavik.iso ./isodir

run:
	qemu-system-$(HOST_ARCH) -serial stdio -cdrom aulavik.iso

# Builds Aulavik, images it, and launches QEMU
all: build iso run