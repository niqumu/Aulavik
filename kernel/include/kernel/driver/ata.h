/*====-------------- ata.h - Parallel ATA disk driver header -------------====*\
 *
 * This code is a part of the Aulavik project.
 * Usage of these works is permitted provided that this instrument is retained
 * with the works, so that any entity that uses the works is notified of this
 * instrument. These works are provided without any warranty.
 *
\*====--------------------------------------------------------------------====*/

#ifndef DRIVER_ATA_H
#define DRIVER_ATA_H

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

#define ATA_CAPABILITIES_LBA            0x200
#define ATA_LBA28_MAX                   0x10000000

/* ------------------------------------------ */
/*  Registers                                 */
/* ------------------------------------------ */

/* channel registers, offset from their base port */
#define ATA_REGISTER_DATA               0       // read/write 
#define ATA_REGISTER_ERROR              1       // read only 
#define ATA_REGISTER_FEATURES           1       // write only 
#define ATA_REGISTER_SEC_COUNT_0        2       // read/write
#define ATA_REGISTER_LBA_0              3       // read/write  bits 0-7 of lba
#define ATA_REGISTER_LBA_1              4       // read/write  bits 8-15 of lba
#define ATA_REGISTER_LBA_2              5       // read/write  bits 16-25 of lba
#define ATA_REGISTER_DRIVE_SELECT       6       // read/write 
#define ATA_REGISTER_COMMAND            7       // write only 
#define ATA_REGISTER_STATUS             7       // read only
#define ATA_REGISTER_SEC_COUNT_1        8
#define ATA_REGISTER_LBA_3              9       // bits 24-31 of lba

/* channel registers, offset from their base control port */
/* the actual values are ten lower, but they have been raised by ten here
 *   to be able to be distinguished from base port offsets. */
#define ATA_REGISTER_LBA_4              10      // bits 32-39 of lba
#define ATA_REGISTER_LBA_5              11      // bits 40-47 of lba
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

/* offsets of values in the identification buffer */
#define ATA_IDENTIFY_TYPE               0
#define ATA_IDENTIFY_CYLINDERS          2
#define ATA_IDENTIFY_HEADS              6
#define ATA_IDENTIFY_SECTORS            12
#define ATA_IDENTIFY_SERIAL             20
#define ATA_IDENTIFY_MODEL              54
#define ATA_IDENTIFY_CAPABILITIES       98
#define ATA_IDENTIFY_FIELD_VALID        106
#define ATA_IDENTIFY_MAX_LBA            120
#define ATA_IDENTIFY_COMMAND_SETS       164
#define ATA_IDENTIFY_MAX_LBA_EXT        200

#define ATA_DIRECTION_READ      0
#define ATA_DIRECTION_WRITE     1

#define ATA_ACCESS_OK                   0
#define ATA_ACCESS_ERROR_NOSUCHDEVICE   1   /* provided drive doesn't exist */
#define ATA_ACCESS_ERROR_OUTOFRANGE     2   /* provided region is too big */
#define ATA_ACCESS_ERROR_INTERNAL       3   /* drive itself raised an error */
#define ATA_ACCESS_ERROR_PROTECTED      4   /* drive is read/write protected */

#define ATA_MAX_DRIVES          4

struct ata_controller {
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

struct ata_device {
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

#define ATA_ADDRESSING_UNKNOWN  0
#define ATA_ADDRESSING_CHS      1
#define ATA_ADDRESSING_LBA28    2
#define ATA_ADDRESSING_LBA48    3
	uint8_t address_mode;

	uint16_t signature;
	uint16_t capabilities;
	uint32_t command_sets;
	uint32_t sectors;

	char name[40];
};

/* implemented in ata_access.c */
uint8_t ata_read_sectors(uint8_t drive_index, uint64_t lba_address,
                         uint8_t sector_count, uint8_t selector, void *buffer);

uint8_t ata_write_sectors(uint8_t drive_index, uint64_t lba_address,
                          uint8_t sector_count, uint8_t selector, void *buffer);

/* implemented in ata.c */
void ata_wait_until_ready(uint8_t channel);

struct ata_device* ata_get_devices(void);

struct ata_controller ata_get_controller(void);

uint8_t ata_read_byte(uint8_t channel, uint8_t registr);

void ata_write_byte(uint8_t channel, uint8_t registr, uint8_t data);

void ata_tick(void);

void ata_dump_info(void);

void ata_init(void);

#endif /* DRIVER_ATA_H */
