/*====------------------ ide.h - IDE disk driver header ------------------====*\
 *
 * This code is a part of the Aulavik project.
 * Usage of these works is permitted provided that this instrument is retained
 * with the works, so that any entity that uses the works is notified of this
 * instrument. These works are provided without any warranty.
 *
\*====--------------------------------------------------------------------====*/

#ifndef DRIVER_IDE_H
#define DRIVER_IDE_H

#include <stdint.h>

#include <kernel/driver/pci.h>

/* ATA ports/locations in compatibility mode. if the channel is in native mode
 * instead, these values must be read from the devices BAR. */
#define ATA_PRIMARY_COMPAT              0x1f0   /* i/o port */
#define ATA_PRIMARY_COMPAT_CONTROL      0x3f6   /* i/o port */
#define ATA_PRIMARY_COMPAT_IRQ          14
#define ATA_SECONDARY_COMPAT            0x170   /* i/o port */
#define ATA_SECONDARY_COMPAT_CONTROL    0x376   /* i/o port */
#define ATA_SECONDARY_COMPAT_IRQ        15

/* pci program interface bits */
#define ATA_PROGIF_PRIMARY_NATIVE       0b00000001
#define ATA_PROGIF_BIT_0_SWITCH         0b00000010 /* native/compat switch */
#define ATA_PROGIF_SECONDARY_NATIVE     0b00000100
#define ATA_PROGIF_BIT_2_SWITCH         0b00001000 /* native/compat switch */
#define ATA_PROGIF_BUS_MASTER           0b10000000

#define ATA_CHANNEL_PRIMARY             0
#define ATA_CHANNEL_SECONDARY           1

/* ------------------------------------------ */
/*  Registers                                 */
/* ------------------------------------------ */

/* channel registers, offset from their base port */
#define ATA_REGISTER_DATA               0       // read/write 
#define ATA_REGISTER_ERROR              1       // read only 
#define ATA_REGISTER_FEATURES           1       // write only 
#define ATA_REGISTER_SEC_COUNT_0        2       // read/write
#define ATA_REGISTER_LBA_0              3       // read/write 
#define ATA_REGISTER_LBA_1              4       // read/write 
#define ATA_REGISTER_LBA_2              5       // read/write 
#define ATA_REGISTER_DRIVE_SELECT       6       // read/write 
#define ATA_REGISTER_COMMAND            7       // write only 
#define ATA_REGISTER_STATUS             7       // read only
#define ATA_REGISTER_SEC_COUNT_1        8
#define ATA_REGISTER_LBA_3              9

/* channel registers, offset from their base control port */
/* the actual values are ten lower, but they have been raised by ten here
 *   to be able to be distinguished from base port offsets. */
#define ATA_REGISTER_LBA_4              10
#define ATA_REGISTER_LBA_5              11
#define ATA_CTRL_REGISTER_STATUS_ALT    12      // read only
#define ATA_CTRL_REGISTER_CONTROL       12      // write only
#define ATA_CTRL_REGISTER_ADDRESS       13      // read only

/* ------------------------------------------ */
/*  Register values                           */
/* ------------------------------------------ */

/* commands sent over command/status port, offset 7 from base */
#define ATA_COMMAND_READ_PIO            0x20
#define ATA_COMMAND_READ_PIO_EXT        0x24
#define ATA_COMMAND_READ_DMA            0xc8
#define ATA_COMMAND_READ_DMA_EXT        0x25
#define ATA_COMMAND_WRITE_PIO           0x30
#define ATA_COMMAND_WRITE_PIO_EXT       0x34
#define ATA_COMMAND_WRITE_DMA           0xca
#define ATA_COMMAND_WRITE_DMA_EXT       0x35
#define ATA_COMMAND_CACHE_FLUSH         0xe7
#define ATA_COMMAND_CACHE_FLUSH_EXT     0xea
#define ATA_COMMAND_PACKET              0xa0
#define ATA_COMMAND_IDENTIFY_PACKET     0xa1
#define ATA_COMMAND_IDENTIFY            0xec

/* statuses received over command/status port, offset 7 from base */
#define ATA_STATUS_ERROR        0b00000001
#define ATA_STATUS_INDEX        0b00000010
#define ATA_STATUS_CORRECTED    0b00000100
#define ATA_STATUS_DREQ_READY   0b00001000
#define ATA_STATUS_SEEK_DONE    0b00010000
#define ATA_STATUS_WRITE_FAULT  0b00100000
#define ATA_STATUS_READY        0b01000000
#define ATA_STATUS_BUSY         0b10000000

/* errors received over features/error port, offset 1 from base */
#define ATA_ERROR_NO_ADDR_MARK  0b00000001
#define ATA_ERROR_NO_TRACK_0    0b00000010
#define ATA_ERROR_COMMAND_ABORT 0b00000100
#define ATA_ERROR_MEDIA_CH_REQ  0b00001000
#define ATA_ERROR_NO_ID_MARK    0b00010000
#define ATA_ERROR_MEDIA_CHANGE  0b00100000
#define ATA_ERROR_UNCORRECTABLE 0b01000000
#define ATA_ERROR_BAD_BLOCK     0b10000000

#define ATA_READ        0
#define ATA_WRITE       1

struct ide_controller {
	struct pci_function function;

	/* primary channel */
	bool primary_native;
	uint32_t primary_port;          /* start of ports */
	uint32_t primary_ctrl_port;     /* start of control ports */
	uint8_t primary_irq;

	/* secondary channel */
	bool secondary_native;
	uint32_t secondary_port;
	uint32_t secondary_ctrl_port;
	uint8_t secondary_irq;
};

struct ide_device {
	bool present;

#define ATA_CHANNEL_PRIMARY     0
#define ATA_CHANNEL_SECONDARY   1
	uint8_t channel;

#define ATA_DRIVE_MASTER        0
#define ATA_DRIVE_SLAVE         1
	uint8_t drive;

#define ATA_TYPE_ATA    0
#define ATA_TYPE_ATAPI  1
	uint8_t type;

	uint16_t signature;
	uint16_t capabilities;
	uint32_t command_sets;
	uint32_t sectors;
	char name[64];
};

struct ide_device* ide_get_devices(void);

struct ide_controller ide_get_controller(void);

void ide_tick(void);

void ide_dump_info(void);

void ide_init(void);

#endif /* DRIVER_IDE_H */