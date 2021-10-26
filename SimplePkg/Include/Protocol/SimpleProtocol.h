/*
* Copied from https://tianocore-docs.github.io/edk2-UefiDriverWritersGuide/draft/appendix_a_edk_ii_file_templates/a5_edk_ii_package_extension_templates/a51_protocol_file_template.html#a51-protocol-file-template
*/

#ifndef __SimpleProtocol_H__
#define __SimpleProtocol_H__

#define EFI_SimpleProtocol_PROTOCOL_GUID \
  { \
    0xfd0a26e9, 0x2e22, 0x4e3c, { 0x95, 0x0c, 0x1b, 0xdc, 0x2f, 0x4d, 0xc7, 0xc1 } \
  }

///
/// Forward declaration
///
typedef struct _EFI_SimpleProtocol_PROTOCOL EFI_SimpleProtocol_PROTOCOL;

///
/// Function prototypes
///
typedef
EFI_STATUS
(EFIAPI *EFI_SimpleProtocol_FUNC1)(
  IN EFI_SimpleProtocol_PROTOCOL             *This
  );

typedef
EFI_STATUS
(EFIAPI *EFI_SimpleProtocol_FUNC2)(
  IN EFI_SimpleProtocol_PROTOCOL    *This,
  IN UINTN                          arg
  );


///
/// Protocol structure
///
struct _EFI_SimpleProtocol_PROTOCOL{
  EFI_SimpleProtocol_FUNC1  Func1;
  EFI_SimpleProtocol_FUNC2  Func2;
};

extern EFI_GUID gEfiSimpleProtocolGuid;

#endif