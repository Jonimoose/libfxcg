
// ScopeTimer not supported on windows simulator or non-Debug
#if DEBUG

#include "scope_timer/scope_timer.h"

#include "calctype/calctype.h"
#include "calctype/fonts/arial_small/arial_small.h"	

#if TARGET_WINSIM
#include <Windows.h>
unsigned int ScopeTimer_FrameCycles = 0;
LONGLONG ScopeTimer_Start = 0;
#else
#include "ptune2_simple/Ptune2_direct.h"
#include <string.h>
#endif

#include <stdio.h>
#include "fxcg/display.h"
#include "fxcg/keyboard.h"

ScopeTimer* ScopeTimer::firstTimer = nullptr;
char ScopeTimer::debugString[128] = { 0 };
int ScopeTimer::fpsValue = 0;

static unsigned int lastFrame = 0;

unsigned int GetCycleFrameTime() {
#if TARGET_PRIZM
	return Ptune2_GetPLLFreq() * 235 * 256 >> Ptune2_GetPFCDiv();
#else
	return ScopeTimer_FrameCycles >> 2;
#endif
}

ScopeTimer::ScopeTimer(const char* withFunctionName, int withLine) : cycleCount(0), numCounts(0), funcName(withFunctionName), line(withLine) {
	// put me in the linked list
	nextTimer = firstTimer;
	firstTimer = this;
}

void ScopeTimer::Register(const char* withFunctionName) {
	// enforce registration if we have an init order problem
	ScopeTimer* curTimer = firstTimer;
	while (curTimer) {
		if (curTimer == this) {
			return;
		}
		curTimer = curTimer->nextTimer;
	}

	// put me back in the linked list
	nextTimer = firstTimer;
	firstTimer = this;
	cycleCount = 0;
	numCounts = 0;
	funcName = withFunctionName;
}

void ScopeTimer::InitSystem() {
	// initialize TMU2 at the fastest clock rate possible

#if TARGET_PRIZM
	// disable so we can set up
	REG_TMU_TSTR &= ~(1 << 2);

	REG_TMU_TCOR_2 = 0x7FFFFFFF; // max int
	REG_TMU_TCNT_2 = 0x7FFFFFFF;
	REG_TMU_TCR_2 = 0x0000; // no interrupt needed

	// enable TMU2
	REG_TMU_TSTR |= (1 << 2);
#endif

#if TARGET_WINSIM
	// 
	LARGE_INTEGER result;
	QueryPerformanceFrequency(&result);
	ScopeTimer_FrameCycles = (unsigned int) (result.QuadPart / 60);
	QueryPerformanceCounter(&result);
	ScopeTimer_Start = result.QuadPart;
#endif

	memset(debugString, 0, sizeof(debugString));

	// clear timers
	ScopeTimer* curTimer = firstTimer;
	while (curTimer) {
		curTimer->cycleCount = 0;
		curTimer->numCounts = 0;
		curTimer = curTimer->nextTimer;
	}
}

void ScopeTimer::ReportFrame() {
	unsigned int curFrame = GetCycles();
	if (lastFrame > curFrame && lastFrame != 0) {

		static int cycleRoll[20] = { 0 };
		static int curCycle = 0;
		cycleRoll[curCycle++] = lastFrame - curFrame;
		if (curCycle >= 20) curCycle = 0;

		int totalCycles = 0;
		for (int i = 0; i < 20; i++) {
			totalCycles += cycleRoll[i];
		}

		fpsValue = GetCycleFrameTime() * 100 / (totalCycles / 1200);
	}

	lastFrame = curFrame;
}

void ScopeTimer::Shutdown() {
#if TARGET_PRIZM
	// disable TMU 2
	REG_TMU_TSTR &= ~(1 << 2);
#endif
}

static void PrintInfo(int row, const char* label, const char* info, unsigned short color) {
	// location of columns
	const int col1 = 0;
	const int col2 = 200;

	int x = col1;
	int y = row * 15 + 2;
	CalcType_Draw(&arial_small, label, x, y, color, 0, 0);

	x = col2;
	y = row * 15 + 2;
	CalcType_Draw(&arial_small, info, x, y, color, 0, 0);
}

void ScopeTimer::FPSToBuffer(char* buffer) {
	sprintf(buffer, "FPS: %d.%d%d", fpsValue / 100, (fpsValue % 100) / 10, fpsValue % 10);
}

void ScopeTimer::DisplayTimes() {
	// display debug view with get key
	int toKey;
	unsigned int maxCycles = 0;
	unsigned int maxCount = 0;

#if TARGET_PRIZM
	// disable TMU 2
	REG_TMU_TSTR &= ~(1 << 2);
#endif

	// mode descriptions
	const int numModes = 4;
	const char* modeNames[numModes] = {
		"% Max",
		"Num Cycles",
		"Num Hits",
		"Cycles/Hit"
	};

	// reset display area just in case
	Bdisp_PutDisp_DD();

	// allocate number of timers in use, support up to 256
	int numTimers = 0;
	ScopeTimer* timers[256];
	{
		ScopeTimer* curTimer = firstTimer;
		// determine maxCycles and maxCount first
		while (curTimer && numTimers < 256) {
			if (curTimer->cycleCount > maxCycles) {
				maxCycles = curTimer->cycleCount;
			}
			if (curTimer->numCounts > maxCount) {
				maxCount = curTimer->numCounts;
			}

			curTimer = curTimer->nextTimer;
		}

		// exclude timers with less than 0.1% of the total cycles
		curTimer = firstTimer;
		while (curTimer && numTimers < 256) {
			if (curTimer->cycleCount > maxCycles / 1024) {
				timers[numTimers] = curTimer;
				numTimers++;
			}
			curTimer = curTimer->nextTimer;
		}

		// then add the timers that are left that fit
		curTimer = firstTimer;
		while (curTimer && numTimers < 256) {
			if (curTimer->cycleCount <= maxCycles / 1024) {
				timers[numTimers] = curTimer;
				numTimers++;
			}
			curTimer = curTimer->nextTimer;
		}

		// now bubble sort by cycle count
		for (int i = 0; i < numTimers - 1; i++) {
			for (int j = i + 1; j < numTimers; j++) {
				if (timers[i]->cycleCount < timers[j]->cycleCount) {
					ScopeTimer* swapTimer = timers[i];
					timers[i] = timers[j];
					timers[j] = swapTimer;
				}
			}
		}
	}

	int startTimer = 0;
	int mode = 0;

	do {
		// create stat display
		Bdisp_Fill_VRAM(0x0000, 3);

		// header
		PrintInfo(0, "Function(line)", modeNames[mode], COLOR_WHITE);

		// notify if there are no timers
		if (numTimers == 0) {
			PrintInfo(1, "No timers found!", "ERROR", COLOR_RED);
		}

		for (int timer = startTimer, row = 1; timer < numTimers && row < 12; timer++, row++) {
			ScopeTimer* curTimer = timers[timer];

			char name[256];
			memset(name, 0, sizeof(name));
			sprintf(name, "%s(%d)", curTimer->funcName, curTimer->line);

			bool isMax;
			char info[256];
			memset(info, 0, sizeof(info));
			switch (mode) {
				case 0:
				{
					int percent = curTimer->cycleCount / (maxCycles / 1000);
					sprintf(info, "%d.%d", percent / 10, percent % 10);
					isMax = curTimer->cycleCount == maxCycles;
					break;
				}
				case 1:
					sprintf(info, "%dk", curTimer->cycleCount / 1024);
					isMax = curTimer->cycleCount == maxCycles;
					break;
				case 2:
					sprintf(info, "%dk", curTimer->numCounts / 1024);
					isMax = curTimer->numCounts == maxCount;
					break;
				case 3:
					if (curTimer->numCounts) {
						// 3 digits
						unsigned int cycleCount = curTimer->cycleCount;
						unsigned int numCounts = curTimer->numCounts;

						// handle overflow
						while (cycleCount > 2 * 1024 * 1024) {
							cycleCount /= 16;
							numCounts /= 16;
						}

						unsigned int result = cycleCount * 1000 / numCounts;
						unsigned int dec0 = (result % 1000) / 100;
						unsigned int dec1 = (result % 100) / 10;
						unsigned int dec2 = (result % 10);

						sprintf(info, "%d.%d%d%d", result / 1000, dec0, dec1, dec2);
					} else
						strcpy(info, "N/A");
					isMax = false;
					break;
			}
			PrintInfo(row, name, info, isMax ? COLOR_SALMON : COLOR_LIGHTGREEN);
		}

		if (debugString[0]) {
			PrintInfo(12, debugString, "Nav: Arrows, Leave: EXIT", COLOR_WHITE);
		} else {
			PrintInfo(12, "Nav: Arrows", "Leave: EXIT", COLOR_WHITE);
		}

		char fpsBuffer[50] = { 0 };
		FPSToBuffer(fpsBuffer);
		PrintInfo(13, fpsBuffer, "F3: Clear values", COLOR_LIGHTBLUE);

		GetKey(&toKey);
		
		switch (toKey) {
			case KEY_CTRL_UP:
				if (startTimer > 0) {
					startTimer--;
				}
				break;
			case KEY_CTRL_DOWN:
				if (startTimer < numTimers - 1) {
					startTimer++;
				}
				break;
			case KEY_CTRL_RIGHT:
				mode = (mode + 1) % numModes;
				break;
			case KEY_CTRL_LEFT:
				mode = (mode + numModes - 1) % numModes;
				break;
			case KEY_CTRL_F3:
			{
				ScopeTimer* curTimer = firstTimer;
				while (curTimer) {
					curTimer->cycleCount = 0;
					curTimer->numCounts = 0;
					curTimer = curTimer->nextTimer;
				}
				break;
			}
		}
	// wait for exit key
	} while (toKey != KEY_CTRL_EXIT);

#if TARGET_PRIZM
	// enable TMU2
	REG_TMU_TSTR |= (1 << 2);
#endif
}

#endif