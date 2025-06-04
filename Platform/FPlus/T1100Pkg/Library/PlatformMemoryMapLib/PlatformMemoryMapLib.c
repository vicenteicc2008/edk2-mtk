#include <Library/BaseLib.h>
#include <Library/PlatformMemoryMapLib.h>

static ARM_MEMORY_REGION_DESCRIPTOR_EX gDeviceMemoryDescriptorEx[] = {
/*                                                    EFI_RESOURCE_ EFI_RESOURCE_ATTRIBUTE_ EFI_MEMORY_TYPE ARM_REGION_ATTRIBUTE_
     MemLabel(32 Char.),  MemBase,    MemSize, BuildHob, ResourceType, ResourceAttribute, MemoryType, CacheAttributes
--------------------- Register ---------------------*/
    {"Periphs",           0x00000000, 0x1B000000,  AddMem, MEM_RES, UNCACHEABLE,  RtCode,   NS_DEVICE},

    //--------------------- DDR --------------------- */
    // 0x040000000 - 0x140000000 (0x100000000)
    {"DDR Memory 1",      0x40000000, 0x00C00000, AddMem, SYS_MEM, SYS_MEM_CAP, Conv,   WRITE_BACK_XN},
    {"UEFI Stack",        0x40C00000, 0x00040000, AddMem, SYS_MEM, SYS_MEM_CAP, BsData, WRITE_BACK},
    {"CPU Vectors",       0x40C40000, 0x00010000, AddMem, SYS_MEM, SYS_MEM_CAP, BsCode, WRITE_BACK},
    {"DXE Heap",          0x40C50000, 0x075b0000, AddMem, SYS_MEM, SYS_MEM_CAP, Conv,   WRITE_BACK},
    {"DDR Memory 2",      0x48400000, 0x07c00000, AddMem, SYS_MEM, SYS_MEM_CAP, Conv,   WRITE_BACK},
    {"UEFI FD",           0x50000000, 0x00700000, AddMem, SYS_MEM, SYS_MEM_CAP, BsCode, WRITE_BACK},
    {"DDR Memory 3",      0x50700000, 0x03f00000, AddMem, SYS_MEM, SYS_MEM_CAP, Conv,   WRITE_BACK},
    {"ATF Mem",           0x54600000, 0x00050000, AddMem, MEM_RES, SYS_MEM_CAP, Reserv, WRITE_BACK},
    {"DDR Memory 4",      0x54650000, 0x009b0000, AddMem, SYS_MEM, SYS_MEM_CAP, Conv,   WRITE_BACK},
    {"RamDisk",           0x55000000, 0x01000000, AddMem, SYS_MEM, SYS_MEM_CAP, Conv,   WRITE_BACK},
    {"DDR Memory 5",      0x56000000, 0x1a600000, AddMem, SYS_MEM, SYS_MEM_CAP, Conv,   WRITE_BACK},
    // TEE Memory
    {"DDR Memory 6",      0x73c00000, 0x043f0000, AddMem, SYS_MEM, SYS_MEM_CAP, Conv,   WRITE_BACK},
    {"SPM Mem",           0x77ff0000, 0x00010000, AddMem, MEM_RES, SYS_MEM_CAP, Reserv, WRITE_BACK},
    {"DDR Memory 7",      0x78000000, 0x04b80000, AddMem, SYS_MEM, SYS_MEM_CAP, Conv,   WRITE_BACK},
    // Framebuffer
    // TEE SecMem
    {"DDR Memory 8",      0x7fe00000, 0x00100000, AddMem, SYS_MEM, SYS_MEM_CAP, Conv,   WRITE_BACK},
    {"SSPM Mem",          0x7ff00000, 0x000c0000, AddMem, MEM_RES, SYS_MEM_CAP, Reserv, WRITE_BACK},
    {"DDR Memory 9",      0x7ffc0000, 0x1cf40000, AddMem, SYS_MEM, SYS_MEM_CAP, Conv,   WRITE_BACK},
    {"SCP Mem",           0x9cf00000, 0x00600000, AddMem, MEM_RES, SYS_MEM_CAP, Reserv, WRITE_BACK},
    {"DDR Memory 10",     0x9d500000, 0x000f0000, AddMem, SYS_MEM, SYS_MEM_CAP, Conv,   WRITE_BACK},
    {"VPU Mem",           0x9d5f0000, 0x02a10000, AddMem, MEM_RES, SYS_MEM_CAP, Reserv, WRITE_BACK},
    {"DDR Memory 11",     0xa0000000, 0xa0000000, AddMem, SYS_MEM, SYS_MEM_CAP, Conv,   WRITE_BACK},

    // Distinct MemoryMaps
    // Aurora ATF doesn't have Trustonic TEE, leaved for compatibility
    {"TEE Memory",        0x70000000, 0x03c00000, AddMem, MEM_RES, SYS_MEM_CAP, Reserv, WRITE_BACK},
    {"TrustZone SecMem",  0x7fc00000, 0x00200000, AddMem, MEM_RES, SYS_MEM_CAP, Reserv, WRITE_BACK},
    // Framebuffer, 46MB (3 Layers of 2176x1600*4 4KB aligned)
    // On Android bootloader address starts at 0x7cb80000
    // While on Aurora bootloader it is at     0x7cd80000
    // I added +2MB to the size to be compatible with both options
    // but it still need different to specify exact address for SimpleFbDxe
    {"Framebuffer",       0x7cb80000, 0x03080000, AddMem, MEM_RES, SYS_MEM_CAP, Reserv, WRITE_THROUGH_XN},
//------------------- Terminator for MMU ---------------------
{"Terminator", 0, 0, 0, 0, 0, 0, 0}};

ARM_MEMORY_REGION_DESCRIPTOR_EX *GetPlatformMemoryMap()
{
  return gDeviceMemoryDescriptorEx;
}
