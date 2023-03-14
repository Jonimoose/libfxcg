#pragma once

/*******************************************************************
 FXCG libraries are copied to the winsim directories to avoid 
 conflicts with window standard lib functionality
*******************************************************************/

// main function should be titled simmain for WINSIM
extern "C" {
	int simmain(void);
}

extern void (*_quitHandler)();

#define Assert(x) if (!(x)) { MessageBox(NULL, "Assertion failed: " #x, "Error", MB_OK); }

#include "fxcg\display.h"
#include "fxcg\keyboard.h"
#include "fxcg\file.h"
#include "fxcg\registers.h"
#include "fxcg\rtc.h"
#include "fxcg\system.h"

// non waiting version of GetKey for ease of use with windows simulator
int GetKey_SimFast(int keycode);

#ifndef GL_UNSIGNED_SHORT_5_6_5
#define GL_UNSIGNED_SHORT_5_6_5 0x8363
#endif

extern HDC renderContext;
extern GLuint screenTexture;
extern HWND GWnd;