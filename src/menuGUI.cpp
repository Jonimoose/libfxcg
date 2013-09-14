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
#include "graphicsProvider.hpp"

int doMenu(Menu* menu) { // returns code telling what user did. selection is on menu->selection. menu->selection starts at 1!
  int key;
  int itemsStartY=menu->startY; // char Y where to start drawing the menu items. Having a title increases this by one
  if (menu->showtitle) itemsStartY++;
  int itemsHeight=menu->height;
  if (menu->showtitle) itemsHeight--;
  
  // prepare item background filler string according to menu width
  char itemBackFiller[23] = "";
  while(1) {
    // Clear the area of the screen we are going to draw on
    DefineStatusMessage((char*)menu->statusText, 1, 0, 0);
    drawRectangle(18*(menu->startX-1), 24*menu->startY, 18*menu->width, 24*menu->height, COLOR_WHITE);
    int curitem= 0; //current processing item
    if (menu->numitems>0) {
      while(curitem < menu->numitems) {
        // print the menu item only when appropriate
        if(menu->scroll < curitem+1 && menu->scroll > curitem-itemsHeight) {
          char menuitem[70] = "";
          strcpy(menuitem, "  ");
          if(menu->type == MENUTYPE_MULTISELECT) strcat(menuitem, "  "); //allow for the folder and selection icons on MULTISELECT menus (e.g. file browser)
          strcat(menuitem, (char*)menu->items[curitem].text);
          //make sure we have a string big enough to have background when item is selected:          
          strcpy(itemBackFiller, "");
          int fillerSpaces = 0;
          // MB_ElementCount is used instead of strlen because multibyte chars count as two with strlen, while graphically they are just one char, making fillerRequired become wrong
          int fillerRequired = menu->width - MB_ElementCount(menu->items[curitem].text) - (menu->type == MENUTYPE_MULTISELECT ? 2 : 0);
          while (fillerSpaces < fillerRequired) {
            strcat(itemBackFiller, " ");
            fillerSpaces++;
          }
          strcat(menuitem, itemBackFiller); 
          PrintXY(menu->startX,curitem+itemsStartY-menu->scroll,(char*)menuitem, (menu->selection == curitem+1 ? TEXT_MODE_INVERT : TEXT_MODE_NORMAL), menu->items[curitem].color);
          // deal with menu items of type MENUITEM_CHECKBOX
          if(menu->items[curitem].type == MENUITEM_CHECKBOX) {
            if(menu->items[curitem].value == MENUITEM_VALUE_CHECKED) {
              PrintXY(menu->startX+menu->width-1,curitem+itemsStartY-menu->scroll,(char*)"  \xe6\xa9", (menu->selection == curitem+1 ? TEXT_MODE_INVERT : TEXT_MODE_NORMAL), menu->items[curitem].color);
            } else {
              PrintXY(menu->startX+menu->width-1,curitem+itemsStartY-menu->scroll,(char*)"  \xe6\xa5", (menu->selection == curitem+1 ? TEXT_MODE_INVERT : TEXT_MODE_NORMAL), menu->items[curitem].color);
            }
          }
          // deal with multiselect menus
          if(menu->type == MENUTYPE_MULTISELECT) {
            if (menu->items[curitem].isfolder == 1) {
              if((curitem+itemsStartY-menu->scroll)>=itemsStartY&&(curitem+itemsStartY-menu->scroll)<=(itemsStartY+itemsHeight)) CopySpriteMasked((unsigned char*)folder_icon, menu->startX+18, (curitem+itemsStartY-menu->scroll)*24+4, 17, 15, 0xf81f  );
            }
            if (menu->items[curitem].isselected) {
              if (menu->selection == curitem+1) {
                  PrintXY(menu->startX,curitem+itemsStartY-menu->scroll,(char*)"   ", TEXT_MODE_INVERT, menu->items[curitem].color);
                  PrintXY(menu->startX,curitem+itemsStartY-menu->scroll,(char*)"  \xe6\x9b", TEXT_MODE_TRANSPARENT_BACKGROUND, (menu->items[curitem].color ==  TEXT_COLOR_GREEN ? TEXT_COLOR_BLUE : TEXT_COLOR_GREEN));
              } else {
                PrintXY(menu->startX,curitem+itemsStartY-menu->scroll,(char*)"   ", TEXT_MODE_NORMAL, TEXT_COLOR_BLACK);
                PrintXY(menu->startX,curitem+itemsStartY-menu->scroll,(char*)"  \xe6\x9b", TEXT_MODE_NORMAL, TEXT_COLOR_PURPLE);
              }
            }
          }
        }
        curitem++;
      }
      if (menu->scrollbar) {
        TScrollbar sb;
        sb.I1 = 0;
        sb.I5 = 0;
        sb.indicatormaximum = menu->numitems;
        sb.indicatorheight = itemsHeight;
        sb.indicatorpos = menu->scroll;
        sb.barheight = itemsHeight*24;
        sb.bartop = (itemsStartY-1)*24;
        sb.barleft = menu->startX*18+menu->width*18 - 18 - (menu->scrollout ? 0 : 5);
        sb.barwidth = 6;
        Scrollbar(&sb);
      }
    } else {
      printCentered((unsigned char*)menu->nodatamsg, (itemsStartY*24)+(itemsHeight*24)/2-12, COLOR_BLACK, COLOR_WHITE);
    }
    DisplayStatusArea();
    //if (GetSetting(SETTING_DISPLAY_STATUSBAR) && GetSetting(SETTING_THEME) == 1) { darkenStatusbar(); DrawFrame( 0x000000  ); }
    if(menu->showtitle) {
      char title[21] = "";
      strcpy(title, "  ");
      strcat(title, menu->title);
      PrintXY(menu->startX, menu->startY, title, TEXT_MODE_TRANSPARENT_BACKGROUND, menu->titleColor);
      
    }
    if(menu->showsubtitle && menu->showtitle) { // linker havocs with PrintMini if I put this if inside the one above
      int textX=MB_ElementCount(menu->title)*18+10, textY=6;
      PrintMini(&textX, &textY, (unsigned char*)menu->subtitle, 0, 0xFFFFFFFF, 0, 0, COLOR_BLACK, COLOR_WHITE, 1, 0);
    }
    /*int iresult;
    if(menu->numitems>0 && menu->type==MENUTYPE_MULTISELECT) {
      if (menu->fkeypage == 0) {
        GetFKeyPtr(0x0037, &iresult); // SELECT (white)
        FKey_Display(0, (int*)iresult);
      }
    }*/
    //if(menu->type != MENUTYPE_NO_KEY_HANDLING) {
      /*if(menu->allowMkey) mGetKey(&key);
      else*/ GetKey(&key);
      
      switch(key)
      {
        case KEY_CTRL_DOWN:
          if(menu->selection == menu->numitems)
          {
            menu->selection = 1;
            menu->scroll = 0;
          }
          else
          {
            menu->selection++;
            if(menu->selection > menu->scroll+(menu->numitems>itemsHeight ? itemsHeight : menu->numitems))
              menu->scroll = menu->selection -(menu->numitems>itemsHeight ? itemsHeight : menu->numitems);
          }
          //if(menu->type == MENUTYPE_INSTANT_RETURN) return MENU_RETURN_INSTANT;
          break;
        case KEY_CTRL_UP:
          if(menu->selection == 1)
          {
            menu->selection = menu->numitems;
            menu->scroll = menu->selection-(menu->numitems>itemsHeight ? itemsHeight : menu->numitems);
          }
          else
          {
            menu->selection--;
            if(menu->selection-1 < menu->scroll)
              menu->scroll = menu->selection -1;
          }
          //if(menu->type == MENUTYPE_INSTANT_RETURN) return MENU_RETURN_INSTANT;
          break;
        /*case KEY_CTRL_F1:
          if(menu->type==MENUTYPE_MULTISELECT && menu->fkeypage == 0) {
            if(menu->items[menu->selection-1].isselected) {
              menu->items[menu->selection-1].isselected=0;
              menu->numselitems = menu->numselitems-1;
            } else {
              menu->items[menu->selection-1].isselected=1;
              menu->numselitems = menu->numselitems+1;
            }
            return key; //return on F1 too so that parent subroutines have a chance to e.g. redraw fkeys
          } else if (menu->type == MENUTYPE_FKEYS) {
            return key;
          } else if(menu->type == MENUTYPE_INSTANT_RETURN) return MENU_RETURN_INSTANT;
          break;
        case KEY_CTRL_F2:
        case KEY_CTRL_F3:
        case KEY_CTRL_F4:
        case KEY_CTRL_F5:
        case KEY_CTRL_F6:
          if (menu->type == MENUTYPE_FKEYS || menu->type==MENUTYPE_MULTISELECT) return key; // MULTISELECT also returns on Fkeys
          break;
        case KEY_CTRL_PASTE:
          if (menu->type==MENUTYPE_MULTISELECT) return key; // MULTISELECT also returns on paste
          break;*/
        case KEY_CTRL_EXE:
          if(menu->numitems>0) { return MENU_RETURN_SELECTION; }
          break;
        case KEY_CTRL_EXIT: return MENU_RETURN_EXIT;
          break;
        case KEY_CHAR_1:
          if(menu->numitems>=1) {menu->selection = 1; return MENU_RETURN_SELECTION; }
          break;
        case KEY_CHAR_2:
          if(menu->numitems>=2) {menu->selection = 2; return MENU_RETURN_SELECTION; }
          break;
        case KEY_CHAR_3:
          if(menu->numitems>=3) {menu->selection = 3; return MENU_RETURN_SELECTION; }
          break;
        case KEY_CHAR_4:
          if(menu->numitems>=4) {menu->selection = 4; return MENU_RETURN_SELECTION; }
          break;
        case KEY_CHAR_5:
          if(menu->numitems>=5) {menu->selection = 5; return MENU_RETURN_SELECTION; }
          break;
        case KEY_CHAR_6:
          if(menu->numitems>=6) {menu->selection = 6; return MENU_RETURN_SELECTION; }
          break;
        case KEY_CHAR_7:
          if(menu->numitems>=7) {menu->selection = 7; return MENU_RETURN_SELECTION; }
          break;
        case KEY_CHAR_8:
          if(menu->numitems>=8) {menu->selection = 8; return MENU_RETURN_SELECTION; }
          break;
        case KEY_CHAR_9:
          if(menu->numitems>=9) {menu->selection = 9; return MENU_RETURN_SELECTION; }
          break;
        case KEY_CHAR_0:
          if(menu->numitems>=10) {menu->selection = 10; return MENU_RETURN_SELECTION; }
          break;
        case KEY_CTRL_XTT:
          if(menu->numitems>=11) {menu->selection = 11; return MENU_RETURN_SELECTION; }
          break;
        case KEY_CHAR_LOG:
          if(menu->numitems>=12) {menu->selection = 12; return MENU_RETURN_SELECTION; }
          break;
        case KEY_CHAR_LN:
          if(menu->numitems>=13) {menu->selection = 13; return MENU_RETURN_SELECTION; }
          break;
        case KEY_CHAR_SIN:
          if(menu->numitems>=14) {menu->selection = 14; return MENU_RETURN_SELECTION; }
          break;
        case KEY_CHAR_COS:
          if(menu->numitems>=15) {menu->selection = 15; return MENU_RETURN_SELECTION; }
          break;
        case KEY_CHAR_TAN:
          if(menu->numitems>=16) {menu->selection = 16; return MENU_RETURN_SELECTION; }
          break;
        case KEY_CHAR_FRAC:
          if(menu->numitems>=17) {menu->selection = 17; return MENU_RETURN_SELECTION; }
          break;
        case KEY_CTRL_FD:
          if(menu->numitems>=18) {menu->selection = 18; return MENU_RETURN_SELECTION; }
          break;
        case KEY_CHAR_LPAR:
          if(menu->numitems>=19) {menu->selection = 19; return MENU_RETURN_SELECTION; }
          break;
        case KEY_CHAR_RPAR:
          if(menu->numitems>=20) {menu->selection = 20; return MENU_RETURN_SELECTION; }
          break;
        case KEY_CHAR_COMMA:
          if(menu->numitems>=21) {menu->selection = 21; return MENU_RETURN_SELECTION; }
          break;
        case KEY_CHAR_STORE:
          if(menu->numitems>=22) {menu->selection = 22; return MENU_RETURN_SELECTION; }
          break;
      }
    /*} else {
      return MENU_RETURN_INSTANT; // we don't want to handle keys
    }*/
  }
  return MENU_RETURN_SELECTION;
} 
