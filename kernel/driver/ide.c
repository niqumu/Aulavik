/*====--------------------- ide.c - IDE disk driver ----------------------====*\
 *
 * This code is a part of the Aulavik project.
 * Usage of these works is permitted provided that this instrument is retained
 * with the works, so that any entity that uses the works is notified of this
 * instrument. These works are provided without any warranty.
 *
\*====--------------------------------------------------------------------====*/

#include <kernel/driver/ide.h>
#include <kernel/driver/pci.h>
#include <kernel/logger.h>

struct ide_device ide_devices[4];

void ide_scan_pci_device(struct pci_device device)
{
	for (int j = 0; j < device.function_count; j++) {
		struct pci_function function = device.functions[j];

		if (function.class_code != PCI_CLASS_MASS_STORAGE)
			continue;

		k_print("IDE: Storage controller at %d.%d:%d!",
		        device.bus, device.slot,
		        function.function_index);
	}
}

void ide_scan()
{
	for (int i = 0; i < pci_device_count; i++) {
		ide_scan_pci_device(pci_devices[i]);
	}
}

void ide_init()
{
	ide_scan();
	k_ok("Loaded IDE driver");
}