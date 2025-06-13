#include <Library/IoLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/DebugLib.h>
#include <Library/TimerLib.h>

#include <Protocol/MT6357Pmic.h>
#include <Protocol/MtkPmicWrapper.h>

#include "MT6357PmicDxe.h"

MTK_PWRAP *PmicWrapper;

VOID
EFIAPI
PowerButtonDebounce (
  OUT BOOLEAN *isPressed
  )
{
  UINT16 DebReg;
  PmicWrapper->Read16 (MT6357_TOPSTATUS, &DebReg);
  DebReg &= MT6357_PWRKEY_DEB;

  *isPressed = !DebReg;
}

VOID
EFIAPI
HomeButtonDebounce (
  OUT BOOLEAN     *isPressed
)
{
  UINT16 DebReg;
  PmicWrapper->Read16 (MT6357_TOPSTATUS, &DebReg);
  DebReg &= MT6357_HOMEKEY_DEB;

  *isPressed = !DebReg;
}

VOID
EFIAPI
RtcGetTime (
  OUT EFI_TIME *Time
  )
{
  UINT16 Second, Minute, Hour, Day, Month, Year;

  PmicWrapper->Read16 (MT6357_RTC_TC_SEC, &Second);
  PmicWrapper->Read16 (MT6357_RTC_TC_MIN, &Minute);
  PmicWrapper->Read16 (MT6357_RTC_TC_HOU, &Hour);
  PmicWrapper->Read16 (MT6357_RTC_TC_DOM, &Day);
  PmicWrapper->Read16 (MT6357_RTC_TC_MTH, &Month);
  PmicWrapper->Read16 (MT6357_RTC_TC_YEA, &Year);

  Time->Second = Second;
  Time->Minute = Minute;
  Time->Hour = Hour;
  Time->Day = Day;
  Time->Month = Month;
  Time->Year = 2000 + Year;
}

MT6357_PMIC MtkPmic = {
  PowerButtonDebounce,
  HomeButtonDebounce,
  RtcGetTime,
};

EFI_STATUS
EFIAPI
MT6357PmicInitialize (
  IN EFI_HANDLE ImageHandle,
  IN EFI_SYSTEM_TABLE *SystemTable
  )
{
  EFI_STATUS Status;

  Status = gBS->LocateProtocol (&gMediaTekPmicWrapperProtocolGuid, NULL, (VOID **)&PmicWrapper);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "MT6357PmicDxe: failed to get pmic wrapper protocol, Status = %r!\n", Status));
    return Status;
  }

  Status = gBS->InstallMultipleProtocolInterfaces (
    &ImageHandle,
    &gMediaTekMT6357PmicProtocolGuid,
    &MtkPmic,
    NULL);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "MT6357PmicDxe: Failed to install protocol, Status = %r\n", Status));
    return Status;
  }

  return Status;
}
