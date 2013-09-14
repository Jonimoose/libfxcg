#ifndef __GRAPHICSPROVIDER_H
#define __GRAPHICSPROVIDER_H
#include <fxcg/display.h>
#include <fxcg/file.h>
#include <fxcg/keyboard.h>
#include <fxcg/system.h>
#include <fxcg/misc.h>
#include <fxcg/app.h>
#include <fxcg/serial.h>
#include <fxcg/rtc.h>
#include <fxcg/heap.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int PrintMiniFix( int x, int y, const char*Msg, const int flags, const short color, const short bcolor ); 
void plot(int x0, int y0, int color);
void drawRectangle(int x, int y, int width, int height, unsigned short color);
void drawLine(int x1, int y1, int x2, int y2, int color);
void VRAMReplaceColorInRect(int x, int y, int width, int height, color_t color_old, color_t color_new);
void darkenStatusbar();
void darkenFkeys(int numkeys);
void drawArrowDown(int bottomX, int bottomY, int color);
void drawFkeyPopup(int Fkey, int darktheme, int showclosemessage);
void CopySprite(const void* datar, int x, int y, int width, int height);
void CopySpriteMasked(const unsigned char* data, int x, int y, int width, int height, int maskcolor);
void CopySpriteNbit(const unsigned char* data, int x, int y, int width, int height, const color_t* palette, unsigned int bitwidth);
int drawRGB24toRGB565(int r, int g, int b);
int alphaBlend(int newcc, int oldcc, float alpha);
void drawtnyimLogo( int x, int y, float alpha);
int textColorToFullColor(int textcolor);
void progressMessage(char* message, int cur, int total);
void printCentered(unsigned char* text, int y, int FGC, int BGC);

#endif