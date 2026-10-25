typedef unsigned char      uint8_t;
typedef unsigned short     uint16_t;
typedef unsigned int       uint32_t;
typedef unsigned long long uint64_t;
typedef unsigned long long uintptr_t;


typedef struct {
    void* framebuffer_base;
    UINTN framebuffer_size;
    UINT32 screen_width;
    UINT32 screen_height;
    UINT32 pixels_per_scanline;
}ScreenInfo;
typedef struct {
    UINTN memory_map_size;
    EFI_MEMORY_DESCRIPTOR* memory_map;
    UINTN map_key;
    UINTN descriptor_size;
    UINT32 descriptor_version;
}MEM_MAP_BOX;
typedef struct {
    MEM_MAP_BOX mem_map;
    void* rsdp;
    ScreenInfo* screen_info;
}efiInfoBox;

typedef struct {
    char asci;
    uint8_t scancode;
}Key;