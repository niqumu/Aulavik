/*====------------------- pci.h - PCI bus driver header ------------------====*\
 *
 * This code is a part of the Aulavik project.
 * Usage of these works is permitted provided that this instrument is retained
 * with the works, so that any entity that uses the works is notified of this
 * instrument. These works are provided without any warranty.
 *
\*====--------------------------------------------------------------------====*/

#include <stdint.h>

#ifndef _DRIVER_PCI_H
#define _DRIVER_PCI_H

#define PCI_CONFIG_ADDRESS      0xcf8
#define PCI_CONFIG_DATA         0xcfc

#define PCI_VENDOR_NONE         0xffff

typedef struct pci_device {
	uint16_t vendor_id;
	uint16_t device_id;
	uint8_t class_code;
	uint8_t subclass_code;
} pci_device_t;

void pci_init(void);

#endif /* _DRIVER_PCI_H */