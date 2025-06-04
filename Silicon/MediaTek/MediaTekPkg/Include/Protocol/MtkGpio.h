#ifndef __MTK_GPIO_H__
#define __MTK_GPIO_H__
//
// Protocol interface structure
//
typedef struct _MTK_GPIO MTK_GPIO;

//
// Data Types
//
typedef enum {
  GPIO_MODE_INPUT              = 0,
  GPIO_MODE_OUTPUT             = 8,
  GPIO_MODE_SPECIAL_FUNCTION_1 = 1,
  GPIO_MODE_SPECIAL_FUNCTION_2 = 2,
  GPIO_MODE_SPECIAL_FUNCTION_3 = 3,
  GPIO_MODE_SPECIAL_FUNCTION_4 = 4,
  GPIO_MODE_SPECIAL_FUNCTION_5 = 5,
  GPIO_MODE_SPECIAL_FUNCTION_6 = 6,
  GPIO_MODE_SPECIAL_FUNCTION_7 = 7
} MTK_GPIO_MODE;

typedef struct {
  UINT32 BaseAddr;
  UINT32 SetOffset;
  UINT32 ResetOffset;
  UINT32 DirOffset;
  UINT32 DataOutOffset;
  UINT32 DataInOffset;
  UINT32 ModeOffset;
} MTK_GPIO_PLATFORM_INFO;

typedef enum {
  GPIO_PULL_NONE,
  GPIO_PULL_UP,
  GPIO_PULL_DOWN
} MTK_GPIO_PULL;

//
// Function Prototypes
//
typedef
VOID
(EFIAPI *MTK_GPIO_GET)(
  IN  UINT32         Pin,
  OUT BOOLEAN       *Value
  );

/*++

Routine Description:

  Gets the state of a GPIO pin

Arguments:

  Pin   - which pin to read
  Value - state of the pin

--*/

typedef
VOID
(EFIAPI *MTK_GPIO_SET)(
  IN UINT32         Pin,
  IN MTK_GPIO_MODE Mode
  );

/*++

Routine Description:

  Sets the state of a GPIO pin

Arguments:

  Pin   - which pin to modify
  Mode  - mode to set

--*/

typedef
VOID
(EFIAPI *MTK_GPIO_GET_MODE)(
  IN  UINT32            Pin,
  OUT MTK_GPIO_MODE    *Mode
  );

/*++

Routine Description:

  Gets the mode (function) of a GPIO pin

Arguments:

  Pin   - which pin
  Mode  - pointer to output mode value

--*/

typedef
VOID
(EFIAPI *MTK_GPIO_SET_PULL)(
  IN  UINT32         Pin,
  IN  MTK_GPIO_PULL Direction
  );

/*++

Routine Description:

  Sets the pull-up / pull-down resistor of a GPIO pin

Arguments:

  Pin   - which pin
  Direction - pull-up, pull-down, or none

--*/

struct _MTK_GPIO {
  MTK_GPIO_GET              Get;
  MTK_GPIO_SET              Set;
  MTK_GPIO_GET_MODE         GetMode;
  MTK_GPIO_SET_PULL         SetPull;
};

extern EFI_GUID  gMtkGpioProtocolGuid;

#endif
