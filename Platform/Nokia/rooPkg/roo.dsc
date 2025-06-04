[Defines]
  PLATFORM_NAME                  = roo
  PLATFORM_GUID                  = 54d47bf2-32f2-4532-9438-1b6971dc59c0
  PLATFORM_VERSION               = 0.1
  DSC_SPECIFICATION              = 0x00010019
  OUTPUT_DIRECTORY               = Build/$(PLATFORM_NAME)
  SUPPORTED_ARCHITECTURES        = AARCH64
  BUILD_TARGETS                  = DEBUG|RELEASE
  SKUID_IDENTIFIER               = DEFAULT
  FLASH_DEFINITION               = rooPkg/roo.fdf
  DEVICE_DXE_FV_COMPONENTS       = rooPkg/roo.fdf.inc

[PcdsFixedAtBuild.common]
  # Simple Framebuffer
  gMediaTekTokenSpaceGuid.PcdMipiFrameBufferWidth|736
  gMediaTekTokenSpaceGuid.PcdMipiFrameBufferHeight|1440
  gMediaTekTokenSpaceGuid.PcdMipiFrameBufferAddress|0x7bee0000

[LibraryClasses]
  PlatformMemoryMapLib|rooPkg/Library/PlatformMemoryMapLib/PlatformMemoryMapLib.inf

!include Silicon/MediaTek/MT6765Pkg/MT6765.dsc
