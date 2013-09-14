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

#include "textGUI.hpp"
#include "graphicsProvider.hpp"

/* copy over the next token from an input string, after
skipping leading blanks (or other whitespace?). The
token is terminated by the first appearance of tokchar,
or by the end of the source string.

The caller must supply sufficient space in token to
receive any token, Otherwise tokens will be truncated.

Returns: a pointer past the terminating tokchar.

This will happily return an infinity of empty tokens if
called with src pointing to the end of a string. Tokens
will never include a copy of tokchar.

A better name would be "strtkn", except that is reserved
for the system namespace. Change to that at your risk.

released to Public Domain, by C.B. Falconer.
Published 2006-02-20. Attribution appreciated.
*/

unsigned char *toksplit(unsigned char *src, /* Source of tokens */
char tokchar, /* token delimiting char */
unsigned char *token, /* receiver of parsed token */
int lgh) /* length token can receive */
/* not including final '\0' */
{
  if (src) {
    //while (' ' == *src) *src++;
    while (' ' == *src) *src=*src+1; // avoids a compiler warning

    while (*src && (tokchar != *src)) {
      if (lgh) {
        *token++ = *src;
        --lgh;
      }
      src++;
    }
    if (*src && (tokchar == *src)) src++;
  }
  *token = '\0';
  return src;
} /* toksplit */


void doTextArea(textArea* text) {
  int scroll = 0;
  int key;
  while(1) {
    drawRectangle(text->x, text->y+24, text->width, LCD_HEIGHT_PX-24, COLOR_WHITE);
    int cur = 0;
    int textX = text->x;
    int textY = scroll+(text->showtitle ? 24 : 0); // 24 pixels for title (or not)
    int temptextY = 0;
    int temptextX = 0;
    while(cur <= text->numelements-1) {
      if(text->elements[cur].newLine) {
        textX=text->x;
        textY=textY+text->lineHeight+text->elements[cur].lineSpacing; 
      }
      unsigned char* singleword = (unsigned char*)malloc(strlen(text->elements[cur].text)); // because of this, a single text element can't have more bytes than malloc can provide
      unsigned char* src = (unsigned char*)text->elements[cur].text;
      while(*src)
      {
        temptextX = 0;
        src = toksplit(src, ' ', (unsigned char*)singleword, strlen(text->elements[cur].text)); //break into words; next word
        //check if printing this word would go off the screen, with fake PrintMini drawing:
        PrintMini(&temptextX, &temptextY, (unsigned char*)singleword, 0, 0xFFFFFFFF, 0, 0, text->elements[cur].color, COLOR_WHITE, 0, 0);
        if(temptextX + textX > text->width-6) {
          //time for a new line
          textX=text->x;
          textY=textY+text->lineHeight;
          PrintMini(&textX, &textY, (unsigned char*)singleword, 0, 0xFFFFFFFF, 0, 0, text->elements[cur].color, COLOR_WHITE, 1, 0);
        } else {
          //still fits, print new word normally
          PrintMini(&textX, &textY, (unsigned char*)singleword, 0, 0xFFFFFFFF, 0, 0, text->elements[cur].color, COLOR_WHITE, 1, 0);
        }
        //add a space, since it was removed from token
        if(*src || text->elements[cur].spaceAtEnd) PrintMini(&textX, &textY, (unsigned char*)" ", 0, 0xFFFFFFFF, 0, 0, COLOR_BLACK, COLOR_WHITE, 1, 0);
      }
      free(singleword);
      cur++;
    }
    if(text->showtitle) {
      unsigned char buffer[50] = "";
      unsigned char buffer2[50] = "";
      strcpy((char*)buffer, "  ");
      strcpy((char*)buffer2, "");
      strcat((char*)buffer, (char*)text->title);
      PrintXY(1, 1, (char*)"                        ", TEXT_MODE_NORMAL, TEXT_COLOR_BLUE);
      PrintXY(1, 1, (char*)buffer, TEXT_MODE_TRANSPARENT_BACKGROUND, TEXT_COLOR_BLUE);
    }
    //draw a scrollbar:
    if(text->scrollbar) {
      TScrollbar sb;
      sb.I1 = 0;
      sb.I5 = 0;
      sb.indicatormaximum = (textY-scroll);
      sb.indicatorheight = 10*17;
      sb.indicatorpos = -scroll;
      sb.barheight = LCD_HEIGHT_PX-24*(text->showtitle ? 2 : 1);
      sb.bartop = (text->showtitle ? 24 : 0);
      sb.barleft = text->width - 6;
      sb.barwidth = 6;

      Scrollbar(&sb);
    }
    GetKey(&key);
    switch(key)
    {
      case KEY_CTRL_UP:
        if (scroll < 0) {
          scroll = scroll + 17;
        }
        break;
      case KEY_CTRL_DOWN:
        if (textY > LCD_HEIGHT_PX-24*2) {
          scroll = scroll - 17;
        }
        break;
      case KEY_CTRL_EXIT: return; break;
    }
  }
}