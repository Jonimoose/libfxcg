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
#include "textGUI.hpp"
extern int esc_flag;
extern int run_startup_script_again;
int execution_in_progress = 0;
int custom_key_to_handle;
int custom_key_to_handle_modifier;
int graphic_mode = 0;
static char expr[INPUTBUFLEN];

void run_script(char* filename);
void run_startup_script();
void check_execution_abort();
void select_script_and_run();
void select_strip_script();
void custom_key_handler();
void script_recorder();
void input_eval_loop(int isRecording);
int is_running_in_strip();
#define DIRNAME (unsigned char*)"@EIGEN"
#define SCRIPTFILE (unsigned char*)"Script"
static int aborttimer = 0;
int
main()
{
  Bdisp_AllClr_VRAM();
  Bdisp_EnableColor(1); 
  printf("Welcome to Eigenmath\n");
  printf("To see version information,\npress Shift then Menu.\n");
  run_startup_script(); 
  aborttimer = Timer_Install(0, check_execution_abort, 100);
  if (aborttimer > 0) { Timer_Start(aborttimer); }
  //in case we're running in a strip, check if this strip has a script to run.
  if(is_running_in_strip()) {
    int MCSsize;
    MCSGetDlen2(DIRNAME, SCRIPTFILE, &MCSsize);
    if (MCSsize > 0) {
      // there is a script to run...
      unsigned char* asrc = (unsigned char*)alloca(MCSsize*sizeof(unsigned char)+5); // 5 more bytes to make sure it fits...
      MCSGetData1(0, MCSsize, asrc); // read script from MCS
      execution_in_progress = 1;
      run((char*)asrc);
      execution_in_progress = 0;
    }
  }
  input_eval_loop(0);
}

void input_eval_loop(int isRecording) {
  char** recHistory = NULL; int curRecHistEntry = 0;
  if(isRecording) recHistory = (char**)alloca(200); // space for 200 pointers to history entries
  while (1) {
    DefineStatusMessage((char*)"", 1, 0, 0);
    strcpy(expr, (char*)"");
    printf(">");
    dConsoleRedraw();
    int res = gets(expr,INPUTBUFLEN);
    if(res == 2) {
      dConsolePut("\n");
      select_script_and_run();
      continue;
    }
    if(res == 4) {
      dConsolePut("\n");
      select_strip_script();
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
    } else if(strcmp(expr, "record") == 0) {
      if(!isRecording) script_recorder();
      else {
        // create and save a script. this must be done here, because we used alloca
        // the "clean" way would be using malloc&free, but on the Prizm the heap is already being heavily used by the Eigenmath core.
        if(curRecHistEntry == 0) {
          printf("Nothing to record.\n");
          return;
        }
        printf("Recording stopped.\n");
        printf("Type a name for the script, or\n");
        printf("leave empty to discard.\n");
        printf(":");
        char inputname[MAX_FILENAME_SIZE+1] = "";
        gets(inputname,MAX_FILENAME_SIZE-50);
        puts(inputname);
        if(strcmp(inputname, (char*)"") == 0) {
          // user aborted
          printf("Recording discarded.\n");
          return;
        }
        if (aborttimer > 0) {
          Timer_Stop(aborttimer);
          Timer_Deinstall(aborttimer);
        }
        char filename[MAX_FILENAME_SIZE+1] = "";
        strcpy(filename, "\\\\fls0\\");
        strcat(filename, inputname);
        strcat(filename, ".txt");
        unsigned short pFile[MAX_FILENAME_SIZE+1];
        Bfile_StrToName_ncpy(pFile, (unsigned char*)filename, strlen(filename)+1);
        // calculate size
        int size = 0;
        int maxHistory = curRecHistEntry - 1; //because we ++'ed at the end of last addition
        for(int i=0; i <= maxHistory; i++) {
          size = size + strlen(recHistory[i]) + 1; // 1 byte for \n. we will use unix line termination
        }
        int BCEres = Bfile_CreateEntry_OS(pFile, CREATEMODE_FILE, &size);
        if(BCEres >= 0) // Did it create?
        {
          BCEres = Bfile_OpenFile_OS(pFile, READWRITE, 0); // Get handle
          for(int i=0; i <= maxHistory; i++) {
            char* buf = (char*)alloca(strlen(recHistory[i])+5);
            strcpy(buf, recHistory[i]);
            strcat(buf, (char*)"\n");
            Bfile_WriteFile_OS(BCEres, buf, strlen(recHistory[i])+1);
          }
          Bfile_CloseFile_OS(BCEres);
          printf("Script created.\n");
        } else {
          printf("An error occurred when creating the script for recording.\n");
        }
        aborttimer = Timer_Install(0, check_execution_abort, 100);
        if (aborttimer > 0) Timer_Start(aborttimer);
        return;
      }
    } else {
      if(graphic_mode) Bdisp_AllClr_VRAM();
      execution_in_progress = 1;
      run(expr);
      // run_startup_script cannot run from inside eval_clear because then it would be a run() inside a run()
      if(run_startup_script_again) { run_startup_script_again = 0; run_startup_script(); }
      execution_in_progress = 0;
      
      // if recording, add input to record
      if(isRecording && curRecHistEntry <= 200) {
        recHistory[curRecHistEntry] = (char*)alloca(strlen(expr)+2); // 2 bytes for security
        strcpy(recHistory[curRecHistEntry], expr);
        curRecHistEntry++;
      }
      
      if(graphic_mode) {
        int key;
        GetKey(&key);
      }
    }
  }
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

void select_strip_script() {
  if(!is_running_in_strip()) {
    printf("This function is only available\n");
    printf("when running as an eActivity\n");
    printf("strip.\n");
    return;
  }
  
  textArea text;
  strcpy(text.title, (char*)"aa");
  text.showtitle=0;

  textElement elem[10];
  text.elements = elem;
  
  elem[0].text = (char*)"This function lets you run a script when this eActivity strip is opened.";
  elem[1].newLine = 1;
  elem[1].text = (char*)"When sharing the eActivity file, it will not be necessary to share any other files - the script is included in the eActivity.";
  elem[2].newLine = 1;
  elem[2].text = (char*)"You will be informed if the script is too big to fit inside the eActivity file.";
  elem[3].newLine = 1;
  elem[3].text = (char*)"Press EXIT now to continue and select a script.";
  text.numelements = 4;
  doTextArea(&text);
  
  char filename[MAX_FILENAME_SIZE+1] = "";
  if(fileBrowser(filename, (unsigned char*)"*.txt", "Scripts")) {
    // get the size of the selected script on SMEM.
    // get free size on the "MCS" of the strip we're running on and see if the script fits
    unsigned short pFile[MAX_FILENAME_SIZE+1];
    Bfile_StrToName_ncpy(pFile, (unsigned char*)filename, strlen(filename)+1); 
    int hFile = Bfile_OpenFile_OS(pFile, READWRITE, 0); // Get handle
    if(hFile >= 0) // Check if it opened
    { //opened
      unsigned int filesize = Bfile_GetFileSize_OS(hFile);  
      //get free size of MCS
      int MCSmaxspace; int MCScurrentload; int MCSfreespace;  
      MCS_GetState( &MCSmaxspace, &MCScurrentload, &MCSfreespace );
      if((int)filesize < MCSfreespace - 50) { // 50 bytes for any headers and the like
        // fits, copy selected script to MCS
        unsigned char* scontents = (unsigned char*)alloca(filesize);
        memset(scontents, filesize, 0);
        int rsize = Bfile_ReadFile_OS(hFile, scontents, filesize, 0);
        scontents[rsize]='\0';
        // script is now in buffer scontents
        // write it to the "MCS"
        int createResult = MCS_CreateDirectory( DIRNAME );
        if(createResult != 0) // Check directory existence
        { // directory already exists, so delete the exiting file that may be there
          MCSDelVar2(DIRNAME, SCRIPTFILE);
        }
        MCSPutVar2(DIRNAME, SCRIPTFILE, rsize, scontents);
        printf("Script set successfully.\n");
      } else {
        printf("The script is too big to be\n");
        printf("included in the eActivity.\n");
      }
      Bfile_CloseFile_OS(hFile);
      return; // don't get to the error message
    }
  }
  printf("There was a problem setting the script for this strip.\n");
}
char curRecordingBuffer[MAX_TEXTVIEWER_FILESIZE+5];

void script_recorder() {
  printf("Recording started: every\n");
  printf("command you enter from on now\n");
  printf("will be recorded, so that you\n");
  printf("can create a script.\n");
  printf("When you're done recording,\n");
  printf("call \"record\" again.\n");
  input_eval_loop(1);
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

int is_running_in_strip() {
  // uses a hack to detect if we're running from an eActivity strip:
  // if the first pixel of the VRAM (first pixel of the status bar) is not white, then we are in a strip
  // (eActivity turns the statusbar background into a checkerbox when in a strip,
  // unless our code messes with the statusbar flags, which this add-in doesn't do.
  // the first pixel of the checkerboard is not white)
  DisplayStatusArea(); // otherwise, we don't know
  if(Bdisp_GetPoint_VRAM( 0, 0 ) != COLOR_WHITE) return 1;
  else return 0;
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
  graphic_mode = !graphic_mode; // shameless plug: use the "draw" command to enable graphic_mode      
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