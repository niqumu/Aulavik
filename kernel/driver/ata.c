/*====--------------------- ata.c - ATA disk driver ----------------------====*\
 *
 * This code is a part of the Aulavik project.
 * Usage of these works is permitted provided that this instrument is retained
 * with the works, so that any entity that uses the works is notified of this
 * instrument. These works are provided without any warranty.
 *
\*====--------------------------------------------------------------------====*/

#include <kernel/driver/ata.h>

struct ata_device ata_devices[4];

void init_ata(uint32_t primary_addr, uint32_t primary_ctrl_addr,
              uint32_t secondary_addr, uint32_t secondary_ctrl_addr,
              uint32_t bus_master)
{

}