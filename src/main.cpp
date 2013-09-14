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

#include "defs.h"
extern "C" {
#include "dConsole.h"
}
#include "fileGUI.hpp"
extern int esc_flag;
extern int run_startup_script_again;
int execution_in_progress = 0;
int custom_key_to_handle;
int custom_key_to_handle_modifier;
static char expr[INPUTBUFLEN];

void run_script(char* filename);
void run_startup_script();
void check_execution_abort();
void select_script_and_run();
void custom_key_handler();

int
main()
{
  Bdisp_AllClr_VRAM();
  Bdisp_EnableColor(1); 
  printf("Welcome to Eigenmath\n");
  printf("To see version information,\npress Shift then Menu.\n");
  run_startup_script(); 
  int aborttimer = Timer_Install(0, check_execution_abort, 100);
  if (aborttimer > 0) { Timer_Start(aborttimer); }
  while (1) {
    strcpy(expr, (char*)"");
    printf(">");
    dConsoleRedraw();
    int res = gets(expr,INPUTBUFLEN);
    if(res == 2) {
      dConsolePut("\n");
      select_script_and_run();
      continue;
    }
    if(res == 3) {
      dConsolePut("\n");
      custom_key_handler();
      if(run_startup_script_again) { run_startup_script_again = 0; run_startup_script(); }
      continue;
    }
    puts(expr);
    update_cmd_history(expr);
    dConsoleRedraw();
    if(strcmp(expr, "testmode") == 0) {
      TestMode(1);
    } else if(strcmp(expr, "meminfo") == 0) {
      print_mem_info();
    } else if(strcmp(expr, "memgc") == 0) {
      gc();
    } else {
      //printf("=");
      //HourGlass();
      execution_in_progress = 1;
      run(expr);
      // run_startup_script cannot run from inside eval_clear because then it would be a run() inside a run()
      if(run_startup_script_again) { run_startup_script_again = 0; run_startup_script(); }
      execution_in_progress = 0;
    }
  }

        /*for (;;) {
                printf("> ");
                fgets(buf, BUFLEN, stdin);
                run(buf);
        }*/
}
void run_script(char* filename) {
  unsigned short pFile[MAX_FILENAME_SIZE+1];
  Bfile_StrToName_ncpy(pFile, (unsigned char*)filename, strlen(filename)+1); 
  int hFile = Bfile_OpenFile_OS(pFile, READWRITE, 0); // Get handle
  
  // Check for file existence
  if(hFile >= 0) // Check if it opened
  {
    // Returned no error, file exists, open it
    int size = Bfile_GetFileSize_OS(hFile);
    // File exists and has size 'size'
    // Read file into a buffer
    if ((unsigned int)size > MAX_TEXTVIEWER_FILESIZE) {
      Bfile_CloseFile_OS(hFile);
      printf("Stop: script too big\n");
      return; //file too big, return
    }
    unsigned char* asrc = (unsigned char*)alloca(size*sizeof(unsigned char)+5); // 5 more bytes to make sure it fits...
    memset(asrc, size+5, 0); //alloca does not clear the allocated space. Make sure the string is null-terminated this way.
    int rsize = Bfile_ReadFile_OS(hFile, asrc, size, 0);
    Bfile_CloseFile_OS(hFile); //we got file contents, close it
    asrc[rsize]='\0';
    execution_in_progress = 1;
    run((char*)asrc);
    execution_in_progress = 0;
  }
}
void run_startup_script() {
  char filename[128] = "";
  strcpy(filename, (char*)"\\\\fls0\\eigensup.txt");
  run_script(filename);
}
void select_script_and_run() {
  char filename[MAX_FILENAME_SIZE+1] = "";
  if(fileBrowser(filename, (unsigned char*)"*.txt", "Scripts")) {
    run_script(filename);
  }
}

void check_execution_abort() {
  if(execution_in_progress) {
    HourGlass();
    short unsigned int key = PRGM_GetKey();
    if(key == KEY_PRGM_ACON) esc_flag = 1;
  }
}
int get_custom_key_handler_state() {
  U* tmp = usr_symbol("prizmUIhandleKeys");
  if (!issymbol(tmp)) return 0;
  tmp = get_binding(tmp);
  if(isnonnegativeinteger(tmp)) {
    return !iszero(tmp);
  } else return 0;
}
void custom_key_handler() {
  char buf[100] = "";
  sprintf(buf, "prizmUIkeyHandler(%d,%d)", custom_key_to_handle, custom_key_to_handle_modifier);
  run(buf);
}
int get_custom_fkey_label(int fkey) {
  U* tmp;
  if(fkey==2) {
    tmp = usr_symbol("prizmUIfkey3label");
  } else if (fkey==3) {
    tmp = usr_symbol("prizmUIfkey4label");
  } else if (fkey==5) {
    tmp = usr_symbol("prizmUIfkey6label");
  } else return 0;
  if (issymbol(tmp)) {
    tmp = get_binding(tmp);
    if(isnonnegativeinteger(tmp)) {
      return *tmp->u.q.a;
    }
  }
  return 0;
}

void
clear_term()
{
  // user issued "clear" command.
  dConsoleCls();
}

extern void eval_print(void);

void
eval_display(void)
{
        eval_print();
}

void
printstr(char *s)
{
        while (*s)
                printchar(*s++);
}

extern int test_flag;

void
printchar(int c)
{
        dConsolePutChar(c);
}

void
printchar_nowrap(int c)
{
        printchar(c);
}

void
eval_draw(void)
{
        push(symbol(NIL));
}

void
eval_sample(void)
{
}

// Command history related:
char *
get_curr_cmd(void)
{
        int i, len;
        char *s;

        len=strlen(expr);
        s = (char*)malloc(len+1);
        strcpy(s, expr);

        // trim trailing spaces

        for (i = len - 1; i >= 0; i--) {
                if (isspace(s[i]))
                        s[i] = 0;
                else
                        break;
        }

        return s;
}
void
update_curr_cmd(char *s)
{
        strcpy(expr, s);
}