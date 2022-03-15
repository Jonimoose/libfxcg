
#if !TARGET_WINSIM

#include "snd.h"
#include "snd_private.h"
#include "..\ptune2_simple\Ptune2_direct.h"
#include "fxcg/serial.h"
#include "fxcg/display.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#ifndef max
#define max(a,b)            (((a) > (b)) ? (a) : (b))
#endif

#ifndef min
#define min(a,b)            (((a) < (b)) ? (a) : (b))
#endif

struct st_scif0 {                                      /* struct SCIF0 */
       union {                                         /* SCSMR        */
             unsigned short WORD;                      /*  Word Access */
             struct {                                  /*  Bit Access  */
                    unsigned short :8;                 /*              */
                    unsigned short CA:1;               /*   CA         */
                    unsigned short CHR:1;              /*   CHR        */
                    unsigned short PE:1;               /*   PE         */
                    unsigned short OE:1;               /*   OE         */
                    unsigned short STOP:1;             /*   STOP       */
                    unsigned short :1;                 /*              */
                    unsigned short CKS:2;              /*   CKS        */
                    } BIT;                             /*              */
             } SCSMR;                                  /*              */
       unsigned char wk0[2];                           /*              */
       unsigned char SCBRR;                            /* SCBRR        */
       unsigned char wk1[3];                           /*              */
       union {                                         /* SCSCR        */
             unsigned short WORD;                      /*  Word Access */
             struct {                                  /*  Bit Access  */
                    unsigned short :8;                 /*              */
                    unsigned short TIE:1;              /*   TIE        */
                    unsigned short RIE:1;              /*   RIE        */
                    unsigned short TE:1;               /*   TE         */
                    unsigned short RE:1;               /*   RE         */
                    unsigned short REIE:1;             /*   REIE       */
                    unsigned short :1;                 /*              */
                    unsigned short CKE:2;              /*   CKE        */
                    } BIT;                             /*              */
             } SCSCR;                                  /*              */
       unsigned char wk2[2];                           /*              */
       unsigned char SCFTDR;                           /* SCFTDR       */
       unsigned char wk3[3];                           /*              */
       union {                                         /* SCFSR        */
             unsigned short WORD;                      /*  Word Access */
             struct {                                  /*  Bit Access  */
                    unsigned short PERC:4;             /*   PERC       */
                    unsigned short FERC:4;             /*   FERC       */
                    unsigned short ER:1;               /*   ER         */
                    unsigned short TEND:1;             /*   TEND       */
                    unsigned short TDFE:1;             /*   TDFE       */
                    unsigned short BRK:1;              /*   BRK        */
                    unsigned short FER:1;              /*   FER        */
                    unsigned short PER:1;              /*   PER        */
                    unsigned short RDF:1;              /*   RDF        */
                    unsigned short DR:1;               /*   DR         */
                    } BIT;                             /*              */
             } SCFSR;                                  /*              */
       unsigned char wk4[2];                           /*              */
       unsigned char SCFRDR;                           /* SCFRDR       */
       unsigned char wk5[3];                           /*              */
       union {                                         /* SCFCR        */
             unsigned short WORD;                      /*  Word Access */
             struct {                                  /*  Bit Access  */
                    unsigned short :5;                 /*              */
                    unsigned short RSTRG:3;            /*   RSTRG      */
                    unsigned short RTRG:2;             /*   RTRG       */
                    unsigned short TTRG:2;             /*   TTRG       */
                    unsigned short MCE:1;              /*   MCE        */
                    unsigned short TFRST:1;            /*   TFRST      */
                    unsigned short RFRST:1;            /*   RFRST      */
                    unsigned short LOOP:1;             /*   LOOP       */
                    } BIT;                             /*              */
             } SCFCR;                                  /*              */
       unsigned char wk6[2];                           /*              */
       union {                                         /* SCFDR        */
             unsigned short WORD;                      /*  Word Access */
             struct {                                  /*  Bit Access  */
                    unsigned short :3;                 /*              */
                    unsigned short TFDC:5;             /*   TFDC       */
                    unsigned short :3;                 /*              */
                    unsigned short RFDC:5;             /*   RFDC       */
                    } BIT;                             /*              */
             } SCFDR;                                  /*              */
       unsigned char wk7[6];                           /*              */
       union {                                         /* SCLSR        */
             unsigned short WORD;                      /*  Word Access */
             struct {                                  /*  Bit Access  */
                    unsigned short :15;                /*              */
                    unsigned short ORER:1;             /*   ORER       */
                    } BIT;                             /*              */
             } SCLSR;                                  /*              */
};           

#define SCIF2 (*(volatile struct st_scif0 *)0xA4410000)/* SCIF0 Address */

#define BUFF_SIZE (SOUND_RATE / 64)

unsigned int lastSoundCounter;

static int curSoundBuffer[BUFF_SIZE];
static int sampleNum = 0;

// initializes the platform sound system, called when emulation begins. Returns false on error
bool sndInit() {
	unsigned char settings[5] = { 0,9,0,0,0 };//115200,1xstop bits
	if (Serial_Open(&settings[0]))
	{
		return false;
	}

	SCIF2.SCSMR.BIT.CKS = 0;	//1/1
	SCIF2.SCSMR.BIT.CA = 1;		//Synchronous mode is 16 times faster

	sampleNum = 0;
	lastSoundCounter = 0x7FFFFFFF;

	// tune bitrate based on clock speed
	if (getDeviceType() == DT_CG50) {
		SCIF2.SCBRR = 32;
	} else {
		switch (Ptune2_GetSetting()) {
			case PT2_DEFAULT:
			case PT2_NOMEMWAIT:
				SCIF2.SCBRR = 16;
				break;
			case PT2_HALFINC:
				SCIF2.SCBRR = 24;
				break;
			case PT2_DOUBLE:
				SCIF2.SCBRR = 32;
				break;
		}
	}

	return true;
}

static void feed() {
	sndFrame(curSoundBuffer, BUFF_SIZE);
	sampleNum = 0;
}

// platform update from emulator for sound system, called 8 times per frame (should be enough!)
void sndUpdate() {
	{
		int toAdd = Serial_PollTX() & 0x1FE;

		// perform this update every 1/360th of a second or so
		lastSoundCounter = REG_TMU_TCNT_1 - 40;

		if (toAdd > 1)
		{
			unsigned char writeBuffer[256];
			int samplesRemaining = toAdd / 2;
			unsigned char* curWrite = writeBuffer;
			int samplesWritten = samplesRemaining;
			while (samplesRemaining) {
				int samplesInBuffer = BUFF_SIZE - sampleNum;
				if (samplesInBuffer < samplesRemaining) {
					ConvertSamples(&curSoundBuffer[sampleNum], curWrite, samplesInBuffer);
					curWrite += samplesInBuffer * 2;
					samplesRemaining -= samplesInBuffer;

					feed();
				} else {
					ConvertSamples(&curSoundBuffer[sampleNum], curWrite, samplesRemaining);
					curWrite += samplesRemaining * 2;
					sampleNum += samplesRemaining;
					samplesRemaining = 0;
				}
			} while (samplesRemaining);

			Serial_Write(writeBuffer, samplesWritten * 2);
		}
	}
}

// cleans up the platform sound system, called when emulation ends
void sndCleanup() {
	Serial_Close(1);
}

#endif