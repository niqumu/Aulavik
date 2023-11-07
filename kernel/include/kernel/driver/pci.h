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

#define PCI_HEADER_STANDARD     1
#define PCI_HEADER_PCI_PCI      2
#define PCI_HEADER_CARDBUS      3

#define PCI_VENDOR_NONE         0xffff

/* representation of a pci function */
typedef struct pci_function {

	/* location */
	uint8_t function_index;

	/* header info */
	uint16_t vendor_id;
	uint16_t device_id;
	uint8_t class_code;
	uint8_t subclass_code;
} pci_function_t;

/* representation of a pci device */
typedef struct pci_device {

	/* location */
	uint8_t bus;
	uint8_t slot;

	/* header info */
	uint16_t vendor_id;
	uint16_t device_id;
	uint8_t class_code;
	uint8_t subclass_code;

	/* functions */
	pci_function_t functions[8];
	uint8_t function_count;
} pci_device_t;

/* returns a pointer to the list of devices, setting device_count to the
 *   number of devices present */
pci_device_t* get_pci_devices(int *device_count);

void pci_scan(void);

void pci_init(void);

#endif /* _DRIVER_PCI_H */