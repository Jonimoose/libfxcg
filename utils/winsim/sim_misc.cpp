
/*******************************************************************
  Windows sim of various Prizm SDK functions to make life easier.
 *******************************************************************/

#include <stdio.h>
#include "freeglut\include\GL\freeglut.h"
#include "prizmsim.h"
#include "prizmfont.h"
#include <ShlObj.h>
#include "scope_timer.h"

 // DmaWaitNext() should be called before starting another Non-blocking DMA transfer or GetKey().
bool dma_transfer;

void PumpMessages() {
	MSG msg;
	for (;; ) {
		if (!PeekMessage(&msg, 0, 0, 0, PM_REMOVE)) {
			return;
		}
		if (msg.message == WM_QUIT) {
			exit(0);
			return;
		}
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// System

void(*_quitHandler)() = NULL;

void _CallQuit() {
	if (_quitHandler) {
		_quitHandler();
		_quitHandler = NULL;
	}
}

void SetQuitHandler(void(*handler)()) {
	_quitHandler = handler;
}

int RTC_GetTicks() {
	return (int)(GetTickCount64() * 16 / 125);
}

unsigned int ToBCD(unsigned int value) {
	unsigned int ret = 0;
	int curDigit = 1;
	while (value) {
		ret += curDigit * (value % 10);
		value /= 10;
		curDigit *= 16;
	}
	return ret;
}

void RTC_GetTime(unsigned int* hours, unsigned int* minutes, unsigned int* seconds, unsigned int* ms) {
	SYSTEMTIME curTime;
	GetLocalTime(&curTime);
	*hours = ToBCD(curTime.wHour);
	*minutes = ToBCD(curTime.wMinute);
	*seconds = ToBCD(curTime.wSecond);
	*ms = ToBCD(curTime.wMilliseconds);
}

void OS_InnerWait_ms(int ms) {
	PumpMessages();
	Sleep(ms);
}

void CMT_Delay_100micros(int units) {
	Sleep(units/100);
}

void CMT_Delay_micros(int units) {
	Sleep(units / 1000);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// Display Generics

struct AdvanceScopeTimer {
	LONGLONG start;
	bool isValid;

	AdvanceScopeTimer() {
		LARGE_INTEGER result;
		isValid = false;
		if (QueryPerformanceCounter(&result) != 0) {
			isValid = true;
			start = result.QuadPart;
		}
	}

	~AdvanceScopeTimer() {
		LARGE_INTEGER result;
		if (isValid && QueryPerformanceCounter(&result) != 0) {
			LONGLONG duration = result.QuadPart - start;
			ScopeTimer_Start += duration;
		}
	}
};

// the actual CPU side screen buffer
unsigned short VRAM[216][384] = { 0 };

unsigned short BackgroundVRAM[216][384] = { 0 };

void Bdisp_EnableColor(int n) {
	//  only true color currently supported
	Assert(n == 1);
}

void EnableStatusArea(int) {
	// ignored, no status area on win sim
}

void SaveVRAM_1() {
	memcpy(BackgroundVRAM, VRAM, sizeof(VRAM));
}

void LoadVRAM_1() {
	memcpy(VRAM, BackgroundVRAM, sizeof(VRAM));
}

void Bdisp_AllClr_VRAM() {
	memset(VRAM, 0xFF, sizeof(VRAM));

	// scope timer shouldn't could the display cost as this isn't similar to device at all with vsync
	AdvanceScopeTimer scoped;
	glutPostRedisplay();
	glutMainLoopEvent();
}

void Bdisp_Fill_VRAM(int color, int mode) {
	for (int y = 0; y < 216; y++) {
		for (int x = 0; x < 384; x++) {
			VRAM[y][x] = color;
		}
	}
}

void Bdisp_PutDisp_DD() {
	// scope timer shouldn't could the display cost as this isn't similar to device at all with vsync
	AdvanceScopeTimer scoped;
	glutPostRedisplay();
	glutMainLoopEvent();
}

void Bdisp_PutDisp_DD_stripe(int y1, int y2) {
	Bdisp_PutDisp_DD();
}

void DrawFrame(int color) {
	// does nothing
}

void *GetVRAMAddress(void) {
	return VRAM;
}

void *GetSecondaryVRAMAddress(void) {
	return BackgroundVRAM;
}

void Bdisp_AreaClr(struct display_fill* area, unsigned char target, unsigned short color) {
	unsigned targetColor = area->mode == 0 ? COLOR_WHITE : color;

	if (area->mode != 4) {
		for (int y = area->y1; y <= area->y2; y++) {
			for (int x = area->x1; x <= area->x2; x++) {
				VRAM[y][x] = targetColor;
			}
		}
	}
	else {
		for (int y = area->y1; y <= area->y2; y++) {
			for (int x = area->x1; x <= area->x2; x++) {
				VRAM[y][x] = ~(VRAM[y][x]);
			}
		}
	}
}

static const int minTable[24] =
{
	0, 0, 0, 0, 6, 6, 6, 12,
	18, 30, 42, 48, 60, 72, 84, 90,
	96, 102, 108, 114, 120, 126, 132, 138
};

#define MAX(a,b) (((a)>(b))?(a):(b))

// Actual OpenGL draw of CPU texture
void DisplayGLUTScreen() {
	static long long lastTicks = 0;
	// artificial "vsync", hold tab to fast forward
	if ((GetAsyncKeyState(VK_TAB) & 0x8000) == 0) {
		long long curTicks;
		while ((curTicks = GetTickCount64()) - 8 < lastTicks) {
			Sleep(0);
		}
		lastTicks = curTicks;
	}

	// process VRAM (to match TFT display, ideally this is done in a shader later
	unsigned char ProcessedDisplay[3 * 384 * 216];
	unsigned short* color = &VRAM[0][0];
	unsigned char* dstColor = &ProcessedDisplay[0];
	for (int x = 0; x < 384 * 216; x++, color++, dstColor += 3) {
		dstColor[0] = (*color & 0xF800) >> 8;
		dstColor[1] = (*color & 0x07E0) >> 3;
		dstColor[2] = (*color & 0x001F) << 3;

		int energy = MAX(MAX(dstColor[0] * 5 / 6, dstColor[1] * 4 / 3), dstColor[2] * 3 / 4) >> 4;
		int minColor = minTable[energy];
		for (int y = 0; y < 3; y++) {
			if (dstColor[y] < minColor) dstColor[y] = minColor;
		}

	}

	GLenum i = glGetError();
	glBindTexture(GL_TEXTURE_2D, screenTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 384, 216, GL_RGB, GL_UNSIGNED_BYTE, ProcessedDisplay);

	glPushMatrix();
	glLoadIdentity();
	glEnable(GL_TEXTURE_2D);
	glBegin(GL_QUADS);

	glTexCoord2f(0, 1);
	glVertex2f(-1, -1); // Upper left
	glTexCoord2f(1, 1);
	glVertex2f(1, -1); // Upper right
	glTexCoord2f(1, 0);
	glVertex2f(1, 1); // Lower right
	glTexCoord2f(0, 0);
	glVertex2f(-1, 1); // Lower left
	glEnd();
	glPopMatrix();

	glutSwapBuffers();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// Text Printing

#define TO_COLORREF_16(x) ( ((x & 0xF800) << 8) | ((x & 0x07E0) << 5) | ((x & 0x001F) << 3) )
#define TO_16_FROM_32(x) ((unsigned short) ( ((x & 0x00F80000) >> 19) | ((x & 0x0000FC00) >> 5) | ((x & 0x000000F8) << 8) ))

// returns width in pixels printed
static int PrintTextHelper(HFONT Font, int height, int x, int y, const char* string, int color, int back_color, bool transparent, int writeflag = 1) {
	HDC Compat = CreateCompatibleDC(wglGetCurrentDC());
	HBITMAP Bitmap = CreateCompatibleBitmap(renderContext, 384, 216);

	::SelectObject(Compat, Bitmap);
	::SelectObject(Compat, Font);

	SetTextColor(Compat, TO_COLORREF_16(color));
	SetBkColor(Compat, TO_COLORREF_16(back_color));

	RECT rect;
	rect.top = 0;
	rect.bottom = height;
	rect.left = 0;
	rect.right = 600;
	int h = DrawText(Compat, (LPCSTR)string, -1, &rect, DT_CALCRECT);
	h = DrawText(Compat, (LPCSTR)string, -1, &rect, DT_SINGLELINE);

	BITMAPINFO info[2] = { 0 };
	info[0].bmiHeader.biSize = sizeof(info[0].bmiHeader);
	GetDIBits(Compat, Bitmap, 0, 216, 0, &info[0], DIB_RGB_COLORS);
	unsigned int* rows = (unsigned int*)malloc(info[0].bmiHeader.biSizeImage);
	Assert(info[0].bmiHeader.biBitCount == 32);
	GetDIBits(Compat, Bitmap, 0, 216, (LPVOID)rows, &info[0], DIB_RGB_COLORS);

	if (writeflag) {
		// blit each row (have to y flip)
		for (int i = 0; i < rect.bottom; i++, y++) {
			if (y >= 0 && y <= 215) {
				for (int j = 0; j < rect.right; j++) {
					if (x + j < 384 && x + j >= 0) {
						if (!transparent || TO_16_FROM_32(rows[(384 * (215 - i) + j)]) != back_color) {
							VRAM[y][x + j] = TO_16_FROM_32(rows[(384 * (215 - i) + j)]);
						}
					}
				}
			}
		}
	}

	free(rows);

	DeleteObject(Bitmap);
	DeleteObject(Compat);

	return rect.right;
}

static int PrintPrizmFont(
	const int* widths,
	const char datas[][46],
	int height,
	int x,
	int y,
	const char* string,
	int color,
	int back_color,
	bool font_back_inverted,
	bool back_transparent,
	bool color_inverted,
	int writeflag = 1)
{
	bool warned = false;
	for (int s = 0; string[s]; s++)
	{
		char c = string[s];
		if ((unsigned)c >= 127)
		{
			if (!warned)
			{
				printf("Not supported: Printing text with (ascii >= 127).\n");
				warned = true;
			}
			x += height * 4 / 5;
		}
		else
		{
			if (writeflag)
			{
				if (color_inverted)
				{
					color = 0xFFFF - color;
					back_color = 0xFFFF - back_color;
				}
				if (back_transparent)
					back_color = -1;
				if (font_back_inverted)
				{
					int tmp = color;
					color = back_color;
					back_color = tmp;
				}
				char cur;
				int nextpos = 0;
				int rest = 0;
				for (int i = 0; i < widths[c]; i++)
				{
					if (x + i >= LCD_WIDTH_PX) break;
					for (int j = 0; j < height; j++)
					{
						if (y + j >= LCD_HEIGHT_PX) break;
						rest--;
						if (rest < 0)
						{
							cur = datas[c][nextpos++] - 48;
							rest = 5;
						}
						if (cur & 1)
						{
							if (color != -1)
								VRAM[y + j][x + i] = color;
						}
						else if (back_color != -1)
							VRAM[y + j][x + i] = back_color;
						cur >>= 1;
					}
				}
			}
			x += widths[c];
		}
	}
	return x;
}

static HFONT NormalFont = CreateFont(24, 18, 0, 0, FW_BOLD, 0, 0, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, NONANTIALIASED_QUALITY, FF_ROMAN, "Consolas");

void PrintMini(int *x, int *y, const char *MB_string, int mode_flags, unsigned int xlimit, int P6, int P7, int color, int back_color, int writeflag, int P11) {
	if (mode_flags & 0x02) back_color = -1;
	*x = PrintPrizmFont(
		widths_m,
		datas_m,
		18,
		*x,
		*y + ((mode_flags & 0x40) ? 0 : 24),
		MB_string,
		color,
		back_color,
		mode_flags & 0x01,
		mode_flags & 0x02,
		mode_flags & 0x04,
		writeflag);
}

void Bdisp_MMPrintRef(int *x, int *y, const char *string, int mode_flags, int xlimit, int P6, int P7, int color, int back_color, int writeflag, int P11) {
	if (mode_flags & 0x02) back_color = -1;
	*x = PrintPrizmFont(
		widths_16,
		datas_16,
		16,
		*x,
		*y + ((mode_flags & 0x40) ? 0 : 24),
		string,
		color,
		back_color,
		mode_flags & 0x01,
		mode_flags & 0x02,
		mode_flags & 0x04,
		writeflag);
}

void Bdisp_MMPrint(int x, int y, const char *string, int mode_flags, int xlimit, int P6, int P7, int color, int back_color, int writeflag, int P11) {
	if (mode_flags & 0x02) back_color = -1;
	PrintPrizmFont(
		widths_16,
		datas_16,
		16,
		x,
		y + ((mode_flags & 0x40) ? 0 : 24),
		string,
		color,
		back_color,
		mode_flags & 0x01,
		mode_flags & 0x02,
		mode_flags & 0x04,
		writeflag);
}

void PrintMiniMini(int *x, int *y, const char *MB_string, int mode1, char color, int mode2) {
	static int color_map[] =
	{
		COLOR_BLACK,
		COLOR_BLUE,
		COLOR_LIME,
		COLOR_CYAN,
		COLOR_RED,
		COLOR_MAGENTA,
		COLOR_YELLOW,
		COLOR_WHITE
	};

	*x = PrintPrizmFont(
		(mode1 & 0x10) ? widths_mmb : widths_mm,
		(mode1 & 0x10) ? datas_mmb : datas_mm,
		10,
		*x,
		*y + ((mode1 & 0x40) ? 0 : 24),
		MB_string,
		color_map[color],
		COLOR_WHITE,
		mode1 & 0x01,
		mode1 & 0x02,
		mode1 & 0x04,
		!mode2);
}

void PrintCXY(int x, int y, const char *cptr, int mode_flags, int P5, int color, int back_color, int P8, int P9) {
	PrintTextHelper(NormalFont, 24, x, y + 24, cptr, color, back_color, mode_flags | TEXT_MODE_TRANSPARENT_BACKGROUND);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// Keyboard handling

struct VKeyMapping {
	DWORD virtualKey;
	unsigned short getKeyCode;
	unsigned char fastKeyCode;
};

// Mapped virtual keys:
#define NUM_KEY_MAPS 60
static VKeyMapping keys[NUM_KEY_MAPS] = {
//  F1-F6 - F1-F6
	{ VK_F1,			KEY_CTRL_F1, 79 },
	{ VK_F2,			KEY_CTRL_F2, 69 },
	{ VK_F3,			KEY_CTRL_F3, 59 },
	{ VK_F4,			KEY_CTRL_F4, 49 },
	{ VK_F5,			KEY_CTRL_F5, 39 },
	{ VK_F6,			KEY_CTRL_F6, 29 },
//  Directional Arrows - Arrow Keys
	{ VK_LEFT,			KEY_CTRL_LEFT, 38 },
	{ VK_RIGHT,			KEY_CTRL_RIGHT, 27 },
	{ VK_UP,			KEY_CTRL_UP, 28 },
	{ VK_DOWN,			KEY_CTRL_DOWN, 37 },
//  SHIFT/OPTN/VARS/MENU - ASDF 
	{ 'A',				KEY_CTRL_SHIFT, 78 },
	{ 'S',				KEY_CTRL_OPTN, 68 },
	{ 'D',				KEY_CTRL_VARS, 58 },
	{ 'F',				KEY_CTRL_MENU, 48 },
//  ALPHA/X^2/^/EXIT - ZXCV
	{ 'Z',				KEY_CTRL_ALPHA, 77 },
	{ 'X',				KEY_CHAR_SQUARE, 67 },
	{ 'C',				KEY_CHAR_POW, 57 },
	{ 'V',				KEY_CTRL_EXIT, 47 },
//  Row of X.theta.tan - TYUIOP
	{ 'T',				KEY_CTRL_XTT, 76 },
	{ 'Y',				KEY_CHAR_LOG, 66 },
	{ 'U',				KEY_CHAR_LN, 56 },
	{ 'I',				KEY_CHAR_SIN, 46 },
	{ 'O',				KEY_CHAR_COS, 36 },
	{ 'P',				KEY_CHAR_TAN, 26 },
//  Row of Ab/c - GHJKL;
	{ 'G',				KEY_CTRL_MIXEDFRAC, 75 },
	{ 'H',				KEY_CTRL_FD, 65 },
	{ 'J',				KEY_CHAR_LPAR, 55 },
	{ 'K',				KEY_CHAR_RPAR, 45 },
	{ 'L',				KEY_CHAR_COMMA, 35 },
	{ VK_OEM_1,			KEY_CHAR_STORE, 25 },		// semicolon
//  Numpad maps to bottom numpad
	{ VK_NUMPAD0,		KEY_CHAR_0, 71 },
	{ VK_NUMPAD1,		KEY_CHAR_1, 72 },
	{ VK_NUMPAD2,		KEY_CHAR_2, 62 },
	{ VK_NUMPAD3,		KEY_CHAR_3, 52 },
	{ VK_NUMPAD4,		KEY_CHAR_4, 73 },
	{ VK_NUMPAD5,		KEY_CHAR_5, 63 },
	{ VK_NUMPAD6,		KEY_CHAR_6, 53 },
	{ VK_NUMPAD7,		KEY_CHAR_7, 74 },
	{ VK_NUMPAD8,		KEY_CHAR_8, 64 },
	{ VK_NUMPAD9,		KEY_CHAR_9, 54 },
	{ VK_OEM_MINUS,		KEY_CHAR_MINUS, 32 },
	{ VK_OEM_PLUS,		KEY_CHAR_PLUS, 42 },
	{ VK_MULTIPLY,		KEY_CHAR_MULT, 43 },
	{ VK_DIVIDE,		KEY_CHAR_DIV, 33 },
	{ VK_OEM_PERIOD,	KEY_CHAR_DP, 61 },
//  DEL - Delete
	{ VK_DELETE,		KEY_CTRL_DEL, 44 },
//  EXP - Home
	{ VK_HOME,			KEY_CHAR_EXP , 51},
//  (-) - End
	{ VK_END,			KEY_CHAR_PMINUS, 41 },
//  EXE - Enter
	{ VK_RETURN,		KEY_CTRL_EXE, 31 },
//  AC/On - Escape (though this will exit the program on simulator anyway)
	{ VK_ESCAPE,		KEY_CTRL_AC, 10 },
	// number row
	{ '0',		KEY_CHAR_0, 71 },
	{ '1',		KEY_CHAR_1, 72 },
	{ '2',		KEY_CHAR_2, 62 },
	{ '3',		KEY_CHAR_3, 52 },
	{ '4',		KEY_CHAR_4, 73 },
	{ '5',		KEY_CHAR_5, 63 },
	{ '6',		KEY_CHAR_6, 53 },
	{ '7',		KEY_CHAR_7, 74 },
	{ '8',		KEY_CHAR_8, 64 },
	{ '9',		KEY_CHAR_9, 54 },
};

int GetKey(int* key) {
	if (dma_transfer)
		printf("Warning: You should call DmaWaitNext() before GetKey() when using Non-blocking DMA.\n");
	int result = 0;
	while (!result) {
		for (int i = 0; i < NUM_KEY_MAPS; i++) {
			if ((GetFocus() == GWnd && GetAsyncKeyState(keys[i].virtualKey) & 0x8000) != 0) {
				result = keys[i].getKeyCode;
				break;
			}
		}
		if (!result) {
			glutPostRedisplay();
			glutMainLoopEvent();
		}
	}

	// wait for no keys to be up
	bool up = false;
	while (!up) {
		up = true;
		for (int i = 0; i < NUM_KEY_MAPS; i++) {
			if ((GetAsyncKeyState(keys[i].virtualKey) & 0x8000) != 0) {
				up = false;
				break;
			}
			if (!up) {
				glutPostRedisplay();
				glutMainLoopEvent();
			}
		}
	};

	if (result == KEY_CTRL_AC) {
//		PostQuitMessage(0);
	}

	*key = result;
	return result >= 30000 ? 0 : 1;
}

extern "C" {
	bool keyDown_fast(unsigned char keycode) {
		static int iter = 0;
		if (iter++ % 1024 == 0) {
			PumpMessages();
		}

		for (int i = 0; i < NUM_KEY_MAPS; i++) {
			if (keys[i].fastKeyCode == keycode) {
				if ((GetFocus() == GWnd && GetAsyncKeyState(keys[i].virtualKey) & 0x8000) != 0) {
					return true;
				} else {
					return false;
				}
			}
		}

		return false;
	}
};

///////////////////////////////////////////////////////////////////////////////////////////////////
// MessageBox

bool msg_box_pushed = false;

const int bx = 11;
const int by = 33;
const int bw = 362;
const int bh = 169;
color_t temp_VRAM[bh][bw];

void MsgBoxPush(int lines)
{
	if (lines < 1 || lines > 6)
		printf("Warning: MsgBox lines should between 1 & 6, inclusive.\n");
	if (msg_box_pushed)
		printf("Warning: Need to call MsgBoxPop() before MsgBoxPush().\n");
	for (int j = 0; j < bh; j++)
		for (int i = 0; i < bw; i++)
			temp_VRAM[j][i] = VRAM[by + j][bx + i];
	msg_box_pushed = true;

	int dx = 27;
	int dy = 33 + (5 - lines) / 2 * 24;
	int w = 331;
	int h = lines * 24 + 25;

	for (int j = 0; j < h; j++)
		for (int i = 0; i < w; i++)
		{
			color_t c = COLOR_WHITE;
			if (i == 0 || j == 0 || i == w - 1 || j == h - 1)
				c = COLOR_BLACK;
			else if (i == 1 || j == 1)
				c = COLOR_WHITE;
			else if (i == w - 2 || j == h - 2)
				c = COLOR_BLACK;
			else if ((i < 6 || i >= w - 6) || (j < 6 || j >= h - 6))
				c = COLOR_BLUE;
			else if (i == w - 7 || j == h - 7)
				c = COLOR_WHITE;
			else if ((i < 8 || i == w - 8) || (j < 8 || j == h - 8))
				c = COLOR_BLACK;
			VRAM[dy + j][dx + i] = c;
		}
}

void MsgBoxPop()
{
	if (!msg_box_pushed)
		printf("Warning: Must call MsgBoxPush() before MsgBoxPop() (System crash).");

	for (int j = 0; j < bh; j++)
		for (int i = 0; i < bw; i++)
			VRAM[by + j][bx + i] = temp_VRAM[j][i];

	msg_box_pushed = false;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// Miscellaneous

void Bfile_NameToStr_ncpy(char*dest, const unsigned short*src, size_t n) {
	WideCharToMultiByte(CP_ACP, 0, (LPWSTR)src, -1, dest, 256, NULL, NULL);
}

void Bfile_StrToName_ncpy(unsigned short *dest, const char *source, size_t n) {
	MultiByteToWideChar(CP_ACP, 0, source, -1, (LPWSTR)dest, 256);
}