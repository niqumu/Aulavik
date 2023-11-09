/*====------------------- pci.h - PCI bus driver header ------------------====*\
 *
 * This code is a part of the Aulavik project.
 * Usage of these works is permitted provided that this instrument is retained
 * with the works, so that any entity that uses the works is notified of this
 * instrument. These works are provided without any warranty.
 *
\*====--------------------------------------------------------------------====*/

#ifndef DRIVER_PCI_H
#define DRIVER_PCI_H

#include <stdbool.h>
#include <stdint.h>

#define PCI_CONFIG_ADDRESS      0xcf8
#define PCI_CONFIG_DATA         0xcfc

#define PCI_MAX_SLOTS           32
#define PCI_MAX_FUNCTIONS       8

#define PCI_HEADER_STANDARD     1
#define PCI_HEADER_PCI_PCI      2
#define PCI_HEADER_CARDBUS      3

/* todo load all these from a file eventually, there are
 *   a lot of these, and thousands of vendors */
#define PCI_CLASS_UNCLASSIFIED          0x0
#define PCI_CLASS_MASS_STORAGE          0x1

/* mass storage subclasses */
#define PCI_SUBCLASS_MS_SCSI            0x0
#define PCI_SUBCLASS_MS_IDE             0x1
#define PCI_SUBCLASS_MS_FLOPPY          0x2
#define PCI_SUBCLASS_MS_IPI             0x3
#define PCI_SUBCLASS_MS_RAID            0x4
#define PCI_SUBCLASS_MS_ATA             0x5
#define PCI_SUBCLASS_MS_SATA            0x6
#define PCI_SUBCLASS_MS_SASCSI          0x7
#define PCI_SUBCLASS_MS_NVMC            0x8
#define PCI_SUBCLASS_MS_OTHER           0x80

#define PCI_VENDOR_NONE         0xffff

/* representation of a pci function */
struct pci_function {

	/* location */
	uint8_t function_index;

	/* header info */
	uint16_t vendor_id;
	uint16_t device_id;
	uint8_t device_revision;
	uint8_t class_code;
	uint8_t subclass_code;
	uint8_t prog_if;
};

/* representation of a pci device */
struct pci_device {

	/* location */
	uint8_t bus;
	uint8_t slot;

	/* header info */
	uint16_t vendor_id;
	uint16_t device_id;
	uint8_t device_revision;
	uint8_t class_code;
	uint8_t subclass_code;
	uint8_t prog_if;

	/* functions */
	struct pci_function functions[PCI_MAX_FUNCTIONS];
	uint8_t function_count;
};

/* devices list */
extern struct pci_device pci_devices[128];
extern int pci_device_count;

/* state */
extern bool scanned;

/* returns a pointer to the list of devices, setting device_count to the
 *   number of devices present */
struct pci_device* pci_get_devices(int *device_count);

/* get the vendor id of the device at the given location */
uint16_t pci_get_vendor(uint8_t bus, uint8_t slot, uint8_t function);

/* get the device id of the device at the given location */
uint16_t pci_get_device_id(uint8_t bus, uint8_t slot, uint8_t function);

/* get the class id of the device at the given location */
uint8_t pci_get_class(uint8_t bus, uint8_t slot, uint8_t function);

/* get the subclass id of the device at the given location */
uint8_t pci_get_subclass(uint8_t bus, uint8_t slot, uint8_t function);

/* get the programming interfaces of the device at the given location */
uint8_t pci_get_prog_ifs(uint8_t bus, uint8_t slot, uint8_t function);

/* get the revision id of the device at the given location */
uint8_t pci_get_revision(uint8_t bus, uint8_t slot, uint8_t function);

/* get the header type of the device at the given location */
uint8_t pci_get_header_type(uint8_t bus, uint8_t slot, uint8_t function);

/* test if the device at the given location has functions */
bool pci_is_multifunction(uint8_t bus, uint8_t slot);

void pci_list_devices(void);

void pci_scan(void);

void pci_init(void);

#endif /* DRIVER_PCI_H */