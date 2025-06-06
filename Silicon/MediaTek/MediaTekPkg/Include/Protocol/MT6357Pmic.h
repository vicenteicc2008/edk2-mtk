#ifndef __MT6357_PMIC_H__
#define __MT6357_PMIC_H__
//
// Protocol interface structure
//

typedef struct _MT6357_PMIC MT6357_PMIC;

//
// Function Prototypes
//

typedef
VOID
(EFIAPI *MT6357_KEY_DEB)(
  OUT BOOLEAN     *isPressed
  );

typedef
VOID
(EFIAPI *MT6357_RTC_GET_TIME)(
  OUT EFI_TIME *Time
  );

struct _MT6357_PMIC {
  MT6357_KEY_DEB        PowerButtonDebounce;
  MT6357_KEY_DEB        HomeButtonDebounce;
  MT6357_RTC_GET_TIME   RtcGetTime;
};

extern EFI_GUID gMediaTekMT6357PmicProtocolGuid;

#endif // __MT6357_PMIC_H__
