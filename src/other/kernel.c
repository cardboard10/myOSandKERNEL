#include <efi.h>
#include <efiprot.h>

static EFI_GUID gEfiGraphicsOutputProtocolGuid = {
    0x9042a9de, 0x23dc, 0x4a38, {0x96, 0xfb, 0x7a, 0xde, 0xd0, 0x80, 0x51, 0x6a}
};
static EFI_GUID gEfiAcpi20TableGuid = {
    0x8868e871, 0xe4f1, 0x11d3, {0xbc, 0x22, 0x00, 0x80, 0xc7, 0x3c, 0x88, 0x81}
};


EFI_STATUS EFIAPI efi_main(EFI_HANDLE ImageHandle,EFI_SYSTEM_TABLE *SystemTable) {
	EFI_STATUS status;
	
	//grab GOP(graphics output protocol
	EFI_GRAPHICS_OUTPUT_PROTOCOL *GOP = NULL;
	status = EFI_BOOT_SERVICES->LocateProtocol(&gEfiGraphicsOutputProtocolGuid, NULL, (void**)&GOP);
	if (EFI_ERROR(status)){
		return status;
	}
	
	//grab frame buffer address and size, and the screen dimensions
	void* framebuffer_base = (void*)GOP->Mode->FrameBufferBase;
	UINTN framebuffer_size = GOP->Mode->FrameBufferSize;
	UINT32 screen_width = GOP->Mode->Info->HorzontalResolution;
	UINT32 screen_height = GOP->Mode->Info->VerticalResolution;
	
	//define the variables to hold the memory map
	UINTN memory_map_size = 0;
	EFI_MEMORY_DESCRIPTOR *memory_map = NULL;
	UINTN map_key = 0;
	UINTN descriptor_size = 0;
	UINT32 descriptor_version = 0;
	
	//grab the memory map
	status=EFI_BOOT_SERVICES->GetMemoryMap(&memory_map_size, memory_map, &map_key, &descriptor_size, &descriptor_version);
	if (EFI_ERROR(status)) {
		return status;
	}
	
	//grab acpi rsdp table?
	void* rsdp = NULL;
	for (UINTN i = 0; i < SystemTable->NumberOfTableEntries; i++) {
		EFI_CONFIGURATION_TABLE *table = &SystemTable->ConfigurationTable[i];
		//check if this table matches the ACPI 2.0 GUID
		if (CompareGuid(&table->VendorGuid, &gEfiAcpi20TableGuid) == 0) {
			rsdp = table->VendorTable;
			break;
		}
	}
	
	//exit boot services
	status = EFI_BOOT_SERVICES->ExitBootServices(ImageHandle, map_key);
	if (EFI_ERROR(status)) {
		return status;
	}
	
	//your on your own now
	while(1){}
}