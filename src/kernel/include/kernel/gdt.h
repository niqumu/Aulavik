#ifndef AULAVIK_GDT_H
#define AULAVIK_GDT_H

#include <stdint.h>

// segment descriptor (table entry) flags
#define SEG_DATA_RD             0x00    // read-only
#define SEG_DATA_RDA            0x01    // read-only, accessed
#define SEG_DATA_RDWR           0x02    // read/write
#define SEG_DATA_RDWRA          0x03    // read/write, accessed
#define SEG_DATA_RDEXPD         0x04    // read-only, expand down
#define SEG_DATA_RDEXPDA        0x05    // read-only, expand down, accessed
#define SEG_DATA_RDWREXPD       0x06    // read/write, expand down
#define SEG_DATA_RDWREXPDA      0x07    // read/write, expand down, accessed
#define SEG_CODE_EX             0x08    // execute-only
#define SEG_CODE_EXA            0x09    // execute-only, accessed
#define SEG_CODE_EXRD           0x0A    // execute/read
#define SEG_CODE_EXRDA          0x0B    // execute/read, accessed
#define SEG_CODE_EXC            0x0C    // execute-only, conforming
#define SEG_CODE_EXCA           0x0D    // execute-only, conforming, accessed
#define SEG_CODE_EXRDC          0x0E    // execute/read, conforming
#define SEG_CODE_EXRDCA         0x0F    // execute/read, conforming, accessed

#ifdef __cplusplus
extern "C" {
#endif

/*
 * struct representation of a 48 bit GDT descriptor.
 */
struct __attribute__((packed)) gdtr {
    uint16_t size;         // the size of the table, in bytes, minus one
    uint32_t offset;       // the linear (not physical) address of the gdt
};

/*
 * struct representation of a single entry in the GDT.
 */
struct __attribute__((packed, aligned(4))) gdt_entry {
    uint16_t base_low;
    uint16_t limit_low;
    uint8_t base_mid;
    uint8_t access;
    uint8_t limit_high_and_flags;
    uint8_t base_high;
};

uint64_t create_entry(uint32_t base, uint32_t limit, uint16_t flag);
void set_up_gdt(void);

#ifdef __cplusplus
}
#endif

#endif //AULAVIK_GDT_H
