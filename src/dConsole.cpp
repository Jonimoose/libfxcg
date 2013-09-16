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
extern "C" {
#include "dConsole.h"
}
#include "catalogGUI.hpp"
#include "aboutGUI.hpp"

#include "graphicsProvider.hpp"
#define LINE_ROW_MAX    10
#define LINE_COL_MAX    32

typedef unsigned int    uint;
typedef unsigned char   uchar;

char    line_buf[LINE_ROW_MAX][LINE_COL_MAX+1];
int             line_index      = 0;
int             line_x          = 0;
int             line_start      = 0;
int             line_count      = 0;

int myconsolex = 0;
int myconsoley = 0;

void locate(int x, int y) {
  myconsolex = x-1;
  myconsoley = y-1;
}
void print(unsigned char* msg) {
  PrintMiniFix( myconsolex*12, myconsoley*17, (char*)msg, 0, COLOR_BLACK, COLOR_WHITE );
  myconsolex = myconsolex + strlen((char*)msg);
}
/*
void dAreaClear (int left,int top,int right,int bottom,int sel) 
{ 
        DISPBOX box; 
        box.left = left;box.top = top;box.right = right;box.bottom = bottom; 
        if (sel != 1) 
                Bdisp_AreaClr_VRAM (&box); 
        if (sel != 0){ 
                Bdisp_DrawLineVRAM (box.left,box.top,box.right,box.top); 
                Bdisp_DrawLineVRAM (box.left,box.bottom,box.right,box.bottom); 
                Bdisp_DrawLineVRAM (box.left,box.top,box.left,box.bottom); 
                Bdisp_DrawLineVRAM (box.right,box.top,box.right,box.bottom); 
    } 
}
*/
uint WaitKey ()
{
        int key;GetKey(&key);return key;
}

char dGetKeyChar (uint key)
{
        if (key>=KEY_CHAR_A && key<=KEY_CHAR_Z)
                return key;
        if (key>=KEY_CHAR_LOWER_A && key<=KEY_CHAR_LOWER_Z)
                return key;
        else if (key>=KEY_CHAR_0 && key<= KEY_CHAR_9)
                return key;
        else if (key>=' ' && key<='~')
                return key;
        switch(key)
        {
                /*case KEY_CHAR_PLUS:             return '+';
                case KEY_CHAR_MINUS:    return '-';
                case KEY_CHAR_MULT:             return '*';
                case KEY_CHAR_DIV:              return '/';
                case KEY_CHAR_POW:              return '^';*/
                case KEY_CHAR_PLUS:             return key;
                case KEY_CHAR_MINUS:    return key;
                case KEY_CHAR_MULT:             return key;
                case KEY_CHAR_DIV:              return key;
                case KEY_CHAR_POW:              return key;
        }

        return 0;
}

void dConsoleCls ()
{
        line_index      = 0;
        line_x          = 0;
        line_start      = 0;
        line_count      = 0;
        //Bdisp_AllClr_VRAM();
        dConsoleRedraw();
}

/*int dGetLineBox (char * s,int max,int width,int x,int y)
{
        int             pos = strlen(s);
        int             refresh = 1;
        uint    key;
        char    c;

        while(1)
        {
                if (refresh)
                {
                        dAreaClear(x,y,x+width*6+2,y+10,2);

                        if (pos<width-1)
                        {
                                printXY (x+1,y+2,(uchar*)s,0);
                                printXY (x+1+pos*6,y+2,(uchar*)"_",0);
                        }
                        else
                        {
                                printXY (x+1,y+2,(uchar*)(s+pos-width+1),0);
                                printXY (x+1+(width-1)*6,y+2,(uchar*)"_",0);
                        }
                        refresh = 0;
                }

                GetKey(&key);

                if ((c=dGetKeyChar(key))!=0)
                {
                        if (pos>=max) continue;

                        s[pos++] = c;s[pos] = '\0';
                        refresh = 1;
                }
                else
                {
                        if (key==KEY_CTRL_DEL)
                        {
                                if (pos<=0) continue;
                                s[--pos] = '\0';
                                refresh  = 1;
                        }
                        else if (key==KEY_CTRL_AC)
                        {
                                *s              = 0;
                                pos             = 0;
                                refresh = 1;
                        }
                        else if (key==KEY_CTRL_EXE) return 1;
                        else if (key==KEY_CTRL_EXIT) return 0;

                }

        }
}*/
void printCursor() {
  int x = myconsolex*12;
  int y = myconsoley*17+24;
  // vertical cursor...
  drawLine(x, y+14, x, y, COLOR_BLACK);
  drawLine(x+1, y+14, x+1, y, COLOR_BLACK); 
}
// inserts into subject at position pos. assumes subject has enough space!
//append(s, (char*)"tan(", pos);
void append(char* subject, const char* insert, int pos) {
  //int totlen = strlen(subject)+strlen(insert);
  //char* buf = (char*)alloca((totlen+5)*sizeof(char));
  char buf[INPUTBUFLEN+5] = ""; //alloca would be the nice and flexible solution, but it contaminates the string even after a memset to zero!
  //memset(buf, totlen+5, 0);
  strcpy(buf, (char*)"");
  if(pos==0) {
    strcpy(buf, insert);
    strcat(buf, subject);
  } else if(pos==(int)strlen(subject)) {
    strcat(subject, insert);
    return;
  } else {
    strncpy(buf, subject, pos); // copy at most first pos characters
    strcat(buf, insert); // copy all of insert[] at the end
    strcat(buf, subject+pos); // copy the rest
  }
  strcpy(subject, buf);   // copy it back to subject
}

void do_up_arrow(void);
void do_down_arrow(void);
extern int custom_key_to_handle;
extern int custom_key_to_handle_modifier;
int get_custom_key_handler_state();
int dGetLine (char * s,int max) // This function is depended on dConsole
                                                                // And this function is not allowed to abolish
{
        int             pos = strlen(s);
        int             refresh = 1;
        int             x,y,l,width;
        int    key;
        char    c;


        l = strlen (line_buf[line_index]);

        if (l>=LINE_COL_MAX)
        {
                dConsolePut("\n");
                l = 0;
        }
        else
                dConsoleRedraw();

        x = l + 1;
        y = line_count;
        width = LINE_COL_MAX - l;

        while (1)
        {
                if (refresh)
                {
                        int i;
                        for (i=x;i<=LINE_COL_MAX;++i)
                        {
                                locate(i,y);print((uchar*)" ");
                        }
                        if (pos<width-1)
                        {
                                locate(x,y);            print((uchar*)s);
                                locate(x+pos,y);        printCursor();
                        }
                        else
                        {
                                locate(x,y);                    print((uchar*)s+pos-width+1);
                                locate(x+width-1,y);    printCursor(); //cursor
                        }
                        refresh = 0;
                }
                int keyflag = GetSetupSetting( (unsigned int)0x14);
                GetKey(&key);
                if(key == KEY_CTRL_F5) {
                  if (keyflag == 0x04 || keyflag == 0x08 || keyflag == 0x84 || keyflag == 0x88) {
                    // ^only applies if some sort of alpha (not locked) is already on
                    if (keyflag == 0x08 || keyflag == 0x88) { //if lowercase
                      SetSetupSetting( (unsigned int)0x14, keyflag-0x04);
                      DisplayStatusArea();
                      continue; //do not process the key, because otherwise we will leave alpha status
                    } else {
                      SetSetupSetting( (unsigned int)0x14, keyflag+0x04);
                      DisplayStatusArea();
                      continue; //do not process the key, because otherwise we will leave alpha status
                    }
                  }
                } else if (key==KEY_CHAR_PLUS) {
                  if ((int)strlen(s)>=max) continue;
                  if(strlen(s)==0) {
                    strcat(s, (char*)"last+");
                    pos=pos+5; refresh = 1; //start of line, append "last" as we're going to do a calculation on the previous value
                  } else {
                    append(s, (char*)"+", pos);
                    pos=pos+1; refresh = 1;
                  }
                } else if (key==KEY_CHAR_MINUS) {
                  if ((int)strlen(s)>=max) continue;
                  if(strlen(s)==0) {
                    strcat(s, (char*)"last-");
                    pos=pos+5; refresh = 1; //start of line, append "last" as we're going to do a calculation on the previous value
                  } else {
                    append(s, (char*)"-", pos);
                    pos=pos+1; refresh = 1;
                  }
                } else if (key==KEY_CHAR_MULT) {
                  if ((int)strlen(s)>=max) continue;
                  if(strlen(s)==0) {
                    strcat(s, (char*)"last*");
                    pos=pos+5; refresh = 1; //start of line, append "last" as we're going to do a calculation on the previous value
                  } else {
                    append(s, (char*)"*", pos);
                    pos=pos+1; refresh = 1;
                  }
                } else if (key==KEY_CHAR_DIV) {
                  if ((int)strlen(s)>=max) continue;
                  if(strlen(s)==0) {
                    strcat(s, (char*)"last/");
                    pos=pos+5; refresh = 1; //start of line, append "last" as we're going to do a calculation on the previous value
                  } else {
                    append(s, (char*)"/", pos);
                    pos=pos+1; refresh = 1;
                  }
                } else if (key==KEY_CHAR_POW) {
                  if ((int)strlen(s)>=max) continue;
                  if(strlen(s)==0) {
                    strcat(s, (char*)"last^");
                    pos=pos+5; refresh = 1; //start of line, append "last" as we're going to do a calculation on the previous value
                  } else {
                    append(s, (char*)"^", pos);
                    pos=pos+1; refresh = 1;
                  }
                } else if (key==KEY_CHAR_SQUARE) {
                  if ((int)strlen(s)+1>=max) continue;
                  if(strlen(s)==0) {
                    strcat(s, (char*)"last^2");
                    pos=pos+6; refresh = 1; //start of line, append "last" as we're going to do a calculation on the previous value
                  } else {
                    append(s, (char*)"^2", pos);
                    pos=pos+2; refresh = 1;
                  }
                } else if (key==KEY_CHAR_ROOT) {
                  if ((int)strlen(s)+4>=max) continue;
                  append(s, (char*)"sqrt(", pos);
                  pos=pos+5; refresh = 1;
                } else if (key==KEY_CHAR_CUBEROOT) {
                  if ((int)strlen(s)+5>=max) continue;
                  append(s, (char*)"^(1/3)", pos); // example: to get cubic root of 27, do 27^(1/3)
                  pos=pos+6; refresh = 1;
                } else if (key==KEY_CHAR_POWROOT) {
                  if ((int)strlen(s)+3>=max) continue;
                  append(s, (char*)"^(1/", pos); // example: to get cubic root of 27, do 27^(1/3)
                  pos=pos+4; refresh = 1;
                } else if (key==KEY_CHAR_THETA) {
                  if ((int)strlen(s)+4>=max) continue;
                  append(s, (char*)"theta", pos);
                  pos=pos+5; refresh = 1;
                } else if (key==KEY_CTRL_XTT) {
                  if ((int)strlen(s)>=max) continue;
                  append(s, (char*)"x", pos);
                  pos=pos+1; refresh = 1;
                } else if (key==KEY_CHAR_LN) { // the log() function in eigenmath is the natural log (Casio's ln)
                  if ((int)strlen(s)+3>=max) continue;
                  append(s, (char*)"log(", pos);
                  pos=pos+4; refresh = 1;
                } else if (key==KEY_CHAR_EXPN) { // the exp() function in eigenmath is the natural expnonent (Casio's e^x)
                  if ((int)strlen(s)+3>=max) continue;
                  append(s, (char*)"exp(", pos);
                  pos=pos+4; refresh = 1;
                } else if (key==KEY_CHAR_SIN) {
                  if ((int)strlen(s)+3>=max) continue;
                  append(s, (char*)"sin(", pos);
                  pos=pos+4; refresh = 1;
                } else if (key==KEY_CHAR_ASIN) {
                  if ((int)strlen(s)+6>=max) continue;
                  append(s, (char*)"arcsin(", pos);
                  pos=pos+7; refresh = 1;
                } else if (key==KEY_CHAR_COS) {
                  if ((int)strlen(s)+3>=max) continue;
                  append(s, (char*)"cos(", pos);
                  pos=pos+4; refresh = 1;
                } else if (key==KEY_CHAR_ACOS) {
                  if ((int)strlen(s)+6>=max) continue;
                  append(s, (char*)"arccos(", pos);
                  pos=pos+7; refresh = 1;
                } else if (key==KEY_CHAR_TAN) {
                  if ((int)strlen(s)+3>=max) continue;
                  append(s, (char*)"tan(", pos);
                  pos=pos+4; refresh = 1;
                } else if (key==KEY_CHAR_ATAN) {
                  if ((int)strlen(s)+6>=max) continue;
                  append(s, (char*)"arctan(", pos);
                  pos=pos+7; refresh = 1;
                } else if (key==KEY_CHAR_FRAC) {
                  if ((int)strlen(s)>=max) continue;
                  append(s, (char*)"/", pos);
                  pos=pos+1; refresh = 1;
                } else if (key==KEY_CTRL_FD) {
                  if ((int)strlen(s)+4>=max) continue;
                  append(s, (char*)"float", pos); // no ( at the end because this is often used to manipulate the last result
                  pos=pos+5; refresh = 1;
                } else if (key==KEY_CHAR_STORE) {
                  if ((int)strlen(s)>=max) continue;
                  append(s, (char*)"!", pos); // this really has nothing to do with the key label, but it's useful
                  pos=pos+1; refresh = 1;
                } else if (key==KEY_CHAR_IMGNRY) {
                  if ((int)strlen(s)>=max) continue;
                  append(s, (char*)"i", pos);
                  pos=pos+1; refresh = 1;
                } else if (key==KEY_CHAR_PI) {
                  if ((int)strlen(s)+1>=max) continue;
                  append(s, (char*)"pi", pos);
                  pos=pos+2; refresh = 1;
                } else if (key==KEY_CHAR_EXP) {
                  if ((int)strlen(s)+4>=max) continue;
                  append(s, (char*)"*10^(", pos);
                  pos=pos+5; refresh = 1;
                } else if (key==KEY_CHAR_PMINUS) {
                  if ((int)strlen(s)+1>=max) continue;
                  append(s, (char*)"(-", pos);
                  pos=pos+2; refresh = 1;
                } else if (key==KEY_CHAR_ANS) {
                  if ((int)strlen(s)+3>=max) continue;
                  append(s, (char*)"last", pos);
                  pos=pos+4; refresh = 1;
                } else if (key==KEY_CTRL_F1) {
                  // open functions catalog
                  char text[20] = "";
                  if(showCatalog(text)) {
                    int len = strlen(text);
                    if(pos+len>max) continue;
                    else {
                      append(s, text, pos);
                      pos=pos+len; refresh = 1;
                    }
                  }
                } else if (key==KEY_CTRL_F2) {
                  // select script and run
                  return 2;
                } else if (key==KEY_CTRL_SETUP) {
                  showAbout();
                  Bdisp_AllClr_VRAM();
                  dConsoleRedraw();
                  refresh = 1;
                } else if (key==KEY_CTRL_UP) {
                  // go up in command history
                  do_up_arrow();
                  pos=strlen(s);
                  refresh = 1;
                } else if (key==KEY_CTRL_DOWN) {
                  // go down in command history
                  do_down_arrow();
                  pos=strlen(s);
                  refresh = 1;
                } else if (key==KEY_CTRL_LEFT) {
                  // move cursor left
                  if(pos<=0) continue;
                  pos--;
                  refresh = 1;
                } else if (key==KEY_CTRL_RIGHT) {
                  // move cursor right
                  if(pos>=(int)strlen(s)) continue;
                  pos++;
                  refresh = 1;
                } else if ((c=dGetKeyChar(key))!=0) {
                        if ((int)strlen(s)>=max) continue;
                        char ns[2] = "";
                        ns[0] = c; ns[1]='\0';
                        append(s, ns, pos);
                        pos++;
                        refresh = 1;
                }
                else if (key==KEY_CTRL_DEL)
                {
                        if (pos<=0) continue;
                        pos--;
                        int i = pos;
                        do {
                                s[i] = s[i+1];
                                i++;
                        } while (s[i] != '\0');
                        refresh = 1;
                }
                else if (key==KEY_CTRL_AC)
                {
                        *s              = 0;
                        pos             = 0;
                        refresh = 1;
                }
                else if (key==KEY_CTRL_EXE) return 1;
                else if (key!=KEY_CTRL_SHIFT && key!=KEY_CTRL_ALPHA && get_custom_key_handler_state()==1) {
                  custom_key_to_handle = key; custom_key_to_handle_modifier = keyflag; return 3;
                }
                
        }
        return 0;
}

int get_custom_fkey_label(int key);
void dConsoleRedraw ()
{
        int i,j, iresult;
        Bdisp_AllClr_VRAM();
        GetFKeyPtr(0x046e, &iresult); // Catalog
        FKey_Display(0, (int*)iresult);
        GetFKeyPtr(0x03b7, &iresult); // LOAD
        FKey_Display(1, (int*)iresult);
        
        int l = get_custom_fkey_label(2);
        GetFKeyPtr(l, &iresult);
        FKey_Display(2, (int*)iresult);
        l = get_custom_fkey_label(3);
        GetFKeyPtr(l, &iresult);
        FKey_Display(3, (int*)iresult);
        
        GetFKeyPtr(0x0307, &iresult); // A<>a
        FKey_Display(4, (int*)iresult);
        
        l = get_custom_fkey_label(5);
        GetFKeyPtr(l, &iresult);
        FKey_Display(5, (int*)iresult);
        
        DisplayStatusArea();
        for(i=0,j=line_start;i<line_count;++i)
        {
                locate(1,i+1);print((uchar*)line_buf[j]);
                if (++j>=LINE_ROW_MAX) j = 0;
        }

}
void dConsolePutChar (char c)
{
        if (line_count == 0) line_count = 1;
        if (c != '\n')
                line_buf[line_index][line_x++] = c;
        else
        {
                line_buf[line_index][line_x] = '\0';
                line_x = LINE_COL_MAX;
        }
        if (line_x>=LINE_COL_MAX)
        {
                line_buf[line_index][line_x] = '\0';

                line_x = 0;
                if (line_count<LINE_ROW_MAX)
                {
                        ++line_count;
                }
                else
                {
                        line_start++;
                        if (line_start>=LINE_ROW_MAX) line_start = 0;
                }
                line_index++;
                if (line_index>=LINE_ROW_MAX) line_index = 0;
        }
        line_buf[line_index][line_x] = '\0';
        //dConsoleRedraw();
}

void dConsolePut(const char * str)
{
        if (line_count == 0) line_count = 1;
        for (;*str;++str)
        {
                if (*str != '\n')
                        line_buf[line_index][line_x++] = *str;
                else
                {
                        line_buf[line_index][line_x] = '\0';
                        line_x = LINE_COL_MAX;
                }
                if (line_x>=LINE_COL_MAX)
                {
                        line_buf[line_index][line_x] = '\0';

                        line_x = 0;
                        if (line_count<LINE_ROW_MAX)
                        {
                                ++line_count;
                        }
                        else
                        {
                                line_start++;
                                if (line_start>=LINE_ROW_MAX) line_start = 0;
                        }
                        line_index++;
                        if (line_index>=LINE_ROW_MAX) line_index = 0;
                }
        }
        line_buf[line_index][line_x] = '\0';
        //dConsoleRedraw();
}

void dPuts(const char * s)
{
        dConsolePut(s);
        dConsolePut("\n");
}

int dPrintf (const char * format,...)
{
        char    buf[256];
        int             length;
        va_list arg_list;

        va_start(arg_list,format);
        length = vsprintf(buf,format,arg_list);
        va_end(arg_list);

        dConsolePut (buf);
        
        return length;
} 
