/** @file

Copyright (c) 2017, Intel Corporation. All rights reserved.<BR>
This program and the accompanying materials are licensed and made available under
the terms and conditions of the BSD License that accompanies this distribution.
The full text of the license may be found at
http://opensource.org/licenses/bsd-license.php.

THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#include <Uefi.h>
#include <Library/TestPointCheckLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>

#define TEST_PATTERN  0x5A

VOID
TestPointDumpCpuInfo (
  VOID
  );

EFI_STATUS
TestTempRam (
  IN VOID   *TempRamTestStart,
  IN VOID   *TempRamTestEnd
  )
{
  UINTN    Index;
  BOOLEAN  Result;

  Result = TRUE;

  DEBUG ((DEBUG_INFO, "TempRamTestStart - 0x%08x\n", TempRamTestStart));
  DEBUG ((DEBUG_INFO, "TempRamTestEnd   - 0x%08x\n", TempRamTestEnd));

  SetMem (TempRamTestStart, (UINTN)TempRamTestEnd - (UINTN)TempRamTestStart, TEST_PATTERN);
  for (Index = 0; Index < (UINTN)TempRamTestEnd - (UINTN)TempRamTestStart; Index++) {
    if (*((UINT8 *)TempRamTestStart + Index) != TEST_PATTERN) {
      Result = FALSE;
    }
  }
  if (Result) {
    DEBUG ((DEBUG_INFO, "TempRam Test PASS\n"));
    return EFI_SUCCESS;
  } else {
    DEBUG ((DEBUG_INFO, "TempRam Test FAIL\n"));
    return EFI_DEVICE_ERROR;
  }
}

EFI_STATUS
EFIAPI
TestPointTempMemoryFunction (
  IN VOID   *TempRamStart,
  IN VOID   *TempRamEnd
  )
{
  UINTN  StackMark;
  DEBUG ((DEBUG_INFO, "======== TestPointTempMemoryFunction - Enter\n"));

  DEBUG ((DEBUG_INFO, "TempRamStart - 0x%08x\n", TempRamStart));
  DEBUG ((DEBUG_INFO, "TempRamEnd   - 0x%08x\n", TempRamEnd));

  TestPointDumpCpuInfo ();

  StackMark = 0;
  TestTempRam (TempRamStart, (VOID *)((UINTN)&StackMark & ~0xFFF));

  DEBUG ((DEBUG_INFO, "======== TestPointTempMemoryFunction - Exit\n"));

  return EFI_SUCCESS;
}
