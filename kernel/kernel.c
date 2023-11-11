/*====------------------ kernel.h - Main kernel entry --------------------====*\
 *
 * This code is a part of the Aulavik project.
 * Usage of these works is permitted provided that this instrument is retained
 * with the works, so that any entity that uses the works is notified of this
 * instrument. These works are provided without any warranty.
 *
\*====--------------------------------------------------------------------====*/

#include <kernel/kernel.h>

#include <kernel/driver/ata.h>
#include <kernel/driver/keyboard.h>
#include <kernel/driver/ps2.h>
#include <kernel/driver/pci.h>
#include <kernel/driver/serial.h>

#include <aulavik/syscall.h>

#include <kernel/bin/shell.h>
#include <kernel/logger.h>
#include <kernel/interrupts/idt.h> /* idt_init() */
#include <kernel/gdt/gdt.h> /* gdt_init(); */
#include <kernel/graphics/graphics.h>
#include <kernel/memory/heap.h>
#include <kernel/memory/paging.h>
#include <kernel/terminal.h>
#include "kernel/interrupts/pic.h"

multiboot_info_t *mb_info;
mb_memory_block_t *mb_memory_map;
uint32_t mb_memory_map_size;

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

//	paging_init();
	heap_init();
}

__attribute__((unused, __noreturn__))
void kernel_main(void)
{
	graphics_init();
	terminal_init(graphics_get_context());
	gdt_init();
	idt_init();
	k_ok("Loading drivers...");
	serial_init(BAUD_38400);
	ps2_init();
	keyboard_init();
	pci_init();
//	ata_init();

	k_print("\nKernel ready");
	k_print("Memory: %dkb lower, %dkb upper\n", mb_info->mem_lower,
		mb_info->mem_upper);

	char message[] = "hello world!";
	syscall_write(0, message, sizeof(message));

	shell_main();

	while (true)
		asm("hlt");
}
