#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include <crtdbg.h>
#include <direct.h>
#include "freeglut\include\GL\freeglut.h"
#include <gl/gl.h>

#include "prizmsim.h"

#define OutputLog(...) { char buffer[1024]; sprintf_s(buffer, 1024, __VA_ARGS__); OutputDebugString(buffer); }

HDC renderContext;
GLuint screenTexture;
HWND GWnd;
extern void DisplayGLUTScreen();
extern void GLUTExit();

typedef bool (APIENTRY *PFNWGLSWAPINTERVALEXTPROC) (int interval);

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
WNDPROC oldProc;

void glutErrorFunc(const char* format, va_list ap) {
	char toBuffer[512];
	vsnprintf(toBuffer, 512, format, ap);

	OutputDebugString("GLUT Error:");
	OutputDebugString(toBuffer);
}

void glutWarningFunc(const char* format, va_list ap) {
	char toBuffer[512];
	vsnprintf(toBuffer, 512, format, ap);

	OutputDebugString("GLUT Warning:");
	OutputDebugString(toBuffer);
}

void StartGlut() {
	int arg = 0;
	glutInit(&arg, NULL);

	glutInitErrorFunc(glutErrorFunc);
	glutInitErrorFunc(glutWarningFunc);

	glutInitWindowSize(384 * 2, 216 * 2);
	glutCreateWindow("Prizm Sim");
	glutDisplayFunc(DisplayGLUTScreen);
	glutInitDisplayMode(GLUT_RGBA);

	GWnd = FindWindow(NULL, "Prizm Sim");
	oldProc = (WNDPROC) GetWindowLong(GWnd, GWL_WNDPROC);
	SetWindowLong(GWnd, GWL_WNDPROC, (LONG) WndProc);

	glGenTextures(1, &screenTexture);
	glBindTexture(GL_TEXTURE_2D, screenTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 384, 216, 0, GL_RGB, GL_UNSIGNED_SHORT_5_6_5, NULL);

	renderContext = wglGetCurrentDC();
}

int main(int argc, char **argv) {
	char *filename = NULL;
	
	printf("Starting Windows Simulator...\n");

	// CRT reporting can be a little bonkers and exit without any indication to us:
	_CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_DEBUG);
	_CrtSetReportMode(_CRT_ASSERT, _CRTDBG_MODE_DEBUG);

	// heap check flags
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	StartGlut();

	int ret = simmain();

	extern void _CallQuit();
	_CallQuit();

	printf("Shutting down Windows Simulator...\n");
	return ret;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	extern void _CallQuit();

	switch(message) {
		case WM_CREATE:
			return 0;
			
		case WM_CLOSE:
			_CallQuit();
			PostQuitMessage(0);
			return 0;
			
		case WM_DESTROY:
			_CallQuit();
			return 0;
			
		case WM_KEYDOWN:
			return 0;
			
		default:
			return CallWindowProc(oldProc, hWnd, message, wParam, lParam);
	}
}
