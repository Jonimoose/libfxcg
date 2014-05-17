#include <fxcg/display.h>

void VRAM_XORSprite(const color_t* data, int x, int y, int width, int height) {
    color_t* VRAM = (color_t*)0xA8000000;
    VRAM += (LCD_WIDTH_PX * y) + x;

    for(int j = y; j < y + height; j++) {
        for(int i = x; i < x + width; i++) {
            *VRAM++ ^= *data++;
        }
        VRAM += LCD_WIDTH_PX - width;
    }
}

void VRAM_CopySprite(const color_t* data, int x, int y, int width, int height) {
    color_t *VRAM = (color_t*)0xA8000000;
    VRAM += (LCD_WIDTH_PX * y) + x;

    for(int j = y; j < y + height; j++) {
        for (int i = x; i < x + width; i++) {
            *VRAM++ = *data++;
        }
        VRAM += LCD_WIDTH_PX - width;
    }
}
