
// sound code relevant to both platforms. Specifically code meant to 
// convert sound sampling to 1-bit sound (enabled by default in winsim, can be
// disabled via bSimulate1Bit = false)

#include "snd.h"

// volume has to be modulated in a number of ways do to reality of 1-bit audio:
const int numVolumes = 7;
const int numSoundSettings = 4;

// vol #
// bit shift used to determine step size of voltage. Smaller = more volts, higher = less accuracy
// bit shift used directly on sample data, which pushes the overall voltage lower
// final additional masking done to output bits. Can be used to force down bits while maintaining voltage simulation
const int perVolumeSettings[numVolumes][numSoundSettings] = {
	{ 0,8, 4,0xCC },
	{ 1,9, 4,0xCC },
	{ 2,9, 3,0xCC },
	{ 3,9, 3,0xFF },
	{ 4,9, 2,0xFF },
	{ 5,9, 1,0xFF },
	{ 6,10,1,0xFF },
};
static int sndSettings[numSoundSettings] = { 3, 9, 3, 0xFF };

static int curVoltage = 8192;

inline unsigned char BTC(int target) {
	unsigned char btc = 0;
	const int shift = sndSettings[1];

	for (int bit = 0; bit < 8; bit++) {
		btc <<= 1;

		int dist = (32768 - curVoltage) >> shift;
		int upVoltage = curVoltage + dist;

		dist = curVoltage >> shift;
		int downVoltage = curVoltage - dist;

		// an early out when voltage is reached that reduces distortion:
		if ((bit & 1) == 0 && upVoltage > target && downVoltage < target) {
			// just maintain position with 1/0 flipping
			unsigned int useBit = (upVoltage - target < target - downVoltage) ? 0x55 : 0xAA;
			btc |= (useBit & 1);
			bit++;
			while (bit < 8) {
				useBit >>= 1;
				btc <<= 1;
				btc |= (useBit & 1);
				bit++;
			}
			break;
		}

		int disthigh = upVoltage - target;
		if (disthigh < 0) disthigh = -disthigh;
		int distlow = downVoltage - target;
		if (distlow < 0) distlow = -distlow;

		if (disthigh < distlow) {
			btc |= 1;
			curVoltage = upVoltage;
		} else {
			curVoltage = downVoltage;
		}
	}

	return btc & sndSettings[3];
}

// Convert samples into 16-bit chunks of 1-bit BTC audio
void ConvertSamples(int* samples, unsigned char* convertedBuffer, int numSamples) {
	const int bitShift = sndSettings[2];
	const int voltageOffset = 8192; //  16384 - (8192 >> bitShift);
	for (int i = 0; i < numSamples; i++) {
		int sampleTarget = (*(samples++) >> bitShift) + voltageOffset;

		*(convertedBuffer++) = BTC((curVoltage + sampleTarget) / 2);
		*(convertedBuffer++) = BTC(sampleTarget);
	}
}

void updateSettings() {
	const int* settings = perVolumeSettings[sndSettings[0]];
	for (int i = 0; i < numSoundSettings; i++) {
		sndSettings[i] = settings[i];
	}
}

void sndVolumeUp() {
	if (sndSettings[0] < numVolumes - 1) {
		sndSettings[0]++;

		updateSettings();
	}
}

void sndVolumeDown() {
	if (sndSettings[0] > 0) {
		sndSettings[0]--;

		updateSettings();
	}
}