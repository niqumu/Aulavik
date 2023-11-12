/*====------------- ata_info.c - Parallel ATA information tool -----------====*\
 *
 * This code is a part of the Aulavik project.
 * Usage of these works is permitted provided that this instrument is retained
 * with the works, so that any entity that uses the works is notified of this
 * instrument. These works are provided without any warranty.
 *
\*====--------------------------------------------------------------------====*/

#include <kernel/driver/ata.h>

#include <kernel/logger.h>

void ata_dump_info(void)
{
	struct ata_controller controller = ata_get_controller();

	if (!controller.function.vendor_id) {
		k_print("\e[91mNo ATA/IDE controller present.");
		return;
	}

	k_print("ATA controller located at %d.%d:%d\n",
		controller.function.parent_device->bus,
		controller.function.parent_device->slot, 
		controller.function.index);

	k_print("Primary channel (%s mode)", controller.primary_native ? 
		"native" : "compatibility");
	k_print(" \e[90m|\e[97m base port: %x, base control port: %x, "
		"irq: %d\n", controller.primary_port,
		controller.primary_ctrl_port, controller.primary_irq);

	k_print("Secondary channel (%s mode)", controller.secondary_native ?
	                                     "native" : "compatibility");
	k_print(" \e[90m|\e[97m base port: %x, base control port: %x, "
		"irq: %d\n", controller.secondary_port,
		controller.secondary_ctrl_port, controller.secondary_irq);

	k_print("Connected drives");

	for (int i = 0; i < 4; i++) {
		struct ata_device device = ata_get_devices()[i];

		if (!device.present) {
			k_print(" \e[90m|\e[37m %d: (not detected)\e[97m", i);
			k_print(" \e[90m|\e[97m");
			continue;
		}

		k_print(" \e[90m|\e[97m %d: \"%s\": %d sectors", i,
			device.name, device.sectors);
		k_print(" \e[90m|\e[97m    Type: %s",
			device.type ? "ATAPI" : "ATA");
		k_print(" \e[90m|\e[97m    Channel: %d, drive: %s",
		        device.channel, device.drive ? "slave" : "master");
		k_print(" \e[90m|\e[97m");
	}
}