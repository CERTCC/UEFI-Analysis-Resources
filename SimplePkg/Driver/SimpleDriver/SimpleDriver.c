#include <Uefi.h>
#include <Library/UefiLib.h>
#include <Library/UefiDriverEntryPoint.h>
#include <Library/UefiBootServicesTableLib.h>

#include <Protocol/SimpleProtocol.h>

// Protocol Function instances
EFI_STATUS EFIAPI Function1(
  IN EFI_SimpleProtocol_PROTOCOL             *This
);

EFI_STATUS EFIAPI Function2(
  IN EFI_SimpleProtocol_PROTOCOL             *This,
  IN UINTN                          arg
);

// Protocol instance
EFI_SimpleProtocol_PROTOCOL gSimpleProtocolInstance = {
    Function1,
    Function2
};

/**
  This is the declaration of an EFI image entry point. This entry point
  Is the same for UEFI Applications, UEFI OS Loaders, and UEFI Drivers including both device drivers and bus drivers.
  @param  ImageHandle  The firmware allocated handle for the UEFI image. @param SystemTable A pointer to the EFI System Table.
  @retval  EFI_SUCCESS  The operation completed successfully. @retval Others An unexpected error occurred.
**/
EFI_STATUS
EFIAPI
SimpleDriverEntryPoint (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  //gBS is a global variable from <Library/UefiBootServicesTableLib.h>
  return gBS->InstallMultipleProtocolInterfaces (
                &ImageHandle,
                &gEfiSimpleProtocolGuid,
                &gSimpleProtocolInstance,
                NULL
                );
}


EFI_STATUS EFIAPI Function1(
  IN EFI_SimpleProtocol_PROTOCOL             *This
){
    Print(L"Function1 called\n");
    return EFI_SUCCESS;
}

EFI_STATUS EFIAPI Function2(
  IN EFI_SimpleProtocol_PROTOCOL             *This,
  IN UINTN                          arg
){
    Print(L"Function2 called with %d", arg);
    return EFI_SUCCESS;
}