# kernel/driver Makefile

include driver/keyboard/make.config
include driver/pci/make.config

KERNEL_DRIVER_OBJS=	\
$(DRIVER_KEYBOARD_OBJS) \
$(DRIVER_PCI_OBJS) \
driver/ata.o \
driver/ioapic.o \
driver/ps2.o \
driver/serial.o \
