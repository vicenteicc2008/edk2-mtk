[Defines]
  PLATFORM_NAME                  = t1
  PLATFORM_GUID                  = 98F65BE4-5A6E-4F0D-94D6-888AF8BE17DD
  PLATFORM_VERSION               = 0.1
  DSC_SPECIFICATION              = 0x00010019
  OUTPUT_DIRECTORY               = Build/$(PLATFORM_NAME)
  SUPPORTED_ARCHITECTURES        = AARCH64
  BUILD_TARGETS                  = DEBUG|RELEASE
  SKUID_IDENTIFIER               = DEFAULT
  FLASH_DEFINITION               = T1Pkg/t1.fdf
  DEVICE_DXE_FV_COMPONENTS       = T1Pkg/t1.fdf.inc

[PcdsFixedAtBuild.common]
  gArmTokenSpaceGuid.PcdSystemMemoryBase|0x40000000
  gArmTokenSpaceGuid.PcdSystemMemorySize|0x100000000 # 4GB
  gArmTokenSpaceGuid.PcdCpuVectorBaseAddress|0x40C40000
  gEmbeddedTokenSpaceGuid.PcdPrePiStackBase|0x40C00000
  gEmbeddedTokenSpaceGuid.PcdPrePiStackSize|0x00040000
  gMediaTekTokenSpaceGuid.PcdUefiMemPoolBase|0x40C50000
  gMediaTekTokenSpaceGuid.PcdUefiMemPoolSize|0x075b0000

  # Simple Framebuffer
  gMediaTekTokenSpaceGuid.PcdMipiFrameBufferWidth|720
  gMediaTekTokenSpaceGuid.PcdMipiFrameBufferHeight|1600
  gMediaTekTokenSpaceGuid.PcdMipiFrameBufferAddress|0x7ec50000

[LibraryClasses]
  PlatformMemoryMapLib|T1Pkg/Library/PlatformMemoryMapLib/PlatformMemoryMapLib.inf

!include Silicon/MediaTek/MT8183Pkg/MT8183.dsc
