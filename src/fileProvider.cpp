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
#include "menuGUI.hpp"
#include "fileProvider.hpp"

int GetFiles(File* files, MenuItem* menuitems, char* basepath, int* count, unsigned char* filter) {
  // searches storage memory for folders and files, puts their count in int* count
  // if File* files is NULL, function will only count files. If it is not null, MenuItem* menuitems will also be updated
  // this function always returns status codes defined on fileProvider.hpp
  // basepath should start with \\fls0\ and should always have a slash (\) at the end
  // filter is the filter for the files to list
  unsigned short path[MAX_FILENAME_SIZE+1], found[MAX_FILENAME_SIZE+1];
  unsigned char buffer[MAX_FILENAME_SIZE+1];

  // make the buffer
  strcpy((char*)buffer, basepath);
  strcat((char*)buffer, "*");
  
  *count = 0;
  file_type_t fileinfo;
  int findhandle;
  Bfile_StrToName_ncpy(path, buffer, MAX_FILENAME_SIZE+1);
  int ret = Bfile_FindFirst_NON_SMEM((const char*)path, &findhandle, (char*)found, &fileinfo);
  Bfile_StrToName_ncpy(path, filter, MAX_FILENAME_SIZE+1);
  while(!ret) {
    Bfile_NameToStr_ncpy(buffer, found, MAX_FILENAME_SIZE+1);
    if(!(strcmp((char*)buffer, "..") == 0 || strcmp((char*)buffer, ".") == 0 || strcmp((char*)buffer, "@MainMem") == 0) &&
       (fileinfo.fsize == 0 || Bfile_Name_MatchMask((const short int*)path, (const short int*)found)))
    {
      if(files != NULL) {
        if(strlen((char*)buffer) > 40) {
          strcpy(menuitems[*count].text, (char*)"[Filename too big]");
        } else {
          strcpy(menuitems[*count].text, (char*)buffer);
        }
        strcpy(files[*count].filename, basepath); 
        strcat(files[*count].filename, (char*)buffer);
        if(fileinfo.fsize == 0) menuitems[*count].isfolder = 1; else menuitems[*count].isfolder = 0;
        menuitems[*count].isselected = 0; //clear selection. this means selection is cleared when changing directory (doesn't happen with native file manager)
        // because usually alloca is used to declare space for MenuItem*, the space is not cleared. which means we need to explicitly set each field:
        menuitems[*count].color=TEXT_COLOR_BLACK;
        menuitems[*count].type=MENUITEM_NORMAL;
        menuitems[*count].value=MENUITEM_VALUE_NONE;
      }
      *count=*count+1;
    }
    if (*count-1==MAX_ITEMS_IN_DIR) { 
      return GETFILES_MAX_FILES_REACHED;
      break; // Don't find more files, the array is full. 
    } 
    ret = Bfile_FindNext_NON_SMEM(findhandle, (char*)found, (char*)&fileinfo);
  }
  Bfile_FindClose(findhandle);
  return GETFILES_SUCCESS;
}

void nameFromFilename(char* filename, char* name) {
  //this function takes a full filename like \\fls0\Folder\file.123
  //and puts file.123 in name.
  strcpy(name, (char*)"");
  int i=strlen(filename)-1;
  while (i>=0 && filename[i] != '\\')
          i--;
  if (filename[i] == '\\') {
    strcpy(name, filename+i+1);
  }
}