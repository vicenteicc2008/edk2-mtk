#include "RamDiskDxe.h"
#include "LzmaDecompress.h"

EFI_STATUS
DecompressRamdisk (
  VOID    *SourcePtr,
  VOID   **DecompressedPtr,
  UINTN    CompressedSize,
  UINTN    DecompressedSize
  )
{
  EFI_STATUS  Status;

  *DecompressedPtr = AllocateAlignedPages (
    (DecompressedSize + SIZE_4KB - 1) / SIZE_4KB,
    SIZE_4KB
  );
  if (*DecompressedPtr == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }
  ZeroMem (*DecompressedPtr, (DecompressedSize + SIZE_4KB - 1));

  Status = LzmaDecompress (
     SourcePtr,
    *DecompressedPtr,
     CompressedSize,
     DecompressedSize
  );

  if (EFI_ERROR (Status)) {
    FreePool(*DecompressedPtr);
    *DecompressedPtr = NULL;
    return Status;
  }

  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
RamDiskDxeInitialize (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  EFI_STATUS Status;
  VOID *DestinationRamdiskPtr;
  UINTN RamDiskSize;

  EFI_GUID *RamDiskRegisterType = &gEfiVirtualDiskGuid;
  EFI_RAM_DISK_PROTOCOL *RamdiskProtocol;
  EFI_DEVICE_PATH_PROTOCOL *DevicePath;
  ARM_MEMORY_REGION_DESCRIPTOR_EX MemoryDescriptor;

  Status = LocateMemoryMapAreaByName ("RamDisk", &MemoryDescriptor);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "RamDiskDxe: Couldn't find the RamDisk Memory Area - %r\n", Status));
    return Status;
  };

  DEBUG ((DEBUG_INFO, "RamDiskDxe: RamDiskAddress: 0x%08x\n", MemoryDescriptor.Address));

  UINT32 Magic = MmioRead32 (MemoryDescriptor.Address);

  if (Magic == 0x8a7a6a5a) {
    VOID *DecompressedPtr;
    RamDiskSize = MmioRead32 (MemoryDescriptor.Address + 0x4);
    UINT32 CompressedSize = MmioRead32 (MemoryDescriptor.Address + 0x8);
    DEBUG ((DEBUG_INFO, "RamDiskDxe: Compressed Ramdisk Detected!! \n"));
    DEBUG ((DEBUG_INFO, "RamDiskDxe: Ramdisk Size: 0x%08x \n", RamDiskSize));
    DEBUG ((DEBUG_INFO, "RamDiskDxe: Compressed Size: 0x%08x \n", CompressedSize));
    Status = DecompressRamdisk (
      (void *)(UINTN)(MemoryDescriptor.Address + 0xc),
      &DecompressedPtr,
      CompressedSize,
      RamDiskSize
    );
    if (EFI_ERROR (Status)) {
      DEBUG((EFI_D_ERROR, "RamDiskDxe: LZMA decompression failed: %r\n", Status));
      return Status;
    }
    DEBUG ((DEBUG_INFO, "RamDiskDxe: Decompression done, ptr: 0x%08x \n", DecompressedPtr));
    DestinationRamdiskPtr = (void *)(UINTN)DecompressedPtr;
  } else {
    DEBUG ((DEBUG_INFO, "RamDiskDxe: Plain Ramdisk Detected\n"));
    DEBUG ((DEBUG_INFO, "RamDiskDxe: RamDisk Size: 0x%08x\n", MemoryDescriptor.Length));
    DestinationRamdiskPtr = (void *)(UINTN)MemoryDescriptor.Address;
    RamDiskSize = MemoryDescriptor.Length;
  }

  Status = gBS->LocateProtocol(&gEfiRamDiskProtocolGuid, NULL, (VOID **)&RamdiskProtocol);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "RamDiskDxe: Couldn't find the RamDisk protocol - %r\n", Status));
    return Status;
  }

  Status = RamdiskProtocol->Register((UINTN)DestinationRamdiskPtr, (UINT64)RamDiskSize, RamDiskRegisterType, NULL, &DevicePath);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "RamDiskDxe: Cannot register RAM Disk - %r\n", Status));
    return Status;
  }

  return Status;
}
