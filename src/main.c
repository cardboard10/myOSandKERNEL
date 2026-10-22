#include <efi.h>
//#include <efiprot.h>

#include <myheaders.h>

static EFI_GUID gEfiGraphicsOutputProtocolGuid = {
    0x9042a9de, 0x23dc, 0x4a38, {0x96, 0xfb, 0x7a, 0xde, 0xd0, 0x80, 0x51, 0x6a}
};

static EFI_GUID gEfiAcpi20TableGuid = {
    0x8868e871, 0xe4f1, 0x11d3, {0xbc, 0x22, 0x00, 0x80, 0xc7, 0x3c, 0x88, 0x81}
};

EFI_STATUS EFIAPI efi_main(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE* SystemTable) {
    EFI_BOOT_SERVICES* BS = SystemTable->BootServices;
    EFI_STATUS status;

    // ---------------------------------------------------------
    // 1. GRAB GOP (Graphics Output Protocol)
    // ---------------------------------------------------------
    EFI_GRAPHICS_OUTPUT_PROTOCOL* GOP = NULL;
    status = BS->LocateProtocol(&gEfiGraphicsOutputProtocolGuid, NULL, (void**)&GOP);
    if (EFI_ERROR(status)) {
        return status;
    }

    // Grab frame buffer address, size, and screen dimensions
    
    ScreenInfo screen_info;
    screen_info.framebuffer_base = (void*)GOP->Mode->FrameBufferBase;
    screen_info.framebuffer_size = GOP->Mode->FrameBufferSize;
    screen_info.screen_width = GOP->Mode->Info->HorizontalResolution;
    screen_info.screen_height = GOP->Mode->Info->VerticalResolution;
    screen_info.pixels_per_scanline = GOP->Mode->Info->PixelsPerScanLine;

    // ---------------------------------------------------------
    // 2. GRAB THE MEMORY MAP (Double-Call Pattern)
    // ---------------------------------------------------------
    UINTN memory_map_size = 0;
    EFI_MEMORY_DESCRIPTOR* memory_map = NULL;
    UINTN map_key = 0;
    UINTN descriptor_size = 0;
    UINT32 descriptor_version = 0;

    // First call: This will return EFI_BUFFER_TOO_SMALL (which is technically an "error", 
    // so we don't check EFI_ERROR here; we just use it to get the required size).
    BS->GetMemoryMap(&memory_map_size, memory_map, &map_key, &descriptor_size, &descriptor_version);

    // Add padding space for safety during allocation
    memory_map_size += 2 * descriptor_size;

    // Allocate pool memory from UEFI
    status = BS->AllocatePool(EfiLoaderData, memory_map_size, (void**)&memory_map);
    if (EFI_ERROR(status)) {
        return status;
    }

    // Second call: Actually fetch the memory map into our allocated buffer
    status = BS->GetMemoryMap(&memory_map_size, memory_map, &map_key, &descriptor_size, &descriptor_version);
    if (EFI_ERROR(status)) {
        return status;
    }

    // ---------------------------------------------------------
    // 3. GRAB THE ACPI RSDP TABLE
    // ---------------------------------------------------------
    void* rsdp = NULL;
    for (UINTN i = 0; i < SystemTable->NumberOfTableEntries; i++) {
        EFI_CONFIGURATION_TABLE* table = &SystemTable->ConfigurationTable[i];

        // Compare vendor GUID with ACPI 2.0 GUID
        // Note: Depending on your headers, you may use memcmp or a built-in comparison helper
        if (table->VendorGuid.Data1 == gEfiAcpi20TableGuid.Data1) { // Quick check or use memcmp
            rsdp = table->VendorTable;
            break;
        }
    }

    // ---------------------------------------------------------
    // 4. EXIT BOOT SERVICES
    // ---------------------------------------------------------
    // Must be called immediately after getting the final map_key
    status = BS->ExitBootServices(ImageHandle, map_key);
    if (EFI_ERROR(status)) {
        // Real kernels loop this part, but for now we return the error code
        return status;
    }

    // =========================================================
    // YOU ARE ON BARE METAL NOW. UEFI IS GONE.
    // =========================================================
    uint32_t color = 0x00FF0000;
    for (int x = 0; x < screen_info.screen_width; x++) {
        for (int y = 0; y < screen_info.screen_height; y++) {
            set_pixel(&screen_info, x, y, color);
        }
    }
    while (1) {
        Key key;
        key = get_key();
        if (key.asci == 'r') {
            uint32_t color = 0x00FF0000;
            for (int x = 0; x < screen_info.screen_width; x++) {
                for (int y = 0; y < screen_info.screen_height; y++) {
                    set_pixel(&screen_info, x, y, color);
                }
            }
        }
        else if (key.asci == 'g') {
            uint32_t color = 0x0000FF00;
            for (int x = 0; x < screen_info.screen_width; x++) {
                for (int y = 0; y < screen_info.screen_height; y++) {
                    set_pixel(&screen_info, x, y, color);
                }
            }
        }
        else if (key.asci == 'b') {
            uint32_t color = 0x000000FF;
            for (int x = 0; x < screen_info.screen_width; x++) {
                for (int y = 0; y < screen_info.screen_height; y++) {
                    set_pixel(&screen_info, x, y, color);
                }
            }
        }
        else if (key.asci == '\n') {
            draw_text(&screen_info,"ABC", 3, 3, 3);
        }
    }
}