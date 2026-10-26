void MAIN(ScreenInfo screen_info, efiInfoBox info) {
    uint32_t color = 0x00FF0000;
    for (int x = 0; x < screen_info.screen_width; x++) {
        for (int y = 0; y < screen_info.screen_height; y++) {
            set_pixel(&screen_info, x, y, color);
        }
    }
    void fillgreen(ScreenInfo screen_info) {
        uint32_t color = 0x0000FF00;
        for (int x = 0; x < screen_info.screen_width; x++) {
            for (int y = 0; y < screen_info.screen_height; y++) {
                set_pixel(&screen_info, x, y, color);
            }
        }
    }
    void fillred(ScreenInfo screen_info) {
        uint32_t color = 0x00FF0000;
        for (int x = 0; x < screen_info.screen_width; x++) {
            for (int y = 0; y < screen_info.screen_height; y++) {
                set_pixel(&screen_info, x, y, color);
            }
        }
    }
    void fillblue(ScreenInfo screen_info) {
        uint32_t color = 0x000000FF;
        for (int x = 0; x < screen_info.screen_width; x++) {
            for (int y = 0; y < screen_info.screen_height; y++) {
                set_pixel(&screen_info, x, y, color);
            }
        }
    }
    while (1) {
        Key key;
        key = get_key();
        if (key.asci == 'R') {
            fillred(screen_info);
        }
        else if (key.asci == 'G') {
            fillgreen(screen_info);
        }
        else if (key.asci == 'B') {
            fillblue(screen_info);
        }
        else if (key.asci == '\n') {
            break;
        }
    }
    int screen_chr_width = screen_info.screen_width / 9;
    int screen_chr_height = screen_info.screen_height / 9;
    int maxlen = screen_chr_width;
    fillgreen(screen_info);
    uint16_t txt[256];
    read_disk(0, txt);
    //for (int i = 0;i < 256;i++) {
    //    txt[i] = 0;
    //}
    int txtlen = 0;
    for (int i = 0;i < 256;i++) {
        if (txt[i] == 0) {
            txtlen = i;
            break;
        }
    }
    draw_text(&screen_info, txt, maxlen, 0, screen_chr_width / 2);
    while (true) {
        Key key = get_key();
        if (key.asci == '\b') {
            if (txtlen>0) {
                txtlen -= 1;
                txt[txtlen] = ' ';
            }
        }
        else if (key.asci == '\n') {
            break;
        }
        else if (key.asci != 0) {
            txt[txtlen] = key.asci;
            txtlen += 1;
        }
        if(key.asci != 0){
            draw_text(&screen_info, txt, maxlen, 0, screen_chr_width/2);
        }
        //draw_text(&screen_info, txt, txtlen+1, 3, 3);
    }
    write_disk(0, txt);
    fillblue(screen_info);
}