
#include <efi.h>

EFI_INPUT_KEY getKey(EFI_SYSTEM_TABLE* SystemTable) {
    EFI_INPUT_KEY Key;

    Key.ScanCode = 0;
    Key.UnicodeChar = 0;

    while (SystemTable->ConIn->ReadKeyStroke(SystemTable->ConIn, &Key) != 0) {

    }
    return Key;
}

__attribute__((ms_abi)) 
EFI_STATUS efi_main(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE* SystemTable) {
    
    
    
    SystemTable->ConOut->OutputString(SystemTable->ConOut, L"Hello from bare-metal UEFI!\r\n");
    
    while (1) {
        EFI_INPUT_KEY Key;
        Key = getKey(SystemTable);
        CHAR16 UserInput[2] = { Key.UnicodeChar, 0 };
        if (Key.UnicodeChar == L'\r') {
            SystemTable->ConOut->OutputString(SystemTable->ConOut, L"\r\n");
        }
        else {
            SystemTable->ConOut->OutputString(SystemTable->ConOut, UserInput);
        }
    }

    return 0;
}