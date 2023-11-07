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

pci_device_t pci_devices[128];
int pci_device_count = 0;

pci_device_t* get_pci_devices(int *device_count)
{
	*device_count = pci_device_count;
	return pci_devices;
}

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
	uint16_t class_data = pci_config_rword(bus, slot, function, 10);
	return (class_data >> 8) & 0xff;
}

/* get the subclass id of the device at the given location */
uint8_t pci_get_subclass(uint8_t bus, uint8_t slot, uint8_t function)
{
	uint16_t class_data = pci_config_rword(bus, slot, function, 10);
	return class_data & 0xff;
}

/* get the header type of the device at the given location */
uint8_t pci_get_header_type(uint8_t bus, uint8_t slot, uint8_t function)
{
	uint16_t header_data = pci_config_rword(bus, slot, function, 14);
	return header_data & 0xf;
}

/* test if the device at the given location has functions */
uint8_t pci_is_multifunction(uint8_t bus, uint8_t slot)
{
	uint16_t header_data = pci_config_rword(bus, slot, 0, 14);
	return header_data & 0b10000000;
}

/* scan the provided function */
void pci_scan_function(uint8_t bus, uint8_t slot, uint8_t function)
{
	uint32_t vendor_id = pci_get_vendor(bus, slot, function);
	uint16_t device_id, class_code, subclass_code;

	if (!vendor_id || vendor_id == PCI_VENDOR_NONE)
		return; /* function doesn't exist */

	device_id = pci_get_device_id(bus, slot, function);
	class_code = pci_get_class(bus, slot, function);
	subclass_code = pci_get_subclass(bus, slot, function);

	/* create and populate a function struct */
	pci_function_t function_t;
	function_t.vendor_id = vendor_id;
	function_t.device_id = device_id;
	function_t.class_code = class_code;
	function_t.subclass_code = subclass_code;

	/* add the function, increment function_count */
	pci_device_t device_t = pci_devices[pci_device_count];
	function_t.function_index = device_t.function_count;
	device_t.functions[device_t.function_count] = function_t;
	device_t.function_count++;
	pci_devices[pci_device_count] = device_t;
}

/* scan the provided device for functions */
void pci_scan_device(uint8_t bus, uint8_t slot)
{
	uint32_t vendor_id = pci_get_vendor(bus, slot, 0);
	uint16_t device_id, class_code, subclass_code;

	if (!vendor_id || vendor_id == PCI_VENDOR_NONE)
		return; /* device doesn't exist */

	device_id = pci_get_device_id(bus, slot, 0);
	class_code = pci_get_class(bus, slot, 0);
	subclass_code = pci_get_subclass(bus, slot, 0);

	pci_devices[pci_device_count].bus = bus;
	pci_devices[pci_device_count].slot = slot;
	pci_devices[pci_device_count].function_count = 0;
	pci_devices[pci_device_count].device_id = device_id;
	pci_devices[pci_device_count].vendor_id = vendor_id;
	pci_devices[pci_device_count].class_code = class_code;
	pci_devices[pci_device_count].subclass_code = subclass_code;

	/* if it's a multifunction device we need to scan its functions */
	if (pci_is_multifunction(bus, slot)) {
		for (int function = 1; function < 8; function++)
			pci_scan_function(bus, slot, function);
	}

	pci_device_count++;
}

/* scan the provided bus for devices */
void pci_scan_bus(uint8_t bus)
{
	for (uint8_t slot = 0; slot < 32; slot++)
		pci_scan_device(bus, slot);
}

/* perform a clean scan for pci devices */
void pci_scan(void)
{
	pci_device_count = 0;

	/* the device at bus 0, device 0, is always a PCI host controller.
	 *   if the controller is not a multifunction device, the system
	 *   will only have devices on bus 0. if it is a multifunction
	 *   device, each function will be a controller for that bus. */
	uint8_t first_multi = pci_is_multifunction(0, 0);

	/* system has a single PCI host controller */
	if (!first_multi) {
		pci_scan_bus(0);
		return;
	}

	/* system has multiple PCI host controllers */
	for (int function = 0; function < 8; function++) {
		if (pci_get_vendor(0, 0, function) == PCI_VENDOR_NONE)
			break; /* no controller, reached the end of them */

		pci_scan_bus(function);
	}
}

/* print a list of all devices and their functions */
void pci_list_devices(void)
{
	for (int index = 0; index < pci_device_count; index++) {
		pci_device_t device = pci_devices[index];

		if (device.subclass_code) {
			k_print("Device %d: vendor: %x, ID: %x, "
			        "class: %x, subclass: %x", index,
			        device.vendor_id, device.device_id,
			        device.class_code, device.subclass_code);
		} else {
			k_print("Device %d: vendor: %x, ID: %x, class: %x",
			        index, device.vendor_id, device.device_id,
			        device.class_code);
		}

		/* print the device's functions */
		for (int f = 0; f < device.function_count; f++) {
			pci_function_t func = device.functions[f];

			if (func.subclass_code) {
				k_print("   | %d: vendor: %x, ID: %x, "
				        "class: %x, subclass: %x", f,
				        func.vendor_id, func.device_id,
				        func.class_code, func.subclass_code);
			} else {
				k_print("   | %d: vendor: %x, ID: %x, "
				        "class: %x", f,
				        func.vendor_id, func.device_id,
				        func.class_code);
			}
		}
	}
}

void pci_init(void)
{
	pci_scan();
	k_ok("Loaded PCI driver, found %d devices", pci_device_count);
//	pci_list_devices();
}