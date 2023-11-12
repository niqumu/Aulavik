/*====----------------- ata.c - Parallel ATA disk driver -----------------====*\
 *
 * This code is a part of the Aulavik project.
 * Usage of these works is permitted provided that this instrument is retained
 * with the works, so that any entity that uses the works is notified of this
 * instrument. These works are provided without any warranty.
 *
\*====--------------------------------------------------------------------====*/

#include <kernel/driver/ata.h>

#include <kernel/driver/ports.h>
#include <kernel/logger.h>

struct ata_device ata_devices[ATA_MAX_DRIVES];
uint8_t drives = 0;

struct ata_controller ata_controller;

uint8_t ata_buffer[2048] = {0};
static uint8_t atapi_packet[12] = {0xab, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

volatile bool waiting = false;

struct ata_device* ata_get_devices(void)
{
	return &ata_devices[0];
}

struct ata_controller ata_get_controller(void)
{
	return ata_controller;
}

void ata_tick(void)
{
	waiting = false;
}

void ata_wait(void)
{
	waiting = true;

	while (waiting)
		;
}

void ata_wait_until_ready(uint8_t channel)
{
	while (ata_read_byte(channel, ATA_REGISTER_STATUS) & ATA_STATUS_BUSY)
		;
}

uint32_t ata_read_dword(uint8_t channel, uint8_t registr)
{
	if (channel == ATA_CHANNEL_PRIMARY) {
		if (registr >= 10) { /* register on control port */
			uint16_t port = ata_controller.primary_ctrl_port;
			return port_inl(port + registr - 10);
		} else { /* register on main port */
			uint16_t port = ata_controller.primary_port;
			return port_inl(port + registr);
		}
	} else { /* secondary channel */
		if (registr >= 10) { /* register on control port */
			uint16_t port = ata_controller.secondary_ctrl_port;
			return port_inl(port + registr - 10);
		} else { /* register on main port */
			uint16_t port = ata_controller.secondary_port;
			return port_inl(port + registr);
		}
	}
}

uint8_t ata_read_byte(uint8_t channel, uint8_t registr)
{
	if (channel == ATA_CHANNEL_PRIMARY) {
		if (registr >= 10) { /* register on control port */
			uint16_t port = ata_controller.primary_ctrl_port;
			return port_inb(port + registr - 10);
		} else { /* register on main port */
			uint16_t port = ata_controller.primary_port;
			return port_inb(port + registr);
		}
	} else { /* secondary channel */
		if (registr >= 10) { /* register on control port */
			uint16_t port = ata_controller.secondary_ctrl_port;
			return port_inb(port + registr - 10);
		} else { /* register on main port */
			uint16_t port = ata_controller.secondary_port;
			return port_inb(port + registr);
		}
	}
}

void ata_write_byte(uint8_t channel, uint8_t registr, uint8_t data)
{
	if (channel == ATA_CHANNEL_PRIMARY) {
		if (registr >= 10) { /* register on control port */
			uint16_t port = ata_controller.primary_ctrl_port;
			port_outb(port + registr - 10, data);
			return;
		} else { /* register on main port */
			uint16_t port = ata_controller.primary_port;
			port_outb(port + registr, data);
			return;
		}
	} else { /* secondary channel */
		if (registr >= 10) { /* register on control port */
			uint16_t port = ata_controller.secondary_ctrl_port;
			port_outb(port + registr - 10, data);
			return;
		} else { /* register on main port */
			uint16_t port = ata_controller.secondary_port;
			port_outb(port + registr, data);
			return;
		}
	}
}

void ata_read_buffer(uint8_t channel, uint8_t registr)
{
	for (int i = 0; i < 512; i += 4) {
		uint32_t data = ata_read_dword(channel, registr);
		ata_buffer[i + 1] = (data >> 0) & 0xff;
		ata_buffer[i + 0] = (data >> 8) & 0xff;
		ata_buffer[i + 3] = (data >> 16) & 0xff;
		ata_buffer[i + 2] = (data >> 24) & 0xff;
	}
}


/* test if a pci device contains an ide controller */
bool ata_scan_pci_device(struct pci_device *device)
{
	for (int j = 0; j < device->function_count; j++) {
		struct pci_function function = device->functions[j];

		if (function.class_code != PCI_CLASS_MASS_STORAGE)
			continue;

		ata_controller.function = function;
		return true;
	}

	return false;
}

/* scan pci devices to find an ide controller */
void ata_scan(void)
{
	for (int i = 0; i < pci_device_count; i++) {
		if (ata_scan_pci_device(&pci_devices[i]))
			break;
	}
}

void ata_set_up_ports(void)
{
	struct pci_function function = ata_controller.function;

	/* primary channel setup */
	/* if a channel is in compatibility mode, it uses pre-defined
	 * ports and a pre-defined irq. if the controller is in native mode
	 * instead, we must read the ports/irq from the appropriate BAR */
	if (function.prog_if & ATA_PROGIF_PRIMARY_NATIVE) {
		ata_controller.primary_native = true;
		ata_controller.primary_port = function.bars[0];
		ata_controller.primary_ctrl_port = function.bars[1];
		ata_controller.primary_irq = 0; /* todo */
	} else { /* compatibility mode */
		ata_controller.primary_native = false;
		ata_controller.primary_port = ATA_PRIMARY_COMPAT;
		ata_controller.primary_ctrl_port = ATA_PRIMARY_COMPAT_CONTROL;
		ata_controller.primary_irq = ATA_PRIMARY_COMPAT_IRQ;
	}

	/* secondary channel setup */
	if (function.prog_if & ATA_PROGIF_SECONDARY_NATIVE) {
		ata_controller.secondary_native = true;
		ata_controller.secondary_port = function.bars[2];
		ata_controller.secondary_ctrl_port = function.bars[3];
		ata_controller.secondary_irq = 0; /* todo */
	} else { /* compatibility mode */
		ata_controller.secondary_native = false;
		ata_controller.secondary_port = ATA_SECONDARY_COMPAT;
		ata_controller.secondary_ctrl_port = ATA_SECONDARY_COMPAT_CONTROL;
		ata_controller.secondary_irq = ATA_SECONDARY_COMPAT_IRQ;
	}
}

void ata_init(void) {
	ata_scan();

	if (!ata_controller.function.vendor_id) {
		k_error("ATA: No IDE controller found!");
		return;
	}

	ata_set_up_ports();

	/* detect drives */
	for (int channel = 0, index = 0; channel < 2; channel++) {
		for (int drive = 0; drive < 2; drive++, index++) {

			uint8_t status, type = ATA_TYPE_ATA;
			bool error = false;
			ata_devices[index].present = false;

			/* select the drive */
			ata_write_byte(channel, ATA_REGISTER_DRIVE_SELECT,
				       0xa0 | (drive << 4));
			ata_wait();

			/* ask the drive to self-identify */
			ata_write_byte(channel, ATA_REGISTER_COMMAND,
				       ATA_COMMAND_IDENTIFY);
			ata_wait();

			/* polling the response */
			if (!ata_read_byte(channel, ATA_REGISTER_STATUS))
				continue; /* no device present */

			while (true) { /* wait until the drive responds */
				status = ata_read_byte(channel,
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
				uint8_t lba1 = ata_read_byte(channel,
						     ATA_REGISTER_LBA_1);
				uint8_t lba2 = ata_read_byte(channel,
			                             ATA_REGISTER_LBA_2);

				if ((lba1 == 0x14 && lba2 == 0xeb) ||
					  (lba1 == 0x69 && lba2 == 0x96))
					type = ATA_TYPE_ATAPI;
				else
					continue; /* unknown */

				ata_write_byte(channel, ATA_REGISTER_COMMAND,
					       ATA_COMMAND_IDENTIFY_PACKET);
				ata_wait();
			}

			ata_read_buffer(channel, ATA_REGISTER_DATA);

			ata_devices[index].present = true;
			ata_devices[index].type = type;
			ata_devices[index].channel = channel;
			ata_devices[index].drive = drive;
			ata_devices[index].signature = *((uint16_t*)
				(ata_buffer + ATA_IDENTIFY_TYPE));
			ata_devices[index].capabilities = *((uint16_t *)
				(ata_buffer + ATA_IDENTIFY_CAPABILITIES));
			ata_devices[index].command_sets = *((uint32_t *)
				(ata_buffer + ATA_IDENTIFY_COMMAND_SETS));

			/* get size */
			if (ata_devices[index].command_sets & (1 << 26))
				ata_devices[index].sectors = *((uint64_t *)
					(ata_buffer + ATA_IDENTIFY_MAX_LBA_EXT));
			else
				ata_devices[index].sectors = *((uint64_t *)
					(ata_buffer + ATA_IDENTIFY_MAX_LBA));

			/* get name */
			for (int k = 0; k < 40; k ++) {
				ata_devices[index].name[k] = (char)
					ata_buffer[ATA_IDENTIFY_MODEL + k];
			}

			/* clean up the name */
			for (int k = 39; k > 0; k--) {
				if (ata_devices[index].name[k] != ' ') {
					ata_devices[index].name[k + 1] = '\0';
					break;
				}
			}

			drives++;
		}
	}

	k_ok("Loaded ATA driver, found %d drives", drives);
}