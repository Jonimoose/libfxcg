#pragma once

/*
	To implement sound in your project, include this utility and then make sure to implement this function:
	
	void sndFrame(int* buffer, int length);

	Which expects the int buffer provided to be filled with length samples. Sample values should range from
	0 - 16384. If you have a full range from -MAX to +MAX, then 0 should map to 8192 in the returned samples.

	The frequency of the samples should match the define below.

	To use the sound system, call sndInit. Then ~200 Hz the call condSoundUpdate(). This is a low cost
	function except for when sound needs to be fed into the system, then speed entirely depends on your
	implementation of sndFrame as mentioned above. Additionally, there is some cost to using the sound
	interrupts the OS uses to keep the sound buffer filled.

	Once finished, call sndCleanup. Do not call sndUpdate directly, using condSndUpdate is faster.

	For your UI, sndVolumeUp and sndVolumeDown can be called to change sound volume on devices with
	about 5 available levels.
 */

#include "fxcg/tmu.h"

// sound baud rate, based on Prizm modem speed
#define SOUND_RATE 28800

// initializes the platform sound system. Returns false on error
bool sndInit();

// platform update from emulator for sound system, please call approx 400 times per second (or use condSoundUpdate() below quite a bit)
void sndUpdate();

// cleans up the platform sound system, called when emulation ends
void sndCleanup();

// volume controls (called per 8 frames if button pressed, if platform implemented)
void sndVolumeUp();
void sndVolumeDown();

// called from the platform sound system to fill a 1/64 second buffer (range 0-15750) based on current sound values
void sndFrame(int* buffer, int length);

// must be defined by using project to tell sound system if sound is enabled, e.g. emulator.settings.sound in Prizoop
extern bool bSoundEnabled;

#if TARGET_PRIZM
extern unsigned int lastSoundCounter;
#define condSoundUpdate() if (bSoundEnabled && lastSoundCounter > REG_TMU_TCNT_1) sndUpdate()
#else
#define condSoundUpdate() if (bSoundEnabled) sndUpdate()
#endif