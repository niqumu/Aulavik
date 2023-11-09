/*====---------------------- pci.c - PCI bus driver ----------------------====*\
 *
 * This code is a part of the Aulavik project.
 * Usage of these works is permitted provided that this instrument is retained
 * with the works, so that any entity that uses the works is notified of this
 * instrument. These works are provided without any warranty.
 *
\*====--------------------------------------------------------------------====*/

#include <kernel/driver/pci.h>

#include <kernel/driver/ports.h> /* ports_outl(), ports_inl() */
#include <kernel/logger.h>

#include "pci_search.h"

/* devices list */
struct pci_device pci_devices[128];
int pci_device_count = 0;

/* state */
bool scanned = false;

/* returns a pointer to the list of devices, setting device_count to the
 *   number of devices present */
struct pci_device* pci_get_devices(int *device_count)
{
	/* if something is somehow trying to fetch the devices before we've
	 * scanned them, stop and scan first */
	if (!scanned)
		pci_scan();

	*device_count = pci_device_count;
	return pci_devices;
}

/* read from a pci header. offset is in bytes */
uint16_t pci_config_rword(uint8_t bus, uint8_t slot,
			  uint8_t function, uint8_t offset) {
	uint32_t bus_l  = (uint32_t) bus;
	uint32_t slot_l = (uint32_t) slot;
	uint32_t func_l = (uint32_t) function;

	/* find configuration address */
	uint32_t address = (uint32_t) ((bus_l << 16) | (slot_l << 11) |
	                               (func_l << 8) | (offset & 0xfc));
	address |= 0x80000000; /* enable bit for config access */

	/* request the config by address */
	port_outl(PCI_CONFIG_ADDRESS, address);
	uint32_t config = port_inl(PCI_CONFIG_DATA);

	return (uint16_t)((config >> ((offset & 2) * 8)) & 0xffff);
}

/* get the vendor id of the device at the given location */
uint16_t pci_get_vendor(uint8_t bus, uint8_t slot, uint8_t function)
{
	return pci_config_rword(bus, slot, function, 0);
}

/* get the device id of the device at the given location */
uint16_t pci_get_device_id(uint8_t bus, uint8_t slot, uint8_t function)
{
	return pci_config_rword(bus, slot, function, 2);
}

/* get the class id of the device at the given location */
uint8_t pci_get_class(uint8_t bus, uint8_t slot, uint8_t function)
{
	/* 11th byte of config */
	uint16_t byte = pci_config_rword(bus, slot, function, 10);
	return (byte >> 8) & 0xff;
}

/* get the subclass id of the device at the given location */
uint8_t pci_get_subclass(uint8_t bus, uint8_t slot, uint8_t function)
{
	/* 10th byte of config */
	uint16_t byte = pci_config_rword(bus, slot, function, 10);
	return byte & 0xff;
}

/* get the programming interfaces of the device at the given location */
uint8_t pci_get_prog_ifs(uint8_t bus, uint8_t slot, uint8_t function)
{
	/* 9th byte of config */
	uint16_t byte = pci_config_rword(bus, slot, function, 8);
	return (byte >> 8) & 0xff;
}

/* get the revision id of the device at the given location */
uint8_t pci_get_revision(uint8_t bus, uint8_t slot, uint8_t function)
{
	/* 8th byte of config */
	uint16_t byte = pci_config_rword(bus, slot, function, 8);
	return byte & 0xff;
}

/* get the header type of the device at the given location */
uint8_t pci_get_header_type(uint8_t bus, uint8_t slot, uint8_t function)
{
	uint16_t header_data = pci_config_rword(bus, slot, function, 14);
	return header_data & 0xf;
}

/* get the (bar)th base address register from the device */
uint32_t pci_get_bar(uint8_t bus, uint8_t slot, uint8_t function, uint8_t bar)
{
	uint8_t offset = 16 + (4 * bar);
	uint32_t lower_bar = pci_config_rword(bus, slot, function, offset);
	uint32_t upper_bar = pci_config_rword(bus, slot, function, offset + 2);
	return lower_bar | (upper_bar << 16);
}

/* test if the device at the given location has functions */
bool pci_is_multifunction(uint8_t bus, uint8_t slot)
{
	uint16_t header_data = pci_config_rword(bus, slot, 0, 14);
	return header_data & 0b10000000;
}

static void pci_list_functions(struct pci_device device)
{
	if (!device.function_count)
		return;

	k_print(" \e[90m|\e[97m Functions:");

	for (int f = 0; f < device.function_count; f++) {
		struct pci_function func = device.functions[f];

		k_print(" \e[90m|\e[97m  \e[90m|\e[97m %d: vendor: "
		        "\e[92m%x\e[97m, ID: \e[92m%x\e[97m, "
		        "type: \e[92m%x\e[97m, subclass: \e[92m%x"
		        "\e[97m, interface: \e[92m%x\e[97m", f, func.vendor_id,
			func.device_id, func.class_code, func.subclass_code,
			func.prog_if);
	}
}

/* print a list of all devices and their functions */
void pci_list_devices(void)
{
	k_print("%d PCI devices present\n", pci_device_count);

	for (int index = 0; index < pci_device_count; index++) {
		struct pci_device device = pci_devices[index];

		k_print("Device \e[96m%d\e[97m: device type: \e[94m%x\e[97m, "
			"device subclass: \e[94m%x\e[97m, device interface: "
			"\e[94m%x\e[97m", index, device.class_code,
			device.subclass_code, device.prog_if);

		k_print(" \e[90m|\e[97m Vendor: %x, device: %x, device "
			"revision: %x", device.vendor_id, device.device_id,
		        device.device_revision);

		k_print(" \e[90m|\e[97m Device location: bus: %d, slot: %d",
			device.bus, device.slot);

		pci_list_functions(device);
		k_print("");
	}
}

void pci_init(void)
{
	pci_scan();
	k_ok("Loaded PCI driver, found %d devices", pci_device_count);
//	pci_list_devices();
}