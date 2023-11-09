/*====--------------------- ide.c - IDE disk driver ----------------------====*\
 *
 * This code is a part of the Aulavik project.
 * Usage of these works is permitted provided that this instrument is retained
 * with the works, so that any entity that uses the works is notified of this
 * instrument. These works are provided without any warranty.
 *
\*====--------------------------------------------------------------------====*/

#include <kernel/driver/ide.h>

#include <kernel/driver/ports.h>
#include <kernel/logger.h>

struct ide_device ide_devices[4];
uint8_t drives = 0;

struct ide_controller ide_controller;

uint8_t ide_buffer[2048] = {0};
static uint8_t atapi_packet[12] = {0xab, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

volatile bool waiting = false;

struct ide_device* ide_get_devices(void)
{
	return &ide_devices[0];
}

struct ide_controller ide_get_controller(void)
{
	return ide_controller;
}

void ide_tick(void)
{
	waiting = false;
}

void ide_wait(void)
{
	waiting = true;

	while (waiting)
		;
}

uint32_t ide_read_dword(uint8_t channel, uint8_t registr)
{
	if (channel == ATA_CHANNEL_PRIMARY) {
		if (registr >= 10) { /* register on control port */
			uint16_t port = ide_controller.primary_ctrl_port;
			return port_inl(port + registr - 10);
		} else { /* register on main port */
			uint16_t port = ide_controller.primary_port;
			return port_inl(port + registr);
		}
	} else { /* secondary channel */
		if (registr >= 10) { /* register on control port */
			uint16_t port = ide_controller.secondary_ctrl_port;
			return port_inl(port + registr - 10);
		} else { /* register on main port */
			uint16_t port = ide_controller.secondary_port;
			return port_inl(port + registr);
		}
	}
}

uint8_t ide_read_byte(uint8_t channel, uint8_t registr)
{
	if (channel == ATA_CHANNEL_PRIMARY) {
		if (registr >= 10) { /* register on control port */
			uint16_t port = ide_controller.primary_ctrl_port;
			return port_inb(port + registr - 10);
		} else { /* register on main port */
			uint16_t port = ide_controller.primary_port;
			return port_inb(port + registr);
		}
	} else { /* secondary channel */
		if (registr >= 10) { /* register on control port */
			uint16_t port = ide_controller.secondary_ctrl_port;
			return port_inb(port + registr - 10);
		} else { /* register on main port */
			uint16_t port = ide_controller.secondary_port;
			return port_inb(port + registr);
		}
	}
}

void ide_write_byte(uint8_t channel, uint8_t registr, uint8_t data)
{
	if (channel == ATA_CHANNEL_PRIMARY) {
		if (registr >= 10) { /* register on control port */
			uint16_t port = ide_controller.primary_ctrl_port;
			port_outb(port + registr - 10, data);
			return;
		} else { /* register on main port */
			uint16_t port = ide_controller.primary_port;
			port_outb(port + registr, data);
			return;
		}
	} else { /* secondary channel */
		if (registr >= 10) { /* register on control port */
			uint16_t port = ide_controller.secondary_ctrl_port;
			port_outb(port + registr - 10, data);
			return;
		} else { /* register on main port */
			uint16_t port = ide_controller.secondary_port;
			port_outb(port + registr, data);
			return;
		}
	}
}

void ide_read_buffer(uint8_t channel, uint8_t registr)
{
	for (int i = 0; i < 512; i += 4) {
		uint32_t data = ide_read_dword(channel, registr);
		ide_buffer[i + 1] = (data >> 0) & 0xff;
		ide_buffer[i + 0] = (data >> 8) & 0xff;
		ide_buffer[i + 3] = (data >> 16) & 0xff;
		ide_buffer[i + 2] = (data >> 24) & 0xff;
	}
}


/* test if a pci device contains an ide controller */
bool ide_scan_pci_device(struct pci_device *device)
{
	for (int j = 0; j < device->function_count; j++) {
		struct pci_function function = device->functions[j];

		if (function.class_code != PCI_CLASS_MASS_STORAGE)
			continue;

		ide_controller.function = function;
		return true;
	}

	return false;
}

/* scan pci devices to find an ide controller */
void ide_scan(void)
{
	for (int i = 0; i < pci_device_count; i++) {
		if (ide_scan_pci_device(&pci_devices[i]))
			break;
	}
}

void ide_set_up_ports(void)
{
	struct pci_function function = ide_controller.function;

	/* primary channel setup */
	/* if a channel is in compatibility mode, it uses pre-defined
	 * ports and a pre-defined irq. if the controller is in native mode
	 * instead, we must read the ports/irq from the appropriate BAR */
	if (function.prog_if & ATA_PROGIF_PRIMARY_NATIVE) {
		ide_controller.primary_native = true;
		ide_controller.primary_port = function.bars[0];
		ide_controller.primary_ctrl_port = function.bars[1];
		ide_controller.primary_irq = 0; /* todo */
	} else { /* compatibility mode */
		ide_controller.primary_native = false;
		ide_controller.primary_port = ATA_PRIMARY_COMPAT;
		ide_controller.primary_ctrl_port = ATA_PRIMARY_COMPAT_CONTROL;
		ide_controller.primary_irq = ATA_PRIMARY_COMPAT_IRQ;
	}

	/* secondary channel setup */
	if (function.prog_if & ATA_PROGIF_SECONDARY_NATIVE) {
		ide_controller.secondary_native = true;
		ide_controller.secondary_port = function.bars[2];
		ide_controller.secondary_ctrl_port = function.bars[3];
		ide_controller.secondary_irq = 0; /* todo */
	} else { /* compatibility mode */
		ide_controller.secondary_native = false;
		ide_controller.secondary_port = ATA_SECONDARY_COMPAT;
		ide_controller.secondary_ctrl_port = ATA_SECONDARY_COMPAT_CONTROL;
		ide_controller.secondary_irq = ATA_SECONDARY_COMPAT_IRQ;
	}
}

void ide_init(void) {
	ide_scan();

	if (!ide_controller.function.vendor_id) {
		k_error("IDE: No IDE controller found!");
		return;
	}

	ide_set_up_ports();

	/* detect drives */
	for (int channel = 0, index = 0; channel < 2; channel++) {
		for (int drive = 0; drive < 2; drive++, index++) {

			uint8_t status, type = ATA_TYPE_ATA;
			bool error = false;
			ide_devices[index].present = false;

			/* select the drive */
			ide_write_byte(channel, ATA_REGISTER_DRIVE_SELECT,
				       0xa0 | (drive << 4));
			ide_wait();

			/* ask the drive to self-identify */
			ide_write_byte(channel, ATA_REGISTER_COMMAND,
				       ATA_COMMAND_IDENTIFY);
			ide_wait();

			/* polling the response */
			if (!ide_read_byte(channel, ATA_REGISTER_STATUS))
				continue; /* no device present */

			while (true) { /* wait until the drive responds */
				status = ide_read_byte(channel,
				        ATA_REGISTER_STATUS);

				if (status & ATA_STATUS_ERROR) {
					error = true;
					break;
				}

				if (!(status & ATA_STATUS_BUSY) &&
					  (status & ATA_STATUS_DREQ_READY)) {
					break;
				}
			}

			/* probe for atapi */
			if (error) {
				uint8_t lba1 = ide_read_byte(channel,
						     ATA_REGISTER_LBA_1);
				uint8_t lba2 = ide_read_byte(channel,
			                             ATA_REGISTER_LBA_2);

				if ((lba1 == 0x14 && lba2 == 0xeb) ||
					  (lba1 == 0x69 && lba2 == 0x96))
					type = ATA_TYPE_ATAPI;
				else
					continue; /* unknown */

				ide_write_byte(channel, ATA_REGISTER_COMMAND,
					       ATA_COMMAND_IDENTIFY_PACKET);
				ide_wait();
			}

			/* todo read id space */
			ide_read_buffer(channel, ATA_REGISTER_DATA);

			ide_devices[index].present = true;
			ide_devices[index].type = type;
			ide_devices[index].channel = channel;
			ide_devices[index].drive = drive;
			ide_devices[index].signature = *((uint16_t*)(ide_buffer + ATA_IDENTIFY_TYPE));
			ide_devices[index].capabilities = *((uint16_t *)(ide_buffer + ATA_IDENTIFY_CAPABILITIES));
			ide_devices[index].command_sets = *((uint32_t *)(ide_buffer + ATA_IDENTIFY_COMMAND_SETS));

			/* get size */
			if (ide_devices[index].command_sets & (1 << 26))
				ide_devices[index].size = *((uint64_t *)
					(ide_buffer + ATA_IDENTIFY_MAX_LBA_EXT));
			else
				ide_devices[index].size = *((uint64_t *)
					(ide_buffer + ATA_IDENTIFY_MAX_LBA));

			/* get name */
			for (int k = 0; k < 40; k ++) {
				ide_devices[index].name[k] = ide_buffer[ATA_IDENTIFY_MODEL + k];
			}

			/* clean up the name */
			for (int k = 39; k > 0; k--) {
				if (ide_devices[index].name[k] != ' ') {
					ide_devices[index].name[k + 1] = '\0';
					break;
				}
			}

			drives++;
		}
	}

	k_ok("Loaded IDE driver, found %d drives", drives);
}