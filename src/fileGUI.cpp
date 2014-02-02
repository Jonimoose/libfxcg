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
#include "fileProvider.hpp" 
#include "fileGUI.hpp"

int fileBrowser(char* filename, unsigned char* filter, char* title) {
  // returns 1 when user selects a file, 0 when aborts (exit)
  int res = 1;
  char browserbasepath[MAX_FILENAME_SIZE+1] = "\\\\fls0\\";  
  while(res) {
    strcpy(filename, (char*)"");
    res = fileBrowserSub(browserbasepath, filename, filter, title);
    if(res==2) return 1; // user selected a file
  }
  return 0;
}

int fileBrowserSub(char* browserbasepath, char* filename, unsigned char* filter, char* title) {
  int inscreen = 1;
  Menu menu;
  
  // first get file count so we know how much to alloc
  GetFiles(NULL, NULL, browserbasepath, &menu.numitems, filter);
  MenuItem* menuitems = NULL;
  File* files = NULL;
  if(menu.numitems > 0) {
    menuitems = (MenuItem*)alloca(menu.numitems*sizeof(MenuItem));
    files = (File*)alloca(menu.numitems*sizeof(File));
    // populate arrays
    GetFiles(files, menuitems, browserbasepath, &menu.numitems, filter);
    menu.items = menuitems;
  }
  
  char titleBuffer[23] = ""; 
  int smemfree;
  unsigned short smemMedia[7]={'\\','\\','f','l','s','0',0};
  Bfile_GetMediaFree_OS( smemMedia, &smemfree );
  
  char friendlypath[MAX_FILENAME_SIZE] = "";
  strcpy(friendlypath, browserbasepath+6);
  friendlypath[strlen(friendlypath)-1] = '\0'; //remove ending slash like OS does
  menu.subtitle = friendlypath;
  menu.showsubtitle = 1;
  menu.type = MENUTYPE_MULTISELECT;
  menu.height = 8;
  menu.scrollout=1;
  strcpy(menu.nodatamsg, "No Data");
  strcpy(menu.title, title);
  menu.showtitle=1;
  while(inscreen) {
    Bdisp_AllClr_VRAM();
    itoa(smemfree, (unsigned char*)menu.statusText);
    LocalizeMessage1( 340, titleBuffer ); //"bytes free"
    strcat((char*)menu.statusText, (char*)titleBuffer);
    /*int iresult;
    if(menu.fkeypage == 0) {
      if(menu.numitems>0) {
        GetFKeyPtr(0x0188, &iresult); // RENAME
        FKey_Display(4, (int*)iresult);
      }
      if(menu.numselitems>0) {
        GetFKeyPtr(0x0069, &iresult); // CUT (white)
        FKey_Display(1, (int*)iresult);
        if(GetSetting(SETTING_SHOW_ADVANCED)) {
          GetFKeyPtr(0x0034, &iresult); // COPY (white)
          FKey_Display(2, (int*)iresult);
        }
        GetFKeyPtr(0x0038, &iresult); // DELETE
        FKey_Display(5, (int*)iresult);
      }
      GetFKeyPtr(0x038E, &iresult); // MKFLDR
      FKey_Display(3, (int*)iresult);
    }*/
    int res = doMenu(&menu);
    switch(res) {
      case MENU_RETURN_EXIT:
        if(!strcmp(browserbasepath,"\\\\fls0\\")) { //check that we aren't already in the root folder
          //we are, return 0 so we exit
          return 0;
        } else {
          int i=strlen(browserbasepath)-2;
          while (i>=0 && browserbasepath[i] != '\\')
                  i--;
          if (browserbasepath[i] == '\\') {
            char tmp[MAX_FILENAME_SIZE] = "";
            memcpy(tmp,browserbasepath,i+1);
            tmp[i+1] = '\0';
            strcpy(browserbasepath, tmp);
          }
          return 1; //reload at new folder
        }
        break;
      case MENU_RETURN_SELECTION:
        if(menuitems[menu.selection-1].isfolder) {
          strcpy(browserbasepath, files[menu.selection-1].filename); //switch to selected folder
          strcat(browserbasepath, "\\");
          return 1; //reload at new folder
        } else {
          strcpy(filename,files[menu.selection-1].filename);
          return 2;
        }
        break;
      /*case KEY_CTRL_F2:
      case KEY_CTRL_F3:
        if(!GetSetting(SETTING_SHOW_ADVANCED) && res == KEY_CTRL_F3) break; //user didn't enable copy tool
        if (menu.numselitems > 0 && menu.fkeypage==0) {
          if((!(*itemsinclip >= MAX_ITEMS_IN_CLIPBOARD)) && menu.numselitems <= MAX_ITEMS_IN_CLIPBOARD-*itemsinclip) {
            int ifile = 0; int hasShownFolderCopyWarning = 0;
            while(ifile < menu.numitems) {  
              if (menu.items[ifile].isselected) {
                if(res == KEY_CTRL_F2) {
                  strcpy(clipboard[*itemsinclip].filename, files[ifile].filename);
                  //0=cut file; 1=copy file:
                  clipboard[*itemsinclip].action = 0;
                  *itemsinclip = *itemsinclip + 1;
                } else {
                  if (!menu.items[ifile].isfolder) {
                    strcpy(clipboard[*itemsinclip].filename, files[ifile].filename);
                    //0=cut file; 1=copy file:
                    clipboard[*itemsinclip].action = 1;
                    *itemsinclip = *itemsinclip + 1;
                  } else {
                    if (!hasShownFolderCopyWarning) {
                      MsgBoxPush(4);
                      PrintXY(3, 2, (char*)"  Copying folders", TEXT_MODE_TRANSPARENT_BACKGROUND, TEXT_COLOR_BLACK);
                      PrintXY(3, 3, (char*)"  not yet supported", TEXT_MODE_TRANSPARENT_BACKGROUND, TEXT_COLOR_BLACK);
                      PrintXY_2(TEXT_MODE_NORMAL, 1, 5, 2, TEXT_COLOR_BLACK); // press exit message
                      int inscreen=1, key;
                      while(inscreen) {
                        mGetKey(&key);
                        switch(key)
                        {
                    case KEY_CTRL_EXIT:
                          case KEY_CTRL_AC:
                            inscreen=0;
                            break;
                        }
                      }
                      MsgBoxPop();
                      hasShownFolderCopyWarning = 1;
                    }
                  }
                }
                menu.items[ifile].isselected = 0; // clear selection
                menu.numselitems--;
              }
              ifile++;
            }
          } else {
            MsgBoxPush(4);
            PrintXY(3, 2, (char*)"  Can't add", TEXT_MODE_TRANSPARENT_BACKGROUND, TEXT_COLOR_BLACK);
            PrintXY(3, 3, (char*)"  selected items to", TEXT_MODE_TRANSPARENT_BACKGROUND, TEXT_COLOR_BLACK);
            PrintXY(3, 4, (char*)"  clipboard.", TEXT_MODE_TRANSPARENT_BACKGROUND, TEXT_COLOR_BLACK);
            PrintXY_2(TEXT_MODE_NORMAL, 1, 5, 2, TEXT_COLOR_BLACK); // press exit message
            int inscreen=1, key;
            while(inscreen) {
              mGetKey(&key);
              switch(key)
              {
                case KEY_CTRL_EXIT:
                case KEY_CTRL_AC:
                  inscreen=0;
                  break;
              }
            }
            MsgBoxPop();
          }
        }
        break;
      case KEY_CTRL_F4:
        if(makeFolderGUI(browserbasepath)) return 1; // if user said yes and files were deleted, reload file list
        break;
      case KEY_CTRL_F5:
        if(renameFileGUI(files, &menu, browserbasepath)) return 1;
        break;
      case KEY_CTRL_F6:
        if(menu.numselitems>0) if(deleteFilesGUI(files, &menu)) return 1; // if user said yes and files were deleted, reload file list
        break;
      case KEY_CTRL_PASTE:
        filePasteClipboardItems(clipboard, browserbasepath, *itemsinclip);
        *itemsinclip = 0;
        return 1;
        break;*/
    }
  }
  return 1;
}