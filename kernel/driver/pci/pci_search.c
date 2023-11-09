/*====--------------- pci_search.h - PCI search functions  ---------------====*\
 *
 * This code is a part of the Aulavik project.
 * Usage of these works is permitted provided that this instrument is retained
 * with the works, so that any entity that uses the works is notified of this
 * instrument. These works are provided without any warranty.
 *
\*====--------------------------------------------------------------------====*/

#include "pci_search.h"
#include "kernel/logger.h"

#include <stdbool.h>

#include <kernel/driver/pci.h>

/* populate the base address registers of a function */
/* todo: there is no way of editing the registers! */
void pci_scan_function_bars(struct pci_function *function)
{
	if (function->header_type == PCI_HEADER_CARDBUS)
		return; /* pci-to-cardbus bridges don't have BARs */

	int max_bars = 6; /* max for general devices */

	if (function->header_type == PCI_HEADER_PCI_PCI)
		max_bars = 2; /* pci-pci bridges only have two BARs */

	for (int bar = 0; bar < max_bars; bar++) {
		uint32_t data = pci_get_bar(function->parent_device->bus,
					    function->parent_device->slot,
					    function->index, bar);
		function->bars[bar] = data;;
	}
}

/* populate the base address registers of a device */
/* todo: there is no way of editing the registers! */
void pci_scan_device_bars(struct pci_device *device)
{
	if (device->header_type == PCI_HEADER_CARDBUS)
		return; /* pci-to-cardbus bridges don't have BARs */

	int max_bars = 6; /* max for general devices */

	if (device->header_type == PCI_HEADER_PCI_PCI)
		max_bars = 2; /* pci-pci bridges only have two BARs */

	for (int bar = 0; bar < max_bars; bar++) {
		uint32_t data = pci_get_bar(device->bus, device->slot, 0, bar);
		device->bars[bar] = data;
	}
}

/* scan the function at the provided location, building a pci_function struct
 * and adding it to the current device (top of pci_devices) if it exists */
void pci_scan_function(uint8_t bus, uint8_t slot, uint8_t function)
{
	uint32_t vendor_id = pci_get_vendor(bus, slot, function);
	uint16_t device_id, class_code, subclass_code, revision,
			prog_if, header_type;

	if (!vendor_id || vendor_id == PCI_VENDOR_NONE)
		return; /* function doesn't exist */

	header_type = pci_get_header_type(bus, slot, function);
	device_id = pci_get_device_id(bus, slot, function);
	class_code = pci_get_class(bus, slot, function);
	subclass_code = pci_get_subclass(bus, slot, function);
	revision = pci_get_revision(bus, slot, function);
	prog_if = pci_get_prog_ifs(bus, slot, function);

	/* create and populate a function struct */
	struct pci_function function_s;
	function_s.header_type = header_type;
	function_s.vendor_id = vendor_id;
	function_s.device_id = device_id;
	function_s.class_code = class_code;
	function_s.subclass_code = subclass_code;
	function_s.device_revision = revision;
	function_s.prog_if = prog_if;
	function_s.parent_device = &pci_devices[pci_device_count];
	
	/* add the function, increment function_count */
	struct pci_device device_s = pci_devices[pci_device_count];
	function_s.index = device_s.function_count;

	/* populate the function's base address registers */
	pci_scan_function_bars(&function_s);

	device_s.functions[device_s.function_count] = function_s;
	device_s.function_count++;
	pci_devices[pci_device_count] = device_s;
}

/* scan the device at the provided location, building a pci_device struct
 * and adding it to the pci_devices list if it exists */
void pci_scan_device(uint8_t bus, uint8_t slot)
{
	uint32_t vendor_id = pci_get_vendor(bus, slot, 0);
	uint16_t device_id, class_code, subclass_code, revision,
			prog_if, header_type;

	if (!vendor_id || vendor_id == PCI_VENDOR_NONE)
		return; /* device doesn't exist */

	header_type = pci_get_header_type(bus, slot, 0);
	device_id = pci_get_device_id(bus, slot, 0);
	class_code = pci_get_class(bus, slot, 0);
	subclass_code = pci_get_subclass(bus, slot, 0);
	revision = pci_get_revision(bus, slot, 0);
	prog_if = pci_get_prog_ifs(bus, slot, 0);

	struct pci_device device_s;
	device_s.header_type = header_type;
	device_s.bus = bus;
	device_s.slot = slot;
	device_s.function_count = 0;
	device_s.device_id = device_id;
	device_s.vendor_id = vendor_id;
	device_s.class_code = class_code;
	device_s.subclass_code = subclass_code;
	device_s.device_revision = revision;
	device_s.prog_if = prog_if;

	/* populate the device's base address registers */
	pci_scan_device_bars(&device_s);
	
	pci_devices[pci_device_count] = device_s;

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