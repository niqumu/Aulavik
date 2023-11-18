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
#include <kernel/driver/fat32.h>
#include <kernel/driver/keyboard.h>
#include <kernel/driver/mouse.h>
#include <kernel/driver/ps2.h>
#include <kernel/driver/pci.h>
#include <kernel/driver/serial.h>

#include <kernel/bin/shell.h>
#include <kernel/logger.h>
#include <kernel/interrupts/idt.h> /* idt_init() */
#include <kernel/gdt/gdt.h> /* gdt_init(); */
#include <kernel/graphics/graphics.h>
#include <kernel/memory/heap.h>
#include <kernel/memory/paging.h>
#include <kernel/terminal.h>
#include <string.h>
#include <stdlib.h>
#include <aulavik/syscall.h>
#include <stdarg.h>
#include <stdio.h>
#include "kernel/task/multitasking.h"
#include "kernel/rainier/rainier.h"
#include "kernel/loader/loader.h"

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

//	paging_init(); // work in progress, crashes instantly still
	heap_init();
}

int test_putchar(int i)
{
	char c = (char) i;
	syscall_write(0, &c, 1);
//	terminal_putc((char) i);
	return i;
}


int test_vprintf(const char* restrict format, va_list parameters)
{
	char *str;
	memset(str, 0, 256); // todo this NEEDS to be malloc
	int ret = vsprintf(str, format, parameters);

	while (*str)
		test_putchar(*str++);

	return ret;
}

int test_printf(const char* restrict format, ...)
{
	va_list args;
	va_start(args, format);
	int i = test_vprintf(format, args);
	va_end(args);
	return i;
}

__attribute__((unused, __noreturn__))
void kernel_main(void)
{
	graphics_init();
	terminal_init(graphics_get_global_rctx());
	gdt_init();
	idt_init();
	k_ok("Loading drivers...");
	serial_init(BAUD_38400);
	ps2_init();
	keyboard_init();
//	mouse_init();
	pci_init();
	ata_init();
	fat32_init();
	multitasking_init();

	k_print("\nKernel ready");
	k_print("Memory: %dkb lower, %dkb upper\n", mb_info->mem_lower,
		mb_info->mem_upper);

//	k_print("Entering Rainier...");
//	terminal_exit();
//	rainier_main();
	terminal_clear();

//	test_printf("Hello world!");

	k_debug("Searching for executable in root directory");

	for (uint32_t i = 0; i < fat32_get_root()->entry_count; i++) {

		struct fat32_directory_entry entry =fat32_get_root()->entries[i];

		if (strcmp(entry.extension, "elf") == 0) {
			k_ok("Found executable: %s", entry.display_name);
			loader_load(entry);
		}
	}

//	shell_main();

	while (true)
		asm("hlt");
}
