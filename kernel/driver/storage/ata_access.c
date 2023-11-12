/*====----------- ata_access.c - Parallel ATA drive read/write -----------====*\
 *
 * This code is a part of the Aulavik project.
 * Usage of these works is permitted provided that this instrument is retained
 * with the works, so that any entity that uses the works is notified of this
 * instrument. These works are provided without any warranty.
 *
\*====--------------------------------------------------------------------====*/

#include <kernel/driver/ata.h>
#include "kernel/gdt/gdt.h"

static uint8_t ata_access(uint8_t dir, struct ata_device drive,
	uint64_t lba_address, uint8_t sector_count, uint8_t *buffer)
{
	uint16_t cylinder;
	uint8_t head, sector;

	/*
	 * todo: SOME ata drives might have sector sizes other than 512-bytes,
	 *   we need to check rather than blindly assuming the standard
	 */
	uint16_t sector_word_size = 256;

	/*
	 * todo: investigate why the QEMU drive indicates not supporting
	 *   LBA, when it does in actuality.
	 */
	/*if (!(drive.capabilities & ATA_CAPABILITIES_LBA)) {
		drive.address_mode = ATA_ADDRESSING_CHS;

		sector = (lba_address % 63) + 1;
		cylinder = (lba_address + 1 - sector) / (16 * 63);
		head = (lba_address + 1 - sector) % (16 * 63) / 63;
	} else*/

	if (lba_address > ATA_LBA28_MAX) {
		drive.address_mode = ATA_ADDRESSING_LBA48;
		head = 0;
	} else {
		drive.address_mode = ATA_ADDRESSING_LBA28;
		head = (lba_address & 0xf000000) >> 24;
	}

	/* wait for the channel to be ready */
	ata_wait_until_ready(drive.channel);

	/* tell the controller to select this drive */
	uint16_t flag = drive.address_mode == ATA_ADDRESSING_CHS ? 0xa0 : 0xe0;
	ata_write_byte(drive.channel, ATA_REGISTER_DRIVE_SELECT,
	               flag | (drive.drive << 4) | head);


	/* write information to the controller */
	ata_write_byte(drive.channel, ATA_REGISTER_SEC_COUNT_0, sector_count);

	if (drive.address_mode == ATA_ADDRESSING_CHS) {
		ata_write_byte(drive.channel, ATA_REGISTER_LBA_0,
		               sector);
		ata_write_byte(drive.channel, ATA_REGISTER_LBA_1,
		               (cylinder >> 0) & 0xff);
		ata_write_byte(drive.channel, ATA_REGISTER_LBA_2,
		               (cylinder >> 8) & 0xff);
	} else {
		ata_write_byte(drive.channel, ATA_REGISTER_LBA_0,
		               (lba_address & 0xff) >> 0);
		ata_write_byte(drive.channel, ATA_REGISTER_LBA_1,
		               (lba_address & 0xff00) >> 8);
		ata_write_byte(drive.channel, ATA_REGISTER_LBA_2,
		               (lba_address & 0xff0000) >> 16);
	}

	/* write extra information (larger values) if using lba48 */
	if (drive.address_mode == ATA_ADDRESSING_LBA48) {
		ata_write_byte(drive.channel, ATA_REGISTER_SEC_COUNT_1, 0);
		ata_write_byte(drive.channel, ATA_REGISTER_LBA_3,
		               (lba_address & 0xff000000) >> 24);
		ata_write_byte(drive.channel, ATA_REGISTER_LBA_4,
		               (lba_address & 0xff00000000) >> 32);
		ata_write_byte(drive.channel, ATA_REGISTER_LBA_5,
		               (lba_address & 0xff0000000000) >> 40);
	}

	/* write command to controller */
	uint16_t command;
	if (dir == ATA_DIRECTION_READ) {
		if (drive.address_mode == ATA_ADDRESSING_LBA48) {
			command = ATA_COMMAND_READ_PIO_EXT;
		} else {
			command = ATA_COMMAND_READ_PIO;
		}
	} else {
		if (drive.address_mode == ATA_ADDRESSING_LBA48) {
			command = ATA_COMMAND_WRITE_PIO_EXT;
		} else {
			command = ATA_COMMAND_WRITE_PIO;
		}
	}
	ata_write_byte(drive.channel, ATA_REGISTER_COMMAND, command);

	uint16_t port = (drive.channel == ATA_CHANNEL_SECONDARY) ?
		ata_get_controller().secondary_port :
		ata_get_controller().primary_port;

	/* actually read/write */
	if (dir == ATA_DIRECTION_READ) {

		for (int i = 0; i < sector_count; i++) {

			/* check for an error */
//			if ((error = ata_read_byte(drive.channel,
//						   ATA_REGISTER_ERROR))) {
//				return error;
//			}

			ata_wait_until_ready(drive.channel);

			asm("pushw %es");
			asm("mov %%ax, %%es" :: "a" (GDT_SEGMENT_KERNEL_DATA));

			/* receive data */
			asm("rep insw" :: "c" (sector_word_size),
			"d" (port), "D" (buffer));

			asm("popw %es");
			buffer += (sector_word_size * 2);
		}

	} else { /* write */

		for (int i = 0; i < sector_count; i++) {

			ata_wait_until_ready(drive.channel);

			asm("pushw %ds");
			asm("mov %%ax, %%ds" :: "a" (GDT_SEGMENT_KERNEL_DATA));

			/* send data */
			asm("rep outsw" :: "c" (sector_word_size),
				"d" (port), "S" (buffer));

			asm("popw %ds");
			buffer += (sector_word_size * 2);
		}

		if (drive.address_mode == ATA_ADDRESSING_LBA48) {
			ata_write_byte(drive.channel, ATA_REGISTER_COMMAND,
				       ATA_COMMAND_CACHE_FLUSH_EXT);

		} else {
			ata_write_byte(drive.channel, ATA_REGISTER_COMMAND,
				       ATA_COMMAND_CACHE_FLUSH);
		}

		ata_wait_until_ready(drive.channel);
	}

	return ATA_ACCESS_OK;
}

/**
 * Read a number of segments, no greater than 255, from an ATA disk, starting
 * at a specifed segment, into a buffer.
 *
 * @param drive The drive to read from
 * @param lba_address The Logical Block Addressing scheme address of the sector
 *      to begin reading data from
 * @param sector_count The number of sectors to read
 * @param buffer The buffer to write data into
 * @return ATA_ACCESS_OK, or an error raised by the ATA controller
 */
uint8_t ata_read_sectors(struct ata_device drive, uint64_t lba_address,
                         uint8_t sector_count, uint8_t *buffer)
{
	/* verify target drive */
	if (!drive.present)
		return ATA_ACCESS_ERROR_NOSUCHDEVICE;

	/* verify input */
	if (drive.type == ATA_TYPE_ATA &&
	    lba_address + sector_count > drive.sectors) {
		return ATA_ACCESS_ERROR_OUTOFRANGE;
	}

	/* read */
	return ata_access(ATA_DIRECTION_READ, drive,
	                  lba_address, sector_count, buffer);
}

/**
 * Write a number of segments, no greater than 255, to an ATA disk, starting
 * at a specifed segment, from a buffer.
 *
 * @param drive The drive to write to
 * @param lba_address The Logical Block Addressing scheme address of the sector
 *      to begin writing data to
 * @param sector_count The number of sectors to write to
 * @param buffer The buffer to read from
 * @return ATA_ACCESS_OK, or an error raised by the ATA controller
 */
uint8_t ata_write_sectors(struct ata_device drive, uint64_t lba_address,
                          uint8_t sector_count, uint8_t *buffer)
{

	/* verify target drive */
	if (!drive.present) {

		/* optical drives can't be written to like this */
		if (drive.type == ATA_TYPE_ATAPI)
			return ATA_ACCESS_ERROR_PROTECTED;

		return ATA_ACCESS_ERROR_NOSUCHDEVICE;
	}

	/* verify input */
	if (drive.type == ATA_TYPE_ATA &&
	    lba_address + sector_count > drive.sectors) {
		return ATA_ACCESS_ERROR_OUTOFRANGE;
	}

	/* write */
	return ata_access(ATA_DIRECTION_WRITE, drive,
	                  lba_address, sector_count, buffer);
}