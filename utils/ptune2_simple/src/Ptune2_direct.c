/*
===============================================================================

  Ptune2 is SH7305 CPG&BSC tuning utility for PRIZM fx-CG10/20  v1.10

 copyright(c)2014,2015,2016 by sentaro21
 e-mail sentaro21@pm.matrix.jp

===============================================================================
*/


#include "Ptune2_direct.h"

#ifdef TARGET_PRIZM

#include "SH7305_CPG_BSC.h"

//---------------------------------------------------------------------------------------------
#define FLF_default  900
#define FLFMAX      2048-1

#define PLL_64x 0b111111 // 
#define PLL_48x 0b101111 //
#define PLL_36x 0b100011 //
#define PLL_32x 0b011111 // 
#define PLL_26x 0b011001 // 
#define PLL_16x 0b001111 // default
#define PLLMAX  64-1

#define DIV_2 0b0000	// 1/2
#define DIV_4 0b0001	// 1/4
#define DIV_8 0b0010	// 1/8
#define DIV16 0b0011	// 1/16
#define DIV32 0b0100	// 1/32
#define DIV64 0b0101	// 1/64

#define WAIT_0 0b0000
#define WAIT_1 0b0001
#define WAIT_2 0b0010	// cs2wcr default 
#define WAIT_3 0b0011	// cs0wcr default 
#define WAIT_4 0b0100
#define WAIT_5 0b0101
#define WAIT_6 0b0110
#define WAIT_8 0b0111
#define WAIT10 0b1000
#define WAIT12 0b1001
#define WAIT14 0b1010
#define WAIT18 0b1011
#define WAIT24 0b1100

#define WAITW0 0b0001
#define WAITW1 0b0010
#define WAITW2 0b0011
#define WAITW3 0b0100
#define WAITW4 0b0101
#define WAITW5 0b0110
#define WAITW6 0b0111

#define FLLFRQ_default	0x00004384
#define FRQCRA_default	0x0F102203

#define CS0BCR_default	0x24920400
#define CS2BCR_default	0x24923400
#define CS3BCR_default  0x24924400
#define CS4BCR_default  0x36DB0400
#define CS5aBCR_default 0x15140400
#define CS5bBCR_default 0x15140400
#define CS6aBCR_default 0x34D30200
#define CS6bBCR_default 0x34D30200

#define CS0WCR_default	0x000001C0
#define CS2WCR_default	0x00000140
#define CS3WCR_default  0x000024D0
#define CS4WCR_default  0x00000540
#define CS5aWCR_default 0x00010240
#define CS5bWCR_default 0x00010240
#define CS6aWCR_default 0x000302C0
#define CS6bWCR_default 0x000302C0

// these mirror the PT2 defines
static const PTuneRegs sets[5] = {
	// PT2_DEFAULT
	{{
		FLLFRQ_default,
		FRQCRA_default,
		CS0BCR_default,
		CS2BCR_default,
		CS0WCR_default,
		CS2WCR_default,
		CS5aBCR_default,
		CS5aWCR_default
	}},
	// PT2_NOMEMWAIT
	{{
		0x00004000 + 900,		// FLL:900		 59MHz
		(PLL_32x << 24) + (DIV_8 << 20) + (DIV16 << 12) + (DIV16 << 8) + DIV32,
		0x04900400,				// IWW:0 IWRRS:0
		0x04903400,				// IWW:0 IWRRS:0
		0x00000140,				// wait:2
		0x000100C0,				// wait:1 WW:0
		CS5aBCR_default,
		CS5aWCR_default
	}},
	// PT2_HALFINC
	{{
		0x00004000 + 900,		// FLL:900		 59MHz
		(PLL_48x << 24) + (DIV_8 << 20) + (DIV16 << 12) + (DIV16 << 8) + DIV32,
		0x14900400,				// IWW:1 IWRRS:0
		0x04903400,				// IWW:0 IWRRS:0
		0x000001C0,				// wait:3
		0x00020140,				// wait:2 WW:1
		CS5aBCR_default,
		CS5aWCR_default
	}},
	// PT2_DOUBLE
	{{
		0x00004000 + 900,		// FLL:900		118MHz
		(PLL_32x << 24) + (DIV_4 << 20) + (DIV_8 << 12) + (DIV_8 << 8) + DIV32,
		0x14900400,				// IWW:1 IWRRS:0
		0x04903400,				// IWW:0 IWRRS:0
		0x000002C0,				// wait:5
		0x000201C0,				// wait:3 WW:1
		CS5aBCR_default,
		CS5aWCR_default
	}},
	// PT2_TRIPLE
	{{
		0x00004000 + 900,		// FLL:900		191MHz
		(PLL_26x << 24) + (DIV_2 << 20) + (DIV_4 << 12) + (DIV_4 << 8) + DIV16,
		0x24900400,			// IWW:2 IWRRS:0
		0x04903400,			// IWW:0 IWRRS:0
		0x000003C0,			// wait:8
		0x000402C0,			// wait:5 WW:3
		CS5aBCR_default,		
		CS5aWCR_default		
	}},
};

//---------------------------------------------------------------------------------------------
static void FRQCR_kick(void) {
    CPG.FRQCRA.BIT.KICK = 1 ;
    while((CPG.LSTATS & 1)!=0);
}
static void change_FRQCRs( unsigned int value) {
    CPG.FRQCRA.LONG = value ;
    FRQCR_kick();
}
static int IsSupported() {
#if TARGET_WINSIM
	return 0;
#else
	return 1;
#endif
}
//---------------------------------------------------------------------------------------------

static void Ptune2_SetWithRegs(const PTuneRegs* regs) {
	if (IsSupported()) {
		BSC.CS0WCR.BIT.WR = WAIT18;
		BSC.CS2WCR.BIT.WR = WAIT18;
		CPG.FLLFRQ.LONG = regs->FLLFRQ;
		change_FRQCRs(	  regs->FRQCR);
		BSC.CS0BCR.LONG = regs->CS0BCR;
		BSC.CS2BCR.LONG = regs->CS2BCR;
		BSC.CS0WCR.LONG = regs->CS0WCR;
		BSC.CS2WCR.LONG = regs->CS2WCR;
		BSC.CS5ABCR.LONG = regs->CS5aBCR;
		BSC.CS5AWCR.LONG = regs->CS5aWCR;
	}
}

PTuneRegs Ptune2_CurrentRegs(){
	PTuneRegs ret;

	if (IsSupported()) {
		ret.FLLFRQ = CPG.FLLFRQ.LONG;
		ret.FRQCR = CPG.FRQCRA.LONG;
		ret.CS0BCR = BSC.CS0BCR.LONG;
		ret.CS2BCR = BSC.CS2BCR.LONG;
		ret.CS0WCR = BSC.CS0WCR.LONG;
		ret.CS2WCR = BSC.CS2WCR.LONG;
		ret.CS5aBCR = BSC.CS5ABCR.LONG;
		ret.CS5aWCR = BSC.CS5AWCR.LONG;
	} else {
		ret = sets[PT2_DEFAULT];
	}

	return ret;
}

void Ptune2_LoadSetting(int setting) {
	switch (setting) {
	case PT2_DEFAULT:
	case PT2_NOMEMWAIT:
	case PT2_HALFINC:
	case PT2_DOUBLE:
	case PT2_TRIPLE:
		Ptune2_SetWithRegs(&sets[setting]);
		break;
	}
}

int Ptune2_GetSetting() {
	int ret = PT2_UNKNOWN;
	int i, set, same;

	PTuneRegs curRegs = Ptune2_CurrentRegs();
	for (i = 0; i < 5; i++) {
		same = 1;
		// the freq register has some trash bits in it on reset:
		unsigned int mask = i == 0 ? 0xFFF0FF0F : 0xFFFFFFFF;

		for (set = 0; set < 8; set++) {
			if ((curRegs.regs[set] & mask) != (sets[i].regs[set] & mask)) {
				// special exception for PTune2-less CPU reset:
				same = 0;
				break;
			}
		}

		if (same) {
			ret = i;
			break;
		}
	}

	return ret;
}

// back up regs start with cpu defaults
static PTuneRegs backup = {{
	FLLFRQ_default,
	FRQCRA_default,
	CS0BCR_default,
	CS2BCR_default,
	CS0WCR_default,
	CS2WCR_default,
	CS5aBCR_default,
	CS5aWCR_default
}};

void Ptune2_LoadBackup() {
	Ptune2_SetWithRegs(&backup);
}

void Ptune2_SaveBackup() {
	backup = Ptune2_CurrentRegs();
}

int Ptune2_GetPLLFreq() {
	return (CPG.FRQCRA.LONG >> 24) + 1;
}

int Ptune2_GetIFCDiv() {
	return ((CPG.FRQCRA.LONG >> 20) & 0xF) + 1;
}

int Ptune2_GetPFCDiv() {
	return ((CPG.FRQCRA.LONG & 0xF) + 1);
}
//---------------------------------------------------------------------------------------------


#else

// stubbed out functions with valid getters
static int CurrentPtune2Setting = PT2_DEFAULT;
static int BackupPtune2Setting = PT2_DEFAULT;
static PTuneRegs BadRegs;

PTuneRegs Ptune2_CurrentRegs() {
	return BadRegs;
}

void Ptune2_LoadSetting(int setting) {
	CurrentPtune2Setting = setting;
}

int Ptune2_GetSetting() {
	return CurrentPtune2Setting;
}

void Ptune2_LoadBackup() {
	CurrentPtune2Setting = BackupPtune2Setting;
}

void Ptune2_SaveBackup() {
	BackupPtune2Setting = CurrentPtune2Setting;
}

int Ptune2_GetPLLFreq() {
	return 1;
}

int Ptune2_GetIFCDiv() {
	return 1;
}

int Ptune2_GetPFCDiv() {
	return 1;
}

#endif