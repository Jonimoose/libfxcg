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

#include "graphicsProvider.hpp"

const short empty[18] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
int PrintMiniFix( int x, int y, const char*Msg, const int flags, const short color, const short bcolor )
{
  int i = 0, dx;
  unsigned short width;
  void*p;

  while ( Msg[ i ] )
  {
    if( Msg[i] == 9) {
      drawLine(x, y+24+7, x+12, y+24+7, color);
      drawLine(x, y+24+8, x+12, y+24+8, color);
      x+=12;
      i++;
      continue;
    } else if( Msg[i] == 31) {
      // small dot for multiply
      int tx = x+2, ty=y;
      PrintMini(&tx, &ty, (unsigned char*)"\xe6\xaa", 0, 0xFFFFFFFF, 0, 0, color, bcolor, 1, 0);
      x+=12;
      i++;
      continue;
    } else p = GetMiniGlyphPtr( Msg[ i ], &width );
    dx = ( 12 - width ) / 2;
    if ( dx > 0 )
    {
      PrintMiniGlyph( x, y, (void*)empty, flags, dx, 0, 0, 0, 0, color, bcolor, 0 );
    }
   
    else dx = 0;
    PrintMiniGlyph( x + dx, y, p, flags, width, 0, 0, 0, 0, color, bcolor, 0 );
    if ( width + dx < 12 )
    {
      PrintMiniGlyph( x + width + dx, y, (void*)empty, flags, 12 - width - dx, 0, 0, 0, 0, color, bcolor, 0 );
    }
    x += 12;
    i++;
  }
  return x;
}

//draws a point of color color at (x0, y0) 
void plot(int x0, int y0, int color) { 
   char* VRAM = (char*)0xA8000000; 
   VRAM += 2*(y0*LCD_WIDTH_PX + x0); 
   *(VRAM++) = (color&0x0000FF00)>>8; 
   *(VRAM++) = (color&0x000000FF); 
   return; 
}

void drawRectangle(int x, int y, int width, int height, unsigned short color) {
  unsigned short*VRAM = (unsigned short*)0xA8000000;
  for(int j = y; j < y+height; j++) {
    for(int i = x; i < x+width; i++) {
      *(j*LCD_WIDTH_PX+i+VRAM) = color;      
    }
  }
}
//Uses the Bresenham line algorithm 
void drawLine(int x1, int y1, int x2, int y2, int color) { 
    signed char ix; 
    signed char iy; 
  
    // if x1 == x2 or y1 == y2, then it does not matter what we set here 
    int delta_x = (x2 > x1?(ix = 1, x2 - x1):(ix = -1, x1 - x2)) << 1; 
    int delta_y = (y2 > y1?(iy = 1, y2 - y1):(iy = -1, y1 - y2)) << 1; 
  
   plot(x1, y1, color);  
    if (delta_x >= delta_y) { 
        int error = delta_y - (delta_x >> 1);        // error may go below zero 
        while (x1 != x2) { 
            if (error >= 0) { 
                if (error || (ix > 0)) { 
                    y1 += iy; 
                    error -= delta_x; 
                }                           // else do nothing 
         }                              // else do nothing 
            x1 += ix; 
            error += delta_y; 
            plot(x1, y1, color); 
        } 
    } else { 
        int error = delta_x - (delta_y >> 1);      // error may go below zero 
        while (y1 != y2) { 
            if (error >= 0) { 
                if (error || (iy > 0)) { 
                    x1 += ix; 
                    error -= delta_y; 
                }                           // else do nothing 
            }                              // else do nothing 
            y1 += iy; 
            error += delta_x;  
            plot(x1, y1, color); 
        } 
    } 
}
//ReplaceColor By Kerm:
void VRAMReplaceColorInRect(int x, int y, int width, int height, color_t color_old, color_t color_new) { 
   //color_t* VRAM = GetVRAMAddress();
   color_t* VRAM = (color_t*)0xA8000000; 
   VRAM += (y*LCD_WIDTH_PX)+x; 
   for(int j=0; j<height; VRAM += (LCD_WIDTH_PX-width), j++) { 
      for(int i=0; i<width; VRAM++, i++) { 
         if (*VRAM == color_old) *VRAM = color_new; 
      } 
   } 
} 

void darkenStatusbar() {  
  VRAMReplaceColorInRect(0, 0, LCD_WIDTH_PX, 24, COLOR_BLACK, COLOR_CYAN);
  VRAMReplaceColorInRect(0, 0, LCD_WIDTH_PX, 24, COLOR_WHITE, COLOR_BLACK);
  VRAMReplaceColorInRect(0, 0, LCD_WIDTH_PX, 24, COLOR_CYAN, COLOR_GRAY);
  VRAMReplaceColorInRect(0, 0, LCD_WIDTH_PX, 24, COLOR_BLUE, COLOR_ORANGE);
}
void darkenFkeys(int numkeys) {
  VRAMReplaceColorInRect(0, LCD_HEIGHT_PX-24, LCD_WIDTH_PX-64*(6-numkeys), 24, COLOR_BLACK, COLOR_CYAN);
  VRAMReplaceColorInRect(0, LCD_HEIGHT_PX-24, LCD_WIDTH_PX-64*(6-numkeys), 24, COLOR_WHITE, COLOR_BLACK);
  VRAMReplaceColorInRect(0, LCD_HEIGHT_PX-24, LCD_WIDTH_PX-64*(6-numkeys), 24, COLOR_CYAN, COLOR_GRAY);  
}
void drawArrowDown(int bottomX, int bottomY, int color) {
  drawLine(bottomX-7,bottomY-7,bottomX,bottomY,color);
  drawLine(bottomX-8,bottomY-7,bottomX,bottomY+1,color); //double thickness of line
  drawLine(bottomX+1,bottomY,bottomX+8,bottomY-7,color);
  drawLine(bottomX+1,bottomY+1,bottomX+9,bottomY-7,color); //double thickness of line
}
void drawFkeyPopup(int Fkey, int darktheme, int showclosemessage) {
//draws a big popup pointing to a Fkey (index zero based, F1 = 0, F6 = 5) with the selected color scheme.
//showclosemessage - select to show a minimini message on the right left saying "...or press: [EXIT]"
// PrintXY text inside the popup starts at X=2 and Y=2
  int fgcolor = COLOR_BLACK;
  int bgcolor = COLOR_WHITE;
  if (darktheme) {
    fgcolor = COLOR_WHITE;
    bgcolor = COLOR_BLACK;
  }
  int c1x = 18; int c1y = 24*2;
  int c2x = 18*20; int c2y = 24*2;
  int c3x = 18*20; int c3y = 24*8-12;
  int c4x = 18; int c4y = 24*8-12;
  drawRectangle(c1x, c1y, c3x-c1x, c3y-c1x-24, bgcolor);
  drawLine(c1x-1, c1y-1, c2x, c2y-1, fgcolor);
  drawLine(c1x-1, c1y-1, c4x-1, c4y+1, fgcolor);
  drawLine(c2x, c2y-1, c3x, c3y+1, fgcolor);
  drawLine(c4x-1, c4y+1, c3x, c3y+1, fgcolor);

  drawLine(c1x-2, c1y-2, c2x+1, c2y-2, fgcolor);
  drawLine(c1x-2, c1y-2, c4x-2, c4y+2, fgcolor);
  drawLine(c2x+1, c2y-2, c3x+1, c3y+2, fgcolor);
  drawLine(c4x-2, c4y+2, c3x+1, c3y+2, fgcolor);

  drawLine(c1x, c1y+23, c2x-1, c2y+23, COLOR_GRAY);
  drawRectangle(c1x, c1y, c2x-c1x, 23, COLOR_LIGHTGRAY);

  drawArrowDown(31+Fkey*64, 190, fgcolor);
  drawLine(23+Fkey*64, 181, 40+Fkey*64, 181, bgcolor);
  drawLine(24+Fkey*64, 182, 39+Fkey*64, 182, bgcolor);


  if (showclosemessage) {
    int textX = 0;
    int textY = c3y-14-20;
    PrintMiniMini( &textX, &textY, (unsigned char*)"...or press: [EXIT]", 0, TEXT_COLOR_BLACK, 1 ); //fake draw
    textX = c3x-textX-4;
    PrintMiniMini( &textX, &textY, (unsigned char*)"...or press: [EXIT]", 0, TEXT_COLOR_BLACK, 0 ); //draw
  }
}
void CopySprite(const void* datar, int x, int y, int width, int height) { 
   color_t*data = (color_t*) datar; 
   color_t* VRAM = (color_t*)0xA8000000; 
   VRAM += LCD_WIDTH_PX*y + x; 
   for(int j=y; j<y+height; j++) { 
      for(int i=x; i<x+width; i++) { 
         *(VRAM++) = *(data++); 
     } 
     VRAM += LCD_WIDTH_PX-width; 
   } 
} 
void CopySpriteMasked(const unsigned char* data, int x, int y, int width, int height, int maskcolor) { 
   char* VRAM = (char*)0xA8000000; 
   VRAM += 2*(LCD_WIDTH_PX*y + x); 
   for(int j=y; j<y+height; j++) { 
      for(int i=x; i<x+width;  i++) { 
         if ((((((int)(*data))&0x000000FF)<<8) | ((((int)(*(data+1))))&0x000000FF)) != maskcolor) { 
            *(VRAM++) = *(data++); 
            *(VRAM++) = *(data++); 
         } else { VRAM += 2; data += 2; } 
      } 
      VRAM += 2*(LCD_WIDTH_PX-width); 
   } 
}
void CopySpriteNbit(const unsigned char* data, int x, int y, int width, int height, const color_t* palette, unsigned int bitwidth) {
   color_t* VRAM = (color_t*)0xA8000000;
   VRAM += (LCD_WIDTH_PX*y + x);
   int offset = 0;
   unsigned char buf = 0;
   for(int j=y; j<y+height; j++) {
      int availbits = 0;
      for(int i=x; i<x+width;  i++) {
         if (!availbits) {
            buf = data[offset++];
            availbits = 8;
         }
         color_t thisthis = ((color_t)buf>>(8-bitwidth));
         *VRAM = palette[(color_t)thisthis];
         VRAM++;
         buf<<=bitwidth;
         availbits-=bitwidth;
      }
      VRAM += (LCD_WIDTH_PX-width);
   }
} 
int drawRGB24toRGB565(int r, int g, int b)  
{  
  return ((r / 8) << 11) | ((g / 4) << 5) | (b / 8);  
}
//the following does not update the screen automatically; it will draw the tny.im logo starting at screen coordinates x,y
//the tny.im logo is great enough not to require any sprites! yay!
//w:138
//h:42
int alphaBlend(int newcc, int oldcc, float alpha) {
  return alpha*newcc+(1-alpha)*oldcc;
}
/* commented because isn't needed
void alphaRGB(int r1, int g1, int b1, int r2, int g2, int b2, int* nr, int* ng, int* nb, float alpha) {
  *nr = alphaBlend(r1, r2, alpha);
  *ng = alphaBlend(g1, g2, alpha);
  *nb = alphaBlend(b1, b2, alpha);
}*/
void drawtnyimLogo( int x, int y, float alpha) {
  //draw t
  int black = drawRGB24toRGB565(alphaBlend(255, 0, alpha), alphaBlend(255, 0, alpha), alphaBlend(255, 0, alpha));
  drawRectangle(x, y+6, 6, 24, black);
  drawRectangle(x+6, y+12, 6, 6, black);
  drawRectangle(x+6, y+30, 6, 6, black);
  //draw n
  drawRectangle(x+18, y+12, 6, 24, black);
  drawRectangle(x+24, y+12, 12, 6, black);
  drawRectangle(x+36, y+18, 6, 18, black);
  //draw y
  drawRectangle(x+48, y+12, 6, 18, black);
  drawRectangle(x+60, y+12, 6, 18, black);
  drawRectangle(x+54, y+30, 6, 6, black);
  drawRectangle(x+48, y+36, 6, 6, black);
  //draw dot
  drawRectangle(x+72, y+30, 6, 6, black);
  int orange = drawRGB24toRGB565(alphaBlend(255, 210, alpha), alphaBlend(255, 68, alpha), alphaBlend(255, 19, alpha));
  //draw i (orange)
  drawRectangle(x+84, y, 6, 6, orange);
  drawRectangle(x+84, y+12, 6, 24, orange);
  //draw m (orange)
  drawRectangle(x+96, y+12, 6, 24, orange);
  drawRectangle(x+102, y+12, 12, 6, orange);
  drawRectangle(x+114, y+18, 6, 18, orange);
  drawRectangle(x+120, y+12, 12, 6, orange);
  drawRectangle(x+132, y+18, 6, 18, orange);
}

int textColorToFullColor(int textcolor) {
  switch(textcolor) {
    case TEXT_COLOR_BLACK: return COLOR_BLACK;
    case TEXT_COLOR_BLUE: return COLOR_BLUE;
    case TEXT_COLOR_GREEN: return COLOR_GREEN;
    case TEXT_COLOR_CYAN: return COLOR_CYAN;
    case TEXT_COLOR_RED: return COLOR_RED;
    case TEXT_COLOR_PURPLE: return COLOR_PURPLE;
    case TEXT_COLOR_YELLOW: return COLOR_YELLOW;
    case TEXT_COLOR_WHITE: return COLOR_LIGHTGRAY;
    default: return COLOR_BLACK;
  }
}

void progressMessage(char* message, int cur, int total) {
  char buffer[30] = "";
  char buffer2[5] = "";
  strcpy(buffer, "  ");
  strcat(buffer, message);
  strcat(buffer, " (");
  itoa(cur, (unsigned char*)buffer2);
  strcat(buffer, buffer2);
  strcat(buffer, "/");
  itoa(total, (unsigned char*)buffer2);
  strcat(buffer, buffer2);
  strcat(buffer, ")");
  PrintXY(1,8,(char*)"                        ", TEXT_MODE_NORMAL, TEXT_COLOR_BLACK);
  PrintXY(1,8,(char*)buffer, TEXT_MODE_NORMAL, TEXT_COLOR_BLACK);
  Bdisp_PutDisp_DD();
}

void printCentered(unsigned char* text, int y, int FGC, int BGC) {
  int len = strlen((char*)text);
  int pixlen = len*18;
  int x = LCD_WIDTH_PX/2-pixlen/2;
  int cur = 0;
  while(cur<len) {
    PrintCXY(x, y, &text[cur], 0x40, -1, FGC, BGC, 1, 0 );
    x=x+18;
    cur++;
  }
}