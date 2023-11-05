/*====------------------ kernel.h - Main kernel entry --------------------====*\
 *
 * This code is a part of the Aulavik project.
 * Usage of these works is permitted provided that this instrument is retained
 * with the works, so that any entity that uses the works is notified of this
 * instrument. These works are provided without any warranty.
 *
\*====--------------------------------------------------------------------====*/

#include <kernel/kernel.h>

#include <kernel/driver/ps2.h>
#include <kernel/driver/tty.h> /* terminal_init(); */
#include <kernel/driver/serial.h>
#include <kernel/driver/graphics.h>

#include <kernel/logger.h>
#include <kernel/memory_manager.h>
#include <kernel/idt/idt.h> /* idt_init() */
#include <kernel/gdt/gdt.h> /* gdt_init(); */

multiboot_info_t *mb_info;
mb_memory_block_t *mb_memory_map;
uint32_t mb_memory_map_size;

multiboot_info_t* kernel_get_mb_info()
{
	return mb_info;
}

mb_memory_block_t* kernel_get_mb_memmap()
{
	return mb_memory_map;
}

uint32_t kernel_get_mb_memmap_size()
{
	return mb_memory_map_size;
}

__attribute__((unused))
void kernel_premain(multiboot_info_t *_mb_info, uint32_t magic)
{
	mb_info = _mb_info;
	mb_memory_map = (mb_memory_block_t *) mb_info->mmap_addr;
	mb_memory_map_size = mb_info->mmap_length / sizeof(mb_memory_block_t);

	if (magic != MULTIBOOT_BOOTLOADER_MAGIC) {
		panic("Bootloader gave bad magic bytes!");
	}

	if (!(mb_info->flags & MULTIBOOT_INFO_MEM_MAP)) {
		panic("Bootloader didn't give a memory map!");
	}

	memory_manager_init();
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
	k_ok("Loaded serial driver");
	ps2_init();
	k_ok("Loaded PS/2 driver");

	k_print("\nKernel ready!");
	k_print("Memory: %dkb lower, %dkb upper\n", mb_info->mem_lower,
		mb_info->mem_upper);

	while (1) {
		asm("hlt");
	}
}