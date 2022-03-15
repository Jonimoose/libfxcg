#pragma once

/*
 * To Use:
 * Call Initialize and Shutdown on the ScopeTimer class, and then use
 * TIME_SCOPE() and TIME_SCOPE_NAMED() in your scopes or functions you want
 * to time with TMU2 or in winsim using the high frequency clock. IF you wish
 * to time frames, make sure you use the ReportFrame() call.
 *
 * View the times by enabling a key to call the DisplayTimes function.
 */

#if TARGET_PRIZM
#include "tmu.h"
#define GetCycles() REG_TMU_TCNT_2
#else
#include <windows.h>
extern LONGLONG ScopeTimer_Start;
inline unsigned int GetCycles() {
	LARGE_INTEGER result;
	if (QueryPerformanceCounter(&result) != 0) {
		return (unsigned int) (ScopeTimer_Start - result.QuadPart) >> 2;
	}

	return 0;
}
#endif

#if DEBUG
struct ScopeTimer {
	unsigned int cycleCount;
	unsigned int numCounts;

	const char* funcName;
	int line;
	static int fpsValue;

	static void FPSToBuffer(char* buffer);

	ScopeTimer* nextTimer;

	ScopeTimer(const char* withFunctionName, int withLine);
	void Register(const char* withFunctionName);

	inline void AddTime(unsigned int cycles) {
		cycleCount += cycles;
		numCounts++;
	}

	static ScopeTimer* firstTimer;
	static char debugString[128];			// per application debug string (placed on last row), usually FPS or similar
	
	// call to initialize use of scope timer system
	static void InitSystem();

	// call when a frame is completed
	static void ReportFrame();

	// call when the user wants to see the scope timer results
	static void DisplayTimes();

	// call to shut down the system
	static void Shutdown();
};

struct TimedInstance {
	ScopeTimer* myTimer;
	unsigned int start;

	inline TimedInstance(ScopeTimer* withTimer) : myTimer(withTimer), start(GetCycles()) {
	}

	inline void Switch(ScopeTimer* newTimer) {
		int elapsed = (int)(start - GetCycles());
		if (elapsed >= 0) {
			myTimer->AddTime(elapsed);
		}
		myTimer = newTimer;
		start = GetCycles();
	}

	inline ~TimedInstance() {
		int elapsed = (int)(start - GetCycles());

		if (elapsed >= 0) {
			myTimer->AddTime(elapsed);
		}
	}
};

#define TIME_SCOPE() static ScopeTimer __timer(__FUNCTION__, __LINE__); TimedInstance __timeMe(&__timer);
#define TIME_SCOPE_NAMED(Name) static ScopeTimer __timer(#Name, __LINE__); TimedInstance __timeMe(&__timer);

// in order to use a scope in a templated function without dupes it needs to be static declared and used instead
#define DECLARE_EXT_TIME_SCOPE(Name) ScopeTimer __timer ## Name ( # Name, __LINE__);
#define EXT_TIME_SCOPE(Name) TimedInstance __timeMe(&__timer##Name);
#define REGISTER_EXT_TIME_SCOPE(Name) extern ScopeTimer __timer ## Name ; __timer ## Name .Register( # Name );

/* Switches a scope to a new scope mid function. Useful for timing different chunks of code in the same function.
*  For example, the following function would time DoStuff and Player2Stuff() separately:
* 
*  DECLARE_EXT_TIME_SCOPE(RunLogic)
*  DECLARE_EXT_TIME_SCOPE(RunLogic_Player2)
*  void MyLogicFn() {
*     EXT_TIME_SCOPE(RunLogic)
*     DoStuff();
*     EXT_TIME_SCOPE_SWITCH(RunLogic_Player2)
*     Player2Stuff()
*  }
*/
#define EXT_TIME_SCOPE_SWITCH(Name) __timeMe.Switch(&__timer##Name);


#else
struct ScopeTimer {
	static void InitSystem() {}
	static void DisplayTimes() {}
	static void Shutdown() {}
	static void ReportFrame() {}
};
#endif

#ifndef TIME_SCOPE
#define TIME_SCOPE() 
#define TIME_SCOPE_NAMED(Name) 
#endif