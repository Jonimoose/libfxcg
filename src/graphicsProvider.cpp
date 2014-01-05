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
      drawLine(x, y+24+7, x+11, y+24+7, color);
      drawLine(x, y+24+8, x+11, y+24+8, color);
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
void plot(int x0, int y0,unsigned short color) {
  unsigned short* VRAM = (unsigned short*)0xA8000000;
  VRAM += (y0*LCD_WIDTH_PX + x0);
  *VRAM=color;
}

void drawRectangle(int x, int y, int width, int height, unsigned short color){
  unsigned short*VRAM = (unsigned short*)0xA8000000;
  VRAM+=(y*384)+x;
  while(height--){
    int i=width;
    while(i--){
      *VRAM++ = color;
    }
    VRAM+=384-width;
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
/*void VRAMReplaceColorInRect(int x, int y, int width, int height, color_t color_old, color_t color_new) { 
   //color_t* VRAM = GetVRAMAddress();
   color_t* VRAM = (color_t*)0xA8000000; 
   VRAM += (y*LCD_WIDTH_PX)+x; 
   for(int j=0; j<height; VRAM += (LCD_WIDTH_PX-width), j++) { 
      for(int i=0; i<width; VRAM++, i++) { 
         if (*VRAM == color_old) *VRAM = color_new; 
      } 
   } 
} */
/*void CopySprite(const void* datar, int x, int y, int width, int height) { 
   color_t*data = (color_t*) datar; 
   color_t* VRAM = (color_t*)0xA8000000; 
   VRAM += LCD_WIDTH_PX*y + x; 
   for(int j=y; j<y+height; j++) { 
      for(int i=x; i<x+width; i++) { 
         *(VRAM++) = *(data++); 
     } 
     VRAM += LCD_WIDTH_PX-width; 
   } 
}*/
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
/*void CopySpriteNbit(const unsigned char* data, int x, int y, int width, int height, const color_t* palette, unsigned int bitwidth) {
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
}*/
//the following does not update the screen automatically; it will draw the tny.im logo starting at screen coordinates x,y
//the tny.im logo is great enough not to require any sprites! yay!
//w:138
//h:42
void drawtnyimLogo( int x, int y) {
  //draw t
  drawRectangle(x, y+6, 6, 24, COLOR_BLACK);
  drawRectangle(x+6, y+12, 6, 6, COLOR_BLACK);
  drawRectangle(x+6, y+30, 6, 6, COLOR_BLACK);
  //draw n
  drawRectangle(x+18, y+12, 6, 24, COLOR_BLACK);
  drawRectangle(x+24, y+12, 12, 6, COLOR_BLACK);
  drawRectangle(x+36, y+18, 6, 18, COLOR_BLACK);
  //draw y
  drawRectangle(x+48, y+12, 6, 18, COLOR_BLACK);
  drawRectangle(x+60, y+12, 6, 18, COLOR_BLACK);
  drawRectangle(x+54, y+30, 6, 6, COLOR_BLACK);
  drawRectangle(x+48, y+36, 6, 6, COLOR_BLACK);
  //draw dot
  drawRectangle(x+72, y+30, 6, 6, COLOR_BLACK);
  //draw i (orange)
  drawRectangle(x+84, y, 6, 6, TNYIM_ORANGE);
  drawRectangle(x+84, y+12, 6, 24, TNYIM_ORANGE);
  //draw m (orange)
  drawRectangle(x+96, y+12, 6, 24, TNYIM_ORANGE);
  drawRectangle(x+102, y+12, 12, 6, TNYIM_ORANGE);
  drawRectangle(x+114, y+18, 6, 18, TNYIM_ORANGE);
  drawRectangle(x+120, y+12, 12, 6, TNYIM_ORANGE);
  drawRectangle(x+132, y+18, 6, 18, TNYIM_ORANGE);
}

/*int textColorToFullColor(int textcolor) {
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
}*/

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