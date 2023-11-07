/*====---------------------- pci.c - PCI bus driver ----------------------====*\
 *
 * This code is a part of the Aulavik project.
 * Usage of these works is permitted provided that this instrument is retained
 * with the works, so that any entity that uses the works is notified of this
 * instrument. These works are provided without any warranty.
 *
\*====--------------------------------------------------------------------====*/

#include <kernel/driver/pci.h>

#include <kernel/driver/ports.h>
#include <kernel/logger.h>

/* read from a pci header. offset is in bytes */
uint16_t pci_config_rword(uint8_t bus, uint8_t slot,
			  uint8_t function, uint8_t offset) {
	uint32_t long_bus  = (uint32_t) bus;
	uint32_t long_slot = (uint32_t) slot;
	uint32_t long_function = (uint32_t) function;

	uint32_t address = (uint32_t)((long_bus << 16) | (long_slot << 11) |
	                     (long_function << 8) | (offset & 0xFC) | ((uint32_t)0x80000000));

	port_outl(PCI_CONFIG_ADDRESS, address);
	return (uint16_t)((port_inl(PCI_CONFIG_DATA) >> ((offset & 2) * 8)) & 0xFFFF);
}

uint16_t pci_get_vendor(uint8_t bus, uint8_t slot, uint8_t function)
{
	return pci_config_rword(bus, slot, function, 0);
}

uint16_t pci_get_device_id(uint8_t bus, uint8_t slot, uint8_t function)
{
	return pci_config_rword(bus, slot, function, 2);
}

uint8_t pci_get_class(uint8_t bus, uint8_t slot, uint8_t function)
{
	uint16_t class_data = pci_config_rword(bus, slot, function, 10);
	return (class_data >> 8) & 0xff;
}

uint8_t pci_get_subclass(uint8_t bus, uint8_t slot, uint8_t function)
{
	uint16_t class_data = pci_config_rword(bus, slot, function, 10);
	return class_data & 0xff;
}

void check_function(uint8_t bus, uint8_t slot, uint8_t function)
{
	uint16_t vendor = pci_get_vendor(bus, slot, function);
	uint16_t device, class, subclass;

	if (vendor != PCI_VENDOR_NONE) {
		device = pci_get_device_id(bus, slot, function);
		class = pci_get_class(bus, slot, function);
		subclass = pci_get_subclass(bus, slot, function);

		k_print("\t  Detected function %d", function);
		k_print("\t   | Vendor ID: %x, device ID: %x", vendor, device);
		k_print("\t   | Device class: %x, device subclass: %x", class, subclass);
	}
}

void check_device(uint8_t bus, uint8_t slot)
{
	uint16_t vendor = pci_get_vendor(bus, slot, 0);
	uint16_t device, class, subclass;

	if (vendor != PCI_VENDOR_NONE) {
		device = pci_get_device_id(bus, slot, 0);
		class = pci_get_class(bus, slot, 0);
		subclass = pci_get_subclass(bus, slot, 0);

		k_ok("Detected PCI device on bus %d slot %d", bus, slot);
		k_print("\t| Vendor ID: %x, device ID: %x", vendor, device);
		k_print("\t| Device class: %x, device subclass: %x", class, subclass);

		/* if the device has multiple functions */
		if (pci_config_rword(bus, slot, 0, 14) & 0b10000000) {
			k_print("\t| Device has multiple functions!\n");

			for (int function = 1; function < 8; function++) {
				check_function(bus, slot, function);
			}
		}

		k_print("");
	}
}

void pci_init(void)
{
	// check all pci busses
//	for (int bus = 0; bus < 256; bus++) {
	for (int bus = 0; bus < 4; bus++) {
		for (int slot = 0; slot < 32; slot++) {
			check_device(bus, slot);
		}
	}
}