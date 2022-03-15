/*
===============================================================================

 Ptune2 is SH7305 CPG&BSC tuning utility for PRIZM fx-CG10/20  v1.10

 copyright(c)2014,2015,2016 by sentaro21
 e-mail sentaro21@pm.matrix.jp

===============================================================================
*/

#ifdef __cplusplus
extern "C" {
#endif

// All Ptune functions should be avoided on CG50 devices until proven functional!
// setting values
#define PT2_DEFAULT 0			// 59 Mhz, default settings
#define PT2_NOMEMWAIT 1			// 59 Mhz, no mem waits
#define PT2_HALFINC	2			// 97 MHz, 150% speed optimized
#define PT2_DOUBLE 3			// 118 MHz, 200% speed optimized
#define PT2_TRIPLE 4			// 191 MHz, 300% speed optimized (would not recommend usage, you normally shouldn't need this much speed)
#define PT2_UNKNOWN -1			// unknown setting, used for settings that are not the above for Ptune2_GetSetting

typedef union {
	unsigned int regs[8];
	struct {
		unsigned int FLLFRQ;
		unsigned int FRQCR;
		unsigned int CS0BCR;
		unsigned int CS2BCR;
		unsigned int CS0WCR;
		unsigned int CS2WCR;
		unsigned int CS5aBCR;
		unsigned int CS5aWCR;
	};
} PTuneRegs;

//---------------------------------------------------------------------------------------------
void Ptune2_LoadSetting(int setting);	// loads setting based on defines above
int Ptune2_GetSetting();				// returns the current CPU setting as defined above

void Ptune2_LoadBackup();				//  backup regs  -> CPG Register
void Ptune2_SaveBackup();				//  CPG Register -> backup regs

PTuneRegs Ptune2_CurrentRegs();			// returns current cpu clock regs directly
int Ptune2_GetPLLFreq();				// returns PLL frequency, useful for certain timing functions
int Ptune2_GetIFCDiv();					// returns the CPU Clock (I) division, as a bit position (1 = 2, 2 = 4, 3 = 8, etc)
int Ptune2_GetPFCDiv();					// returns the Peripheral Clock (P) division, as a bit position (1 = 2, 2 = 4, 3 = 8, etc)

#ifdef __cplusplus
}
#endif