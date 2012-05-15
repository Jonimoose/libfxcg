#ifndef _SPRITE_H
#define _SPRITE_H
 
#include "string.h"
#include "color.h"
 
#define LCD_WIDTH_PX 384
#define LCD_HEIGHT_PX 216
 
void VRAM_XORSprite(const color_t* data, int x, int y, int width, int height);
 
void VRAM_XORSprite_Char(const char* data, int x, int y, int width, int height) { VRAM_XORSprite((const color_t *)data, x, y, width, height); }
 
void VRAM_CopySprite(const color_t* data, int x, int y, int width, int height);
 
void VRAM_CopySprite_Char(const char* data, int x, int y, int width, int height) { VRAM_CopySprite((const color_t *)data, x, y, width, height); }
 
#endif
