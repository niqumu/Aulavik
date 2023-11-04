/*====------------------ kernel.h - Main kernel entry --------------------====*\
 *
 * This code is a part of the Aulavik project.
 * Usage of these works is permitted provided that this instrument is retained
 * with the works, so that any entity that uses the works is notified of this
 * instrument. These works are provided without any warranty.
 *
\*====--------------------------------------------------------------------====*/

#include <kernel/kernel.h>

#include <kernel/driver/tty.h> /* terminal_init(); */
#include <kernel/driver/serial.h>

#include <kernel/logger.h>
#include <kernel/idt/idt.h> /* idt_init() */
#include <kernel/gdt/gdt.h> /* gdt_init(); */

multiboot_info_t *multiboot_info;
mmap_entry_t *memory_map;
uint32_t memory_map_size;

__attribute__((unused))
void kernel_premain(multiboot_info_t *_multiboot_info, uint32_t magic)
{
	multiboot_info = _multiboot_info;
	memory_map = (mmap_entry_t *) multiboot_info->mmap_addr;
	memory_map_size = multiboot_info->mmap_length / sizeof(mmap_entry_t);

	if (!(multiboot_info->flags & MULTIBOOT_INFO_MEM_MAP)) {
		panic("Bootloader didn't give a memory map!");
	}

	if (magic != MULTIBOOT_BOOTLOADER_MAGIC) {
		panic("Bootloader gave bad magic bytes!");
	}
}

__attribute__((unused))
void kernel_main(void)
{
	terminal_init();
	k_ok("Initialized terminal");
	gdt_init();
	k_ok("Initialized GDT");
	idt_init();
	k_ok("Initialized IDT");
	k_ok("Loading drivers...");
	serial_init(PORT_COM_1, BAUD_38400);
	k_ok("Loaded serial driver!");

	k_print("\nKernel ready!");
	k_print("Memory: %dkb lower, %dkb upper\n", multiboot_info->mem_lower,
		multiboot_info->mem_upper);

	k_print("Memory map:");
	for (uint32_t i = 0; i < memory_map_size; i++) {
		mmap_entry_t mmmt = memory_map[i];

		k_print("address = %x, length = %x, type = %x",
		        mmmt.base_addr_low,
		        mmmt.length_low,
		        mmmt.type);

		if (mmmt.type == MULTIBOOT_MEMORY_AVAILABLE) {
			k_ok("Memory section starting at %x is free!", mmmt.base_addr_low);
		}
	}
}