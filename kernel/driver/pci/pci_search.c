/*====--------------- pci_search.h - PCI search functions  ---------------====*\
 *
 * This code is a part of the Aulavik project.
 * Usage of these works is permitted provided that this instrument is retained
 * with the works, so that any entity that uses the works is notified of this
 * instrument. These works are provided without any warranty.
 *
\*====--------------------------------------------------------------------====*/

#include "pci_search.h"

#include <stdbool.h>

#include <kernel/driver/pci.h>

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
	struct pci_function function_s;
	function_s.vendor_id = vendor_id;
	function_s.device_id = device_id;
	function_s.class_code = class_code;
	function_s.subclass_code = subclass_code;

	/* add the function, increment function_count */
	struct pci_device device_s = pci_devices[pci_device_count];
	function_s.function_index = device_s.function_count;
	device_s.functions[device_s.function_count] = function_s;
	device_s.function_count++;
	pci_devices[pci_device_count] = device_s;
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
		for (int func = 1; func < PCI_MAX_FUNCTIONS; func++)
			pci_scan_function(bus, slot, func);
	}

	pci_device_count++;
}

/* scan the provided bus for devices */
void pci_scan_bus(uint8_t bus)
{
	for (uint8_t slot = 0; slot < PCI_MAX_SLOTS; slot++)
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
	bool first_multi = pci_is_multifunction(0, 0);

	/* system has a single PCI host controller */
	if (!first_multi) {
		pci_scan_bus(0);
		return; /* other buses are guaranteed to be empty */
	}

	/* system has multiple PCI host controllers */
	for (int func = 0; func < PCI_MAX_FUNCTIONS; func++) {

		/* each function of the host at bus 0, device 0 controls a
		 * bus matching its function number */
		if (pci_get_vendor(0, 0, func) == PCI_VENDOR_NONE)
			break; /* no controller, reached the end of them */

		pci_scan_bus(func);
	}

	scanned = true;
}