
extern uint8_t getkeydata();
extern uint8_t getkeystatus();


Key get_key() {
	Key key;
	uint8_t status = getkeystatus();
	if (status & 0x01) {
		key.scancode = getkeydata();
		key.asci = scancode_to_ascii[key.scancode];
		return key;
	}
	else {
		key.scancode = 0;
		key.asci = 0;
		return key;
	}
}
// 002,004,008,016,032,064,128,256
// 001,002,004,008,016,032,064,128

void set_pixel(ScreenInfo *screen_info,int x, int y, uint32_t color) {
	uintptr_t pixel_address;
	pixel_address = (uintptr_t)screen_info->framebuffer_base + (y * screen_info->pixels_per_scanline + x) * 4;
	uint32_t* pixel_byte = (uint32_t*)pixel_address;
	*pixel_byte = color;
}
uint32_t get_pixel(ScreenInfo* screen_info, int x, int y) {
	uintptr_t pixel_address;
	pixel_address = (uintptr_t)screen_info->framebuffer_base + (y * screen_info->pixels_per_scanline + x) * 4;
	uint32_t* pixel_byte = (uint32_t*)pixel_address;
	uint32_t color;
	color = *pixel_byte;
	return color;
}



void draw_chr(ScreenInfo* screen_info, char chr, int gx, int gy) {
	uint8_t bmap[8];
	bmap[0] = font8x8[chr][0];
	bmap[1] = font8x8[chr][1];
	bmap[2] = font8x8[chr][2];
	bmap[3] = font8x8[chr][3];
	bmap[4] = font8x8[chr][4];
	bmap[5] = font8x8[chr][5];
	bmap[6] = font8x8[chr][6];
	bmap[7] = font8x8[chr][7];
	uint32_t red = 0x00FF0000;
	uint32_t blue = 0x000000FF;
	for (int x = 0;x < 8;x++) {
		for (int y = 0;y < 8;y++) {
			set_pixel(screen_info, (gx*8) + x, (gy * 8) + y, 0x00FF0000);
		}
	}
	for (int x = 0;x < 8;x++) {
		for (int y = 0;y < 8;y++) {
			if ((bmap[y] >> (7 - x)) & 1) {
				set_pixel(screen_info, (gx * 8) + x, (gy * 8) + y, 0x00000000FF);
			}
		}
	}
}

void draw_text(ScreenInfo* screen_info, char txt[], int txt_len, int gx, int gy) {
	for (int i = 0;i < txt_len;i++) {
		draw_chr(screen_info, txt[i], gx + i, gy);
	}
}