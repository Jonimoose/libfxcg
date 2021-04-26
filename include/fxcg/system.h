#ifdef __cplusplus
extern "C" {
#endif

// This adds in syscalls for interfacing with the OS and hardware.

void SetAutoPowerOffTime(int durationInMinutes);
int GetAutoPowerOffTime(); //returns duration in minutes

void SetBacklightDuration(char durationInHalfMinutes);
char GetBacklightDuration(); //returns duration in half-minutes

void SetBatteryType(int type);
int GetBatteryType(void);

int GetMainBatteryVoltage(int one); //parameter should be 1
void PowerOff(int displayLogo);
void Restart();
void SpecialMatrixcodeProcessing(int*col, int*row);
void TestMode(int);
void*GetStackPtr(void);

void SetSetupSetting(unsigned int SystemParameterNo, unsigned char SystemParameterValue);
unsigned char GetSetupSetting(unsigned int SystemParameterNo);

int Timer_Install(int InternalTimerID, void (*hander)(void), int elapse);
int Timer_Deinstall(int InternalTimerID);
int Timer_Start(int InternalTimerID);
int Timer_Stop(int InternalTimerID);

void TakeScreenshot(void);
void TakeScreenshot2(void); //seems to be the same as the one above

void DisplayMainMenu(void);

//Hold program execution:
void OS_InnerWait_ms(int);

void CMT_Delay_100micros(int); //does CMT stand for Composable Memory Transactions? Couldn't find documentation on this
void CMT_Delay_micros(int); //   nor on this (gbl08ma)

void SetQuitHandler(void (*)()); // sets callback to be run when user exits through the main menu from one app to another. eActivity uses this in the "Save file?" dialog

void Alpha_SetData( char VarName, void* Src );
void Alpha_GetData( char VarName, void* Dest );

int CLIP_Store( unsigned char*buffer, int length ); // stores buffer of length length in the system clipboard.

int MB_ElementCount(char* buf); // like strlen but for the graphical length of multibyte strings
int MB_IsLead(char c); // returns 1 if the character is the first byte of a multibyte char

int GlibGetOSVersionInfo(char *major, char *minor, short int *c, short int *d); 

#ifdef __cplusplus
}
#endif
