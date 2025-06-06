#include <Uefi/UefiBaseType.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/DebugLib.h>

#include "LzmaDecompress.h"
#include "7zip/C/LzmaDec.h"

VOID *
SzAlloc (
  CONST ISzAlloc  *This,
  SizeT            Size
  )
{
  DEBUG ((DEBUG_INFO, "LzmaDecompress: Allocating pool with size %d \n", (UINTN)Size));
  return AllocatePool ((UINTN)Size);
}

VOID
SzFree (
  CONST ISzAlloc  *This,
  VOID            *Address
  )
{
  if (!Address)
    return;
  DEBUG ((DEBUG_INFO, "LzmaDecompress: Freeing pool at address: 0x%p \n", Address));
  FreePool (Address);
}

ISzAlloc AllocFuncs = {
  SzAlloc,
  SzFree
};

EFI_STATUS
EFIAPI
LzmaDecompress (
  VOID  *SourcePtr,
  VOID  *DecompressedPtr,
  UINTN  SourceSize,
  UINTN  DecompressedSize
)
{
  CLzmaDec    State;
  ELzmaStatus Status;
  SRes        Res;

  LzmaDec_Construct (&State);

  Res = LzmaDec_Allocate(&State, SourcePtr, LZMA_PROPS_SIZE, &AllocFuncs);
  if (Res != SZ_OK) {
    DEBUG ((DEBUG_ERROR, "LzmaDecompress: Failed init LZMA decoder!\n"));
    return EFI_DEVICE_ERROR;
  }

  LzmaDec_Init (&State);

  Res = LzmaDec_DecodeToBuf (
    &State,
    DecompressedPtr,
    (SizeT*)&DecompressedSize,
    SourcePtr + LZMA_PROPS_SIZE + 0x8,
    (SizeT*)&SourceSize,
    LZMA_FINISH_END,
    &Status
  );

  if (Res != SZ_OK) {
    DEBUG ((DEBUG_ERROR, "LzmaDecompress: LZMA decompression failed!\n"));
    return EFI_DEVICE_ERROR;
  }

  if (Status != LZMA_STATUS_FINISHED_WITH_MARK && 
      Status != LZMA_STATUS_MAYBE_FINISHED_WITHOUT_MARK) {
    DEBUG ((DEBUG_ERROR, "LzmaDecompress: Decompression finished with warnings \n"));
  }

  LzmaDec_Free(&State, &AllocFuncs);

  return EFI_SUCCESS;
}
