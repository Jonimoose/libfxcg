#include <sprite.h>

void VRAM_XORSprite(const color_t* data, int x, int y, int width, int height) {
   color_t* VRAM = (color_t*)0xA8000000;
   VRAM += (LCD_WIDTH_PX*y + x);
   short i, j;
   for(j=y; j<y+height; j++) {
      for(i=x; i<x+width;  i++) {
         *(VRAM++) ^= *(data++);
         //*(VRAM++) ^= *(data++);
      }
      VRAM += (LCD_WIDTH_PX-width);
   }
}

void VRAM_CopySprite(const color_t* data, int x, int y, int width, int height) {
        color_t* VRAM = (color_t*)0xA8000000;
        VRAM += (LCD_WIDTH_PX*y + x);
        short j;
        for(j=y; j<y+height; j++) {
                memcpy(VRAM,data,2*width);
                VRAM +=LCD_WIDTH_PX;
                data += width;
        }
}
