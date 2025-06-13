#ifndef _LZMA_DECOMPRESS_H_
#define _LZMA_DECOMPRESS_H_

EFI_STATUS
EFIAPI
LzmaDecompress (
  VOID  *SourcePtr,
  VOID  *DecompressedPtr,
  UINTN  SourceSize,
  UINTN  DecompressedSize
);

#endif /* _LZMA_DECOMPRESS_H_ */