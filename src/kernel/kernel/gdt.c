#include <kernel/gdt.h>

uint64_t create_entry(uint32_t base, uint32_t limit, uint16_t flag)
{
        uint64_t descriptor;

        // Create high 32-bit segment
        descriptor  =  limit        & 0x000f0000;
        descriptor |= (flag <<  8)  & 0x00f0ff00;
        descriptor |= (base >> 16)  & 0x000000ff;
        descriptor |=  base         & 0xff000000;
        descriptor <<= 32;

        // Create the low 32-bit segment
        descriptor |= base  << 16;
        descriptor |= limit  & 0x0000ffff;

        return descriptor;
}

void set_up_gdt(void)
{

}