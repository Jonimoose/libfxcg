#ifndef __TEXTGUI_H
#define __TEXTGUI_H
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

typedef struct
{
  char* text;
  int newLine=0; // if 1, new line will be drawn before the text
  color_t color=COLOR_BLACK;
  int spaceAtEnd=0;
  int lineSpacing=0;
} textElement;

typedef struct
{
  int x=0;
  int y=0;
  int width=LCD_WIDTH_PX;
  //int height=LCD_HEIGHT_PX-24;
  int lineHeight=17;
  textElement* elements;
  int numelements;
  char title[42];
  int showtitle = 1;
  int scrollbar=1;
} textArea;

void doTextArea(textArea* text);

#endif 
