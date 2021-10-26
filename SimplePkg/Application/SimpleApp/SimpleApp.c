#include <Uefi.h>
#include <Library/UefiLib.h>
#include <Library/UefiApplicationEntryPoint.h>
#include <Library/UefiBootServicesTableLib.h>

#include <Protocol/SimpleProtocol.h>

EFI_STATUS
EFIAPI
SimpleAppEntryPoint (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
    EFI_SimpleProtocol_PROTOCOL *simpleProtocol;
    EFI_STATUS status;

    CHAR16 hello_str[] = L"Hello, from SimpleApp\n";
    Print(hello_str);


    status = gBS->LocateProtocol(&gEfiSimpleProtocolGuid, NULL, (VOID **) &simpleProtocol);
    if (EFI_ERROR (status)) {
      Print(L"Unable to open SimpleProtocol\n");
      return EFI_NOT_FOUND;
    }


    simpleProtocol->Func1(simpleProtocol);

    return EFI_SUCCESS;
}