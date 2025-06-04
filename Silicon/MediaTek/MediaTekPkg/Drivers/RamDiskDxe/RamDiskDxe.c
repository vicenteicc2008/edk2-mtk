#include <Library/UefiBootServicesTableLib.h>
#include <Library/DebugLib.h>
#include <Library/MemoryMapHelperLib.h>
#include <Library/DevicePathLib.h>
#include <Library/PlatformMemoryMapLib.h>
#include <Protocol/RamDisk.h>

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

  DEBUG ((DEBUG_INFO, "RamDiskDxe: RamDiskAddress: 0x%0x\n", MemoryDescriptor.Address));
  DEBUG ((DEBUG_INFO, "RamDiskDxe: RamDiskSize: 0x%0x\n", MemoryDescriptor.Length));

  DestinationRamdiskPtr = (void *)(UINTN)MemoryDescriptor.Address;
  RamDiskSize = MemoryDescriptor.Length;

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
