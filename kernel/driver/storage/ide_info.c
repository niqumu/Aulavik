/*====----------------- ide_info.c - IDE information tool ----------------====*\
 *
 * This code is a part of the Aulavik project.
 * Usage of these works is permitted provided that this instrument is retained
 * with the works, so that any entity that uses the works is notified of this
 * instrument. These works are provided without any warranty.
 *
\*====--------------------------------------------------------------------====*/

#include <kernel/driver/ide.h>

#include <kernel/driver/pci.h>
#include <kernel/logger.h>

void ide_dump_info(void)
{
	struct ide_controller controller = ide_get_controller();

	if (!controller.function.vendor_id) {
		k_print("\e[91mNo IDE controller present.");
		return;
	}

	k_print("IDE controller located at %d.%d:%d\n", 
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
		struct ide_device device = ide_get_devices()[i];

		if (!device.present) {
			k_print(" \e[90m|\e[37m %d: absent\e[97m", i);
			continue;
		}

		k_print(" \e[90m|\e[97m %d: %s %s", i, device.channel ?
			"Secondary" : "Primary", device.drive ? "slave" : "master");
	}
}