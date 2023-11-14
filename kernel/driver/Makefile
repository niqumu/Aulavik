# kernel/driver Makefile

include driver/fat32/make.config
include driver/keyboard/make.config
include driver/mouse/make.config
include driver/pci/make.config
include driver/storage/make.config

KERNEL_DRIVER_OBJS=	\
$(DRIVER_FAT32_OBJS) \
$(DRIVER_KEYBOARD_OBJS) \
$(DRIVER_MOUSE_OBJS) \
$(DRIVER_PCI_OBJS) \
$(DRIVER_STORAGE_OBJS) \
driver/ioapic.o \
driver/ports.o \
driver/ps2.o \
driver/serial.o \
