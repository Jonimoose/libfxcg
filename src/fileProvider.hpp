#ifndef __FILEPROVIDER_H
#define __FILEPROVIDER_H
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

#include "menuGUI.hpp"

#define MAX_FILENAME_SIZE 768 //full path with //fls0/, extension and everything
#define MAX_NAME_SIZE 256 //friendly name (in "//fls0/folder/file.txt", this would be "file.txt")
#define MAX_ITEMS_IN_DIR 200
#define MAX_ITEMS_IN_CLIPBOARD 51
#define MAX_TEXTVIEWER_FILESIZE 64*1024
typedef struct
{
  char filename[MAX_FILENAME_SIZE]; //filename, not proper for use with Bfile.
  int action; // mostly for clipboard, can be used to tag something to do with the file
} File; // right now File only holds the filename as other fields are now set directly on a MenuItem array

typedef struct
{
        unsigned short id, type;
        unsigned long fsize, dsize;
        unsigned int property;
        unsigned long address;
} file_type_t; 

#define GETFILES_SUCCESS 0
#define GETFILES_MAX_FILES_REACHED 1

int GetFiles(File* files, MenuItem* menuitems, char* basepath, int* count, unsigned char* filter);
void nameFromFilename(char* filename, char* name);

#endif