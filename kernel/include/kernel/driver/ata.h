/*====------------------ ata.c - ATA disk driver header ------------------====*\
 *
 * This code is a part of the Aulavik project.
 * Usage of these works is permitted provided that this instrument is retained
 * with the works, so that any entity that uses the works is notified of this
 * instrument. These works are provided without any warranty.
 *
\*====--------------------------------------------------------------------====*/

#include <stdint.h>

#ifndef _DRIVER_ATA_H
#define _DRIVER_ATA_H

/* commands sent over command/status port */
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

/* statuses received over command/status port */
#define ATA_STATUS_ERROR        0b00000001
#define ATA_STATUS_INDEX        0b00000010
#define ATA_STATUS_CORRECTED    0b00000100
#define ATA_STATUS_DREQ_READY   0b00001000
#define ATA_STATUS_SEEK_DONE    0b00010000
#define ATA_STATUS_WRITE_FAULT  0b00100000
#define ATA_STATUS_READY        0b01000000
#define ATA_STATUS_BUSY         0b10000000

/* errors received over features/error port */
#define ATA_ERROR_NO_ADDR_MARK  0b00000001
#define ATA_ERROR_NO_TRACK_0    0b00000010
#define ATA_ERROR_COMMAND_ABORT 0b00000100
#define ATA_ERROR_MEDIA_CH_REQ  0b00001000
#define ATA_ERROR_NO_ID_MARK    0b00010000
#define ATA_ERROR_MEDIA_CHANGE  0b00100000
#define ATA_ERROR_UNCORRECTABLE 0b01000000
#define ATA_ERROR_BAD_BLOCK     0b10000000

#define ATA_PRIMARY     0
#define ATA_SECONDARY   1
#define ATA_READ        0
#define ATA_WRITE       1
#define ATA_ATA         0
#define ATA_ATAPI       1
#define ATA_MASTER      0
#define ATA_SLAVE       1

typedef struct ata_device {
	uint8_t present;
	uint8_t secondary;
	uint8_t slave;
	uint8_t atapi;
	uint16_t signature;
	uint16_t capabilities;
	uint32_t command_sets;
	uint32_t sectors;
	char name[64];
} ata_device_t;

void init_ata(uint32_t primary_addr, uint32_t primary_ctrl_addr,
	      uint32_t secondary_addr, uint32_t secondary_ctrl_addr,
	      uint32_t bus_master);

#endif /* _DRIVER_ATA_H */