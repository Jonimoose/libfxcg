#ifndef __CATALOGGUI_H
#define __CATALOGGUI_H

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

typedef struct {
        char* name;
        char* insert;
        char* desc;
        int category;
} catalogFunc;

#define CAT_CATEGORY_ALL 0
#define CAT_CATEGORY_COMPLEXNUM 1
#define CAT_CATEGORY_LINEARALG 2
#define CAT_CATEGORY_CALCULUS 3
#define CAT_CATEGORY_POLYNOMIAL 4
#define CAT_CATEGORY_SPECIAL 5
#define CAT_CATEGORY_PROG 6
#define CAT_CATEGORY_TRIG 7
#define CAT_CATEGORY_OTHER 8

#define CAT_COMPLETE_COUNT 99

int showCatalog(char* insertText);
int doCatalogMenu(char* insertText, char* title, int category);

#endif
