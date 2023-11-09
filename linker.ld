ENTRY(_start)

SECTIONS
{
	/* I'll be honest, I have zero idea what I'm doing here, but mint says
	     to use 2mb over 1mb, and I trust them, so... */
        . = 2M;

        .text BLOCK(4K) : ALIGN(4K)
        {
                *(.multiboot)
                *(.text)
        }

        .rodata BLOCK(4K) : ALIGN(4K)
        {
                *(.rodata)
        }

        .data BLOCK(4K) : ALIGN(4K)
        {
                *(.data)
        }

        .bss BLOCK(4K) : ALIGN(4K)
        {
                *(COMMON)
                *(.bss)
        }
}
