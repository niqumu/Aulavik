# kernel/driver Makefile

include driver/keyboard/make.config
include driver/pci/make.config
include driver/storage/make.config

KERNEL_DRIVER_OBJS=	\
$(DRIVER_KEYBOARD_OBJS) \
$(DRIVER_PCI_OBJS) \
$(DRIVER_STORAGE_OBJS) \
driver/ioapic.o \
driver/ports.o \
driver/ps2.o \
driver/serial.o \
