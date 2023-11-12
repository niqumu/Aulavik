/*====----------- ata_access.c - Parallel ATA drive read/write -----------====*\
 *
 * This code is a part of the Aulavik project.
 * Usage of these works is permitted provided that this instrument is retained
 * with the works, so that any entity that uses the works is notified of this
 * instrument. These works are provided without any warranty.
 *
\*====--------------------------------------------------------------------====*/

#include <kernel/driver/ata.h>

static uint8_t ata_access(uint8_t dir, uint8_t drive_ind, uint64_t lba_address,
		   uint8_t sector_count, uint8_t selector, void *buffer)
{
	struct ata_device drive = ata_get_devices()[drive_ind];
	uint16_t cylinder;
	uint8_t head, sector, error;

	/*
	 * todo: SOME ata drives might have sector sizes other than 512-bytes,
	 *   we need to check rather than blindly assuming the standard
	 */
	uint16_t sector_wsize = 256;

	if (!(drive.capabilities & ATA_CAPABILITIES_LBA)) {
		drive.address_mode = ATA_ADDRESSING_CHS;

		// todo i have no idea where these come from and why it works
		sector = (lba_address % 63) + 1;
		cylinder = (lba_address + 1 - sector) / (16 * 63);
		head = (lba_address + 1 - sector) % (16 * 63) / 63;
	} else if (lba_address > ATA_LBA28_MAX) {
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

	uint16_t port = (drive.channel == ATA_CHANNEL_SECONDARY) ?
		ata_get_controller().secondary_port :
		ata_get_controller().primary_port;

	/* actually read/write */
	if (dir == ATA_DIRECTION_READ) {

		for (int i = 0; i < sector_count; i++) {

			/* check for an error */
			if ((error = ata_read_byte(drive.channel,
						   ATA_REGISTER_ERROR))) {
				return error;
			}

			asm("pushw %es");
			asm("mov %%ax, %%es" :: "a" (selector));

			/* receive data */
			asm("rep insw" :: "c" (sector_wsize),
				"d" (port), "D" (buffer));

			asm("popw %es");
			buffer += (sector_wsize * 2);
		}

	} else { /* write */

		for (int i = 0; i < sector_count; i++) {

			ata_wait_until_ready(drive.channel);

			asm("pushw %ds");
			asm("mov %%ax, %%ds" :: "a" (selector));

			/* send data */
			asm("rep outsw" :: "c" (sector_wsize),
				"d" (port), "S" (buffer));

			asm("popw %ds");
			buffer += (sector_wsize * 2);
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

uint8_t ata_read_sectors(uint8_t drive_index, uint64_t lba_address,
			 uint8_t sector_count, uint8_t selector, void *buffer)
{
	struct ata_device drive = ata_get_devices()[drive_index];

	/* verify target drive */
	if (drive_index > ATA_MAX_DRIVES - 1 || !drive.present)
		return ATA_ACCESS_ERROR_NOSUCHDEVICE;

	/* verify input */
	if (drive.type == ATA_TYPE_ATA &&
		lba_address + sector_count > drive.sectors) {
		return ATA_ACCESS_ERROR_OUTOFRANGE;
	}

	/* read */
	if (ata_access(ATA_DIRECTION_READ, drive_index,
		       lba_address, sector_count, selector, buffer))
		return ATA_ACCESS_ERROR_INTERNAL;

	return ATA_ACCESS_OK;
}

uint8_t ata_write_sectors(uint8_t drive_index, uint64_t lba_address,
                          uint8_t sector_count, uint8_t selector, void *buffer)
{
	struct ata_device drive = ata_get_devices()[drive_index];

	/* verify target drive */
	if (drive_index > ATA_MAX_DRIVES - 1 || !drive.present) {

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
	if (ata_access(ATA_DIRECTION_WRITE, drive_index,
	               lba_address, sector_count, selector, buffer))
		return ATA_ACCESS_ERROR_INTERNAL;

	return ATA_ACCESS_OK;
}