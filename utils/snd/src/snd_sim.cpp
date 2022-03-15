#if TARGET_WINSIM

#include "snd.h"
#include "snd_private.h"

#include <Windows.h>
#include <assert.h>

bool bSimulate1Bit = false;

HWAVEOUT device;

#define USE_VOLTAGE_POTENTIAL 1
#define SUPER_SAMPLE 8
#define PLAYBACK_RATE SOUND_RATE * SUPER_SAMPLE

#define RENDER_BUFFER_SIZE SOUND_RATE * 4 / 256
#define PLAY_BUFFER_SIZE PLAYBACK_RATE * 4 / 256
#define NUM_BUFFERS 4

static int corr0 = 0;
static short buffer[NUM_BUFFERS][PLAY_BUFFER_SIZE];
static int corr1 = 0;
static WAVEHDR headers[NUM_BUFFERS];
static int curBuffer = 0;
static bool sndShutdown = false;

static void prepareBuffer(HWAVEOUT hwo, int bufferNum) {
	static int renderBuffer[RENDER_BUFFER_SIZE];
	headers[bufferNum].dwFlags &= ~WHDR_DONE;

	// buffer is 4 frames
	sndFrame(&renderBuffer[0], RENDER_BUFFER_SIZE);

	if (bSimulate1Bit) {
		// render to 1 bit BTC
		unsigned char btcBuffer[RENDER_BUFFER_SIZE * 2];
		ConvertSamples(renderBuffer, btcBuffer, RENDER_BUFFER_SIZE);

		unsigned char* btc = btcBuffer;
		short* target = buffer[bufferNum];

#if defined(USE_VOLTAGE_POTENTIAL)
		static float curVoltage = 16384;
		static float stepSizeUp = 0.002f;
		static float stepSizeDown = -0.002f;
		static float minStep = 0.25f;
		for (int i = 0; i < RENDER_BUFFER_SIZE * 2; i++) {
			// each 16-bit sample gets rendered into 8 playback samples, so 8 -> 4
			unsigned char bits = *(btc++);
			for (int j = 0; j < 4; j++) {
				curVoltage += (bits & 0x01) ? max(1.0f - curVoltage, minStep) * stepSizeUp : max(curVoltage, minStep) * stepSizeDown;
				curVoltage += (bits & 0x02) ? max(1.0f - curVoltage, minStep) * stepSizeUp : max(curVoltage, minStep) * stepSizeDown;
				bits >>= 2;
				if (curVoltage < 0) curVoltage = 0;
				if (curVoltage > 1) curVoltage = 1;
				*(target++) = short((curVoltage * 2.0f - 1.0f) * 32000);
			}
		}
#else 
		static int curVoltage = 0;
		static short stepSizeUp = 1200;
		static short stepSizeDown = -500;
		for (int i = 0; i < RENDER_BUFFER_SIZE * 2; i++) {
			// each 16-bit sample gets rendered into 8 playback samples, so 8 -> 4
			unsigned char bits = *(btc++);
			for (int j = 0; j < 4; j++) {
				curVoltage += (bits & 0x01) ? stepSizeUp : stepSizeDown;
				curVoltage += (bits & 0x02) ? stepSizeUp : stepSizeDown;
				bits >>= 2;
				if (curVoltage < -32767) curVoltage = -32767;
				if (curVoltage > 32767) curVoltage = 32767;
				*(target++) = curVoltage;
			}
		}
#endif
	} else {
		int last = buffer[bufferNum][PLAY_BUFFER_SIZE - 1];

		for (int i = 0 ; i < PLAY_BUFFER_SIZE; i++) {
			int newSample = renderBuffer[i / SUPER_SAMPLE];
			buffer[bufferNum][i] = (newSample + last) / 2;
			last = newSample;
		}
	}
}

static void CALLBACK sndProc(
	HWAVEOUT  hwo,
	UINT      uMsg,
	DWORD_PTR dwInstance,
	DWORD_PTR dwParam1,
	DWORD_PTR dwParam2
) {
}

// initializes the platform sound system, called when emulation begins
bool sndInit() {
	sndShutdown = false;
	WAVEFORMATEX format;
	format.wFormatTag = WAVE_FORMAT_PCM;
	format.nChannels = 1;
	format.nSamplesPerSec = PLAYBACK_RATE;
	format.nAvgBytesPerSec = PLAYBACK_RATE;
	format.nBlockAlign = 2;
	format.wBitsPerSample = 16;
	format.cbSize = 0;
	
	MMRESULT result = waveOutOpen(
		&device,
		WAVE_MAPPER,
		&format,
		(DWORD_PTR) sndProc,
		NULL,
		CALLBACK_FUNCTION);

	if (result != 0)
		return false;

	memset(&headers[0], 0, sizeof(headers));
	for (int i = 0; i < NUM_BUFFERS; i++) {
		headers[i].lpData = (LPSTR)&buffer[i][0];
		headers[i].dwBufferLength = PLAY_BUFFER_SIZE * 2;

		MMRESULT result = waveOutPrepareHeader(device, &headers[i], sizeof(headers[i]));
		assert(result == 0);

		headers[i].dwFlags |= WHDR_DONE;
	}

	// write all 8 
	curBuffer = 0;

	return true;
}

void sndUpdate() {
	if (headers[curBuffer].dwFlags & WHDR_DONE) {
		prepareBuffer(device, curBuffer);
		waveOutWrite(device, &headers[curBuffer], sizeof(headers[0]));
		curBuffer = (curBuffer + 1) % NUM_BUFFERS;
	}
}

// cleans up the platform sound system, called when emulation ends
void sndCleanup() {
	sndShutdown = true;

	waveOutReset(device);

	for (int i = 0; i < NUM_BUFFERS; i++) {
		MMRESULT result = waveOutUnprepareHeader(device, &headers[i], sizeof(headers[0]));
		assert(result == 0);
	}

	waveOutClose(device);
}

#endif