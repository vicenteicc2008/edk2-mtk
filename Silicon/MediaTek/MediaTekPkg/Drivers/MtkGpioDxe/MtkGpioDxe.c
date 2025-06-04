#include <Library/IoLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/MtkGpioImplLib.h>
#include <Library/DebugLib.h>

#include <Protocol/MtkGpio.h>

#define PIN_SG_OFFSET(p) ((p/32)*0x10) /* Offset for Set|Get values */

// Internal functions
VOID
GpioWrite (
  IN UINT32    Reg,
  IN UINT32    Value
  )
{
  UINT32 GpioAddr = PlatformInfo.BaseAddr + Reg;

  MmioWrite32(GpioAddr, Value);
}

VOID
GpioRead (
  IN  UINT32    Reg,
  OUT UINT32   *Value
  )
{
  UINT32 GpioAddr = PlatformInfo.BaseAddr + Reg;

  *Value = MmioRead32(GpioAddr);
}

VOID
GetMode (
  IN  UINT32          Pin,
  OUT MTK_GPIO_MODE  *Mode
  )
{
  UINT32 Reg, PinMask, DirValue;

  PinMask = 1U << (Pin % 32);
  Reg = PlatformInfo.DirOffset + PIN_SG_OFFSET(Pin);

  GpioRead(Reg, &DirValue);

  if (DirValue & PinMask)
  {
    *Mode = GPIO_MODE_OUTPUT;
  }
  else
  {
    *Mode = GPIO_MODE_INPUT;
  }
}

VOID
Get (
  IN  UINT32         Pin,
  OUT BOOLEAN       *Value
  )
{
  UINT32 PinMask, Reg, BankValue, Dir, Mode;

  Dir = PlatformInfo.DataOutOffset;

  GetMode(Pin, &Mode);

  if (Mode == GPIO_MODE_INPUT)
  {
    Dir = PlatformInfo.DataInOffset;
  }

  PinMask = 1U << (Pin % 32);
  Reg = Dir + PIN_SG_OFFSET(Pin);
  
  GpioRead(Reg, &BankValue);

  if (BankValue & PinMask)
  {
    *Value = 0;
  }
  else
  {
    *Value = 1;
  }
}

VOID
Set (
  IN UINT32         Pin,
  IN MTK_GPIO_MODE  Mode
  )
{
  if (Mode < 8)
  {
    UINT32 Reg, ModeBit, DataSet, DataReset;

    Reg = PlatformInfo.ModeOffset + ((Pin / 8) * 0x010);
    ModeBit = Pin > 8 ? (Pin % 8) * 4 : Pin * 4;

    GpioRead(Reg + PlatformInfo.SetOffset, &DataSet);
    GpioRead(Reg + PlatformInfo.ResetOffset, &DataReset);

    for (int i = 0; i < 3; i++)
    {
      if (Mode & (1 << i))
      {
        DataSet |= (1U << (ModeBit + i));
      }
      else
      {
        DataReset |= (1U << (ModeBit + i));
      }
    }

    GpioWrite(Reg + PlatformInfo.SetOffset, DataSet);
    GpioWrite(Reg + PlatformInfo.ResetOffset, DataReset);
  }
  else
  {
    UINT32 PinMask, Reg, BankValue;

    PinMask = 1U << (Pin % 32);
    Reg = PlatformInfo.DirOffset + PIN_SG_OFFSET(Pin);

    if (Mode == GPIO_MODE_INPUT)
    {
      Reg += PlatformInfo.ResetOffset;
    }
    if (Mode == GPIO_MODE_OUTPUT)
    {
      Reg += PlatformInfo.SetOffset;
    }

    GpioRead(Reg, &BankValue);

    BankValue |= PinMask;

    GpioWrite(Reg, BankValue);
  }
}

VOID
SetPull (
  IN UINT32         Pin,
  IN MTK_GPIO_PULL  State
  )
{
  UINT32 PinMask, Reg, BankValue;

  PinMask = 1U << (Pin % 32);
  Reg = PlatformInfo.DataOutOffset + PIN_SG_OFFSET(Pin);

  if (State == GPIO_PULL_DOWN)
  {
    Reg += PlatformInfo.ResetOffset;
  }
  if (State == GPIO_PULL_UP)
  {
    Reg += PlatformInfo.SetOffset;
  }

  GpioRead(Reg, &BankValue);

  BankValue |= PinMask;

  GpioWrite(Reg, BankValue);
}


MTK_GPIO MtkGpio = {
  Get,
  Set,
  GetMode,
  SetPull,
};

EFI_STATUS
EFIAPI
MtkGpioInitialize(
  IN EFI_HANDLE ImageHandle, IN EFI_SYSTEM_TABLE *SystemTable)
{
  EFI_STATUS Status;

  Status = gBS->InstallMultipleProtocolInterfaces(
    &ImageHandle,
    &gMtkGpioProtocolGuid,
    &MtkGpio,
    NULL);

  return Status;
}
