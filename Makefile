# Aulavik master Makefile

# Troubleshooting:
# * Is the file listed in its directories make.config?
# * Is the directories make.config included in the next higher Makefile/config?
# * Is the directories list used in the next higher Makefile/make.config?
#
# Weird Makefile parsing errors (like expecting a tab, something about ***,
#   etc) coming from a source file likely means it was accidentally added to
#   a make.config file as a .c/.asm (or whatever) file rather than a .o file.
#
# GCC errors about -lk being an unrecognized option/file mean that the libk
#   did not compile or link correctly. Clean and build again, looking for
#   issues coming from libk sources.

# Default CFLAGS
CFLAGS?=-O2 -g
CFLAGS+= -Wall -Wextra

# All modules of the project
# Each one is expected to be a directory containing its own Makefile
# Each one is expected to have clean and install tasks
MODULES=\
libc\
bin\
kernel\

# Targeted architecture for build
export DEFAULT_HOST:=i686-elf
export HOST?=$(DEFAULT_HOST)
export HOST_ARCH!=./target-to-arch.sh $(HOST)
export ARCHDIR=arch/$(HOST_ARCH)

# Architecture-specific toolchain
export AR=$(HOST)-ar
export CC=$(HOST)-gcc
export LD=$(HOST)-ld

# File system layout for libraries and headers
export PREFIX=/usr
export EXEC_PREFIX=$(PREFIX)
export BINDIR=/bin
export BOOTDIR=/boot
export LIBDIR=$(EXEC_PREFIX)/lib
export INCLUDEDIR=$(PREFIX)/include

# Configure compiler to use system root
export SYSROOT!=pwd
export SYSROOT:=$(SYSROOT)/sysroot
export CC+= --sysroot=$(SYSROOT) -isystem=$(INCLUDEDIR)
export DESTDIR=$(SYSROOT)

.PHONY: all build clean iso run

# Build Aulavik
build:
	mkdir -p $(SYSROOT)
	# The || exit at the end stops the build process immediately on error
	$(foreach module, $(MODULES), $(MAKE) -C $(module) install-headers || exit;) \
	$(foreach module, $(MODULES), $(MAKE) -C $(module) install || exit;)

# Clean binaries. Good first resort if Make is acting strange
clean:
	$(foreach module, $(MODULES), $(MAKE) -C $(module) clean;) \
	rm -rf ./sysroot 		\
	rm -rf ./isodir 		\
	rm -rf ./out 			\
	rm -rf ./aulavik.iso

# Create a bootable image with GRUB
iso:
	mkdir -p ./isodir/boot/grub
	cp ./sysroot/boot/aulavik.kernel ./isodir/boot/aulavik.kernel
	cp ./grub.cfg ./isodir/boot/grub/grub.cfg
	grub-mkrescue -o aulavik.iso ./isodir

# Launch QEMU, boot the image, mount the virtual drive
run:
	# Start the VM, mounting the ./sysroot directory as a FAT formatted ATA
	#   drive, and the aulavik ISO as a CD-ROM. -d flag forces CD boot.
	#
	#   https://wiki.gentoo.org/wiki/QEMU/Options#Hard_drive
	qemu-system-$(HOST_ARCH) -m 256M -serial stdio -hdb ./ted.img -cdrom ./aulavik.iso -boot d

	#qemu-system-$(HOST_ARCH) -serial stdio -drive file="./aulavik.iso",
	#media=disk -drive file=fat:rw:"./sysroot",format=raw

# Builds Aulavik, images it, and launches QEMU
all: build iso run