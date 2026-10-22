#ifndef EFI_H
#define EFI_H

typedef unsigned short CHAR16;
typedef unsigned long long EFI_STATUS;
typedef void* EFI_HANDLE;
typedef unsigned long long UINT64;
typedef unsigned int UINT32;

typedef struct {
    UINT64 Signature;
    UINT32 Revision;
    UINT32 HeaderSize;
    UINT32 CRC32;
    UINT32 Reserved;
} EFI_TABLE_HEADER;

typedef struct {
    unsigned short ScanCode;
    CHAR16 UnicodeChar;
} EFI_INPUT_KEY;

typedef struct {
    void* Reset;
    EFI_STATUS(__attribute__((ms_abi))* OutputString)(void* This, CHAR16* String);
} EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL;

typedef struct {
    void* Reset;
    EFI_STATUS(__attribute__((ms_abi))* ReadKeyStroke)(void* This, EFI_INPUT_KEY* Key);
    void* WaitForKey;
} EFI_SIMPLE_TEXT_INPUT_PROTOCOL_IN;

typedef struct {
    EFI_TABLE_HEADER Hdr;
    CHAR16* FirmwareVendor;
    UINT32 FirmwareRevision;
    UINT32 _pad; // Ensures 8-byte alignment for pointers on x64
    EFI_HANDLE ConsoleInHandle;
    EFI_SIMPLE_TEXT_INPUT_PROTOCOL_IN* ConIn;
    EFI_HANDLE ConsoleOutHandle;
    EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL* ConOut;
} EFI_SYSTEM_TABLE;

#endif