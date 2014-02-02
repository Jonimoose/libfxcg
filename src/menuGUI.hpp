#ifndef __MENUGUI_H
#define __MENUGUI_H

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

#define MENUITEM_NORMAL 0
#define MENUITEM_CHECKBOX 1
#define MENUITEM_VALUE_NONE 0
#define MENUITEM_VALUE_CHECKED 1
typedef struct
{
  char text[42]; // text to be shown on screen. This shouldn't take more than 21 chars (20 in case of checkboxes) but I'm giving 42 because of multibyte codes...
  char tag[30]; // internal var of the menu item, may be useful for some thing.
  //void (*handler)(void); // routine to call when user performs an action on this menu item. for normal menuitems, this is when it is selected (press EXE). for checkboxes, this is when the checkbox is toggled.
  int type=MENUITEM_NORMAL; // type of the menu item. use MENUITEM_* to set this
  int value=MENUITEM_VALUE_NONE; // value of the menu item. For example, if type is MENUITEM_CHECKBOX and the checkbox is checked, the value of this var will be MENUITEM_VALUE_CHECKED
  int color=TEXT_COLOR_BLACK; // color of the menu item (use TEXT_COLOR_* to define)
  // The following two settings require the menu type to be set to MENUTYPE_MULTISELECT
  int isfolder=0; // for file browsers, this will show a folder before the item.
  int isselected=0; // for file browsers and other multi-select screens, this will show a arrow before the item
} MenuItem;

#define MENUTYPE_NORMAL 0
#define MENUTYPE_MULTISELECT 1
/*#define MENUTYPE_INSTANT_RETURN 2 // this type of menu insantly returns even if user hasn't selected an option (allows for e.g. redrawing the GUI behind it). if user hasn't exited or selected an option, menu will return MENU_RETURN_INSTANT
#define MENUTYPE_NO_KEY_HANDLING 3 //this type of menu doesn't handle any keys, only draws.
#define MENUTYPE_FKEYS 4 // returns GetKey value of a Fkey when one is pressed*/
typedef struct {
  char statusText[75]; // text to be shown on the status bar, may be empty
  int showtitle=0; // whether to show a title as the first line
  char title[21]; // title to be shown on the first line if showtitle is !=0
  char* subtitle;
  int showsubtitle=0;
  int titleColor=TEXT_COLOR_BLUE; //color of the title
  char nodatamsg[21]; // message to show when there are no menu items to display
  int startX=1; //X where to start drawing the menu. NOTE this is not absolute pixel coordinates but rather character coordinates
  int startY=1; //Y where to start drawing the menu. NOTE this is not absolute pixel coordinates but rather character coordinates
  int width=21; // NOTE this is not absolute pixel coordinates but rather character coordinates
  int height=8; // NOTE this is not absolute pixel coordinates but rather character coordinates
  int scrollbar=1; // 1 to show scrollbar, 0 to not show it.
  int scrollout=0; // whether the scrollbar goes out of the menu area (1) or it overlaps some of the menu area (0)
  int numitems; // number of items in menu
  int type=MENUTYPE_NORMAL; // set to MENUTYPE_* .
  int selection=1; // currently selected item. starts counting at 1
  int scroll=0; // current scrolling position
  int allowMkey=1; // 1: allow for usage of mGetKey to retrieve keyboard input when user is in the selector
  int fkeypage=0; // for MULTISELECT menu if it should allow file selecting and show the fkey label
  int numselitems=0; // number of selected items
  MenuItem* items; // items in menu
} Menu;

#define MENU_RETURN_EXIT 0
#define MENU_RETURN_SELECTION 1
//#define MENU_RETURN_INSTANT 2
int doMenu(Menu* menu);
const color_t folder_icon[510] = {
                0xf81f,0xf81f,0x52aa,0x52aa,0x52aa,0x52aa,0x52aa,0xf81f,0xf81f,0xf81f,0xf81f,0xf81f,0xf81f,0xf81f,0xf81f,0xf81f,0xf81f,
                0xf81f,0x52aa,0xdef9,0xfffc,0xfffc,0xffba,0xde74,0x52aa,0xf81f,0xf81f,0xf81f,0xf81f,0xf81f,0xf81f,0xf81f,0xf81f,0xf81f,
                0x52aa,0xdef9,0xfffc,0xffbc,0xffba,0xffb9,0xff78,0xde74,0x52aa,0x52aa,0x52aa,0x52aa,0x52aa,0x52aa,0x52aa,0x52aa,0xf81f,
                0x52aa,0xfffc,0xffbb,0xffba,0xffb9,0xff78,0xff76,0xff34,0xff34,0xfef3,0xf6f1,0xf6f1,0xf6f1,0xf6f1,0xf6f1,0x52aa,0xf81f,
                0x52aa,0xffbb,0xffbb,0xffb9,0xff78,0xff76,0xff74,0xff34,0xfef3,0xf6f1,0xf6f1,0xf6b1,0xf6f1,0xf6b1,0xf6b1,0x52aa,0xad55,
                0x52aa,0xffbb,0xffb9,0xff77,0xff76,0xff35,0xff34,0xfef2,0xf6f1,0xf6b1,0xf6f1,0xf6b1,0xf6b1,0xf6f1,0xf6f1,0x52aa,0xad55,
                0x52aa,0xffb9,0xff78,0xff76,0xff35,0xff34,0xfef2,0xf6f2,0xf6b1,0xf6f1,0xf6b1,0xf6f1,0xf6f1,0xf6f1,0xf6b1,0x52aa,0xad55,
                0x52aa,0xff77,0xff76,0xff35,0xff33,0xff33,0xf6f1,0xf6b1,0xf6f1,0xf6b1,0xeef1,0xf6b1,0xf6b1,0xf6f1,0xf6b1,0x52aa,0xad55,
                0x52aa,0xff76,0xff75,0xff34,0xfef3,0xf6f1,0xf6b1,0xf6f1,0xf6f1,0xf6b1,0xf6f1,0xf6b1,0xf6f1,0xf6b1,0xf6f1,0x52aa,0xad55,
                0x52aa,0xff35,0xff33,0xff32,0xf6f1,0xf6b1,0xf6f1,0xf6b1,0xeeb1,0xf6f1,0xf6b1,0xf6f1,0xeeb1,0xf6f1,0xf6b1,0x52aa,0xad55,
                0x52aa,0xff33,0xff33,0xf6b2,0xf6f1,0xf6b1,0xf6f1,0xf6b1,0xf6f1,0xf6b1,0xf6f1,0xf6b1,0xf6f1,0xf6b1,0xf6f1,0x52aa,0xad55,
                0x52aa,0xfef3,0xf6f1,0xf6b1,0xf6b1,0xf6f1,0xeeb1,0xf6f1,0xf6b1,0xf6f1,0xeeb1,0xf6f1,0xf6b1,0xf6f1,0xf6b1,0x52aa,0xad55,
                0x52aa,0xf6f1,0xf6b1,0xf6f1,0xf6f1,0xf6b1,0xf6f1,0xf6b1,0xf6f1,0xf6b1,0xf6f1,0xf6b1,0xf6f1,0xeeb1,0xf6f1,0x52aa,0xad55,
                0x52aa,0x52aa,0x52aa,0x52aa,0x52aa,0x52aa,0x52aa,0x52aa,0x52aa,0x52aa,0x52aa,0x52aa,0x52aa,0x52aa,0x52aa,0x52aa,0xad55,
                0xf81f,0xf81f,0xad55,0xad55,0xad55,0xad55,0xad55,0xad55,0xad55,0xad55,0xad55,0xad55,0xad55,0xad55,0xad55,0xad55,0xad55
        };
#endif