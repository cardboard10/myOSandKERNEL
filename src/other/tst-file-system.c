#include <efi.h>

EFI_INPUT_KEY getKey(EFI_SYSTEM_TABLE* SystemTable) {
    EFI_INPUT_KEY Key;

    Key.ScanCode = 0;
    Key.UnicodeChar = 0;

    while (SystemTable->ConIn->ReadKeyStroke(SystemTable->ConIn, &Key) != 0) {

    }
    return Key;
}
void getLine(EFI_SYSTEM_TABLE* SystemTable, CHAR16* line, int len) {
    int i;
    int lastindex = len - 3;
    for (i = 0; i < lastindex; i++) {
        line[i] = L' ';
    }
    i = 0;
    while (i < lastindex) {
        EFI_INPUT_KEY Key;
        Key = getKey(SystemTable);
        if (Key.UnicodeChar == L'\r') {
            break;
        }
        line[len - 3] = L'\r';
        line[len - 2] = L'\r';
        line[len - 1] = 0;
        if (Key.UnicodeChar == L'\b') {
            line[i-1] = L' ';
            i -= 1;
            SystemTable->ConOut->OutputString(SystemTable->ConOut, L"\r");
            SystemTable->ConOut->OutputString(SystemTable->ConOut, line);
            continue;
        }
        line[i] = Key.UnicodeChar;
        SystemTable->ConOut->OutputString(SystemTable->ConOut, L"\r");
        SystemTable->ConOut->OutputString(SystemTable->ConOut, line);
        
        line[len - 2] = L'\n';
        i++;
    }
}
EFI_STATUS EFIAPI efi_main(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE* SystemTable) {
    // 1. Print using the native ConOut interface (no efitext/efilib required)
    SystemTable->ConOut->OutputString(SystemTable->ConOut, L"Starting raw file I/O...\r\n");

    EFI_STATUS Status;
    EFI_SIMPLE_FILE_SYSTEM_PROTOCOL* FileSys;
    EFI_FILE_PROTOCOL* Root;
    EFI_FILE_PROTOCOL* FileHandle;

    EFI_GUID FileSysGuid = EFI_SIMPLE_FILE_SYSTEM_PROTOCOL_GUID;

    // 2. Locate the Simple File System Protocol via Boot Services
    Status = SystemTable->BootServices->LocateProtocol(&FileSysGuid, NULL, (VOID**)&FileSys);
    if (EFI_ERROR(Status)) {
        SystemTable->ConOut->OutputString(SystemTable->ConOut, L"Failed to locate File System Protocol!\r\n");
        return Status;
    }

    // 3. Open the Volume directly through the protocol function pointer
    Status = FileSys->OpenVolume(FileSys, &Root);
        if (EFI_ERROR(Status)) {
            SystemTable->ConOut->OutputString(SystemTable->ConOut, L"Failed to open root volume!\r\n");
            return Status;
        }

    // 4. Open the target file for reading
    UINT64 OpenMode = EFI_FILE_MODE_WRITE | EFI_FILE_MODE_READ;
    Status = Root->Open(Root, &FileHandle, L"test.txt", OpenMode, 0);
    if (EFI_ERROR(Status)) {
        SystemTable->ConOut->OutputString(SystemTable->ConOut, L"Failed to open 'test.txt'!\r\n");
        Root->Close(Root);
        return Status;
    }
           

    SystemTable->ConOut->OutputString(SystemTable->ConOut, L"Successfully opened 'test.txt' raw!\r\n");


    // LD. grab the file size
    // allocate the memory needed to grab file info
    EFI_FILE_INFO* FileInfo = NULL;
    UINTN BufferSize = sizeof(EFI_FILE_INFO) + 256;
    Status = SystemTable->BootServices->AllocatePool(
        EfiLoaderData,      // mem type
        BufferSize,         // alloc size
        (VOID**)&FileInfo  // pointer to recieve address
    );
    if (EFI_ERROR(Status)) {
        SystemTable->ConOut->OutputString(SystemTable->ConOut, L"Failed to allocate memory to store the file info\r\n");
        Root->Close(Root);
        FileHandle->Close(FileHandle);
        return Status;
    }
    // grab info
    EFI_GUID FileInfoID = EFI_FILE_INFO_ID;
    Status = FileHandle->GetInfo(FileHandle, &FileInfoID, &BufferSize, FileInfo);
    if (EFI_ERROR(Status)) {
        SystemTable->ConOut->OutputString(SystemTable->ConOut, L"Failed to grab file info\r\n");
        Root->Close(Root);
        FileHandle->Close(FileHandle);
        SystemTable->BootServices->FreePool(FileInfo);
        return Status;
    }
    UINT64 FileSize = FileInfo->FileSize;
    FileSize+=2;
    SystemTable->BootServices->FreePool(FileInfo);
    // LD. read and print file contents to console
    CHAR16 FileContents[FileSize];
    FileHandle->Read(FileHandle, &FileSize, FileContents);
    SystemTable->ConOut->OutputString(SystemTable->ConOut, L"file contents:\"\"\"\r\n");
    SystemTable->ConOut->OutputString(SystemTable->ConOut, FileContents);
    SystemTable->ConOut->OutputString(SystemTable->ConOut, L"\"\"\"\r\n");
    // LD. getting some user input on what the file should contain
    CHAR16 line[20];
    getLine(SystemTable, line, 20);
    SystemTable->ConOut->OutputString(SystemTable->ConOut, L"\r\n\r\n");
    SystemTable->ConOut->OutputString(SystemTable->ConOut, line);
    // LD. writting and flushing data to the file
    //CHAR16 str[] = L"HEWO WORLD!!!";
    UINTN BSIZE = sizeof(line) -2;
    FileHandle->Write(FileHandle, &BSIZE, line);
    FileHandle->Flush(FileHandle);
    // 5. Clean up and close handles
    FileHandle->Close(FileHandle);
    Root->Close(Root);

    return EFI_SUCCESS;
}