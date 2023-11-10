/*====----------- ata_access.c - Parallel ATA drive read/write -----------====*\
 *
 * This code is a part of the Aulavik project.
 * Usage of these works is permitted provided that this instrument is retained
 * with the works, so that any entity that uses the works is notified of this
 * instrument. These works are provided without any warranty.
 *
\*====--------------------------------------------------------------------====*/

#include <kernel/driver/ata.h>

void ata_access(uint8_t drive_index, uint64_t lba_address)
{
	struct ata_device drive = ata_get_devices()[drive_index];

	if (lba_address >= ATA_LBA28_MAX) {

		drive.address_mode = ATA_ADDRESSING_LBA48;

	} else if (drive.capabilities & ATA_CAPABILITIES_LBA) {

		drive.address_mode = ATA_ADDRESSING_LBA28;

	} else {

		drive.address_mode = ATA_ADDRESSING_CHS;

	}
}